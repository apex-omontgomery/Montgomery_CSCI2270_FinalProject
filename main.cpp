//
//  main.cpp
//  ReactionGenerator
//
//  Created by Saman Moein and William Montgomery on 4/21/15.
//
//  Copyright (c) 2015 Saman Moein. All rights reserved.

#include <iostream>

#include <vector>
#include <sstream>
#include <fstream>
#include <stdlib.h>




void build_rxns(const char *);


#include <iostream>
#include "Graph.h"

graph RG;//RG for Reaction Graph
int main(int argc, const char * argv[])
{

    //RG.build_graph(argv[1]);
    bool repeat(1);
    while(repeat)
    {
        cout << "Enter the name of the chemicals you wish to use:" << endl;
        vector<ver *> reactors;



        //handle the input and put each name in a block in a vector: reactors;




        cout << "Enter the name of the products you wish to obtain:" << endl;
        vector<ver *> products;






        //handle the input and put each name in a block in a vector: products;





        vector<vector<adj*>> * Paths = RG.reactionGenerator(reactors, products);
        if( Paths != nullptr )
            RG.print_paths(*Paths);
        else
            cout << "There is not a set of reactions to obtain desired chemicals."<<endl;
        cout << "Do you want to try one more time? 0 for no, 1 for yes: ";
        cin >> repeat;
    }


}

void build_rxns(const char * filename){

string word;
    ifstream input_stream (filename);
    if (!input_stream){
        cout << "Could not open the file!" << endl;
	}
	else{
		while (getline(input_stream, word)){
            RG.addRXN(word);
        }

	}
	input_stream.close();
    return;
}






