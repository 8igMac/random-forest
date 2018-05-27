#include <iostream>
#include <vector>

#include "../include/decision_tree.h"
using namespace std;

int main(int argc, char** argv)
{
	int num_attr = 4;
	int num_cls = 3;
	dataSet data_set;
	vector<data_inst> irisTrainSet, irisValiSet;
	random_forest irisForest;
	irisAnalyser irsAlyzr;

	if (argc != 2)
		cout << "usage: ./main data-set" << endl;

	// extract data from file
	data_set.get_data_from_file(argv[1], num_attr, num_cls);

	// split data into training subset and
	// validation subset
	data_set.split_data(irisTrainSet, irisValiSet);

	// build forest
	irisForest.build_forest(irisTrainSet);
	
	// validate and analyse accuracy
	irsAlyzr.analyse(irisForest, irisValiSet);
	irsAlyzr.print_result();
	
	return 0;
}
