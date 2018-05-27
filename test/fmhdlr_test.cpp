#include <iostream>
#include <vector>

#include "../include/decision_tree.h"
using namespace std;

int main(int argc, char** argv)
{
	formatHandler fmhdlr;
	vector<string> formatTb;

	if (argc != 3)
		cout << "usage: ./main data-set data-format" << endl;

	// handle data format
	formatTb = fmhdlr.handle_format(argv[2]);

	// print result
	cout << "number of attributes: " << fmhdlr.get_num_attr() << endl;
	cout << "number of classes: " << fmhdlr.get_num_cls() << endl;
	cout << "number of instances: " << fmhdlr.get_num_inst() << endl; 
	
	cout << "format table: \n";
	for (auto item: formatTb)
		cout << item << endl;


	return 0;
}
