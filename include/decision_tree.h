#ifndef DECISION_TREE_H
#define DECISION_TREE_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;

struct iris
{
	// instance number
	int inst_num;
	// septal length in cm
	float sl;
	// septal width in cm
	float sw;
	// petal lenght in cm
	float pl;
	// petal width in cm
	float pw;
	// class
	string cls;
};

class irisDataSet
{
	public:	
		// get iris data from file
		void get_data_from_file(char* fileName);
		void print_dataSet();

	private:
		vector<iris> dataSet;
};


class node
{

};

class decision_tree
{

};

class random_forest
{

};

#endif
