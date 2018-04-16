<?php

# phpunit has memory leak by itself. Thus, it cannot be used to test memory leak.

# require_once('generated/NoNamespaceEnum.php');
# require_once('generated/NoNamespaceMessage.php');
# require_once('generated/NoNamespaceMessage_NestedEnum.php');
# require_once('generated/PrefixEmpty.php');
# require_once('generated/PrefixTestPrefix.php');
# require_once('generated/TestEmptyNamespace.php');
require_once('generated/Bar/TestInclude.php');
# require_once('generated/Foo/PBARRAY.php');
# require_once('generated/Foo/PBEmpty.php');
require_once('generated/Foo/TestEnum.php');
# require_once('generated/Foo/TestIncludeNamespaceMessage.php');
# require_once('generated/Foo/TestIncludePrefixMessage.php');
require_once('generated/Foo/TestMessage.php');
# require_once('generated/Foo/TestMessage_Empty.php');
# require_once('generated/Foo/TestMessage_NestedEnum.php');
require_once('generated/Foo/TestMessage_Sub.php');
# require_once('generated/Foo/TestPackedMessage.php');
# require_once('generated/Foo/TestPhpDoc.php');
# require_once('generated/Foo/TestRandomFieldOrder.php');
# require_once('generated/Foo/TestReverseFieldOrder.php');
# require_once('generated/Foo/TestUnpackedMessage.php');
# require_once('generated/Foo/testLowerCaseMessage.php');
# require_once('generated/Foo/testLowerCaseEnum.php');
require_once('generated/GPBMetadata/Proto/Test.php');
require_once('generated/GPBMetadata/Proto/TestEmptyPhpNamespace.php');
require_once('generated/GPBMetadata/Proto/TestInclude.php');
require_once('generated/GPBMetadata/Proto/TestNoNamespace.php');
require_once('generated/GPBMetadata/Proto/TestPhpNamespace.php');
require_once('generated/GPBMetadata/Proto/TestPrefix.php');
require_once('generated/Php/Test/TestNamespace.php');
require_once('test_util.php');

use Foo\TestMessage;
use Foo\TestMessage_Sub;
use Google\Protobuf\Internal\GPBType;
use Google\Protobuf\Internal\MapField;
use Google\Protobuf\Internal\RepeatedField;

#########################################################
# Test Round Trip.
#########################################################

$from = new TestMessage();
TestUtil::setTestMessage($from);
TestUtil::assertTestMessage($from);

$data = $from->serializeToString();

$to = new TestMessage();
$to->mergeFromString($data);

TestUtil::assertTestMessage($to);

#########################################################
# Test Unset.
#########################################################

$map = new MapField(GPBType::STRING, GPBType::STRING);
$map['abc'] = 'abc';
unset($map['abc']);

$map = new MapField(GPBType::INT32,
    GPBType::MESSAGE, TestMessage_Sub::class);
$sub_m = new TestMessage_Sub();
$arr[0] = $sub_m;
unset($arr[0]);

# TODO(teboring): Add these back when upb array support unset.
# $arr = new RepeatedField(GPBType::STRING);
# $arr[] = 'abc';
# unset($arr[0]);
# 
# $arr = new RepeatedField(GPBType::MESSAGE, TestMessage_Sub::class);
# $arr[] = new TestMessage_Sub();
# unset($arr[0]);

#########################################################
# Test Iterator.
#########################################################

$map = new MapField(GPBType::INT32,
    GPBType::MESSAGE, TestMessage_Sub::class);
$map[0] = new TestMessage_Sub();
$map[1] = new TestMessage_Sub();
$map[2] = new TestMessage_Sub();

foreach ($map as $key => $val) {
  $val->setA($key);
}

$arr = new RepeatedField(GPBType::MESSAGE, TestMessage_Sub::class);
$arr[0] = new TestMessage_Sub();
$arr[1] = new TestMessage_Sub();
$arr[2] = new TestMessage_Sub();

foreach ($arr as $val) {
  $val->setA(0);
}

# $from->setRecursive($from);
# 
# $arr = new RepeatedField(GPBType::MESSAGE, TestMessage::class);
# $arr[] = new TestMessage;
# $arr[0]->SetRepeatedRecursive($arr);
# 
# // Test oneof fields.
# $m = new TestMessage();
# 
# $m->setOneofInt32(1);
# assert(1 === $m->getOneofInt32());
# assert(0.0 === $m->getOneofFloat());
# assert('' === $m->getOneofString());
# assert(NULL === $m->getOneofMessage());
# $data = $m->serializeToString();
# $n = new TestMessage();
# $n->mergeFromString($data);
# assert(1 === $n->getOneofInt32());
# 
# $m->setOneofFloat(2.0);
# assert(0 === $m->getOneofInt32());
# assert(2.0 === $m->getOneofFloat());
# assert('' === $m->getOneofString());
# assert(NULL === $m->getOneofMessage());
# $data = $m->serializeToString();
# $n = new TestMessage();
# $n->mergeFromString($data);
# assert(2.0 === $n->getOneofFloat());
# 
# $m->setOneofString('abc');
# assert(0 === $m->getOneofInt32());
# assert(0.0 === $m->getOneofFloat());
# assert('abc' === $m->getOneofString());
# assert(NULL === $m->getOneofMessage());
# $data = $m->serializeToString();
# $n = new TestMessage();
# $n->mergeFromString($data);
# assert('abc' === $n->getOneofString());
# 
# $sub_m = new TestMessage_Sub();
# $sub_m->setA(1);
# $m->setOneofMessage($sub_m);
# assert(0 === $m->getOneofInt32());
# assert(0.0 === $m->getOneofFloat());
# assert('' === $m->getOneofString());
# assert(1 === $m->getOneofMessage()->getA());
# $data = $m->serializeToString();
# $n = new TestMessage();
# $n->mergeFromString($data);
# assert(1 === $n->getOneofMessage()->getA());
# 
# $m = new TestMessage();
# $m->mergeFromString(hex2bin('F80601'));
# assert('F80601', bin2hex($m->serializeToString()));

$from = new TestMessage();
$to = new TestMessage();
TestUtil::setTestMessage($from);
$to->mergeFrom($from);
TestUtil::assertTestMessage($to);

#########################################################
# Test Clear.
#########################################################
$m = new TestMessage();

$sub_m1 = new TestMessage_Sub();
$sub_m1->setA(1);
$m->setOptionalMessage($sub_m1);
assert($m->getOptionalMessage()->getA() === 1);

$arr = $m->getRepeatedMessage();
$arr[] = new TestMessage_Sub();
assert(1 === count($arr));

$map = $m->getMapInt32Message();
$map[0] = new TestMessage_Sub();
assert(1 === count($map));

$m->clear();

assert(is_null($m->getOptionalMessage()));

$arr = $m->getRepeatedMessage();
assert(0 === count($arr));

$map = $m->getMapInt32Message();
assert(0 === count($map));

#########################################################
# Test Clear Oneof.
#########################################################
$m = new TestMessage();
$subm = new TestMessage_Sub();
$subm->setA(1);
$m->setOneofMessage($subm);
assert(1 === $m->getOneofMessage()->getA());
$m->clear();
assert(0 === $m->getOneofInt32());
assert(is_null($m->getOneofMessage()));
