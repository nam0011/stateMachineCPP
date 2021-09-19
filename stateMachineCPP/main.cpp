#include <iostream>
#include <fstream>


using namespace std;


//DECLARATION OF ALL GLOBAL VARIABLES WE WILL USE
const int CONNECTIONS = 9;
const int STATES = 7;

const int FOLLOW = 0;
const int PULLOUT = 1;
const int ACCELERATE = 2;
const int PULLINAHEAD = 3;
const int PULLINBEHIND = 4;
const int DECELERATE = 5;
const int DONE = 6;



//DELARATION OF ALL STATE FUNCTIONS WE WILL NEED TO USE

void followAction(ofstream &outFile, int stateCounter[], bool scenarioTrace);
void pullOutAction(ofstream &outFile, int stateCounter[], bool scenarioTrace);
void accelerateAction(ofstream &outFile, int stateCounter[], bool scenarioTrace);
void pullInAheadAction(ofstream &outFile, int stateCounter[], bool scenarioTrace);
void pullInBehindAction(ofstream &outFile, int stateCounter[], bool scenarioTrace);
void decelerateAction(ofstream &outFile, int stateCounter[], bool scenarioTrace);
void doneAction(ofstream &outFile, int stateCounter[], bool scenarioTrace);

//BEGIN MAIN ARGUMENT
int main() {

//DECLARE OUR LOCAL VARIABLES
    int scenario;
    string testFile = "C:/Users/Nathan Moore/OneDrive - uah.edu/Nathan/SP21/CS 330 - AI for Games/Projects/stateMachineCPP/stateMachineCPP/testFile.txt";       //file location information
    string statsFile = "C:/Users/Nathan Moore/OneDrive - uah.edu/Nathan/SP21/CS 330 - AI for Games/Projects/stateMachineCPP/stateMachineCPP/statsFile.txt";       //file location information
    ofstream outFile;
    string scenarioName;
    bool scenarioTrace = false;
    int scenarioIterations = 0;
    int scenarioInterval = 0;
    int stateCounter[STATES] = {0,0,0,0,0,0,0};
    int transitionCounter[CONNECTIONS] = {0,0,0,0,0,0,0,0,0};
    double probabilities[CONNECTIONS] = {0,0,0,0,0,0,0,0,0};
    int state;
    double randomNum;
    double stateSum = 0;
    double stateFreq[STATES] = {0,0,0,0,0,0,0};
    double transitionSum = 0;
    double transFreq[CONNECTIONS] = {0,0,0,0,0,0,0,0,0};
    int error = 0;


    //PROMPT USER FOR SCENARIO
    cout << endl << endl << "Welcome to the Driving State Machine please enter which scenario you would like to run" << endl;
    cout << "1 - test(100 iterations)" << endl << "2 - Statistics(1000000 iterations)" << endl;
    cin >> scenario;


    //SET FILES AND VALUES BASED ON USER SCENARIO CHOICE
    switch (scenario) {
        case 1:
            outFile.open(testFile);  //opening output file
            cout << endl;
            if (!outFile) {     //if the file doesn't open
                cout << "test file could not be opened please try again"; //prompt failure
                return 2;   //return error code quit program
            } else {    //if success
                cout << "test file opened successfully." << endl << endl; //prompt success continue program
            }

            scenarioTrace = true;
            scenarioIterations = 100;
            scenarioInterval = 1;
            scenarioName = "TEST";
            probabilities[0] = 0.8;
            probabilities[1] = 0.4;
            probabilities[2] = 0.3;
            probabilities[3] = 0.4;
            probabilities[4] = 0.3;
            probabilities[5] = 0.3;
            probabilities[6] = 0.8;
            probabilities[7] = 0.8;
            probabilities[8] = 0.5;

            break;

        case 2:
            outFile.open(statsFile);  //opening output file
            cout << endl;
            if (!outFile) {     //if the file doesn't open
                cout << "stats file could not be opened please try again"; //prompt failure
                return 2;   //return error code quit program
            } else {    //if success
                cout << "stats file opened successfully." << endl << endl; //prompt success continue program
            }

            scenarioTrace = false;
            scenarioIterations = 1000000;
            scenarioInterval = 10000;
            scenarioName = "STATISTICS";
            probabilities[0] = 0.8;
            probabilities[1] = 0.5;
            probabilities[2] = 0.4;
            probabilities[3] = 0.3;
            probabilities[4] = 0.2;
            probabilities[5] = 0.3;
            probabilities[6] = 0.8;
            probabilities[7] = 0.8;
            probabilities[8] = 0.8;

            break;
    }


    //ONCE SCENARIO IS CHOSEN RUN UNTIL THE CHOSEN AMOUNT OF ITERATIONS HAS BEEN FULFILLED
    for(int i = 0; i < scenarioIterations; i++){

        if(scenarioTrace){
            outFile <<"iteration: "<< i+1 <<endl;
        }
        //EACH TIME WE RESTART WE ALWAYS WANT TO START BY FOLLOWING ANOTHER CAR
        state = FOLLOW;
        followAction(outFile, stateCounter, scenarioTrace);

        //WE KEEP TRYING TO PASS THE CAR INFRONT OF US DON'T STOP UNTIL WE CAN ACCOMPLISH THIS TASK
        while(state != DONE){

            randomNum = ((double) rand() / (RAND_MAX)); //generates same random numbers each time depending on architecture
                                                        //don't know the <random> library functions well but this works for the
                                                        //purpose of a single run random enough to well distribute our output
        //THIS COULD ALSO WE A SWITCH STATEMENT HERE (PROB BETTER) BUT THIS WORKS AS WELL
            if(state == FOLLOW){
                if(randomNum < probabilities[0]){
                    transitionCounter[0] += 1;
                    state = PULLOUT;
                    pullOutAction(outFile, stateCounter, scenarioTrace);
                }else{
                    state = FOLLOW;
                    followAction(outFile, stateCounter, scenarioTrace);
                }
            }

            else if(state == PULLOUT){
                if(randomNum < probabilities[1]){
                    transitionCounter[1] += 1;
                    state = ACCELERATE;
                    accelerateAction(outFile, stateCounter, scenarioTrace);
                }
                else if (randomNum < probabilities[1]+probabilities[3]){
                    transitionCounter[3] += 1;
                    state = PULLINBEHIND;
                    pullInBehindAction(outFile, stateCounter, scenarioTrace);
                }
                else{
                    state = PULLOUT;
                    pullOutAction(outFile, stateCounter, scenarioTrace);
                }
            }

            else if(state == ACCELERATE){
                if(randomNum < probabilities[2]){
                    transitionCounter[2] += 1;
                    state = PULLINAHEAD;
                    pullInAheadAction(outFile, stateCounter, scenarioTrace);
                }
                else if(randomNum < probabilities[2]+probabilities[4]){
                    transitionCounter[4] += 1;
                    state = PULLINBEHIND;
                    pullInBehindAction(outFile, stateCounter, scenarioTrace);
                }
                else if(randomNum < probabilities[2]+probabilities[4]+probabilities[5]){
                    transitionCounter[5] += 1;
                    state = DECELERATE;
                    decelerateAction(outFile, stateCounter, scenarioTrace);
                }
                else{
                    state = ACCELERATE;
                    accelerateAction(outFile, stateCounter, scenarioTrace);
                }
            }

            else if(state == PULLINAHEAD){
                if(randomNum < probabilities[8]){
                    transitionCounter[8] += 1;
                    state = DONE;
                    doneAction(outFile, stateCounter, scenarioTrace);
                }
                else{
                    state = PULLINAHEAD;
                    pullInAheadAction(outFile, stateCounter, scenarioTrace);
                }
            }

            else if(state == PULLINBEHIND){
                if(randomNum < probabilities[6]){
                    transitionCounter[6] += 1;
                    state = FOLLOW;
                    followAction(outFile, stateCounter, scenarioTrace);
                }
                else{
                    state = PULLINBEHIND;
                    pullInBehindAction(outFile, stateCounter, scenarioTrace);
                }
            }

            else if(state == DECELERATE){
                if(randomNum < probabilities[7]){
                    transitionCounter[7] += 1;
                    state = PULLINBEHIND;
                    pullInBehindAction(outFile, stateCounter, scenarioTrace);
                }
                else{
                    state = DECELERATE;
                    decelerateAction(outFile, stateCounter, scenarioTrace);
                }
            }
            else{
                cout << "unexpected state value error ending program"<<endl;
                continue;
            }

            if(i % scenarioInterval ==  0){
                cout<<".";
            }
            //IF WE REACH HERE WE HAVE GOTTEN THROUGH A SCENARIO
        }
    }

    //SETTING ARRAYS OF PERCENTAGES AFTER GATHERING COUNTERS FROM
    //ABOVE STATEMACHINE
    for(int i = 0; i < STATES; i++){
        stateSum += stateCounter[i];
        stateFreq[i] = stateCounter[i]/stateSum;
    }

    for(int j = 0; j < CONNECTIONS; j++){
        transitionSum += transitionCounter[j];
        transFreq[j] = transitionCounter[j]/transitionSum;
    }

    //PRINTING INFORMATION TO THE CHOSEN FILE
    if(scenarioTrace == 1) {
        outFile << endl << endl;    //if we have a trace then skip some lines before printing information
    }
    outFile << "scenario: " << scenario << endl;
    outFile << "name: " << scenarioName << endl;
    outFile << "trace: ";
        if(scenarioTrace == 0){
            outFile << "NO" << endl;
        }else{
            outFile << "YES" << endl;
        }
    outFile << "iterations: " << scenarioIterations << endl;
    outFile << "transition probabilities: ";
        for(double probability : probabilities){
            outFile << probability << " ";
        }
        outFile << endl;
    outFile << "state counts: ";
        for(int aState : stateCounter){
            outFile << aState << " ";
        }
        outFile << endl;
    outFile << "state frequencies: ";
        for(double freq : stateFreq){
            outFile << freq << " ";
        }
        outFile << endl;
    outFile << "transition counts: ";
        for(int aTrans : transitionCounter){
            outFile << aTrans << " ";
        }
        outFile << endl;
    outFile << "transition frequencies: ";
        for(double freq : transFreq){
            outFile  << freq << " ";
        }
        outFile << endl;

//CHECK FOR ANY ERRORS IN STATE MACHINE AND DISPLAY IT TO CONSOLE
        if(stateCounter[0] < transitionCounter[6]){
            error = 1;
        }
        else if(stateCounter[1] < transitionCounter[0]){
            error = 2;
        }
        else if(stateCounter[2] < transitionCounter[1]){
            error = 3;
        }
        else if(stateCounter[3] < transitionCounter[2]){
            error = 4;
        }
        else if(stateCounter[4] < transitionCounter[3]+transitionCounter[4]+transitionCounter[7]){
            error = 5;
        }
        else if(stateCounter[5] < transitionCounter[5]){
            error = 6;
        }
        else if(stateCounter[6] < transitionCounter[8]){
            error = 7;
        }
        else if(stateCounter[1] < scenarioIterations){
            error = 8;
        }
        else if(stateCounter[7] != scenarioIterations){
            error = 9;
        }

        if(error == 0){
            cout << endl << "Verification Passed" << endl;
        }
        else{
            cout << endl << "Verification Failed with Error Code: " << error << endl;
        }

//END OF MAIN ARGUMENT
    return 0;
}


//BEGIN DEFINITIONS OF ALL THE METHODS WE WILL UTILIZE
void followAction(ofstream &outFile, int stateCounter[], bool scenarioTrace){
    if(scenarioTrace){
        outFile << "state = 1 Follow"<<endl;
    }
    stateCounter[FOLLOW] += 1;
}

void pullOutAction(ofstream &outFile, int stateCounter[], bool scenarioTrace){
    if(scenarioTrace){
        outFile << "state = 2 Pull Out"<<endl;
    }
    stateCounter[PULLOUT] += 1;
}

void accelerateAction(ofstream &outFile, int stateCounter[], bool scenarioTrace){
    if(scenarioTrace){
        outFile << "state = 3 Accelerate"<<endl;
    }
    stateCounter[ACCELERATE] += 1;
}

void pullInAheadAction(ofstream &outFile, int stateCounter[], bool scenarioTrace){
    if(scenarioTrace){
        outFile << "state = 4 Pull In Ahead"<<endl;
    }
    stateCounter[PULLINAHEAD] += 1;
}

void pullInBehindAction(ofstream &outFile, int stateCounter[], bool scenarioTrace){
    if(scenarioTrace){
        outFile << "state = 5 Pull In Behind"<<endl;
    }
    stateCounter[PULLINBEHIND] += 2;
}

void decelerateAction(ofstream &outFile, int stateCounter[], bool scenarioTrace){
    if(scenarioTrace){
        outFile << "state = 6 Decelerate"<<endl;
    }
    stateCounter[DECELERATE] += 1;
}

void doneAction(ofstream &outFile, int stateCounter[], bool scenarioTrace){
    if(scenarioTrace){
        outFile << "state = 7 Done"<<endl;
    }
    stateCounter[DONE] += 1;
}