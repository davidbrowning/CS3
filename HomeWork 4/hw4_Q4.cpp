#include <iostream>
#include <fstream>
#include <string>


std::ofstream outputFile; //outputFile is defined as a global variable

struct AvlNode
{
	int key;
	int height;
	AvlNode *left;				
	AvlNode *right;				

	AvlNode (int key1, int height1 = 0, AvlNode *left1 = NULL, AvlNode *right1 = NULL)
	{
		key = key1;
		height = height1;
		left = left1;
		right = right1;
	}
};

class AvlTree 
{
	private:
		AvlNode * root;
		void destroySubTree(AvlNode * v);//destroy a subtree rooted at v
		void displayPreOrder(AvlNode *);//pre-order traversal
		void displayInOrder(AvlNode *);//in-order traversal
		void displayPostOrder(AvlNode *);//post-order traversal
		void remove(AvlNode * &, int);//remove
		void insert(AvlNode * &, int);//insert
		void setHeight(AvlNode * &);//determineHeight
		void balance(AvlNode * &); //make the tree balanced after each insert/remove
		void rightRotate (AvlNode * &);//right rotation
		void leftRotate (AvlNode * &);//left rotation
		void doubleLeftRightRotate (AvlNode * &);//left right double rotation
		void doubleRightLeftRotate (AvlNode * &);//right left double rotation
		int max(int, int); //determine which height is bigger

	public:
		AvlTree() { root = NULL; }
		~AvlTree() { destroySubTree(root); }
		AvlNode * search(int);
		void displayPreOrder() { displayPreOrder(root); }
		void displayInOrder() { displayInOrder(root); }
		void displayPostOrder() { displayPostOrder(root); }
		void insert(int x) { insert(root,x); }//insert a new key x
		void remove(int x) { remove(root,x); }//remove a key x
		int getHeight(AvlNode * v);//return the height of node v
		int getTreeHeight() {return getHeight(root);}//return the height of the tree
		int getRoot() {return root->key;}//return the root
};

int AvlTree::getHeight(AvlNode * v)
{
	if (v == NULL)
		return -1;
	else
		return v->height;
}

void AvlTree::destroySubTree(AvlNode *v)
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

void AvlTree::insert(AvlNode * & v, int x)
{
	if (v == NULL)
		v = new AvlNode(x);
	else if (x == v->key)
		return;
	else if (x < v->key){
		insert(v->left, x);
	}
	else{
		insert(v->right, x);
	}

	balance(v);
}

int AvlTree::max(int l, int r){
	if (l > r){
		return l;
	}
	else return r;
}

//please complete the following five functions
void AvlTree::balance(AvlNode * & v)
{  
	if (v == NULL){
		return;
	}
	//v->height = 1 + max(getHeight(v->left), getHeight(v->right));
	/*Prepare for brainstorming: I need to figure out whether the node I've been given is unbalanced...
	I have available to me the height of any given node within the subtree of the node I've been given
	if the height of this node is greater than the height of any of its children by more than 1, the node is unbalanced.
	for example, say the height of this node is 3, and the height of one of its' children is 1. The difference between the two
	is 2. The node is therefore unbalanced, and it is the child with the lowest height that must be corrected.
	If, however, the node is balanced, do we then need to check its' children? Yes! a parent can seem balanced though
	its children are not! For example: A parent has a heightof 5, both of its children, a height of 4 the left child's left
	child has a height of 3, but the left child's right pointer is NULL and has a height of -1. Because of this Idea, we must
	check any children of the given node whose height is > 1. We can do this recursively. Upon further reflection, this is no longer
	necessary as we will recurse into the function until the entire subtree is also balanced.

	an attempt at pseudo-code:

	if v->height - v->left->height > 1, rightRotate(v); per node in the AVL tree, there can be only one solution.
	else if v->height - v->right->height > 1, leftRotate(v)
	else if v->left->height - v->left->right->height > 1, doubleLeftRightRotate(v)
	else if v->right->height - v->right->left->height > 1, doubleRightLeftRotate(v)

	balance(v->right); my base case is that v == NULL, therefore I have to check every node to assure the tree is balanced.
	balance(v->left);

	Upon further reflection, my pseudo code will not work in the event of v->right, v->left, v->right->left, or v->left->right
	being a NULL pointer. Before determining which rotation(s) to make, it seems pertinant to discover which (if any) are null,
	and to assure that the function understands that a NULL pointer indicates a 0 height. As I read over the code, I discovered
	the getHeight() function and feel a bit foolish for not discovering it earlier.

	modified pseudo-code:

	//heights[0] is current node
	//heights[1] is right
	//heights[2] is left
	//heights[3] is right left
	//heights[4] is left right

	int[] heights = {v->getHeight(), v->right->getHeight(), v->left->getHeight(), v->right->left->getHeight(), v->left->right->getHeight()}
	if heights[0] - heights[2] > 1, rightRotate(v); //per node in the AVL tree, there can be only one solution.
	else if heights[0] - heights[1] > 1, leftRotate(v)
	else if heights[2] - heights[4] > 1, doubleLeftRightRotate(v)
	else if heights[1] - heights[3] > 1, doubleRightLeftRotate(v)

	balance(v->right); //my base case is that v == NULL, therefore I have to check every node to assure the tree is balanced.
	balance(v->left);

	I'm trying to think through the logic, because since NULL returns -1, the logic as it stands doesn't fit i.e. if the node itself had
	a height of 1 with a null right child and a leaf left child, the result as per the function as written is to call leftRotate(v) since
	1--1 is 2.  I cannot have getHeight return any positive number. I'm not sure why, but I seem to have hit a bit of a roadblock.

	I can't think of an alternative way to determine whether or not the tree is balanced than to use the height, but how to use it
	in a way that would still make sense of negative responses from getHeight()...For the intent of this function, can I have getHeight
	return 0 for a NULL pointer? nope, that wouldn't make sense, but what does make sense is that I had a miscompmrehension
	of the height difference, it seems that the heights must differ by a value greater than 2, not 1 in order to be considered
	unbalanced.

	Final draft of pseudocode, I believe I'm ready to test the function:

	//heights[0] is current node
	//heights[1] is right
	//heights[2] is left
	//heights[3] is right left
	//heights[4] is left right

	int[] heights = {v->getHeight(), v->right->getHeight(), v->left->getHeight(), v->right->left->getHeight(), v->left->right->getHeight()}
	if heights[0] - heights[2] > 2, rightRotate(v); //per node in the AVL tree, there can be only one solution.
	else if heights[0] - heights[1] > 2, leftRotate(v)
	else if heights[2] - heights[4] > 2, doubleLeftRightRotate(v)
	else if heights[1] - heights[3] > 2, doubleRightLeftRotate(v)

	balance(v->right); //my base case is that v == NULL, therefore I have to check every node to assure the tree is balanced.
	balance(v->left);

	I do not even begin to comprehend what to do regarding determining the height of a given node.
	*/

	//After reviewing the pseudo code from class, the Array was a stupid Idea. 

	if ((getHeight(v->left) - getHeight(v->right)) > 1){
		if (getHeight(v->left->left) >= getHeight(v->left->right)) { rightRotate(v); }  //per node in the AVL tree, there can be only one solution.
		else doubleLeftRightRotate(v);
	}
	if ((getHeight(v->right) - getHeight(v->left) > 1)){
		if (getHeight(v->right->right) >= getHeight(v->right->left)) { leftRotate(v); }
		else doubleRightLeftRotate(v);
	}

	v->height = 1 + max(getHeight(v->left), getHeight(v->right));

	//balance(v->right); //my base case is that v == NULL, therefore I have to check every node to assure the tree is balanced.
	//balance(v->left);

	//after first test, program crashed... great :/ ah! if v->left or v->right is NULL, it has no left or right, and I can't call getHeight on it...
}

void AvlTree::rightRotate(AvlNode * & k2)
{
	std::cout << "right rotation" << std::endl;
	//I have tried to write this function 3 times. it is 1:22 am. I 
	//have to go to work at 5:45. if it doesn't work this time, I 
	//give up. I looked up more class notes.
	auto x = k2;
	auto y = k2->left;
	x->left = y->right;
	y->right = x;
	x->height = 1 + max(getHeight(x->left), getHeight(x->right));
	y->height = 1 + max(getHeight(y->left), getHeight(y->right));
	k2 = y;
}

void AvlTree::leftRotate (AvlNode * & k1)
{
	auto x = k1;
	auto y = k1->right;
	x->right = y->left;
	y->left = x;
	x->height = 1 + max(getHeight(x->left), getHeight(x->right));
	y->height = 1 + max(getHeight(y->left), getHeight(y->right));
	k1 = y;
	return;
}

void AvlTree::doubleLeftRightRotate (AvlNode * & v)
{
	std::cout << "perform doubleLeftRightRotation" << std::endl;
	leftRotate(v->left);
	rightRotate(v);
	return;

}

void AvlTree::doubleRightLeftRotate (AvlNode * & v)
{
	std::cout << "perform doubleRightLeftRotation" << std::endl;
	rightRotate(v->right);
	leftRotate(v);
	return;
}

void AvlTree::displayPreOrder(AvlNode *v)
{
	if (v != NULL)
	{
		std::cout << v->key << "  ";
		outputFile << v->key << "  ";
		displayPreOrder(v->left);
		displayPreOrder(v->right);
	}
}

void AvlTree::displayInOrder(AvlNode *v)
{
	if (v != NULL)
	{
		displayInOrder(v->left);
		std::cout << v->key << "  ";
		outputFile << v->key << "  ";
		displayInOrder(v->right);
	}
}

void AvlTree::displayPostOrder(AvlNode *v)
{
	if (v != NULL)
	{
		displayPostOrder(v->left);
		displayPostOrder(v->right);
		std::cout << v->key << "  ";
		outputFile << v->key << "  ";
	}
}

AvlNode * AvlTree::search(int x)
{
	AvlNode *v = root;
	while (v != NULL && v->key != x)
	{
		if (x < v->key)
			v = v->left;
		else
			v = v->right;
	}

	if (v != NULL)
		return v;
	else
		return NULL;
}

void AvlTree::remove(AvlNode * & v, int x)
{
	if (v == NULL)
		return;
	if (x < v->key)
		remove(v->left, x);
	else if (x > v->key)
		remove(v->right, x);
	else//x == v->key
	{
		// if at least one child is NULL
		if (v->left == NULL || v->right == NULL)
		{
			AvlNode * garbageNode = v;
			if (v->right == NULL)
				v = v->left;
			else
				v = v->right;
		}
		else // v has two children
		{
			AvlNode * u = v->right;
			//find the smallest key on the right subtree of v
			while(u->left != NULL)
				u = u->left;
			//assign the key value of u to v
			v->key = u->key;
			//remove the node u from the right subtree of v
			remove(v->right,u->key);
		}
	}

	balance(v);
}

int main()
{
	std::cout << "initiating..." << std::endl;
	AvlTree tree;
	
	//open the input file
	std::ifstream inputFile;
	inputFile.open("hw4_Q4_input.txt");
	if (!inputFile)
		std::cout << "Error opening the input file " << std::endl;
	
	//open the output file, outputFile is defined as a global variable
	outputFile.open("hw4_Q4_output.txt");
	if (!outputFile)
		std::cout << "Error opening the output file " << std::endl;

	//read operations from the input file
	std::string op;
	int x;
	while(inputFile >> op)
	{
		if (op == "insert")
		{
			inputFile >> x; // read the value x for insert
			tree.insert(x);
		}
		else if (op == "remove")
		{
			inputFile >> x; // read the value x for remove 
			tree.remove(x);
		}
		else
			std::cout << "Error reading opeartions"; 
	}

	//print the pre-odrder traversal to both screen and the outputfile
	std::cout << "The pre-order traversal list is: " << std::endl;
	outputFile << "The pre-order traversal list is: " << std::endl;
	tree.displayPreOrder();
	std::cout << std::endl;
	outputFile << std::endl;

	//print the in-order traversal to both screen and the outputfile
	std::cout << "The in-order traversal list is: " << std::endl;
	outputFile << "The in-order traversal list is: " << std::endl;
	tree.displayInOrder();
	std::cout << std::endl;
	outputFile << std::endl;
	
	//print the root and its height to both screen and the output file 
	std::cout << "The root is: " << tree.getRoot() << std::endl;
	outputFile << "The root is: " << tree.getRoot() << std::endl;
	std::cout << "The height of the tree is: " << tree.getTreeHeight() << std::endl;
	outputFile << "The height of the tree is: " << tree.getTreeHeight() << std::endl;
	
	/* The following commented code may help you debug your program.
	 * You may change "16" to other keys as you need. 
	AvlNode * v = tree.search(16);
	if (v != NULL)
	{
	    cout << "The key is found. Its height is: " << tree.getHeight(v) << endl;
		if (v->left != NULL) 
			cout << "It's left child is: " << v->left->key << "." << endl;
		else
			cout << "It does not have a left child. " << endl;
		
		if (v->right != NULL) 
			cout << "It's right child is: " << v->right->key << "." << endl;
		else
			cout << "It does not have a right child. " << endl;
	}
	else
		cout << "The key is not found." << endl;
	*/


	inputFile.close();
	outputFile.close();

	return 0;
}
