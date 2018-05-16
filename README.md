Random Forest
===

- This is a random forest classifier implemented on C++.
- Use CART (Classfication and regression tree) as a base tree
- Using data set: the famous [Iris data set](http://archive.ics.uci.edu/ml/datasets/Iris) on UCI Machine Learning Repository. 

## Experiments on performance 
- Relative sizes of the training and validation subsets.
- Number of trees in the forest.
- Parameters used during tree induction. Such as how many attributes to consider during node splitting.
- Methods that limit a tree's size. Example incude the minimum number of samples per node, or an upper bound on the tree's depth.
- **Extremely random forest**: At each node splitting, just randomly select an attribute.
