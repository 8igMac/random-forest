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
// 	struct data_inst
// -----------------------

data_inst::data_inst(int num_attr, int num_cls)
	: attr(num_attr), num_cls(num_cls) {}

// -----------------------
// 		class dataSet
// -----------------------
void dataSet::get_data_from_file(char* fileName, int num_attr, int num_cls, vector<string> formatTb)
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

		// use stringstream to get data
		ss << buffer;
		for (int i=0; i<num_attr; i++)
			ss >> inst.attr[i];
		ss >> classBuffer;

		// store data index
		inst.inst_num = index;

		// covert class string to enum
		for (int i=0; i<(int)formatTb.size(); i++)
			if (classBuffer.compare(formatTb.at(i)) == 0)
			{
				inst.cls = i;
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
	int trainSize = TRAINPERCENT * (dataSet.size()/100);
	for (int i=0; i<(int)dataSet.size(); i++)
	{
		if (i<trainSize)
			trainSet.push_back(dataSet.at(rdmPermutation[i]));
		else
			valiSet.push_back(dataSet.at(rdmPermutation[i]));
	}
}

// -----------------------
// 			class node 
// -----------------------

node::node()
	: parentMajCls(0), leftChild(NULL), rightChild(NULL) {}

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
		return parentMajCls;
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

int node::set_size_of_class()
{
	if (samplePool.size() == 0)
	{
		cout << "set size error: size of samplePool is 0" << endl;
		return -1;
	}
	else
	{
		vector<int> clsSize(samplePool.at(0).num_cls,0);

		// calculate number of member for each class
		for (auto item: samplePool)
			clsSize[item.cls]++;

		size_of_each_class = clsSize;
		return 0;
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
	build_tree(rootNodePtr, trainSet, 1, rootNodePtr->majClass());
}

void decision_tree::build_tree(node* nodePtr, vector<data_inst> sampleSet, int depth, int parentMaj)
{
//	cout << "depth: " << depth << " sample size: " << sampleSet.size() << endl; //debug
	nodePtr->depth = depth;
	nodePtr->samplePool = sampleSet;
	nodePtr->numSample = sampleSet.size();
	nodePtr->parentMajCls = parentMaj;
	if (nodePtr->set_size_of_class() < 0)
		return;

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
		build_tree(nodePtr->leftChild, leftSample, depth+1, nodePtr->majClass());
		nodePtr->rightChild = new node;
		build_tree(nodePtr->rightChild, rightSample, depth+1, nodePtr->majClass());
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
	int num_attr = sampleSet.at(0).attr.size();
	vector<int> attrList(rdmSelectSet(0,num_attr-1,ATTRBAGGING));

	
	// select attribut, and threshold
	for (auto attr_idx: attrList)
	{
		vector<float> valueList;
		vector<float> prunedValuList;
		vector<float> thresholdList;

		// get value list
		for (auto item: sampleSet)
			valueList.push_back(item.attr[attr_idx]);

		// sort value list
		sort(valueList.begin(), valueList.end());

		// prun value list
		for (int i=0; i<(int)valueList.size(); i++)
		{
			if (i==0)
				prunedValuList.push_back(valueList[i]);
			else
			{
				if (valueList[i] != valueList[i-1])
					prunedValuList.push_back(valueList[i]);
			}
		}

		// get threshold list
		if (prunedValuList.size() == 1)
			thresholdList.push_back(prunedValuList[0]);
		else
		{
			for (int i=0; i < ((int)prunedValuList.size()-1); i++)
				thresholdList.push_back((prunedValuList[i]+prunedValuList[i+1])/2);
		}

		// get gest threshold
		for (auto threshold: thresholdList)
		{
			// split samples into two groups
			vector<data_inst> leftSample, rightSample;
			for (auto sample: sampleSet)
			{
				if (sample.attr.at(attr_idx) <= threshold)
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

	//here
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
	if (leaf != NULL)
	{
		destory_tree(leaf->leftChild);
		destory_tree(leaf->rightChild);
		delete leaf;
	}
}

// -----------------------
// 	class random_forest
// -----------------------

random_forest::random_forest()
	: treeSet(NUMTREE) {}

void random_forest::build_forest(vector<data_inst> trainSet)
{
	int idx = 1;
	for (vector<decision_tree>::iterator itr = treeSet.begin();
		 	 itr != treeSet.end(); itr++)
	{
		//debug
		cout << "building tree " << idx << "... " << endl;
		itr->build_tree(treeBagging(trainSet));
		cout << "finished." << endl;
		idx++;
	}
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
	vector<int> vote(valiInst.num_cls,0);
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
// 		 class analyser
// -----------------------

// constructor
analyser::analyser(vector<string> format)
	: true_pos(format.size(),0),
	  false_pos(format.size(),0),
	  false_neg(format.size(),0),
	  precision(format.size(),0),
	  recall(format.size(),0),
		formatTb(format) {}

void analyser::analyse(random_forest &forest, vector<data_inst> valiSet)
{
	int clsfyResult;
	for (int i=0; i<(int)valiSet.size(); i++)
	{
		clsfyResult = forest.classify(valiSet.at(i));
		if (valiSet.at(i).cls == clsfyResult)
		{
			// correctly classified
			true_pos[valiSet.at(i).cls]++;
		}
		else
		{
			// target false negative++
			false_neg[valiSet.at(i).cls]++;
			// other false positive++
			false_pos[clsfyResult]++;
		}
	}

	calculate_result();
}

void analyser::calculate_result()
{
	for (int cls=0; cls < (int)formatTb.size(); cls++)
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

void analyser::print_result(int num_inst)
{
	int trainSize = TRAINPERCENT * (num_inst/100);
	cout << "tran size: " << trainSize << endl;
	cout << "validation size: " << num_inst-trainSize << endl;
	cout << "number of trees: " << NUMTREE << endl;
	cout << "attribute bagging: " << ATTRBAGGING << endl;
	cout << "decision tree max depth:  : " << MAXDEPTH << endl;
	cout << "mininum samples of a node: " << MINSAMPLE << endl;
	cout << endl; 

	for (int i=0; i<(int)formatTb.size(); i++)
	{
		cout << formatTb.at(i) << "\t"
				 << "precision: " << precision[i] << " "
				 << "recall: " << recall[i] << "\n" << endl;
	}
}

// -----------------------
// 	class formatHandler
// -----------------------
vector<string> formatHandler::handle_format(char* fileName)
{
	ifstream ifs(fileName, ifstream::in);
	string buffer;
	vector<string> formatTb;
	
	// set number of attrubutes 
	// and number of classes(labels)
	ifs >> num_attr >> num_cls >> num_inst;

	// set format table
	while (ifs >> buffer)
		formatTb.push_back(buffer);

	ifs.close();
	return formatTb;
}

int formatHandler::get_num_attr()
{
	return num_attr;
}

int formatHandler::get_num_cls()
{
	return num_cls;
}

int formatHandler::get_num_inst()
{
	return num_inst;
}
