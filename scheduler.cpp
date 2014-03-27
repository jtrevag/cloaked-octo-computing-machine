#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "process.h"
using namespace std;

int equalDistribution (process processes[], int size) {
	int target, total = 0;
	
	for (int i = 0; i < size; i++)
	{
		total += processes[i].cycles;
	}
	target = floor(total/5); //This is the target cycle number for each processor. 
	
	int count = 0;
	for (int i =0; i < 5; i++)
	{
		int limit = 0;
		while (limit <= target && count < 100)
		{
			limit += processes[count].cycles;
			count++;
		}
		cout << "Processor: " << i << " has " << limit << " cycles." << endl;
	}
	cout << count << endl;
}

//For each processor, take the next smalles and the next largest process based on number of cycles from each side of the process list and put it on that processor.
int listDistribution(process processes[], int size){
	process* sortedProcesses = new process[size];
	sortedProcesses = sortProcesses(processes, size); 

	process processors[5][size];
	
	int processorCount = 0;
	int elementCount = 0;
	for(int i = 0; i<size; i++){
		processors[processorCount][elementCount] = sortedProcesses[i];
		cout << "Processor " << processorCount << " got a process with " << processors[processorCount][elementCount].cycles << " cycles." << endl;
		if(processorCount < 4){
			processorCount++;
		} else{
			processorCount = 0;
			elementCount++;
		}
	}

	cout << elementCount << endl;

	for(int i = 0; i < 5; i++){
		int total = 0;
		for(int j = 0; j < elementCount; j++){
			total += processors[i][j].cycles;
		}

		cout << "The total for processor " << i << " is: " << total << endl;
		total = 0;
	}
}

//For each processor, take the next smalles and the next largest process based on number of cycles from each side of the process list and put it on that processor.
int pincerListDistribution(process processes[], int size){
	process* sortedProcesses = new process[size];
	sortedProcesses = sortProcesses(processes, size); 

	process processors[5][size];
	
	int processorCount = 0;
	int elementCount = 0;
	for(int i = 0; i<size/2; i++){
		processors[processorCount][elementCount] = sortedProcesses[i];
		processors[processorCount][elementCount+1] = sortedProcesses[size-i];
		if(processorCount < 4){
			processorCount++;
		} else{
			processorCount = 0;
			elementCount += 2;
		}
	}

	cout << elementCount << endl;

	for(int i = 0; i < 5; i++){
		int total = 0;
		for(int j = 0; j < elementCount; j++){
			total += processors[i][j].cycles;
		}

		cout << "The total for processor " << i << " is: " << total << endl;
		total = 0;
	}

}

//For each processor, take the next smalles and the next largest process based on number of cycles from each side of the process list and put it on that processor.
int sieveDistribution(process processes[], int size){
	process* sortedProcesses = new process[size];
	sortedProcesses = sortProcesses(processes, size); 

	process processors[5][size];
	
	int processorCount = 0;
	int elementCount = 0;
	int forward = 1;
	for(int i = 0; i<size; i++){
		processors[processorCount][elementCount] = sortedProcesses[i];
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

	cout << elementCount << endl;

	for(int i = 0; i < 5; i++){
		int total = 0;
		for(int j = 0; j < elementCount; j++){
			total += processors[i][j].cycles;
		}

		cout << "The total for processor " << i << " is: " << total << endl;
		total = 0;
	}

}

//Very poor implementation. Don't do this. Ever. 
int limitedMemory (process processes[], int size) {
	process* sortedProcesses = new process[size];
	sortedProcesses = sortProcesses(processes, size); 

	process processors[5][size];
	int elementCount[5] = { 0 };
	
	int processorCount = 0;
	for (int i = 0; i < size; i++) {
		if (processorCount > 4)
			processorCount = 0;
		switch (processorCount) {
			case (0): 
				if (sortedProcesses[i].memory > 1000) { //If the process won't fit in this bucket, skip to the next bucket. 
					i--;
					processorCount++;
					continue;
				}
				else
					processors[processorCount][elementCount[processorCount]] = sortedProcesses[i];
				break;
			case (1):
				if (sortedProcesses[i].memory > 1000) {
					i--;
					processorCount++;
					continue;
				}
				else
					processors[processorCount][elementCount[processorCount]] = sortedProcesses[i];
				break;
			case (2): 
				if (sortedProcesses[i].memory > 2000) {
					i--;
					processorCount++;
					continue;
				}
				else
					processors[processorCount][elementCount[processorCount]] = sortedProcesses[i];
				break;
			case (3):
				if (sortedProcesses[i].memory > 2000) {
					i--;
					processorCount++;
					continue;
				}
				else
					processors[processorCount][elementCount[processorCount]] = sortedProcesses[i];
				break;
			case (4):
				processors[processorCount][elementCount[processorCount]] = sortedProcesses[i];
				break;
		}
		elementCount[processorCount]++;
		processorCount++;
	}
	
	for (int i = 0; i < 5; i++) {
		int total = 0;
		for(int j = 0; j < elementCount[i]; j++){
			total += processors[i][j].cycles;
		}

		cout << "The total for processor " << i << " is: " << total << endl;
		total = 0;
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
	
	processor processorList[5];
	for(int i = 0; i < 5; i++){
		processorList[i].totalCycles = 0;
		processorList[i].countProcesses=0;
		processorList[i].adjustedCycles=0;
		processorList[i].avgWait = 0;
		processorList[i].avgTurn = 0;
		processorList[i].processList = new process[size];
	}
	
	cout << "The number of cycles that MUST be in processor 4 is: " << highMemCycles << endl;
	for (int i = 0; i < 5; i++) {
		int total = 0;
		for(int j = 0; j < elementCount[i]; j++){
			total += processors[i][j].cycles;
		}

		cout << "The total for processor " << i << " is: " << total << endl;
		total = 0;
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
		processors[i].turnaroundTime[0] = processors[i].processList[0].cycles / 1000;
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
			// processors[i].waitTime[j] = processors[i].waitTime[j-1] + (processors[i].processList[j - 1].cycles / 4000);
			// processors[i].turnaroundTime[j] = processors[i].turnaroundTime[j-1] + (processors[i].processList[j].cycles / 4000);
			totalWait += processors[i].waitTime[j];
			totalTurn += processors[i].turnaroundTime[j];
		}
		processors[i].avgWait = totalWait / processors[i].countProcesses;
		processors[i].avgTurn = totalTurn / processors[i].countProcesses;
	}
	
	for(int i = 0; i < 5; i++){
		cout << "Processor " << i << " has a total of " << processors[i].countProcesses << " processes, with " << processors[i].totalCycles << " total cycles and " << processors[i].adjustedCycles << " adjusted cycles." << endl;
		cout << "with an average waiting time of: " << processors[i].avgWait << " and average turnaround time: " << processors[i].avgTurn << endl;
	}

}

int sequentialDistribution(process processes[], int size){
	process processors[5][size];
	processor totalCycles[5];
	int smallest;
	int processIndex;

	for(int i = 0; i<size; i++){
		//first 5 processes to come in are spread evenly among existing processes
		if(i < 5){
			processors[i][0] = processes[i];
			totalCycles[i].countProcesses = 1;
			totalCycles[i].totalCycles = processes[i].cycles;
		}

		else{
			//find the processor with the least cycles
			smallest = totalCycles[0].totalCycles;
			processIndex = 0;
			for(int j = 0; j < 5; j++){
				if(smallest > totalCycles[j].totalCycles){
					smallest = totalCycles[j].totalCycles;
					processIndex = j;
				}
			}	

			processors[processIndex][totalCycles[processIndex].countProcesses] = processes[i];
			totalCycles[processIndex].countProcesses++;
			totalCycles[processIndex].totalCycles += processes[i].cycles;

		}

	}

	for(int i = 0; i < 5; i++){
		cout << "Processor " << i << " has a total of " << totalCycles[i].countProcesses << " processes, with " << totalCycles[i].totalCycles << " total cycles." << endl;
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
		cout << "Process " << i << " has " << processes[i].cycles << " cycles, and " << processes[i].memory << " memory." << endl;
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
	
	// cout << "With unlimited resources" << endl;
	// sieveDistribution(processes, num);
	// cout << "With processor memory limited" << endl;
	// restrictedMemory(processes, num);
	cout << "With processor speed limited" << endl;
	limitedSpeed(processes,num);
	// cout << "With processes being entered into the system sequentially" << endl;
	// sequentialDistribution(processes, num);

	
	return 0;
}