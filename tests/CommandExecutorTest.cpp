#include <gtest/gtest.h>

#include "CommandExecutor.h"
#include "DataStore.h"
#include "ServerStats.h"

TEST(CommandExecutorTest, Ping)
{
    DataStore store;
    ServerStats stats;

    CommandExecutor executor(
        store,
        stats);

    EXPECT_EQ(
        executor.execute({"PING"}),
        "PONG");
}

TEST(CommandExecutorTest, SetGet)
{
    DataStore store;
    ServerStats stats;

    CommandExecutor executor(
        store,
        stats);

    EXPECT_EQ(
        executor.execute(
            {"SET","name","Shravani"}),
        "OK");

    EXPECT_EQ(
        executor.execute(
            {"GET","name"}),
        "Shravani");
}

TEST(CommandExecutorTest, Delete)
{
    DataStore store;
    ServerStats stats;

    CommandExecutor executor(
        store,
        stats);

    executor.execute(
        {"SET","x","100"});

    EXPECT_EQ(
        executor.execute(
            {"DEL","x"}),
        "1");
}