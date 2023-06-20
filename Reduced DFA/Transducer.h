# pragma once

# define MaxRE 100000
# define X first
# define Y second

#include <iostream>
#include <string.h>
#include <fstream>
#include <stack>
#include <vector>
#include <regex>
#include <set>
#include <map>
#include <queue>
using namespace std;


string formThree(int num);                      // 1 -> q001, 15 -> q015
string formToregular(string regex_i);           // ab -> a.b or a∙b -> a.b

void inTopost(string regex_i);                  // Step0
void reToenfa();                                // Step1    RE -> ε-NFA
void makeDeltaSet(int num1, char c, int num2);
void enfaTodfa();                               // Step2    ε-NFA -> DFA                      
void dfaToredfa();                              // Step3    DFA -> reduced DFA
void printFA(string fileName);                  // Step4    print Finite Automata


struct nodeState {
    pair<int, char> trans; 
    vector<int> outputSet;
    nodeState(const pair<int, char>& trans, const vector<int>& outputSet) : trans(trans), outputSet(outputSet) {}
};


struct operandState {
    int startmp;
    int endtmp;
    operandState(const int& start, const int& end) : startmp(start), endtmp(end) {}
};
