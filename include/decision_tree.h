#ifndef DECISION_TREE_H
#define DECISION_TREE_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;

#define TRAINSIZE 100
#define VALIDSIZE 50


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
		void split_data(vector<iris> &trainSet, vector<iris> &valiSet);

	private:
		vector<iris> dataSet;
};


class node
{

};

template <class T>
class decision_tree
{
	public:
		decision_tree(vector<iris> trainSet);
		~decision_tree();
		void build_tree();

	private:
		vector<T> trainSet;
		node* root;

		void destory_tree(node* leaf);
};

template <class T>
class random_forest
{
	public:
		void build_forest(vector<T> trainSet);

	private:
		vector<decision_tree<T>> treeSet;
		vector<T> trainSet;
};

template <class T>
class analyser
{
	public:
		void analyse(random_forest<T> forest, vector<T> valiSet);
		void print_result();
};

#endif
