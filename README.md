Random Forest
===

- This is a random forest classifier implemented on C++.
- Use CART (Classfication and regression tree) as a base tree
- Using data set: the famous [Iris data set](http://archive.ics.uci.edu/ml/datasets/Iris) on UCI Machine Learning Repository. 

## Usage
To run the program
- make
- ./run.sh
- make clean (To clean the project)

## Playing with parameter
To play around the parameter, you can modify the macro defined in **decision_tree.h** file in **./include**

## Change dataset
To change dataset
- put the dataset in **./data** directory
- add the dataformat file in **./data/format** and name it as **datasetName.format**.
- edit **run.sh** script

## Experiments on performance 
- Relative sizes of the training and validation subsets.
- Number of trees in the forest.
- Parameters used during tree induction. Such as how many attributes to consider during node splitting.
- Methods that limit a tree's size. Example incude the minimum number of samples per node, or an upper bound on the tree's depth.
- **Extremely random forest**: At each node splitting, just randomly select an attribute.
