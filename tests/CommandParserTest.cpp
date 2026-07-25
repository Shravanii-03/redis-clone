#include <gtest/gtest.h>

#include "CommandParser.h"

TEST(CommandParserTest, ParsePing)
{
    CommandParser parser;

    auto tokens =
        parser.parse("PING");

    ASSERT_EQ(tokens.size(), 1);

    EXPECT_EQ(tokens[0], "PING");
}

TEST(CommandParserTest, ParseSet)
{
    CommandParser parser;

    auto tokens =
        parser.parse(
            "SET name Shravani");

    ASSERT_EQ(tokens.size(), 3);

    EXPECT_EQ(tokens[0], "SET");
    EXPECT_EQ(tokens[1], "name");
    EXPECT_EQ(tokens[2], "Shravani");
}

TEST(CommandParserTest, ParseGet)
{
    CommandParser parser;

    auto tokens =
        parser.parse("GET name");

    ASSERT_EQ(tokens.size(), 2);

    EXPECT_EQ(tokens[0], "GET");
    EXPECT_EQ(tokens[1], "name");
}