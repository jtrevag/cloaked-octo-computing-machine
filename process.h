#ifndef PROCESS_H
#define PROCESS_H

struct process{
		int id;
		int cycles;
		int memory;
}; 

int getAverage(int*, int);
int* genNormalData(int, int, int, int);
int equalDistribution(process*);

#endif PROCESS_H
