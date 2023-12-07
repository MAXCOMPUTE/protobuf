// Protocol Buffers - Google's data interchange format
// Copyright 2023 Google LLC.  All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#include "google/protobuf/compiler/rust/naming.h"

#include <string>
#include <vector>

#include "absl/log/absl_log.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_format.h"
#include "absl/strings/str_join.h"
#include "absl/strings/str_replace.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"
#include "absl/strings/substitute.h"
#include "google/protobuf/compiler/code_generator.h"
#include "google/protobuf/compiler/rust/context.h"
#include "google/protobuf/descriptor.h"

namespace google {
namespace protobuf {
namespace compiler {
namespace rust {
namespace {
std::string GetUnderscoreDelimitedFullName(Context<Descriptor> msg) {
  std::string result = msg.desc().full_name();
  absl::StrReplaceAll({{".", "_"}}, &result);
  return result;
}
}  // namespace

std::string GetCrateName(Context<FileDescriptor> dep) {
  absl::string_view path = dep.desc().name();
  auto basename = path.substr(path.rfind('/') + 1);
  return absl::StrReplaceAll(basename, {{".", "_"}, {"-", "_"}});
}

std::string GetRsFile(Context<FileDescriptor> file) {
  auto basename = StripProto(file.desc().name());
  switch (auto k = file.opts().kernel) {
    case Kernel::kUpb:
      return absl::StrCat(basename, ".u.pb.rs");
    case Kernel::kCpp:
      return absl::StrCat(basename, ".c.pb.rs");
    default:
      ABSL_LOG(FATAL) << "Unknown kernel type: " << static_cast<int>(k);
      return "";
  }
}

std::string GetThunkCcFile(Context<FileDescriptor> file) {
  auto basename = StripProto(file.desc().name());
  return absl::StrCat(basename, ".pb.thunks.cc");
}

std::string GetHeaderFile(Context<FileDescriptor> file) {
  auto basename = StripProto(file.desc().name());
  return absl::StrCat(basename, ".proto.h");
}

namespace {

template <typename T>
std::string FieldPrefix(Context<T> field) {
  // NOTE: When field.is_upb(), this functions outputs must match the symbols
  // that the upbc plugin generates exactly. Failure to do so correctly results
  // in a link-time failure.
  absl::string_view prefix = field.is_cpp() ? "__rust_proto_thunk__" : "";
  std::string thunk_prefix =
      absl::StrCat(prefix, GetUnderscoreDelimitedFullName(
                               field.WithDesc(field.desc().containing_type())));
  return thunk_prefix;
}

template <typename T>
std::string Thunk(Context<T> field, absl::string_view op) {
  std::string thunk = FieldPrefix(field);

  absl::string_view format;
  if (field.is_upb() && op == "get") {
    // upb getter is simply the field name (no "get" in the name).
    format = "_$1";
  } else if (field.is_upb() && op == "get_mut") {
    // same as above, with with `mutable` prefix
    format = "_mutable_$1";
  } else if (field.is_upb() && op == "case") {
    // some upb functions are in the order x_op compared to has/set/clear which
    // are in the other order e.g. op_x.
    format = "_$1_$0";
  } else {
    format = "_$0_$1";
  }

  absl::SubstituteAndAppend(&thunk, format, op, field.desc().name());
  return thunk;
}

std::string ThunkMapOrRepeated(Context<FieldDescriptor> field,
                               absl::string_view op) {
  if (!field.is_upb()) {
    return Thunk<FieldDescriptor>(field, op);
  }

  std::string thunk = absl::StrCat("_", FieldPrefix(field));
  absl::string_view format;
  if (op == "get") {
    format = field.desc().is_map() ? "_$1_upb_map" : "_$1_upb_array";
  } else if (op == "get_mut") {
    format =
        field.desc().is_map() ? "_$1_mutable_upb_map" : "_$1_mutable_upb_array";
  } else {
    return Thunk<FieldDescriptor>(field, op);
  }

  absl::SubstituteAndAppend(&thunk, format, op, field.desc().name());
  return thunk;
}

}  // namespace

std::string Thunk(Context<FieldDescriptor> field, absl::string_view op) {
  if (field.desc().is_map() || field.desc().is_repeated()) {
    return ThunkMapOrRepeated(field, op);
  }
  return Thunk<FieldDescriptor>(field, op);
}

std::string Thunk(Context<OneofDescriptor> field, absl::string_view op) {
  return Thunk<OneofDescriptor>(field, op);
}

std::string Thunk(Context<Descriptor> msg, absl::string_view op) {
  absl::string_view prefix = msg.is_cpp() ? "__rust_proto_thunk__" : "";
  return absl::StrCat(prefix, GetUnderscoreDelimitedFullName(msg), "_", op);
}

std::string PrimitiveRsTypeName(const FieldDescriptor& desc) {
  switch (desc.type()) {
    case FieldDescriptor::TYPE_BOOL:
      return "bool";
    case FieldDescriptor::TYPE_INT32:
    case FieldDescriptor::TYPE_SINT32:
    case FieldDescriptor::TYPE_SFIXED32:
      return "i32";
    case FieldDescriptor::TYPE_INT64:
    case FieldDescriptor::TYPE_SINT64:
    case FieldDescriptor::TYPE_SFIXED64:
      return "i64";
    case FieldDescriptor::TYPE_FIXED32:
    case FieldDescriptor::TYPE_UINT32:
      return "u32";
    case FieldDescriptor::TYPE_FIXED64:
    case FieldDescriptor::TYPE_UINT64:
      return "u64";
    case FieldDescriptor::TYPE_FLOAT:
      return "f32";
    case FieldDescriptor::TYPE_DOUBLE:
      return "f64";
    case FieldDescriptor::TYPE_BYTES:
      return "[u8]";
    case FieldDescriptor::TYPE_STRING:
      return "::__pb::ProtoStr";
    default:
      break;
  }
  ABSL_LOG(FATAL) << "Unsupported field type: " << desc.type_name();
  return "";
}

// Constructs a string of the Rust modules which will contain the message.
//
// Example: Given a message 'NestedMessage' which is defined in package 'x.y'
// which is inside 'ParentMessage', the message will be placed in the
// x::y::ParentMessage_ Rust module, so this function will return the string
// "x::y::ParentMessage_::".
//
// If the message has no package and no containing messages then this returns
// empty string.
std::string RustModule(Context<Descriptor> msg) {
  const Descriptor& desc = msg.desc();

  std::vector<std::string> modules;

  std::vector<std::string> package_modules =
      absl::StrSplit(desc.file()->package(), '.', absl::SkipEmpty());

  modules.insert(modules.begin(), package_modules.begin(),
                 package_modules.end());

  // Innermost to outermost order.
  std::vector<std::string> modules_from_containing_types;
  const Descriptor* parent = desc.containing_type();
  while (parent != nullptr) {
    modules_from_containing_types.push_back(absl::StrCat(parent->name(), "_"));
    parent = parent->containing_type();
  }

  // Add the modules from containing messages (rbegin/rend to get them in outer
  // to inner order).
  modules.insert(modules.end(), modules_from_containing_types.rbegin(),
                 modules_from_containing_types.rend());

  // If there is any modules at all, push an empty string on the end so that
  // we get the trailing ::
  if (!modules.empty()) {
    modules.push_back("");
  }

  return absl::StrJoin(modules, "::");
}

std::string RustInternalModuleName(Context<FileDescriptor> file) {
  // TODO: Introduce a more robust mangling here to avoid conflicts
  // between `foo/bar/baz.proto` and `foo_bar/baz.proto`.
  return absl::StrReplaceAll(StripProto(file.desc().name()), {{"/", "_"}});
}

std::string GetCrateRelativeQualifiedPath(Context<Descriptor> msg) {
  return absl::StrCat(RustModule(msg), msg.desc().name());
}

std::string FieldInfoComment(Context<FieldDescriptor> field) {
  absl::string_view label =
      field.desc().is_repeated() ? "repeated" : "optional";
  std::string comment =
      absl::StrCat(field.desc().name(), ": ", label, " ",
                   FieldDescriptor::TypeName(field.desc().type()));

  if (auto* m = field.desc().message_type()) {
    absl::StrAppend(&comment, " ", m->full_name());
  }
  if (auto* m = field.desc().enum_type()) {
    absl::StrAppend(&comment, " ", m->full_name());
  }

  return comment;
}

std::string ViewTypeName(Context<FieldDescriptor> field,
                         absl::string_view lifetime) {
  const FieldDescriptor& desc = field.desc();

  if (desc.is_map()) {
    std::string key_type = PrimitiveRsTypeName(*desc.message_type()->map_key());
    std::string value_type =
        PrimitiveRsTypeName(*desc.message_type()->map_value());
    return absl::StrFormat("::__pb::MapView<'%s, %s, %s>", lifetime, key_type,
                           value_type);
  }

  if (desc.is_repeated()) {
    return absl::StrFormat("::__pb::RepeatedView<'%s, %s>", lifetime,
                           PrimitiveRsTypeName(desc));
  }

  switch (desc.type()) {
    case FieldDescriptor::TYPE_BYTES:
      return absl::StrFormat("&'%s [u8]", lifetime);
    case FieldDescriptor::TYPE_STRING:
      return absl::StrFormat("&'%s ::__pb::ProtoStr", lifetime);
    case FieldDescriptor::TYPE_MESSAGE: {
      auto msg_type = field.WithDesc(desc.message_type());
      return absl::StrFormat("crate::%sView::<'%s>",
                             GetCrateRelativeQualifiedPath(msg_type), lifetime);
    }
    default:
      return PrimitiveRsTypeName(desc);
  }
}

std::string MutTypeName(Context<FieldDescriptor> field,
                        absl::string_view lifetime) {
  const FieldDescriptor& desc = field.desc();

  if (desc.is_map()) {
    std::string key_type = PrimitiveRsTypeName(*desc.message_type()->map_key());
    std::string value_type =
        PrimitiveRsTypeName(*desc.message_type()->map_value());
    return absl::StrFormat("::__pb::MapMut<'%s, %s, %s>", lifetime, key_type,
                           value_type);
  }

  if (desc.is_repeated()) {
    return absl::StrFormat("::__pb::RepeatedMut<'%s, %s>", lifetime,
                           PrimitiveRsTypeName(desc));
  }

  if (desc.type() == FieldDescriptor::TYPE_MESSAGE) {
    auto msg_type = field.WithDesc(desc.message_type());
    return absl::StrFormat("crate::%sMut::<'%s>",
                           GetCrateRelativeQualifiedPath(msg_type), lifetime);
  }

  return absl::StrFormat("::__pb::Mut<'%s, %s>", lifetime,
                         PrimitiveRsTypeName(desc));
}

std::string FieldEntryTypeName(Context<FieldDescriptor> field,
                               absl::string_view lifetime) {
  const FieldDescriptor& desc = field.desc();

  if (!desc.has_presence()) {
    ABSL_LOG(FATAL) << "FieldEntry is only defined for with-presence fields, "
                       "cannot provide one for "
                    << field.desc().name();
    return "";
  }

  return absl::StrFormat("::__pb::FieldEntry<'%s, %s>", lifetime,
                         PrimitiveRsTypeName(desc));
}

}  // namespace rust
}  // namespace compiler
}  // namespace protobuf
}  // namespace google
