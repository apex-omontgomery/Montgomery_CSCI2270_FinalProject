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

int graph::plusCounter(1);//The plusCounter is used to name our plus signs. the int will be changed to string so that each plus sign has a number a name.
/* As mentioned in the header file, all the functions from reactionGenerator to make_default do the algorithm to find pathways.
 reactionGenerator, gets reactors and products from the user input and find a pathway from each reactor to each product, using the function reactionFinder.*/
vector<vector<adj*>> * graph::reactionGenerator (vector<ver *> & reactors, vector<ver *> & products )
{
    
    vector<vector<adj*>> * paths = new vector<vector<adj*>>;
    vector<adj*> path;
    for( ver * reactor : reactors )
    {
        for( ver * product : products )
        {
            reactionFinder(reactor, product, *paths,path,nullptr,path);
            path.clear();
            make_firstState(reactors);
            if(!paths->empty())
                refine(paths->back());
        }
        
    }
    
    
    return paths;
    
    
}

/*reactionFinder gets one reactor and one product from the input list of chemicals by user. 
It is a recursive depth-first based search. It also gets a vector of paths and a path. Whenever it finds a path, it will push_back it to the paths vector.
 The two other varibales it gets are formerStep and divergedFrom. formerStep, as the name implies is the former vertex. 
 divergedFrom is a bit more complicated. It is the pathway from which we are diverging, This will be explained in more detail.
 The Algorithm: we start from a reactor, we through all its adjacents. There are two kinds of adjacents: some are in the dif_side others are in the same side of the reaction.
 But before explaining the how the algorithm treats these two cases let us explain something about the structure:
 Because chemicals can be used more than once, being visited for chemicals (vertices) are meaningless here. 
 However, edges representing pathways should have a boolean for them being visited; We don’t want to do the same reaction twice.
 
 Now back to the algorithm,
 1. If the adjacent is in the same side of the reaction( the dif_side boolean is false ) : 
 block the edge to the former step (if there is one, meaning that it’s not the first step ) and if it’s not visited do either of these:
 -if A, the adjacent is a plus sign and if all the reactors for that plus sign are available, call the reactionFinder function with A as a vertex (A->toself, toself points to the vertex A).
 -else if A is not a plus sign call the reactionFinder with A->toself as the reactor.
 (Note that the first case also needs A to have all available(with the function all_av) reactors. 
 Therefore sometimes neither of these conditions are fulfilled. (when A is a plus sign however the group it is representing is not available therefore it cannot be acceptable as a valid reactor.)
 
 2. If A is in the different side: meaning that A is a product or a plus sign representing a group of products, set the path to the ‘divergedFrom’ path, push_back A as a new step in the path, then set divergedFrom to path. 
 (note that divergedFrom is not a single object, there are many divergeFrom objects since the function makes a copy of it.) Make the whole product group available (with function make_av) and then call reactionFinder with A->toself.
 
 Note : Since it would be meaningless to use the final product as a reactor in a new reaction, we also check that the reactor that has the A adjacent, is not the product itself. This is done by the function isReactor.)
    */
void graph::reactionFinder( ver * reactor, ver * product,vector<vector<adj*>> & paths, vector<adj*> & path, ver * formerStep,vector<adj*> divergedFrom)
{
    
    if(product->name == reactor->name && !path.empty() )
    {
        paths.push_back(path);
        path.clear();
    }
    else
        for( adj *& A : reactor->adjs )
        {
            if( !A->dif_side && formerStep != nullptr  )
                block_formerStep(reactor,formerStep);
            if (!A->visited && !inPath(path, A))
            {
                if( !A->dif_side )
                    A->visited = 1;
                
                if (A->dif_side && !isReactor(reactor, product))
                {
                    path = divergedFrom;
                    path.push_back(A);
                    divergedFrom = path;
                    make_adjs_av(A->toself);
                    reactionFinder(A->toself, product, paths,path,reactor,divergedFrom);
                }
                else
                {
                    if (A->toself->plussign && all_av(A->toself))
                        reactionFinder(A->toself, product, paths,path,reactor,divergedFrom);
                    if(!A->toself->plussign)
                        reactionFinder(A->toself, product, paths,path,reactor,divergedFrom);
                }
            }
        }
    
}

void graph::make_adjs_av( ver * plus )//This makes all the same side adjacents available.
{
    plus->availabe = true;
    for( size_t i(0); i < plus->adjs.size(); i++)
        plus->adjs[i]->toself->availabe = true;
}

bool graph::all_av(ver * plussign)//This checks if all the same side adjacents are avaible
{
    for( size_t i(0); i < plussign->adjs.size(); i++)
        if(!plussign->adjs[i]->toself->availabe && !plussign->adjs[i]->dif_side )
            return false;
    return true;
}

bool graph::inPath( vector<adj*> & path, adj * A)//This is used for debugging purposes. It checks if A reaction is in the path.
{
    for( adj * step : path )
        if(step->parent == A->parent && step->toself == A->toself)
            return true;
    return false;
}

bool graph::isReactor( ver * reactor, ver * product)//This makes sure that we are not using the final product as a reactor, cause that would be silly.
{
    if(reactor == product)
        return true;
    for( adj * neighbour : reactor->adjs )
    {
        if( !neighbour->dif_side && neighbour->toself == product )
            return true;
    }
    return false;
}

void graph::block_formerStep(ver * chem,ver * formerStep)//This blocks the edge to the former vertex.
{
    for( adj * A : chem->adjs )
        if( formerStep == A->toself )
            A->visited = 1;
}

void graph::makeReactors_av(vector<ver *>& reactors)//This makes all the reactors user has chosen, available.
{
    for( ver * chem : reactors )
        chem->availabe = 1;
}

//After each loop in reactionGenetor we want to go back to the first state: All edges not visited, only vertices that are reactors be available.
void graph::make_firstState(vector<ver *> & reactors){
    bool is_reactor(0);
    for( ver * chem : vertices )
    {
        for( ver * reactor : reactors )
            if( reactor->name == chem->name )
            {
                is_reactor = 1;
                break;
            }
            else
                chem->availabe = 0;
        for( adj * adjchem : chem->adjs )
            adjchem->visited = 0;
    }
}


/* The task of refine function (which uses isUsed function ) is to refine(!) the pathways and get rid of some useless reactions. 
The reactions found in our reactionFinder are logical, however some of them are unnecessary. The most important case is when there are 'deadends' in our reactions:
 consider this set of reactions:
 Reactor -> Junk
 Reactor -> Product
 Our graph is sorted alphabetically. Since Junk starts with J and Product starts with P, the algorithm, starting from Reactor’s adjacents,  will first go to J puts it into path and then goes to ‘Product’. 
 and then we will have Reaction -> Junk, Reaction -> Product as a pathway. It is technically a pathway but it’s not smart.
 Refine function omits all these junk reactions. We know that the last reaction in a pathway is going to have the desired product in its products. 
 So we start from the reaction before the last one. We look if any of the products of the reaction is used later in another reaction (this is done by the function isUsed) if it is not used we will delete the reaction.
 Note: The reason for starting from the end of the pathway is that by deleting the very last useless steps, the steps before them will show if they are actually only being only used for a later useless step or not.*/
void graph::refine(vector<adj *> & path )
{
    for( size_t p(path.size()-2); p != -1; p--)
    {
        bool used(0);
        vector<adj*>::iterator pos(path.begin()+p);
        for( size_t r(p+1); r < path.size(); r++)
        {
            used = isUsed(path[r]->parent,path[p]->toself);
            if(used)
                break;
        }
        if(!used)
            path.erase(pos);
    }
    
}

bool graph::isUsed( ver * side1, ver * side2 )
{
    if( side1->plussign && side2->plussign )
    {
        if(side2 == side1)
            return true;
        else
            for( adj * neighbour1 : side1->adjs )
                for( adj * neighbour2 : side2->adjs )
                {
                    if( !neighbour1->dif_side && !neighbour2->dif_side && neighbour1->toself == neighbour2->toself )
                        return true;
                }
        
        
    }
    
    else if(!side1->plussign && !side2->plussign)
    {
        if(side1 == side2)
            return true;
    }
    else if(!side2->plussign)
        for( adj * neighbour : side2->adjs )
        {
            if( !neighbour->dif_side && neighbour->toself == side1 )
                return true;
        }
    else if (!side1->plussign)
        for( adj * neighbour : side1->adjs )
        {
            if( !neighbour->dif_side && neighbour->toself == side2 )
                return true;
        }
    return false;
}

/* giveSuggestions will be called if no valid pathway is found. It will still look for the products the user wants but uses the whole list chemicals instead of ones the user had said he wanted to use.
 After suggesting a pathway it will tell the user what reactors he needs, in addition to what he already has. The alreadyThere function checks what reactors from the needed reactor list were the first input of the user.
 */
void graph::giveSuggestions(vector <ver *> & products,vector <ver *> & reactors)
{
    int i(1);
    vector<vector<adj *>> * paths = reactionGenerator(vertices, products);
    
    cout << endl;
    cout << "Suggested Paths: " << endl;
    for( vector<adj*> & path : *paths )
    {
        
        cout << i << " :" << endl;//These can be sorted by total cost,number of reactions, change in H,S,G etc. USING A TREE.
        int j(1);
        vector<ver *> youneed;
        for(adj * step : path )
        {
            cout << "   "<< j << ". ";
            if( step->parent->plussign )
            {
                vector<ver*> group;
                vector<adj*>::iterator rct(step->parent->adjs.begin());
                for(; rct != step->parent->adjs.end(); rct++ )
                    if( !(*rct)->dif_side )
                        group.push_back((*rct)->toself);
            
                vector<ver*>::iterator member(group.begin());
                for(; member != group.end()-1; member++ )
                    cout << (*member)->name << " + ";
                cout << (*member)->name;
                size_t c(0);
                for(; c < group.size(); c++ )
                {
                    bool found(0);
                    size_t r(0);
                    for( ver * reactor : reactors){
                        if(group[c] == reactor)
                        {
                            found = 1;
                            break;
                        }
                        else
                            r++;
                    }
                    if(!found && !alreadythere(group[c], youneed))
                        youneed.push_back(group[c]);
                }
                
            }
            else
            {
                cout << step->parent->name;
                bool found(0);
                for( ver * reactor : reactors)
                    if(step->parent == reactor)
                        found = 1;
                if(!found && !alreadythere(step->parent, youneed))
                    youneed.push_back(step->parent);
            }
            
            
            cout << " --> ";//Used catalyst can be reported here: a good way to do that : A + B --Cat--> C + D
            
            if( step->toself->plussign )
            {
                vector<ver*> group;
                vector<adj*>::iterator rct(step->toself->adjs.begin());
                for(; rct != step->toself->adjs.end(); rct++ )
                    if( !(*rct)->dif_side )
                        group.push_back((*rct)->toself);
                
                vector<ver*>::iterator member(group.begin());
                for(; member != group.end()-1; member++ )
                    cout << (*member)->name << " + ";
                cout << (*member)->name;
                
            }
            else
                cout << step->toself->name;
            
            cout << endl;
            j++;
            
        }
        i++;
        cout << endl;
        cout << "For this path you need these chemicals besides what you already have:"<<endl;
        for(ver * chem : youneed)
            cout<< chem->name <<endl;
        cout << endl;
    }
    
    
}

bool graph::alreadythere(ver * chem, vector<ver *> & youneed)
{
    for( ver * inchem : youneed )
        if(chem == inchem)
            return true;
    return false;
}
/* This function simply prints the pathways. if there is no pathway it will call giveSuggestion function */
void graph::print_paths(vector<vector<adj*>> & paths, vector<ver *> & products, vector <ver *> & reactors)
{
    int i(1);
    cout << endl;
    if( paths.empty() )
    {
        cout<<"You can't have the products you want from the reactors you provided."<<endl;
        giveSuggestions(products,reactors);
    }
    else
        cout << "Paths: " << endl;
    for( vector<adj*> & path : paths )
    {
        
        cout << i << " :" << endl;//These can be sorted by total cost,number of reactions, change in H,S,G etc. USING A TREE.
        int j(1);
        for(adj * step : path )
        {
            cout << "   "<< j << ". ";
            if( step->parent->plussign )
            {
                vector<ver*> group;
                vector<adj*>::iterator rct(step->parent->adjs.begin());
                for(; rct != step->parent->adjs.end(); rct++ )
                    if( !(*rct)->dif_side )
                        group.push_back((*rct)->toself);
                
                vector<ver*>::iterator member(group.begin());
                for(; member != group.end()-1; member++ )
                    cout << (*member)->name << " + ";
                cout << (*member)->name;
            }
            else
                cout << step->parent->name;
            
            cout << " --> ";//Used catalyst can be reported here: a good way to do that : A + B --Cat--> C + D
            
            if( step->toself->plussign )
            {
                vector<ver*> group;
                vector<adj*>::iterator rct(step->toself->adjs.begin());
                for(; rct != step->toself->adjs.end(); rct++ )
                    if( !(*rct)->dif_side )
                        group.push_back((*rct)->toself);
                
                vector<ver*>::iterator member(group.begin());
                for(; member != group.end()-1; member++ )
                    cout << (*member)->name << " + ";
                cout << (*member)->name;
                
            }
            else
                cout << step->toself->name;
            
            //cout << " Change in Enthalpy: " << step->H;  //many other things such as change in S,G, cost etc. can be reported here.
            cout << endl;
            j++;
            
        }
        i++;
        cout << endl; // The total change in H,S,G total cost etc. can be reported here.
    }
}


void graph::make_default()//Set everything to default. No Chemical available, nothing visited.
{
    for( ver * chem : vertices )
    {
        chem->availabe = 0;
        for( adj * adjchem : chem->adjs )
            adjchem->visited = 0;
    }
}



void graph::namesList(vector<ver *> & list)//The list of names of the chamicals that will be printed in the main manu.
{
    for( size_t i(0); i < vertices.size(); i++ )
    {
        if( !vertices[i]->plussign )
            list.push_back(vertices[i]);
    }
}

void graph::rearrange()//Sort the graph in alphabetical order.
{
    vector<ver *> modified;
    for (unsigned int i =0; i < vertices.size(); i++){
        if (!vertices[i]->plussign){
            modified.push_back(vertices[i]);
            vertices.erase(vertices.begin() +i);
            i--;
        }
    }
    bool swapped= true;
    ver * temp;
    while (swapped){
        swapped = false;
        for (unsigned int i = 0; i < modified.size()-1; i++){
            if (strcmp(modified[i]->name.c_str(), modified[i+1]->name.c_str() ) > 0){
                temp = modified[i];
                modified[i]= modified[i+1];
                modified[i+1]=  temp;
                swapped = true;
            }
        }
    }
    for( unsigned int i = 0; i < vertices.size(); i++){
        modified.push_back(vertices[i]);
    }
    vertices = modified;
    
}


/*These last functions are used for building the graph. This is not a simple graph. There are two kinds of edges ( dif_side or not ) two kinds of vertices (plus sign or not) and reading the file is not easy either.
 The build_graph function reads each line from the file, and calls addRXN with it.
 addRXN creates a vector of chemicals using the line it receives. it also determines what chemicals are the products and what are the reactors.
 addRXN, also uses the function sameGroup two times: one for the group of reactions and one for the group of products in the vector it has made. 
 sameGroup function -simply put- determines if a group chemicals are already being used in the graph, For example:
 In : (A + B + C - > D and E + F -> A + B + C) we have three groups: (A,B,C), (D), (E,F) and the first one is used two times, and we don’t want to make new vertices for it. 
 and also we want (E,F) to point to the same plus sign in A + B + C -> D. Also Note that in: A + B > G, (A,B) is a new group with a new plus sign vertex connecting them together.
 addRXN calls addVertice and addAdj. addVertice is not complicated, addAdj is: using the information from what we got calling sameGroup once for reactors and once for products it connects the vertices together.
 In addAdj we use pro_pos to determine what kind of reaction we are dealing with, pro_pos is where products start in the reactionVec; the vector of chemicals in a given reaction.
 if reactionVec.size is 2 it means the reaction is something like this : A → B
 else if pro_pos is 1: A → B + C + ...
 else if pro_pos is at the end of the vector( being reactionVec.size() -1 ) : A + B + … → Z
 else ( reactionVec.size is bigger than one, pro_pos is in the middle) : A + B + … → X + Y + …
 */
void graph::build_graph(const string & fileD)
{
    
    int i;
    string word;
    ifstream input_stream (fileD);
    if (!input_stream)
        cout << "Could not open the file!" << endl;
    
    else
    {
        cout << "Reactions :"<<endl;
        cout << endl;
        while (getline(input_stream, word))
        {
            cout << word << endl ;
            addRXN(word);
        }
        cout << endl;
    }
    rearrange();
    input_stream.close();
    return;
}

void graph::addRXN(const string & reactionLine)
{
    bool found(0);
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
    pos = 0;
    vector<ver *> reactionVec;
    ver * sameGroupR;
    ver * sameGroupP;
    for( string splitted2 : splittedVec )
    {
        found = 0;
        for( size_t i(0); i<vertices.size(); i++ )
        {
            ver & chem(*vertices[i]);
            if ( chem.name == splitted2 )
            {
                reactionVec.push_back(&chem);
                found = 1;
                break;
            }
            
        }
        if( !found )
            addVertice(splitted2,reactionVec);
        pos++;
    }
    
    
    sameGroupR = sameGroup(reactionVec, pro_pos,0);
    sameGroupP = sameGroup(reactionVec, pro_pos,1);
    
    if( pro_pos > 1 && !sameGroupR)
    {
        addVertice("+",reactionVec);
        sameGroupR = vertices.back();
    }
    if( pro_pos < reactionVec.size() - 1 && !sameGroupP )
    {
        addVertice("+",reactionVec);
        sameGroupP = vertices.back();
    }
    addAdj(reactionVec, pro_pos,sameGroupR,sameGroupP);
}

void graph::addVertice(const string & name,vector<ver *> & reactionVec )
{
    ver * temp = new ver;
    if (name == "+")
    {
        temp->name = to_string(plusCounter);
        temp->plussign = true;
        plusCounter++;
    }
    else
        temp->name = name;
    
    vertices.push_back(temp);
    if (name != "+")
        reactionVec.push_back(temp);
}

ver * graph::sameGroup(vector<ver *> & reactionVec, const int & pro_pos, bool P)
{
    bool some_share(0);
    
    size_t myend;
    size_t mybeginning;
    if(P)//product
    {
        mybeginning = pro_pos;
        myend = reactionVec.size();
    }
    else//reactor
    {
        mybeginning = 0;
        myend = pro_pos;
    }
    size_t J(0);
    size_t K(0);
    ver * sharedPlus(nullptr);
    if( pro_pos == 1 && !P )
        return reactionVec[0];
    else if( pro_pos == reactionVec.size()-1 && P )
        return reactionVec.back();
    else
    {
        for( adj * maybeUnique : reactionVec[mybeginning]->adjs )
        {
            for( size_t j(mybeginning+1); j < myend; j++ )
            {
                some_share = 0;
                for( size_t k(0); k < reactionVec[j]->adjs.size(); k++ )
                    if( maybeUnique == reactionVec[j]->adjs[k] )
                    {
                        J = j;
                        K = k;
                        sharedPlus = reactionVec[j]->adjs[k]->toself;
                        some_share = 1;
                        break;
                    }
                if( some_share == 0 )
                    break;
            }
        }
        
        int counter(0);
        
        if( sharedPlus != nullptr )
            for( adj * neighbour : sharedPlus->adjs )
                if(!neighbour->dif_side)
                    for( adj * neighbour2 : neighbour->toself->adjs )
                        if( neighbour2->toself == sharedPlus )
                            counter++;
        if (counter > pro_pos)
            sharedPlus = nullptr;
        
        if( sharedPlus != nullptr )
            return  reactionVec[J]->adjs[K]->toself;
        else
            return nullptr;
    }
    
}

void graph::addAdj(vector<ver *> & reactionVec, const int & pro_pos , ver * sameGroupR, ver * sameGroupP)
{
    size_t productPlus(vertices.size()-1);
    
    if( reactionVec.size() == 2 )
    {
        adj * chemadj = new adj(reactionVec[1], reactionVec[0], 1);
        reactionVec[0]->adjs.push_back(chemadj);
    }
    else if( pro_pos == 1 )
    {
        
        adj * difSide_plusAdj = new adj(sameGroupP, reactionVec[0], 1);
        
        reactionVec[0]->adjs.push_back(difSide_plusAdj);
        
        if(sameGroupP == vertices[productPlus])
            for (size_t i(1); i < reactionVec.size(); i++)
            {
                adj * sameSide_plusAdj = new adj(sameGroupP, reactionVec[0], 0);
                adj * chemadj = new adj(reactionVec[i],nullptr, 0);
                sameGroupP->adjs.push_back(chemadj);
                reactionVec[i]->adjs.push_back(sameSide_plusAdj);
            }
        
    }
    else if( pro_pos == reactionVec.size()-1 )
    {
        
        adj * chemadj = new adj(reactionVec[pro_pos], sameGroupR, 1);
        sameGroupR->adjs.push_back(chemadj);
        
        if(sameGroupR == vertices[productPlus])
            for (size_t i(0); i < pro_pos; i++)
            {
                adj * sameSide_plusAdj = new adj(sameGroupR, nullptr, 0);
                adj * chemadj = new adj(reactionVec[i],nullptr, 0);
                sameGroupR->adjs.push_back(chemadj);
                reactionVec[i]->adjs.push_back(sameSide_plusAdj);
            }
        
    }
    else
    {
        adj * difSide_plusAdjP = new adj( sameGroupP, sameGroupR, 1);
        sameGroupR->adjs.push_back(difSide_plusAdjP);
        
        
        if(sameGroupR == vertices[productPlus-1])
            for (size_t i(0); i < pro_pos; i++)
            {
                adj * sameSide_plusAdjR = new adj(sameGroupR,nullptr, 0);
                adj * chemadj=  new adj(reactionVec[i],nullptr, 0);
                sameGroupR->adjs.push_back(chemadj);
                reactionVec[i]->adjs.push_back(sameSide_plusAdjR);
            }
        if(sameGroupP == vertices[productPlus])
            for (size_t i(pro_pos); i < reactionVec.size(); i++)
            {
                adj * sameSide_plusAdjP = new adj( sameGroupP, sameGroupR, 0);
                adj * chemadj = new adj(reactionVec[i],nullptr, 0);
                sameGroupP->adjs.push_back(chemadj);
                reactionVec[i]->adjs.push_back(sameSide_plusAdjP);
            }
    }
    
    
}







