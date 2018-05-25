#include "../include/decision_tree.h"

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
	int index = 0;

	for (; !ifs.eof(); index++)
	{
		// read line into buffer
		ifs >> buffer;

		// replace ',' with ' '
		while ((pos = buffer.find(',')) != string::npos)
			buffer[pos] = ' ';

		// use stringstream to get iris data
		ss << buffer;
		ss >> myiris.sl >> myiris.sw >> myiris.pl >> myiris.pw >> classBuffer;

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
				 << setw(3) << item.sl << " " 
				 << setw(3) << item.sw << " " 
				 << setw(3) << item.pl << " " 
				 << setw(3) << item.pw << " "
				 << setw(3) << item.cls << endl;
}

// -----------------------
// 	class decision_tree
// -----------------------

// -----------------------
// 	class random_forest
// -----------------------

// -----------------------
// 		 class irisAnalyser
// -----------------------

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
 

