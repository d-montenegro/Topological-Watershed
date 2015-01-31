#include <iostream>
#include <algorithm>

#include "gtest/gtest.h"

#include "ImageFourNeighborType.h"
#include "ComponentTree.h"
#include "LCASolver.h"

namespace {

vector<ushort> getComponent(const ComponentMapping& componentMapping,
                            Node* node)
{
    vector<ushort> result;
    for(auto& element : componentMapping)
    {
        if(element.second == node)
        {
            result.push_back(element.first);
        }
    }

    return result;
}

} // anonymous namespace

TEST(ComponentTreeTest, checkComponentTree_0) {
    vector<ushort> imagePixels = { 1,1,1,1,3,0,0,0,0, // 8
                                   0,0,0,3,2,3,0,0,0, // 17
                                   0,0,3,2,2,2,3,0,0, // 26
                                   0,3,1,3,2,3,1,3,0, // 35
                                   0,0,3,0,2,0,2,0,0, // 44
                                   0,0,0,0,2,0,0,0,0 }; // 53
    ImageFourNeighborType image(imagePixels,9,6);
    ComponentTree tree(image);

    ASSERT_EQ(7,tree.getTotalNodes());

    Node *c1 = tree.getComponentMapping().at(9);
    Node *c2 = tree.getComponentMapping().at(5);
    Node *c3 = tree.getComponentMapping().at(0);
    Node *c5 = tree.getComponentMapping().at(33);
    Node *c6 = tree.getComponentMapping().at(29);
    Node *c7 = tree.getComponentMapping().at(13);
    Node *c9 = tree.getComponentMapping().at(4);

    ASSERT_EQ(vector<ushort>({ 9,10,11,18,19,27,36,37,39,45,46,47,48 }),
        getComponent(tree.getComponentMapping(),c1));

    ASSERT_EQ(vector<ushort>({ 5,6,7,8,15,16,17,25,26,35,41,43,44,50,51,52,53 }),
        getComponent(tree.getComponentMapping(),c2));

    ASSERT_EQ(vector<ushort>({ 0,1,2,3 }),
        getComponent(tree.getComponentMapping(),c3));

    ASSERT_EQ(vector<ushort>({ 33 }),
        getComponent(tree.getComponentMapping(),c5));

    ASSERT_EQ(vector<ushort>({ 29 }),
        getComponent(tree.getComponentMapping(),c6));

    ASSERT_EQ(vector<ushort>({ 13,21,22,23,31,40,42,49 }),
        getComponent(tree.getComponentMapping(),c7));

    ASSERT_EQ(vector<ushort>({ 4,12,14,20,24,28,30,32,34,38 }),
        getComponent(tree.getComponentMapping(),c9));

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


TEST(ComponentTreeTest, checkComponentTree_1) {

    vector<ushort> pixels = { 0,9,0,0,0,8,8,2,2,2,5,3,3,
                              0,9,6,5,8,8,8,8,2,7,7,7,3,
                              0,9,1,1,8,9,8,8,2,7,7,3,3,
                              0,9,9,7,8,8,8,8,2,7,7,7,3,
                              0,9,0,0,0,8,8,5,2,7,7,5,3,
                              0,9,9,0,0,8,8,5,2,7,7,5,3 };

    ImageFourNeighborType image(pixels,13,6);
    ComponentTree tree(image);

    ASSERT_EQ(13, tree.getTotalNodes());

    Node* a = tree.getComponentMapping().at(0);
    Node* b = tree.getComponentMapping().at(2);
    Node* c = tree.getComponentMapping().at(54);
    Node* d = tree.getComponentMapping().at(28);
    Node* e = tree.getComponentMapping().at(7);
    Node* f = tree.getComponentMapping().at(11);
    Node* g = tree.getComponentMapping().at(16);
    Node* h = tree.getComponentMapping().at(10);
    Node* i = tree.getComponentMapping().at(15);
    Node* j = tree.getComponentMapping().at(42);
    Node* k = tree.getComponentMapping().at(22);
    Node* l = tree.getComponentMapping().at(5);
    Node* m = tree.getComponentMapping().at(1);

    // Check component mapping
    ASSERT_EQ(vector<ushort>({ 0,13,26,39,52,65 }),
        getComponent(tree.getComponentMapping(),a));

    ASSERT_EQ(vector<ushort>({ 2,3,4 }),
        getComponent(tree.getComponentMapping(),b));

    ASSERT_EQ(vector<ushort>({ 54,55,56,68,69 }),
        getComponent(tree.getComponentMapping(),c));

    ASSERT_EQ(vector<ushort>({ 28,29 }),
        getComponent(tree.getComponentMapping(),d));

    ASSERT_EQ(vector<ushort>({ 7,8,9,21,34,47,60,73 }),
        getComponent(tree.getComponentMapping(),e));

    ASSERT_EQ(vector<ushort>({ 11,12,25,37,38,51,64,77 }),
        getComponent(tree.getComponentMapping(),f));

    ASSERT_EQ(vector<ushort>({ 16 }),getComponent(tree.getComponentMapping(),g));

    ASSERT_EQ(vector<ushort>({ 10,59,63,72,76 }),
        getComponent(tree.getComponentMapping(),h));

    ASSERT_EQ(vector<ushort>({ 15 }),
        getComponent(tree.getComponentMapping(),i));

    ASSERT_EQ(vector<ushort>({ 42 }),
        getComponent(tree.getComponentMapping(),j));

    ASSERT_EQ(vector<ushort>({ 22,23,24,35,36,48,49,50,61,62,74,75 }),
        getComponent(tree.getComponentMapping(),k));

    ASSERT_EQ(vector<ushort>({ 5,6,17,18,19,20,30,32,33,43,44,45,46,57,58,70,71 }),
        getComponent(tree.getComponentMapping(),l));

    ASSERT_EQ(vector<ushort>({ 1,14,27,31,40,41,53,66,67 }),
        getComponent(tree.getComponentMapping(),m));

    // Check root
    ASSERT_EQ(m, tree.getRoot());
    ASSERT_EQ(2, m->getChilds().size());
    ASSERT_EQ(10, m->getLevel());
    ASSERT_NE(m->getChilds().end(), m->getChilds().find(a));
    ASSERT_NE(m->getChilds().end(), m->getChilds().find(l));

    ASSERT_EQ(2, l->getChilds().size());
    ASSERT_EQ(9, l->getLevel());
    ASSERT_NE(l->getChilds().end(), l->getChilds().find(j));
    ASSERT_NE(l->getChilds().end(), l->getChilds().find(k));

    ASSERT_EQ(2, j->getChilds().size());
    ASSERT_EQ(8, j->getLevel());
    ASSERT_NE(j->getChilds().end(), j->getChilds().find(i));
    ASSERT_NE(j->getChilds().end(), j->getChilds().find(c));

    ASSERT_EQ(1, k->getChilds().size());
    ASSERT_EQ(8, k->getLevel());
    ASSERT_NE(k->getChilds().end(), j->getChilds().find(h));

    ASSERT_EQ(1, i->getChilds().size());
    ASSERT_EQ(7, i->getLevel());
    ASSERT_NE(i->getChilds().end(), i->getChilds().find(g));

    ASSERT_EQ(2, g->getChilds().size());
    ASSERT_EQ(6, g->getLevel());
    ASSERT_NE(g->getChilds().end(), g->getChilds().find(b));
    ASSERT_NE(g->getChilds().end(), g->getChilds().find(d));

    ASSERT_EQ(2, h->getChilds().size());
    ASSERT_EQ(6, h->getLevel());
    ASSERT_NE(h->getChilds().end(), h->getChilds().find(e));
    ASSERT_NE(h->getChilds().end(), h->getChilds().find(f));

    ASSERT_EQ(0, f->getChilds().size());
    ASSERT_EQ(4, f->getLevel());

    ASSERT_EQ(0, e->getChilds().size());
    ASSERT_EQ(3, e->getLevel());

    ASSERT_EQ(0, d->getChilds().size());
    ASSERT_EQ(2, d->getLevel());

    ASSERT_EQ(0, c->getChilds().size());
    ASSERT_EQ(1, c->getLevel());

    ASSERT_EQ(0, b->getChilds().size());
    ASSERT_EQ(1, b->getLevel());

    ASSERT_EQ(0, a->getChilds().size());
    ASSERT_EQ(1, a->getLevel());
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


TEST(ComponentTreeTest, checkComponentTree_2) {
    vector<ushort> pixels = { 255,255,255,255,255,
                              255,63,61,59,58,
                              255,58,57,56,56,
                              255,57,58,57,56,
                              255,56,57,57,56 };

    ImageFourNeighborType image(pixels,5,5);
    ComponentTree tree(image);

    ASSERT_EQ(8,tree.getTotalNodes());

    Node *z = tree.getComponentMapping().at(0);
    Node *i = tree.getComponentMapping().at(6);
    Node *g = tree.getComponentMapping().at(7);
    Node *e = tree.getComponentMapping().at(8);
    Node *d = tree.getComponentMapping().at(9);
    Node *c = tree.getComponentMapping().at(12);
    Node *b = tree.getComponentMapping().at(13);
    Node *a = tree.getComponentMapping().at(21);

    ASSERT_EQ(vector<ushort>({ 0,1,2,3,4,5,10,15,20 }),
        getComponent(tree.getComponentMapping(),z));

    ASSERT_EQ(vector<ushort>({ 6 }),
        getComponent(tree.getComponentMapping(),i));

    ASSERT_EQ(vector<ushort>({ 7 }),
        getComponent(tree.getComponentMapping(),g));

    ASSERT_EQ(vector<ushort>({ 8 }),
        getComponent(tree.getComponentMapping(),e));

    ASSERT_EQ(vector<ushort>({ 9,11,17 }),
        getComponent(tree.getComponentMapping(),d));

    ASSERT_EQ(vector<ushort>({ 12,16,18,22,23 }),
        getComponent(tree.getComponentMapping(),c));

    ASSERT_EQ(vector<ushort>({ 13,14,19,24 }),
        getComponent(tree.getComponentMapping(),b));

    ASSERT_EQ(vector<ushort>({ 21 }),
        getComponent(tree.getComponentMapping(),a));

    ASSERT_NE(z,a);
    ASSERT_NE(z,b);
    ASSERT_NE(z,c);
    ASSERT_NE(z,d);
    ASSERT_NE(z,e);
    ASSERT_NE(z,g);
    ASSERT_NE(z,i);

    ASSERT_NE(i,a);
    ASSERT_NE(i,b);
    ASSERT_NE(i,c);
    ASSERT_NE(i,d);
    ASSERT_NE(i,e);
    ASSERT_NE(i,g);

    ASSERT_NE(g,a);
    ASSERT_NE(g,b);
    ASSERT_NE(g,c);
    ASSERT_NE(g,d);
    ASSERT_NE(g,e);

    ASSERT_NE(e,a);
    ASSERT_NE(e,b);
    ASSERT_NE(e,c);
    ASSERT_NE(e,d);

    ASSERT_NE(d,a);
    ASSERT_NE(d,b);
    ASSERT_NE(d,c);

    ASSERT_NE(c,a);
    ASSERT_NE(c,b);

    ASSERT_NE(b,a);

    ASSERT_EQ(z,tree.getRoot());

    ASSERT_EQ(1,z->getChilds().size());
    ASSERT_EQ(i,*z->getChilds().begin());

    ASSERT_EQ(1,i->getChilds().size());
    ASSERT_EQ(g,*i->getChilds().begin());

    ASSERT_EQ(1,g->getChilds().size());
    ASSERT_EQ(e,*g->getChilds().begin());

    ASSERT_EQ(1,e->getChilds().size());
    ASSERT_EQ(d,*e->getChilds().begin());

    ASSERT_EQ(1,d->getChilds().size());
    ASSERT_EQ(c,*d->getChilds().begin());

    ASSERT_EQ(2,c->getChilds().size());
    ASSERT_TRUE(c->getChilds().find(a) != c->getChilds().end());
    ASSERT_TRUE(c->getChilds().find(b) != c->getChilds().end());

    ASSERT_EQ(0,b->getChilds().size());
    ASSERT_EQ(0,a->getChilds().size());

    NodeSet aAndb = {a,b};
    ASSERT_EQ(c,tree.getHighestFork(aAndb));

    NodeSet eAndg = {e,g};
    ASSERT_EQ(0,tree.getHighestFork(eAndg));
}
