//
//  Vertex.h
//  ReactionGenerator
//
//  Created by Saman Moein on 4/21/15.
//  Copyright (c) 2015 Saman Moein. All rights reserved.
//

#ifndef __ReactionGenerator__Graph__
#define __ReactionGenerator__Graph__

#include <iostream>
#include <vector>


using namespace std;


struct ver;

/*  adjacents to a chemical can be either the other reactors in a reaction or another group of products. if there is more than one reactor or product we will use a dummy node ( wroking as the plus sign in the reaction ) to connect all the chemicals. So in A+B -> C+D, we have (A) - (+) - (B) and (C) - (+) - (D) and there is a pointer from the first (+) to the second. */
struct adj
{
    bool visited = false;
    bool dif_side = 0 ;
    int H;
    ver * parent;
    ver * toself;
public:
    adj(ver * in_toself, ver * in_parent, bool in_dif_side ): toself(in_toself), parent(in_parent), dif_side(in_dif_side) {}
    adj(): toself(nullptr), parent(nullptr) {}
    friend class graph;
};


struct ver // each chemical is a vertex
{
    string name;//do not initilize the name in case it's a plus sign.
    vector<adj *> adjs;
    bool availabe = 0;
    bool plussign = false;//true if it's a plus
friend class graph;
};



class graph
{
private:
    static int plusCounter;
    vector<ver *> vertices;// vector of chemicals
public:
    void make_adjs_av( ver * );
    bool all_av ( ver * );
    void reactionFinder(ver * , ver *,vector<vector<adj*>> &, vector<adj*> &, ver *,vector<adj*> );
    vector<vector<adj*>> * reactionGenerator (vector<ver *> &, vector<ver *> & );
    void print_paths(vector<vector<adj*>> &);
    void build_graph(const string &);
    void make_firstState(vector<ver *> &);
    void makeReactors_av(vector<ver *>&);
    void make_default();
    void addRXN(const string & );
    void addVertice(const string &,vector<ver *> &);
    void addAdj(vector<ver *> & , const int & , ver *, ver *);
    void namesList(vector<ver *> &);
    bool inPath( vector<adj*> &, adj *);
    bool isReactor ( ver * , ver * );
    ver * sameGroup( vector<ver *>  &, const int &, bool );
    void block_formerStep(ver * chem,ver * formerStep);
    void refine(vector<adj*>&);
    bool isUsed( ver * , ver * );
    void rearrange();

};




#endif /* defined(__ReactionGenerator__Graph__) */
