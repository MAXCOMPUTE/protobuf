// <auto-generated>
//     Generated by the protocol buffer compiler.  DO NOT EDIT!
//     source: csharp/protos/unittest_nrt_proto3.proto
// </auto-generated>
#nullable enable annotations
#pragma warning disable 1591, 0612, 3021, 8981
#region Designer generated code

using pb = global::Google.Protobuf;
using pbc = global::Google.Protobuf.Collections;
using pbr = global::Google.Protobuf.Reflection;
using scg = global::System.Collections.Generic;
namespace Google.Protobuf.TestProtos {

  /// <summary>Holder for reflection information generated from csharp/protos/unittest_nrt_proto3.proto</summary>
  public static partial class UnittestNrtProto3Reflection {

    #region Descriptor
    /// <summary>File descriptor for csharp/protos/unittest_nrt_proto3.proto</summary>
    public static pbr::FileDescriptor Descriptor {
      get { return descriptor; }
    }
    private static pbr::FileDescriptor descriptor;

    static UnittestNrtProto3Reflection() {
      byte[] descriptorData = global::System.Convert.FromBase64String(
          string.Concat(
            "Cidjc2hhcnAvcHJvdG9zL3VuaXR0ZXN0X25ydF9wcm90bzMucHJvdG8SEnBy",
            "b3RvYnVmX3VuaXR0ZXN0MyKjAQoOVGVzdE5ydE1lc3NhZ2USTgoNbWVzc2Fn",
            "ZV9maWVsZBgBIAEoCzI3LnByb3RvYnVmX3VuaXR0ZXN0My5UZXN0TnJ0TWVz",
            "c2FnZS5UZXN0TnJ0TmVzdGVkTWVzc2FnZRIUCgxzdHJpbmdfZmllbGQYAiAB",
            "KAkSEwoLaW50MzJfZmllbGQYAyABKAUaFgoUVGVzdE5ydE5lc3RlZE1lc3Nh",
            "Z2VCHaoCGkdvb2dsZS5Qcm90b2J1Zi5UZXN0UHJvdG9zYgZwcm90bzM="));
      descriptor = pbr::FileDescriptor.FromGeneratedCode(descriptorData,
          new pbr::FileDescriptor[] { },
          new pbr::GeneratedClrTypeInfo(null, null, new pbr::GeneratedClrTypeInfo[] {
            new pbr::GeneratedClrTypeInfo(typeof(global::Google.Protobuf.TestProtos.TestNrtMessage), global::Google.Protobuf.TestProtos.TestNrtMessage.Parser, new[]{ "MessageField", "StringField", "Int32Field" }, null, null, null, new pbr::GeneratedClrTypeInfo[] { new pbr::GeneratedClrTypeInfo(typeof(global::Google.Protobuf.TestProtos.TestNrtMessage.Types.TestNrtNestedMessage), global::Google.Protobuf.TestProtos.TestNrtMessage.Types.TestNrtNestedMessage.Parser, null, null, null, null, null)})
          }));
    }
    #endregion

  }
  #region Messages
  public sealed partial class TestNrtMessage : pb::IMessage<TestNrtMessage>
  #if !GOOGLE_PROTOBUF_REFSTRUCT_COMPATIBILITY_MODE
      , pb::IBufferMessage
  #endif
  {
    private static readonly pb::MessageParser<TestNrtMessage> _parser = new pb::MessageParser<TestNrtMessage>(() => new TestNrtMessage());
    private pb::UnknownFieldSet _unknownFields;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    public static pb::MessageParser<TestNrtMessage> Parser { get { return _parser; } }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    public static pbr::MessageDescriptor Descriptor {
      get { return global::Google.Protobuf.TestProtos.UnittestNrtProto3Reflection.Descriptor.MessageTypes[0]; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    pbr::MessageDescriptor pb::IMessage.Descriptor {
      get { return Descriptor; }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    public TestNrtMessage() {
      OnConstruction();
    }

    partial void OnConstruction();

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    public TestNrtMessage(TestNrtMessage other) : this() {
      messageField_ = other.messageField_ != null ? other.messageField_.Clone() : null;
      stringField_ = other.stringField_;
      int32Field_ = other.int32Field_;
      _unknownFields = pb::UnknownFieldSet.Clone(other._unknownFields);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    public TestNrtMessage Clone() {
      return new TestNrtMessage(this);
    }

    /// <summary>Field number for the "message_field" field.</summary>
    public const int MessageFieldFieldNumber = 1;
    private global::Google.Protobuf.TestProtos.TestNrtMessage.Types.TestNrtNestedMessage messageField_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    public global::Google.Protobuf.TestProtos.TestNrtMessage.Types.TestNrtNestedMessage? MessageField {
      get { return messageField_; }
      set {
        messageField_ = value;
      }
    }

    /// <summary>Field number for the "string_field" field.</summary>
    public const int StringFieldFieldNumber = 2;
    private string stringField_ = "";
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    public string StringField {
      get { return stringField_; }
      set {
        stringField_ = pb::ProtoPreconditions.CheckNotNull(value, "value");
      }
    }

    /// <summary>Field number for the "int32_field" field.</summary>
    public const int Int32FieldFieldNumber = 3;
    private int int32Field_;
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    public int Int32Field {
      get { return int32Field_; }
      set {
        int32Field_ = value;
      }
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    public override bool Equals(object other) {
      return Equals(other as TestNrtMessage);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    public bool Equals(TestNrtMessage other) {
      if (ReferenceEquals(other, null)) {
        return false;
      }
      if (ReferenceEquals(other, this)) {
        return true;
      }
      if (!object.Equals(MessageField, other.MessageField)) return false;
      if (StringField != other.StringField) return false;
      if (Int32Field != other.Int32Field) return false;
      return Equals(_unknownFields, other._unknownFields);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    public override int GetHashCode() {
      int hash = 1;
      if (messageField_ != null) hash ^= MessageField.GetHashCode();
      if (StringField.Length != 0) hash ^= StringField.GetHashCode();
      if (Int32Field != 0) hash ^= Int32Field.GetHashCode();
      if (_unknownFields != null) {
        hash ^= _unknownFields.GetHashCode();
      }
      return hash;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    public override string ToString() {
      return pb::JsonFormatter.ToDiagnosticString(this);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    public void WriteTo(pb::CodedOutputStream output) {
    #if !GOOGLE_PROTOBUF_REFSTRUCT_COMPATIBILITY_MODE
      output.WriteRawMessage(this);
    #else
      if (messageField_ != null) {
        output.WriteRawTag(10);
        output.WriteMessage(MessageField);
      }
      if (StringField.Length != 0) {
        output.WriteRawTag(18);
        output.WriteString(StringField);
      }
      if (Int32Field != 0) {
        output.WriteRawTag(24);
        output.WriteInt32(Int32Field);
      }
      if (_unknownFields != null) {
        _unknownFields.WriteTo(output);
      }
    #endif
    }

    #if !GOOGLE_PROTOBUF_REFSTRUCT_COMPATIBILITY_MODE
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    void pb::IBufferMessage.InternalWriteTo(ref pb::WriteContext output) {
      if (messageField_ != null) {
        output.WriteRawTag(10);
        output.WriteMessage(MessageField);
      }
      if (StringField.Length != 0) {
        output.WriteRawTag(18);
        output.WriteString(StringField);
      }
      if (Int32Field != 0) {
        output.WriteRawTag(24);
        output.WriteInt32(Int32Field);
      }
      if (_unknownFields != null) {
        _unknownFields.WriteTo(ref output);
      }
    }
    #endif

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    public int CalculateSize() {
      int size = 0;
      if (messageField_ != null) {
        size += 1 + pb::CodedOutputStream.ComputeMessageSize(MessageField);
      }
      if (StringField.Length != 0) {
        size += 1 + pb::CodedOutputStream.ComputeStringSize(StringField);
      }
      if (Int32Field != 0) {
        size += 1 + pb::CodedOutputStream.ComputeInt32Size(Int32Field);
      }
      if (_unknownFields != null) {
        size += _unknownFields.CalculateSize();
      }
      return size;
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    public void MergeFrom(TestNrtMessage other) {
      if (other == null) {
        return;
      }
      if (other.messageField_ != null) {
        if (messageField_ == null) {
          MessageField = new global::Google.Protobuf.TestProtos.TestNrtMessage.Types.TestNrtNestedMessage();
        }
        MessageField.MergeFrom(other.MessageField);
      }
      if (other.StringField.Length != 0) {
        StringField = other.StringField;
      }
      if (other.Int32Field != 0) {
        Int32Field = other.Int32Field;
      }
      _unknownFields = pb::UnknownFieldSet.MergeFrom(_unknownFields, other._unknownFields);
    }

    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
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
          case 10: {
            if (messageField_ == null) {
              MessageField = new global::Google.Protobuf.TestProtos.TestNrtMessage.Types.TestNrtNestedMessage();
            }
            input.ReadMessage(MessageField);
            break;
          }
          case 18: {
            StringField = input.ReadString();
            break;
          }
          case 24: {
            Int32Field = input.ReadInt32();
            break;
          }
        }
      }
    #endif
    }

    #if !GOOGLE_PROTOBUF_REFSTRUCT_COMPATIBILITY_MODE
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    void pb::IBufferMessage.InternalMergeFrom(ref pb::ParseContext input) {
      uint tag;
      while ((tag = input.ReadTag()) != 0) {
        switch(tag) {
          default:
            _unknownFields = pb::UnknownFieldSet.MergeFieldFrom(_unknownFields, ref input);
            break;
          case 10: {
            if (messageField_ == null) {
              MessageField = new global::Google.Protobuf.TestProtos.TestNrtMessage.Types.TestNrtNestedMessage();
            }
            input.ReadMessage(MessageField);
            break;
          }
          case 18: {
            StringField = input.ReadString();
            break;
          }
          case 24: {
            Int32Field = input.ReadInt32();
            break;
          }
        }
      }
    }
    #endif

    #region Nested types
    /// <summary>Container for nested types declared in the TestNrtMessage message type.</summary>
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
    [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
    public static partial class Types {
      public sealed partial class TestNrtNestedMessage : pb::IMessage<TestNrtNestedMessage>
      #if !GOOGLE_PROTOBUF_REFSTRUCT_COMPATIBILITY_MODE
          , pb::IBufferMessage
      #endif
      {
        private static readonly pb::MessageParser<TestNrtNestedMessage> _parser = new pb::MessageParser<TestNrtNestedMessage>(() => new TestNrtNestedMessage());
        private pb::UnknownFieldSet _unknownFields;
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
        [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
        public static pb::MessageParser<TestNrtNestedMessage> Parser { get { return _parser; } }

        [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
        [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
        public static pbr::MessageDescriptor Descriptor {
          get { return global::Google.Protobuf.TestProtos.TestNrtMessage.Descriptor.NestedTypes[0]; }
        }

        [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
        [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
        pbr::MessageDescriptor pb::IMessage.Descriptor {
          get { return Descriptor; }
        }

        [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
        [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
        public TestNrtNestedMessage() {
          OnConstruction();
        }

        partial void OnConstruction();

        [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
        [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
        public TestNrtNestedMessage(TestNrtNestedMessage other) : this() {
          _unknownFields = pb::UnknownFieldSet.Clone(other._unknownFields);
        }

        [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
        [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
        public TestNrtNestedMessage Clone() {
          return new TestNrtNestedMessage(this);
        }

        [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
        [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
        public override bool Equals(object other) {
          return Equals(other as TestNrtNestedMessage);
        }

        [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
        [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
        public bool Equals(TestNrtNestedMessage other) {
          if (ReferenceEquals(other, null)) {
            return false;
          }
          if (ReferenceEquals(other, this)) {
            return true;
          }
          return Equals(_unknownFields, other._unknownFields);
        }

        [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
        [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
        public override int GetHashCode() {
          int hash = 1;
          if (_unknownFields != null) {
            hash ^= _unknownFields.GetHashCode();
          }
          return hash;
        }

        [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
        [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
        public override string ToString() {
          return pb::JsonFormatter.ToDiagnosticString(this);
        }

        [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
        [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
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
        [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
        void pb::IBufferMessage.InternalWriteTo(ref pb::WriteContext output) {
          if (_unknownFields != null) {
            _unknownFields.WriteTo(ref output);
          }
        }
        #endif

        [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
        [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
        public int CalculateSize() {
          int size = 0;
          if (_unknownFields != null) {
            size += _unknownFields.CalculateSize();
          }
          return size;
        }

        [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
        [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
        public void MergeFrom(TestNrtNestedMessage other) {
          if (other == null) {
            return;
          }
          _unknownFields = pb::UnknownFieldSet.MergeFrom(_unknownFields, other._unknownFields);
        }

        [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
        [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
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
        [global::System.CodeDom.Compiler.GeneratedCode("protoc", null)]
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

    }
    #endregion

  }

  #endregion

}

#endregion Designer generated code
