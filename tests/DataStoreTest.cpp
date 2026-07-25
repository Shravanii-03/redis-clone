#include <gtest/gtest.h>

#include "DataStore.h"

TEST(DataStoreTest, SetAndGet)
{
    DataStore store;

    store.set("name", "Shravani");

    EXPECT_EQ(
        store.get("name"),
        "Shravani");
}

TEST(DataStoreTest, MissingKey)
{
    DataStore store;

    EXPECT_EQ(
        store.get("unknown"),
        "(nil)");
}

TEST(DataStoreTest, DeleteExistingKey)
{
    DataStore store;

    store.set("city", "Bangalore");

    EXPECT_TRUE(
        store.del("city"));

    EXPECT_EQ(
        store.get("city"),
        "(nil)");
}

TEST(DataStoreTest, DeleteMissingKey)
{
    DataStore store;

    EXPECT_FALSE(
        store.del("missing"));
}

TEST(DataStoreTest, Keys)
{
    DataStore store;

    store.set("a", "1");
    store.set("b", "2");

    auto keys = store.keys();

    EXPECT_EQ(
        keys.size(),
        2);
}