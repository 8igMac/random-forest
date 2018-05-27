// test split data into training set and validation set

#include <iostream>
#include <vector>

#include "../include/decision_tree.cpp"
using namespace std;

int main(int argc, char** argv)
{
	irisDataSet dataSet;
	vector<iris> irisTrainSet(TRAINSIZE), irisValiSet(VALIDSIZE);
	decision_tree<iris> tree;
	random_forest<iris> forest;

	if (argc != 2)
		cout << "usage: ./main data-set" << endl;

	// extract data from file
	dataSet.get_data_from_file(argv[1]);

	// split data into training subset and
	// validation subset
	dataSet.split_data(irisTrainSet, irisValiSet);

	forest.build_forest(irisTrainSet);

	for (auto sample: irisValiSet)
		cout << "actual: " << sample.cls << "pridict: " << forest.classify(sample) << endl;


	return 0;
}
