#include "../include/decision_tree.h"

// class irisDataSet
void irisDataSet::get_data_from_file(char* fileName)
{
	ifstream ifs(fileName, ifstream::in);
	string buffer;
	size_t pos;
	stringstream ss;
	iris myiris;
	int index = 0;

	for (; !ifs.eof(); index++)
	{
		// read line into buffer
		ifs >> buffer;

		// replace ',' with ' '
		while ((pos = buffer.find(',')) != string::npos)
			buffer[pos] = ' ';

		// use stringstream to get iris data
		ss << buffer;
		ss >> myiris.sl >> myiris.sw >> myiris.pl >> myiris.pw >> myiris.cls;
		myiris.inst_num = index;
		dataSet.push_back(myiris);
		
		ss.clear();
	}

	ifs.close();
}

void irisDataSet::print_dataSet()
{
	for (auto item: dataSet)
		cout << setw(3) << item.inst_num << " " 
				 << setw(3) << item.sl << " " 
				 << setw(3) << item.sw << " " 
				 << setw(3) << item.pl << " " 
				 << setw(3) << item.pw << " "
				 << setw(3) << item.cls << endl;
}

