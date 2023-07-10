// Protocol Buffers - Google's data interchange format
// Copyright 2014 Google Inc.  All rights reserved.
// https://developers.google.com/protocol-buffers/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "protobuf.h"

#include <ruby/version.h>

#include "defs.h"
#include "map.h"
#include "message.h"
#include "repeated_field.h"

VALUE cParseError;
VALUE cTypeError;

const upb_FieldDef *map_field_key(const upb_FieldDef *field) {
  const upb_MessageDef *entry = upb_FieldDef_MessageSubDef(field);
  return upb_MessageDef_FindFieldByNumber(entry, 1);
}

const upb_FieldDef *map_field_value(const upb_FieldDef *field) {
  const upb_MessageDef *entry = upb_FieldDef_MessageSubDef(field);
  return upb_MessageDef_FindFieldByNumber(entry, 2);
}

// -----------------------------------------------------------------------------
// StringBuilder, for inspect
// -----------------------------------------------------------------------------

struct StringBuilder {
  size_t size;
  size_t cap;
  char *data;
};

typedef struct StringBuilder StringBuilder;

static size_t StringBuilder_SizeOf(size_t cap) {
  return sizeof(StringBuilder) + cap;
}

StringBuilder *StringBuilder_New() {
  const size_t cap = 128;
  StringBuilder *builder = malloc(sizeof(*builder));
  builder->size = 0;
  builder->cap = cap;
  builder->data = malloc(builder->cap);
  return builder;
}

void StringBuilder_Free(StringBuilder *b) {
  free(b->data);
  free(b);
}

void StringBuilder_Printf(StringBuilder *b, const char *fmt, ...) {
  size_t have = b->cap - b->size;
  size_t n;
  va_list args;

  va_start(args, fmt);
  n = vsnprintf(&b->data[b->size], have, fmt, args);
  va_end(args);

  if (have <= n) {
    while (have <= n) {
      b->cap *= 2;
      have = b->cap - b->size;
    }
    b->data = realloc(b->data, StringBuilder_SizeOf(b->cap));
    va_start(args, fmt);
    n = vsnprintf(&b->data[b->size], have, fmt, args);
    va_end(args);
    PBRUBY_ASSERT(n < have);
  }

  b->size += n;
}

VALUE StringBuilder_ToRubyString(StringBuilder *b) {
  VALUE ret = rb_str_new(b->data, b->size);
  rb_enc_associate(ret, rb_utf8_encoding());
  return ret;
}

static void StringBuilder_PrintEnum(StringBuilder *b, int32_t val,
                                    const upb_EnumDef *e) {
  const upb_EnumValueDef *ev = upb_EnumDef_FindValueByNumber(e, val);
  if (ev) {
    StringBuilder_Printf(b, ":%s", upb_EnumValueDef_Name(ev));
  } else {
    StringBuilder_Printf(b, "%" PRId32, val);
  }
}

void StringBuilder_PrintMsgval(StringBuilder *b, upb_MessageValue val,
                               TypeInfo info) {
  switch (info.type) {
    case kUpb_CType_Bool:
      StringBuilder_Printf(b, "%s", val.bool_val ? "true" : "false");
      break;
    case kUpb_CType_Float: {
      VALUE str = rb_inspect(DBL2NUM(val.float_val));
      StringBuilder_Printf(b, "%s", RSTRING_PTR(str));
      break;
    }
    case kUpb_CType_Double: {
      VALUE str = rb_inspect(DBL2NUM(val.double_val));
      StringBuilder_Printf(b, "%s", RSTRING_PTR(str));
      break;
    }
    case kUpb_CType_Int32:
      StringBuilder_Printf(b, "%" PRId32, val.int32_val);
      break;
    case kUpb_CType_UInt32:
      StringBuilder_Printf(b, "%" PRIu32, val.uint32_val);
      break;
    case kUpb_CType_Int64:
      StringBuilder_Printf(b, "%" PRId64, val.int64_val);
      break;
    case kUpb_CType_UInt64:
      StringBuilder_Printf(b, "%" PRIu64, val.uint64_val);
      break;
    case kUpb_CType_String:
      StringBuilder_Printf(b, "\"%.*s\"", (int)val.str_val.size,
                           val.str_val.data);
      break;
    case kUpb_CType_Bytes:
      StringBuilder_Printf(b, "\"%.*s\"", (int)val.str_val.size,
                           val.str_val.data);
      break;
    case kUpb_CType_Enum:
      StringBuilder_PrintEnum(b, val.int32_val, info.def.enumdef);
      break;
    case kUpb_CType_Message:
      Message_PrintMessage(b, val.msg_val, info.def.msgdef);
      break;
  }
}

// -----------------------------------------------------------------------------
// Arena
// -----------------------------------------------------------------------------

typedef struct {
  upb_Arena *arena;
  // IMPORTANT: WB_PROTECTED objects must only use the RB_OBJ_WRITE()
  // macro to update VALUE references, as to trigger write barriers.
  VALUE pinned_objs;
} Arena;

static void Arena_mark(void *data) {
  Arena *arena = data;
  rb_gc_mark(arena->pinned_objs);
}

static void Arena_free(void *data) {
  Arena *arena = data;
  upb_Arena_Free(arena->arena);
  xfree(arena);
}

static VALUE cArena;

const rb_data_type_t Arena_type = {
    "Google::Protobuf::Internal::Arena",
    {Arena_mark, Arena_free, NULL},
    .flags = RUBY_TYPED_FREE_IMMEDIATELY | RUBY_TYPED_WB_PROTECTED,
};

static void *ruby_upb_allocfunc(upb_alloc *alloc, void *ptr, size_t oldsize,
                                size_t size) {
  if (size == 0) {
    xfree(ptr);
    return NULL;
  } else {
    return xrealloc(ptr, size);
  }
}

upb_alloc ruby_upb_alloc = {&ruby_upb_allocfunc};

static VALUE Arena_alloc(VALUE klass) {
  Arena *arena = ALLOC(Arena);
  arena->arena = upb_Arena_Init(NULL, 0, &ruby_upb_alloc);
  arena->pinned_objs = Qnil;
  return TypedData_Wrap_Struct(klass, &Arena_type, arena);
}

upb_Arena *Arena_get(VALUE _arena) {
  Arena *arena;
  TypedData_Get_Struct(_arena, Arena, &Arena_type, arena);
  return arena->arena;
}

void Arena_fuse(VALUE _arena, upb_Arena *other) {
  Arena *arena;
  TypedData_Get_Struct(_arena, Arena, &Arena_type, arena);
  if (!upb_Arena_Fuse(arena->arena, other)) {
    rb_raise(rb_eRuntimeError,
             "Unable to fuse arenas. This should never happen since Ruby does "
             "not use initial blocks");
  }
}

VALUE Arena_new() { return Arena_alloc(cArena); }

void Arena_Pin(VALUE _arena, VALUE obj) {
  Arena *arena;
  TypedData_Get_Struct(_arena, Arena, &Arena_type, arena);
  if (arena->pinned_objs == Qnil) {
    RB_OBJ_WRITE(_arena, &arena->pinned_objs, rb_ary_new());
  }
  rb_ary_push(arena->pinned_objs, obj);
}

void Arena_register(VALUE module) {
  VALUE internal = rb_define_module_under(module, "Internal");
  VALUE klass = rb_define_class_under(internal, "Arena", rb_cObject);
  rb_define_alloc_func(klass, Arena_alloc);
  rb_gc_register_address(&cArena);
  cArena = klass;
}

// -----------------------------------------------------------------------------
// Object Cache
// -----------------------------------------------------------------------------

// Public ObjectCache API.

VALUE weak_obj_cache = Qnil;
ID item_get;
ID item_try_add;

static void ObjectCache_Init(VALUE protobuf) {
  item_get = rb_intern("get");
  item_try_add = rb_intern("try_add");

  rb_gc_register_address(&weak_obj_cache);
#if RUBY_API_VERSION_CODE >= 20700 && SIZEOF_LONG >= SIZEOF_VALUE
  VALUE cache_class = rb_const_get(protobuf, rb_intern("ObjectCache"));
#else
  VALUE cache_class = rb_const_get(protobuf, rb_intern("LegacyObjectCache"));
#endif

  weak_obj_cache = rb_class_new_instance(0, NULL, cache_class);
  rb_const_set(protobuf, rb_intern("OBJECT_CACHE"), weak_obj_cache);
  rb_const_set(protobuf, rb_intern("SIZEOF_LONG"), INT2NUM(SIZEOF_LONG));
  rb_const_set(protobuf, rb_intern("SIZEOF_VALUE"), INT2NUM(SIZEOF_VALUE));
}

VALUE ObjectCache_TryAdd(const void *key, VALUE val) {
  VALUE key_val = (VALUE)key;
  PBRUBY_ASSERT((key_val & 3) == 0);
  return rb_funcall(weak_obj_cache, item_try_add, 2, LL2NUM(key_val), val);
}

// Returns the cached object for this key, if any. Otherwise returns Qnil.
VALUE ObjectCache_Get(const void *key) {
  VALUE key_val = (VALUE)key;
  PBRUBY_ASSERT((key_val & 3) == 0);
  return rb_funcall(weak_obj_cache, item_get, 1, LL2NUM(key_val));
}

/*
 * call-seq:
 *     Google::Protobuf.discard_unknown(msg)
 *
 * Discard unknown fields in the given message object and recursively discard
 * unknown fields in submessages.
 */
static VALUE Google_Protobuf_discard_unknown(VALUE self, VALUE msg_rb) {
  const upb_MessageDef *m;
  upb_Message *msg = Message_GetMutable(msg_rb, &m);
  if (!upb_Message_DiscardUnknown(msg, m, 128)) {
    rb_raise(rb_eRuntimeError, "Messages nested too deeply.");
  }

  return Qnil;
}

/*
 * call-seq:
 *     Google::Protobuf.deep_copy(obj) => copy_of_obj
 *
 * Performs a deep copy of a RepeatedField instance, a Map instance, or a
 * message object, recursively copying its members.
 */
VALUE Google_Protobuf_deep_copy(VALUE self, VALUE obj) {
  VALUE klass = CLASS_OF(obj);
  if (klass == cRepeatedField) {
    return RepeatedField_deep_copy(obj);
  } else if (klass == cMap) {
    return Map_deep_copy(obj);
  } else {
    VALUE new_arena_rb = Arena_new();
    upb_Arena *new_arena = Arena_get(new_arena_rb);
    const upb_MessageDef *m;
    const upb_Message *msg = Message_Get(obj, &m);
    upb_Message *new_msg = Message_deep_copy(msg, m, new_arena);
    return Message_GetRubyWrapper(new_msg, m, new_arena_rb);
  }
}

// -----------------------------------------------------------------------------
// Initialization/entry point.
// -----------------------------------------------------------------------------

// This must be named "Init_protobuf_c" because the Ruby module is named
// "protobuf_c" -- the VM looks for this symbol in our .so.
__attribute__((visibility("default"))) void Init_protobuf_c() {
  VALUE google = rb_define_module("Google");
  VALUE protobuf = rb_define_module_under(google, "Protobuf");

  ObjectCache_Init(protobuf);
  Arena_register(protobuf);
  Defs_register(protobuf);
  RepeatedField_register(protobuf);
  Map_register(protobuf);
  Message_register(protobuf);

  cParseError = rb_const_get(protobuf, rb_intern("ParseError"));
  rb_gc_register_mark_object(cParseError);
  cTypeError = rb_const_get(protobuf, rb_intern("TypeError"));
  rb_gc_register_mark_object(cTypeError);

  rb_define_singleton_method(protobuf, "discard_unknown",
                             Google_Protobuf_discard_unknown, 1);
  rb_define_singleton_method(protobuf, "deep_copy", Google_Protobuf_deep_copy,
                             1);
}
