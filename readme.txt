Project Summary:
The intent of this project is to simulate the process of finding reaction pathways from known reactions. The foundation will be a set of chemical reactions that while are actual reactions are muchly simplified. Since we are looking for pathways and not the actual stoichiometry the reactions do need to be balanced. Since the input file will be a text document sub and superscript are not possible so the algorithm should detect differences based on charge symbols (+/-) parenthesis for chemicals and since numbers never denote elements each chemical will be easily differentiated and divided into chemical. 

Once the chemicals are input a graph will be created that has an adjacency matrix for each chemical. In the adjacency matrix each chemical will have a number associated with it which is the associated reaction to get to that chemical in the adjacency matrix. Additionally the reactions will have to be stored in some method because just because you have one chemical in the reaction doesn't mean you have both. So this logic should be checked so invalid reactions are not added to the pathway. 

Additionally the assumption that reactions proceed to completion is important and as thus a vector of current chemicals needs to implemented and when a reaction is complete that chemical is removed. In the case of reversable reactions the reversable reaction doesn't proceed to completion until most of the products of that reaction are done. 

Finally I have to figure a way that will output multiple pathways since the first found might not the be best pathway.   



How to Run:


Dependencies:

System Requirements:

Group Members:

Contributors:

Open Issues and Bugs: