//
//  Vertex.cpp
//  ReactionGenerator
//
//  Created by Saman Moein on 4/21/15.
//  Copyright (c) 2015 Saman Moein. All rights reserved.
//

#include "Graph.h"
#include <sstream>
#include <fstream>
#include <string>


void graph::make_adjs_av( ver * plus )
{
    plus->availabe = true;
    for( size_t i(0); i < plus->adjs.size(); i++)
        plus->adjs[i].toself->availabe = true;
}

bool graph::all_av(ver * plussign)
{
    for( size_t i(0); i < plussign->adjs.size(); i++)
        if(!plussign->adjs[i].toself->availabe && plussign->adjs[i].dif_side == 0 )
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
        reactor->availabe = 1;
        for( ver * product : products )
        {
            bool found(false);
            reactionFinder(reactor, product, *paths, found,path);
            if (!found)
                return nullptr;
        }
        make_firstState(reactors);
    }
    
    return paths;
    
    
}

void graph::reactionFinder( ver * reactors, ver * product,vector<vector<adj*>> & paths, bool & found,vector<adj*> & path)
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
            
            if( A.toself->plussign )
                path.push_back(&A);
            
            
            if (A.dif_side == 1)
            {
                make_adjs_av(A.toself);
                reactionFinder(A.toself, product, paths, found,path);
            }
            else
            {
                if (A.toself->plussign && all_av(A.toself))
                    reactionFinder(A.toself, product, paths, found,path);
                if(!A.toself->plussign)
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
            if( step->parent->plussign )
            {
                vector<adj>::iterator rct(step->parent->adjs.begin());
                for(; rct != step->parent->adjs.end()-1; rct++ )
                    cout << rct->toself->name << " + ";
                cout << rct->toself->name;
            }
            else
                cout << step->parent->name;
            
            cout << " --> ";//Used catalyst can be reported here: a good way to do that : A + B --Cat--> C + D
            
            if( step->toself->plussign )
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

void graph::make_firstState(vector<ver *> & reactors)
{
    bool is_reactor(0);
    for( ver & chem : vertices )
    {
        for( ver * reactor : reactors )
            if( reactor->name == chem.name )
            {
                is_reactor = 1;
                break;
            }
            else
                chem.availabe = 0;
        for( adj & adjchem : chem.adjs )
            adjchem.visited = 0;
    }
}

void graph::make_default()
{
    for( ver & chem : vertices )
    {
        chem.availabe = 0;
        for( adj & adjchem : chem.adjs )
            adjchem.visited = 0;
    }
}

void graph::addRXN(const string & reactionLine)
{
    stringstream splitted(reactionLine);
    vector <string> splittedVec;
    int pro_pos(0);
    int pos(0);
    do {
        string current;
        splitted >> current ;
        if(current == ">" )
            pro_pos = pos;
        else if( current != "+" )
        {
            splittedVec.push_back(current);
            pos++;
        }
    } while(!splitted.eof());
    
    vector<ver *> reactionVec;
    for( string splitted2 : splittedVec )
    {
        bool found(0);
        for( ver & chem : vertices )
        {
            if ( chem.name == splitted2 )
            {
                reactionVec.push_back(&chem);
                found = 1;
                break;
            }
        }
        if( !found )
            addVertice(splitted2,reactionVec);
    }
    if( pro_pos > 1 )
        addVertice("+",reactionVec);
    addVertice("+",reactionVec);
    addAdj(reactionVec, pro_pos);
}

void graph::addVertice(const string & name,vector<ver *> & reactionVec )
{
    ver temp;
    temp.name = name;
    if (name == "+")
        temp.plussign = true;
    vertices.push_back(temp);
    if (name != "+")
        reactionVec.push_back(&vertices[vertices.size()-1]);
}

void graph::addAdj(vector<ver *> & reactionVec, const int & pro_pos )
{
    
    size_t productPlus(vertices.size()-1);
    if( pro_pos == 1 )
    {
        adj Plusadj(reactionVec[productPlus], reactionVec[0], 1);
        reactionVec[0]->adjs.push_back(Plusadj);
        for (size_t i(1); i < reactionVec.size(); i++)
        {
            adj chemadj(reactionVec[i],nullptr, 0);
            vertices[productPlus].adjs.push_back(chemadj);
            adj plusadj(&vertices[productPlus], nullptr, 0);
            reactionVec[i]->adjs.push_back(plusadj);
        }
        
    }
    else
    {
        adj plusadjP( &vertices[productPlus], &vertices[productPlus-1], 1);
        vertices[productPlus-1].adjs.push_back(plusadjP);
        for (size_t i(0); i < pro_pos; i++)
        {
            adj chemadj(reactionVec[i],nullptr, 0);
            reactionVec[i]->adjs.push_back(chemadj);
            adj plusadjR(&vertices[productPlus-1],nullptr, 0);
            reactionVec[i]->adjs.push_back(plusadjR);
        }
        for (size_t i(pro_pos); i < reactionVec.size(); i++)
        {
            adj chemadj(reactionVec[i],nullptr, 0);
            reactionVec[i]->adjs.push_back(chemadj);
            adj plusadjP(&vertices[productPlus], reactionVec[i], 0);
            reactionVec[i]->adjs.push_back(plusadjP);
        }
        
        
        
        
    }
    
}
void graph::build_graph(const string & fileD)
{
    string word;
    ifstream input_stream (fileD);
    if (!input_stream)
        cout << "Could not open the file!" << endl;
    
    else
        while (getline(input_stream, word))
            addRXN(word);
    
    input_stream.close();
    return;
    
}

