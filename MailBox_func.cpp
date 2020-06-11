#include "MailBox.hpp"
#include <fstream>
#include <sstream>
#include <cctype>

map<string, int> month2int;
unordered_set<int> ID_visited;
char op[5] = { '(', ')', '!', '&', '|' };

bool dateComp(int* a, int* b) { // is date a happen after date b ?
	if ( a[0] < b[0] ) return false;
	else if ( a[0] == b[0] ) {
		if ( a[1] < b[1] ) return false;
		else if ( a[1] == b[1] ) {
			if ( a[2] < b[2] ) return false;
			else if ( a[2] == b[2] ) {
				if ( a[3] < b[3] ) return false;
			}
		}
	}
	return true;	
}

int processInput( string& path, string& from, string& to, int* date, int& id, int& char_count, unordered_set<string>& words ) {
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
				word += tolower(tmp[i]);
				char_count++;
			}
			else if ( word.empty() == false ) {
				words.insert(word);
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
					word += tolower(line[i]);
					char_count++;
				}
				else if ( word.empty() == false ) {
					words.insert(word);
					word.clear();
				}
			}
		}
	}
	fin.close();
	return 1;
}

int getOperator(char c) {
	for ( int i = 0; i < 5; i++ ) {
		if ( c == op[i] ) return i; 
	}
	return -1;
}  
void processQuery(string& input, string& from, string& to, int* start, int* end , vector<char>& oprtor, vector<string>& keywords) {
	stringstream ss(input);		
	string token;
	while ( getline(ss, token, ' ') ) {
		if ( token[0] == '-' ) {
			if ( token[1] == 'f' ) {
				size_t pos = token.find_last_of('"');
				from = token.substr(3, pos-3);
			} else if ( token[1] == 't' ) {
				size_t pos = token.find_last_of('"');
				to = token.substr(3, pos-3);
			} else {
				size_t pos = token.find_first_of('~');
				if ( pos > 2 ) {
					start[0] = stoi(token.substr(2,4));
					start[1] = stoi(token.substr(6,2));
					start[2] = stoi(token.substr(8,2));
					start[3] = stoi(token.substr(10,4));
				}
				if ( token.size() - 1 > pos ) {
					token = token.substr(pos+1, 12);
					end[0] = stoi(token.substr(0,4));
					end[1] = stoi(token.substr(4,2));
					end[2] = stoi(token.substr(6,2));
					end[3] = stoi(token.substr(8,4));
				}
			}
		} else if ( isalnum(token[0]) ) {
			string word;
			int idx;
			for ( int i = 0; i < token.size(); i++ ) {
				if ( (idx = getOperator(token[i])) == -1 ) {
					word += token[i];
				} else if ( word.empty() == false ) {
					oprtor.push_back(tolower(op[idx]));
					keywords.push_back(word);
					word.clear();
				}
			}
			if ( word.empty() == false )
				keywords.push_back(word);
		}
	}
}

// MailBox function
void MailBox::add(string& path) {
	month2int["January"] = 1; month2int["February"] = 2; month2int["March"] = 3; month2int["April"] = 4; month2int["May"] = 5; month2int["June"] = 6; month2int["July"] = 7; month2int["August"] = 8; month2int["September"] = 9; month2int["October"] = 10; month2int["November"] = 11; month2int["December"] = 12;
	string from, to;
	int* date = (int*)malloc(sizeof(int)*4);
	int id, char_count = 0;
	unordered_set<string> words;
	if ( processInput(path, from, to, date, id, char_count, words) == 0 ) { // if mail already added
		printf("-\n");
	} else {
		ID_visited.insert(id);
		printf("%lu\n", ID_visited.size());

		// insert element.
		Mail mail(from, to, date, id, char_count, &words);
		mailMap.insert(pair<int, Mail>(id, mail));

		FromElem::IDElem f_ide(to, date, &words);
		auto fpos = fromMap.find(from);
		if ( fpos == fromMap.end() ) {
			FromElem fromElem;
			auto fp = fromMap.insert(pair<string, FromElem>(from, fromElem));
			fp.first->second.IDMap.insert(pair<int, FromElem::IDElem>(id, f_ide));
		} else {
			fpos->second.IDMap.insert(pair<int, FromElem::IDElem>(id, f_ide));
		}

		ToElem::IDElem t_ide(date, &words);
		auto tpos = toMap.find(to);
		if ( tpos == toMap.end() ) {
			ToElem toElem;
			auto tp = toMap.insert(pair<string, ToElem>(to, toElem));
			tp.first->second.IDMap.insert(pair<int, ToElem::IDElem>(id, t_ide));
		} else {
			tpos->second.IDMap.insert(pair<int, ToElem::IDElem>(id, t_ide));
		}

		charCountMap.insert(char_count, id);

		words.clear();
	}
}

void MailBox::remove(int target_id) {
	auto mail_pair = mailMap.find(target_id);
	if ( mail_pair == mailMap.end() ) // if not found
		printf("-\n");
	else {
		// mailMap
		mailMap.erase(mail_pair);
		// fromMap
		auto fpos = fromMap.find(mail_pair->second.from);
		fpos->second.IDMap.erase(target_id);
		// toMap
		auto tpos = toMap.find(mail_pair->second.to);
		tpos->second.IDMap.erase(target_id);
		// charCountMap
		charCountMap.erase(mail_pair->second.char_count, mail_pair->second.id);
		
		ID_visited.erase(target_id);
		printf("%lu\n", ID_visited.size());
	}
}

void MailBox::longest() {
	charCountMap.longest();
}

void MailBox::query(string& from, string& to, int* start, int* end, vector<char>& oprtor, vector<string>& keywords) {

	vector<int> id_matched;

	if ( from != "" ) { // if using '-f' flag
		auto fp = fromMap.find(from);
		if ( fp == fromMap.end() ) printf("-\n");
		else {
			auto pass = &(fp->second.IDMap);
			for ( auto p = pass->begin(); p != pass->end(); ++p ) {
				if ( to != "" && p->second.to != to ) continue; // "to" not matched	
				if ( start[0] != -1 && dateComp(p->second.date, start) == false ) continue; // "start" not matched
				if ( end[0] != -1 && dateComp(end, p->second.date) == false ) continue; // "end" not matched
			
			}
			if ( id_matched.empty() ) printf("-\n");
			else {

			}
		}
	} else if ( to != "" ) { // if using '-t' flag but no '-f' flag
		auto tp = toMap.find(to);
		if ( tp == toMap.end() ) printf("-\n");
		else {
			auto pass = &(tp->second.IDMap);
			for ( auto p = pass->begin(); p != pass->end(); ++p ) {
				if ( start[0] != -1 && dateComp(p->second.date, start) == false ) continue; // "start" not matched
				if ( end[0] != -1 && dateComp(end, p->second.date) == false ) continue; // "end" not matched

			}
			if ( id_matched.empty() ) printf("-\n");
			else {

			}
		}
	} else { // no '-f', '-t' flags are used
		for ( auto p = mailMap.begin(); p != mailMap.end(); ++p ) {
			if ( start[0] != -1 && dateComp(p->second.date, start) == false ) continue; // "start" not matched
			if ( end[0] != -1 && dateComp(end, p->second.date) == false ) continue; // "end" not matched
		}
		if ( id_matched.empty() ) printf("-\n");
		else {

		}
	}
}

// Element Function
void Mail::mailInfo() {
	printf("--------------------------------------------------\n");
	printf("                  Mail Info                       \n");
	printf("--------------------------------------------------\n");
	cout << "From      " << from << endl;
	cout << "To        " << to << endl;
	cout << "Date      " << date[0] << '/' << date[1] << '/' << date[2] << ' ' << date[3] << endl;
	cout << "Mail-id   " << id << endl;
	printf("--------------------------------------------------\n");
	printf("There are %d alphanumeric chracters in the mail.\n", char_count);
	printf("There are %lu keywords in the mail.\n", words->size());
	printf("--------------------------------------------------\n");
}
