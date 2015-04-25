//
//  Vertex.cpp
//  ReactionGenerator
//
//  Created by Saman Moein on 4/21/15.
//  Copyright (c) 2015 Saman Moein. All rights reserved.
//

#include "Graph.h"
#include <sstream>

void graph::make_adjs_av( ver * plus )
{
    plus->availabe = true;
    for( size_t i(0); i < plus->adjs.size(); i++)
        plus->adjs[i].toself->availabe = true;
}

bool graph::all_av(ver * plus)
{
    for( size_t i(0); i < plus->adjs.size(); i++)
        if(!plus->adjs[i].toself->availabe && plus->adjs[i].dif_side == 0 )
            return false;
    return true;
}
ver * graph::find_make_av(ver * reactors)
{
    ver * found(nullptr);
    for( size_t i(0); i < vertices.size(); i++)
    {
        if( vertices[i].name == reactors->name )
        {
            found = &vertices[i];// this should be a vector of pointers and we will add founds to the vector.
            found->availabe = 1;
        }
    }
    return found;
}
vector<vector<adj*>> * graph::reactionGenerator (vector<ver *> & reactors, vector<ver *> & products )
{

    vector<vector<adj*>> * paths = new vector<vector<adj*>>;
    vector<adj*> path;
    for( ver * reactor : reactors )
    {
        for( ver * product : products )
        {
            bool found(false);
            reactionFinder(reactor, product, *paths, found,path);
            if (!found)
                return nullptr;
        }
    }

    return paths;


}

void graph::reactionFinder( ver * reactors, ver * product,vector<vector<adj*> > & paths, bool & found,vector<adj*> & path)
{
        ver * chem = reactors ;
        if(product->name == chem->name)
        {
            paths.push_back(path);
            path.clear();
            found = true;
        }

        for( adj & A : chem->adjs )
        {
            if (!A.visited)
            {
                A.visited = 1;

                if( A.toself->plus )
                    path.push_back(&A);


                if (A.dif_side == 1)
                {
                    make_adjs_av(A.toself);
                    reactionFinder(A.toself, product, paths, found,path);
                }
                else
                {
                    if (A.toself->plus && all_av(A.toself))
                        reactionFinder(A.toself, product, paths, found,path);
                    if(!A.toself->plus)
                        reactionFinder(A.toself, product, paths, found,path);
                }
            }
        }

}

void graph::print_paths(vector<vector<adj*>> & paths)
{
    int i(1);
    for( vector<adj*> & path : paths )
    {
        cout << i << " :" << endl;//These can be sorted by total cost,number of reactions, change in H,S,G etc. USING A TREE.
        int j(1);
        for(adj * step : path )
        {
            cout << "   "<< j << ". ";
            if( step->parent->plus )
            {
                vector<adj>::iterator rct(step->parent->adjs.begin());
                for(; rct != step->parent->adjs.end()-1; rct++ )
                    cout << rct->toself->name << " + ";
                cout << rct->toself->name;
            }
            else
                cout << step->parent->name;

            cout << " --> ";//Used catalyst can be reported here: a good way to do that : A + B --Cat--> C + D

            if( step->toself->plus )
            {
                vector<adj>::iterator rct(step->toself->adjs.begin());
                for(; rct != step->toself->adjs.end()-1; rct++ )
                    cout << rct->toself->name << " + ";
                cout << rct->toself->name;
            }
            else
                cout << step->toself->name;

            cout << " Change in Enthalpy: " << step->H;  //many other things such as change in S,G, cost etc. can be reported here.
            cout << endl;
            j++;

        }
        i++;
        cout << endl; // The total change in H,S,G total cost etc. can be reported here.
    }


}


void graph::addRXN(string reactionLine){
    istringstream splitted(reactionLine);
    vector <string> splittedVec;
    do {
        string current;
        splitted >> current;
        splittedVec.push_back(current);
        cout << "test: " << current << endl;
    } while(splitted);



}
void graph::addVertice(string name){
    ver temp;
    ver.name = name;
    if (name == "+"){
        plus = true;
    }
}


//void build_graph(const string & fileD)
//{
    //build graph using the file directory in the command line
//}

