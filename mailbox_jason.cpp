#include "mail.h"
#include <fstream>
#include <map>
#include <sstream>
#include <cctype>
#include <unordered_set>

map<string, int> month2int;
vector<string> keywords;
unordered_set<int> ID_visited;

int processInput( string& path, string& from, string& to, int* date, int& id, int& char_count, vector<string>& keywords ) {
	ifstream fin(path);
	if ( fin.is_open() ) {
		string line;
		// from
		getline(fin, line); 
		from = line.substr(6);
		// date
		getline(fin, line);
		stringstream ss(line);
		string token;
		for ( int i = 0; getline(ss, token, ' '); i++ ) {
			if ( i == 0 || i == 4 ) continue;
			else if ( i == 1 ) date[2] = stoi(token);
			else if ( i == 2 ) date[1] = month2int[token];
			else if ( i == 3 ) date[0] = stoi(token);
			else if ( i == 5 ) {
				size_t pos = token.find_first_of(":");
				date[3] = 100 * stoi(token.substr(0, pos)) + stoi(token.substr(pos+1));
			}
		}
		// id
		getline(fin, line);
		id = stoi(line.substr(12));
		if ( ID_visited.find(id) != ID_visited.end() ) return 0;
		// subject
		getline(fin, line);
		string tmp = line.substr(9);
		string word;
		for ( int i = 0; i < tmp.size(); i++ ) {
			if ( isalnum(tmp[i]) ) {
				word += tmp[i];
				char_count++;
			}
			else if ( word.empty() == false ) {
				keywords.push_back(word);
				word.clear();
			}
		}
		// to
		getline(fin, line);
		to = line.substr(4);
		// content
		getline(fin, line);
		while ( getline(fin, line) ) {
			for ( int i = 0; i < line.size(); i++ ) {
				if ( isalnum(line[i]) ) {
					word += line[i];
					char_count++;
				}
				else if ( word.empty() == false ) {
					keywords.push_back(word);
					word.clear();
				}
			}
		}
	}
	fin.close();
	return 1;
}

void MailBox::add(string path) {
	month2int["January"] = 1; month2int["February"] = 2; month2int["March"] = 3; month2int["April"] = 4; month2int["May"] = 5; month2int["June"] = 6; month2int["July"] = 7; month2int["August"] = 8; month2int["September"] = 9; month2int["October"] = 10; month2int["November"] = 11; month2int["December"] = 12;
	string from, to;
	int* date = (int*)malloc(sizeof(int)*4);
	int id, char_count = 0;
	if ( processInput(path, from, to, date, id, char_count, keywords) ) {
		ID_visited.insert(id);
		printf("%lu\n", ID_visited.size());
	 } else printf("-\n");
	
	// construct element.
	Mail mail(from, to, date, id, char_count, keywords);
	RbElem rbElem(id, char_count, from, date);	

	// add element.
	pair<string, Mail> h_pair (from, mail);
	hash_table.insert(h_pair);

	pair<int, RbElem> r_pair (id, rbElem);
	rb_tree.insert(r_pair);
	
	Lgst.insert(char_count, id);

	keywords.clear();
}

void MailBox::remove(int target_id) {
	auto r_pair = rb_tree.find(target_id);
	if ( r_pair == rb_tree.end() )
		printf("-\n");
	else {
		rb_tree.erase(r_pair);
		Lgst.remove(r_pair->second.char_count);
		for ( auto pos = hash_table.find(r_pair->second.from); pos != hash_table.end(); ++pos ) {
			if ( pos->second.id == r_pair->second.id ) {
				hash_table.erase(pos);
				break;
			}
		}
		ID_visited.erase(r_pair->second.id);
		printf("%lu\n", ID_visited.size());
	}
}

void MailBox::longest() {
	node* ptr = Lgst.MaxElem();
	if ( ptr == NULL )
		printf("-\n");
	else
		printf("%d\n", ptr->id);
}
