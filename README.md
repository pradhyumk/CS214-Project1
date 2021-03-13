# CS214 - Project 1: Word Wrap
Instructor: David Menendez

Assignment Partners:  
  * Pradhyum Krishnan (pk555)  
  * Gaurav Patel (gvp23)  

Objective: Write a reformating program which limits the number of characters to the specified width.

Due date: 12 March 2021  

## Strategy
First, we started by identifying all possible test cases that the program would encounter; this included, but not limited to, a combination of characters, tabs, abrupt paragraph breaks, etc. Based on the test cases, we constructed an algorithm which traverses through the entire string of characters. If the traversal encountered any combination of whitespace or characters, the program would decide whether to include it or not depending on the combination of upcoming whitespaces and/or words. For word wrapping directories, the program initially verifies the user input for a valid path. If so, it changes directory and uses the algorithm's method in a loop to reformat valid files in the folder. Formatted files in this scenario begin with the prefix "wrap.".

### The program was tested with following scenarios:
- Width 0/1
- Large passages (from Jeckyll & Hyde)
- Words that fit the width exactly
- Extra lines before the first word and after the last word
- Extra '\n' after a paragraph break ("\n\n\n\n\n")
- Varying sequences such as "\n\t\t\n", "\n   \n", "    \n   \t\t   \nword \t\t\n\t    \t\nword\n\n\n\n", etc
- Using files that begin with "." and "wrap." inside directories 
- Moving test case files to other directories