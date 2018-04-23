// FreqMethod.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>

using namespace std;

/**
 * constants
 */

string const SOLS_FILE_NAME = "ouR.txt";
string const PRIOR_METH = "[JAC]";

/**
* classes
*/

struct letter //letter with char and frequency
{
	letter(char const ch, bool const pr) : ch_(ch), pr_(pr) {}
	char get_ch() const { return ch_; }
	char get_freq() const { return freq_; }
	void inc() { freq_++; }
private:
	char ch_;
	int freq_ = 1;
	bool pr_;
};

/**
* functions
*/

int read_frag(ifstream & fin, int & n_sols, vector<string> & sols) //reading all solutions from fragment, return № of priority method
{
	string stemp;
	int itemp, prior = 0;
	fin >> stemp; //skip _i_
	for (int i = 0; i < n_sols; i++)
	{
		fin >> stemp; //ABC
		sols.push_back(stemp);
		
		fin >> itemp; //skip №
		fin >> itemp; //skip №
		fin >> stemp; //skip [***]
		if (stemp == PRIOR_METH) prior = i;
	}
	return prior;

}

void add_letter(vector<letter> & letters, char & ch, bool prior, set<char> & cand, string & solution)
{
	bool found = false;
	for (int i = 0; i < letters.size(); i++)
	{
		if (letters[i].get_ch() == ch)
		{
			letters[i].inc();
			found = true;
			break;
		}
	}
	if (!found) letters.push_back(letter(ch, prior));
}

string most_freq_sol(vector<string> & sols, int & n_sols, int & len_sols, int & prior) //computing solution with the most frequent characters
{
	string solution;
	set<char> candidates;
	for (int i = 0; i < len_sols; ++i)
	{
		vector<letter> letters;

		for (int it_sol = 0; it_sol < n_sols; ++it_sol)
		{
			add_letter(letters, sols[it_sol][i], prior == it_sol, candidates, solution);

		}
	}
	return solution;
}



int main()
{
	setlocale(LC_ALL, "rus");

	ifstream fin(SOLS_FILE_NAME); //init ouR file
	if (!fin.is_open()) 
		return 1;

	int n_frags = 0, n_sols = 0;

	fin >> n_frags >> n_sols;
	for (int it_frags = 0; it_frags < n_frags; ++it_frags)
	{
		vector<string> sols;
		read_frag(fin, n_sols, sols);

		for (int i = 0; i < sols.size(); i++) {
			cout << sols[i] << endl;
		}
	}
	

	fin.close();

	system("pause");
    return 0;
}

