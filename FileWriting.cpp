#include <iostream>
#include <fstream>
#include <cmath>
#define maxNumErrors 10
using namespace std;


struct Student{
	int studentID;
	int grade;
};

struct Dataset{
	int numStudents;
	Student* students;
};

struct Rejects{
	int numRejects;
	int* rejects;
};

struct Mode{
	int numModes;
	int* modes;
};

struct Statistics{
	int   minimum;
	float average;
	int   maximum;
	float popStdDev;
	float smplStdDev;
	Mode  mode;
	int   histogram[10];
};

bool sort(int data2[], const int size);

int readCSV(const char filename[],const int minAcceptableID, 
				const int maxAcceptableID,Dataset& data, 
				Rejects& rejects){

	if (minAcceptableID > maxAcceptableID)
		return -1;
	
	char filename2[50];
	
	for (int i = 0; i <= 50; i++){
		filename2[i] = filename[i];
	}
	
	bool ext = false;
	
	bool end = false;
	
	int counter = 0;
	
	while (!end && !ext && counter <= 50){
		
		if (filename2[counter] == '.')
			ext = true;
				
		else if (filename2[counter] == 0)
			end = true;
		
		else
			counter++;
	}
	
	if (end)
		filename2[counter] = '.';
	
	if (end || (ext && (filename2[counter+1] != 'c' || filename2[counter+2] != 's' || filename2[counter+3] != 'v'))){
		
		filename2[counter+1] = 'c';
		
		filename2[counter+2] = 's';
		
		filename2[counter+3] = 'v';
		
		filename2[counter+4] = 0;
	}	
	
	const int maxLineLength = 100;
	
	char line[maxLineLength];
	
	ifstream infile;
	
	infile.open(filename2);
	
	if (!infile.is_open())
		return -1;
		
	bool done = false;
	
	int filelinenumber = 1;
	
	rejects.numRejects = 0;
	
	rejects.rejects = new int[maxNumErrors];
	
	for (int i = 0; i < maxNumErrors; i++){
		rejects.rejects[i] = 0;
	}
	
	data.numStudents = 0;
	
	data.students = new Student[100];
	
	for (int i = 0; i < 100; i++){
		
		data.students[i].studentID = 0;
		
		data.students[i].grade = 0;
	}	
	
	while (!done){
	
		if (!infile.getline(line, maxLineLength)){
			
			if (infile.eof())
				done = true;
			
			else
				return -1;
		}
		
		else{
			
			bool badline = false;
			
			bool comma = false;
			
			int i = 0;
		
			while (line[i] == ' ' || line[i] == '\t' || line[i] == 13){
				i++;
			}
		
			if (!(line[i] >= '0' && line[i] <= '9'))
				badline = true;
			
			int studentid = 0;
		
			while (line[i] >= '0' && line[i] <= '9'){
			
				studentid = (studentid * 10) + line[i] - '0';
				
				i++;
			}
		
			while (line[i] == ' ' || line[i] == '\t' || line[i] == 13){
				i++;
			}
		
			if (line[i] != ',')
				badline = true;
			
			else
				i++;
				
			while (line[i] == ' ' || line[i] == '\t' || line[i] == 13){
				i++;
			}
			
			int mark = 0;
		
			if (!(line[i] >= '0' && line[i] <= '9'))
				badline = true;
			
			while (line[i] >= '0' && line[i] <= '9'){
			
				mark = (mark * 10) + line[i] - '0';
				
				i++;
			}
			
			while (line[i] == ' ' || line[i] == '\t' || line[i] == 13){
				i++;
			}
			
			if (line[i] != '\n' && line[i] != 0)
				badline = true;
			
			if (!(studentid >= minAcceptableID && studentid <= maxAcceptableID))
				badline = true;
			
			if (!(mark >= 0 && mark <= 100))
				badline = true;

			if (!badline){
					
				data.students[data.numStudents].studentID = studentid;
					
				data.students[data.numStudents].grade = mark;
					
				data.numStudents++;
			}	
				
			else{
				
				rejects.rejects[rejects.numRejects] = filelinenumber;
			
				rejects.numRejects++;
				
				if (rejects.numRejects >= maxNumErrors)
					return -1;
			}
		
			filelinenumber++;
		}
	}
	
	infile.close();
	
	if (rejects.numRejects != 0)
		return rejects.numRejects;
	
	return 0;
}

int computeStatistics(Dataset& data, Statistics& stats){
	
	if (data.numStudents == 0)
		return -1;
	
	stats.minimum = data.students[0].grade;
	
	for (int i = 1; i < data.numStudents; i++){
		
		if (data.students[i].grade < stats.minimum)
			stats.minimum = data.students[i].grade;
	}
	
	stats.maximum = data.students[0].grade;
	
	for (int i = 1; i < data.numStudents; i++){
		
		if (data.students[i].grade > stats.maximum)
			stats.maximum = data.students[i].grade;
	}
	
	float sum = 0;
	
	for (int i = 0; i < data.numStudents; i++){
		
		sum += data.students[i].grade;
	}
	
	stats.average = sum / data.numStudents;
	
	float sum2 = 0;
	
	for (int i = 0; i < data.numStudents; i++){
		
		sum2 += (data.students[i].grade - stats.average) * (data.students[i].grade - stats.average);
	}
	
	stats.popStdDev = sqrt(sum2 / data.numStudents);
	
	if (data.numStudents == 1)
		stats.smplStdDev = 0;
	
	else
		stats.smplStdDev = sqrt(sum2 / (data.numStudents - 1));
	
	for (int i = 0; i < 10; i++){
			
		stats.histogram[i] = 0;
	}
	
	for (int i = 0; i < data.numStudents; i++){
		
		if (data.students[i].grade == 100)
			stats.histogram[9]++;
		
		else
			stats.histogram[data.students[i].grade / 10]++;
	}
	
	int data2[data.numStudents];
	
	for (int i = 0; i < data.numStudents; i++){
		
		data2[i] = data.students[i].grade;
	}
	
	sort(data2, data.numStudents);
	
	int newnums[data.numStudents];
	
	int newnumstally[data.numStudents];
	
	for (int i = 0; i < data.numStudents; i++){
		
		newnums[i] = 0;
	
		newnumstally[i] = 0;
	}
	
	newnums[0] = data2[0];
	
	newnumstally[0] = 1;
	
	int newnumsindex = 0;
	
	for (int i = 1; i < data.numStudents; i++){
		
		if (data2[i] != data2[i-1]){
				
			newnumsindex++;
			
			newnums[newnumsindex] = data2[i];
			
			newnumstally[newnumsindex] = 1;
		}
	
		else
			newnumstally[newnumsindex]++;
	}
	
	int maxnumstally = newnumstally[0];
	
	for (int i = 1; i <= newnumsindex; i++){
		
		if (newnumstally[i] > maxnumstally)
			maxnumstally = newnumstally[i];
	}
	
	stats.mode.numModes = 0;
	
	stats.mode.modes = new int[newnumsindex + 1];
	
	for (int i = 0; i <= newnumsindex; i++){
		
		stats.mode.modes[i] = 0;
	}
	
	for (int i = 0; i <= newnumsindex; i++){
		
		if (newnumstally[i] == maxnumstally){
			
			stats.mode.modes[stats.mode.numModes] = newnums[i];
			
			stats.mode.numModes++;
		}
	}
	
	return 0;
}

int writeCSV(const char filename[], const Statistics& stats){
	
	char filename2[50];
	
	for (int i = 0; i <= 50; i++){
		filename2[i] = filename[i];
	}

	bool end = false;
	
	bool ext = false;
	
	int counter = 0;
	
	while (!end && !ext && counter <= 50){
		
		if (filename2[counter] == '.')
			ext = true;
				
		else if (filename2[counter] == 0)
			end = true;
		
		else
			counter++;
	}
	
	if (end)
		filename2[counter] = '.';
	
	if (end || (ext && (filename2[counter+1] != 's' || filename2[counter+2] != 't' || filename2[counter+3] != 'a' || filename2[counter+4] != 't'))){
		
		filename2[counter+1] = 's';
		
		filename2[counter+2] = 't';
		
		filename2[counter+3] = 'a';
		
		filename2[counter+4] = 't';	
		
		filename2[counter+5] = 0;
	}
	
	ofstream outfile;
	
	outfile.open(filename2);
	
	if (!outfile.is_open())
		return -1;

	outfile << "Minimum: " << stats.minimum << endl;
	
	outfile << "Average: " << stats.average << endl;
	
	outfile << "Maximum: " << stats.maximum << endl;

	outfile << "Population Standard Deviation: " << stats.popStdDev << endl;
	
	outfile << "Sample Standard Deviation: " << stats.smplStdDev << endl;
	
	for (int i = 0; i < stats.mode.numModes; i++){
		
		if (i == 0)
			outfile << "Modes: " << stats.mode.modes[i];
		
		else
			outfile << ", " << stats.mode.modes[i];
	}
	
	outfile << endl << "Histogram:" << endl;
	
	outfile << "[0-9]: " << stats.histogram[0] << endl;
	
	outfile << "[10-19]: " << stats.histogram[1] << endl;
	
	outfile << "[20-29]: " << stats.histogram[2] << endl;
	
	outfile << "[30-39]: " << stats.histogram[3] << endl;
	
	outfile << "[40-49]: " << stats.histogram[4] << endl;
	
	outfile << "[50-59]: " << stats.histogram[5] << endl;
	
	outfile << "[60-69]: " << stats.histogram[6] << endl;
	
	outfile << "[70-79]: " << stats.histogram[7] << endl;
	
	outfile << "[80-89]: " << stats.histogram[8] << endl;
	
	outfile << "[90-100]: " << stats.histogram[9] << endl;
	
	outfile.close();
	
	return 0;
}

#define isNaN(X) (X != X)

int main(const int argc, const char* const argv[]) {
  	
	// Some test driver code here ....
	int minAcceptableID = 0;
	int maxAcceptableID = 2000000;

	Dataset data = {0};
	Rejects rejects = {0};
	Statistics stats = {0};

	std::cout << std::endl << "Implement some more appropriate tests in main()" << std::endl << std::endl;

	if(readCSV(argv[1], minAcceptableID, maxAcceptableID, data, rejects) < 0)
	{
		std::cout << ".csv file could not be read" << std::endl;
	}

	if (computeStatistics(data, stats) < 0)
	{
		std::cout << "Stats could not be computed" << std::endl;
	}

	if (writeCSV(argv[1], stats) < 0)
	{
		std::cout << ".stat file could not be written" << std::endl;
	}

	std::cout << "Average: " << stats.average << std::endl;
	std::cout << "Minimum: " << stats.minimum << std::endl;
	std::cout << "Maximum: " << stats.maximum << std::endl;
	std::cout << "Population standard deviation: " << stats.popStdDev << std::endl;
	std::cout << "Sample standard deviation: " << stats.smplStdDev << std::endl;
	
	
	for (unsigned char i = 0; i < stats.mode.numModes; i++)
	{
		std::cout << "Mode: " << stats.mode.modes[i] << std::endl;
	}

	for (unsigned char i = 0; i < 10; i++)
	{
		std::cout << "Histogram bin [" << (i*10) << ", " << ((((i+1)*10) - 1) + i/9) << "]: " << stats.histogram[i] << std::endl;
	}
	

	return 0;
}

bool sort(int data2[], const int size){
	
	if (size == 1)
		return true;
	
	int maxindex = 0;
	
	for (int i = 1; i < size; i++){
		
		if (data2[i] > data2[maxindex])
			maxindex = i;
	}
	
	int temp = data2[size-1];
	
	data2[size-1] = data2[maxindex];

	data2[maxindex] = temp;
	
	sort(data2, (size - 1));
}
