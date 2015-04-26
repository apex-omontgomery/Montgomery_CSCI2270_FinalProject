Project Summary:
The intent of this project is to create a simple pathway for chemical reactions. This will be done utilizing mockup reactions that have been simplified to most basic design. This can be done by calling a chemical such as "t-butyl Alcohol" the name "A." 

The basic foundation is a Edge on a graph for each chemical. Each chemical has a name, vector of adjacencies, a boolean for availability, and a boolean for the plus-sign in the reaction. The adjacency structure has a boolean for visited, boolean to denote if the parent of the vertice is on the opposite side of reaction, and two pointers parent points to the chemical, null or the plus sign of the reaction. Self give the pointer of the adjacency chemical that is found in the graph edge. 

This program will allow a text file of chemical reactions in this format "C1 + C2 > C3 + C4" with n chemicals on each side and n = positive integer > 0. The code requires plus sign between chemicals on each side and arrow of this form ">" . When the chemicals are added to the graph you can select desired product and desired input. This will give one or many paths. 

How to Run:


Dependencies:

System Requirements:

Group Members:

Contributors:

Open Issues and Bugs: