// test get data from file

#include <iostream>
#include <vector>

#include "../include/decision_tree.h"
using namespace std;

int main(int argc, char** argv)
{
	irisDataSet dataSet;

	if (argc != 2)
		cout << "usage: ./main data-set" << endl;

	// extract data from file
	dataSet.get_data_from_file(argv[1]);
	dataSet.print_dataSet();

	return 0;
}
