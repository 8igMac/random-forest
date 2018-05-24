#include <iostream>
#include <vector>

#include "../include/decision_tree.h"
using namespace std;

int main(int argc, char** argv)
{
	irisDataSet myDataSet;

	if (argc != 2)
		cout << "usage: ./main data-set" << endl;

	myDataSet.get_data_from_file(argv[1]);




	return 0;
}
