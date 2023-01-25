
#ifndef ASSIGNMENT3_MACHINE_H
#define ASSIGNMENT3_MACHINE_H
#include <vector>
#include <algorithm>
#include <stack>
#include <string>
using namespace std;

class Machine {
public:
    vector<string> states; //Q (set of states)
    vector<string> startStates;
    vector<string> endStates;

    vector<string> inpAlph; //A (input alphabet)
    vector<string> stAlph; //Z (stack alphabet)

    vector<vector<string>> transition; //Transition rules
    string currentState;
    string read;
    string toPop;
    string nextState;
    string toPush;

    stack<string> st; //[STACK]
    vector<vector<string>> inputs; //data of the input file

    string finalState; //"ACCEPT" or "REJECT"

    Machine();
    Machine(vector<string> dpda, vector<string> inpFile);
    ~Machine();

    bool dpdaCheck();
    string status();
    bool stackEmpty();
};

#endif //ASSIGNMENT3_MACHINE_H
