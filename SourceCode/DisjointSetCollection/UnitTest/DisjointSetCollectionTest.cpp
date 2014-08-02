#include <iostream>
#include <stdexcept>
#include "gtest/gtest.h"

#include "DisjointSetCollection.h"

class DisjointSetCollectionTest : public ::testing::Test
{
protected:
    DisjointSetCollectionTest()
    {
        collection.addNewSet(0);
        collection.addNewSet(1);
        collection.addNewSet(2);
        collection.addNewSet(3);
        collection.addNewSet(4);
        collection.addNewSet(5);
        collection.addNewSet(6);
        collection.addNewSet(7);
        collection.addNewSet(8);
    }

    virtual ~DisjointSetCollectionTest()
    {
    }

    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }

    DisjointSetCollection collection;
};

TEST_F(DisjointSetCollectionTest, newSets) {

    // Check I have 9 disjoint sets
    ASSERT_EQ(0, collection.find(0));
    ASSERT_EQ(1, collection.find(1));
    ASSERT_EQ(2, collection.find(2));
    ASSERT_EQ(3, collection.find(3));
    ASSERT_EQ(4, collection.find(4));
    ASSERT_EQ(5, collection.find(5));
    ASSERT_EQ(6, collection.find(6));
    ASSERT_EQ(7, collection.find(7));
    ASSERT_EQ(8, collection.find(8));
}

TEST_F(DisjointSetCollectionTest, findUnexistingElement) {

    // Should throw exception if element does not exists
    ASSERT_THROW(collection.find(9), std::invalid_argument);
}

TEST_F(DisjointSetCollectionTest, LinkNewSets) {
    unsigned int canonicalElement = collection.linkSets(0,1);

    // Check new canonicalElement of set {0,1} is 0 or 1
    ASSERT_TRUE(0 == canonicalElement || 1 == canonicalElement);

    // Check 0 and 1 belongs to the same set
    ASSERT_EQ(canonicalElement, collection.find(0));
    ASSERT_EQ(canonicalElement, collection.find(1));
}

TEST_F(DisjointSetCollectionTest, LinkAlreadyLinkedSets) {

    // Linking sets {0} and {1}
    collection.linkSets(0,1);
    // Check exception while linking already linked sets
    ASSERT_THROW(collection.linkSets(0,1), std::invalid_argument);
}

TEST_F(DisjointSetCollectionTest, LinkUnexistingSets) {

    // Linking unexisting elements
    ASSERT_THROW(collection.linkSets(9,10), std::invalid_argument);
}



