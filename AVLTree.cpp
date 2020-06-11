#include "MailBox.hpp"

template <class T>
AVLTreeNode<T>::AVLTreeNode(T value, int id): value(value), id(id){
    count = 1;
    height = 1;
    
    left = nullptr;
    right = nullptr;
}

template <class T>
void AVLTreeNode<T>::updateValues(){
    count = (left != nullptr ? left->count : 0) + (right != nullptr ? right->count : 0) + 1;
    
    height = std::max(left != nullptr ? left->height : 0,
                      right != nullptr ? right->height : 0) + 1;
}

template <class T>
int AVLTreeNode<T>::balanceFactor(){
    return (left != nullptr ? left->height : 0) - (right != nullptr ? right->height: 0);
}

template <class T>
AVLTreeNode<T>* AVLTreeNode<T>::left_rotate(){
    AVLTreeNode* R = right;
    right = right->left;
    R->left = this;
    
    this->updateValues();  // the order is important
    R->updateValues();
    
    return R;
}

template <class T>
AVLTreeNode<T>* AVLTreeNode<T>::right_rotate(){
    AVLTreeNode* L = left;
    left = left->right;
    L->right = this;
    
    this->updateValues();  // the order is important
    L->updateValues();
    
    return L;
}


template <class T>
AVLTree<T>::AVLTree(){
    root = nullptr;
    _size = 0;
}

template <class T>
AVLTree<T>::~AVLTree(){
    clear();
}

template <class T>
void AVLTree<T>::clear(){
    std::vector<AVLTreeNode<T>*> stack;
    
    if (root != nullptr)
        stack.push_back(root);
    
    while (!stack.empty()){
        AVLTreeNode<T> *node = stack.back();
        stack.pop_back();
        
        if (node->left != nullptr)
            stack.push_back(node->left);
        
        if (node->right != nullptr)
            stack.push_back(node->right);
        
        _size--;
        delete node;
    }
    
    root = nullptr;
}

template <class T>
void AVLTree<T>::insert(T value, int id){
    AVLTreeNode<T> **indirect = &root;  // to generalize insertion
    std::vector<AVLTreeNode<T>**> path;  // to update height values
    
    while (*indirect != nullptr){
        path.push_back(indirect);
        
        if ((*indirect)->value > value || ((*indirect)->value == value && (*indirect)->id < id))
            indirect = &((*indirect)->left);
        else 
            indirect = &((*indirect)->right);
    }
    
    *indirect = new AVLTreeNode<T>(value, id);
    path.push_back(indirect);
    
    balance(path);
    _size++;
}

template <class T>
void AVLTree<T>::erase(T value, int id){
    AVLTreeNode<T> **indirect = &root;  // to generalize insertion
    std::vector<AVLTreeNode<T>**> path;  // to update height values
    
    while (*indirect != nullptr && !((*indirect)->value == value && (*indirect)->id == id)){
        path.push_back(indirect);
        
        if ((*indirect)->value > value || ((*indirect)->value == value && (*indirect)->id < id))
            indirect = &((*indirect)->left);
        else
            indirect = &((*indirect)->right);
    }
    
    if (*indirect == nullptr)  // the value does not exist in tree
        return;  // may be raising an Exception is more elegant
    
    else
        path.push_back(indirect);
    
    std::size_t index = path.size();
    
    if ((*indirect)->left == nullptr and (*indirect)->right == nullptr){  // the node is leaf
        delete *indirect;  // just delete node
        *indirect = nullptr;
        path.pop_back();  // pop the deleted node from path
    }
    
    else if ((*indirect)->right == nullptr){  // only left child exists
        AVLTreeNode<T> *toRemove = *indirect;
        
        (*indirect) = (*indirect)->left;
        delete toRemove;
        
        path.pop_back();
    }
    
    else{  // right child exists
        AVLTreeNode<T> **successor = &((*indirect)->right);
        
        while ((*successor)->left != nullptr){
            path.push_back(successor);
            successor = &((*successor)->left);
        }
        
        if (*successor == (*indirect)->right){
            (*successor)->left = (*indirect)->left;
            
            AVLTreeNode<T> *toRemove = *indirect;
            *indirect = *successor;
            delete toRemove;
        }
        
        else{
            AVLTreeNode<T> *tmp = *path.back(), *suc = *successor;
            
            tmp->left = (*successor)->right;
            suc->left = (*indirect)->left;
            suc->right = (*indirect)->right;
            
            delete *indirect;
            *indirect = suc;
            path[index] = &(suc->right);
        }
    }
    
    balance(path);
    _size--;
}

template <class T>
void AVLTree<T>::balance(std::vector<AVLTreeNode<T> **> path){  // starting from root
    std::reverse(path.begin(), path.end());
    
    for (auto indirect: path){
        (*indirect)->updateValues();
        
        if ((*indirect)->balanceFactor() >= 2 and (*indirect)->left->balanceFactor() >= 0)   // left - left
            *indirect = (*indirect)->right_rotate();
        
        else if ((*indirect)->balanceFactor() >= 2){  // left - right
            (*indirect)->left = (*indirect)->left->left_rotate();
            *indirect = (*indirect)->right_rotate();
        }
        
        else if ((*indirect)->balanceFactor() <= -2 and (*indirect)->right->balanceFactor() <= 0)  // right - right
            *indirect = (*indirect)->left_rotate();
        
        else if ((*indirect)->balanceFactor() <= -2){  // right - left
            (*indirect)->right = ((*indirect)->right)->right_rotate();
            *indirect = (*indirect)->left_rotate();
        }
    }
}

template <class T>
bool AVLTree<T>::empty() const{
    return _size == 0;
}

template <class T>
int AVLTree<T>::size() const{
    return _size;
}

template <class T>
int AVLTree<T>::find(T value) const{
    AVLTreeNode<T> *direct = root;
    int idx = 0;
    
    while (direct != nullptr and direct->value != value){
        if (direct->value > value)
            direct = direct->left;
        else{
            idx += (direct->left ? direct->left->count : 0) + 1;
            direct = direct->right;
        }
    }
    
    if (direct == nullptr)
        return -1;
    
    else
        return direct->id; 
}

template <class T>
const T& AVLTree<T>::find_max() const{
    AVLTreeNode<T> *cur = root;
    
    while (cur->right != nullptr)
        cur = cur->right;
    
    return cur->value;
}

template <class T>
void AVLTree<T>::longest() {
	AVLTreeNode<T> *cur = root;
	if ( root == nullptr ) {
		printf("-\n");
		return;
	}
	while ( cur->right != nullptr ) {
		cur = cur->right;
	}
	printf("%d %d\n", cur->id, cur->value);
}

template <class T>
void AVLTree<T>::inorder_trvs(AVLTreeNode<T> *node) {
	if ( node->left != NULL ) inorder_trvs(node->left);	
	printf("(%d, %d) ", node->id, node->value);
	if ( node->right != NULL ) inorder_trvs(node->right);
}

template class AVLTreeNode<int>;
template class AVLTree<int>;
