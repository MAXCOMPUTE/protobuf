// <auto-generated>
//     Generated by the protocol buffer compiler.  DO NOT EDIT!
//     source: unittest_issue6936_a.proto
// </auto-generated>
#pragma warning disable 1591, 0612, 3021, 8981
#region Designer generated code

using pb = global::Google.Protobuf;
using pbc = global::Google.Protobuf.Collections;
using pbr = global::Google.Protobuf.Reflection;
using scg = global::System.Collections.Generic;
namespace UnitTest.Issues.TestProtos {

  /// <summary>Holder for reflection information generated from unittest_issue6936_a.proto</summary>
  public static partial class UnittestIssue6936AReflection {

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

    #region Descriptor
    private static readonly global::System.Lazy<pbr::FileDescriptor> descriptor = new global::System.Lazy<pbr::FileDescriptor>(CreateFileDescriptor);
    /// <summary>File descriptor for unittest_issue6936_a.proto</summary>
    public static pbr::FileDescriptor Descriptor {
      get { return descriptor.Value; }
    }

    private static pbr::FileDescriptor CreateFileDescriptor() {
      pbr::RuntimeVersion.Validate(MinimumRuntimeVersion);
      byte[] descriptorData = global::System.Convert.FromBase64String(
          string.Concat(
            "Chp1bml0dGVzdF9pc3N1ZTY5MzZfYS5wcm90bxIPdW5pdHRlc3RfaXNzdWVz",
            "GiBnb29nbGUvcHJvdG9idWYvZGVzY3JpcHRvci5wcm90bzouCgNvcHQSHy5n",
            "b29nbGUucHJvdG9idWYuTWVzc2FnZU9wdGlvbnMY0IYDIAEoCUIdqgIaVW5p",
            "dFRlc3QuSXNzdWVzLlRlc3RQcm90b3NiBnByb3RvMw=="));
      return pbr::FileDescriptor.FromGeneratedCode(descriptorData,
          new pbr::FileDescriptor[] { global::Google.Protobuf.Reflection.DescriptorReflection.Descriptor, },
          new pbr::GeneratedClrTypeInfo(null, new pb::Extension[] { UnittestIssue6936AExtensions.Opt }, null));
    }
    #endregion

  }
  /// <summary>Holder for extension identifiers generated from the top level of unittest_issue6936_a.proto</summary>
  public static partial class UnittestIssue6936AExtensions {
    public static readonly pb::Extension<global::Google.Protobuf.Reflection.MessageOptions, string> Opt =
      new pb::Extension<global::Google.Protobuf.Reflection.MessageOptions, string>(50000, pb::FieldCodec.ForString(400002, ""));
  }

}

#endregion Designer generated code
