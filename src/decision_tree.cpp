#include "../include/decision_tree.h"

// randomly select 'n' number without replacement
// on the interval [begin, end]
vector<int> rdmSelectSet(int begin, int end, int n)
{
	int size = end-begin+1;
	// implement Kruth shuffle algorithm
	vector<int> list(size);

	// init list 
	for (int i=begin; i<=end; i++)
		list[i] = i;

	// shuffle
	for (int i=size-1; i>=0; i--)
	{
		int j = rand()%(i+1);
		swap(list[i], list[j]);
	}

	return vector<int>(list.begin(), list.begin()+n);
}

// -----------------------
// 	struct iris
// -----------------------

data_inst::data_inst(int num_attr, int num_cls)
	: attr(num_attr), num_cls(num_cls) {}

// -----------------------
// 		class dataSet
// -----------------------
void dataSet::get_data_from_file(char* fileName, int num_attr, int num_cls)
{
	ifstream ifs(fileName, ifstream::in);
	string buffer;
	string classBuffer;
	size_t pos;
	stringstream ss;
	data_inst inst(num_attr, num_cls);

	for (int index = 0 ; ifs >> buffer; index++)
	{
		// replace ',' with ' '
		while ((pos = buffer.find(',')) != string::npos)
			buffer[pos] = ' ';

		// use stringstream to get iris data
		ss << buffer;
		for (int i=0; i<num_attr; i++)
			ss >> inst.attr[i];
		ss >> classBuffer;

		// store data index
		inst.inst_num = index;

		// covert class string to enum
		if (classBuffer.compare("Iris-setosa") == 0)
			inst.cls = setosa;
		else if (classBuffer.compare("Iris-versicolor") == 0)
			inst.cls = versicolor;
		else if (classBuffer.compare("Iris-virginica") == 0)
			inst.cls = virginica;
		else
		{
			cout << "iris class name error: " << classBuffer << endl;
			break;
		}

		dataSet.push_back(inst);
		ss.clear();
	}

	ifs.close();
}

void dataSet::print_dataSet()
{
	for (auto item: dataSet)
		cout << setw(3) << item.inst_num << " " 
				 << setw(3) << item.attr[0] << " " 
				 << setw(3) << item.attr[1] << " " 
				 << setw(3) << item.attr[2] << " " 
				 << setw(3) << item.attr[3] << " "
				 << setw(3) << item.cls << endl;
}

void dataSet::split_data(vector<data_inst> &trainSet, vector<data_inst> &valiSet)
{
	vector<int> rdmPermutation(rdmSelectSet(0,dataSet.size()-1, dataSet.size()));
	for (int i=0; i<(int)dataSet.size(); i++)
	{
		if (i<TRAINSIZE)
			trainSet.push_back(dataSet.at(rdmPermutation[i]));
		else
			valiSet.push_back(dataSet.at(rdmPermutation[i]));
	}
}

// -----------------------
// 			class node 
// -----------------------

node::node()
	: leftChild(NULL), rightChild(NULL) {}

node* node::traceNextNode(data_inst valiInst)
{
	// if <= threshold -> leftChild
	// if >  threshold -> rightChild
	if (valiInst.attr[attr.first] <= attr.second)
		return leftChild;
	else
		return rightChild;
}

int node::majClass()
{
	if (samplePool.size() == 0)
		return -1;
	else
	{
		int majCls = -1;
		int max = -1;
		for (int i=0; i<(int)size_of_each_class.size(); i++)
			if (size_of_each_class[i] > max)
			{
				max = size_of_each_class[i];
				majCls = i;
			}

		return majCls;
	}
}

bool node::poolPure()
{
	for (auto size: size_of_each_class)
		if (size == (int)samplePool.size())
			return true;

	return false;
}

void node::set_size_of_class()
{
	if (samplePool.size() == 0)
	{
		cout << "error: size of samplePool is 0" << endl;
		return;
	}
	else
	{
		vector<int> clsSize(samplePool.at(0).num_cls,0);

		// calculate number of member for each class
		for (auto item: samplePool)
			clsSize[item.cls]++;

		size_of_each_class = clsSize;
	}
}

// -----------------------
// 	class decision_tree
// -----------------------

decision_tree::decision_tree()
	: rootNodePtr(NULL) {}

decision_tree::~decision_tree()
{
	destory_tree(rootNodePtr);
}

void decision_tree::build_tree(vector<data_inst> trainSet)
{
	rootNodePtr = new node;
	build_tree(rootNodePtr, trainSet, 1);
}

void decision_tree::build_tree(node* nodePtr, vector<data_inst> sampleSet, int depth)
{
	nodePtr->depth = depth;
	nodePtr->samplePool = sampleSet;
	nodePtr->numSample = sampleSet.size();
	nodePtr->set_size_of_class();

	if (depth <= MAXDEPTH && 
			nodePtr->numSample >= MINSAMPLE &&
			!(nodePtr->poolPure()))
	{
		// select attribut
		nodePtr->attr = selectAttr(sampleSet);

		// split sample into two group
		vector<data_inst> leftSample, rightSample;
		for (auto sample: sampleSet)
		{
			if (sample.attr.at(nodePtr->attr.first) <= nodePtr->attr.second)
				leftSample.push_back(sample);
			else
				rightSample.push_back(sample);
		}

		// generate next node
		nodePtr->leftChild = new node;
		build_tree(nodePtr->leftChild, leftSample, depth+1);
		nodePtr->rightChild = new node;
		build_tree(nodePtr->rightChild, rightSample, depth+1);
	}

}

float decision_tree::impurity(vector<data_inst> sampleSet)
{
	vector<int> sizeClass(sampleSet.at(0).num_cls, 0);

	for (auto sample: sampleSet)
		sizeClass[sample.cls]++;

	// sum of square of probability of each class
	float sum_square_of_p = 0;
	for (auto size: sizeClass)
		sum_square_of_p += pow(((double)size/(double)sampleSet.size()), 2.0);
	
	return 1-sum_square_of_p;
}

pair<int,float> decision_tree::selectAttr(vector<data_inst> sampleSet)
{
	float bestThreshold;
	float lowestImpurity = 2;
	int attrSelected;

	// attribute bagging
	vector<int> attrList(rdmSelectSet(0,3,ATTRBAGGING));

	// select attribut, and threshold
	for (auto attr_idx: attrList)
	{
		vector<float> valueList;
		vector<float> thresholdList;

		// get threshold list
		for (auto item: sampleSet)
			valueList.push_back(item.attr[attr_idx]);

		sort(valueList.begin(), valueList.end());

		for (int i=0; i < ((int)valueList.size()-1); i++)
			thresholdList.push_back((valueList[i]+valueList[i+1])/2);

		// get gest threshold
		for (auto threshold: thresholdList)
		{
			// split samples into two groups
			vector<data_inst> leftSample, rightSample;
			for (auto sample: sampleSet)
			{
				if (sample.attr[attr_idx] <= threshold)
					leftSample.push_back(sample);
				else
					rightSample.push_back(sample);
			}

			// calculate gini impurity of two group
			float imp, leftImpurity, rightImpurity;
			if (leftSample.size() == 0)
				leftImpurity = 0;
			else
				leftImpurity = impurity(leftSample);

			if (rightSample.size() == 0)
				rightImpurity = 0;
			else
				rightImpurity = impurity(rightSample);
			
			imp = (((float)leftSample.size() * leftImpurity) +
								 	((float)rightSample.size() * rightImpurity) ) / (float) sampleSet.size();

			// updates lowest impurity and best threshold
			if (imp < lowestImpurity)
			{
				bestThreshold = threshold;
				lowestImpurity = imp;
				attrSelected = attr_idx;
			}
		}
	}

	return pair<int, float>(attrSelected, bestThreshold);
}

int decision_tree::classify(data_inst valiInst)
{
	return classify(rootNodePtr, valiInst);
}

int decision_tree::classify(node* nodePtr, data_inst valiInst)
{
	if (nodePtr->leftChild == NULL && nodePtr->rightChild == NULL)
		return nodePtr->majClass();
	else
		return classify(nodePtr->traceNextNode(valiInst), valiInst);
}

void decision_tree::destory_tree(node* leaf)
{
	if (leaf == NULL)
		return;
	else
	{
		destory_tree(leaf->leftChild);
		destory_tree(leaf->rightChild);
		free(leaf);
	}
}

// -----------------------
// 	class random_forest
// -----------------------

random_forest::random_forest()
	: treeSet(NUMTREE) {}

void random_forest::build_forest(vector<data_inst> trainSet)
{
	for (vector<decision_tree>::iterator itr = treeSet.begin();
		 	 itr != treeSet.end(); itr++)
		itr->build_tree(treeBagging(trainSet));
}

vector<data_inst> random_forest::treeBagging(vector<data_inst> trainSet)
{
	vector<data_inst> newTrainSet;
	int sample;

	for(int i=0; i<(int)trainSet.size(); i++)
	{
		sample = rand()%(trainSet.size());
		newTrainSet.push_back(trainSet.at(sample));
	}

	return newTrainSet;
}

int random_forest::classify(data_inst valiInst)
{
	// vote
	vector<int> vote(3,0);
	for (vector<decision_tree>::iterator itr = treeSet.begin();
			 itr != treeSet.end(); itr++)
		vote.at(itr->classify(valiInst))++;

	// reach consensus among trees
	int majorVote=0;
	for (int i=1; i<(int)vote.size(); i++)
	{
		if (vote[majorVote] < vote[i])
			majorVote = i;
		else if (vote[majorVote] == vote[i])
		{
			// break tie by select random
			if (rand()%2 == 0)
				majorVote = i;
		}
	}

	return majorVote;
}


// -----------------------
// 		 class irisAnalyser
// -----------------------

// constructor
irisAnalyser::irisAnalyser()
	: true_pos({{setosa, 0}, {versicolor, 0}, {virginica, 0}}),
	  false_pos({{setosa, 0}, {versicolor, 0}, {virginica, 0}}),
	  false_neg({{setosa, 0}, {versicolor, 0}, {virginica, 0}}),
	  precision({{setosa, 0}, {versicolor, 0}, {virginica, 0}}),
	  recall({{setosa, 0}, {versicolor, 0}, {virginica, 0}}) {}

void irisAnalyser::analyse(random_forest forest, vector<data_inst> valiSet)
{
	int clsfyResult;
	for (auto item: valiSet)
	{
		clsfyResult = forest.classify(item);
		if (item.cls == clsfyResult)
		{
			// correctly classified
			true_pos[item.cls]++;
		}
		else
		{
			// target false negative++
			false_neg[item.cls]++;
			// other false positive++
			false_pos[clsfyResult]++;
		}
	}

	calculate_result();
}

void irisAnalyser::calculate_result()
{
	for (int cls=setosa; cls != Last; cls++)
	{
		if ((true_pos[cls] + false_pos[cls]) == 0)
			cout << "precision error: class " << cls << " dominator is 0" << endl;
		else
			precision[cls] = (float)true_pos[cls] / (float)(true_pos[cls] + false_pos[cls]);

		if ((true_pos[cls] + false_pos[cls]) == 0)
			cout << "recall error: class " << cls << " dominator is 0" << endl;
		else
			recall[cls] = (float)true_pos[cls] / (float)(true_pos[cls] + false_neg[cls]);
	}
}

void irisAnalyser::print_result()
{
	cout << "traning set size: " << TRAINSIZE << endl;
	cout << "validation set size: " << VALIDSIZE << endl;
	cout << "number of trees: " << NUMTREE << endl;
	cout << "attribute bagging: " << ATTRBAGGING << endl;
	cout << "decision tree max depth:  : " << MAXDEPTH << endl;
	cout << "mininum samples of a node: " << MINSAMPLE << endl;
	cout << endl; 

	cout << "setosa\n"
			 << "---------------------------\n"
			 << "precision: " << precision[setosa] << " "
			 << "recall: " << recall[setosa] << "\n" << endl;

	cout << "versicolor\n"
			 << "---------------------------\n"
			 << "precision: " << precision[versicolor] << " "
			 << "recall: " << recall[versicolor] << "\n" << endl;

	cout << "virginica\n"
			 << "---------------------------\n"
			 << "precision: " << precision[virginica] << " "
			 << "recall: " << recall[virginica] << "\n" << endl;
}
