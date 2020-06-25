#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <algorithm>
#define MAXMAILNUM 10002

using namespace std;

class MailBox;

struct Mail { // store the infomation of a mail.
	// variable
	int64_t date;
	int id;
	int char_count;
	string from;
	string to;
	string preview;
	string subject;
	string tag;
	// function
	Mail(string _from, string _to, int64_t _date, int _id, int _char_count, string preview, string subject) : date(_date), id(_id), char_count(_char_count), from(_from), to(_to), preview(preview), subject(subject) { }
	void mailInfo();
};

struct Tag {
	// variable
	string description;
	bitset<MAXMAILNUM> IDSet;	
	// function
	int size() { return (int)(IDSet.count()); }
	void addDescription(string& des) { description = des; }
	int64_t findOldest();
	int64_t findLatest();
};

template <class T>
class AVLTreeNode {
	public:
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

	public:
		MailBox() {
			mailVec.resize(MAXMAILNUM, Mail("", "", 0, 0, 0, "", ""));
		}
		~MailBox() { 
			mailVec.clear();
			charCountMap.clear();
		}
		void add(string& path, string& tag);
		void remove(int target_id);
		void longest();
		void query(string& from, string& to, int64_t& start, int64_t& end, vector<string>& split); 
		void mailInfo(int id) { 
			if ( idstate[id] == false ) printf("Mail %d does not exist.\n", id);
			else mailVec[id].mailInfo(); 
		}
		void tagInfo(string& tag);

		bool met[MAXMAILNUM] = {false};
		// the state within the box right now
		bitset<MAXMAILNUM> idstate;
		// tags
		unordered_map<string, Tag> tags;
		// vector storing info by id
		vector<Mail> mailVec;
		AVLTree<int> charCountMap;
};

// other function
void processAdd(string& path, string& tag);
void processInput(string& path, string& from, string& to, int64_t& date, int& id, int& char_count, string& preview, string& subject);
void processQuery(string& input, string& from, string& to, int64_t& start, int64_t& end, vector<string>& split);
void processTag(vector<int>& IDs, string& tag);
void bsetcompute(stack<string>& wordzz, string& oprtor);
void bsetexps(bitset<MAXMAILNUM>& results, vector<string>& split);
