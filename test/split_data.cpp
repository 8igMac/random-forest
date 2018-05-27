// test split data into training set and validation set

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

	if (argc != 2)
		cout << "usage: ./main data-set" << endl;

	// extract data from file
	data_set.get_data_from_file(argv[1], num_attr, num_cls);

	// split data into training subset and
	// validation subset
	data_set.split_data(irisTrainSet, irisValiSet);

	cout << "------------ training set --------------" << endl;
	for (auto item: irisTrainSet)
		cout << setw(3) << item.inst_num << " " 
				 << setw(3) << item.attr[0] << " " 
				 << setw(3) << item.attr[1] << " " 
				 << setw(3) << item.attr[2] << " " 
				 << setw(3) << item.attr[3] << " "
				 << setw(3) << item.cls << endl;

	cout << "------------ validation set --------------" << endl;
	for (auto item: irisValiSet)
		cout << setw(3) << item.inst_num << " " 
				 << setw(3) << item.attr[0] << " " 
				 << setw(3) << item.attr[1] << " " 
				 << setw(3) << item.attr[2] << " " 
				 << setw(3) << item.attr[3] << " "
				 << setw(3) << item.cls << endl;

	return 0;
}
