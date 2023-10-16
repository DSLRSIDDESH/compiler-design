Given a CFG as an input file in which each line is having exactly one production rule and epsilon as #,  (NT-Capital letters, T-small letters).
Also given an input string containing set of terminals (T-small letters).

Write a c program for the LL(1) parsing.

Following things are expected from the program.
1. Constructing LL(1) parsing table based on the already computed first sets of non-terminals.
2. Identifying whether the given CFG is LL(1) or not.
3. If yes, then implement the LL(1) parsing by making use of stack and LL(1) parsing table.
4. If the given CFG is not LL(1), then print it without parsing.
5. If the given CFG is LL(1), then program should give the output as whether the given string is accepted or not.