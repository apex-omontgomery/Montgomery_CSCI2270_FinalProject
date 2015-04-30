Project Summary:
The intent of this project is to create a simple pathway or pathways for chemical reactions:
If, for instance, we have this sample set of reactions:

Reactor > B + C + D

B + D > E

E + Reactor > F

F > G

G + E > Product

B + R > F

F + G > I + Product

and the user tells the program that he has 'Reactor' and wants to get 'Product' the output of the program will be:

Paths: 
1 :
   1. Reactor --> B + C + D
   2. B + D --> E
   3. E + Reactor --> F
   4. F --> G
   5. G + E --> Product

2 :
   1. Reactor --> B + C + D
   2. B + D --> E
   3. E + Reactor --> F
   4. F --> G
   5. F + G --> I + Product

The basic foundation is a vertex on a graph for each chemical and each plus sign. Each vertex has a name, (which for plus signs will be a number) vector of adjacencies, a boolean for availability, and a boolean if it is a plus-sign. The adjacency structure has a boolean for visited, boolean to denote if the adjecent is on the opposite side of reaction, and two pointers: parent and toself. The funcionality of these objects will be explained below. 
If in a side of a reaction the is more than one chemical (like A + B >...), the chemicals will be connected with a not directed edge to the plus sign and then a directed edge will be pointing to the other side of the reaction from that plus sign.

*In  A + B > C we have A - + - C (the dashes indicate edges) and the plus sign is then connected to C with a onr way edge:

A

)

(+) -> C

)

B

*In C > D, we have no plus signs:

A

)

(+) -> C -> D

)

B

You can find a better illustration in : 
     

This program will allow a text file of chemical reactions in this format "C1 + C2 > C3 + C4" with n chemicals on each side and n = positive integer > 0. The code requires plus sign between chemicals on each side and arrow of this form ">" . When the chemicals are added to the graph you can select desired product and desired input. This will give one path.

How to Run:

1. Download Graph.cpp, Graph.h, ProjectDriver.cpp, reactionlist.txt and move to desired folder.
2. Open compiler of choice (geany, code blocks, etc).
3. Locate folder with project files and load ProjectDriver.cpp in compiler.
4. Compile and run ProjectDriver.cpp
5. Program will load, and create graph with no user input. 
6. Follow directions in user prompt. 



Dependencies:

There are no dependencies other than the required programs and files in the how to run section.


System Requirements:

This program has only been tested on OSX 10 and Windows 8.1 and Windows 7.


Group Members:

William Montgomery
Saman Moein

Contributors:

NA

Open Issues and Bugs:

-- If the user enters multiple reactors and products there maybe some pathways repeated in the output.
-- sort chemical output by name alphabetically for readability
