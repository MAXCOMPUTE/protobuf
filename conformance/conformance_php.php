<?php

require_once("Conformance/WireFormat.php");
require_once("Conformance/ConformanceResponse.php");
require_once("Conformance/ConformanceRequest.php");
require_once("Google/Protobuf/Any.php");
require_once("Google/Protobuf/Duration.php");
require_once("Google/Protobuf/FieldMask.php");
require_once("Google/Protobuf/Struct.php");
require_once("Google/Protobuf/Value.php");
require_once("Google/Protobuf/ListValue.php");
require_once("Google/Protobuf/NullValue.php");
require_once("Google/Protobuf/Timestamp.php");
require_once("Google/Protobuf/DoubleValue.php");
require_once("Google/Protobuf/BytesValue.php");
require_once("Google/Protobuf/FloatValue.php");
require_once("Google/Protobuf/Int64Value.php");
require_once("Google/Protobuf/UInt32Value.php");
require_once("Google/Protobuf/BoolValue.php");
require_once("Google/Protobuf/DoubleValue.php");
require_once("Google/Protobuf/Int32Value.php");
require_once("Google/Protobuf/StringValue.php");
require_once("Google/Protobuf/UInt64Value.php");
require_once("Protobuf_test_messages/Proto3/ForeignMessage.php");
require_once("Protobuf_test_messages/Proto3/ForeignEnum.php");
require_once("Protobuf_test_messages/Proto3/TestAllTypes.php");
require_once("Protobuf_test_messages/Proto3/TestAllTypes_NestedMessage.php");
require_once("Protobuf_test_messages/Proto3/TestAllTypes_NestedEnum.php");

require_once("GPBMetadata/Conformance.php");
require_once("GPBMetadata/Google/Protobuf/Any.php");
require_once("GPBMetadata/Google/Protobuf/Duration.php");
require_once("GPBMetadata/Google/Protobuf/FieldMask.php");
require_once("GPBMetadata/Google/Protobuf/Struct.php");
require_once("GPBMetadata/Google/Protobuf/Timestamp.php");
require_once("GPBMetadata/Google/Protobuf/Wrappers.php");
require_once("GPBMetadata/Google/Protobuf/TestMessagesProto3.php");

use  \Conformance\WireFormat;

$test_count = 0;

function doTest($request)
{
    fwrite(STDERR, "Start Test.\n");
    $test_message = new \Protobuf_test_messages\Proto3\TestAllTypes();
    $response = new \Conformance\ConformanceResponse();
    if ($request->getPayload() == "protobuf_payload") {
      fwrite(STDERR, "Protobuf Payload\n");
      try {
          fwrite(STDERR, "Request Protobuf Payload:" . bin2hex($request->getProtobufPayload()) . "\n");
          $test_message->mergeFromString($request->getProtobufPayload());
      } catch (Exception $e) {
          # fwrite(STDERR, "Protobuf Error: " . $e->getMessage() . "\n");
          $response->setParseError($e->getMessage());
          return $response;
      }
    } elseif ($request->getPayload() == "json_payload") {
      fwrite(STDERR, "Json Payload\n");
      try {
          fwrite(STDERR, "Request Json Payload:" . $request->getJsonPayload() . "\n");
          $test_message->mergeFromJsonString($request->getJsonPayload());
      } catch (Exception $e) {
          fwrite(STDERR, "Invalid Json Payload\n");
          $response->setParseError($e->getMessage());
          return $response;
      }
    } else {
      trigger_error("Request didn't have payload.", E_USER_ERROR);
    }
    # fwrite(STDERR, "Finish Parsing Request.\n");

    if ($request->getRequestedOutputFormat() == WireFormat::UNSPECIFIED) {
      fwrite(STDERR, "Response Format: UNSPECIFIED.\n");
      trigger_error("Unspecified output format.", E_USER_ERROR);
    } elseif ($request->getRequestedOutputFormat() == WireFormat::PROTOBUF) {
      $response->setProtobufPayload($test_message->serializeToString());
      fwrite(STDERR, "Response Protobuf Payload:" . bin2hex($response->getProtobufPayload()) . "\n");
    } elseif ($request->getRequestedOutputFormat() == WireFormat::JSON) {
      $response->setJsonPayload($test_message->serializeToJsonString());
      fwrite(STDERR, "Response Json Payload:" . $response->getJsonPayload() . "\n");
    }

    fwrite(STDERR, "Finish Test.\n");
    return $response;
}

function doTestIO()
{
    $length_bytes = fread(STDIN, 4);
    if (strlen($length_bytes) == 0) {
      return false;   # EOF
    } elseif (strlen($length_bytes) != 4) {
      trigger_error("I/O error", E_USER_ERROR);
    }

    $length = unpack("V", $length_bytes)[1];
    $serialized_request = fread(STDIN, $length);
    fwrite(STDERR, "=================================\n");
    fwrite(STDERR, "Request: ". bin2hex($serialized_request) ."\n");
    if (strlen($serialized_request) != $length) {
      trigger_error("I/O error", E_USER_ERROR);
    }

    $request = new \Conformance\ConformanceRequest();
    $request->mergeFromString($serialized_request);

    $response = doTest($request);

    $serialized_response = $response->serializeToString();
    fwrite(STDERR, "Response:". bin2hex($serialized_response) ."\n");
    fwrite(STDOUT, pack("V", strlen($serialized_response)));
    fwrite(STDOUT, $serialized_response);

    $GLOBALS['test_count'] += 1;

    return true;
}

while(true){
  if (!doTestIO()) {
      fprintf(STDERR,
             "conformance_php: received EOF from test runner " +
             "after %d tests, exiting\n", $test_count);
      exit;
  }
}
