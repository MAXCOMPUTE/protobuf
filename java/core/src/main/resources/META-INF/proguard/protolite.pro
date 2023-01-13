# Skip runtime check for isOnAndroidDevice().
# One line to make it easy to remove with sed.
# Safe to default since proguard rules are only active on Android.
-assumevalues class com.google.protobuf.Android { static boolean ASSUME_ANDROID return true; }

# Don't strip fields which are required for serialization
# https://github.com/protocolbuffers/protobuf/issues/11252
-keepclassmembers class * extends com.google.protobuf.GeneratedMessageLite {
  <fields>;
}
