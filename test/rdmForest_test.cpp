// test split data into training set and validation set

#include <iostream>
#include <vector>

#include "../include/decision_tree.h"
using namespace std;

int main(int argc, char** argv)
{
	formatHandler fmhdlr;
	dataSet data_set;
	vector<data_inst> TrainSet, ValiSet;
	random_forest Forest;
	vector<string> formatTb;

	if (argc != 3)
		cout << "usage: ./main data-set data-format" << endl;

	// handle data format
	formatTb = fmhdlr.handle_format(argv[2]);

	// extract data from file
	data_set.get_data_from_file(argv[1], fmhdlr.get_num_attr(), fmhdlr.get_num_cls(), formatTb);

	// split data into training subset and
	// validation subset
	data_set.split_data(TrainSet, ValiSet);

	// build forest
	Forest.build_forest(TrainSet);

	// print result
	for (auto sample: ValiSet)
		cout << "actual: " << sample.cls << "pridict: " << Forest.classify(sample) << endl;



	return 0;
}
