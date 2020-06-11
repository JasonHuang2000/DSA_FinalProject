#include <iostream>
#include <string>
#include <vector>
#include <map> // std::multiset (rb-tree)
#include <unordered_set>
#include <stack>
#include <algorithm>

using namespace std;

class MailBox;
struct Mail;
struct FromElem;
struct ToElem;

struct Mail { // store the infomation of a mail.
	// variable
	string from;
	string to;
	int* date;
	int id;
	int char_count;
	unordered_set<string> *words;
	// function
	Mail(string _from, string _to, int* _date, int _id, int _char_count, unordered_set<string> *_words) : from(_from), to(_to), date(_date), id(_id), char_count(_char_count), words(_words) { }
	void mailInfo();
};

struct FromElem {
	// variable
	struct IDElem {
		string to;
		int* date;	
		unordered_set<string> *words;
		IDElem(string to, int* date, unordered_set<string> *words) : to(to), date(date), words(words) {}
	};
	map<int, IDElem> IDMap;	
};
struct ToElem {
	// variable
	struct IDElem {
		int* date;	
		unordered_set<string> *words;
		IDElem(int* date, unordered_set<string> *words) : date(date), words(words) {}
	};
	map<int, IDElem> IDMap;	
};

template <class T>
struct AVLTreeNode {
    AVLTreeNode *left;
    AVLTreeNode *right;
    
    const T value;
	int id;
    int count;  // how many nodes are there in this subtree
    int height;
    
    AVLTreeNode(T value, int id);
    void updateValues();
    int balanceFactor();
    
    AVLTreeNode* left_rotate();
    AVLTreeNode* right_rotate();
};

template <class T>
class AVLTree {
    int _size;
    void balance(std::vector<AVLTreeNode<T>**> path);
    
public:
    AVLTree();
    ~AVLTree();
    
    AVLTreeNode<T> *root;
    void insert(T value, int id);
    void erase(T value, int id);
    
    void clear();
    bool empty() const;
    int size() const;
    int find(T value) const;
    const T& find_max() const;

	void inorder_trvs(AVLTreeNode<T> *node);
	void longest();
};

class MailBox { // storage for Mail.

	private:
		map<int, Mail> mailMap;
		map<string, FromElem> fromMap;
		map<string, ToElem> toMap;
		AVLTree<int> charCountMap;

	public:
		void add(string& path);
		void remove(int target_id);
		void longest();
		void query(string& from, string& to, int* start, int* end, vector<char>& oprtor, vector<string>& keywords); 
		void AVLtrvs() { charCountMap.inorder_trvs(charCountMap.root); }
		~MailBox() { 
			mailMap.clear();
			fromMap.clear();
			toMap.clear();
			charCountMap.clear();
		}
};

// other function
bool dateComp(int* a, int* b);
int processInput( string& path, string& from, string& to, int* date, int& id, int& char_count, unordered_set<string>& keywords);
void processQuery(string& input, string& from, string& to, int* start, int* end , vector<char>& oprtor, vector<string>& keywords);

