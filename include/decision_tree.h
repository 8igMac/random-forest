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
#include <algorithm>
#include <cmath>

using namespace std;

// parameter for tuing
#define TRAINSIZE 100
#define VALIDSIZE 50
#define NUMTREE 8
#define ATTRBAGGING 2
#define MAXDEPTH 10
#define MINSAMPLE 1


// Last is a dummy element
enum irisClass { setosa, versicolor, virginica, Last };

// randomly select 'n' number without replacement
// on the interval [begin, end]
vector<int> rdmSelectSet(int begin, int end, int n);

struct data_inst
{
	data_inst(int num_attr, int num_cls);

	// instance number
	int inst_num;
	// attributes
	vector<float> attr;
	// class
	int cls;
	// number of cls
	int num_cls;
};

class dataSet
{
	public:	
		// get iris data from file
		void get_data_from_file(char* fileName, int num_attr, int num_cls);
		void print_dataSet();
		void split_data(vector<data_inst> &trainSet, vector<data_inst> &valiSet);

	private:
		vector<data_inst> dataSet;
};


class node
{
	public:
		node();
		// return next node for given validation instance
		node* traceNextNode(data_inst valiInst);
		// major class of this node
		int majClass();
		bool poolPure();
		// get quanty of each class in sample pool
		void set_size_of_class();


		// store quanty of each class in sample pool
		vector<int> size_of_each_class;
		// pair<attr_index, threshold
		pair<int, float> attr;
		// depth of the node, number of sample
		int depth, numSample;
		// all the samples
		vector<data_inst> samplePool;
		// if <= threshold -> leftChild
		// if >  threshold -> rightChild
		node *leftChild, *rightChild;
};

class decision_tree
{
	public:
		decision_tree();
		~decision_tree();
		void build_tree(vector<data_inst> trainSet);
		int classify(data_inst valiInst);

	private:
		int classify(node* nodePtr, data_inst valiInst);
		void destory_tree(node* leaf);
		void build_tree(node* nodePtr, vector<data_inst> sampleSet, int depth);
		pair<int,float> selectAttr(vector<data_inst> sampleSet);
		float impurity(vector<data_inst> sampleSet);

		vector<data_inst> trainSet;
		node* rootNodePtr;
};

class random_forest
{
	public:
		random_forest();
		void build_forest(vector<data_inst> trainSet);
		int classify(data_inst valiInst);

	private:
		vector<data_inst> treeBagging(vector<data_inst> trainSet);
		vector<decision_tree> treeSet;
};

class irisAnalyser
{
	public:
		irisAnalyser();
		void analyse(random_forest forest, vector<data_inst> valiSet);
		void print_result();
	
	private:
		void calculate_result();

		// use class of iris as key
		// see irisClass
		unordered_map<int, int> true_pos;
		unordered_map<int, int> false_pos;
		unordered_map<int, int> false_neg;

		unordered_map<int, float> precision;
		unordered_map<int, float> recall;
};

#endif
