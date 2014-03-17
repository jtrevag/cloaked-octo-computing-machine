#ifndef PROCESS_H
#define PROCESS_H

struct process{
		int id;
		int cycles;
		int memory;
}; 

int getAverage(int*, int);
int* genNormalData(int, int, int, int);
int equalDistribution(process*, int);
int trevDistribution(process*, int);
int trevDistribution2(process*, int);
process* sortProcesses(process*, int);
void printProcesses(process*, int);

#endif