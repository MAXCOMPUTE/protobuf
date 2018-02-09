<?php
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: test.proto

namespace Foo;

use Google\Protobuf\Internal\GPBType;
use Google\Protobuf\Internal\RepeatedField;
use Google\Protobuf\Internal\GPBUtil;

/**
 *   // Singular
 *   int32 optional_int32 = 1;
 *   int64 optional_int64 = 2;
 *   uint32 optional_uint32 = 3;
 *   uint64 optional_uint64 = 4;
 *   sint32 optional_sint32 = 5;
 *   sint64 optional_sint64 = 6;
 *   fixed32 optional_fixed32 = 7;
 *   fixed64 optional_fixed64 = 8;
 *   sfixed32 optional_sfixed32 = 9;
 *   sfixed64 optional_sfixed64 = 10;
 *   float optional_float = 11;
 *   double optional_double = 12;
 *   bool optional_bool = 13;
 *   string optional_string = 14;
 *   bytes optional_bytes = 15;
 * 
 *   Sub optional_message = 17;
 * 
 *   // Repeated
 *   repeated    int32 repeated_int32    = 31;
 *   repeated    int64 repeated_int64    = 32;
 *   repeated   uint32 repeated_uint32   = 33;
 *   repeated   uint64 repeated_uint64   = 34;
 *   repeated   sint32 repeated_sint32   = 35;
 *   repeated   sint64 repeated_sint64   = 36;
 *   repeated  fixed32 repeated_fixed32  = 37;
 *   repeated  fixed64 repeated_fixed64  = 38;
 *   repeated sfixed32 repeated_sfixed32 = 39;
 *   repeated sfixed64 repeated_sfixed64 = 40;
 *   repeated    float repeated_float    = 41;
 *   repeated   double repeated_double   = 42;
 *   repeated     bool repeated_bool     = 43;
 *   repeated   string repeated_string   = 44;
 *   repeated    bytes repeated_bytes    = 45;
 * 
 *   repeated TestEnum repeated_enum = 46;
 *   repeated Sub repeated_message = 47;
 *
 * Generated from protobuf message <code>foo.TestMessage</code>
 */
class TestMessage extends \Google\Protobuf\Internal\Message
{
    public function __construct() {
        \GPBMetadata\Test::initOnce();
        parent::__construct();
    }

    /**
     * Map
     *
     * Generated from protobuf field <code>map<int32, int32> map_int32_int32 = 71;</code>
     * @return \Google\Protobuf\Internal\MapField
     */
    public function getMapInt32Int32()
    {
        return $this->map_int32_int32;
    }

    /**
     * Map
     *
     * Generated from protobuf field <code>map<int32, int32> map_int32_int32 = 71;</code>
     * @param array|\Google\Protobuf\Internal\MapField $var
     * @return $this
     */
    public function setMapInt32Int32($var)
    {
        $arr = GPBUtil::checkMapField($var, \Google\Protobuf\Internal\GPBType::INT32, \Google\Protobuf\Internal\GPBType::INT32);
        $this->map_int32_int32 = $arr;

        return $this;
    }

}

