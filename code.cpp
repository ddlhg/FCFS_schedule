/*******************************FCFS SCHEDULER SIMULATOR*********************************/
#include<iostream>
#include<string>
#include<list>
#include <iomanip>
using namespace std;

struct process
{
    string id; //process number
    int CPU[10]; 
    int IO[10]; 
    int cpu_Burst; //current cpu burst value
    int nextCPU; //next cpu burst in cpu array
    int io_burst; //current io burst value
    int nextIO; //next in i/o array
    int RT; //response time
    int TT; //turnaround time
    int WT; //wait time
}

//structure for the input cpu burst time, i/o time and RT
p1 = { "P1",{6,5,7,8,6,5,9,7,8},        {41,42,40,38,44,41,31,43},       0,0,0,0,-1,0,0},
p2 = { "P2",{9,7,8,12,9,11,8,12,7,6},   {24,21,36,26,31,28,21,13,11},    0,0,0,0,-1,0,0},
p3 = { "P3",{7,8,12,6,8,9,6,4,16},      {21,25,29,26,33,22,24,29},       0,0,0,0,-1,0,0},
p4 = { "P4",{5,7,14,4,9,10,11,5,3},     {35,41,45,51,61,54,82,77},       0,0,0,0,-1,0,0},
p5 = { "P5",{6,7,5,9,8,5,7,4,3},        {33,44,42,37,46,41,31,43},       0,0,0,0,-1,0,0},
p6 = { "P6",{8,12,11,12,9,19,10,6,3,4}, {24,21,36,26,31,28,21,13,11},    0,0,0,0,-1,0,0},
p7 = { "P7",{7,3,12,8,4,6,12,10},       {46,41,42,21,32,19,33},          0,0,0,0,-1,0,0},
p8 = { "P8",{6,7,8,9,10,11,8},          {14,33,51,63,87,74},             0,0,0,0,-1,0,0},
p9 = { "P9",{4,5,6,4,5,6,4,5,6},        {32,40,29,21,44,24,31,33},       0,0,0,0,-1,0,0};

/************************DECLARING THE FUNCTIONS**************************/

//Initializes the ready queue from cero time
void initializeReadyQ(list<process> &ready);

//Gets the next cpu burst
int getCPU_burst(list<process>::iterator pos, int position);

//Gest the next i/o burst
int getIO_burst(process pos, int position);

//Sorts processes based on their id (process number)
bool orderProcesses(const process &p1, const process &p2);

//Increases the waiting time for processes in the ready queue
void increaseWT(list<process> &ready);

//Prints the ready queue
void printReady(list<process> &ready);

//Prints the i/o queue
void printIO(list<process> &IO);

//Prints the processes that have been completly executed
void printFinished(list<process>& finished);

int main()
{
    list<process> finished; //list of completed processes
    list<process> IO;  //list for i/o
    list<process> ready; //list for ready processes
    process onCPU;  //process on cpu
    bool idle = false; // flag for idle cpu
    bool erased = true; // flag to erase repeating processes i/o 
    int time = 0; //current time
    int counter = 0; //counter for current cpu burst
    int numberProcesses = 9; 
    int totalRT = 0; 
    int totalWT = 0; 
    int totalTT = 0; 
    int idleTime = 0;

    //all processes are in the ready queue at time cero
    initializeReadyQ(ready);

    //getting all the processes' first cpu burst into the ready queue
    for ( list<process>::iterator i = ready.begin(); i != ready.end(); ++i ) 
    { 
        i->cpu_Burst = getCPU_burst(i, i->nextCPU);
    }

    //when no processes are in the ready and i/o queue loop will end
    while (!ready.empty() || !IO.empty())
    {
        if (idle == true)
        {
            time++; //increasing current time
            idleTime++; //increasing cpu idle time 
            if (!ready.empty())
            {
                idle = false; //ready queue is not empty so no idle time
                increaseWT(ready); //increasing waiting time for processes waiting in the ready queue 
            }

            //decrease cpu burst of processes in i/o 
            if (!IO.empty())
            {
                //go through all the processes in the i/o queue and decrease their i/o burst 
                list<process>::iterator i = IO.begin();
                while (i != IO.end() || erased == true)
                {
                    if (i->io_burst > 1)
                    {
                        i->io_burst--; //decreasing i/o burst
                        erased = false;
                        ++i; 
                    }

                    //process i/o time has been completed or has a single time unit left
                    else
                    {
                        if (i->io_burst == 1)
                        {
                            i->io_burst--; //decreasing i/o burst
                        }
                        //getting the next cpu-burst
                        i->nextCPU++;
                        i->cpu_Burst = getCPU_burst(i, i->nextCPU);

                        //go back into the ready queue
                        ready.push_back(*i);
                        idle = false;
                        i = IO.erase(i);
                        if (i != IO.end())
                        {
                            erased = true;
                        }
                    }
                }
            }
        }
        else
        {
            if (ready.empty()) //no processes ready for the cpu
            {
                idle = true; //cpu is idle
                cout << "Current time is " << time << "\nNext process on the CPU: IDLE\n";
               
                //PRINTING CONTEXT SWITCH
                printReady(ready);
                printIO(IO);
                printFinished(finished);
            }
            else  
            {
                idle = false; //processes are on cpu and ready queue
                cout << "Current time is " << time << endl << endl;
                
                onCPU = ready.front(); //first process on ready queue goes into cpu
                if (onCPU.RT < 0)
                {
                    onCPU.RT = time; //set response time
                }

                counter = onCPU.cpu_Burst; //counter set with current burst
                ready.pop_front(); //first process on ready queue is out 

                //PRINTING CONTEXT SWITCH
                cout << "Next process on the CPU: " << onCPU.id << endl; 
                printReady(ready);
                printIO(IO);
                printFinished(finished);
                           
                //decreasing current cpu burst time 
                for (counter; counter > 0; counter--)
                {
                    onCPU.cpu_Burst--; //decreases burst time of process on cpu
                    time++;
                    increaseWT(ready); //waiting time increase for processes in the ready queue  

                    //decrease i/o burst time to processes on i/o queue 
                    if (!IO.empty())
                    {
                        list<process>::iterator i = IO.begin();
                        while (i != IO.end() || erased == true)
                        {
                            if (i->io_burst > 1)
                            {
                                i->io_burst--;
                                erased = false;
                                ++i;
                            }
                            //process has completed i/o time or has a single time unit of i/o
                            else
                            {
                                if (i->io_burst == 1)
                                {
                                    i->io_burst--; //i/o time is cero so moving back into ready queue
                                }
                                
                                i->nextCPU++; //getting next cpu-burst 
                                i->cpu_Burst = getCPU_burst(i, i->nextCPU);
                                ready.push_back(*i); //because no i/o time, pushing back into ready queue
                                i = IO.erase(i);
                                if (i != IO.end())
                                {
                                    erased = true;
                                }
                            }
                        }
                    }
                }
                
                if (onCPU.cpu_Burst == 0)
                {
                    //getting next i/o burst time
                    onCPU.io_burst = getIO_burst(onCPU, onCPU.nextIO); 
                    onCPU.nextIO++;
                    
                    //no i/o time is left, the process goes to the finished list
                    if (onCPU.io_burst == 0)
                    { 
                        if (finished.empty()) 
                        {
                            onCPU.TT = time; //set turnaround time to the current time
                            finished.push_front(onCPU); //first process to be done goes in front of finished list
                        }
                        else
                        {
                            onCPU.TT = time;
                            finished.push_back(onCPU);
                        }
                    }
                    //there is i/o time on the current process
                    else
                    {
                        IO.push_back(onCPU);
                    }
                }
            }
        }
    }

    //PRINTING CONTEXT SWITCH
    cout << "Current time is " << time << "\nAll processes are done.\n\n";
    //no more processes left to be executed
    idle = true; 
    printReady(ready);
    printIO(IO);
    printFinished(finished);
    
    //printing results table (RT, WT, TT, averages, total time, and cpu utilization)
    cout << "The total time is " << time << endl;
    cout << "\n_________________________________________";
    cout << "\n|Process \tRT \tWT \tTT\t|";
    cout << "\n|=======================================|";

    finished.sort(orderProcesses);  //sorts finished processes from 1-9 to be in the table
    for (list<process>::iterator i = finished.begin(); i != finished.end(); ++i)
    {
        cout << "\n|" << i->id << "\t";
        cout << "\t" << i->RT << "\t" << i->WT << "\t" << i->TT <<"\t|";
        totalRT += i->RT;
        totalWT += i->WT;
        totalTT += i->TT;
    }
    cout << "\n|=======================================|";
    cout << setprecision(2) << fixed;
    cout << "\n|AVERAGES\t" << (float)totalRT / (float)numberProcesses << "\t" <<(float)totalWT / (float)numberProcesses <<"\t"<< (float)totalTT / (float)numberProcesses<<"\t|";
    cout << "\n|---------------------------------------|";
    cout << "\n|CPU utilization is " << 100 * (time - idleTime) / (float)time << "%\t\t|";
    cout << "\n|_______________________________________|\n\n";
    
    return 0;
}

//Initializes the ready queue from cero time
void initializeReadyQ(list<process> &ready)
{
    ready.push_front(p9);
    ready.push_front(p8);
    ready.push_front(p7);
    ready.push_front(p6);
    ready.push_front(p5);
    ready.push_front(p4);
    ready.push_front(p3);
    ready.push_front(p2);
    ready.push_front(p1);
}

//Gets the next cpu burst
int getCPU_burst(list<process>::iterator pos, int position)
{
    pos->cpu_Burst = pos->CPU[position];
    position++;
    return pos->cpu_Burst;
}

//Gest the next i/o burst
int getIO_burst(process pos, int position)
{
    pos.io_burst = pos.IO[position];
    position++;
    return pos.io_burst;
}

//Sorts processes based on their id (process number)
bool orderProcesses(const process &p1, const process &p2)
{
    if (p1.id != p2.id) return p1.id < p2.id;
}

//Increases the waiting time for processes in the ready queue
void increaseWT(list<process> &ready)
{
    for (list<process>::iterator i = ready.begin(); i != ready.end(); ++i)
    {
        i->WT++;
    }
}

//Prints the ready queue
void printReady(list<process> &ready)
{
    cout << "------------------------------------------------------\n";
    cout << "Processes in the ready queue: \n\n";
    cout << "\tProcess\t\tCPU Burst\n";
    if (ready.empty())
    {
        cout << "\tEMPTY\t\tEMPTY\n";
    }
    else
    {
        for (list<process>::iterator i = ready.begin(); i != ready.end(); ++i)
        {
            cout << "\t" << i->id << "\t\t" << i->cpu_Burst << endl;
        }
    }
}

//Prints the i/o queue
void printIO(list<process> &IO)
{
    cout << "\n------------------------------------------------------\n";
    cout << "Processes in I/O: \n\n";
    cout << "\tProcess\t\tTime left\n";
    if (IO.empty())
    {
        cout << "\tEMPTY\t\tEMPTY\n";
    }
    else
    {
        for (list<process>::iterator i = IO.begin(); i != IO.end(); ++i)
        {
            cout << "\t" << i->id << "\t\t" << i->io_burst << endl;
        }
    }
    cout << "------------------------------------------------------\n";
}

//Prints the processes that have been completly executed
void printFinished(list<process> &finished)
{
    if (!finished.empty())
    {
        cout << "Finished processes: \n\t";
        for (list<process>::iterator i = finished.begin(); i != finished.end(); ++i)
        {
            cout << i->id << ". ";
        }
    }
    else
    {
        cout << "No processes have been completed.";
    }
    cout << "\n\n=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n";
}
