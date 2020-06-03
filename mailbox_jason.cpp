#include "mail.h"
#include <fstream>
#include <map>
#include <sstream>
#include <cctype>

map<string, int> month2int;
vector<string> keywords;

void processInput( string& path, string& from, string& to, int* date, int& id, int& char_count, vector<string>& keywords ) {
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
}

void MailBox::add(string path) {
	month2int["Jan"] = 1; month2int["Feb"] = 2; month2int["Mar"] = 3; month2int["Apr"] = 4; month2int["May"] = 5; month2int["Jun"] = 6; month2int["Jul"] = 7; month2int["Aug"] = 8; month2int["Sep"] = 9; month2int["Oct"] = 10; month2int["Nov"] = 11; month2int["Dec"] = 12;
	string from, to;
	int* date = (int*)malloc(sizeof(int)*4);
	int id, char_count = 0;
	processInput(path, from, to, date, id, char_count, keywords);
	
	// construct element.
	Mail mail(from, to, date, id, char_count, keywords);
	RbElem rbElem(id, char_count, from, date);	

	// add element.
	hash_table.insert({from, mail});
	hash_table.find(from)->second.mailInfo();
	rb_tree.insert(rbElem);
	rb_tree.find

	keywords.clear();
}
