// DESCRIPTION:   Unit tests for UserDefinedSerial
//
// COPYRIGHT:     University of California San Francisco, 2014
//
// LICENSE:       This file is distributed under the BSD license.
//                License text is included with the source distribution.
//
//                This file is distributed in the hope that it will be useful,
//                but WITHOUT ANY WARRANTY; without even the implied warranty
//                of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
//                IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//                CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//                INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES.
//
// AUTHOR:        Mark Tsuchida

#include <gtest/gtest.h>

#include "UserDefinedSerial.h"


// EscapedStringFromByteString() is simple, already tested, and trustworthy, so
// use it to simplify these tests (since it is a bother to code
// std::vector<char> output).
class ParameterizedUnescapeTest : public ::testing::Test,
   public ::testing::WithParamInterface< std::pair< std::string,
   std::pair<int, std::string> > >
{
protected:
   std::string input_;
   int expectedRetVal_;
   std::string expectedReescapedOutput_;
   virtual void SetUp()
   {
      input_ = GetParam().first;
      expectedRetVal_ = GetParam().second.first;
      expectedReescapedOutput_ = GetParam().second.second;
   }
};

TEST_P(ParameterizedUnescapeTest, MatchWithReescaped)
{
   std::vector<char> output;
   int err = ByteStringFromEscapedString(input_, output);
   ASSERT_EQ(expectedRetVal_, err);
   ASSERT_EQ(expectedReescapedOutput_,
         EscapedStringFromByteString(output));
}

template <typename T, typename U, typename V>
std::pair< std::string, std::pair<int, std::string> >
mktesttriple(T t, U u, V v)
{ return std::make_pair(t, std::make_pair(u, v)); }


INSTANTIATE_TEST_CASE_P(BasicTestCase, ParameterizedUnescapeTest,
      ::testing::Values(
         mktesttriple("", DEVICE_OK, ""),
         mktesttriple("x", DEVICE_OK, "x"),
         mktesttriple("xy", DEVICE_OK, "xy"),
         mktesttriple("xyz", DEVICE_OK, "xyz"),
         mktesttriple("xx", DEVICE_OK, "xx"),
         mktesttriple("xxx", DEVICE_OK, "xxx")
      ));

INSTANTIATE_TEST_CASE_P(SingleEscapeTestCase, ParameterizedUnescapeTest,
      ::testing::Values(
         mktesttriple("\\'", DEVICE_OK, "'"),
         mktesttriple("\\\"", DEVICE_OK, "\""),
         mktesttriple("\\?", DEVICE_OK, "?"),
         mktesttriple("\\\\", DEVICE_OK, "\\\\"),
         mktesttriple("\\a", DEVICE_OK, "\\x07"),
         mktesttriple("\\b", DEVICE_OK, "\\x08"),
         mktesttriple("\\f", DEVICE_OK, "\\x0c"),
         mktesttriple("\\n", DEVICE_OK, "\\n"),
         mktesttriple("\\r", DEVICE_OK, "\\r"),
         mktesttriple("\\t", DEVICE_OK, "\\x09"),
         mktesttriple("\\v", DEVICE_OK, "\\x0b")
      ));

INSTANTIATE_TEST_CASE_P(OctalEscapeTestCase, ParameterizedUnescapeTest,
      ::testing::Values(
         mktesttriple("\\0", DEVICE_OK, "\\x00"),
         mktesttriple("\\0x", DEVICE_OK, "\\x00x"),
         mktesttriple("\\00", DEVICE_OK, "\\x00"),
         mktesttriple("\\00x", DEVICE_OK, "\\x00x"),
         mktesttriple("\\000", DEVICE_OK, "\\x00"),
         mktesttriple("\\000x", DEVICE_OK, "\\x00x"),
         mktesttriple("\\0000", DEVICE_OK, "\\x000"),
         mktesttriple("\\00000", DEVICE_OK, "\\x0000")
      ));

INSTANTIATE_TEST_CASE_P(HexEscapeTestCase, ParameterizedUnescapeTest,
      ::testing::Values(
         mktesttriple("\\x0", DEVICE_OK, "\\x00"),
         mktesttriple("\\x0x", DEVICE_OK, "\\x00x"),
         mktesttriple("\\x00", DEVICE_OK, "\\x00"),
         mktesttriple("\\x00x", DEVICE_OK, "\\x00x"),
         mktesttriple("\\x000", DEVICE_OK, "\\x000"),
         mktesttriple("\\x000x", DEVICE_OK, "\\x000x"),
         mktesttriple("\\x0000", DEVICE_OK, "\\x0000"),
         mktesttriple("\\x00000", DEVICE_OK, "\\x00000")
      ));


int main(int argc, char **argv)
{
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}