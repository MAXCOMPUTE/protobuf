<?php
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: google/protobuf/descriptor.proto

namespace Google\Protobuf\Internal;

use Google\Protobuf\Internal\GPBType;
use Google\Protobuf\Internal\GPBWire;
use Google\Protobuf\Internal\RepeatedField;
use Google\Protobuf\Internal\InputStream;
use Google\Protobuf\Internal\GPBUtil;
/**
 * Describes a complete .proto file.
 *
 * Generated from protobuf message <code>google.protobuf.FileDescriptorProto</code>
 */
class FileDescriptorProto extends \Google\Protobuf\Internal\Message
{
    /**
     * file name, relative to root of source tree
     *
     * Generated from protobuf field <code>optional string name = 1;</code>
     */
    private $name = '';
    private $has_name = false;
    /**
     * e.g. "foo", "foo.bar", etc.
     *
     * Generated from protobuf field <code>optional string package = 2;</code>
     */
    private $package = '';
    private $has_package = false;
    /**
     * Names of files imported by this file.
     *
     * Generated from protobuf field <code>repeated string dependency = 3;</code>
     */
    private $dependency;
    private $has_dependency = false;
    /**
     * Indexes of the public imported files in the dependency list above.
     *
     * Generated from protobuf field <code>repeated int32 public_dependency = 10;</code>
     */
    private $public_dependency;
    private $has_public_dependency = false;
    /**
     * Indexes of the weak imported files in the dependency list.
     * For Google-internal migration only. Do not use.
     *
     * Generated from protobuf field <code>repeated int32 weak_dependency = 11;</code>
     */
    private $weak_dependency;
    private $has_weak_dependency = false;
    /**
     * All top-level definitions in this file.
     *
     * Generated from protobuf field <code>repeated .google.protobuf.DescriptorProto message_type = 4;</code>
     */
    private $message_type;
    private $has_message_type = false;
    /**
     * Generated from protobuf field <code>repeated .google.protobuf.EnumDescriptorProto enum_type = 5;</code>
     */
    private $enum_type;
    private $has_enum_type = false;
    /**
     * Generated from protobuf field <code>repeated .google.protobuf.ServiceDescriptorProto service = 6;</code>
     */
    private $service;
    private $has_service = false;
    /**
     * Generated from protobuf field <code>repeated .google.protobuf.FieldDescriptorProto extension = 7;</code>
     */
    private $extension;
    private $has_extension = false;
    /**
     * Generated from protobuf field <code>optional .google.protobuf.FileOptions options = 8;</code>
     */
    private $options = null;
    private $has_options = false;
    /**
     * This field contains optional information about the original source code.
     * You may safely remove this entire field without harming runtime
     * functionality of the descriptors -- the information is needed only by
     * development tools.
     *
     * Generated from protobuf field <code>optional .google.protobuf.SourceCodeInfo source_code_info = 9;</code>
     */
    private $source_code_info = null;
    private $has_source_code_info = false;
    /**
     * The syntax of the proto file.
     * The supported values are "proto2" and "proto3".
     *
     * Generated from protobuf field <code>optional string syntax = 12;</code>
     */
    private $syntax = '';
    private $has_syntax = false;

    /**
     * Constructor.
     *
     * @param array $data {
     *     Optional. Data for populating the Message object.
     *
     *     @type string $name
     *           file name, relative to root of source tree
     *     @type string $package
     *           e.g. "foo", "foo.bar", etc.
     *     @type string[]|\Google\Protobuf\Internal\RepeatedField $dependency
     *           Names of files imported by this file.
     *     @type int[]|\Google\Protobuf\Internal\RepeatedField $public_dependency
     *           Indexes of the public imported files in the dependency list above.
     *     @type int[]|\Google\Protobuf\Internal\RepeatedField $weak_dependency
     *           Indexes of the weak imported files in the dependency list.
     *           For Google-internal migration only. Do not use.
     *     @type \Google\Protobuf\Internal\DescriptorProto[]|\Google\Protobuf\Internal\RepeatedField $message_type
     *           All top-level definitions in this file.
     *     @type \Google\Protobuf\Internal\EnumDescriptorProto[]|\Google\Protobuf\Internal\RepeatedField $enum_type
     *     @type \Google\Protobuf\Internal\ServiceDescriptorProto[]|\Google\Protobuf\Internal\RepeatedField $service
     *     @type \Google\Protobuf\Internal\FieldDescriptorProto[]|\Google\Protobuf\Internal\RepeatedField $extension
     *     @type \Google\Protobuf\Internal\FileOptions $options
     *     @type \Google\Protobuf\Internal\SourceCodeInfo $source_code_info
     *           This field contains optional information about the original source code.
     *           You may safely remove this entire field without harming runtime
     *           functionality of the descriptors -- the information is needed only by
     *           development tools.
     *     @type string $syntax
     *           The syntax of the proto file.
     *           The supported values are "proto2" and "proto3".
     * }
     */
    public function __construct($data = NULL) {
        \GPBMetadata\Google\Protobuf\Internal\Descriptor::initOnce();
        parent::__construct($data);
    }

    /**
     * file name, relative to root of source tree
     *
     * Generated from protobuf field <code>optional string name = 1;</code>
     * @return string
     */
    public function getName()
    {
        return $this->name;
    }

    /**
     * file name, relative to root of source tree
     *
     * Generated from protobuf field <code>optional string name = 1;</code>
     * @param string $var
     * @return $this
     */
    public function setName($var)
    {
        GPBUtil::checkString($var, True);
        $this->name = $var;
        $this->has_name = true;

        return $this;
    }

    public function hasName()
    {
        return $this->has_name;
    }

    /**
     * e.g. "foo", "foo.bar", etc.
     *
     * Generated from protobuf field <code>optional string package = 2;</code>
     * @return string
     */
    public function getPackage()
    {
        return $this->package;
    }

    /**
     * e.g. "foo", "foo.bar", etc.
     *
     * Generated from protobuf field <code>optional string package = 2;</code>
     * @param string $var
     * @return $this
     */
    public function setPackage($var)
    {
        GPBUtil::checkString($var, True);
        $this->package = $var;
        $this->has_package = true;

        return $this;
    }

    public function hasPackage()
    {
        return $this->has_package;
    }

    /**
     * Names of files imported by this file.
     *
     * Generated from protobuf field <code>repeated string dependency = 3;</code>
     * @return \Google\Protobuf\Internal\RepeatedField
     */
    public function getDependency()
    {
        return $this->dependency;
    }

    /**
     * Names of files imported by this file.
     *
     * Generated from protobuf field <code>repeated string dependency = 3;</code>
     * @param string[]|\Google\Protobuf\Internal\RepeatedField $var
     * @return $this
     */
    public function setDependency($var)
    {
        $arr = GPBUtil::checkRepeatedField($var, \Google\Protobuf\Internal\GPBType::STRING);
        $this->dependency = $arr;
        $this->has_dependency = true;

        return $this;
    }

    public function hasDependency()
    {
        return $this->has_dependency;
    }

    /**
     * Indexes of the public imported files in the dependency list above.
     *
     * Generated from protobuf field <code>repeated int32 public_dependency = 10;</code>
     * @return \Google\Protobuf\Internal\RepeatedField
     */
    public function getPublicDependency()
    {
        return $this->public_dependency;
    }

    /**
     * Indexes of the public imported files in the dependency list above.
     *
     * Generated from protobuf field <code>repeated int32 public_dependency = 10;</code>
     * @param int[]|\Google\Protobuf\Internal\RepeatedField $var
     * @return $this
     */
    public function setPublicDependency($var)
    {
        $arr = GPBUtil::checkRepeatedField($var, \Google\Protobuf\Internal\GPBType::INT32);
        $this->public_dependency = $arr;
        $this->has_public_dependency = true;

        return $this;
    }

    public function hasPublicDependency()
    {
        return $this->has_public_dependency;
    }

    /**
     * Indexes of the weak imported files in the dependency list.
     * For Google-internal migration only. Do not use.
     *
     * Generated from protobuf field <code>repeated int32 weak_dependency = 11;</code>
     * @return \Google\Protobuf\Internal\RepeatedField
     */
    public function getWeakDependency()
    {
        return $this->weak_dependency;
    }

    /**
     * Indexes of the weak imported files in the dependency list.
     * For Google-internal migration only. Do not use.
     *
     * Generated from protobuf field <code>repeated int32 weak_dependency = 11;</code>
     * @param int[]|\Google\Protobuf\Internal\RepeatedField $var
     * @return $this
     */
    public function setWeakDependency($var)
    {
        $arr = GPBUtil::checkRepeatedField($var, \Google\Protobuf\Internal\GPBType::INT32);
        $this->weak_dependency = $arr;
        $this->has_weak_dependency = true;

        return $this;
    }

    public function hasWeakDependency()
    {
        return $this->has_weak_dependency;
    }

    /**
     * All top-level definitions in this file.
     *
     * Generated from protobuf field <code>repeated .google.protobuf.DescriptorProto message_type = 4;</code>
     * @return \Google\Protobuf\Internal\RepeatedField
     */
    public function getMessageType()
    {
        return $this->message_type;
    }

    /**
     * All top-level definitions in this file.
     *
     * Generated from protobuf field <code>repeated .google.protobuf.DescriptorProto message_type = 4;</code>
     * @param \Google\Protobuf\Internal\DescriptorProto[]|\Google\Protobuf\Internal\RepeatedField $var
     * @return $this
     */
    public function setMessageType($var)
    {
        $arr = GPBUtil::checkRepeatedField($var, \Google\Protobuf\Internal\GPBType::MESSAGE, \Google\Protobuf\Internal\DescriptorProto::class);
        $this->message_type = $arr;
        $this->has_message_type = true;

        return $this;
    }

    public function hasMessageType()
    {
        return $this->has_message_type;
    }

    /**
     * Generated from protobuf field <code>repeated .google.protobuf.EnumDescriptorProto enum_type = 5;</code>
     * @return \Google\Protobuf\Internal\RepeatedField
     */
    public function getEnumType()
    {
        return $this->enum_type;
    }

    /**
     * Generated from protobuf field <code>repeated .google.protobuf.EnumDescriptorProto enum_type = 5;</code>
     * @param \Google\Protobuf\Internal\EnumDescriptorProto[]|\Google\Protobuf\Internal\RepeatedField $var
     * @return $this
     */
    public function setEnumType($var)
    {
        $arr = GPBUtil::checkRepeatedField($var, \Google\Protobuf\Internal\GPBType::MESSAGE, \Google\Protobuf\Internal\EnumDescriptorProto::class);
        $this->enum_type = $arr;
        $this->has_enum_type = true;

        return $this;
    }

    public function hasEnumType()
    {
        return $this->has_enum_type;
    }

    /**
     * Generated from protobuf field <code>repeated .google.protobuf.ServiceDescriptorProto service = 6;</code>
     * @return \Google\Protobuf\Internal\RepeatedField
     */
    public function getService()
    {
        return $this->service;
    }

    /**
     * Generated from protobuf field <code>repeated .google.protobuf.ServiceDescriptorProto service = 6;</code>
     * @param \Google\Protobuf\Internal\ServiceDescriptorProto[]|\Google\Protobuf\Internal\RepeatedField $var
     * @return $this
     */
    public function setService($var)
    {
        $arr = GPBUtil::checkRepeatedField($var, \Google\Protobuf\Internal\GPBType::MESSAGE, \Google\Protobuf\Internal\ServiceDescriptorProto::class);
        $this->service = $arr;
        $this->has_service = true;

        return $this;
    }

    public function hasService()
    {
        return $this->has_service;
    }

    /**
     * Generated from protobuf field <code>repeated .google.protobuf.FieldDescriptorProto extension = 7;</code>
     * @return \Google\Protobuf\Internal\RepeatedField
     */
    public function getExtension()
    {
        return $this->extension;
    }

    /**
     * Generated from protobuf field <code>repeated .google.protobuf.FieldDescriptorProto extension = 7;</code>
     * @param \Google\Protobuf\Internal\FieldDescriptorProto[]|\Google\Protobuf\Internal\RepeatedField $var
     * @return $this
     */
    public function setExtension($var)
    {
        $arr = GPBUtil::checkRepeatedField($var, \Google\Protobuf\Internal\GPBType::MESSAGE, \Google\Protobuf\Internal\FieldDescriptorProto::class);
        $this->extension = $arr;
        $this->has_extension = true;

        return $this;
    }

    public function hasExtension()
    {
        return $this->has_extension;
    }

    /**
     * Generated from protobuf field <code>optional .google.protobuf.FileOptions options = 8;</code>
     * @return \Google\Protobuf\Internal\FileOptions
     */
    public function getOptions()
    {
        return $this->options;
    }

    /**
     * Generated from protobuf field <code>optional .google.protobuf.FileOptions options = 8;</code>
     * @param \Google\Protobuf\Internal\FileOptions $var
     * @return $this
     */
    public function setOptions($var)
    {
        GPBUtil::checkMessage($var, \Google\Protobuf\Internal\FileOptions::class);
        $this->options = $var;
        $this->has_options = true;

        return $this;
    }

    public function hasOptions()
    {
        return $this->has_options;
    }

    /**
     * This field contains optional information about the original source code.
     * You may safely remove this entire field without harming runtime
     * functionality of the descriptors -- the information is needed only by
     * development tools.
     *
     * Generated from protobuf field <code>optional .google.protobuf.SourceCodeInfo source_code_info = 9;</code>
     * @return \Google\Protobuf\Internal\SourceCodeInfo
     */
    public function getSourceCodeInfo()
    {
        return $this->source_code_info;
    }

    /**
     * This field contains optional information about the original source code.
     * You may safely remove this entire field without harming runtime
     * functionality of the descriptors -- the information is needed only by
     * development tools.
     *
     * Generated from protobuf field <code>optional .google.protobuf.SourceCodeInfo source_code_info = 9;</code>
     * @param \Google\Protobuf\Internal\SourceCodeInfo $var
     * @return $this
     */
    public function setSourceCodeInfo($var)
    {
        GPBUtil::checkMessage($var, \Google\Protobuf\Internal\SourceCodeInfo::class);
        $this->source_code_info = $var;
        $this->has_source_code_info = true;

        return $this;
    }

    public function hasSourceCodeInfo()
    {
        return $this->has_source_code_info;
    }

    /**
     * The syntax of the proto file.
     * The supported values are "proto2" and "proto3".
     *
     * Generated from protobuf field <code>optional string syntax = 12;</code>
     * @return string
     */
    public function getSyntax()
    {
        return $this->syntax;
    }

    /**
     * The syntax of the proto file.
     * The supported values are "proto2" and "proto3".
     *
     * Generated from protobuf field <code>optional string syntax = 12;</code>
     * @param string $var
     * @return $this
     */
    public function setSyntax($var)
    {
        GPBUtil::checkString($var, True);
        $this->syntax = $var;
        $this->has_syntax = true;

        return $this;
    }

    public function hasSyntax()
    {
        return $this->has_syntax;
    }

}

