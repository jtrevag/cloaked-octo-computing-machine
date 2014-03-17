#ifndef PROCESS_H
#define PROCESS_H

struct process{
		int id;
		int cycles;
		int memory;
}; 

struct processor{
	int countProcesses;
	int totalCycles;
};

int getAverage(int*, int);
int* genNormalData(int, int, int, int);
int equalDistribution(process*, int);
int listDistribution(process*, int);
int pincerListDistribution(process*, int);
int sieveDistribution(process*, int);
int limitedMemory(process*, int);
int restrictedMemory(process*, int);
int sequentialDistribution(process*, int);

process* sortProcesses(process*, int);
void printProcesses(process*, int);

#endif