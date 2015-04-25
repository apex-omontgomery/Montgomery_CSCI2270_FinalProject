//
//  Vertex.cpp
//  ReactionGenerator
//
//  Created by Saman Moein on 4/21/15.
//  Copyright (c) 2015 Saman Moein. All rights reserved.
//

#include "Graph.h"
#include <sstream>

void graph::make_adjs_av( ver * plussign )
{
    plussign->availabe = true;
    for( size_t i(0); i < plussign->adjs.size(); i++)
        plussign->adjs[i].toself->availabe = true;
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

// this function inputs a const string reference and breaks the reaction text line into a vector of strings. Any + or arrow
// signs are not added to the vector of strings and instead a simple indice of the first product is saved.
// For our logic any reaction has at least one plus sign (we set this for products) and if here are more than one reactant
// we add another plus sign. so if the position of first product is greater than 1 (starting from 0). Then we know we have multiple
// plus signs for the reaction logic.


void graph::addRXN(const string & reactionLine){
    istringstream splitted(reactionLine);
    vector <string> splittedVec;
    vector <ver*> reactionVec;


    int pro_pos(0);
    int pos(0);
    do {
        string current;
        splitted >> current;
        if (current == ">"){
            pro_pos = pos;
        }

        else if (current != "+"){
            splittedVec.push_back(current);
            pos++;
        }
    } while(splitted);

    // now we have a vector <r1, r2,... rn, p1, p2,... pn>
    // and position of p1 (first product)

    // we now need to create a new vector of pointers to the struct ver so we can manipulate the graph.
    // we find if the new reaction has

    for (string splitted2 : splittedVec){ // creates list of strings from new reaction
        bool found(0); // initialize
        for (ver & chem: vertices){
            if (chem.name == splitted2){
                found = 1;
                reactionVec.push_back(&chem);
                break;
            }
        }
        if (!found){
                addVertice(splitted2, reactionVec);
        }
    }

    if (pro_pos > 1){
        addVertice("+", reactionVec);
    }
    addVertice("+",reactionVec); // always add plus sign for products
    addAdj(reactionVec, pro_pos);
}

void graph::addVertice(const string & name, vector <ver*> &reactionVec){
    ver temp;
    temp.name = name;
    if (name == "+"){
        temp.plussign = true;
    }
    vertices.push_back(temp);


    if (name != "+"){
        reactionVec.push_back(&vertices[vertices.size()-1]);
    }
}

void graph::addAdj( vector < ver*> reactionVec, int pro_pos){
    if (pro_pos == 0){
        cout << "ooops no reaction" << endl;
    }
    else if (pro_pos==1){

        size_t productPlus = vertices.size()-1;
        adj newadj (&vertices[productPlus], reactionVec[0], 1);
        reactionVec[0]->adjs.push_back(newadj);
        for (unsigned int i = 1; i < reactionVec.size(); i++){
            // point plus sign to products and products to plus sign
            adj newadj (reactionVec[i], &vertices[productPlus], 0);
            reactionVec[i]->adjs.push_back(newadj);
            adj newadj2 (&vertices[productPlus],reactionVec[i], 0);
            vertices[productPlus].adjs.push_back(newadj);
        }
    }
    else{
        size_t productPlus = vertices.size()-1; // the other plus sign is productPlus -1.
       // adj plusadjP( vertices[productPlus-1], nullptr,1);





    }

}


//void build_graph(const string & fileD)
//{
    //build graph using the file directory in the command line
//}

