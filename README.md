# Implementing_NFA

This project is a C++ implementation of a Non-Deterministic Finite Automata that receives input file(s) containing NFA of the form (a,b) where a and b are tuples.

An NFA is a finite state machine in which the automaton can advance to numerous possible following states from each state and a given input symbol. The main function forms the driver program to test these functions and acceptance states.

The program in the essence provides accepted inputs, states (start state, end state), the working algorithm for the provided automata and the inputs and follows the following road-map:

1. Read file
2. Separate the values into different vectors (and a map or similar for the nfa)
3. Ask user for inputs if file does not have any
4. Run NFA
5. Print if input is valid

The Pseudocode, Data Structures used and Test cases can be found below: 

[NFA_Pseudocode_TestCases.pdf](https://github.com/Abiraam/Implementing_NFA/files/8796421/NFA_Pseudocode_TestCases.pdf)
