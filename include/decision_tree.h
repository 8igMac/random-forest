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
#define NUMTREE 5
#define ATTRBAGGING 2
#define MAXDEPTH 10
#define MINSAMPLE 1


// Last is a dummy element
enum irisClass { setosa, versicolor, virginica, Last };

// randomly select 'n' number without replacement
// on the interval [begin, end]
vector<int> rdmSelectSet(int begin, int end, int n);

struct iris
{
	iris();

	// instance number
	int inst_num;
	// attributes
	vector<float> attr;
	// class
	int cls;
	// number of cls
	int num_cls;
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


template <class T>
class node
{
	public:
		node();
		// return next node for given validation instance
		node<T>* traceNextNode(T valiInst);
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
		vector<T> samplePool;
		// if <= threshold -> leftChild
		// if >  threshold -> rightChild
		node<T> *leftChild, *rightChild;
};

template <class T>
class decision_tree
{
	public:
		decision_tree();
		~decision_tree();
		void build_tree(vector<T> trainSet);
		int classify(T valiInst);

	private:
		int classify(node<T>* nodePtr, T valiInst);
		void destory_tree(node<T>* leaf);
		void build_tree(node<T>* nodePtr, vector<T> sampleSet, int depth);
		pair<int,float> selectAttr(vector<T> sampleSet);
		float impurity(vector<T> sampleSet);

		vector<T> trainSet;
		node<T>* rootNodePtr;
};

template <class T>
class random_forest
{
	public:
		random_forest();
		void build_forest(vector<T> trainSet);
		int classify(T valiInst);

	private:
		vector<T> treeBagging(vector<T> trainSet);
		vector<decision_tree<T>> treeSet;
};

class irisAnalyser
{
	public:
		irisAnalyser();
		void analyse(random_forest<iris> forest, vector<iris> valiSet);
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
