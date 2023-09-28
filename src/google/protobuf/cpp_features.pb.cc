// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: google/protobuf/cpp_features.proto

#include "google/protobuf/cpp_features.pb.h"

#include <algorithm>
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/extension_set.h"
#include "google/protobuf/wire_format_lite.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/generated_message_reflection.h"
#include "google/protobuf/reflection_ops.h"
#include "google/protobuf/wire_format.h"
#include "google/protobuf/generated_message_tctable_impl.h"
// @@protoc_insertion_point(includes)

// Must be included last.
#include "google/protobuf/port_def.inc"
PROTOBUF_PRAGMA_INIT_SEG
namespace _pb = ::google::protobuf;
namespace _pbi = ::google::protobuf::internal;
namespace _fl = ::google::protobuf::internal::field_layout;
namespace pb {

inline constexpr CppFeatures::Impl_::Impl_(
    ::_pbi::ConstantInitialized) noexcept
      : _cached_size_{0},
        legacy_closed_enum_{false} {}

template <typename>
PROTOBUF_CONSTEXPR CppFeatures::CppFeatures(::_pbi::ConstantInitialized)
    : _impl_(::_pbi::ConstantInitialized()) {}
struct CppFeaturesDefaultTypeInternal {
  PROTOBUF_CONSTEXPR CppFeaturesDefaultTypeInternal() : _instance(::_pbi::ConstantInitialized{}) {}
  ~CppFeaturesDefaultTypeInternal() {}
  union {
    CppFeatures _instance;
  };
};

PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_EXPORT
    PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 CppFeaturesDefaultTypeInternal _CppFeatures_default_instance_;
}  // namespace pb
static ::_pb::Metadata file_level_metadata_google_2fprotobuf_2fcpp_5ffeatures_2eproto[1];
static constexpr const ::_pb::EnumDescriptor**
    file_level_enum_descriptors_google_2fprotobuf_2fcpp_5ffeatures_2eproto = nullptr;
static constexpr const ::_pb::ServiceDescriptor**
    file_level_service_descriptors_google_2fprotobuf_2fcpp_5ffeatures_2eproto = nullptr;
const ::uint32_t TableStruct_google_2fprotobuf_2fcpp_5ffeatures_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(
    protodesc_cold) = {
    PROTOBUF_FIELD_OFFSET(::pb::CppFeatures, _impl_._has_bits_),
    PROTOBUF_FIELD_OFFSET(::pb::CppFeatures, _internal_metadata_),
    ~0u,  // no _extensions_
    ~0u,  // no _oneof_case_
    ~0u,  // no _weak_field_map_
    ~0u,  // no _inlined_string_donated_
    ~0u,  // no _split_
    ~0u,  // no sizeof(Split)
    PROTOBUF_FIELD_OFFSET(::pb::CppFeatures, _impl_.legacy_closed_enum_),
    0,
};

static const ::_pbi::MigrationSchema
    schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
        {0, 9, -1, sizeof(::pb::CppFeatures)},
};

static const ::_pb::Message* const file_default_instances[] = {
    &::pb::_CppFeatures_default_instance_._instance,
};
const char descriptor_table_protodef_google_2fprotobuf_2fcpp_5ffeatures_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
    "\n\"google/protobuf/cpp_features.proto\022\002pb"
    "\032 google/protobuf/descriptor.proto\"A\n\013Cp"
    "pFeatures\0222\n\022legacy_closed_enum\030\001 \001(\010B\026\210"
    "\001\001\230\001\004\230\001\001\242\001\n\022\005false\030\350\007::\n\003cpp\022\033.google.pr"
    "otobuf.FeatureSet\030\350\007 \001(\0132\017.pb.CppFeature"
    "s"
};
static const ::_pbi::DescriptorTable* const descriptor_table_google_2fprotobuf_2fcpp_5ffeatures_2eproto_deps[1] =
    {
        &::descriptor_table_google_2fprotobuf_2fdescriptor_2eproto,
};
static ::absl::once_flag descriptor_table_google_2fprotobuf_2fcpp_5ffeatures_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_google_2fprotobuf_2fcpp_5ffeatures_2eproto = {
    false,
    false,
    201,
    descriptor_table_protodef_google_2fprotobuf_2fcpp_5ffeatures_2eproto,
    "google/protobuf/cpp_features.proto",
    &descriptor_table_google_2fprotobuf_2fcpp_5ffeatures_2eproto_once,
    descriptor_table_google_2fprotobuf_2fcpp_5ffeatures_2eproto_deps,
    1,
    1,
    schemas,
    file_default_instances,
    TableStruct_google_2fprotobuf_2fcpp_5ffeatures_2eproto::offsets,
    file_level_metadata_google_2fprotobuf_2fcpp_5ffeatures_2eproto,
    file_level_enum_descriptors_google_2fprotobuf_2fcpp_5ffeatures_2eproto,
    file_level_service_descriptors_google_2fprotobuf_2fcpp_5ffeatures_2eproto,
};

// This function exists to be marked as weak.
// It can significantly speed up compilation by breaking up LLVM's SCC
// in the .pb.cc translation units. Large translation units see a
// reduction of more than 35% of walltime for optimized builds. Without
// the weak attribute all the messages in the file, including all the
// vtables and everything they use become part of the same SCC through
// a cycle like:
// GetMetadata -> descriptor table -> default instances ->
//   vtables -> GetMetadata
// By adding a weak function here we break the connection from the
// individual vtables back into the descriptor table.
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_google_2fprotobuf_2fcpp_5ffeatures_2eproto_getter() {
  return &descriptor_table_google_2fprotobuf_2fcpp_5ffeatures_2eproto;
}
namespace pb {
// ===================================================================

class CppFeatures::_Internal {
 public:
  using HasBits = decltype(std::declval<CppFeatures>()._impl_._has_bits_);
  static constexpr ::int32_t kHasBitsOffset =
    8 * PROTOBUF_FIELD_OFFSET(CppFeatures, _impl_._has_bits_);
  static void set_has_legacy_closed_enum(HasBits* has_bits) {
    (*has_bits)[0] |= 1u;
  }
};

CppFeatures::CppFeatures(::google::protobuf::Arena* arena)
    : ::google::protobuf::Message(arena) {
  SharedCtor(arena);
  // @@protoc_insertion_point(arena_constructor:pb.CppFeatures)
}
CppFeatures::CppFeatures(
    ::google::protobuf::Arena* arena, const CppFeatures& from)
    : CppFeatures(arena) {
  MergeFrom(from);
}
inline PROTOBUF_NDEBUG_INLINE CppFeatures::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility,
    ::google::protobuf::Arena* arena)
      : _cached_size_{0} {}

inline void CppFeatures::SharedCtor(::_pb::Arena* arena) {
  new (&_impl_) Impl_(internal_visibility(), arena);
  _impl_.legacy_closed_enum_ = {};
}
CppFeatures::~CppFeatures() {
  // @@protoc_insertion_point(destructor:pb.CppFeatures)
  _internal_metadata_.Delete<::google::protobuf::UnknownFieldSet>();
  SharedDtor();
}
inline void CppFeatures::SharedDtor() {
  ABSL_DCHECK(GetArena() == nullptr);
  _impl_.~Impl_();
}

PROTOBUF_NOINLINE void CppFeatures::Clear() {
// @@protoc_insertion_point(message_clear_start:pb.CppFeatures)
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.legacy_closed_enum_ = false;
  _impl_._has_bits_.Clear();
  _internal_metadata_.Clear<::google::protobuf::UnknownFieldSet>();
}

const char* CppFeatures::_InternalParse(
    const char* ptr, ::_pbi::ParseContext* ctx) {
  ptr = ::_pbi::TcParser::ParseLoop(this, ptr, ctx, &_table_.header);
  return ptr;
}


PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1
const ::_pbi::TcParseTable<0, 1, 0, 0, 2> CppFeatures::_table_ = {
  {
    PROTOBUF_FIELD_OFFSET(CppFeatures, _impl_._has_bits_),
    0, // no _extensions_
    1, 0,  // max_field_number, fast_idx_mask
    offsetof(decltype(_table_), field_lookup_table),
    4294967294,  // skipmap
    offsetof(decltype(_table_), field_entries),
    1,  // num_field_entries
    0,  // num_aux_entries
    offsetof(decltype(_table_), field_names),  // no aux_entries
    &_CppFeatures_default_instance_._instance,
    ::_pbi::TcParser::GenericFallback,  // fallback
  }, {{
    // optional bool legacy_closed_enum = 1 [retention = RETENTION_RUNTIME, targets = TARGET_TYPE_FIELD, targets = TARGET_TYPE_FILE, edition_defaults = {
    {::_pbi::TcParser::SingularVarintNoZag1<bool, offsetof(CppFeatures, _impl_.legacy_closed_enum_), 0>(),
     {8, 0, 0, PROTOBUF_FIELD_OFFSET(CppFeatures, _impl_.legacy_closed_enum_)}},
  }}, {{
    65535, 65535
  }}, {{
    // optional bool legacy_closed_enum = 1 [retention = RETENTION_RUNTIME, targets = TARGET_TYPE_FIELD, targets = TARGET_TYPE_FILE, edition_defaults = {
    {PROTOBUF_FIELD_OFFSET(CppFeatures, _impl_.legacy_closed_enum_), _Internal::kHasBitsOffset + 0, 0,
    (0 | ::_fl::kFcOptional | ::_fl::kBool)},
  }},
  // no aux_entries
  {{
  }},
};

::uint8_t* CppFeatures::_InternalSerialize(
    ::uint8_t* target,
    ::google::protobuf::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:pb.CppFeatures)
  ::uint32_t cached_has_bits = 0;
  (void)cached_has_bits;

  cached_has_bits = _impl_._has_bits_[0];
  // optional bool legacy_closed_enum = 1 [retention = RETENTION_RUNTIME, targets = TARGET_TYPE_FIELD, targets = TARGET_TYPE_FILE, edition_defaults = {
  if (cached_has_bits & 0x00000001u) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteBoolToArray(
        1, this->_internal_legacy_closed_enum(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target =
        ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
            _internal_metadata_.unknown_fields<::google::protobuf::UnknownFieldSet>(::google::protobuf::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:pb.CppFeatures)
  return target;
}

::size_t CppFeatures::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:pb.CppFeatures)
  ::size_t total_size = 0;

  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // optional bool legacy_closed_enum = 1 [retention = RETENTION_RUNTIME, targets = TARGET_TYPE_FIELD, targets = TARGET_TYPE_FILE, edition_defaults = {
  cached_has_bits = _impl_._has_bits_[0];
  if (cached_has_bits & 0x00000001u) {
    total_size += 2;
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::google::protobuf::Message::ClassData CppFeatures::_class_data_ = {
    CppFeatures::MergeImpl,
    nullptr,  // OnDemandRegisterArenaDtor
};
const ::google::protobuf::Message::ClassData* CppFeatures::GetClassData() const {
  return &_class_data_;
}

void CppFeatures::MergeImpl(::google::protobuf::Message& to_msg, const ::google::protobuf::Message& from_msg) {
  auto* const _this = static_cast<CppFeatures*>(&to_msg);
  auto& from = static_cast<const CppFeatures&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:pb.CppFeatures)
  ABSL_DCHECK_NE(&from, _this);
  ::uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if ((from._impl_._has_bits_[0] & 0x00000001u) != 0) {
    _this->_internal_set_legacy_closed_enum(from._internal_legacy_closed_enum());
  }
  _this->_internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(from._internal_metadata_);
}

void CppFeatures::CopyFrom(const CppFeatures& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:pb.CppFeatures)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

PROTOBUF_NOINLINE bool CppFeatures::IsInitialized() const {
  return true;
}

::_pbi::CachedSize* CppFeatures::AccessCachedSize() const {
  return &_impl_._cached_size_;
}
void CppFeatures::InternalSwap(CppFeatures* PROTOBUF_RESTRICT other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_impl_._has_bits_[0], other->_impl_._has_bits_[0]);
        swap(_impl_.legacy_closed_enum_, other->_impl_.legacy_closed_enum_);
}

::google::protobuf::Metadata CppFeatures::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_google_2fprotobuf_2fcpp_5ffeatures_2eproto_getter, &descriptor_table_google_2fprotobuf_2fcpp_5ffeatures_2eproto_once,
      file_level_metadata_google_2fprotobuf_2fcpp_5ffeatures_2eproto[0]);
}
PROTOBUF_CONSTINIT PROTOBUF_EXPORT PROTOBUF_ATTRIBUTE_INIT_PRIORITY2
::google::protobuf::internal::ExtensionIdentifier< ::google::protobuf::FeatureSet,
    ::google::protobuf::internal::MessageTypeTraits< ::pb::CppFeatures >, 11, false>
  cpp(kCppFieldNumber);
// @@protoc_insertion_point(namespace_scope)
}  // namespace pb
namespace google {
namespace protobuf {
}  // namespace protobuf
}  // namespace google
// @@protoc_insertion_point(global_scope)
#include "google/protobuf/port_undef.inc"
