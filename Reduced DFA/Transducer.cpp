#define _CRT_SECURE_NO_WARNINGS

#include "Transducer.h"
using namespace std;


char arrRE[MaxRE];                  // Regular Expression
set<int> stateSet;                  // StateSet
set<char> terminalSet;              // TerminalSet
vector<nodeState> deltafuncSet;     // DeltaFunctions
int startState;                     // StartState
set<int> finalstateSet;             // FinalStateSet



int startnode, finalnode;           // Currnent startnode, finalnode
int opidx = 0;                      // arrRE에서 사용된 연산자, 피연산자의 총 개수
int statecnt = 0;                   // 현재까지 사용된 상태 개수


string formThree(int num){          // Make Fornal Node : 15 -> q015
    string str = "q";
    if(num%10 == num) str = str + "00" + to_string(num);
    else if(num%100 == num) str = str + "0" + to_string(num);
    else str = to_string(num);

    return str;
}

// RE -> ε-NFA
void reToenfa(){            
      
    int newNode1 = 0;
    char input;
    int newNode2 = 0;               // (newNode1, input) = {newNode2}
    stack<operandState> tempStack;

    for(int i=0; i<opidx; i++){

        if(arrRE[i] == '*'){
            operandState temp1 = tempStack.top();
            tempStack.pop();
            newNode1 = statecnt++;
            input = '^';
            newNode2 = statecnt++;

            makeDeltaSet(newNode1, input, temp1.startmp); 
            makeDeltaSet(newNode1, input, newNode2); 
            makeDeltaSet(temp1.endtmp, input, temp1.startmp); 
            makeDeltaSet(temp1.endtmp, input, newNode2); 

            startnode = newNode1;
            finalnode = newNode2;
            tempStack.push(operandState(startnode, finalnode));

            stateSet.insert(newNode1);
            stateSet.insert(newNode2);       // stateSet에 추가
        }
        else if(arrRE[i] == '.'){
            operandState temp2 = tempStack.top();
            tempStack.pop();
            operandState temp1 = tempStack.top();
            tempStack.pop();
            input = '^';

            makeDeltaSet(temp1.endtmp, input, temp2.startmp);
            startnode = temp1.startmp;
            finalnode = temp2.endtmp;
            tempStack.push(operandState(startnode, finalnode));                       // temp1의 끝노드와 temp2의 시작노드를 잇는다.
        }
        else if(arrRE[i] == '+'){
            operandState temp2 = tempStack.top();
            tempStack.pop();
            operandState temp1 = tempStack.top();
            tempStack.pop();
            newNode1 = statecnt++;
            input = '^';
            newNode2 = statecnt++;

            makeDeltaSet(newNode1, input, temp1.startmp); 
            makeDeltaSet(newNode1, input, temp2.startmp);
            makeDeltaSet(temp1.endtmp, input, newNode2); 
            makeDeltaSet(temp2.endtmp, input, newNode2); 
            
            startnode = newNode1;
            finalnode = newNode2;
            tempStack.push(operandState(startnode, finalnode));

            stateSet.insert(newNode1);       
            stateSet.insert(newNode2);       // stateSet에 추가
        }
        else{                                   // Terminal symbol
            newNode1 = statecnt++;
            input = arrRE[i];
            newNode2 = statecnt++;

            makeDeltaSet(newNode1, input, newNode2); 
            tempStack.push(operandState(newNode1, newNode2));

            stateSet.insert(newNode1);
            stateSet.insert(newNode2);       // stateSet에 추가
            bool isExist = false;
            terminalSet.insert(input);    // terminalSet에 추가
        }
    }
    startState = startnode;                   // startState
    finalstateSet.insert(finalnode);       // finalStateSet
}

// Compute epsilon closure of a set of states
set<int> epsilonClosure(const set<int>& states) {
    set<int> closure = states;
    stack<int> stateStack;
    
    // Push initial states onto the stack
    for (int state : states) {
        stateStack.push(state);
    }
    
    while (!stateStack.empty()) {
        int currentState = stateStack.top();
        stateStack.pop();
        
        // Find epsilon transitions from the current state
        for (const nodeState& transition : deltafuncSet) {
            if (transition.trans.X == currentState && transition.trans.Y == '^') {
                for (int nextState : transition.outputSet) {
                    // Add the next state to the closure if it's not already present
                    if (closure.find(nextState) == closure.end()) {
                        closure.insert(nextState);
                        stateStack.push(nextState);
                    }
                }
            }
        }
    }
    
    return closure;
}


// ε-NFA -> DFA
void enfaTodfa() {
    // set<int> startState;
    // startState.insert(startnode);

    // set<set<int> > dfaStates;
    // map<set<int>, int> dfaStateMapping;
    
    // set<int> initialClosure = epsilonClosure(startState);
    // dfaStates.insert(initialClosure);
    // dfaStateMapping[initialClosure] = 0;
    
    // queue<set<int> > stateQueue;
    // stateQueue.push(initialClosure);
    
    // while (!stateQueue.empty()) {
    //     set<int> currentState = stateQueue.front();
    //     stateQueue.pop();
        
    //     // Compute transitions for each terminal symbol
    //     for (char terminal : terminalSet) {
    //         set<int> transitionSet;
            
    //         for (int state : currentState) {
                
    //             for (auto& transition : deltafuncSet) {
    //                 if (transition.trans.X == state && transition.trans.Y == terminal) {
    //                     for (int nextState : transition.outputSet) {
                            
    //                         transitionSet.insert(nextState);
    //                     }
    //                 }
    //             }
    //         }
            
    //         set<int> epsilonClosureSet = epsilonClosure(transitionSet);
            
    //         if (!epsilonClosureSet.empty() && dfaStates.find(epsilonClosureSet) == dfaStates.end()) {
    //             dfaStates.insert(epsilonClosureSet);
    //             stateQueue.push(epsilonClosureSet);
                
                
    //             int newStateNumber = dfaStates.size() - 1;
    //             dfaStateMapping[epsilonClosureSet] = newStateNumber;
    //         }
            
            
    //         if (!epsilonClosureSet.empty()) {
    //             makeDeltaSet(dfaStateMapping[currentState], terminal, dfaStateMapping[epsilonClosureSet]);
    //         }
    //     }
    // }
}




// DFA -> Reduced DFA
void dfaToredfa() {
    
}



// Make deltaSet
void makeDeltaSet(int num1, char c, int num2){
    bool isExist = false;
    for(auto& e : deltafuncSet){
        if(e.trans.X == num1 && e.trans.Y == c){
            e.outputSet.push_back(num2);
            
            isExist = true;
            break;
        }
    }
    if (!isExist) {
        vector<int> strVec;
        strVec.push_back(num2);
        deltafuncSet.push_back(nodeState(make_pair(num1, c), strVec));
    }
}



// Print Finite Automata
void printFA(string file){
    
    ofstream file_writer(file);         // FILE Writer

    file_writer << "StateSet       = { ";
    for(auto it = stateSet.begin(); it != stateSet.end();) {
        file_writer << formThree(*it);
        it++;
        if(it!= stateSet.end()) file_writer << ", ";
    }
    file_writer << " }\n";

    file_writer << "TerminalSet    = { ";
    for(auto it = terminalSet.begin(); it != terminalSet.end();) {
        file_writer << *it;
        it++;
        if(it!= terminalSet.end()) file_writer << ", ";
    }
    file_writer << " }\n";

    file_writer << "DeltaFunctions = { \n";
    for(auto& c : deltafuncSet){
        file_writer << "     (" << formThree(c.trans.X) << ", ";
        if(c.trans.Y == '^') file_writer << "ε";
        else file_writer << c.trans.Y;
        file_writer << ") = " << "{ ";
        for(auto it = c.outputSet.begin(); it != c.outputSet.end();) {
            file_writer << formThree(*it);
            it++;
            if(it!= c.outputSet.end()) file_writer << ", ";
        }
        file_writer << " }" << "\n";
    }
    file_writer << "}\n";

    file_writer << "StartState     =  ";
    file_writer << formThree(startState);
    file_writer << "\n";

    file_writer << "FinalStateSet  = { "; 
    for(auto it = finalstateSet.begin(); it != finalstateSet.end();) {
        file_writer << formThree(*it);
        it++;
        if(it!= finalstateSet.end()) file_writer << ", ";
    }
    file_writer << " }\n";

    file_writer.close();
}

// Change short from to regular form
string formToregular(string regex_i){
    regex_i = regex_replace(regex_i, regex("∙"), ".");
    string temp = "";
    for(int i=0; i<regex_i.length(); i++){
        temp += regex_i[i];
        if(i+1 == regex_i.length()) continue;
        if((regex_i[i] >= 48 && regex_i[i] <= 57 || regex_i[i] >= 65 && regex_i[i] <= 90 || regex_i[i] >= 97 && regex_i[i] <= 122)
            && ((regex_i[i+1] >= 48 && regex_i[i+1] <= 57 || regex_i[i+1] >= 65 && regex_i[i+1] <= 90 || regex_i[i+1] >= 97 && regex_i[i+1] <= 122) 
            || (regex_i[i+1] == '('))) temp += '.';
        else if((regex_i[i] == '*') 
            && ((regex_i[i+1] == '(') 
            || (regex_i[i+1] >= 48 && regex_i[i+1] <= 57 || regex_i[i+1] >= 65 && regex_i[i+1] <= 90 || regex_i[i+1] >= 97 && regex_i[i+1] <= 122))) temp += '.';
        else if((regex_i[i] == ')') 
            && ((regex_i[i+1] >= 48 && regex_i[i+1] <= 57 || regex_i[i+1] >= 65 && regex_i[i+1] <= 90 || regex_i[i+1] >= 97 && regex_i[i+1] <= 122) 
            || (regex_i[i+1] == '('))) temp += '.';
    }
    return temp;
}

// Change Regualr Expression to Postfix
void inTopost(string regex_i){
    
    stack<char> Stack;

    for(auto c : regex_i){
        if(c >= 48 && c <= 57 || c >= 65 && c <= 90 || c >= 97 && c <= 122) arrRE[opidx++] = c;
        
        else{
            switch(c){
            case '(':
                Stack.push(c);
                break;

            case ')':
                if(!Stack.empty()) arrRE[opidx++] = Stack.top();
                while(Stack.top() != '(') Stack.pop();
                Stack.pop();  
                break;

            case '*':
                if(!Stack.empty()){
                    while(Stack.top() == '*'){
                        arrRE[opidx++] = Stack.top();
                        Stack.pop();     
                        if(Stack.empty()) break;         
                    }
                }
                Stack.push(c);
                break;
                
            case '.':
                if(!Stack.empty()){
                    while(Stack.top() == '*' || Stack.top() == '.'){
                        arrRE[opidx++] = Stack.top();
                        Stack.pop();        
                        if(Stack.empty()) break;         
                    }
                }
                Stack.push(c);
                break;

            case '+':
                if(!Stack.empty()){
                    while(Stack.top() == '*' || Stack.top() == '.' || Stack.top() == '+'){
                        arrRE[opidx++] = Stack.top();
                        Stack.pop();         
                        if(Stack.empty()) break;        
                    }
                }
                Stack.push(c);
                break;
            }
        }
    }
    while(!Stack.empty()){
        arrRE[opidx++] = Stack.top();
        Stack.pop();
        if(Stack.empty()) break;   
    }
}

