#region Copyright notice and license
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
#endregion

using System.IO;
using Google.Protobuf.TestProtos;
using NUnit.Framework;

namespace Google.Protobuf
{
    public abstract class CodedInputTestBase
    {
        /// <summary>
        /// Helper to construct a byte array from a bunch of bytes.  The inputs are
        /// actually ints so that I can use hex notation and not get stupid errors
        /// about precision.
        /// </summary>
        protected static byte[] Bytes(params int[] bytesAsInts)
        {
            byte[] bytes = new byte[bytesAsInts.Length];
            for (int i = 0; i < bytesAsInts.Length; i++)
            {
                bytes[i] = (byte) bytesAsInts[i];
            }
            return bytes;
        }

        /// <summary>
        /// Parses the given bytes using ReadRawVarint32() and ReadRawVarint64()
        /// </summary>
        protected abstract void AssertReadVarint(byte[] data, ulong value);

        /// <summary>
        /// Parses the given bytes using ReadRawVarint32() and ReadRawVarint64() and
        /// expects them to fail with an InvalidProtocolBufferException whose
        /// description matches the given one.
        /// </summary>
        protected abstract void AssertReadVarintFailure(InvalidProtocolBufferException expected, byte[] data);

        /// <summary>
        /// Parses the given bytes using ReadRawLittleEndian32() and checks
        /// that the result matches the given value.
        /// </summary>
        protected abstract void AssertReadLittleEndian32(byte[] data, uint value);

        /// <summary>
        /// Parses the given bytes using ReadRawLittleEndian64() and checks
        /// that the result matches the given value.
        /// </summary>
        protected abstract void AssertReadLittleEndian64(byte[] data, ulong value);

        protected abstract void AssertEnum(byte[] data, int value);

        protected abstract void ReadTag(byte[] data);

        protected abstract void AssertReadFloat(byte[] data, float value);

        protected abstract void AssertReadDouble(byte[] data, double value);

        protected abstract T ParseFrom<T>(MessageParser<T> messageParser, ByteString bytes) where T : IMessage<T>;

        protected abstract T ParseFromWithLimits<T>(MessageParser<T> messageParser, ByteString bytes, int sizeLimit, int resursionLimits) where T : IMessage<T>;

        internal static TestRecursiveMessage MakeRecursiveMessage(int depth)
        {
            if (depth == 0)
            {
                return new TestRecursiveMessage { I = 5 };
            }
            else
            {
                return new TestRecursiveMessage { A = MakeRecursiveMessage(depth - 1) };
            }
        }

        internal static void AssertMessageDepth(TestRecursiveMessage message, int depth)
        {
            if (depth == 0)
            {
                Assert.IsNull(message.A);
                Assert.AreEqual(5, message.I);
            }
            else
            {
                Assert.IsNotNull(message.A);
                AssertMessageDepth(message.A, depth - 1);
            }
        }

        [Test]
        public void MaliciousRecursion()
        {
            ByteString atRecursiveLimit = MakeRecursiveMessage(CodedInputStream.DefaultRecursionLimit).ToByteString();
            ByteString beyondRecursiveLimit = MakeRecursiveMessage(CodedInputStream.DefaultRecursionLimit + 1).ToByteString();

            AssertMessageDepth(ParseFrom(TestRecursiveMessage.Parser, atRecursiveLimit), CodedInputStream.DefaultRecursionLimit);

            Assert.Throws<InvalidProtocolBufferException>(() => ParseFrom(TestRecursiveMessage.Parser, beyondRecursiveLimit));

            CodedInputStream input = CodedInputStream.CreateWithLimits(new MemoryStream(atRecursiveLimit.ToByteArray()), 1000000, CodedInputStream.DefaultRecursionLimit - 1);
            Assert.Throws<InvalidProtocolBufferException>(() => TestRecursiveMessage.Parser.ParseFrom(input));

            Assert.Throws<InvalidProtocolBufferException>(() => ParseFromWithLimits(TestRecursiveMessage.Parser, atRecursiveLimit, sizeLimit: 1000000, resursionLimits: CodedInputStream.DefaultRecursionLimit - 1));
        }

        [Test]
        public void ReadHugeBlob()
        {
            // Allocate and initialize a 1MB blob.
            byte[] blob = new byte[1 << 20];
            for (int i = 0; i < blob.Length; i++)
            {
                blob[i] = (byte)i;
            }

            // Make a message containing it.
            var message = new TestAllTypes { SingleBytes = ByteString.CopyFrom(blob) };

            // Serialize and parse it.  Make sure to parse from an InputStream, not
            // directly from a ByteString, so that CodedInputStream uses buffered
            // reading.
            TestAllTypes message2 = ParseFrom(TestAllTypes.Parser, message.ToByteString());

            Assert.AreEqual(message, message2);
        }

        [Test]
        public void ReadLittleEndian()
        {
            AssertReadLittleEndian32(Bytes(0x78, 0x56, 0x34, 0x12), 0x12345678);
            AssertReadLittleEndian32(Bytes(0xf0, 0xde, 0xbc, 0x9a), 0x9abcdef0);

            AssertReadLittleEndian64(Bytes(0xf0, 0xde, 0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12),
                                     0x123456789abcdef0L);
            AssertReadLittleEndian64(
                Bytes(0x78, 0x56, 0x34, 0x12, 0xf0, 0xde, 0xbc, 0x9a), 0x9abcdef012345678UL);
        }

        [Test]
        public void ReadVarint()
        {
            AssertReadVarint(Bytes(0x00), 0);
            AssertReadVarint(Bytes(0x01), 1);
            AssertReadVarint(Bytes(0x7f), 127);
            // 14882
            AssertReadVarint(Bytes(0xa2, 0x74), (0x22 << 0) | (0x74 << 7));
            // 2961488830
            AssertReadVarint(Bytes(0xbe, 0xf7, 0x92, 0x84, 0x0b),
                             (0x3e << 0) | (0x77 << 7) | (0x12 << 14) | (0x04 << 21) |
                             (0x0bL << 28));

            // 64-bit
            // 7256456126
            AssertReadVarint(Bytes(0xbe, 0xf7, 0x92, 0x84, 0x1b),
                             (0x3e << 0) | (0x77 << 7) | (0x12 << 14) | (0x04 << 21) |
                             (0x1bL << 28));
            // 41256202580718336
            AssertReadVarint(Bytes(0x80, 0xe6, 0xeb, 0x9c, 0xc3, 0xc9, 0xa4, 0x49),
                             (0x00 << 0) | (0x66 << 7) | (0x6b << 14) | (0x1c << 21) |
                             (0x43L << 28) | (0x49L << 35) | (0x24L << 42) | (0x49L << 49));
            // 11964378330978735131
            AssertReadVarint(Bytes(0x9b, 0xa8, 0xf9, 0xc2, 0xbb, 0xd6, 0x80, 0x85, 0xa6, 0x01),
                             (0x1b << 0) | (0x28 << 7) | (0x79 << 14) | (0x42 << 21) |
                             (0x3bUL << 28) | (0x56UL << 35) | (0x00UL << 42) |
                             (0x05UL << 49) | (0x26UL << 56) | (0x01UL << 63));

            // Failures
            AssertReadVarintFailure(
                InvalidProtocolBufferException.MalformedVarint(),
                Bytes(0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
                      0x00));
            AssertReadVarintFailure(
                InvalidProtocolBufferException.TruncatedMessage(),
                Bytes(0x80));
        }

        // Representations of a tag for field 0 with various wire types
        [Test]
        [TestCase(0)]
        [TestCase(1)]
        [TestCase(2)]
        [TestCase(3)]
        [TestCase(4)]
        [TestCase(5)]
        public void ReadTag_ZeroFieldRejected(byte tag)
        {
            try
            {
                ReadTag(new byte[] { tag });
                Assert.Fail();
            }
            catch (InvalidProtocolBufferException)
            {
            }
        }

        [Test]
        public void TestNegativeEnum()
        {
            byte[] bytes = { 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01 };

            AssertEnum(bytes, (int)SampleEnum.NegativeValue);
        }

        [Test]
        public void DecodeZigZag32()
        {
            Assert.AreEqual(0, CodedInputStream.DecodeZigZag32(0));
            Assert.AreEqual(-1, CodedInputStream.DecodeZigZag32(1));
            Assert.AreEqual(1, CodedInputStream.DecodeZigZag32(2));
            Assert.AreEqual(-2, CodedInputStream.DecodeZigZag32(3));
            Assert.AreEqual(0x3FFFFFFF, CodedInputStream.DecodeZigZag32(0x7FFFFFFE));
            Assert.AreEqual(unchecked((int)0xC0000000), CodedInputStream.DecodeZigZag32(0x7FFFFFFF));
            Assert.AreEqual(0x7FFFFFFF, CodedInputStream.DecodeZigZag32(0xFFFFFFFE));
            Assert.AreEqual(unchecked((int)0x80000000), CodedInputStream.DecodeZigZag32(0xFFFFFFFF));
        }

        [Test]
        public void DecodeZigZag64()
        {
            Assert.AreEqual(0, CodedInputStream.DecodeZigZag64(0));
            Assert.AreEqual(-1, CodedInputStream.DecodeZigZag64(1));
            Assert.AreEqual(1, CodedInputStream.DecodeZigZag64(2));
            Assert.AreEqual(-2, CodedInputStream.DecodeZigZag64(3));
            Assert.AreEqual(0x000000003FFFFFFFL, CodedInputStream.DecodeZigZag64(0x000000007FFFFFFEL));
            Assert.AreEqual(unchecked((long)0xFFFFFFFFC0000000L), CodedInputStream.DecodeZigZag64(0x000000007FFFFFFFL));
            Assert.AreEqual(0x000000007FFFFFFFL, CodedInputStream.DecodeZigZag64(0x00000000FFFFFFFEL));
            Assert.AreEqual(unchecked((long)0xFFFFFFFF80000000L), CodedInputStream.DecodeZigZag64(0x00000000FFFFFFFFL));
            Assert.AreEqual(0x7FFFFFFFFFFFFFFFL, CodedInputStream.DecodeZigZag64(0xFFFFFFFFFFFFFFFEL));
            Assert.AreEqual(unchecked((long)0x8000000000000000L), CodedInputStream.DecodeZigZag64(0xFFFFFFFFFFFFFFFFL));
        }

        [Test]
        public void ReadFloat()
        {
            AssertReadFloat(new byte[] { 0, 0, 0, 0 }, 0f);
            AssertReadFloat(new byte[] { 205, 204, 140, 63 }, 1.1f);
            AssertReadFloat(new byte[] { 255, 255, 127, 127 }, float.MaxValue);
            AssertReadFloat(new byte[] { 255, 255, 127, 255 }, float.MinValue);
        }

        [Test]
        public void ReadDouble()
        {
            AssertReadDouble(new byte[] { 0, 0, 0, 0, 0, 0, 0, 0 }, 0f);
            AssertReadDouble(new byte[] { 0, 0, 0, 160, 153, 153, 241, 63 }, 1.1f);
            AssertReadDouble(new byte[] { 255, 255, 255, 255, 255, 255, 239, 127 }, double.MaxValue);
            AssertReadDouble(new byte[] { 255, 255, 255, 255, 255, 255, 239, 255 }, double.MinValue);
        }
    }
}
