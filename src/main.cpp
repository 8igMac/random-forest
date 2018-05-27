#include <iostream>
#include <vector>

#include "../include/decision_tree.h"
using namespace std;

int main(int argc, char** argv)
{
	irisDataSet dataSet;
	vector<iris> irisTrainSet(TRAINSIZE), irisValiSet(VALIDSIZE);
	random_forest<iris> irisForest;
	irisAnalyser irsAlyzr;

	if (argc != 2)
		cout << "usage: ./main data-set" << endl;

	// extract data from file
	dataSet.get_data_from_file(argv[1]);

	// split data into training subset and
	// validation subset
	dataSet.split_data(irisTrainSet, irisValiSet);

	// build forest
	irisForest.build_forest(irisTrainSet);
	
	// validate and analyse accuracy
	irsAlyzr.analyse(irisForest, irisValiSet);
	irsAlyzr.print_result();
	
	return 0;
}
