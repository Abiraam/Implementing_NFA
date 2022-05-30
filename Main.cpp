

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include <stdio.h>

bool isAccepted;
void parse_machine(std::vector<char> machine, std::vector <std::vector<std::string> >& NFA);
void print_nfa(std::vector <std::vector<std::string> > NFA);
void fix_delta(std::vector <std::vector<std::string> > NFA, std::vector<std::vector<std::string> >& delta);
bool is_ignored_char(char c);
bool is_more_than_parens_and_commas(std::string str);
void transversal(std::vector<std::vector<std::string>> NFA, std::vector<std::vector<std::string>> delta, std::string state, std::string input);
bool validInput(const std::string& str, std::vector<std::string> lan);

int main()
{
	// String to hold file name & ifstream to actually read text file.
	std::string file_name;
	std::ifstream input_stream;

	// Create a vector to store characters from input text file.
	std::vector<char> machine;
	char character = 0;

	// Ask user for input file and keep asking until a valid text file is found.
	do
	{
		std::cout << "Please input the file name: ";
		std::cin >> file_name;

		input_stream.open(file_name.c_str());
		if (input_stream.fail())
		{
			std::cout << "Couldn't find file. Try again." << std::endl;
		}

	} while (input_stream.fail());

	// Loop through all characters in input file and add to our vector (dynamic array).
	while (input_stream.get(character))
	{
		machine.push_back(character);
	}

	std::vector <std::vector<std::string> > NFA;
	std::vector<std::string> alphabet;
	std::vector<std::string> states;
	std::vector<std::string> start_states;
	std::vector<std::string> final_states;
	std::vector<std::string> transitions;
	std::vector<std::string> input_strings;

	NFA.push_back(alphabet);	// Sorry this is ugly lol
	NFA.push_back(states);
	NFA.push_back(start_states);
	NFA.push_back(final_states);
	NFA.push_back(transitions);
	NFA.push_back(input_strings);

	parse_machine(machine, NFA);
	//print_nfa(NFA);

	// This is the actual delta vector (use this)
	std::vector<std::vector<std::string> > delta;
	fix_delta(NFA, delta);

	/*
		Ok finally, you can do the traversal here. This code is scuffed lol, but it works
		Here are some notes:

			alphabet = NFA[0]			So, NFA[0][0] is first character of alpha bet, NFA[0][1] is second, etc.
			states = NFA[1]
			start states = NFA[2]
			final states = NFA[3]
			input_strings = NFA[5]		(if there are any in file)
			transitions = delta			(Not part of the NFA nested vectors)
	*/

	std::string input;
	bool finished = false;

	std::vector<std::string> personalInput;
	// NFA[5] will be empty if there aren't any input strings in the input text file
	if (NFA[5].empty())
	{
		while (!finished)
		{
			std::cout << "Please input a string(s) (type exit to stop adding inputs, all valid inputs will be tested afterwards): ";
			std::cin >> input;

			// If the input is an empty string, the user is done
			if (input == "exit")
			{
				finished = true;
				break;
			}
			if (validInput(input, NFA[0])) {
				personalInput.push_back(input);
			}
			else {
				std::cout << "not a valid input" << std::endl;
			}
			// Check if the input is supported by the NFA
			//
			
		}
		for (std::string i : personalInput) {
			isAccepted = false;
			transversal(NFA, delta, NFA[2][0], i);
			if (!isAccepted) {
				std::cout << "Input " << i << " is not accepted" << std::endl;
			}
			else {
				std::cout << "Input " << i << " is accepted" << std::endl;
			}
		}
	}
	else
	{
		// NFA[5] actually has values here. Check if they are accepted by the NFA
		//

		for (std::string i : NFA[5]) {
			if (validInput(i, NFA[0])) {
				isAccepted = false;
				transversal(NFA, delta, NFA[2][0], i);
				if (!isAccepted) {
					std::cout << "Input " << i << " is rejected" << std::endl;
				}
				else {
					std::cout << "Input "<< i << " is accepted" << std::endl;
				}

			}
			else {
				std::cout << i << " is not a valid input" << std::endl;
			}
			
		}
	}

	return 0;
}

bool validInput(const std::string& str, std::vector<std::string> lan) {
	std::string fulLan;
	for (std::string i : lan) {
		fulLan += i;
	}
	return str.find_first_not_of(fulLan) == std::string::npos;
}

void transversal(std::vector<std::vector<std::string>> NFA, std::vector<std::vector<std::string>> delta, std::string state, std::string input) {
	char key = input[0];
	std::vector<std::string> nextState;


	for (std::vector<std::string> i : delta) {
		if (key == i.at(1)[0] && state == i.at(0)) {
			nextState.push_back(i.at(2));
		}
	}

	if (input.size() > 0) {
		std::string nextInput = input.substr(1);
		for (std::string i : nextState) {
			transversal(NFA, delta, i, nextInput);
		}
	}
	else {
		if (state == NFA[3][0]) {
			isAccepted = true;
		}
	}
}

void parse_machine(std::vector<char> machine, std::vector <std::vector<std::string> >& NFA)
{
	std::string temp;
	int current_property = 0;

	for (int i = 0; i < machine.size(); i++)
	{
		std::vector<std::string>* current_input = &NFA[current_property];

		while (i < machine.size() - 1 && machine[i] != '\n')
		{
			i++;
			if (machine[i] == ',' && is_more_than_parens_and_commas(temp))
			{
				current_input->push_back(temp);
				temp = "";
			}
			else if (!is_ignored_char(machine[i]))
			{
				temp.push_back(machine[i]);
			}
		}

		if (!temp.empty() && is_more_than_parens_and_commas(temp))
		{
			current_input->push_back(temp);
			temp = "";
		}

		if (current_property <= 4 && !current_input->empty())
		{
			current_property++;
		}
	}
}

void fix_delta(std::vector <std::vector<std::string> > NFA, std::vector<std::vector<std::string> >& delta)
{
	int num_of_transitions = NFA[4].size() / 3;

	// Add empty vectors to delta before assinging transitions to them
	for (int i = 0; i < num_of_transitions; i++)
	{
		std::vector<std::string> temp;
		delta.push_back(temp);
	}

	// From the NFA vector, copy the transitions into the new delta vector
	int j = 0;
	for (int i = 0; i < NFA[4].size(); i++)
	{
		delta[j].push_back(NFA[4][i]);

		// tranisitons consist of 3 elements
		if ((i + 1) % 3 == 0)
		{
			j++;
		}
	}

	// Print the delta vector
	// std::cout << "\n";
	// for (int i = 0; i < delta.size(); i++)
	// {
	// 	for (int j = 0; j < delta[i].size(); j++)
	// 	{
	// 		std::cout << delta[i][j] << " ";
	// 	}
	// 	std::cout << "\n";
	// }
}

void print_nfa(std::vector <std::vector<std::string> > NFA)
{
	for (int i = 0; i < NFA.size(); i++)
	{
		for (int j = 0; j < NFA[i].size(); j++)
		{
			std::cout << NFA[i][j] << " ";
		}
		std::cout << "\n";
	}
}

bool is_ignored_char(char c)
{
	if (c == '(' || c == ',' || c == ')' || c == ' ' || c == '\t' || c == '\n')
	{
		return true;
	}
	return false;
}

bool is_more_than_parens_and_commas(std::string str)
{
	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] != '(' && str[i] != ')' && str[i] != ',' && str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
		{
			return true;
		}
	}
	return false;
}