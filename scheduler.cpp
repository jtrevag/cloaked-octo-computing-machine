#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include "process.h"
using namespace std;

ofstream file;

//For each processor, take the next smalles and the next largest process based on number of cycles from each side of the process list and put it on that processor.
int sieveDistribution(process processes[], int size){
	process* sortedProcesses = new process[size];
	sortedProcesses = sortProcesses(processes, size); 

	processor processorList[5];

	for(int i = 0; i < 5; i++){
		processorList[i].totalCycles =0;
		processorList[i].countProcesses=0;
		processorList[i].avgWait = 0;
		processorList[i].avgTurn = 0;
		processorList[i].processList = new process[size];
	}
	
	int processorCount = 0;
	int elementCount = 0;
	int forward = 1;
	for(int i = 0; i<size; i++){
		processorList[processorCount].processList[elementCount] = sortedProcesses[i];
		processorList[processorCount].totalCycles += sortedProcesses[i].cycles;
		processorList[processorCount].countProcesses++;
		// processors[processorCount][elementCount] = sortedProcesses[i];
		if(forward){
			processorCount++;
		} else{
			processorCount--;
		}

		if(processorCount == 5){
			forward = 0;
			processorCount = 4;
			elementCount++;
		}
		if(processorCount == -1){
			forward = 1;
			processorCount = 0;
			elementCount++;
		}
	}

	
	//Sorts the processes to reduce the total waiting time. This reduces the average wait time by about 5000 seconds. 
	for (int i = 0; i < 5; i++) {
		processorList[i].processList = sortProcesses(processorList[i].processList, processorList[i].countProcesses);
		processorList[i].waitTime = new int[processorList[i].countProcesses];
		processorList[i].turnaroundTime = new int[processorList[i].countProcesses];
	}
	
	//Finds the total and average for wait and turnaround time. 
	for (int i = 0; i < 5; i++) {
		int totalWait = 0;
		int totalTurn = 0;
		processorList[i].waitTime[0] = 0;
		processorList[i].turnaroundTime[0] = processorList[i].processList[0].cycles / 4000;
		totalTurn += processorList[i].turnaroundTime[0];
		for (int j = 1; j < processorList[i].countProcesses; j++) {
			processorList[i].waitTime[j] = processorList[i].waitTime[j-1] + (processorList[i].processList[j - 1].cycles / 4000);
			processorList[i].turnaroundTime[j] = processorList[i].turnaroundTime[j-1] + (processorList[i].processList[j].cycles / 4000);
			totalWait += processorList[i].waitTime[j];
			totalTurn += processorList[i].turnaroundTime[j];
		}
		processorList[i].avgWait = totalWait / processorList[i].countProcesses;
		processorList[i].avgTurn = totalTurn / processorList[i].countProcesses;
	}
	
	for(int i = 0; i < 5; i++){
		file << "Processor " << i << endl;
		file << "Total runtime: " << processorList[i].totalCycles / 4000 << "s\tAvg Wait Time: " << processorList[i].avgWait << "s\tAvg Turnaround Time: " << processorList[i].avgTurn << "s" << endl;
	}
}


//Places a process in whichever bucket it is allowed in which is ALSO the smallest size. Similar to sequential. 
/*
NOTES [Why I had trouble with this]
The difficulty with this problem is distributing the processes evenly while ALSO putting them in allowed buckets. 
Suppose there is a process distribution with very very few processes with memory > 2000. If I only put processes
with mem > 2000 in the 4th processor, the cycle count will be too low. So I had to use a similar method to the one 
found in sequentialDistribution to make sure that the processes are split evenly while also ensuring they are able to 
run within memory requirements for that processor. 
*/
int restrictedMemory (process processes[], int size) {
	process* sortedProcesses = new process[size];
	sortedProcesses = sortProcesses(processes, size); 

	process processors[5][size];
	int processCount[5] = { 0 }; 
	int elementCount[5] = { 0 };
	
	int mIndex = 0; //keeps track of the processor with the smallest number of cycles. 
	int min = 0;
	
	int highMemCycles = 0; //Keeps track of cycles coming from processes with memory > 2000
	
	for (int i = size - 1; i >= 0; i--) {
		if (sortedProcesses[i].memory > 2000) { //These processes MUST go in the last processor.
			processors[4][elementCount[4]] = sortedProcesses[i];
			elementCount[4]++;
			processCount[4] += sortedProcesses[i].cycles;
			highMemCycles += sortedProcesses[i].cycles;
		}
		else if (sortedProcesses[i].memory > 1000) {
			min = processCount[2]; //Code block to find the smallest cycle count among processors 3-5
			mIndex = 2;
			for (int j = 2; j < 5; j++) {
				if (processCount[j] < min) {
					min = processCount[j];
					mIndex = j;
				}
			}
			
			processors[mIndex][elementCount[mIndex]] = sortedProcesses[i];
			elementCount[mIndex]++;
			processCount[mIndex] += sortedProcesses[i].cycles;
		}
		else { //The process has memory < 1000
			min = processCount[0]; //Code block to find the smallest cycle count among all processors. 
			mIndex = 0;
			for (int j = 0; j < 5; j++) {
				if (processCount[j] < min) {
					min = processCount[j];
					mIndex = j;
				}
			}
			
			processors[mIndex][elementCount[mIndex]] = sortedProcesses[i];
			elementCount[mIndex]++;
			processCount[mIndex] += sortedProcesses[i].cycles;
		}
	}
	
	//Converting the processor[] array into an array using struct processor. 
	processor processorList[5];
	for(int i = 0; i < 5; i++){
		processorList[i].processList = new process[elementCount[i]];
		for (int j = 0; j < elementCount[i]; j++) {
			processorList[i].processList[j] = processors[i][j];
		}
		processorList[i].totalCycles = processCount[i];
		processorList[i].countProcesses=elementCount[i];
		processorList[i].avgWait = 0;
		processorList[i].avgTurn = 0;
	}
	
	//Sorts the processes to reduce the total waiting time. This reduces the average wait time by about 5000 seconds. 
	for (int i = 0; i < 5; i++) {
		processorList[i].processList = sortProcesses(processorList[i].processList, processorList[i].countProcesses);
		processorList[i].waitTime = new int[processorList[i].countProcesses];
		processorList[i].turnaroundTime = new int[processorList[i].countProcesses];
	}
	
	//Finds the total and average for wait and turnaround time. 
	for (int i = 0; i < 5; i++) {
		int totalWait = 0;
		int totalTurn = 0;
		processorList[i].waitTime[0] = 0;
		processorList[i].turnaroundTime[0] = processorList[i].processList[0].cycles / 4000;
		totalTurn += processorList[i].turnaroundTime[0];
		for (int j = 1; j < processorList[i].countProcesses; j++) {
			processorList[i].waitTime[j] = processorList[i].waitTime[j-1] + (processorList[i].processList[j - 1].cycles / 4000);
			processorList[i].turnaroundTime[j] = processorList[i].turnaroundTime[j-1] + (processorList[i].processList[j].cycles / 4000);
			totalWait += processorList[i].waitTime[j];
			totalTurn += processorList[i].turnaroundTime[j];
		}
		processorList[i].avgWait = totalWait / processorList[i].countProcesses;
		processorList[i].avgTurn = totalTurn / processorList[i].countProcesses;
	}
	
	for(int i = 0; i < 5; i++){
		file << "Processor " << i << endl;
		file << "Total runtime: " << processorList[i].totalCycles / 4000 << "s\tAvg Wait Time: " << processorList[i].avgWait << "s\tAvg Turnaround Time: " << processorList[i].avgTurn << "s" << endl;
	}
}	

int limitedSpeed (process processes[], int size) {
	process* sortedProcesses = new process[size];
	sortedProcesses = sortProcesses(processes, size); 

	//Structure to hold the total number of cycles and adjusted number of cycles for each processor.
	processor processors[5];
	for(int i = 0; i < 5; i++){
		processors[i].totalCycles =0;
		processors[i].countProcesses=0;
		processors[i].adjustedCycles=0;
		processors[i].avgWait = 0;
		processors[i].avgTurn = 0;
		processors[i].processList = new process[size];
	}

	
	for(int i = 99; i>=0; i--){
		//find processor with minimum
		int procIndexMin = 0;
		for(int j = 0; j < 5; j++){
			if(processors[procIndexMin].adjustedCycles > processors[j].adjustedCycles){
				procIndexMin = j;
			}
		}
	
		processors[procIndexMin].totalCycles += sortedProcesses[i].cycles;
		processors[procIndexMin].processList[processors[procIndexMin].countProcesses] = sortedProcesses[i];
		processors[procIndexMin].countProcesses++;
		switch(procIndexMin){
			case(0):
				processors[procIndexMin].adjustedCycles +=  (2 * sortedProcesses[i].cycles);
				break;
			case(1):
				processors[procIndexMin].adjustedCycles +=  (2 * sortedProcesses[i].cycles);
				break;
			case(2):
				processors[procIndexMin].adjustedCycles +=  (1 * sortedProcesses[i].cycles);
				break;
			case(3):
				processors[procIndexMin].adjustedCycles +=  (1 * sortedProcesses[i].cycles);
				break;
			default:
				processors[procIndexMin].adjustedCycles +=  (0.5 * sortedProcesses[i].cycles);
				break;
		}
	}
	
	//Sorts the processes to reduce the total waiting time. This reduces the average wait time by about 5000 seconds. 
	for (int i = 0; i < 5; i++) {
		processors[i].processList = sortProcesses(processors[i].processList, processors[i].countProcesses);
		processors[i].waitTime = new int[processors[i].countProcesses];
		processors[i].turnaroundTime = new int[processors[i].countProcesses];
	}

	//This monstrous block of code is for determining the wait time and turnaround time for each process. 
	for (int i = 0; i < 5; i++) {
		int totalWait = 0;
		int totalTurn = 0;
		processors[i].waitTime[0] = 0;
		processors[i].turnaroundTime[0] = processors[i].processList[0].cycles / 2000;
		totalTurn += processors[i].turnaroundTime[0];
		for (int j = 1; j < processors[i].countProcesses; j++) {
			switch (i) {
				case (0):
					processors[i].waitTime[j] = processors[i].waitTime[j-1] + (processors[i].processList[j - 1].cycles / 1000);
					processors[i].turnaroundTime[j] = processors[i].turnaroundTime[j-1] + (processors[i].processList[j].cycles / 1000);
					break;
				case (1):
					processors[i].waitTime[j] = processors[i].waitTime[j-1] + (processors[i].processList[j - 1].cycles / 1000);
					processors[i].turnaroundTime[j] = processors[i].turnaroundTime[j-1] + (processors[i].processList[j].cycles / 1000);
					break;
				case (2):
					processors[i].waitTime[j] = processors[i].waitTime[j-1] + (processors[i].processList[j - 1].cycles / 2000);
					processors[i].turnaroundTime[j] = processors[i].turnaroundTime[j-1] + (processors[i].processList[j].cycles / 2000);
					break;
				case (3):
					processors[i].waitTime[j] = processors[i].waitTime[j-1] + (processors[i].processList[j - 1].cycles / 2000);
					processors[i].turnaroundTime[j] = processors[i].turnaroundTime[j-1] + (processors[i].processList[j].cycles / 2000);
					break;
				case (4):
					processors[i].waitTime[j] = processors[i].waitTime[j-1] + (processors[i].processList[j - 1].cycles / 4000);
					processors[i].turnaroundTime[j] = processors[i].turnaroundTime[j-1] + (processors[i].processList[j].cycles / 4000);
					break;
			}
			totalWait += processors[i].waitTime[j];
			totalTurn += processors[i].turnaroundTime[j];
		}
		processors[i].avgWait = totalWait / processors[i].countProcesses;
		processors[i].avgTurn = totalTurn / processors[i].countProcesses;
	}
	
	for(int i = 0; i < 5; i++){
		file << "Processor " << i << endl;
		switch(i) {
			case (0):
				file << "Total runtime: " << processors[i].totalCycles / 1000 << "s\tAvg Wait Time: " << processors[i].avgWait << "s\tAvg Turnaround Time: " << processors[i].avgTurn << "s" << endl;
				break;
			case (1):
				file << "Total runtime: " << processors[i].totalCycles / 1000 << "s\tAvg Wait Time: " << processors[i].avgWait << "s\tAvg Turnaround Time: " << processors[i].avgTurn << "s" << endl;
				break;
			case (2):
				file << "Total runtime: " << processors[i].totalCycles / 2000 << "s\tAvg Wait Time: " << processors[i].avgWait << "s\tAvg Turnaround Time: " << processors[i].avgTurn << "s" << endl;
				break;
			case (3):
				file << "Total runtime: " << processors[i].totalCycles / 2000 << "s\tAvg Wait Time: " << processors[i].avgWait << "s\tAvg Turnaround Time: " << processors[i].avgTurn << "s" << endl;
				break;
			case (4):
				file << "Total runtime: " << processors[i].totalCycles / 4000 << "s\tAvg Wait Time: " << processors[i].avgWait << "s\tAvg Turnaround Time: " << processors[i].avgTurn << "s" << endl;
				break;
		}
	}

}

int sequentialDistribution(process processes[], int size){
	processor processorList[5];
	int smallest;
	int processIndex;

	for(int i = 0; i < 5; i++){
		processorList[i].totalCycles =0;
		processorList[i].countProcesses=0;
		processorList[i].avgWait = 0;
		processorList[i].avgTurn = 0;
		processorList[i].processList = new process[size];
	}

	for(int i = 0; i<size; i++){
		//first 5 processes to come in are spread evenly among existing processes
		if(i < 5){
			processorList[i].countProcesses = 1;
			processorList[i].totalCycles = processes[i].cycles;
			processorList[i].processList[0] = processes[i];
		}



		else{
			//find the processor with the least cycles
			smallest = processorList[0].totalCycles;
			processIndex = 0;
			for(int j = 0; j < 5; j++){
				if(smallest > processorList[j].totalCycles){
					smallest = processorList[j].totalCycles;
					processIndex = j;
				}
			}	

			processorList[processIndex].processList[processorList[processIndex].countProcesses] = processes[i];
			processorList[processIndex].countProcesses++;
			processorList[processIndex].totalCycles += processes[i].cycles;
			
		}

	}

	
	//Sets up wait time and turnaroud time arrays for each set of processes on each processor. 
	for (int i = 0; i < 5; i++) {
		processorList[i].waitTime = new int[processorList[i].countProcesses];
		processorList[i].turnaroundTime = new int[processorList[i].countProcesses];
	}
	
	//Finds the total and average for wait and turnaround time. 
	for (int i = 0; i < 5; i++) {
		int totalWait = 0;
		int totalTurn = 0;
		processorList[i].waitTime[0] = 0;
		processorList[i].turnaroundTime[0] = processorList[i].processList[0].cycles / 4000;
		totalTurn += processorList[i].turnaroundTime[0];
		for (int j = 1; j < processorList[i].countProcesses; j++) {
			processorList[i].waitTime[j] = processorList[i].waitTime[j-1] + (processorList[i].processList[j - 1].cycles / 4000);
			processorList[i].turnaroundTime[j] = processorList[i].turnaroundTime[j-1] + (processorList[i].processList[j].cycles / 4000);
			totalWait += processorList[i].waitTime[j];
			totalTurn += processorList[i].turnaroundTime[j];
		}
		processorList[i].avgWait = totalWait / processorList[i].countProcesses;
		processorList[i].avgTurn = totalTurn / processorList[i].countProcesses;
	}
	
	for(int i = 0; i < 5; i++){
		file << "Processor " << i << endl;
		file << "Total runtime: " << processorList[i].totalCycles / 4000 << "s\tAvg Wait Time: " << processorList[i].avgWait << "s\tAvg Turnaround Time: " << processorList[i].avgTurn << "s" << endl;
	}

}

process* sortProcesses(process processes[], int size){
	process swap;

 	for (int i = 0 ; i < ( size - 1 ); i++)
  	{
    	for (int j = 0 ; j < size - i - 1; j++)
    	{
      		if (processes[j].cycles > processes[j+1].cycles)
      		{
        		swap       = processes[j];
        		processes[j]   = processes[j+1];
        		processes[j+1] = swap;
      		}
    	}
  	}

  	return processes;
}

void printProcesses(process processes[], int size){
	for(int i = 0; i < size; i++){
		file << "Process " << i << " has " << processes[i].cycles << " cycles, and " << processes[i].memory << " memory." << endl;
	}
}

int main() {
	int num = 100;
	int *cycles = new int[num];
	int *memory = new int[num];
	cycles = genNormalData(2500000, 1, 5000000, num);
	memory = genNormalData(1000, .25, 4000, num);
	
	process processes[num];
	
	for(int i = 0; i < num; i++){
		processes[i].id = i;
		processes[i].cycles = cycles[i];
		processes[i].memory = memory[i];
	}
	
	file.open("results.txt");
	
	file << "---------------------------------------------------------------------------" << endl;
	file << "With unlimited resources" << endl;
	sieveDistribution(processes, num);
	file << "---------------------------------------------------------------------------" << endl;
	file << "With processor memory limited" << endl;
	restrictedMemory(processes, num);
	file << "---------------------------------------------------------------------------" << endl;
	file << "With processor speed limited" << endl;
	limitedSpeed(processes,num);
	file << "---------------------------------------------------------------------------" << endl;
	file << "With processes being entered into the system sequentially" << endl;
	sequentialDistribution(processes, num);

	file.close();
	return 0;
}