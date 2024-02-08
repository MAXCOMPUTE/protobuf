// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#ifndef GOOGLE_PROTOBUF_MAP_TYPE_HANDLER_H__
#define GOOGLE_PROTOBUF_MAP_TYPE_HANDLER_H__

#include <cstddef>
#include <cstdint>
#include <type_traits>

#include "google/protobuf/arena.h"
#include "google/protobuf/arenastring.h"
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/parse_context.h"
#include "google/protobuf/wire_format_lite.h"

#ifdef SWIG
#error "You cannot SWIG proto headers"
#endif

namespace google {
namespace protobuf {
namespace internal {

template <WireFormatLite::FieldType field_type, typename Type>
class MapTypeHandler;

template <typename Type>
class MapTypeHandler<WireFormatLite::TYPE_MESSAGE, Type> {
 public:
  // Functions used in parsing and serialization. ===================
  static inline size_t ByteSize(const Type& value);
  static inline int GetCachedSize(const Type& value);

  static inline uint8_t* Write(int field, const Type& value, uint8_t* ptr,
                               io::EpsCopyOutputStream* stream);
};

#define MAP_HANDLER(FieldType)                                               \
  template <typename Type>                                                   \
  class MapTypeHandler<WireFormatLite::TYPE_##FieldType, Type> {             \
   public:                                                                   \
    static inline int ByteSize(const Type& value);                           \
    static inline int GetCachedSize(const Type& value);                      \
    static inline uint8_t* Write(int field, const Type& value, uint8_t* ptr, \
                                 io::EpsCopyOutputStream* stream);           \
  };
MAP_HANDLER(STRING)
MAP_HANDLER(BYTES)
MAP_HANDLER(INT64)
MAP_HANDLER(UINT64)
MAP_HANDLER(INT32)
MAP_HANDLER(UINT32)
MAP_HANDLER(SINT64)
MAP_HANDLER(SINT32)
MAP_HANDLER(ENUM)
MAP_HANDLER(DOUBLE)
MAP_HANDLER(FLOAT)
MAP_HANDLER(FIXED64)
MAP_HANDLER(FIXED32)
MAP_HANDLER(SFIXED64)
MAP_HANDLER(SFIXED32)
MAP_HANDLER(BOOL)
#undef MAP_HANDLER

template <typename Type>
inline size_t MapTypeHandler<WireFormatLite::TYPE_MESSAGE, Type>::ByteSize(
    const Type& value) {
  return WireFormatLite::MessageSizeNoVirtual(value);
}

#define GOOGLE_PROTOBUF_BYTE_SIZE(FieldType, DeclaredType)                     \
  template <typename Type>                                                     \
  inline int MapTypeHandler<WireFormatLite::TYPE_##FieldType, Type>::ByteSize( \
      const Type& value) {                                                     \
    return static_cast<int>(WireFormatLite::DeclaredType##Size(value));        \
  }

GOOGLE_PROTOBUF_BYTE_SIZE(STRING, String)
GOOGLE_PROTOBUF_BYTE_SIZE(BYTES, Bytes)
GOOGLE_PROTOBUF_BYTE_SIZE(INT64, Int64)
GOOGLE_PROTOBUF_BYTE_SIZE(UINT64, UInt64)
GOOGLE_PROTOBUF_BYTE_SIZE(INT32, Int32)
GOOGLE_PROTOBUF_BYTE_SIZE(UINT32, UInt32)
GOOGLE_PROTOBUF_BYTE_SIZE(SINT64, SInt64)
GOOGLE_PROTOBUF_BYTE_SIZE(SINT32, SInt32)
GOOGLE_PROTOBUF_BYTE_SIZE(ENUM, Enum)

#undef GOOGLE_PROTOBUF_BYTE_SIZE

#define FIXED_BYTE_SIZE(FieldType, DeclaredType)                               \
  template <typename Type>                                                     \
  inline int MapTypeHandler<WireFormatLite::TYPE_##FieldType, Type>::ByteSize( \
      const Type& /* value */) {                                               \
    return WireFormatLite::k##DeclaredType##Size;                              \
  }

FIXED_BYTE_SIZE(DOUBLE, Double)
FIXED_BYTE_SIZE(FLOAT, Float)
FIXED_BYTE_SIZE(FIXED64, Fixed64)
FIXED_BYTE_SIZE(FIXED32, Fixed32)
FIXED_BYTE_SIZE(SFIXED64, SFixed64)
FIXED_BYTE_SIZE(SFIXED32, SFixed32)
FIXED_BYTE_SIZE(BOOL, Bool)

#undef FIXED_BYTE_SIZE

template <typename Type>
inline int MapTypeHandler<WireFormatLite::TYPE_MESSAGE, Type>::GetCachedSize(
    const Type& value) {
  return static_cast<int>(WireFormatLite::LengthDelimitedSize(
      static_cast<size_t>(value.GetCachedSize())));
}

#define GET_CACHED_SIZE(FieldType, DeclaredType)                        \
  template <typename Type>                                              \
  inline int MapTypeHandler<WireFormatLite::TYPE_##FieldType,           \
                            Type>::GetCachedSize(const Type& value) {   \
    return static_cast<int>(WireFormatLite::DeclaredType##Size(value)); \
  }

GET_CACHED_SIZE(STRING, String)
GET_CACHED_SIZE(BYTES, Bytes)
GET_CACHED_SIZE(INT64, Int64)
GET_CACHED_SIZE(UINT64, UInt64)
GET_CACHED_SIZE(INT32, Int32)
GET_CACHED_SIZE(UINT32, UInt32)
GET_CACHED_SIZE(SINT64, SInt64)
GET_CACHED_SIZE(SINT32, SInt32)
GET_CACHED_SIZE(ENUM, Enum)

#undef GET_CACHED_SIZE

#define GET_FIXED_CACHED_SIZE(FieldType, DeclaredType)                      \
  template <typename Type>                                                  \
  inline int MapTypeHandler<WireFormatLite::TYPE_##FieldType,               \
                            Type>::GetCachedSize(const Type& /* value */) { \
    return WireFormatLite::k##DeclaredType##Size;                           \
  }

GET_FIXED_CACHED_SIZE(DOUBLE, Double)
GET_FIXED_CACHED_SIZE(FLOAT, Float)
GET_FIXED_CACHED_SIZE(FIXED64, Fixed64)
GET_FIXED_CACHED_SIZE(FIXED32, Fixed32)
GET_FIXED_CACHED_SIZE(SFIXED64, SFixed64)
GET_FIXED_CACHED_SIZE(SFIXED32, SFixed32)
GET_FIXED_CACHED_SIZE(BOOL, Bool)

#undef GET_FIXED_CACHED_SIZE

template <typename Type>
inline uint8_t* MapTypeHandler<WireFormatLite::TYPE_MESSAGE, Type>::Write(
    int field, const Type& value, uint8_t* ptr,
    io::EpsCopyOutputStream* stream) {
  ptr = stream->EnsureSpace(ptr);
  return WireFormatLite::InternalWriteMessage(
      field, value, value.GetCachedSize(), ptr, stream);
}

#define WRITE_METHOD(FieldType, DeclaredType)                    \
  template <typename Type>                                       \
  inline uint8_t*                                                \
  MapTypeHandler<WireFormatLite::TYPE_##FieldType, Type>::Write( \
      int field, const Type& value, uint8_t* ptr,                \
      io::EpsCopyOutputStream* stream) {                         \
    ptr = stream->EnsureSpace(ptr);                              \
    return stream->Write##DeclaredType(field, value, ptr);       \
  }

WRITE_METHOD(STRING, String)
WRITE_METHOD(BYTES, Bytes)

#undef WRITE_METHOD
#define WRITE_METHOD(FieldType, DeclaredType)                               \
  template <typename Type>                                                  \
  inline uint8_t*                                                           \
  MapTypeHandler<WireFormatLite::TYPE_##FieldType, Type>::Write(            \
      int field, const Type& value, uint8_t* ptr,                           \
      io::EpsCopyOutputStream* stream) {                                    \
    ptr = stream->EnsureSpace(ptr);                                         \
    return WireFormatLite::Write##DeclaredType##ToArray(field, value, ptr); \
  }

WRITE_METHOD(INT64, Int64)
WRITE_METHOD(UINT64, UInt64)
WRITE_METHOD(INT32, Int32)
WRITE_METHOD(UINT32, UInt32)
WRITE_METHOD(SINT64, SInt64)
WRITE_METHOD(SINT32, SInt32)
WRITE_METHOD(ENUM, Enum)
WRITE_METHOD(DOUBLE, Double)
WRITE_METHOD(FLOAT, Float)
WRITE_METHOD(FIXED64, Fixed64)
WRITE_METHOD(FIXED32, Fixed32)
WRITE_METHOD(SFIXED64, SFixed64)
WRITE_METHOD(SFIXED32, SFixed32)
WRITE_METHOD(BOOL, Bool)

#undef WRITE_METHOD

// Functions for operating on a map entry using type handlers.
//
// Does not contain any representation (this class is not intended to be
// instantiated).
template <typename Key, typename Value, WireFormatLite::FieldType kKeyFieldType,
          WireFormatLite::FieldType kValueFieldType>
struct MapEntryFuncs {
  typedef MapTypeHandler<kKeyFieldType, Key> KeyTypeHandler;
  typedef MapTypeHandler<kValueFieldType, Value> ValueTypeHandler;
  enum : int {
    kKeyFieldNumber = 1,
    kValueFieldNumber = 2
  };

  static uint8_t* InternalSerialize(int field_number, const Key& key,
                                    const Value& value, uint8_t* ptr,
                                    io::EpsCopyOutputStream* stream) {
    ptr = stream->EnsureSpace(ptr);
    ptr = WireFormatLite::WriteTagToArray(
        field_number, WireFormatLite::WIRETYPE_LENGTH_DELIMITED, ptr);
    ptr = io::CodedOutputStream::WriteVarint32ToArray(GetCachedSize(key, value),
                                                      ptr);

    ptr = KeyTypeHandler::Write(kKeyFieldNumber, key, ptr, stream);
    return ValueTypeHandler::Write(kValueFieldNumber, value, ptr, stream);
  }

  static size_t ByteSizeLong(const Key& key, const Value& value) {
    // Tags for key and value will both be one byte (field numbers 1 and 2).
    size_t inner_length =
        2 + KeyTypeHandler::ByteSize(key) + ValueTypeHandler::ByteSize(value);
    return inner_length + io::CodedOutputStream::VarintSize32(
                              static_cast<uint32_t>(inner_length));
  }

  static int GetCachedSize(const Key& key, const Value& value) {
    // Tags for key and value will both be one byte (field numbers 1 and 2).
    return 2 + KeyTypeHandler::GetCachedSize(key) +
           ValueTypeHandler::GetCachedSize(value);
  }
};

}  // namespace internal
}  // namespace protobuf
}  // namespace google

#endif  // GOOGLE_PROTOBUF_MAP_TYPE_HANDLER_H__
