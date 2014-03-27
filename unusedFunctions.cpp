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