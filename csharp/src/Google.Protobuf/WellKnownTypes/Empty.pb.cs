// <auto-generated>
//     Generated by the protocol buffer compiler.  DO NOT EDIT!
//     source: google/protobuf/empty.proto
// </auto-generated>
#pragma warning disable 1591, 0612, 3021, 8981
#region Designer generated code

using pb = global::Google.Protobuf;
using pbc = global::Google.Protobuf.Collections;
using pbr = global::Google.Protobuf.Reflection;
using scg = global::System.Collections.Generic;
namespace Google.Protobuf.WellKnownTypes {

  /// <summary>Holder for reflection information generated from google/protobuf/empty.proto</summary>
  public static partial class EmptyReflection {

    internal const string ConstProtocVersion = "3.21.9";
    /// <summary>Version of protoc used to generate this code.</summary>
    public static global::System.Version ProtocVersion {
      get { return protocVersion; }
    }
    private static readonly global::System.Version protocVersion = new global::System.Version(ConstProtocVersion);

    /// <summary>Minimum version of Google.Protobuf which is compatible with this generated code.</summary>
    public static global::System.Version MinimumRuntimeVersion {
      get { return minimumRuntimeVersion; }
    }
    private static readonly global::System.Version minimumRuntimeVersion = new global::System.Version(3, 21, 9);

    static EmptyReflection() {
      pbr::RuntimeVersion.Validate(MinimumRuntimeVersion);
    }

    internal static bool ValidateRuntimeVersion() {
      return true;
    }

    #region Descriptor
    private static readonly global::System.Lazy<pbr::FileDescriptor> descriptor = new global::System.Lazy<pbr::FileDescriptor>(CreateFileDescriptor);
    /// <summary>File descriptor for google/protobuf/empty.proto</summary>
    public static pbr::FileDescriptor Descriptor {
      get { return descriptor.Value; }
    }

    private static pbr::FileDescriptor CreateFileDescriptor() {
      pbr::RuntimeVersion.Validate(MinimumRuntimeVersion);
      byte[] descriptorData = global::System.Convert.FromBase64String(
          string.Concat(
            "Chtnb29nbGUvcHJvdG9idWYvZW1wdHkucHJvdG8SD2dvb2dsZS5wcm90b2J1",
            "ZiIHCgVFbXB0eUJ9ChNjb20uZ29vZ2xlLnByb3RvYnVmQgpFbXB0eVByb3Rv",
            "UAFaLmdvb2dsZS5nb2xhbmcub3JnL3Byb3RvYnVmL3R5cGVzL2tub3duL2Vt",
            "cHR5cGL4AQGiAgNHUEKqAh5Hb29nbGUuUHJvdG9idWYuV2VsbEtub3duVHlw",
            "ZXNiBnByb3RvMw=="));
      return pbr::FileDescriptor.FromGeneratedCode(descriptorData,
          new pbr::FileDescriptor[] { },
          new pbr::GeneratedClrTypeInfo(null, null, new pbr::GeneratedClrTypeInfo[] {
            new pbr::GeneratedClrTypeInfo(typeof(global::Google.Protobuf.WellKnownTypes.Empty), global::Google.Protobuf.WellKnownTypes.Empty.Parser, null, null, null, null, null)
          }));
    }
    #endregion

  }
  #region Messages
  /// <summary>
  /// A generic empty message that you can re-use to avoid defining duplicated
  /// empty messages in your APIs. A typical example is to use it as the request
  /// or the response type of an API method. For instance:
  ///
  ///     service Foo {
  ///       rpc Bar(google.protobuf.Empty) returns (google.protobuf.Empty);
  ///     }
  /// </summary>
  public sealed partial class Empty : pb::IMessage<Empty>
  #if !GOOGLE_PROTOBUF_REFSTRUCT_COMPATIBILITY_MODE
      , pb::IBufferMessage
  #endif
  {
    private static readonly bool _runtimeVersionValidated = global::Google.Protobuf.WellKnownTypes.EmptyReflection.ValidateRuntimeVersion();

    private static readonly pb::MessageParser<Empty> _parser = new pb::MessageParser<Empty>(() => new Empty());
    private pb::UnknownFieldSet _unknownFields;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", global::Google.Protobuf.WellKnownTypes.EmptyReflection.ConstProtocVersion)]
    public static pb::MessageParser<Empty> Parser { get { return _parser; } }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", global::Google.Protobuf.WellKnownTypes.EmptyReflection.ConstProtocVersion)]
    public static pbr::MessageDescriptor Descriptor {
      get { return global::Google.Protobuf.WellKnownTypes.EmptyReflection.Descriptor.MessageTypes[0]; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", global::Google.Protobuf.WellKnownTypes.EmptyReflection.ConstProtocVersion)]
    pbr::MessageDescriptor pb::IMessage.Descriptor {
      get { return Descriptor; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", global::Google.Protobuf.WellKnownTypes.EmptyReflection.ConstProtocVersion)]
    public Empty() {
      global::Google.Protobuf.WellKnownTypes.EmptyReflection.ValidateRuntimeVersion();
      OnConstruction();
    }

    partial void OnConstruction();

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", global::Google.Protobuf.WellKnownTypes.EmptyReflection.ConstProtocVersion)]
    public Empty(Empty other) : this() {
      _unknownFields = pb::UnknownFieldSet.Clone(other._unknownFields);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", global::Google.Protobuf.WellKnownTypes.EmptyReflection.ConstProtocVersion)]
    public Empty Clone() {
      return new Empty(this);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", global::Google.Protobuf.WellKnownTypes.EmptyReflection.ConstProtocVersion)]
    public override bool Equals(object other) {
      return Equals(other as Empty);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", global::Google.Protobuf.WellKnownTypes.EmptyReflection.ConstProtocVersion)]
    public bool Equals(Empty other) {
      if (ReferenceEquals(other, null)) {
        return false;
      }
      if (ReferenceEquals(other, this)) {
        return true;
      }
      return Equals(_unknownFields, other._unknownFields);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", global::Google.Protobuf.WellKnownTypes.EmptyReflection.ConstProtocVersion)]
    public override int GetHashCode() {
      int hash = 1;
      if (_unknownFields != null) {
        hash ^= _unknownFields.GetHashCode();
      }
      return hash;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", global::Google.Protobuf.WellKnownTypes.EmptyReflection.ConstProtocVersion)]
    public override string ToString() {
      return pb::JsonFormatter.ToDiagnosticString(this);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", global::Google.Protobuf.WellKnownTypes.EmptyReflection.ConstProtocVersion)]
    public void WriteTo(pb::CodedOutputStream output) {
    #if !GOOGLE_PROTOBUF_REFSTRUCT_COMPATIBILITY_MODE
      output.WriteRawMessage(this);
    #else
      if (_unknownFields != null) {
        _unknownFields.WriteTo(output);
      }
    #endif
    }

    #if !GOOGLE_PROTOBUF_REFSTRUCT_COMPATIBILITY_MODE
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", global::Google.Protobuf.WellKnownTypes.EmptyReflection.ConstProtocVersion)]
    void pb::IBufferMessage.InternalWriteTo(ref pb::WriteContext output) {
      if (_unknownFields != null) {
        _unknownFields.WriteTo(ref output);
      }
    }
    #endif

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", global::Google.Protobuf.WellKnownTypes.EmptyReflection.ConstProtocVersion)]
    public int CalculateSize() {
      int size = 0;
      if (_unknownFields != null) {
        size += _unknownFields.CalculateSize();
      }
      return size;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", global::Google.Protobuf.WellKnownTypes.EmptyReflection.ConstProtocVersion)]
    public void MergeFrom(Empty other) {
      if (other == null) {
        return;
      }
      _unknownFields = pb::UnknownFieldSet.MergeFrom(_unknownFields, other._unknownFields);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", global::Google.Protobuf.WellKnownTypes.EmptyReflection.ConstProtocVersion)]
    public void MergeFrom(pb::CodedInputStream input) {
    #if !GOOGLE_PROTOBUF_REFSTRUCT_COMPATIBILITY_MODE
      input.ReadRawMessage(this);
    #else
      uint tag;
      while ((tag = input.ReadTag()) != 0) {
        switch(tag) {
          default:
            _unknownFields = pb::UnknownFieldSet.MergeFieldFrom(_unknownFields, input);
            break;
        }
      }
    #endif
    }

    #if !GOOGLE_PROTOBUF_REFSTRUCT_COMPATIBILITY_MODE
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", global::Google.Protobuf.WellKnownTypes.EmptyReflection.ConstProtocVersion)]
    void pb::IBufferMessage.InternalMergeFrom(ref pb::ParseContext input) {
      uint tag;
      while ((tag = input.ReadTag()) != 0) {
        switch(tag) {
          default:
            _unknownFields = pb::UnknownFieldSet.MergeFieldFrom(_unknownFields, ref input);
            break;
        }
      }
    }
    #endif

  }

  #endregion

}

#endregion Designer generated code
