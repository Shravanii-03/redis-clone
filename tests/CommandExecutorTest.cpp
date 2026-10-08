#include <gtest/gtest.h>


#include "CommandExecutor.h"
#include "DataStore.h"
#include "ServerStats.h"
#include "ClientSession.h"

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
// ---------------- Authentication ----------------

TEST(CommandExecutorTest, NoPasswordMeansNoAuthNeeded)
{
    DataStore store;
    ServerStats stats;
    ClientSession session;

    CommandExecutor executor(store, stats, &session, "");

    EXPECT_EQ(executor.execute({"SET", "a", "1"}), "OK");
}

TEST(CommandExecutorTest, CommandsRejectedBeforeAuth)
{
    DataStore store;
    ServerStats stats;
    ClientSession session;

    CommandExecutor executor(store, stats, &session, "secret");

    EXPECT_EQ(executor.execute({"SET", "a", "1"}),
              "NOAUTH Authentication required");
    EXPECT_EQ(executor.execute({"GET", "a"}),
              "NOAUTH Authentication required");
    EXPECT_EQ(executor.execute({"PING"}),
              "NOAUTH Authentication required");
    EXPECT_FALSE(session.isAuthenticated());
}

TEST(CommandExecutorTest, WrongPasswordRejected)
{
    DataStore store;
    ServerStats stats;
    ClientSession session;

    CommandExecutor executor(store, stats, &session, "secret");

    EXPECT_EQ(executor.execute({"AUTH", "wrong"}),
              "ERR invalid password");
    EXPECT_FALSE(session.isAuthenticated());
    EXPECT_EQ(executor.execute({"GET", "a"}),
              "NOAUTH Authentication required");
}

TEST(CommandExecutorTest, CorrectPasswordUnlocksSession)
{
    DataStore store;
    ServerStats stats;
    ClientSession session;

    CommandExecutor executor(store, stats, &session, "secret");

    EXPECT_EQ(executor.execute({"AUTH", "secret"}), "OK");
    EXPECT_TRUE(session.isAuthenticated());
    EXPECT_EQ(executor.execute({"SET", "a", "1"}), "OK");
    EXPECT_EQ(executor.execute({"GET", "a"}), "1");
}

TEST(CommandExecutorTest, SessionsAreIndependent)
{
    DataStore store;
    ServerStats stats;
    ClientSession s1, s2;

    CommandExecutor e1(store, stats, &s1, "secret");
    CommandExecutor e2(store, stats, &s2, "secret");

    EXPECT_EQ(e1.execute({"AUTH", "secret"}), "OK");
    EXPECT_EQ(e2.execute({"GET", "a"}),
              "NOAUTH Authentication required");
}

TEST(CommandExecutorTest, AuthWithoutPasswordConfigured)
{
    DataStore store;
    ServerStats stats;
    ClientSession session;

    CommandExecutor executor(store, stats, &session, "");

    EXPECT_EQ(executor.execute({"AUTH", "x"}),
              "ERR Client sent AUTH, but no password is set");
}

TEST(CommandExecutorTest, CommandNamesAreCaseInsensitive)
{
    DataStore store;
    ServerStats stats;

    CommandExecutor executor(store, stats);

    EXPECT_EQ(executor.execute({"set", "k", "v"}), "OK");
    EXPECT_EQ(executor.execute({"get", "k"}), "v");
}