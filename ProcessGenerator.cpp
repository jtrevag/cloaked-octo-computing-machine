using namespace std; 
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <time.h>

//I am using a struct to hold my processes. It has all three pieces of required data. 
struct fauxProcess 
{
	int procID;
	int numCycles;
	int memSize;
};

//This function simply prints all of the processes and their information, as well as the average for cycles and memory. 
void printProcesses (fauxProcess processList[], int k)
{
	int sumCycles, sumMemSize;
	for (int i = 0; i < k; i++)
	{
		cout << "Process ID: " << processList[i].procID << "\tNumber of Cycles: " << processList[i].numCycles << "\tMemory Footprint (KB): " << processList[i].memSize << endl;
		sumCycles += processList[i].numCycles;
		sumMemSize += processList[i].memSize;
	}
	
	cout << "Average Number of Cycles: " << sumCycles / k << "\tAverage Memory Footprint: " << sumMemSize / k << endl;
};

void generateProcesses (int k)
{
	int numCycles, memSize;
	int majority = floor(0.5 * k);
	int mid = floor (0.33 * k);
	int remainder = k - majority - mid;
	
	fauxProcess processList[k];
	srand (time(NULL));
	
	for (int i = 0; i < k; i++)
	{
		if (i < majority) {  //The majority of process will be in the range <4000, 8000> cycles. 
			numCycles = rand() % 4000 + 4000;
		if (i % 17 == 0)   //Only 1 in 17 processes will have a memory footprint > 30 KB. 
			memSize = rand() % 100 + 30;
		else
			memSize = rand() % 30 + 1;
		}
		else if (i < mid) {  //The second most processes will be a bit further away, <2000,4000> and <8000, 10000>. 
			if (i % 2 == 0)  //Every other process will be in the lower bounded range, and the rest will be the upper range. 
				numCycles = rand() % 2000 + 2000;
			else 
				numCycles = rand() % 2000 + 8000;
			if (i % 15 == 0) 
				memSize = rand() % 100 + 30;
			else
				memSize = rand() % 30 + 1;
		}
		else {  //The rest of the processes will be in the range <1000, 2000> and <10000, 11000>. 
			if (i % 2 == 0)
				numCycles = rand() % 1000 + 1000;
			else 
				numCycles = rand() % 1000 + 10000;
			if (i % 13 == 0) 
				memSize = rand() % 100 + 30;
			else
				memSize = rand() % 30 + 1;
		}
		
		//Store all of the process information in the process array. 
		processList[i].procID = i;
		processList[i].numCycles = numCycles;
		processList[i].memSize = memSize;
	}
	
	printProcesses(processList, k);
};


int main ()
{
	generateProcesses(10000);
	return 0;
}
