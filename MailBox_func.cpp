#include "MailBox.hpp"
#include <fstream>
#include <sstream>
#include <cctype>

map<string, int> month2int;
unordered_set<int> ID_visited;

bool dateComp(int* a, int* b) {
	if ( a[0] < b[0] ) return true;
	else if ( a[0] == b[0] ) {
		if ( a[1] < b[1] ) return true;
		else if ( a[1] == b[1] ) {
			if ( a[2] < b[2] ) return true;
			else if ( a[2] == b[2] ) {
				if ( a[3] < b[3] ) return true;
			}
		}
	}
	return false;	
}

int processInput( string& path, string& from, string& to, int* date, int& id, int& char_count, unordered_set<string>& keywords ) {
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
				keywords.insert(word);
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
					keywords.insert(word);
					word.clear();
				}
			}
		}
	}
	fin.close();
	return 1;
}

// MailBox function
void MailBox::add(string& path) {
	month2int["January"] = 1; month2int["February"] = 2; month2int["March"] = 3; month2int["April"] = 4; month2int["May"] = 5; month2int["June"] = 6; month2int["July"] = 7; month2int["August"] = 8; month2int["September"] = 9; month2int["October"] = 10; month2int["November"] = 11; month2int["December"] = 12;
	string from, to;
	int* date = (int*)malloc(sizeof(int)*4);
	int id, char_count = 0;
	unordered_set<string> keywords;
	if ( processInput(path, from, to, date, id, char_count, keywords) ) {
		ID_visited.insert(id);
		printf("%lu\n", ID_visited.size());
	 } else printf("-\n");
	
	// construct element.
	Mail mail(from, to, date, id, char_count, &keywords);
	FromElem fromElem(id, to, date);
	ToElem toElem(id, date);

	// add element.
	mailSet.insert(pair<int, Mail>(id, mail));
	auto fpos = fromSet.find(from);
	if ( fpos == fromSet.end() )
		fromSet.insert(pair<string, FromElem>(from, fromElem));
	else
		fpos->second.insert(&fromElem);
	auto tpos = toSet.find(to);
	if ( tpos == toSet.end() )
		toSet.insert(pair<string, ToElem>(to, toElem));
	else
		tpos->second.insert(&toElem);
	charCountSet.insert(char_count, id);

	keywords.clear();
}

void MailBox::remove(int target_id) {
	auto mail_pair = mailSet.find(target_id);
	if ( mail_pair == mailSet.end() )
		printf("-\n");
	else {
		mailSet.erase(mail_pair);
		auto fpos = fromSet.find(mail_pair->second.from);
		if ( fpos->second.next == NULL ) fromSet.erase(mail_pair->second.from);
		else fpos->second.erase(target_id);
		auto tpos = toSet.find(mail_pair->second.to);
		if ( tpos->second.next == NULL ) toSet.erase(mail_pair->second.to);
		else tpos->second.erase(target_id);
		charCountSet.erase(mail_pair->second.char_count, mail_pair->second.id);
		ID_visited.erase(target_id);
		printf("%lu\n", ID_visited.size());
	}
}

void MailBox::longest() {
	charCountSet.longest();
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
	printf("There are %lu keywords in the mail.\n", keywords->size());
	printf("--------------------------------------------------\n");
}

void FromElem::insert(FromElem* fe) {
	FromElem* cur = this;
	while ( cur != NULL ) {
		cur = cur->next;
	}
	cur = fe;
}
void FromElem::erase(int id) {
	FromElem* cur = this;
	FromElem* prev = this;
	while ( cur->id != id ) {
		prev = cur;
		cur = cur->next;
	}
	if ( cur == prev ) {
		cur->id = cur->next->id;
		cur->to = cur->next->to;
		cur->date = cur->next->date;
		cur->next = cur->next->next;
	} else prev->next = cur->next;
	delete cur;	
}

void ToElem::insert(ToElem* te) {
	ToElem* cur = this;
	while ( cur != NULL ) {
		cur = cur->next;
	}
	cur = te;
}
void ToElem::erase(int id) {
	ToElem* cur = this;
	ToElem* prev = this;
	while ( cur->id != id ) {
		prev = cur;
		cur = cur->next;
	}
	if ( cur == prev ) {
		cur->id = cur->next->id;
		cur->date = cur->next->date;
		cur->next = cur->next->next;
	} else prev->next = cur->next;
	delete cur;	
}
