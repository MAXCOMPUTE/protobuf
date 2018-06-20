// <auto-generated>
//     Generated by the protocol buffer compiler.  DO NOT EDIT!
//     source: conformance.proto
// </auto-generated>
#pragma warning disable 1591, 0612, 3021
#region Designer generated code

using pb = global::Google.Protobuf;
using pbc = global::Google.Protobuf.Collections;
using pbr = global::Google.Protobuf.Reflection;
using scg = global::System.Collections.Generic;
namespace Conformance {

  /// <summary>Holder for reflection information generated from conformance.proto</summary>
  public static partial class ConformanceReflection {

    #region Descriptor
    /// <summary>File descriptor for conformance.proto</summary>
    public static pbr::FileDescriptor Descriptor {
      get { return descriptor; }
    }
    private static pbr::FileDescriptor descriptor;

    static ConformanceReflection() {
      byte[] descriptorData = global::System.Convert.FromBase64String(
          string.Concat(
            "ChFjb25mb3JtYW5jZS5wcm90bxILY29uZm9ybWFuY2UiowEKEkNvbmZvcm1h",
            "bmNlUmVxdWVzdBIaChBwcm90b2J1Zl9wYXlsb2FkGAEgASgMSAASFgoManNv",
            "bl9wYXlsb2FkGAIgASgJSAASOAoXcmVxdWVzdGVkX291dHB1dF9mb3JtYXQY",
            "AyABKA4yFy5jb25mb3JtYW5jZS5XaXJlRm9ybWF0EhQKDG1lc3NhZ2VfdHlw",
            "ZRgEIAEoCUIJCgdwYXlsb2FkIrEBChNDb25mb3JtYW5jZVJlc3BvbnNlEhUK",
            "C3BhcnNlX2Vycm9yGAEgASgJSAASGQoPc2VyaWFsaXplX2Vycm9yGAYgASgJ",
            "SAASFwoNcnVudGltZV9lcnJvchgCIAEoCUgAEhoKEHByb3RvYnVmX3BheWxv",
            "YWQYAyABKAxIABIWCgxqc29uX3BheWxvYWQYBCABKAlIABIRCgdza2lwcGVk",
            "GAUgASgJSABCCAoGcmVzdWx0KjUKCldpcmVGb3JtYXQSDwoLVU5TUEVDSUZJ",
            "RUQQABIMCghQUk9UT0JVRhABEggKBEpTT04QAkIhCh9jb20uZ29vZ2xlLnBy",
            "b3RvYnVmLmNvbmZvcm1hbmNlYgZwcm90bzM="));
      descriptor = pbr::FileDescriptor.FromGeneratedCode(descriptorData,
          new pbr::FileDescriptor[] { },
          new pbr::GeneratedClrTypeInfo(new[] {typeof(global::Conformance.WireFormat), }, new pbr::GeneratedClrTypeInfo[] {
            new pbr::GeneratedClrTypeInfo(typeof(global::Conformance.ConformanceRequest), global::Conformance.ConformanceRequest.Parser, new[]{ "ProtobufPayload", "JsonPayload", "RequestedOutputFormat", "MessageType" }, new[]{ "Payload" }, null, null),
            new pbr::GeneratedClrTypeInfo(typeof(global::Conformance.ConformanceResponse), global::Conformance.ConformanceResponse.Parser, new[]{ "ParseError", "SerializeError", "RuntimeError", "ProtobufPayload", "JsonPayload", "Skipped" }, new[]{ "Result" }, null, null)
          }));
    }
    #endregion

  }
  #region Enums
  public enum WireFormat {
    [pbr::OriginalName("UNSPECIFIED")] Unspecified = 0,
    [pbr::OriginalName("PROTOBUF")] Protobuf = 1,
    [pbr::OriginalName("JSON")] Json = 2,
  }

  #endregion

  #region Messages
  /// <summary>
  /// Represents a single test case's input.  The testee should:
  ///
  ///   1. parse this proto (which should always succeed)
  ///   2. parse the protobuf or JSON payload in "payload" (which may fail)
  ///   3. if the parse succeeded, serialize the message in the requested format.
  /// </summary>
  public sealed partial class ConformanceRequest : pb::IMessage<ConformanceRequest> {
    private static readonly pb::MessageParser<ConformanceRequest> _parser = new pb::MessageParser<ConformanceRequest>(() => new ConformanceRequest());
    private pb::UnknownFieldSet _unknownFields;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pb::MessageParser<ConformanceRequest> Parser { get { return _parser; } }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pbr::MessageDescriptor Descriptor {
      get { return global::Conformance.ConformanceReflection.Descriptor.MessageTypes[0]; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    pbr::MessageDescriptor pb::IMessage.Descriptor {
      get { return Descriptor; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public ConformanceRequest() {
      OnConstruction();
    }

    partial void OnConstruction();

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public ConformanceRequest(ConformanceRequest other) : this() {
      requestedOutputFormat_ = other.requestedOutputFormat_;
      messageType_ = other.messageType_;
      switch (other.PayloadCase) {
        case PayloadOneofCase.ProtobufPayload:
          ProtobufPayload = other.ProtobufPayload;
          break;
        case PayloadOneofCase.JsonPayload:
          JsonPayload = other.JsonPayload;
          break;
      }

      _unknownFields = pb::UnknownFieldSet.Clone(other._unknownFields);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public ConformanceRequest Clone() {
      return new ConformanceRequest(this);
    }

    /// <summary>Field number for the "protobuf_payload" field.</summary>
    public const int ProtobufPayloadFieldNumber = 1;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public pb::ByteString ProtobufPayload {
      get { return payloadCase_ == PayloadOneofCase.ProtobufPayload ? (pb::ByteString) payload_ : pb::ByteString.Empty; }
      set {
        payload_ = pb::ProtoPreconditions.CheckNotNull(value, "value");
        payloadCase_ = PayloadOneofCase.ProtobufPayload;
      }
    }
    /// <summary> Clears the value of the oneof if it's currently set to "protobuf_payload" </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void ClearProtobufPayload() {
      if (payloadCase_ == PayloadOneofCase.ProtobufPayload) {
        ClearPayload();
      }
    }

    /// <summary>Field number for the "json_payload" field.</summary>
    public const int JsonPayloadFieldNumber = 2;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public string JsonPayload {
      get { return payloadCase_ == PayloadOneofCase.JsonPayload ? (string) payload_ : ""; }
      set {
        payload_ = pb::ProtoPreconditions.CheckNotNull(value, "value");
        payloadCase_ = PayloadOneofCase.JsonPayload;
      }
    }
    /// <summary> Clears the value of the oneof if it's currently set to "json_payload" </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void ClearJsonPayload() {
      if (payloadCase_ == PayloadOneofCase.JsonPayload) {
        ClearPayload();
      }
    }

    /// <summary>Field number for the "requested_output_format" field.</summary>
    public const int RequestedOutputFormatFieldNumber = 3;
    /// <summary>Default value for the "requested_output_format" field</summary>
    public const global::Conformance.WireFormat RequestedOutputFormatDefaultValue = 0;

    private global::Conformance.WireFormat requestedOutputFormat_ = RequestedOutputFormatDefaultValue;
    /// <summary>
    /// Which format should the testee serialize its message to?
    /// </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public global::Conformance.WireFormat RequestedOutputFormat {
      get { return requestedOutputFormat_; }
      set {
        requestedOutputFormat_ = value;
      }
    }
    /// <summary>Clears the value of the "requested_output_format" field</summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void ClearRequestedOutputFormat() {
      requestedOutputFormat_ = RequestedOutputFormatDefaultValue;
    }

    /// <summary>Field number for the "message_type" field.</summary>
    public const int MessageTypeFieldNumber = 4;
    /// <summary>Default value for the "message_type" field</summary>
    public const string MessageTypeDefaultValue = "";

    private string messageType_ = MessageTypeDefaultValue;
    /// <summary>
    /// The full name for the test message to use; for the moment, either:
    /// protobuf_test_messages.proto3.TestAllTypesProto3 or
    /// protobuf_test_messages.proto2.TestAllTypesProto2.
    /// </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public string MessageType {
      get { return messageType_; }
      set {
        messageType_ = pb::ProtoPreconditions.CheckNotNull(value, "value");
      }
    }
    /// <summary>Clears the value of the "message_type" field</summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void ClearMessageType() {
      messageType_ = MessageTypeDefaultValue;
    }

    private object payload_;
    /// <summary>Enum of possible cases for the "payload" oneof.</summary>
    public enum PayloadOneofCase {
      None = 0,
      ProtobufPayload = 1,
      JsonPayload = 2,
    }
    private PayloadOneofCase payloadCase_ = PayloadOneofCase.None;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public PayloadOneofCase PayloadCase {
      get { return payloadCase_; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void ClearPayload() {
      payloadCase_ = PayloadOneofCase.None;
      payload_ = null;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override bool Equals(object other) {
      return Equals(other as ConformanceRequest);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public bool Equals(ConformanceRequest other) {
      if (ReferenceEquals(other, null)) {
        return false;
      }
      if (ReferenceEquals(other, this)) {
        return true;
      }
      if (ProtobufPayload != other.ProtobufPayload) return false;
      if (JsonPayload != other.JsonPayload) return false;
      if (RequestedOutputFormat != other.RequestedOutputFormat) return false;
      if (MessageType != other.MessageType) return false;
      if (PayloadCase != other.PayloadCase) return false;
      return Equals(_unknownFields, other._unknownFields);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override int GetHashCode() {
      int hash = 1;
      if (payloadCase_ == PayloadOneofCase.ProtobufPayload) hash ^= ProtobufPayload.GetHashCode();
      if (payloadCase_ == PayloadOneofCase.JsonPayload) hash ^= JsonPayload.GetHashCode();
      if (RequestedOutputFormat != 0) hash ^= RequestedOutputFormat.GetHashCode();
      if (MessageType.Length != 0) hash ^= MessageType.GetHashCode();
      hash ^= (int) payloadCase_;
      if (_unknownFields != null) {
        hash ^= _unknownFields.GetHashCode();
      }
      return hash;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override string ToString() {
      return pb::JsonFormatter.ToDiagnosticString(this);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void WriteTo(pb::CodedOutputStream output) {
      if (payloadCase_ == PayloadOneofCase.ProtobufPayload) {
        output.WriteRawTag(10);
        output.WriteBytes(ProtobufPayload);
      }
      if (payloadCase_ == PayloadOneofCase.JsonPayload) {
        output.WriteRawTag(18);
        output.WriteString(JsonPayload);
      }
      if (RequestedOutputFormat != 0) {
        output.WriteRawTag(24);
        output.WriteEnum((int) RequestedOutputFormat);
      }
      if (MessageType.Length != 0) {
        output.WriteRawTag(34);
        output.WriteString(MessageType);
      }
      if (_unknownFields != null) {
        _unknownFields.WriteTo(output);
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int CalculateSize() {
      int size = 0;
      if (payloadCase_ == PayloadOneofCase.ProtobufPayload) {
        size += 1 + pb::CodedOutputStream.ComputeBytesSize(ProtobufPayload);
      }
      if (payloadCase_ == PayloadOneofCase.JsonPayload) {
        size += 1 + pb::CodedOutputStream.ComputeStringSize(JsonPayload);
      }
      if (RequestedOutputFormat != 0) {
        size += 1 + pb::CodedOutputStream.ComputeEnumSize((int) RequestedOutputFormat);
      }
      if (MessageType.Length != 0) {
        size += 1 + pb::CodedOutputStream.ComputeStringSize(MessageType);
      }
      if (_unknownFields != null) {
        size += _unknownFields.CalculateSize();
      }
      return size;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(ConformanceRequest other) {
      if (other == null) {
        return;
      }
      if (other.RequestedOutputFormat != 0) {
        RequestedOutputFormat = other.RequestedOutputFormat;
      }
      if (other.MessageType.Length != 0) {
        MessageType = other.MessageType;
      }
      switch (other.PayloadCase) {
        case PayloadOneofCase.ProtobufPayload:
          ProtobufPayload = other.ProtobufPayload;
          break;
        case PayloadOneofCase.JsonPayload:
          JsonPayload = other.JsonPayload;
          break;
      }

      _unknownFields = pb::UnknownFieldSet.MergeFrom(_unknownFields, other._unknownFields);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(pb::CodedInputStream input) {
      uint tag;
      while ((tag = input.ReadTag()) != 0) {
        switch(tag) {
          default:
            _unknownFields = pb::UnknownFieldSet.MergeFieldFrom(_unknownFields, input);
            break;
          case 10: {
            ProtobufPayload = input.ReadBytes();
            break;
          }
          case 18: {
            JsonPayload = input.ReadString();
            break;
          }
          case 24: {
            requestedOutputFormat_ = (global::Conformance.WireFormat) input.ReadEnum();
            break;
          }
          case 34: {
            MessageType = input.ReadString();
            break;
          }
        }
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public bool IsInitialized() {
      return true;
    }
  }

  /// <summary>
  /// Represents a single test case's output.
  /// </summary>
  public sealed partial class ConformanceResponse : pb::IMessage<ConformanceResponse> {
    private static readonly pb::MessageParser<ConformanceResponse> _parser = new pb::MessageParser<ConformanceResponse>(() => new ConformanceResponse());
    private pb::UnknownFieldSet _unknownFields;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pb::MessageParser<ConformanceResponse> Parser { get { return _parser; } }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public static pbr::MessageDescriptor Descriptor {
      get { return global::Conformance.ConformanceReflection.Descriptor.MessageTypes[1]; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    pbr::MessageDescriptor pb::IMessage.Descriptor {
      get { return Descriptor; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public ConformanceResponse() {
      OnConstruction();
    }

    partial void OnConstruction();

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public ConformanceResponse(ConformanceResponse other) : this() {
      switch (other.ResultCase) {
        case ResultOneofCase.ParseError:
          ParseError = other.ParseError;
          break;
        case ResultOneofCase.SerializeError:
          SerializeError = other.SerializeError;
          break;
        case ResultOneofCase.RuntimeError:
          RuntimeError = other.RuntimeError;
          break;
        case ResultOneofCase.ProtobufPayload:
          ProtobufPayload = other.ProtobufPayload;
          break;
        case ResultOneofCase.JsonPayload:
          JsonPayload = other.JsonPayload;
          break;
        case ResultOneofCase.Skipped:
          Skipped = other.Skipped;
          break;
      }

      _unknownFields = pb::UnknownFieldSet.Clone(other._unknownFields);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public ConformanceResponse Clone() {
      return new ConformanceResponse(this);
    }

    /// <summary>Field number for the "parse_error" field.</summary>
    public const int ParseErrorFieldNumber = 1;
    /// <summary>
    /// This string should be set to indicate parsing failed.  The string can
    /// provide more information about the parse error if it is available.
    ///
    /// Setting this string does not necessarily mean the testee failed the
    /// test.  Some of the test cases are intentionally invalid input.
    /// </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public string ParseError {
      get { return resultCase_ == ResultOneofCase.ParseError ? (string) result_ : ""; }
      set {
        result_ = pb::ProtoPreconditions.CheckNotNull(value, "value");
        resultCase_ = ResultOneofCase.ParseError;
      }
    }
    /// <summary> Clears the value of the oneof if it's currently set to "parse_error" </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void ClearParseError() {
      if (resultCase_ == ResultOneofCase.ParseError) {
        ClearResult();
      }
    }

    /// <summary>Field number for the "serialize_error" field.</summary>
    public const int SerializeErrorFieldNumber = 6;
    /// <summary>
    /// If the input was successfully parsed but errors occurred when
    /// serializing it to the requested output format, set the error message in
    /// this field.
    /// </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public string SerializeError {
      get { return resultCase_ == ResultOneofCase.SerializeError ? (string) result_ : ""; }
      set {
        result_ = pb::ProtoPreconditions.CheckNotNull(value, "value");
        resultCase_ = ResultOneofCase.SerializeError;
      }
    }
    /// <summary> Clears the value of the oneof if it's currently set to "serialize_error" </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void ClearSerializeError() {
      if (resultCase_ == ResultOneofCase.SerializeError) {
        ClearResult();
      }
    }

    /// <summary>Field number for the "runtime_error" field.</summary>
    public const int RuntimeErrorFieldNumber = 2;
    /// <summary>
    /// This should be set if some other error occurred.  This will always
    /// indicate that the test failed.  The string can provide more information
    /// about the failure.
    /// </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public string RuntimeError {
      get { return resultCase_ == ResultOneofCase.RuntimeError ? (string) result_ : ""; }
      set {
        result_ = pb::ProtoPreconditions.CheckNotNull(value, "value");
        resultCase_ = ResultOneofCase.RuntimeError;
      }
    }
    /// <summary> Clears the value of the oneof if it's currently set to "runtime_error" </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void ClearRuntimeError() {
      if (resultCase_ == ResultOneofCase.RuntimeError) {
        ClearResult();
      }
    }

    /// <summary>Field number for the "protobuf_payload" field.</summary>
    public const int ProtobufPayloadFieldNumber = 3;
    /// <summary>
    /// If the input was successfully parsed and the requested output was
    /// protobuf, serialize it to protobuf and set it in this field.
    /// </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public pb::ByteString ProtobufPayload {
      get { return resultCase_ == ResultOneofCase.ProtobufPayload ? (pb::ByteString) result_ : pb::ByteString.Empty; }
      set {
        result_ = pb::ProtoPreconditions.CheckNotNull(value, "value");
        resultCase_ = ResultOneofCase.ProtobufPayload;
      }
    }
    /// <summary> Clears the value of the oneof if it's currently set to "protobuf_payload" </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void ClearProtobufPayload() {
      if (resultCase_ == ResultOneofCase.ProtobufPayload) {
        ClearResult();
      }
    }

    /// <summary>Field number for the "json_payload" field.</summary>
    public const int JsonPayloadFieldNumber = 4;
    /// <summary>
    /// If the input was successfully parsed and the requested output was JSON,
    /// serialize to JSON and set it in this field.
    /// </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public string JsonPayload {
      get { return resultCase_ == ResultOneofCase.JsonPayload ? (string) result_ : ""; }
      set {
        result_ = pb::ProtoPreconditions.CheckNotNull(value, "value");
        resultCase_ = ResultOneofCase.JsonPayload;
      }
    }
    /// <summary> Clears the value of the oneof if it's currently set to "json_payload" </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void ClearJsonPayload() {
      if (resultCase_ == ResultOneofCase.JsonPayload) {
        ClearResult();
      }
    }

    /// <summary>Field number for the "skipped" field.</summary>
    public const int SkippedFieldNumber = 5;
    /// <summary>
    /// For when the testee skipped the test, likely because a certain feature
    /// wasn't supported, like JSON input/output.
    /// </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public string Skipped {
      get { return resultCase_ == ResultOneofCase.Skipped ? (string) result_ : ""; }
      set {
        result_ = pb::ProtoPreconditions.CheckNotNull(value, "value");
        resultCase_ = ResultOneofCase.Skipped;
      }
    }
    /// <summary> Clears the value of the oneof if it's currently set to "skipped" </summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void ClearSkipped() {
      if (resultCase_ == ResultOneofCase.Skipped) {
        ClearResult();
      }
    }

    private object result_;
    /// <summary>Enum of possible cases for the "result" oneof.</summary>
    public enum ResultOneofCase {
      None = 0,
      ParseError = 1,
      SerializeError = 6,
      RuntimeError = 2,
      ProtobufPayload = 3,
      JsonPayload = 4,
      Skipped = 5,
    }
    private ResultOneofCase resultCase_ = ResultOneofCase.None;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public ResultOneofCase ResultCase {
      get { return resultCase_; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void ClearResult() {
      resultCase_ = ResultOneofCase.None;
      result_ = null;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override bool Equals(object other) {
      return Equals(other as ConformanceResponse);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public bool Equals(ConformanceResponse other) {
      if (ReferenceEquals(other, null)) {
        return false;
      }
      if (ReferenceEquals(other, this)) {
        return true;
      }
      if (ParseError != other.ParseError) return false;
      if (SerializeError != other.SerializeError) return false;
      if (RuntimeError != other.RuntimeError) return false;
      if (ProtobufPayload != other.ProtobufPayload) return false;
      if (JsonPayload != other.JsonPayload) return false;
      if (Skipped != other.Skipped) return false;
      if (ResultCase != other.ResultCase) return false;
      return Equals(_unknownFields, other._unknownFields);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override int GetHashCode() {
      int hash = 1;
      if (resultCase_ == ResultOneofCase.ParseError) hash ^= ParseError.GetHashCode();
      if (resultCase_ == ResultOneofCase.SerializeError) hash ^= SerializeError.GetHashCode();
      if (resultCase_ == ResultOneofCase.RuntimeError) hash ^= RuntimeError.GetHashCode();
      if (resultCase_ == ResultOneofCase.ProtobufPayload) hash ^= ProtobufPayload.GetHashCode();
      if (resultCase_ == ResultOneofCase.JsonPayload) hash ^= JsonPayload.GetHashCode();
      if (resultCase_ == ResultOneofCase.Skipped) hash ^= Skipped.GetHashCode();
      hash ^= (int) resultCase_;
      if (_unknownFields != null) {
        hash ^= _unknownFields.GetHashCode();
      }
      return hash;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public override string ToString() {
      return pb::JsonFormatter.ToDiagnosticString(this);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void WriteTo(pb::CodedOutputStream output) {
      if (resultCase_ == ResultOneofCase.ParseError) {
        output.WriteRawTag(10);
        output.WriteString(ParseError);
      }
      if (resultCase_ == ResultOneofCase.RuntimeError) {
        output.WriteRawTag(18);
        output.WriteString(RuntimeError);
      }
      if (resultCase_ == ResultOneofCase.ProtobufPayload) {
        output.WriteRawTag(26);
        output.WriteBytes(ProtobufPayload);
      }
      if (resultCase_ == ResultOneofCase.JsonPayload) {
        output.WriteRawTag(34);
        output.WriteString(JsonPayload);
      }
      if (resultCase_ == ResultOneofCase.Skipped) {
        output.WriteRawTag(42);
        output.WriteString(Skipped);
      }
      if (resultCase_ == ResultOneofCase.SerializeError) {
        output.WriteRawTag(50);
        output.WriteString(SerializeError);
      }
      if (_unknownFields != null) {
        _unknownFields.WriteTo(output);
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public int CalculateSize() {
      int size = 0;
      if (resultCase_ == ResultOneofCase.ParseError) {
        size += 1 + pb::CodedOutputStream.ComputeStringSize(ParseError);
      }
      if (resultCase_ == ResultOneofCase.SerializeError) {
        size += 1 + pb::CodedOutputStream.ComputeStringSize(SerializeError);
      }
      if (resultCase_ == ResultOneofCase.RuntimeError) {
        size += 1 + pb::CodedOutputStream.ComputeStringSize(RuntimeError);
      }
      if (resultCase_ == ResultOneofCase.ProtobufPayload) {
        size += 1 + pb::CodedOutputStream.ComputeBytesSize(ProtobufPayload);
      }
      if (resultCase_ == ResultOneofCase.JsonPayload) {
        size += 1 + pb::CodedOutputStream.ComputeStringSize(JsonPayload);
      }
      if (resultCase_ == ResultOneofCase.Skipped) {
        size += 1 + pb::CodedOutputStream.ComputeStringSize(Skipped);
      }
      if (_unknownFields != null) {
        size += _unknownFields.CalculateSize();
      }
      return size;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(ConformanceResponse other) {
      if (other == null) {
        return;
      }
      switch (other.ResultCase) {
        case ResultOneofCase.ParseError:
          ParseError = other.ParseError;
          break;
        case ResultOneofCase.SerializeError:
          SerializeError = other.SerializeError;
          break;
        case ResultOneofCase.RuntimeError:
          RuntimeError = other.RuntimeError;
          break;
        case ResultOneofCase.ProtobufPayload:
          ProtobufPayload = other.ProtobufPayload;
          break;
        case ResultOneofCase.JsonPayload:
          JsonPayload = other.JsonPayload;
          break;
        case ResultOneofCase.Skipped:
          Skipped = other.Skipped;
          break;
      }

      _unknownFields = pb::UnknownFieldSet.MergeFrom(_unknownFields, other._unknownFields);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public void MergeFrom(pb::CodedInputStream input) {
      uint tag;
      while ((tag = input.ReadTag()) != 0) {
        switch(tag) {
          default:
            _unknownFields = pb::UnknownFieldSet.MergeFieldFrom(_unknownFields, input);
            break;
          case 10: {
            ParseError = input.ReadString();
            break;
          }
          case 18: {
            RuntimeError = input.ReadString();
            break;
          }
          case 26: {
            ProtobufPayload = input.ReadBytes();
            break;
          }
          case 34: {
            JsonPayload = input.ReadString();
            break;
          }
          case 42: {
            Skipped = input.ReadString();
            break;
          }
          case 50: {
            SerializeError = input.ReadString();
            break;
          }
        }
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    public bool IsInitialized() {
      return true;
    }
  }

  #endregion

}

#endregion Designer generated code
