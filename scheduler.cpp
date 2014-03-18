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
	
};

//For each processor, take the next smalles and the next largest process based on number of cycles from each side of the process list and put it on that processor.
int sequentialDistribution(process processes[], int size){
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

};

//For each processor, take the next smalles and the next largest process based on number of cycles from each side of the process list and put it on that processor.
int simultaneousSequentialDistribution(process processes[], int size){
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

};

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
		cout << "Processor " << processorCount << " got a process with " << processors[processorCount][elementCount].cycles << " cycles." << endl;
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

};

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
};

void printProcesses(process processes[], int size){
	for(int i = 0; i < size; i++){
		cout << "Process " << i << " has " << processes[i].cycles << " cycles, and " << processes[i].memory << " memory." << endl;
	}
};

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
	
	trevDistribution3(processes, num);
	//equalDistribution(processes, num);
	
	return 0;
}