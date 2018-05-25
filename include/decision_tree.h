#ifndef DECISION_TREE_H
#define DECISION_TREE_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <utility>
#include <unordered_map>
#include <ctime>
#include <cstdlib>

using namespace std;

#define TRAINSIZE 100
#define VALIDSIZE 50

#define NUMTREE 5

// Last is a dummy element
enum irisClass { setosa, versicolor, virginica, Last };

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
	int cls;
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
		decision_tree();
		~decision_tree();
		void build_tree(vector<iris> trainSet);
		int classify(T valiInst);

	private:
		vector<T> trainSet;
		node* root;

		void destory_tree(node* leaf);
};

template <class T>
class random_forest
{
	public:
		random_forest();
		void build_forest(vector<T> trainSet);
		int classify(T valiInst);

	private:
		vector<decision_tree<T>> treeSet;
		vector<T> trainSet;
};

class irisAnalyser
{
	public:
		irisAnalyser();
		void analyse(random_forest<iris> forest, vector<iris> valiSet);
		void print_result();
	
	private:
		// use class of iris as key
		// key: sts, vsclr, vgnc
		unordered_map<int, int> true_pos;
		unordered_map<int, int> false_pos;
		unordered_map<int, int> false_neg;

		unordered_map<int, float> precision;
		unordered_map<int, float> recall;

		void calculate_result();
};

#endif
