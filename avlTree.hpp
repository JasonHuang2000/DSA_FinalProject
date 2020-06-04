#include <algorithm>
using namespace std;

struct node {
	int data;
	int id;
	node* left;
	node* right;
	int height;
};

class BST {
	
	private:
		node* root;
		void makeEmpty(node *t);
		node* insert(int x, int id, node* t);
		node* singleLeftRotate(node* &t);
		node* singleRightRotate(node* &t);
		node* doubleLeftRotate(node* &t);
		node* doubleRightRotate(node* &t);
		node* remove(int x, node* t);
		int height(node *t) { return (t == NULL ? -1 : t->height); }
		node* findMax(node* t) {
			if (t == NULL)
				return NULL;
			else if (t->right == NULL)
				return t;
			else
				return findMax(t->right);
		}
		node* findMin(node* t) {
			if(t == NULL)
				return NULL;
			else if(t->left == NULL)
				return t;
			else
				return findMin(t->left);
		}

	public:
		BST() { root = NULL; }
		void insert(int x, int id) { root = insert(x, id, root); }
		void remove(int x) { root = remove(x, root); }
		node* MaxElem() { return findMax(root); }
};
