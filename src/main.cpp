#include <iostream>
#include "../include/decision_tree.h"
using namespace std;

int main(int argc, char** argv)
{
	if (argc <= 1)
		cout << "usage: ./main data_set" << endl;

	cout << argv[1] << endl;
	dummy();


	return 0;
}
