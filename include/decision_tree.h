#ifndef DECISION_TREE_H
#define DECISION_TREE_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <utility>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <cmath>

using namespace std;

// parameter for tuning
// trainpercet: n = n%*all_sample
#define TRAINPERCENT 30
#define NUMTREE 100
// adjust here !!!!!
#define ATTRBAGGING 1
#define MAXDEPTH 12
#define MINSAMPLE 5

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
		// get data from file
		void get_data_from_file(char* fileName, int num_attr, int num_cls, vector<string> formatTb);
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
		int set_size_of_class();


		// parent major class
		int parentMajCls;
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
		void build_tree(node* nodePtr, vector<data_inst> sampleSet, int depth, int parentMaj);
		pair<int,float> selectAttr(vector<data_inst> sampleSet);
		float impurity(vector<data_inst> sampleSet);

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

class analyser
{
	public:
		analyser(vector<string> format);
		void analyse(random_forest &forest, vector<data_inst> valiSet);
		void print_result(int num_inst);
		float get_precision(int cls);
		float get_recall(int cls);
	
	private:
		// confusion matrix
		vector<vector<float>> confusionMtrx;
		vector<string> formatTb;
};

class formatHandler
{
	public:
		vector<string> handle_format(char* fileName);
		int get_num_attr();
		int get_num_cls();
		int get_num_inst();
	private:
		int num_attr, num_cls, num_inst;
};

#endif
