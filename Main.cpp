#include <iostream>
#include "Machine.h"
#include <fstream>
#include <sstream>

int main(int argc, char** argv) {

    //Getting the data of dpda file
    vector<string> data;
    fstream new_file;
    new_file.open(argv[1], ios::in);
    string line;
    while (getline (new_file, line))
    {
        data.push_back(line);
    }
    new_file.close();

    //Getting the data of input file
    vector<string> inpdata;
    fstream new_file2;
    new_file2.open(argv[2], ios::in);
    string line2;
    while (getline (new_file2, line2))
    {
        inpdata.push_back(line2);
    }
    new_file2.close();

    Machine* m = new Machine(data, inpdata);

    if(m->dpdaCheck()) //Checking the DPDA file
    {
        ofstream outputFile(argv[3]);
        outputFile << "Error [1]:DPDA description is invalid!" << endl;
        outputFile.close();
        return 1;
    }

    vector<vector<string>> tmpInputs; //Temporary inputs
    for(vector<string> i: m->inputs)
    {
        tmpInputs.push_back(i);
    }

    while(!(tmpInputs.empty()))
    {
        int currentInput = 0;

        //A special case where input line is empty
        //and there is no starting selection for
        //the initial state
        vector<vector<string>> testCase;
        for(vector<string> t: m->transition)
        {
            if(t[0] == m->startStates[0] && t[1] == "e")
            {
                testCase.push_back(t);
            }
        }
        do
        {
            if(tmpInputs[0].empty() && testCase.empty())
            {
                ofstream outputFile2;
                outputFile2.open(argv[3], ios_base::app);
                outputFile2 << m->finalState << endl;
                outputFile2 << endl;
                outputFile2.close();

                tmpInputs.erase(tmpInputs.begin());
            }
        }while(tmpInputs[0].empty());

        //Selecting the starting transition rule.
        vector<vector<string>> candidatesForStart;
        vector<vector<string>> candidatesForEmptyStart;
        for(vector<string> t: m->transition)
        {
            if(t[0] == m->startStates[0] && t[1] == tmpInputs[0][currentInput])
            {
                candidatesForStart.push_back(t);
            }
            else if(t[0] == m->startStates[0] && t[1] == "e")
            {
                candidatesForEmptyStart.push_back(t);
            }
        }
        if(!candidatesForStart.empty())
        {
            m->currentState = candidatesForStart[0][0];
            m->read = candidatesForStart[0][1];
            m->toPop = candidatesForStart[0][2];
            m->nextState = candidatesForStart[0][3];
            m->toPush = candidatesForStart[0][4];
            currentInput++;
        }
        else
        {
            m->currentState = candidatesForEmptyStart[0][0];
            m->read = candidatesForEmptyStart[0][1];
            m->toPop = candidatesForEmptyStart[0][2];
            m->nextState = candidatesForEmptyStart[0][3];
            m->toPush = candidatesForEmptyStart[0][4];
        }

        //First operation
        if(m->toPush != "e")
        {
            m->st.push(m->toPush);
        }

        ofstream outputFile;
        outputFile.open(argv[3], ios_base::app);
        outputFile << m->status() << endl;
        outputFile.close();

        //Switching between transition rules and performing operations
        do {
            //Switching
            vector<vector<string>> candidatesForNextState;
            for(vector<string> t: m->transition)
            {
                if(t[0] == m->nextState)
                {
                    candidatesForNextState.push_back(t);
                }
            }

            vector<vector<string>> nextStateEmpty;
            vector<vector<string>> nextStateInput;
            for(vector<string> t: candidatesForNextState)
            {
                if(currentInput < tmpInputs[0].size() && t[1] == tmpInputs[0][currentInput])
                {
                    nextStateInput.push_back(t);
                }
                else if(t[1] == "e")
                {
                    nextStateEmpty.push_back(t);
                }
            }

            if(nextStateInput.size() >= 2)
            {
                for(vector<string> t: nextStateInput)
                {
                    if(t[2] == m->st.top())
                    {
                        m->currentState = t[0];
                        m->read = t[1];
                        m->toPop = t[2];
                        m->nextState = t[3];
                        m->toPush = t[4];
                        currentInput++;
                        break;
                    }
                }
            }
            else if(nextStateInput.size() == 1)
            {
                m->currentState = nextStateInput[0][0];
                m->read = nextStateInput[0][1];
                m->toPop = nextStateInput[0][2];
                m->nextState = nextStateInput[0][3];
                m->toPush = nextStateInput[0][4];
                currentInput++;
            }
            else
            {
                if(!nextStateEmpty.empty())
                {
                    m->currentState = nextStateEmpty[0][0];
                    m->read = nextStateEmpty[0][1];
                    m->toPop = nextStateEmpty[0][2];
                    m->nextState = nextStateEmpty[0][3];
                    m->toPush = nextStateEmpty[0][4];
                }
                else
                {
                    break;
                }
            }

            //Perform operations
            if(m->toPop != "e")
            {
                if(m->toPop != m->st.top())
                {
                    break;
                }
                m->st.pop();
            }

            if(m->toPush != "e")
            {
                m->st.push(m->toPush);
            }
            ofstream outputFile2;
            outputFile2.open(argv[3], ios_base::app);
            outputFile2 << m->status() << endl;
            outputFile2.close();
        } while (currentInput <= tmpInputs[0].size() || m->read != "e");


        //Checking the final state of the machine
        if(m->stackEmpty() && count(m->endStates.begin(), m->endStates.end(), m->nextState) != 0)
        {
            ofstream outputFile2;
            outputFile2.open(argv[3], ios_base::app);
            outputFile2 << m->finalState << endl;
            outputFile2 << endl;
            outputFile2.close();
        }
        else
        {
            m->finalState = "REJECT";
            ofstream outputFile2;
            outputFile2.open(argv[3], ios_base::app);
            outputFile2 << m->finalState << endl;
            outputFile2 << endl;
            outputFile2.close();
        }

        m->finalState = "ACCEPT";
        while (!m->st.empty())
        {
            m->st.pop();
        }
        m->currentState = "";
        m->read = "";
        m->toPop = "";
        m->nextState = "";
        m->toPush = "";

        tmpInputs.erase(tmpInputs.begin());
    }

    return 0;
}
