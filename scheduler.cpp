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
	
	equalDistribution(processes, num);
	
	return 0;
}