#include "deque.h"
#include "gtest\gtest.h"
#include <algorithm>

void testTime(const int MAXN){
    Deque <int> d;
    int ind = 0;
    for (int i = 0; i < MAXN; ++i) {
        d.push_back(i);
    }
    while (!d.empty()) {
        d.pop_front();
    }
}

TEST(testTime0, CheckIt) {
    testTime(100000);
}
TEST(testTime1, CheckIt) {
    testTime(1000000);
}
TEST(testTime2, CheckIt) {
    testTime(10000000);
}
TEST(testTime3, CheckIt) {
    testTime(100000000);
}

TEST(testReverseIterator, CheckIt) {
    const int MAXN = 10000000;

    Deque <int> d;
    int ind = 0;
    for (int i = 0; i < MAXN; ++i) {
        d.push_back(i);
    }
    for (Deque <int>::reverse_iterator i = d.rend() - 1; i - d.rbegin() > 0; --i) {
        ASSERT_EQ(*i, ind);
        ++ind;
    }
}
TEST(testIterator, CheckDeque) {
    const int MAXN = 10000000;
    Deque <int> d;
    for (int i = 0; i < MAXN; ++i) {
        d.push_back(i);
    }
    int ind = 0;
    for (Deque <int>::iterator i = d.begin(); i != d.end(); ++i, ++ind) {
        ASSERT_EQ(*i, ind);
    }
    ind = MAXN - 1;
    for (Deque <int>::reverse_iterator i = d.rbegin(); i != d.rend(); ++i, --ind) {
        ASSERT_EQ(*i, ind);
    }

    ind = 0;
    for (Deque <int>::iterator i = d.begin(); i < d.end(); i += 10, ind += 10) {
        ASSERT_EQ(*i, ind);
    }
    ind = MAXN - 1;
    for (Deque <int>::reverse_iterator i = d.rbegin(); i < d.rend(); i += 10, ind += 10) {
        ASSERT_EQ(*i, ind);
    }
}
TEST(testPushPop, CheckDeque) {
    const int MAXN = 10000000;
    Deque <int> d;
    for (int i = 0; i < MAXN; ++i) {
        if (rand() & 1) {
            if (rand() & 1) {
                d.push_back(1);
            }
            else {
                d.push_front(1);
            }
        }
        else {
            if (!d.empty()) {
                if (rand() & 1) {
                    d.pop_back();
                }
                else {
                    d.pop_front();
                }
            }
        }

        ASSERT_EQ((d.end() - d.begin()), d.size());
        ASSERT_EQ((d.rend() - d.rbegin()), d.size());
    }

    for (int i = 0; i < d.size(); i++) {
        ASSERT_EQ(d[i], 1);
    }
}
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
