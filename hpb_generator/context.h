// Protocol Buffers - Google's data interchange format
// Copyright 2024 Google LLC.  All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#ifndef GOOGLE_PROTOBUF_COMPILER_HPB_CONTEXT_H__
#define GOOGLE_PROTOBUF_COMPILER_HPB_CONTEXT_H__

#include <string>

#include "absl/strings/ascii.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_replace.h"
#include "absl/strings/string_view.h"
#include "absl/strings/substitute.h"
#include "absl/types/source_location.h"
#include "absl/types/span.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/io/printer.h"
#include "google/protobuf/io/zero_copy_stream.h"
#include "upb/reflection/def.hpp"
#include "upb_generator/common/cpp_to_upb_def.h"

namespace google::protobuf::hpb_generator {

enum class Backend { UPB, CPP };

struct Options {
  Backend backend = Backend::UPB;
};

/**
 * This Context object will be used throughout hpb generation.
 * It is a thin wrapper around an io::Printer and can be easily extended
 * to support more options.
 *
 * Expected usage is:
 * SomeGenerationFunc(..., Context& context) {
 *   context.Emit({{"some_key", some_computed_val}}, R"cc(
 *   // hpb gencode ...
 *  )cc);
 * }
 */
class Context final {
 public:
  Context(const FileDescriptor* file, io::ZeroCopyOutputStream* stream,
          const Options& options)
      : stream_(stream), printer_(stream_), options_(options) {
    BuildDefPool(file);
  }

  void Emit(absl::Span<const io::Printer::Sub> vars, absl::string_view format,
            absl::SourceLocation loc = absl::SourceLocation::current()) {
    printer_.Emit(vars, format, loc);
  }

  void Emit(absl::string_view format,
            absl::SourceLocation loc = absl::SourceLocation::current()) {
    printer_.Emit(format, loc);
  }

  // TODO: b/373438292 - Remove EmitLegacy in favor of Emit.
  // This is an interim solution while we migrate from Output to io::Printer
  template <class... Arg>
  void EmitLegacy(absl::string_view format, const Arg&... arg) {
    auto res = absl::Substitute(format, arg...);
    printer_.Emit(res, absl::SourceLocation::current());
  }

  const Options& options() { return options_; }
  io::Printer& printer() { return printer_; }

  inline std::string GetLayoutIndex(const FieldDescriptor* field) {
    return absl::StrCat(
        upb::generator::FindBaseFieldDef(pool_, field).layout_index());
  }

  Context(const Context&) = delete;
  Context& operator=(const Context&) = delete;
  Context(Context&&) = delete;
  Context& operator=(Context&&) = delete;

 private:
  inline void BuildDefPool(const FileDescriptor* file) {
    upb::generator::AddFile(file, &pool_);
  }

  io::ZeroCopyOutputStream* stream_;
  io::Printer printer_;
  const Options& options_;
  upb::DefPool pool_;
};

// TODO: b/373438292 - re-house these 4 legacy funcs post io::Printer move
inline std::string ToCIdent(absl::string_view str) {
  return absl::StrReplaceAll(str, {{".", "_"}, {"/", "_"}, {"-", "_"}});
}

inline std::string ToPreproc(absl::string_view str) {
  return absl::AsciiStrToUpper(ToCIdent(str));
}

inline void EmitFileWarning(const google::protobuf::FileDescriptor* file, Context& ctx) {
  ctx.EmitLegacy(
      R"cc(
        /* This file was generated by hpb_generator (Handle Protobuf) "
        from the input
         * file:
         *
         *     $0
         *
         * Do not edit -- your changes will be discarded when the file is
         * regenerated. */
      )cc",
      file->name());
  ctx.Emit("\n");
}

// TODO: b/346865271 append ::hpb instead of ::protos after namespace swap
inline std::string NamespaceFromPackageName(absl::string_view package_name) {
  return absl::StrCat(absl::StrReplaceAll(package_name, {{".", "::"}}),
                      "::protos");
}

template <typename T>
void WrapNamespace(const google::protobuf::FileDescriptor* file, Context& ctx, T&& body) {
  if (file->package().empty()) {
    body();
  } else {
    ctx.Emit(
        {
            {"body", body},
            {"namespace", NamespaceFromPackageName(file->package())},
        },
        R"cc(
          namespace $namespace$ {

          $body$

          }  // namespace $namespace$
        )cc");
  }
}
}  // namespace protobuf
}  // namespace google::hpb_generator

#endif  // GOOGLE_PROTOBUF_COMPILER_HPB_CONTEXT_H__
