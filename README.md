# huffman-tree
This program is an assignment for my university cs course: data structure and algorithm.  
The target is to build a huffman code encoder.  
  
Here are the main steps constructing the program:  
1.read the input file.  
2.construct the frequency table.  
3.build the huffman tree according to the frequency table.  
4.encode each letter in the tree and store the code in a table.  
5.output the huffman code.  
6.output the encrypted input from step1.  

Some rules in constructing the program:  
1.I used the letter with the smallest ASCII code as the representative of a subtree.  
2.In case of ambiguity, I selected the two trees with the smallest representatives and when combining the two trees, the one with the smaller representative was on the left.  

