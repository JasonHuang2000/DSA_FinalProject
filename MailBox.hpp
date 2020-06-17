#include <iostream>
#include <string>
#include <vector>
#include <map> // std::multiset (rb-tree)
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <algorithm>
#define MAXMAILNUM 10002

using namespace std;

class MailBox;
struct Mail;
struct FromElem;
struct ToElem;

struct Mail { // store the infomation of a mail.
	// variable
	string from;
	string to;
	int64_t date;
	int id;
	int char_count;
	// function
	Mail(string _from, string _to, int64_t _date, int _id, int _char_count) : from(_from), to(_to), date(_date), id(_id), char_count(_char_count){ }
	void mailInfo();
};

struct FromElem {
	// variable
	unordered_set<int> id;	
};
struct ToElem {
	// variable
	unordered_set<int> id;
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
		bool met[MAXMAILNUM] = {false};
		// the state within the box right now
		unordered_set<int> IDState;
		unordered_map<string, FromElem> fromState;
		unordered_map<string, ToElem> toState;
		// vector storing info by id
		vector<Mail> mailVec;
		vector<unordered_set<string>> wordsVec;

		AVLTree<int> charCountMap;

	public:
		MailBox() {
			mailVec.resize(MAXMAILNUM, Mail("", "", 0, 0, 0));
			wordsVec.resize(MAXMAILNUM); 
		}
		~MailBox() { 
			IDState.clear();
			fromState.clear();
			toState.clear();
			charCountMap.clear();
			wordsVec.clear();
		}
		void add(string& path);
		void remove(int target_id);
		void longest();
		void query(string& from, string& to, int64_t& start, int64_t& end, vector<string>& split); 

		// debug function
		void mapSize();
		void AVLtrvs() { charCountMap.inorder_trvs(charCountMap.root); }
		/* void FROMtrvs() { */
		/* 	for ( auto p = fromMap.begin(); p != fromMap.end(); ++p ) { */
		/* 		cout << p->first << " { "; */
		/* 		for ( auto i = p->second.IDMap.begin(); i != p->second.IDMap.end(); ++i ) */
		/* 			cout << i->first << ' '; */	
		/* 		cout << '}' << endl; */
		/* 	} */
		/* } */
		/* void TOtrvs() { */
		/* 	for ( auto p = toMap.begin(); p != toMap.end(); ++p ) { */
		/* 		cout << p->first << " { "; */
		/* 		for ( auto i = p->second.IDMap.begin(); i != p->second.IDMap.end(); ++i ) */
		/* 			cout << i->first << ' '; */	
		/* 		cout << '}' << endl; */
		/* 	} */
		/* } */
		/* void WORDtrvs() { */
		/* 	for ( int i = 0; i < wordsMap.size(); ++i ) { */
		/* 		if ( wordsMap[i].empty() == false ) { */
		/* 			printf("(%d) ", i); */
		/* 			for ( auto p = wordsMap[i].begin(); p != wordsMap[i].end(); ++p ) */ 
		/* 				cout << *p << ' '; */
		/* 			cout << endl; */
		/* 		} */
		/* 	} */
		/* } */
};

// other function
bool dateComp(int* a, int* b);
void processInput( string& path, string& from, string& to, int64_t& date, int& id, int& char_count, unordered_set<string>& keywords);
void processQuery(string& input, string& from, string& to, int64_t& start, int64_t& end, vector<string>& split);
bool exps(unordered_set<string>& words, vector<string>& split);

