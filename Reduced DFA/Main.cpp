/***************************************************************
*      RE -> ε-NFA -> DFA -> reduced DFA                       *
*                                           컴퓨터공학과 유수민     *
*                                              2023. 6. 19     *
***************************************************************/

#include "Transducer.h"
using namespace std;

string fileName; 
int main(int argc, char *argv[]){

    string regex_i = argv[1];
    
    regex_i = formToregular(regex_i);
    inTopost(regex_i);
    
    fileName = "e-NFA";
    reToenfa();
    printFA(fileName);

    fileName = "DFA";
    //enfaTodfa();
    //printFA(fileName);

    fileName = "reduced-DFA";
    //dfaToredfa();
    //printFA(fileName);

    return 0;
}



