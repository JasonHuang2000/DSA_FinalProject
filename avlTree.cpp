#include "avlTree.hpp"

void BST::makeEmpty(node* t) {
	if(t == NULL)
		return;
	makeEmpty(t->left);
	makeEmpty(t->right);
	delete t;
}

node* BST::insert(int x, int id, node* t) {
	if(t == NULL) {
		t = new node;
		t->data = x;
		t->id = id;
		t->height = 0;
		t->left = t->right = NULL;
	} else if ( x < t->data ) {
		t->left = insert(x, id, t->left);
		if(height(t->left) - height(t->right) == 2) {
			if(x < t->left->data)
				t = singleRightRotate(t);
			else
				t = doubleRightRotate(t);
		}
	}
	else if(x > t->data) {
		t->right = insert(x, id, t->right);
		if(height(t->right) - height(t->left) == 2) {
			if(x > t->right->data)
				t = singleLeftRotate(t);
			else
				t = doubleLeftRotate(t);
		}
	}

	t->height = max(height(t->left), height(t->right))+1;
	return t;
}

node* BST::singleRightRotate(node* &t) {
	if (t->left != NULL) {
		node* u = t->left;
		t->left = u->right;
		u->right = t;
		t->height = max(height(t->left), height(t->right)) + 1;
		u->height = max(height(u->left), t->height) + 1;
		return u;
	}
	return t;
}

node* BST::singleLeftRotate(node* &t) {
	if (t->right != NULL) {
		node* u = t->right;
		t->right = u->left;
		u->left = t;
		t->height = max(height(t->left), height(t->right)) + 1;
		u->height = max(height(t->right), t->height) + 1;
		return u;
	}
	return t;
}

node* BST::doubleLeftRotate(node* &t) {
	t->right = singleRightRotate(t->right);
	return singleLeftRotate(t);
}

node* BST::doubleRightRotate(node* &t) {
	t->left = singleLeftRotate(t->left);
	return singleRightRotate(t);
}

node* BST::remove(int x, node* t) {
	node* temp;

	// Element not found
	if(t == NULL)
		return NULL;

	// Searching for element
	else if(x < t->data)
		t->left = remove(x, t->left);
	else if(x > t->data)
		t->right = remove(x, t->right);

	// Element found
	// With 2 children
	else if(t->left && t->right) {
		temp = findMin(t->right);
		t->data = temp->data;
		t->right = remove(t->data, t->right);
	}
	// With one or zero child
	else {
		temp = t;
		if(t->left == NULL)
			t = t->right;
		else if(t->right == NULL)
			t = t->left;
		delete temp;
	}
	if(t == NULL)
		return t;

	t->height = max(height(t->left), height(t->right))+1;

	// If node is unbalanced
	// If left node is deleted, right case
	if(height(t->left) - height(t->right) == 2) {
		// right right case
		if(height(t->left->left) - height(t->left->right) == 1)
			return singleLeftRotate(t);
		// right left case
		else
			return doubleLeftRotate(t);
	}
	// If right node is deleted, left case
	else if(height(t->right) - height(t->left) == 2) {
		// left left case
		if(height(t->right->right) - height(t->right->left) == 1)
			return singleRightRotate(t);
		// left right case
		else
			return doubleRightRotate(t);
	}
	return t;
}


