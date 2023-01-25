
#include "Machine.h"
#include <sstream>
#include <iostream>

//A function to split the string
vector<string> string_split(const string& str){
    vector<string> result;
    istringstream iss(str);
    for (string s; iss >> s; )
        result.push_back(s);
    return result;
}

Machine::Machine(){}
Machine::~Machine(){}
Machine::Machine(vector<string> dpda, vector<string> inpFile)
{
    //Initializing the states
    vector<string> tempStates;
    tempStates = string_split(dpda[0]);
    tempStates[0].erase(0,2);

    stringstream test(tempStates[0]);
    string segment;
    while(getline(test, segment, ','))
    {
        this->states.push_back(segment);
    }

    stringstream test2(tempStates[2]);
    string segment2;
    while(getline(test2, segment2, ','))
    {
        if(segment2.find('(') != string::npos)
        {
            segment2.erase(0,1);
            segment2.pop_back();
            this->startStates.push_back(segment2);
        }
        else if(segment2.find('[') != string::npos)
        {
            segment2.erase(0,1);
            segment2.pop_back();
            this->endStates.push_back(segment2);
        }
    }

    //Initializing the input alphabet
    vector<string> tempInpAlph;
    tempInpAlph.push_back(dpda[1]);
    tempInpAlph[0].erase(0,2);
    stringstream test3(tempInpAlph[0]);
    string segment3;
    while(getline(test3, segment3, ','))
    {
        this->inpAlph.push_back(segment3);
    }

    //Initializing the stack alphabet
    vector<string> tempStAlph;
    tempStAlph.push_back(dpda[2]);
    tempStAlph[0].erase(0,2);
    stringstream test4(tempStAlph[0]);
    string segment4;
    while(getline(test4, segment4, ','))
    {
        this->stAlph.push_back(segment4);
    }

    //Initializing transitions
    for(int i = 3; i<dpda.size(); i++)
    {
        vector<string> tempTrans;
        tempTrans.push_back(dpda[i]);
        tempTrans[0].erase(0,2);
        stringstream test5(tempTrans[0]);
        string segment5;
        vector<string> tmp;
        while(getline(test5, segment5, ','))
        {
            tmp.push_back(segment5);
        }
        this->transition.push_back(tmp);
    }

    //Initializing the inputs
    for(int i = 0; i<inpFile.size(); i++)
    {
        vector<string> tempInps;
        tempInps.push_back(inpFile[i]);
        stringstream test6(tempInps[0]);
        string segment6;
        vector<string> tmpInps;
        while(getline(test6, segment6, ','))
        {
            tmpInps.push_back(segment6);
        }
        this->inputs.push_back(tmpInps);
    }

    this->finalState = "ACCEPT";
}

bool Machine::dpdaCheck()
{
    for(vector<string> i: this->transition)
    {
        if(count(this->states.begin(), this->states.end(), i[0]) == 0) // check states
        {
            return true;
        }
        else if(count(this->inpAlph.begin(), this->inpAlph.end(), i[1]) == 0 && i[1] != "e") // check read
        {
            return true;
        }
        else if(count(this->stAlph.begin(), this->stAlph.end(), i[2]) == 0 && i[2] != "e") // check pop
        {
            return true;
        }
        else if(count(this->states.begin(), this->states.end(), i[3]) == 0) // check next-state
        {
            return true;
        }
        else if(count(this->stAlph.begin(), this->stAlph.end(), i[4]) == 0 && i[4] != "e") // check oush
        {
            return true;
        }
    }
    return false;
}

string Machine::status()
{
    string result;
    result = this->currentState + "," + this->read + "," + this->toPop + " => " + this->nextState + "," + this->toPush +
             " [STACK]:";
    stack<string> temp;
    while (this->st.empty() == false) {
        temp.push(this->st.top());
        this->st.pop();
    }

    while (temp.empty() == false) {
        string t = temp.top();
        result += t + ",";
        temp.pop();

        // To restore contents of the original stack.
        this->st.push(t);
    }
    if(!this->st.empty())
    {
        result.pop_back();
    }
    return result;
}

bool Machine::stackEmpty()
{
    vector<string> temp;
    while (this->st.empty() == false) {
        temp.insert(temp.begin() ,this->st.top());
        this->st.pop();
    }

    for(string i: temp)
    {
        this->st.push(i);
    }

    for(string i: temp)
    {
        if(count(this->stAlph.begin(), this->stAlph.end(), i) != 0)
        {
            if(i == "$")
            {
                continue;
            }
            return false;
        }
    }

    return true;
}