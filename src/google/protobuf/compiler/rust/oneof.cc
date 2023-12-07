// Protocol Buffers - Google's data interchange format
// Copyright 2023 Google LLC.  All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#include "google/protobuf/compiler/rust/oneof.h"

#include <string>

#include "absl/strings/string_view.h"
#include "google/protobuf/compiler/cpp/helpers.h"
#include "google/protobuf/compiler/rust/context.h"
#include "google/protobuf/compiler/rust/naming.h"
#include "google/protobuf/descriptor.h"

namespace google {
namespace protobuf {
namespace compiler {
namespace rust {

// We emit three Rust enums:
// -  An enum acting as a tagged union that has each case holds a View<> of
//    each of the cases. Named as the one_of name in CamelCase.
// -  An enum acting as a tagged union that has each case holds a Mut<> of
//    each of the cases. Named as one_of name in CamelCase with "Mut" appended.
//    [TODO: Mut not implemented yet].
// -  A simple enum whose cases have int values matching the cpp or upb's
//    case enum. Named as the one_of camelcase with "Case" appended.
// All three contain cases matching the fields in the oneof CamelCased.
// The first and second are exposed in the API, the third is internal and
// used for interop with the Kernels in the generation of the other two.
//
// Example:
// For this oneof:
// message SomeMsg {
//   oneof some_oneof {
//     int32 field_a = 7;
//     uint32 field_b = 9;
//   }
// }
//
// This will emit as the exposed API:
// pub mod SomeMsg_ {
//   // The 'view' struct (no suffix on the name)
//   pub enum SomeOneof<'msg> {
//     FieldA(i32) = 7,
//     FieldB(u32) = 9,
//     not_set(std::marker::PhantomData<&'msg ()>)
//   }
//   pub enum SomeOneofMut<'msg> {
//     FieldA(Mut<'msg, i32>) = 7,
//     FieldB(Mut<'msg, u32>) = 9,
//     not_set(std::marker::PhantomData<&'msg ()>)
//   }
// }
// impl SomeMsg {
//   pub fn some_oneof() -> SomeOneof {...}
//   pub fn some_oneof_mut() -> SomeOneofMut {...}

// }
//
// An additional "Case" enum which just reflects the corresponding slot numbers
// is emitted for usage with the FFI (exactly matching the Case struct that both
// cpp and upb generate).
//
// #[repr(C)] pub(super) enum SomeOneofCase {
//   FieldA = 7,
//   FieldB = 9,
//   not_set = 0
// }

namespace {
std::string ToCamelCase(absl::string_view name) {
  return cpp::UnderscoresToCamelCase(name, /* upper initial letter */ true);
}

std::string oneofViewEnumRsName(const OneofDescriptor& desc) {
  return ToCamelCase(desc.name());
}

std::string oneofMutEnumRsName(const OneofDescriptor& desc) {
  return ToCamelCase(desc.name()) + "Mut";
}

std::string oneofCaseEnumName(const OneofDescriptor& desc) {
  // Note: This is the name used for the cpp Case enum, we use it for both
  // the Rust Case enum as well as for the cpp case enum in the cpp thunk.
  return ToCamelCase(desc.name()) + "Case";
}

bool isSupportedOneofField(const FieldDescriptor& desc) {
  // TODO: Fields with a ctype set not supported in v0.6 api.
  return !desc.options().has_ctype() &&
         desc.type() != FieldDescriptor::TYPE_ENUM &&
         desc.type() != FieldDescriptor::TYPE_GROUP;
}

}  // namespace

void GenerateOneofDefinition(Context<OneofDescriptor> oneof) {
  const auto& desc = oneof.desc();

  oneof.Emit(
      {{"view_enum_name", oneofViewEnumRsName(desc)},
       {"mut_enum_name", oneofMutEnumRsName(desc)},
       {"view_fields",
        [&] {
          for (int i = 0; i < desc.field_count(); ++i) {
            const auto& field = *desc.field(i);
            if (!isSupportedOneofField(field)) {
              continue;
            }
            oneof.Emit({{"name", ToCamelCase(field.name())},
                        {"view_type", ViewTypeName(oneof.WithDesc(field),
                                                   /* lifetime */ "msg")},
                        {"number", std::to_string(field.number())}},
                       R"rs($name$($view_type$) = $number$,
                )rs");
          }
        }},
       {"mut_fields",
        [&] {
          for (int i = 0; i < desc.field_count(); ++i) {
            const auto& field = *desc.field(i);
            if (!isSupportedOneofField(field)) {
              continue;
            }
            oneof.Emit({{"name", ToCamelCase(field.name())},
                        {"mut_type", MutTypeName(oneof.WithDesc(field),
                                                 /* lifetime */ "msg")},
                        {"number", std::to_string(field.number())}},
                       R"rs($name$($mut_type$) = $number$,
                )rs");
          }
        }}},
      // TODO: Revisit if isize is the optimal repr for this enum.
      // TODO: not_set currently has phantom data just to avoid the
      // lifetime on the enum breaking compilation if there are zero supported
      // fields on it (e.g. if the oneof only has Messages inside).
      R"rs(
      #[non_exhaustive]
      #[derive(Debug)]
      #[allow(dead_code)]
      #[repr(isize)]
      pub enum $view_enum_name$<'msg> {
        $view_fields$

        #[allow(non_camel_case_types)]
        not_set(std::marker::PhantomData<&'msg ()>) = 0
      }

      #[non_exhaustive]
      #[derive(Debug)]
      #[allow(dead_code)]
      #[repr(isize)]
      pub enum $mut_enum_name$<'msg> {
        $mut_fields$

        #[allow(non_camel_case_types)]
        not_set(std::marker::PhantomData<&'msg ()>) = 0
      }

      )rs");

  // Note: This enum is used as the Thunk return type for getting which case is
  // used: it exactly matches the generate case enum that both cpp and upb use.
  oneof.Emit({{"case_enum_name", oneofCaseEnumName(desc)},
              {"cases",
               [&] {
                 for (int i = 0; i < desc.field_count(); ++i) {
                   const auto& field = desc.field(i);
                   oneof.Emit({{"name", ToCamelCase(field->name())},
                               {"number", std::to_string(field->number())}},
                              R"rs($name$ = $number$,
                )rs");
                 }
               }}},
             R"rs(
      #[repr(C)]
      #[derive(Debug, Copy, Clone, PartialEq, Eq)]
      pub(super) enum $case_enum_name$ {
        $cases$

        #[allow(non_camel_case_types)]
        not_set = 0
      }

      )rs");
}

void GenerateOneofAccessors(Context<OneofDescriptor> oneof) {
  const auto& desc = oneof.desc();

  oneof.Emit(
      {{"oneof_name", desc.name()},
       {"view_enum_name", oneofViewEnumRsName(desc)},
       {"mut_enum_name", oneofMutEnumRsName(desc)},
       {"case_enum_name", oneofCaseEnumName(desc)},
       {"view_cases",
        [&] {
          for (int i = 0; i < desc.field_count(); ++i) {
            const auto& field = *desc.field(i);
            if (!isSupportedOneofField(field)) {
              continue;
            }
            oneof.Emit(
                {
                    {"case", ToCamelCase(field.name())},
                    {"rs_getter", field.name()},
                },
                R"rs($Msg$_::$case_enum_name$::$case$ => $Msg$_::$view_enum_name$::$case$(self.$rs_getter$()),
                )rs");
          }
        }},
       {"mut_cases",
        [&] {
          for (int i = 0; i < desc.field_count(); ++i) {
            const auto& field = *desc.field(i);
            if (!isSupportedOneofField(field)) {
              continue;
            }
            oneof.Emit(
                {{"case", ToCamelCase(field.name())},
                 {"rs_mut_getter", field.name() + "_mut"},

                 // Any extra behavior needed to map the mut getter into the
                 // unwrapped Mut<>. Right now Message's _mut already returns
                 // the Mut directly, but for scalars the accessor will return
                 // an Optional which we then grab the mut by doing
                 // .try_into_mut().unwrap().
                 //
                 // Note that this unwrap() is safe because the flow is:
                 // 1) Find out which oneof field is already set (if any)
                 // 2) If a field is set, call the corresponding field's _mut()
                 // and wrap the result in the SomeOneofMut enum.
                 // The unwrap() will only ever panic if the which oneof enum
                 // disagrees with the corresponding field presence which.
                 // TODO: If the message _mut accessor returns
                 // Optional<> then this conditional behavior should be removed.
                 {"into_mut_transform",
                  field.type() == FieldDescriptor::TYPE_MESSAGE
                      ? ""
                      : ".try_into_mut().unwrap()"}},
                R"rs($Msg$_::$case_enum_name$::$case$ =>
               $Msg$_::$mut_enum_name$::$case$(self.$rs_mut_getter$()$into_mut_transform$), )rs");
          }
        }},
       {"case_thunk", Thunk(oneof, "case")}},
      R"rs(
        pub fn r#$oneof_name$(&self) -> $Msg$_::$view_enum_name$ {
          match unsafe { $case_thunk$(self.inner.msg) } {
            $view_cases$
            _ => $Msg$_::$view_enum_name$::not_set(std::marker::PhantomData)
          }
        }

        pub fn r#$oneof_name$_mut(&mut self) -> $Msg$_::$mut_enum_name$ {
          match unsafe { $case_thunk$(self.inner.msg) } {
            $mut_cases$
            _ => $Msg$_::$mut_enum_name$::not_set(std::marker::PhantomData)
          }
        }

      )rs");
}

void GenerateOneofExternC(Context<OneofDescriptor> oneof) {
  const auto& desc = oneof.desc();
  oneof.Emit(
      {
          {"case_enum_rs_name", oneofCaseEnumName(desc)},
          {"case_thunk", Thunk(oneof, "case")},
      },
      R"rs(
        fn $case_thunk$(raw_msg: $pbi$::RawMessage) -> $Msg$_::$case_enum_rs_name$;
      )rs");
}

void GenerateOneofThunkCc(Context<OneofDescriptor> oneof) {
  const auto& desc = oneof.desc();
  oneof.Emit(
      {
          {"oneof_name", desc.name()},
          {"case_enum_name", oneofCaseEnumName(desc)},
          {"case_thunk", Thunk(oneof, "case")},
          {"QualifiedMsg", cpp::QualifiedClassName(desc.containing_type())},
      },
      R"cc(
        $QualifiedMsg$::$case_enum_name$ $case_thunk$($QualifiedMsg$* msg) {
          return msg->$oneof_name$_case();
        }
      )cc");
}

}  // namespace rust
}  // namespace compiler
}  // namespace protobuf
}  // namespace google
