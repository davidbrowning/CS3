#include <iostream>
#include <fstream>
#include <string>
using namespace std;

ofstream outputFile; //outputFile is defined as a global variable

struct TreeNode
{
	int key;
	TreeNode *left;				
	TreeNode *right;				

	//constructor of the struct
	TreeNode (int key1, TreeNode *left1 = NULL, TreeNode *right1 = NULL)
	{
		key = key1;
		left = left1;
		right = right1;
	}
};

class BST
{
	private:
		TreeNode * root; // the root of the tree
		void destroySubTree(TreeNode *);// delete all nodes of the tree
		void displayPreOrder(TreeNode *); //pre-order traversal
		void displayInOrder(TreeNode *);//in-order traversal
		void displayPostOrder(TreeNode *); //post-order traversal
		void remove(TreeNode * & v, int x); 
		void insert(TreeNode * & v, int x);
		bool search(TreeNode * & v, int x);

	public:
		BST() { root = NULL; }
		~BST() { destroySubTree(root); }
		void insert(int x){ insert(root, x); }
		bool search(int x){ bool here = search(root, x); return here; }
		void displayPreOrder() { displayPreOrder(root); }// pre-order traversal
		void displayInOrder() { displayInOrder(root); }//in-order traversal
		void displayPosteOrder() { displayPostOrder(root); }//post-order traversal
		void remove(int x) { remove(root,x); }
		int findMin();
};

void BST::destroySubTree(TreeNode *v)
{
	if (v == NULL)
		return;
	else
	{
		destroySubTree(v->left);
		destroySubTree(v->right);
		delete v;
	}
}

void BST::displayPreOrder(TreeNode *v)
{
	if (v != NULL)
	{
		cout << v->key << "  ";
		outputFile << v->key << "  ";
		displayPreOrder(v->left);
		displayPreOrder(v->right);
	}
}

void BST::displayInOrder(TreeNode *v)
{
	if (v != NULL)
	{
		displayInOrder(v->left);
		cout << v->key << "  ";
		outputFile << v->key << "  ";
		displayInOrder(v->right);
	}
}

void BST::displayPostOrder(TreeNode *v)
{
	if (v != NULL)
	{
		displayPostOrder(v->left);
		displayPostOrder(v->right);
		cout << v->key << "  ";
		outputFile << v->key << "  ";
	}
}


int BST::findMin()
{
	auto v = root;
	while (v->left != NULL){
		v = v->left;
	}
	return v->key;
}


void BST::insert(TreeNode * & v, int x)
{
	if (v == NULL){
		v = new TreeNode(x); return;
	}
	if (x == v->key){
		return;
	}
	if (x < v->key){
		if (v->left == NULL){
			v->left = new TreeNode(x);
			return;
		}
		else insert(v->left, x);
	}
	if (x > v->key){
		if (v->right == NULL){
			v->right = new TreeNode(x);
			return;
		}
		else insert(v->right, x);
	}
}


bool BST::search(TreeNode * & v, int x)
{
	if (v == NULL){
		return false;
	}
	if (x == v->key){
		return true;
	}
	else if (x < v->key){
		search(v->left, x);
	}
	else if (x > v->key){
		search(v->right, x);
	}
	else if (v == NULL){
		return false;
	}
}

void BST::remove(TreeNode * & v, int x)
{
	if (v == NULL){
		return;
	}
	else if (x == v->key){
		if (v->right == NULL && v->left == NULL){
			v = NULL;
			//std::cout << "deleted leaf" << std::endl;
			delete v;
			return;
		}
		else if (v->right == NULL){
			v = v->left;
			//std::cout << "v->right is NULL" << std::endl;
			//how would I delete the old pointer? Usually I'd just use smart pointers,
			//but I'd like to learn how raw pointers work? I'm fine with losing points,
			//could you write a quick sample code as to how to do it?
		}
		else if (v->left == NULL){
			v = v->right;
			//std::cout << "v->left is NULL" << std::endl;
		}
		else{
			auto u = v->right;
			while (u->left != NULL){
				u = u->left;
			}
			v->key = u->key;
			remove(u, u->key); //I'm not sure if this is 100% correct, but I'm pretty sure
			//that's the idea.
		}
		return;
	}
	else if (x < v->key && v->left != NULL){
		remove(v->left, x);
	}
	else if (x > v->key && v->right != NULL){
		remove(v->right, x);
	}
	return;
}

int main()
{
	BST tree;

	string inputFileName = "hw3_Q4_input.txt"; // input file with operations 
	string outputFileName = "hw3_Q4_output.txt"; // output file

	//open files
	ifstream inputFile;
	inputFile.open(inputFileName.c_str());
	if (!inputFile)
		cout << "Error opening the input file " << endl;
	
	outputFile.open(outputFileName.c_str());
	if (!outputFile)
		cout << "Error opening the output file " << endl;

	//read operations from the input file
	string op;
	int x;
	while(inputFile >> op)
	{
		if (op == "insert")
		{
			inputFile >> x; // read the value x for insert
			tree.insert(x);
		}
		if (op == "remove")
		{
			inputFile >> x; // read the value x for remove 
			tree.remove(x);
		}
		if (op == "search")
		{
			inputFile >> x;
			if (tree.search(x) == true)
			{
				cout << "The key " << x << " is in the current tree.\n";
				outputFile << "The key " << x << " is in the current tree.\n";
			}
			else// x is not in the tree
			{
				cout << "The key " << x << " is not in the current tree.\n";
				outputFile << "The key " << x << " is not in the current tree.\n";
			}
		}
		if (op == "findMin")
		{
			cout << "The smallest key in the current tree is " << tree.findMin() << endl;
			outputFile << "The smallest key in the current tree is " << tree.findMin() << endl;
		}
	}

	//print the pre-odrder traversal to both screen and the outputfile
	cout << "The pre-order traversal list is: " << endl;
	outputFile << "The pre-order traversal list is: " << endl;
	tree.displayPreOrder();
	cout << endl;
	outputFile << endl;

	//print the in-odrder traversal to both screen and the outputfile
	cout << "The in-order traversal list is: " << endl;
	outputFile << "The in-order traversal list is: " << endl;
	tree.displayInOrder();
	cout << endl;
	outputFile << endl;

	inputFile.close();
	outputFile.close();

	return 0;
}
