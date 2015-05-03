//
//  main.cpp
//  ReactionGenerator
//
//  Created by Saman Moein on 4/21/15.
//  Copyright (c) 2015 Saman Moein. All rights reserved.
//

#include <iostream>
#include "Graph.h"
int main(int argc, const char * argv[])
{
    bool validInput = true;
    graph RG;//RG for Reaction Graph
    RG.build_graph(argv[1]);
    bool repeat(1);
    vector<ver *> names;
    RG.namesList(names);
    cout << "Chemicals :"<<endl;
    cout<< endl;
    for( size_t i(0); i < names.size(); i++ )
    {
        cout << i+1 << ". ";
        cout << names[i]->name << endl;
    }
    cout << endl;
    while(validInput == true)
    {
        while(repeat)
        {
            cout << "From the printed list, Enter the chemicals' codes you wish to use on each line. Enter 0 when you are finished." << endl;
            vector<ver *> reactors;
            size_t i;
            cin >> i;
            while(i == 0)
            {
                cout << "You should choose at least one chemical as reactors. Enter the chemicals' code you wish to use."<< endl;
                cin >> i;
            }

            while( i != 0)
            {
                if(i > 0 && i<=20)
                {
                    reactors.push_back(names[i-1]);
                    cin >> i;
                }
                else
                {
                    cout << "Not a valid input, please try again." << endl;
                    validInput = false;
                    break;

                }
            }
            if(validInput == false)
            {
                cout << endl;
                break;
            }
            RG.makeReactors_av(reactors);

            cout << "Now enter the products' codes you wish to obtain on each line:" << endl;
            vector<ver *> products;
            size_t j;
            cin >> j;
            while(j == 0)
            {
                cout << "You should choose at least one chemical as products. Enter the chemicals' code you wish to obtain."<< endl;
                cin >> j;
            }

            while( j != 0 )
            {
                if(j > 0 && j <= 20)
                {
                    products.push_back(names[j-1]);
                    cin >> j;
                }
                else
                {
                    cout << "Not a valid input, please start over." << endl;
                    validInput = false;
                    break;
                }
            }
            if(validInput == false)
            {
                cout << endl;
                break;
            }

            vector<vector<adj*>> * Paths = RG.reactionGenerator(reactors, products);
            if( Paths != nullptr )
                RG.print_paths(*Paths,products,reactors);
            else
                cout << "There is not a set of reactions to obtain desired chemicals."<<endl;
            cout << "Do you want to try one more time? 0 for no, 1 for yes: ";
            cin >> repeat;
            if( repeat )
                RG.make_default();
        }
        if(repeat == 0)
        {
            validInput = false;
        }
        else
        {
            validInput = true;
        }
    }
    return 0;
}
