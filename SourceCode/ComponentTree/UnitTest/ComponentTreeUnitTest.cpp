#include <iostream>
#include <algorithm>

#include "gtest/gtest.h"

#include "ImageFourNeighborType.h"
#include "ComponentTree.h"
#include "LCASolver.h"

TEST(ComponentTreeTest, checkComponentTree) {
    vector<ushort> imagePixels = { 1,1,1,1,3,0,0,0,0, // 8
                                   0,0,0,3,2,3,0,0,0, // 17
                                   0,0,3,2,2,2,3,0,0, // 26
                                   0,3,1,3,2,3,1,3,0, // 35
                                   0,0,3,0,2,0,2,0,0, // 44
                                   0,0,0,0,2,0,0,0,0 }; // 53
    ImageFourNeighborType image(imagePixels,9,6);
    ComponentTree tree(image);

    ASSERT_EQ(7,tree.getTotalNodes());

    vector<ushort> component_1 = { 9,10,11,18,19,27,36,37,39,45,46,47,48 };
    vector<ushort> component_2 = { 5,6,7,8,15,16,17,25,26,35,41,43,44,50,51,52,53 };
    vector<ushort> component_3 = { 0,1,2,3 };
    vector<ushort> component_5 = { 33 };
    vector<ushort> component_6 = { 29 };
    vector<ushort> component_7 = { 13,21,22,23,31,40,49 };
    vector<ushort> component_9 = { 4,12,14,20,24,28,30,32,34,38};

    Node *c1 = tree.getComponentMapping().at(component_1.at(0));
    for (auto& element : component_1)
    {
        ASSERT_EQ(c1,tree.getComponentMapping().at(element));
    }

    Node *c2 = tree.getComponentMapping().at(component_2.at(0));
    for (auto& element : component_2)
    {
        ASSERT_EQ(c2,tree.getComponentMapping().at(element));
    }

    Node *c3 = tree.getComponentMapping().at(component_3.at(0));
    for (auto& element : component_3)
    {
        ASSERT_EQ(c3,tree.getComponentMapping().at(element));
    }

    Node *c5 = tree.getComponentMapping().at(component_5.at(0));
    for (auto& element : component_5)
    {
        ASSERT_EQ(c5,tree.getComponentMapping().at(element));
    }

    Node *c6 = tree.getComponentMapping().at(component_6.at(0));
    for (auto& element : component_6)
    {
        ASSERT_EQ(c6,tree.getComponentMapping().at(element));
    }

    Node *c7 = tree.getComponentMapping().at(component_7.at(0));
    for (auto& element : component_7)
    {
        ASSERT_EQ(c7,tree.getComponentMapping().at(element));
    }

    Node *c9 = tree.getComponentMapping().at(component_9.at(0));
    for (auto& element : component_9)
    {
        ASSERT_EQ(c9,tree.getComponentMapping().at(element));
    }

    ASSERT_NE(c1,c2);
    ASSERT_NE(c1,c3);
    ASSERT_NE(c1,c5);
    ASSERT_NE(c1,c6);
    ASSERT_NE(c1,c7);
    ASSERT_NE(c1,c9);

    ASSERT_NE(c2,c3);
    ASSERT_NE(c2,c5);
    ASSERT_NE(c2,c6);
    ASSERT_NE(c2,c7);
    ASSERT_NE(c2,c9);

    ASSERT_NE(c3,c5);
    ASSERT_NE(c3,c6);
    ASSERT_NE(c3,c7);
    ASSERT_NE(c3,c9);

    ASSERT_NE(c5,c6);
    ASSERT_NE(c5,c7);
    ASSERT_NE(c5,c9);

    ASSERT_NE(c6,c7);
    ASSERT_NE(c6,c9);

    ASSERT_NE(c7,c9);

    Node* root = tree.getRoot();

    ASSERT_EQ(c9,root);

    ASSERT_EQ(2,c9->getChilds().size());
    ASSERT_TRUE(c9->getChilds().find(c6) != c9->getChilds().end());
    ASSERT_TRUE(c9->getChilds().find(c7) != c9->getChilds().end());

    ASSERT_EQ(0,c6->getChilds().size());
    ASSERT_EQ(3,c7->getChilds().size());

    ASSERT_TRUE(c7->getChilds().find(c2) != c9->getChilds().end());
    ASSERT_TRUE(c7->getChilds().find(c3) != c9->getChilds().end());
    ASSERT_TRUE(c7->getChilds().find(c5) != c9->getChilds().end());

    ASSERT_EQ(0,c2->getChilds().size());
    ASSERT_EQ(0,c5->getChilds().size());

    ASSERT_EQ(1,c3->getChilds().size());
    ASSERT_EQ(c1,*c3->getChilds().begin());

    NodeSet c1Andc3 = {c1,c3};
    ASSERT_EQ(0,tree.getHighestFork(c1Andc3));

    NodeSet c2Andc3 = {c2,c3};
    ASSERT_EQ(c7,tree.getHighestFork(c2Andc3));

    NodeSet c3Andc5Andc6 = {c2,c3,c6};
    ASSERT_EQ(c9,tree.getHighestFork(c3Andc5Andc6));

    NodeSet c1Andc5 = {c1,c5};
    ASSERT_EQ(c7,tree.getHighestFork(c1Andc5));

    NodeSet c1Andc9 = {c1,c9};
    ASSERT_EQ(0,tree.getHighestFork(c1Andc9));

    NodeSet allButc9 = {c1,c2,c3,c5,c6,c7};
    ASSERT_EQ(c9,tree.getHighestFork(allButc9));

    NodeSet allNodes = {c1,c2,c3,c5,c6,c7,c9};
    ASSERT_EQ(c9,tree.getHighestFork(allNodes));
}

TEST(LCASolverTest, checkLCASolver)
{
    Node* a = new Node(4);

    Node* b = new Node(3);
    Node* c = new Node(3);

    Node* d = new Node(2);
    Node* e = new Node(2);
    Node* f = new Node(2);
    Node* g = new Node(2);
    Node* h = new Node(2);

    Node* i = new Node(1);

    Node* j = new Node(0);

    a->addChild(b);
    a->addChild(c);

    b->addChild(d);
    b->addChild(e);
    b->addChild(f);

    c->addChild(g);
    c->addChild(h);

    f->addChild(i);

    i->addChild(j);

    LCASolver lcaSolver(a);
    ASSERT_EQ(a,lcaSolver.getLCA(a,b));
    ASSERT_EQ(a,lcaSolver.getLCA(a,c));
    ASSERT_EQ(a,lcaSolver.getLCA(b,c));
    ASSERT_EQ(b,lcaSolver.getLCA(d,e));
    ASSERT_EQ(a,lcaSolver.getLCA(d,g));
    ASSERT_EQ(b,lcaSolver.getLCA(d,i));
    ASSERT_EQ(f,lcaSolver.getLCA(j,f));
    ASSERT_EQ(a,lcaSolver.getLCA(h,j));
    ASSERT_EQ(c,lcaSolver.getLCA(g,h));
    ASSERT_EQ(a,lcaSolver.getLCA(a,j));

    delete a;
    delete b;
    delete c;
    delete d;
    delete e;
    delete f;
    delete g;
    delete h;
    delete i;
    delete j;
}
