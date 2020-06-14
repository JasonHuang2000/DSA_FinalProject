#include <iostream>
#include <string>
#include <vector>
#include <map> // std::multiset (rb-tree)
#include <unordered_set>
#include <stack>
#include <algorithm>
#include <stack>
#define MAXMAILNUM 100001

using namespace std;

class MailBox;
struct Mail;
struct FromElem;
struct ToElem;

struct Mail { // store the infomation of a mail.
	// variable
	string path;
	string from;
	string to;
	int* date;
	int id;
	int char_count;
	// function
	Mail(string path, string _from, string _to, int* _date, int _id, int _char_count) : path(path), from(_from), to(_to), date(_date), id(_id), char_count(_char_count){ }
	void mailInfo();
};

struct FromElem {
	// variable
	struct IDElem {
		string to;
		int* date;	
		IDElem(string to, int* date) : to(to), date(date) {}
	};
	map<int, IDElem> IDMap;	
};
struct ToElem {
	// variable
	struct IDElem {
		int* date;	
		IDElem(int* date) : date(date) {}
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
		vector<unordered_set<string>> wordsMap;

	public:
		MailBox() { wordsMap.resize(MAXMAILNUM); }
		~MailBox() { 
			mailMap.clear();
			fromMap.clear();
			toMap.clear();
			charCountMap.clear();
			wordsMap.clear();
		}
		void add(string& path);
		void remove(int target_id);
		void longest();
		void query(string& from, string& to, int* start, int* end, vector<string>& split); 

		// debug function
		void mapSize();
		void AVLtrvs() { charCountMap.inorder_trvs(charCountMap.root); }
		void FROMtrvs() {
			for ( auto p = fromMap.begin(); p != fromMap.end(); ++p ) {
				cout << p->first << " { ";
				for ( auto i = p->second.IDMap.begin(); i != p->second.IDMap.end(); ++i )
					cout << i->first << ' ';	
				cout << '}' << endl;
			}
		}
		void TOtrvs() {
			for ( auto p = toMap.begin(); p != toMap.end(); ++p ) {
				cout << p->first << " { ";
				for ( auto i = p->second.IDMap.begin(); i != p->second.IDMap.end(); ++i )
					cout << i->first << ' ';	
				cout << '}' << endl;
			}
		}
		void WORDtrvs() {
			for ( int i = 0; i < wordsMap.size(); ++i ) {
				if ( wordsMap[i].empty() == false ) {
					printf("(%d) ", i);
					for ( auto p = wordsMap[i].begin(); p != wordsMap[i].end(); ++p ) 
						cout << *p << ' ';
					cout << endl;
				}
			}
		}
};

// other function
bool dateComp(int* a, int* b);
void processInput( string& path, string& from, string& to, int* date, int& id, int& char_count, unordered_set<string>& keywords);
void processQuery(string& input, string& from, string& to, int* start, int* end , vector<string>& split);
bool exps(unordered_set<string>& words, vector<string>& split);

