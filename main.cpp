#include "gtest/gtest.h"

#include "my_set.h"

TEST(correctness, single_element_insert)
{
    Set<int> q;
    q.insert(1);
}

TEST(correctness, several_inserts)
{
    Set<int> q;
    q.insert(1);
    q.insert(2);
    q.insert(3);
}

TEST(correctness, iterator_inc)
{
    Set<int> q;
    q.insert(1);
    q.insert(2);
    q.insert(3);

    ASSERT_EQ(*(++q.begin()), 2);
    auto it = (++q.begin());
    it++;
    ASSERT_EQ(*it, 3);
}

TEST(correctness, iterator_dec)
{
    Set<int> q;
    q.insert(1);
    q.insert(2);
    q.insert(3);

    ASSERT_EQ(*(--q.end()), 3);
    auto it = (--q.end());
    it--;
    ASSERT_EQ(*it, 2);
}

TEST(correctness, empty)
{
    Set<int> q;
    ASSERT_TRUE(q.empty());
    q.insert(5);
    ASSERT_TRUE(!q.empty());
}

TEST(correctness, erase)
{
    Set<int> q;
    q.insert(1);
    q.insert(2);
    q.insert(3);

    q.erase(q.begin());
    ASSERT_EQ(*(q.begin()), 2);
}


TEST(correctness, erase_all_childs)
{
    Set<int> q;
    q.insert(2);
    q.insert(1);
    q.insert(4);
    q.insert(3);
    q.insert(5);

    q.erase(++(q.begin()));
    ASSERT_EQ(*(++(q.begin())), 3);
    ASSERT_EQ(*(++(++(q.begin()))), 4);
}

#include <iostream>

TEST(correctness, andrey_test)
{
    Set<int> q;
    q.insert(5);
    q.insert(2);
    q.insert(10);
    q.insert(6);
    q.insert(14);
    q.insert(7);
    q.insert(8);

    for (auto kek : q)
        std::cout << kek << std::endl;

}
/*
TEST(correctness, find)
{
    Set<int> q;


}*/