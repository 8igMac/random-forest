#include "../include/decision_tree.h"

// randomly select 'n' number without replacement
// on the interval [begin, end]
vector<int> rdmSelectSet(int begin, int end, int n)
{
	// implement Kruth shuffle algorithm
	vector<int> list(n);
	srand(time(NULL));

	// init list 
	for (int i=0; i<n; i++)
		list[i] = begin+i;

	// shuffle
	for (int i=n-1; i>=0; i--)
	{
		int j = rand()%(i+1);
		swap(list[i], list[j]);
	}

	return vector<int>(list.begin(), list.begin()+n);
}

// -----------------------
// 	struct iris
// -----------------------

iris::iris()
	: attr(4), num_cls(3) {}

// -----------------------
// 		class irisDataSet
// -----------------------
void irisDataSet::get_data_from_file(char* fileName)
{
	ifstream ifs(fileName, ifstream::in);
	string buffer;
	string classBuffer;
	size_t pos;
	stringstream ss;
	iris myiris;

	for (int index = 0 ; ifs >> buffer; index++)
	{
		// replace ',' with ' '
		while ((pos = buffer.find(',')) != string::npos)
			buffer[pos] = ' ';

		// use stringstream to get iris data
		ss << buffer;
		ss >> myiris.attr[0] >> myiris.attr[1] >> myiris.attr[2] >> myiris.attr[3] >> classBuffer;

		// store data index
		myiris.inst_num = index;

		// covert class string to enum
		if (classBuffer.compare("Iris-setosa") == 0)
			myiris.cls = setosa;
		else if (classBuffer.compare("Iris-versicolor") == 0)
			myiris.cls = versicolor;
		else if (classBuffer.compare("Iris-virginica") == 0)
			myiris.cls = virginica;
		else
		{
			cout << "iris class name error: " << classBuffer << endl;
			break;
		}

		dataSet.push_back(myiris);
		ss.clear();
	}

	ifs.close();
}

void irisDataSet::print_dataSet()
{
	for (auto item: dataSet)
		cout << setw(3) << item.inst_num << " " 
				 << setw(3) << item.attr[0] << " " 
				 << setw(3) << item.attr[1] << " " 
				 << setw(3) << item.attr[2] << " " 
				 << setw(3) << item.attr[3] << " "
				 << setw(3) << item.cls << endl;
}

void irisDataSet::split_data(vector<iris> &trainSet, vector<iris> &valiSet)
{
	vector<int> rdmPermutation(rdmSelectSet(0,dataSet.size()-1, dataSet.size()));
	for (int i=0; i<dataSet.size(); i++)
	{
		if (i<TRAINSIZE)
			trainSet.at(i) = dataSet.at(rdmPermutation[i]);
		else
			valiSet.at(i-TRAINSIZE) = dataSet.at(rdmPermutation[i]);
	}
}

// -----------------------
// 			class node 
// -----------------------

template <class T>
node<T>::node()
	: samplePool(0), leftChild(NULL), rightChild(NULL) {}

template <class T>
node<T>* node<T>::traceNextNode(T valiInst)
{
	// if <= threshold -> leftChild
	// if >  threshold -> rightChild
	if (valiInst.attr[attr.first] <= attr.second)
		return leftChild;
	else
		return rightChild;
}

template <class T>
int node<T>::majClass()
{
	if (samplePool.size() == 0)
		return -1;
	else
	{
		int majCls = -1;
		int max = -1;
		for (int i=0; i<size_of_each_class.size(); i++)
			if (size_of_each_class[i] > max)
			{
				max = size_of_each_class[i];
				majCls = i;
			}

		return majCls;
	}
}

template <class T>
bool node<T>::poolPure()
{
	for (auto size: size_of_each_class)
		if (size == samplePool.size())
			return true;

	return false;
}

template <class T>
void node<T>::set_size_of_class()
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

template <class T>
decision_tree<T>::decision_tree()
	: rootNodePtr(NULL) {}

template <class T>
decision_tree<T>::~decision_tree()
{
	destory_tree(rootNodePtr);
}

template <class T>
void decision_tree<T>::build_tree(vector<T> trainSet)
{
	rootNodePtr = new node<T>;
	build_tree(rootNodePtr, trainSet, 1);
}

template <class T>
void decision_tree<T>::build_tree(node<T>* nodePtr, vector<T> sampleSet, int depth)
{
	nodePtr->depth = depth;
	nodePtr->samplePool = sampleSet;
	nodePtr->numSample = sampleSet.size();
	nodePtr->set_size_of_class();

	if (depth >= MAXDEPTH || 
			nodePtr->numSample >= MINSAMPLE || 
			!(nodePtr->poolPure()))
	{
		// select attribut
		nodePtr->attr = selectAttr(sampleSet);

		// split sample into two group
		vector<T> leftSample, rightSample;
		for (auto sample: sampleSet)
		{
			if (sample.attr[nodePtr->attr.first] <= nodePtr->attr.second)
				leftSample.push_back(sample);
			else
				rightSample.push_back(sample);
		}

		// generate next node
		nodePtr->leftChild = new node<T>;
		build_tree(nodePtr->leftChild, leftSample, depth+1);
		nodePtr->rightChild = new node<T>;
		build_tree(nodePtr->leftChild, rightSample, depth+1);
	}
}

template <class T>
float decision_tree<T>::impurity(vector<T> sampleSet)
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

template <class T>
pair<int,float> decision_tree<T>::selectAttr(vector<T> sampleSet)
{
	float bestThreshold;
	float lowestImpurity = 1;
	int attrSelected;

	// attribute bagging
	vector<int> attrList(rdmSelectSet(0,3,2));

	// select attribut, and threshold
	for (auto attr_idx: attrList)
	{
		vector<float> valueList;
		vector<float> thresholdList;

		// get threshold list
		for (auto item: sampleSet)
			valueList.push_back(item.attr[attr_idx]);

		sort(valueList.begin(), valueList.end());

		for (int i=0; i < (valueList.size()-1); i++)
			thresholdList.push_back((valueList[i]+valueList[i+1])/2);

		// get gest threshold
		for (auto threshold: thresholdList)
		{
			// split samples into two groups
			vector<T> leftSample, rightSample;
			for (auto sample: sampleSet)
			{
				if (sample.attr[attr_idx] <= threshold)
					leftSample.push_back(sample);
				else
					rightSample.push_back(sample);
			}

			// calculate gini impurity of two group
			float imp, leftImpurity, rightImpurity;
			leftImpurity = impurity(leftSample);
			rightImpurity = impurity(rightSample);
			imp = (((float)leftSample.size() * leftImpurity) +
								 	((float)leftSample.size() * leftImpurity) ) / (float) sampleSet.size();

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

template <class T>
int decision_tree<T>::classify(T valiInst)
{
	return classify(rootNodePtr, valiInst);
}

template <class T>
int decision_tree<T>::classify(node<T>* nodePtr, T valiInst)
{
	if (nodePtr->leftChild == NULL && nodePtr->rightChild == NULL)
		return nodePtr->majClass();
	else
		return classify(nodePtr->traceNextNode(valiInst), valiInst);
}

template <class T>
void decision_tree<T>::destory_tree(node<T>* leaf)
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

template <class T>
random_forest<T>::random_forest()
	: treeSet(NUMTREE) {}

template <class T>
void random_forest<T>::build_forest(vector<T> trainSet)
{
	for (auto tree: treeSet)
		tree.build_tree(treeBagging(trainSet));	
}

// Implement tree bagging: random sample with replacement on 
// given trainset.
template <class T>
vector<T> random_forest<T>::treeBagging(vector<T> trainSet)
{
	vector<T> newTrainSet(trainSet.size());
	int sample;

	// generate random seed
	srand(time(NULL));

	for(int i=0; i<newTrainSet.size(); i++)
	{
		sample = rand()%(trainSet.size());
		newTrainSet.at(i) = trainSet.at(sample);
	}

	return newTrainSet;
}

template <class T>
int random_forest<T>::classify(T valiInst)
{
	// random number seed
	srand(time(NULL));

	// vote
	vector<int> vote(3,0);
	for (auto tree: treeSet)
		vote[tree.classify(valiInst)]++;

	// reach consensus among trees
	int majorVote=0;
	for (int i=1; i<vote.size(); i++)
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

void irisAnalyser::analyse(random_forest<iris> forest, vector<iris> valiSet)
{
	int clsfyResult;
	for (auto item: valiSet)
	{
		clsfyResult = forest.classify(item);
		if (item.cls == clsfyResult)
			// correctly classified
			true_pos[item.cls]++;
		else
		{
			// target false negative++
			false_neg[item.cls]++;
			// other false positive++
			false_pos[clsfyResult]++;
		}
	}
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

/*
//debug
int main(int argc, char** argv)
{
	irisDataSet dataSet;
	vector<iris> irisTrainSet(TRAINSIZE), irisValiSet(VALIDSIZE);
	random_forest<iris> irisForest;
	irisAnalyser irsAlyzr;

	if (argc != 2)
		cout << "usage: ./main data-set" << endl;

	// extract data from file
	dataSet.get_data_from_file(argv[1]);

	// split data into training subset and
	// validation subset
	dataSet.split_data(irisTrainSet, irisValiSet);

	// build forest
	irisForest.build_forest(irisTrainSet);
	
	// validate and analyse accuracy
	irsAlyzr.analyse(irisForest, irisValiSet);
	irsAlyzr.print_result();
	
	return 0;
}
*/
