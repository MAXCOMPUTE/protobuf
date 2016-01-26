// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
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

package com.google.protobuf;

import static java.util.Collections.singletonList;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertSame;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import com.google.protobuf.DescriptorProtos.DescriptorProto;
import com.google.protobuf.DescriptorProtos.EnumDescriptorProto;
import com.google.protobuf.DescriptorProtos.EnumValueDescriptorProto;
import com.google.protobuf.DescriptorProtos.FieldDescriptorProto;
import com.google.protobuf.DescriptorProtos.FileDescriptorProto;
import com.google.protobuf.Descriptors.Descriptor;
import com.google.protobuf.Descriptors.DescriptorValidationException;
import com.google.protobuf.Descriptors.EnumDescriptor;
import com.google.protobuf.Descriptors.EnumValueDescriptor;
import com.google.protobuf.Descriptors.FieldDescriptor;
import com.google.protobuf.Descriptors.FileDescriptor;
import com.google.protobuf.Descriptors.MethodDescriptor;
import com.google.protobuf.Descriptors.OneofDescriptor;
import com.google.protobuf.Descriptors.ServiceDescriptor;
import com.google.protobuf.test.UnittestImport;
import com.google.protobuf.test.UnittestImport.ImportEnum;
import com.google.protobuf.test.UnittestImport.ImportEnumForMap;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;

import protobuf_unittest.TestCustomOptions;
import protobuf_unittest.UnittestCustomOptions;
import protobuf_unittest.UnittestProto;
import protobuf_unittest.UnittestProto.ForeignEnum;
import protobuf_unittest.UnittestProto.ForeignMessage;
import protobuf_unittest.UnittestProto.TestAllExtensions;
import protobuf_unittest.UnittestProto.TestAllTypes;
import protobuf_unittest.UnittestProto.TestExtremeDefaultValues;
import protobuf_unittest.UnittestProto.TestMultipleExtensionRanges;
import protobuf_unittest.UnittestProto.TestRequired;
import protobuf_unittest.UnittestProto.TestReservedFields;
import protobuf_unittest.UnittestProto.TestService;

import java.util.Arrays;
import java.util.Collections;
import java.util.List;

/**
 * Unit test for {@link Descriptors}.
 *
 * @author kenton@google.com Kenton Varda
 */
@RunWith(JUnit4.class)
public class DescriptorsTest {

  // Regression test for bug where referencing a FieldDescriptor.Type value
  // before a FieldDescriptorProto.Type value would yield a
  // ExceptionInInitializerError.
  @SuppressWarnings("unused")
  private static final Object STATIC_INIT_TEST = FieldDescriptor.Type.BOOL;

  @Test
  public void testFieldTypeEnumMapping() throws Exception {
    assertEquals(FieldDescriptor.Type.values().length,
        FieldDescriptorProto.Type.values().length);
    for (FieldDescriptor.Type type : FieldDescriptor.Type.values()) {
      FieldDescriptorProto.Type protoType = type.toProto();
      assertEquals("TYPE_" + type.name(), protoType.name());
      assertEquals(type, FieldDescriptor.Type.valueOf(protoType));
    }
  }

  @Test
  public void testFileDescriptor() throws Exception {
    FileDescriptor file = UnittestProto.getDescriptor();

    assertEquals("google/protobuf/unittest.proto", file.getName());
    assertEquals("protobuf_unittest", file.getPackage());

    assertEquals("UnittestProto", file.getOptions().getJavaOuterClassname());
    assertEquals("google/protobuf/unittest.proto",
                 file.toProto().getName());

    assertEquals(singletonList(UnittestImport.getDescriptor()),
                 file.getDependencies());

    Descriptor messageType = TestAllTypes.getDescriptor();
    assertEquals(messageType, file.getMessageTypes().get(0));
    assertEquals(messageType, file.findMessageTypeByName("TestAllTypes"));
    assertNull(file.findMessageTypeByName("NoSuchType"));
    assertNull(file.findMessageTypeByName("protobuf_unittest.TestAllTypes"));
    for (int i = 0; i < file.getMessageTypes().size(); i++) {
      assertEquals(i, file.getMessageTypes().get(i).getIndex());
    }

    EnumDescriptor enumType = ForeignEnum.getDescriptor();
    assertEquals(enumType, file.getEnumTypes().get(0));
    assertEquals(enumType, file.findEnumTypeByName("ForeignEnum"));
    assertNull(file.findEnumTypeByName("NoSuchType"));
    assertNull(file.findEnumTypeByName("protobuf_unittest.ForeignEnum"));
    assertEquals(Arrays.asList(ImportEnum.getDescriptor(),
                               ImportEnumForMap.getDescriptor()),
                 UnittestImport.getDescriptor().getEnumTypes());
    for (int i = 0; i < file.getEnumTypes().size(); i++) {
      assertEquals(i, file.getEnumTypes().get(i).getIndex());
    }

    ServiceDescriptor service = TestService.getDescriptor();
    assertEquals(service, file.getServices().get(0));
    assertEquals(service, file.findServiceByName("TestService"));
    assertNull(file.findServiceByName("NoSuchType"));
    assertNull(file.findServiceByName("protobuf_unittest.TestService"));
    assertEquals(Collections.emptyList(),
                 UnittestImport.getDescriptor().getServices());
    for (int i = 0; i < file.getServices().size(); i++) {
      assertEquals(i, file.getServices().get(i).getIndex());
    }

    FieldDescriptor extension =
      UnittestProto.optionalInt32Extension.getDescriptor();
    assertEquals(extension, file.getExtensions().get(0));
    assertEquals(extension,
                 file.findExtensionByName("optional_int32_extension"));
    assertNull(file.findExtensionByName("no_such_ext"));
    assertNull(file.findExtensionByName(
      "protobuf_unittest.optional_int32_extension"));
    assertEquals(Collections.emptyList(),
                 UnittestImport.getDescriptor().getExtensions());
    for (int i = 0; i < file.getExtensions().size(); i++) {
      assertEquals(i, file.getExtensions().get(i).getIndex());
    }
  }

  @Test
  public void testDescriptor() throws Exception {
    Descriptor messageType = TestAllTypes.getDescriptor();
    Descriptor nestedType = TestAllTypes.NestedMessage.getDescriptor();

    assertEquals("TestAllTypes", messageType.getName());
    assertEquals("protobuf_unittest.TestAllTypes", messageType.getFullName());
    assertEquals(UnittestProto.getDescriptor(), messageType.getFile());
    assertNull(messageType.getContainingType());
    assertEquals(DescriptorProtos.MessageOptions.getDefaultInstance(),
                 messageType.getOptions());
    assertEquals("TestAllTypes", messageType.toProto().getName());

    assertEquals("NestedMessage", nestedType.getName());
    assertEquals("protobuf_unittest.TestAllTypes.NestedMessage",
                 nestedType.getFullName());
    assertEquals(UnittestProto.getDescriptor(), nestedType.getFile());
    assertEquals(messageType, nestedType.getContainingType());

    FieldDescriptor field = messageType.getFields().get(0);
    assertEquals("optional_int32", field.getName());
    assertEquals(field, messageType.findFieldByName("optional_int32"));
    assertNull(messageType.findFieldByName("no_such_field"));
    assertEquals(field, messageType.findFieldByNumber(1));
    assertNull(messageType.findFieldByNumber(571283));
    for (int i = 0; i < messageType.getFields().size(); i++) {
      assertEquals(i, messageType.getFields().get(i).getIndex());
    }

    assertEquals(nestedType, messageType.getNestedTypes().get(0));
    assertEquals(nestedType, messageType.findNestedTypeByName("NestedMessage"));
    assertNull(messageType.findNestedTypeByName("NoSuchType"));
    for (int i = 0; i < messageType.getNestedTypes().size(); i++) {
      assertEquals(i, messageType.getNestedTypes().get(i).getIndex());
    }

    EnumDescriptor enumType = TestAllTypes.NestedEnum.getDescriptor();
    assertEquals(enumType, messageType.getEnumTypes().get(0));
    assertEquals(enumType, messageType.findEnumTypeByName("NestedEnum"));
    assertNull(messageType.findEnumTypeByName("NoSuchType"));
    for (int i = 0; i < messageType.getEnumTypes().size(); i++) {
      assertEquals(i, messageType.getEnumTypes().get(i).getIndex());
    }
  }

  @Test
  public void testFieldDescriptor() throws Exception {
    Descriptor messageType = TestAllTypes.getDescriptor();
    FieldDescriptor primitiveField =
      messageType.findFieldByName("optional_int32");
    FieldDescriptor enumField =
      messageType.findFieldByName("optional_nested_enum");
    FieldDescriptor messageField =
      messageType.findFieldByName("optional_foreign_message");
    FieldDescriptor cordField =
      messageType.findFieldByName("optional_cord");
    FieldDescriptor extension =
      UnittestProto.optionalInt32Extension.getDescriptor();
    FieldDescriptor nestedExtension = TestRequired.single.getDescriptor();

    assertNotNull(primitiveField);
    assertNotNull(enumField);
    assertNotNull(messageField);
    assertNotNull(cordField);

    assertEquals("optional_int32", primitiveField.getName());
    assertEquals("protobuf_unittest.TestAllTypes.optional_int32",
                 primitiveField.getFullName());
    assertEquals(1, primitiveField.getNumber());
    assertEquals(messageType, primitiveField.getContainingType());
    assertEquals(UnittestProto.getDescriptor(), primitiveField.getFile());
    assertEquals(FieldDescriptor.Type.INT32, primitiveField.getType());
    assertEquals(FieldDescriptor.JavaType.INT, primitiveField.getJavaType());
    assertEquals(DescriptorProtos.FieldOptions.getDefaultInstance(),
                 primitiveField.getOptions());
    assertFalse(primitiveField.isExtension());
    assertEquals("optional_int32", primitiveField.toProto().getName());

    assertEquals("optional_nested_enum", enumField.getName());
    assertEquals(FieldDescriptor.Type.ENUM, enumField.getType());
    assertEquals(FieldDescriptor.JavaType.ENUM, enumField.getJavaType());
    assertEquals(TestAllTypes.NestedEnum.getDescriptor(),
                 enumField.getEnumType());

    assertEquals("optional_foreign_message", messageField.getName());
    assertEquals(FieldDescriptor.Type.MESSAGE, messageField.getType());
    assertEquals(FieldDescriptor.JavaType.MESSAGE, messageField.getJavaType());
    assertEquals(ForeignMessage.getDescriptor(), messageField.getMessageType());

    assertEquals("optional_cord", cordField.getName());
    assertEquals(FieldDescriptor.Type.STRING, cordField.getType());
    assertEquals(FieldDescriptor.JavaType.STRING, cordField.getJavaType());
    assertEquals(DescriptorProtos.FieldOptions.CType.CORD,
                 cordField.getOptions().getCtype());

    assertEquals("optional_int32_extension", extension.getName());
    assertEquals("protobuf_unittest.optional_int32_extension",
                 extension.getFullName());
    assertEquals(1, extension.getNumber());
    assertEquals(TestAllExtensions.getDescriptor(),
                 extension.getContainingType());
    assertEquals(UnittestProto.getDescriptor(), extension.getFile());
    assertEquals(FieldDescriptor.Type.INT32, extension.getType());
    assertEquals(FieldDescriptor.JavaType.INT, extension.getJavaType());
    assertEquals(DescriptorProtos.FieldOptions.getDefaultInstance(),
                 extension.getOptions());
    assertTrue(extension.isExtension());
    assertEquals(null, extension.getExtensionScope());
    assertEquals("optional_int32_extension", extension.toProto().getName());

    assertEquals("single", nestedExtension.getName());
    assertEquals("protobuf_unittest.TestRequired.single",
                 nestedExtension.getFullName());
    assertEquals(TestRequired.getDescriptor(),
                 nestedExtension.getExtensionScope());
  }

  @Test
  public void testFieldDescriptorLabel() throws Exception {
    FieldDescriptor requiredField =
      TestRequired.getDescriptor().findFieldByName("a");
    FieldDescriptor optionalField =
      TestAllTypes.getDescriptor().findFieldByName("optional_int32");
    FieldDescriptor repeatedField =
      TestAllTypes.getDescriptor().findFieldByName("repeated_int32");
    assertNotNull(requiredField);
    assertNotNull(optionalField);
    assertNotNull(repeatedField);
    assertTrue(requiredField.isRequired());
    assertFalse(requiredField.isRepeated());
    assertFalse(optionalField.isRequired());
    assertFalse(optionalField.isRepeated());
    assertFalse(repeatedField.isRequired());
    assertTrue(repeatedField.isRepeated());
  }

  @Test
  public void testFieldDescriptorJsonName() throws Exception {
    FieldDescriptor requiredField = TestRequired.getDescriptor().findFieldByName("a");
    FieldDescriptor optionalField = TestAllTypes.getDescriptor().findFieldByName("optional_int32");
    FieldDescriptor repeatedField = TestAllTypes.getDescriptor().findFieldByName("repeated_int32");
    assertNotNull(requiredField);
    assertNotNull(optionalField);
    assertNotNull(repeatedField);
    assertEquals("a", requiredField.getJsonName());
    assertEquals("optionalInt32", optionalField.getJsonName());
    assertEquals("repeatedInt32", repeatedField.getJsonName());
  }

  @Test
  public void testFieldDescriptorDefault() throws Exception {
    Descriptor d = TestAllTypes.getDescriptor();
    FieldDescriptor optionalField = d.findFieldByName("optional_int32");
    FieldDescriptor defaultField = d.findFieldByName("default_int32");
    assertNotNull(optionalField);
    assertNotNull(defaultField);
    assertFalse(optionalField.hasDefaultValue());
    assertEquals(0, optionalField.getDefaultValue());
    assertTrue(defaultField.hasDefaultValue());
    assertEquals(41, defaultField.getDefaultValue());

    d = TestExtremeDefaultValues.getDescriptor();
    FieldDescriptor escapedField = d.findFieldByName("escaped_bytes");
    FieldDescriptor large32Field = d.findFieldByName("large_uint32");
    FieldDescriptor large64Field = d.findFieldByName("large_uint64");
    assertNotNull(escapedField);
    assertNotNull(large32Field);
    assertNotNull(large64Field);
    assertEquals(
      ByteString.copyFrom(
        "\0\001\007\b\f\n\r\t\013\\\'\"\u00fe".getBytes(Internal.ISO_8859_1)),
      escapedField.getDefaultValue());
    assertEquals(-1, large32Field.getDefaultValue());
    assertEquals(-1L, large64Field.getDefaultValue());
  }

  @Test
  public void testEnumDescriptor() throws Exception {
    EnumDescriptor enumType = ForeignEnum.getDescriptor();
    EnumDescriptor nestedType = TestAllTypes.NestedEnum.getDescriptor();

    assertEquals("ForeignEnum", enumType.getName());
    assertEquals("protobuf_unittest.ForeignEnum", enumType.getFullName());
    assertEquals(UnittestProto.getDescriptor(), enumType.getFile());
    assertNull(enumType.getContainingType());
    assertEquals(DescriptorProtos.EnumOptions.getDefaultInstance(),
                 enumType.getOptions());

    assertEquals("NestedEnum", nestedType.getName());
    assertEquals("protobuf_unittest.TestAllTypes.NestedEnum",
                 nestedType.getFullName());
    assertEquals(UnittestProto.getDescriptor(), nestedType.getFile());
    assertEquals(TestAllTypes.getDescriptor(), nestedType.getContainingType());

    EnumValueDescriptor value = ForeignEnum.FOREIGN_FOO.getValueDescriptor();
    assertEquals(value, enumType.getValues().get(0));
    assertEquals("FOREIGN_FOO", value.getName());
    assertEquals("FOREIGN_FOO", value.toString());
    assertEquals(4, value.getNumber());
    assertEquals(value, enumType.findValueByName("FOREIGN_FOO"));
    assertEquals(value, enumType.findValueByNumber(4));
    assertNull(enumType.findValueByName("NO_SUCH_VALUE"));
    for (int i = 0; i < enumType.getValues().size(); i++) {
      assertEquals(i, enumType.getValues().get(i).getIndex());
    }
  }

  @Test
  public void testServiceDescriptor() throws Exception {
    ServiceDescriptor service = TestService.getDescriptor();

    assertEquals("TestService", service.getName());
    assertEquals("protobuf_unittest.TestService", service.getFullName());
    assertEquals(UnittestProto.getDescriptor(), service.getFile());


    MethodDescriptor fooMethod = service.getMethods().get(0);
    assertEquals("Foo", fooMethod.getName());
    assertEquals(UnittestProto.FooRequest.getDescriptor(),
                 fooMethod.getInputType());
    assertEquals(UnittestProto.FooResponse.getDescriptor(),
                 fooMethod.getOutputType());
    assertEquals(fooMethod, service.findMethodByName("Foo"));

    MethodDescriptor barMethod = service.getMethods().get(1);
    assertEquals("Bar", barMethod.getName());
    assertEquals(UnittestProto.BarRequest.getDescriptor(),
                 barMethod.getInputType());
    assertEquals(UnittestProto.BarResponse.getDescriptor(),
                 barMethod.getOutputType());
    assertEquals(barMethod, service.findMethodByName("Bar"));

    assertNull(service.findMethodByName("NoSuchMethod"));

    for (int i = 0; i < service.getMethods().size(); i++) {
      assertEquals(i, service.getMethods().get(i).getIndex());
    }
  }

  @Test
  public void testCustomOptions() throws Exception {
    // Get the descriptor indirectly from a dependent proto class. This is to
    // ensure that when a proto class is loaded, custom options defined in its
    // dependencies are also properly initialized.
    Descriptor descriptor =
        TestCustomOptions.TestMessageWithCustomOptionsContainer.getDescriptor()
        .findFieldByName("field").getMessageType();

    assertTrue(
      descriptor.getOptions().hasExtension(UnittestCustomOptions.messageOpt1));
    assertEquals(Integer.valueOf(-56),
      descriptor.getOptions().getExtension(UnittestCustomOptions.messageOpt1));

    FieldDescriptor field = descriptor.findFieldByName("field1");
    assertNotNull(field);

    assertTrue(
      field.getOptions().hasExtension(UnittestCustomOptions.fieldOpt1));
    assertEquals(Long.valueOf(8765432109L),
      field.getOptions().getExtension(UnittestCustomOptions.fieldOpt1));

    EnumDescriptor enumType =
      UnittestCustomOptions.TestMessageWithCustomOptions.AnEnum.getDescriptor();

    assertTrue(
      enumType.getOptions().hasExtension(UnittestCustomOptions.enumOpt1));
    assertEquals(Integer.valueOf(-789),
      enumType.getOptions().getExtension(UnittestCustomOptions.enumOpt1));

    ServiceDescriptor service =
      UnittestCustomOptions.TestServiceWithCustomOptions.getDescriptor();

    assertTrue(
      service.getOptions().hasExtension(UnittestCustomOptions.serviceOpt1));
    assertEquals(Long.valueOf(-9876543210L),
      service.getOptions().getExtension(UnittestCustomOptions.serviceOpt1));

    MethodDescriptor method = service.findMethodByName("Foo");
    assertNotNull(method);

    assertTrue(
      method.getOptions().hasExtension(UnittestCustomOptions.methodOpt1));
    assertEquals(UnittestCustomOptions.MethodOpt1.METHODOPT1_VAL2,
      method.getOptions().getExtension(UnittestCustomOptions.methodOpt1));
  }

  /**
   * Test that the FieldDescriptor.Type enum is the same as the
   * WireFormat.FieldType enum.
   */
  @Test
  public void testFieldTypeTablesMatch() throws Exception {
    FieldDescriptor.Type[] values1 = FieldDescriptor.Type.values();
    WireFormat.FieldType[] values2 = WireFormat.FieldType.values();

    assertEquals(values1.length, values2.length);

    for (int i = 0; i < values1.length; i++) {
      assertEquals(values1[i].toString(), values2[i].toString());
    }
  }

  /**
   * Test that the FieldDescriptor.JavaType enum is the same as the
   * WireFormat.JavaType enum.
   */
  @Test
  public void testJavaTypeTablesMatch() throws Exception {
    FieldDescriptor.JavaType[] values1 = FieldDescriptor.JavaType.values();
    WireFormat.JavaType[] values2 = WireFormat.JavaType.values();

    assertEquals(values1.length, values2.length);

    for (int i = 0; i < values1.length; i++) {
      assertEquals(values1[i].toString(), values2[i].toString());
    }
  }

  @Test
  public void testEnormousDescriptor() throws Exception {
    // The descriptor for this file is larger than 64k, yet it did not cause
    // a compiler error due to an over-long string literal.
    assertTrue(
        UnittestEnormousDescriptor.getDescriptor()
          .toProto().getSerializedSize() > 65536);
  }

  /**
   * Tests that the DescriptorValidationException works as intended.
   */
  @Test
  public void testDescriptorValidatorException() throws Exception {
    FileDescriptorProto fileDescriptorProto = FileDescriptorProto.newBuilder()
      .setName("foo.proto")
      .addMessageType(DescriptorProto.newBuilder()
      .setName("Foo")
        .addField(FieldDescriptorProto.newBuilder()
          .setLabel(FieldDescriptorProto.Label.LABEL_OPTIONAL)
          .setType(FieldDescriptorProto.Type.TYPE_INT32)
          .setName("foo")
          .setNumber(1)
          .setDefaultValue("invalid")
          .build())
        .build())
      .build();
    try {
      Descriptors.FileDescriptor.buildFrom(fileDescriptorProto,
          new FileDescriptor[0]);
      fail("DescriptorValidationException expected");
    } catch (DescriptorValidationException e) {
      // Expected; check that the error message contains some useful hints
      assertTrue(e.getMessage().contains("foo"));
      assertTrue(e.getMessage().contains("Foo"));
      assertTrue(e.getMessage().contains("invalid"));
      assertTrue(e.getCause() instanceof NumberFormatException);
      assertTrue(e.getCause().getMessage().contains("invalid"));
    }
  }

  /**
   * Tests the translate/crosslink for an example where a message field's name
   * and type name are the same.
   */
  @Test
  public void testDescriptorComplexCrosslink() throws Exception {
    FileDescriptorProto fileDescriptorProto = FileDescriptorProto.newBuilder()
      .setName("foo.proto")
      .addMessageType(DescriptorProto.newBuilder()
        .setName("Foo")
        .addField(FieldDescriptorProto.newBuilder()
          .setLabel(FieldDescriptorProto.Label.LABEL_OPTIONAL)
          .setType(FieldDescriptorProto.Type.TYPE_INT32)
          .setName("foo")
          .setNumber(1)
          .build())
        .build())
      .addMessageType(DescriptorProto.newBuilder()
        .setName("Bar")
        .addField(FieldDescriptorProto.newBuilder()
          .setLabel(FieldDescriptorProto.Label.LABEL_OPTIONAL)
          .setTypeName("Foo")
          .setName("Foo")
          .setNumber(1)
          .build())
        .build())
      .build();
    // translate and crosslink
    FileDescriptor file =
      Descriptors.FileDescriptor.buildFrom(fileDescriptorProto,
          new FileDescriptor[0]);
    // verify resulting descriptors
    assertNotNull(file);
    List<Descriptor> msglist = file.getMessageTypes();
    assertNotNull(msglist);
    assertTrue(msglist.size() == 2);
    boolean barFound = false;
    for (Descriptor desc : msglist) {
      if (desc.getName().equals("Bar")) {
        barFound = true;
        assertNotNull(desc.getFields());
        List<FieldDescriptor> fieldlist = desc.getFields();
        assertNotNull(fieldlist);
        assertTrue(fieldlist.size() == 1);
        assertTrue(fieldlist.get(0).getType() == FieldDescriptor.Type.MESSAGE);
        assertTrue(fieldlist.get(0).getMessageType().getName().equals("Foo"));
      }
    }
    assertTrue(barFound);
  }

  @Test
  public void testDependencyOrder() throws Exception {
    FileDescriptorProto fooProto = FileDescriptorProto.newBuilder()
        .setName("foo.proto").build();
    FileDescriptorProto barProto = FileDescriptorProto.newBuilder()
        .setName("bar.proto")
        .addDependency("foo.proto")
        .build();
    FileDescriptorProto bazProto = FileDescriptorProto.newBuilder()
        .setName("baz.proto")
        .addDependency("foo.proto")
        .addDependency("bar.proto")
        .addPublicDependency(0)
        .addPublicDependency(1)
        .build();
    FileDescriptor fooFile = Descriptors.FileDescriptor.buildFrom(fooProto,
        new FileDescriptor[0]);
    FileDescriptor barFile = Descriptors.FileDescriptor.buildFrom(barProto,
        new FileDescriptor[] {fooFile});

    // Items in the FileDescriptor array can be in any order.
    Descriptors.FileDescriptor.buildFrom(bazProto,
        new FileDescriptor[] {fooFile, barFile});
    Descriptors.FileDescriptor.buildFrom(bazProto,
        new FileDescriptor[] {barFile, fooFile});
  }

  @Test
  public void testInvalidPublicDependency() throws Exception {
    FileDescriptorProto fooProto = FileDescriptorProto.newBuilder()
        .setName("foo.proto").build();
    FileDescriptorProto barProto = FileDescriptorProto.newBuilder()
        .setName("boo.proto")
        .addDependency("foo.proto")
        .addPublicDependency(1)  // Error, should be 0.
        .build();
    FileDescriptor fooFile = Descriptors.FileDescriptor.buildFrom(fooProto,
        new FileDescriptor[0]);
    try {
      Descriptors.FileDescriptor.buildFrom(barProto,
          new FileDescriptor[] {fooFile});
      fail("DescriptorValidationException expected");
    } catch (DescriptorValidationException e) {
      assertTrue(e.getMessage().contains("Invalid public dependency index."));
    }
  }

  @Test
  public void testHiddenDependency() throws Exception {
    FileDescriptorProto barProto = FileDescriptorProto.newBuilder()
        .setName("bar.proto")
        .addMessageType(DescriptorProto.newBuilder().setName("Bar"))
        .build();
    FileDescriptorProto forwardProto = FileDescriptorProto.newBuilder()
        .setName("forward.proto")
        .addDependency("bar.proto")
        .build();
    FileDescriptorProto fooProto = FileDescriptorProto.newBuilder()
        .setName("foo.proto")
        .addDependency("forward.proto")
        .addMessageType(DescriptorProto.newBuilder()
            .setName("Foo")
            .addField(FieldDescriptorProto.newBuilder()
                .setLabel(FieldDescriptorProto.Label.LABEL_OPTIONAL)
                .setTypeName("Bar")
                .setName("bar")
                .setNumber(1)))
        .build();
    FileDescriptor barFile = Descriptors.FileDescriptor.buildFrom(
        barProto, new FileDescriptor[0]);
    FileDescriptor forwardFile = Descriptors.FileDescriptor.buildFrom(
        forwardProto, new FileDescriptor[] {barFile});

    try {
      Descriptors.FileDescriptor.buildFrom(
          fooProto, new FileDescriptor[] {forwardFile});
      fail("DescriptorValidationException expected");
    } catch (DescriptorValidationException e) {
      assertTrue(e.getMessage().contains("Bar"));
      assertTrue(e.getMessage().contains("is not defined"));
    }
  }

  @Test
  public void testPublicDependency() throws Exception {
    FileDescriptorProto barProto = FileDescriptorProto.newBuilder()
        .setName("bar.proto")
        .addMessageType(DescriptorProto.newBuilder().setName("Bar"))
        .build();
    FileDescriptorProto forwardProto = FileDescriptorProto.newBuilder()
        .setName("forward.proto")
        .addDependency("bar.proto")
        .addPublicDependency(0)
        .build();
    FileDescriptorProto fooProto = FileDescriptorProto.newBuilder()
        .setName("foo.proto")
        .addDependency("forward.proto")
        .addMessageType(DescriptorProto.newBuilder()
            .setName("Foo")
            .addField(FieldDescriptorProto.newBuilder()
                .setLabel(FieldDescriptorProto.Label.LABEL_OPTIONAL)
                .setTypeName("Bar")
                .setName("bar")
                .setNumber(1)))
        .build();
    FileDescriptor barFile = Descriptors.FileDescriptor.buildFrom(
        barProto, new FileDescriptor[0]);
    FileDescriptor forwardFile = Descriptors.FileDescriptor.buildFrom(
        forwardProto, new FileDescriptor[]{barFile});
    Descriptors.FileDescriptor.buildFrom(
        fooProto, new FileDescriptor[] {forwardFile});
  }

  /**
   * Tests the translate/crosslink for an example with a more complex namespace
   * referencing.
   */
  @Test
  public void testComplexNamespacePublicDependency() throws Exception {
    FileDescriptorProto fooProto = FileDescriptorProto.newBuilder()
        .setName("bar.proto")
        .setPackage("a.b.c.d.bar.shared")
        .addEnumType(EnumDescriptorProto.newBuilder()
            .setName("MyEnum")
            .addValue(EnumValueDescriptorProto.newBuilder()
                .setName("BLAH")
                .setNumber(1)))
        .build();
    FileDescriptorProto barProto = FileDescriptorProto.newBuilder()
        .setName("foo.proto")
        .addDependency("bar.proto")
        .setPackage("a.b.c.d.foo.shared")
        .addMessageType(DescriptorProto.newBuilder()
            .setName("MyMessage")
            .addField(FieldDescriptorProto.newBuilder()
                .setLabel(FieldDescriptorProto.Label.LABEL_REPEATED)
                .setTypeName("bar.shared.MyEnum")
                .setName("MyField")
                .setNumber(1)))
        .build();
    // translate and crosslink
    FileDescriptor fooFile = Descriptors.FileDescriptor.buildFrom(
        fooProto, new FileDescriptor[0]);
    FileDescriptor barFile = Descriptors.FileDescriptor.buildFrom(
        barProto, new FileDescriptor[]{fooFile});
    // verify resulting descriptors
    assertNotNull(barFile);
    List<Descriptor> msglist = barFile.getMessageTypes();
    assertNotNull(msglist);
    assertTrue(msglist.size() == 1);
    Descriptor desc = msglist.get(0);
    if (desc.getName().equals("MyMessage")) {
      assertNotNull(desc.getFields());
      List<FieldDescriptor> fieldlist = desc.getFields();
      assertNotNull(fieldlist);
      assertTrue(fieldlist.size() == 1);
      FieldDescriptor field = fieldlist.get(0);
      assertTrue(field.getType() == FieldDescriptor.Type.ENUM);
      assertTrue(field.getEnumType().getName().equals("MyEnum"));
      assertTrue(field.getEnumType().getFile().getName().equals("bar.proto"));
      assertTrue(field.getEnumType().getFile().getPackage().equals(
          "a.b.c.d.bar.shared"));
    }
  }

  @Test
  public void testOneofDescriptor() throws Exception {
    Descriptor messageType = TestAllTypes.getDescriptor();
    FieldDescriptor field =
        messageType.findFieldByName("oneof_nested_message");
    OneofDescriptor oneofDescriptor = field.getContainingOneof();
    assertNotNull(oneofDescriptor);
    assertSame(oneofDescriptor, messageType.getOneofs().get(0));
    assertEquals("oneof_field", oneofDescriptor.getName());

    assertEquals(4, oneofDescriptor.getFieldCount());
    assertSame(oneofDescriptor.getField(1), field);

    assertEquals(4, oneofDescriptor.getFields().size());
    assertEquals(oneofDescriptor.getFields().get(1), field);
  }

  @Test
  public void testMessageDescriptorExtensions() throws Exception {
    assertFalse(TestAllTypes.getDescriptor().isExtendable());
    assertTrue(TestAllExtensions.getDescriptor().isExtendable());
    assertTrue(TestMultipleExtensionRanges.getDescriptor().isExtendable());

    assertFalse(TestAllTypes.getDescriptor().isExtensionNumber(3));
    assertTrue(TestAllExtensions.getDescriptor().isExtensionNumber(3));
    assertTrue(TestMultipleExtensionRanges.getDescriptor().isExtensionNumber(42));
    assertFalse(TestMultipleExtensionRanges.getDescriptor().isExtensionNumber(43));
    assertFalse(TestMultipleExtensionRanges.getDescriptor().isExtensionNumber(4142));
    assertTrue(TestMultipleExtensionRanges.getDescriptor().isExtensionNumber(4143));
  }

  @Test
  public void testReservedFields() {
    Descriptor d = TestReservedFields.getDescriptor();
    assertTrue(d.isReservedNumber(2));
    assertFalse(d.isReservedNumber(8));
    assertTrue(d.isReservedNumber(9));
    assertTrue(d.isReservedNumber(10));
    assertTrue(d.isReservedNumber(11));
    assertFalse(d.isReservedNumber(12));
    assertFalse(d.isReservedName("foo"));
    assertTrue(d.isReservedName("bar"));
    assertTrue(d.isReservedName("baz"));
  }

  @Test
  public void testToString() {
    assertEquals("protobuf_unittest.TestAllTypes.optional_uint64",
        UnittestProto.TestAllTypes.getDescriptor().findFieldByNumber(
            UnittestProto.TestAllTypes.OPTIONAL_UINT64_FIELD_NUMBER).toString());
  }

  @Test
  public void testPackedEnumField() throws Exception {
    FileDescriptorProto fileDescriptorProto = FileDescriptorProto.newBuilder()
        .setName("foo.proto")
        .addEnumType(EnumDescriptorProto.newBuilder()
          .setName("Enum")
          .addValue(EnumValueDescriptorProto.newBuilder()
            .setName("FOO")
            .setNumber(1)
            .build())
          .build())
        .addMessageType(DescriptorProto.newBuilder()
          .setName("Message")
          .addField(FieldDescriptorProto.newBuilder()
            .setName("foo")
            .setTypeName("Enum")
            .setNumber(1)
            .setLabel(FieldDescriptorProto.Label.LABEL_REPEATED)
            .setOptions(DescriptorProtos.FieldOptions.newBuilder()
              .setPacked(true)
              .build())
            .build())
          .build())
        .build();
    Descriptors.FileDescriptor.buildFrom(
        fileDescriptorProto, new FileDescriptor[0]);
  }
}
