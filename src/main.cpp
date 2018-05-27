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

	cout << "get data from file ... ";
	// extract data from file
	data_set.get_data_from_file(argv[1], fmhdlr.get_num_attr(), fmhdlr.get_num_cls(), formatTb);
	cout << "done" << endl;

	cout << "split data ... ";
	// split data into training subset and
	// validation subset
	data_set.split_data(TrainSet, ValiSet);
	cout << "done" << endl;

	cout << "building forest ... " << endl;
	// build forest
	Forest.build_forest(TrainSet);
	cout << "done" << endl;
	
	// validate and analyse accuracy
	analyser alyzr(formatTb);
	alyzr.analyse(Forest, ValiSet);
	alyzr.print_result(fmhdlr.get_num_inst());
	
	return 0;
}
