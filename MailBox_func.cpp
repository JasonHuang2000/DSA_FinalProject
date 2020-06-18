#include "MailBox.hpp"
#include <fstream>
#include <sstream>
#include <cctype>

map<string, int> month2int;
char op[5] = { '(', ')', '!', '&', '|' };

bool dateComp(int* a, int* b) { // is date a happen after/simultaneously date b ?
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

void processInput( string& path, string& from, string& to, int64_t& date_ll, int& id, int& char_count, unordered_set<string>& words ) {
	ifstream fin(path);
	if ( fin.is_open() == true ) {
		string line = "";

		// from
		getline(fin, line); 
		from = line.substr(6);
		for ( int i = 0; i < from.size(); i++ ) from[i] = tolower(from[i]);

		// date
		getline(fin, line);
		stringstream ss(line);
		string token = "";
		int date[4];
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
		date_ll = (int64_t)date[0]*100000000 + date[1]*1000000 + date[2]*10000 + date[3];

		// id
		getline(fin, line);
		id = stoi(line.substr(12));

		// subject
		getline(fin, line);
		string tmp = line.substr(9);
		string word = "";
		for ( int i = 0; i < tmp.size(); i++ ) {
			if ( isalnum(tmp[i]) ) {
				word += tolower(tmp[i]);
			}
			else if ( word.empty() == false ) {
				words.insert(word);
				word.clear();
			}
		}
		if ( word.empty() == false ) {
			words.insert(word);
			word.clear();
		}

		// to
		getline(fin, line);
		to = line.substr(4);
		for ( int i = 0; i < to.size(); i++ ) to[i] = tolower(to[i]);

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
			if ( word.empty() == false ) {
				words.insert(word);
				word.clear();
			}
		}
	}
	fin.close();
}

int getOperator(char c) { // if operator, return the index of op; else, return -1
	for ( int i = 0; i < 5; i++ ) {
		if ( c == op[i] ) return i; 
	}
	return -1;
}  

void processQuery(string& input, string& from, string& to, int64_t& start, int64_t& end, vector<string>& split) {
	stringstream ss(input);		
	string token;
	getline(ss, token, ' ');
	while ( getline(ss, token, ' ') ) {
		if ( token[0] == '-' ) { // flag
			if ( token[1] == 'f' ) { // -f flag

				size_t pos = token.find_last_of('"');
				from = token.substr(3, pos-3);
				for ( int i = 0; i < from.size(); i++ ) from[i] = tolower(from[i]);

			} else if ( token[1] == 't' ) { // -t flag

				size_t pos = token.find_last_of('"');
				to = token.substr(3, pos-3);
				for ( int i = 0; i < to.size(); i++ ) to[i] = tolower(to[i]);

			} else { // -d flag

				size_t pos = token.find_first_of('~');
				if ( pos > 2 ) {
					start = stoll(token.substr(2,12));
				}
				if ( token.size() - 1 > pos ) {
					end = stoll(token.substr(pos+1,12));
				}
			}
		} else { // expression

			string word = "";
			int idx;
			for ( int i = 0; i < token.size(); i++ ) {
				if ( (idx = getOperator(token[i])) == -1 ) { // part of a word
					word += tolower(token[i]);
				} else { // we met an operator
					if ( word.empty() == false ) {
						split.push_back(word);
						word.clear();
					}
					switch (idx) { // QQ
						case 0: split.push_back("("); break;
						case 1: split.push_back(")"); break;
						case 2: split.push_back("!"); break;
						case 3: split.push_back("&"); break;
						case 4: split.push_back("|"); break;
					}
				}
			}

			if ( word.empty() == false ) { // push the rest word if there is any
				split.push_back(word);
				word.clear();
			}
		}
	}
}

// MailBox function
void MailBox::add(string& path) {
	if ( month2int.empty() ) { month2int["January"] = 1; month2int["February"] = 2; month2int["March"] = 3; month2int["April"] = 4; month2int["May"] = 5; month2int["June"] = 6; month2int["July"] = 7; month2int["August"] = 8; month2int["September"] = 9; month2int["October"] = 10; month2int["November"] = 11; month2int["December"] = 12; }

	int id = 0;
	for ( int i = 0; i < path.size(); i++ ) {
		if ( isdigit(path[i]) ) {
			id = stoi(path.substr(i));
			break;
		}
	}

	if ( IDState.find(id) == IDState.end() ) { // if mail is not in the box right now
		int char_count = 0;
		string from, to;
		if ( met[id] == false ) { // if mail hasn't been added

			met[id] = true;	
			int64_t date;
			unordered_set<string> _words;

			processInput(path, from, to, date, id, char_count, _words);

			Mail mail(from, to, date, id, char_count);
			mailVec[id] = mail;
			wordsVec[id] = _words;
			_words.clear();

		} else {
			char_count = mailVec[id].char_count;
			from = mailVec[id].from;
			to = mailVec[id].to;
		}

		charCountMap.insert(char_count, id);
		
		auto fpos = fromState.find(from);
		if ( fpos == fromState.end() ) {
			FromElem fromElem;
			fromElem.id.insert(id);
			fromState.insert(pair<string, FromElem>(from, fromElem));
		} else fpos->second.id.insert(id);

		auto tpos = toState.find(to);
		if ( tpos == toState.end() ) {
			ToElem toElem;
			toElem.id.insert(id);
			toState.insert(pair<string, ToElem>(to, toElem));
		} else tpos->second.id.insert(id);

		IDState.insert(id);
		printf("%lu\n", IDState.size());

	} else printf("-\n"); 
}

void MailBox::remove(int target_id) {
	auto IDptr = IDState.find(target_id);
	if ( IDptr != IDState.end() ) { // the [target_id] mail is in the box right now
		int char_count = mailVec[target_id].char_count;
		string from = mailVec[target_id].from;
		string to = mailVec[target_id].to;

		charCountMap.erase(char_count, target_id);

		auto fpos = fromState.find(from);
		fpos->second.id.erase(target_id);
		if ( fpos->second.id.empty() == true ) fromState.erase(fpos);

		auto tpos = toState.find(to);
		tpos->second.id.erase(target_id);
		if ( tpos->second.id.empty() == true ) toState.erase(tpos);

		IDState.erase(target_id);
		printf("%lu\n", IDState.size());
	} else printf("-\n");
}

void MailBox::longest() {
	charCountMap.longest();
}

void MailBox::query(string& from, string& to, int64_t& start, int64_t& end, vector<string>& split) {

	vector<int> id_matched;

	// query debug
	/* cout << "---------------------" << endl; */
	/* cout << "From: " << from << endl; */
	/* cout << "To: " << to << endl; */
	/* cout << "Start: " << start << endl; */
	/* cout << "End: " << end << endl; */
	/* cout << "---------------------" << endl; */

	if ( from != "" ) { // if using '-f' flag

		auto fp = fromState.find(from);
		if ( fp == fromState.end() ) printf("-\n");
		else {
			auto pass = &(fp->second.id);
			for ( auto p = pass->begin(); p != pass->end(); ++p ) {
				if ( to != "" && mailVec[*p].to != to ) continue; // "to" not matched	
				if ( start != 0 && mailVec[*p].date < start ) continue; // "start" not matched
				if ( end != 0 && end <  mailVec[*p].date ) continue; // "end" not matched
				if ( exps(wordsVec[*p], split) ) id_matched.push_back(*p);
			}
			if ( id_matched.empty() ) printf("-\n");
			else {
				/* sort(id_matched.begin(), id_matched.end()); */
				int s = id_matched.size();
				for ( int i = 0; i < s; i++ ) {
					printf("%d%c", id_matched[i], i == s - 1 ? '\n' : ' ' );
				}
			}
		}
	} else if ( to != "" ) { // if using '-t' flag but no '-f' flag

		auto tp = toState.find(to);
		if ( tp == toState.end() ) printf("-\n");
		else {
			auto pass = &(tp->second.id);
			for ( auto p = pass->begin(); p != pass->end(); ++p ) {
				if ( start != 0 && mailVec[*p].date < start ) continue; // "start" not matched
				if ( end != 0 && end <  mailVec[*p].date ) continue; // "end" not matched
				if ( exps(wordsVec[*p], split) ) id_matched.push_back(*p);
			}
			if ( id_matched.empty() ) printf("-\n");
			else {
				/* sort(id_matched.begin(), id_matched.end()); */
				int s = id_matched.size();
				for ( int i = 0; i < s; i++ ) {
					printf("%d%c", id_matched[i], i == s - 1 ? '\n' : ' ' );
				}
			}
		}
	} else { // no '-f', '-t' flags are used

		for ( auto p = IDState.begin(); p != IDState.end(); ++p ) {
			if ( start != 0 && mailVec[*p].date < start ) continue; // "start" not matched
			if ( end != 0 && end < mailVec[*p].date ) continue; // "end" not matched
			if ( exps(wordsVec[*p], split) ) id_matched.push_back(*p);
		}
		if ( id_matched.empty() ) printf("-\n");
		else {
			/* sort(id_matched.begin(), id_matched.end()); */
			int s = id_matched.size();
			for ( int i = 0; i < s; i++ ) {
				printf("%d%c", id_matched[i], i == s - 1 ? '\n' : ' ' );
			}
		}
	}
	id_matched.clear();
}

// Element Function
void Mail::mailInfo() {
	printf("--------------------------------------------------\n");
	printf("                  Mail Info                       \n");
	printf("--------------------------------------------------\n");
	cout << "From      " << from << endl;
	cout << "To        " << to << endl;
	cout << "Date      " << date << endl;
	cout << "Mail-id   " << id << endl;
	printf("--------------------------------------------------\n");
	printf("There are %d alphanumeric chracters in the mail.\n", char_count);
	printf("--------------------------------------------------\n");
}

void compute(stack<bool>& result, string oprtor) { // compute the result according to the operator
	if ( oprtor == "!" ) {
		bool operand = result.top();
		result.pop();
		result.push(!operand);
	} else if ( oprtor == "|" ) {
		bool operand1 = result.top(); result.pop();
		bool operand2 = result.top(); result.pop();
		result.push(operand1 || operand2);
	} else if ( oprtor == "&" ) {
		bool operand1 = result.top(); result.pop();
		bool operand2 = result.top(); result.pop();
		result.push(operand1 && operand2);
	}
}

string precedence[3] = { "|", "&", "!" };
bool compOP(string cur, string last) { // compare the precedence of cur(on-hand) and last(top of the stack)
	if ( last == "(" ) return false;
	int curi, lasti;
	for ( int i = 0; i < 3; i++ ) {
		if ( cur == precedence[i] ) curi = i;
		if ( last == precedence[i] ) lasti = i;
	}
	return curi <= lasti;
}

bool exps(unordered_set<string>& words, vector<string>& split) {
	stack<bool> result;
	stack<string> oprtor;
	for ( int i = 0; i < split.size(); ++i ) {
		if ( isalnum(split[i][0]) ) { // if keyword
			if ( words.find(split[i]) == words.end() ) result.push(false); 
			else result.push(true);
		} else { // if operator
			if ( oprtor.empty() == true ) oprtor.push(split[i]); // the stack is empty
			else {
				if ( split[i] == "(" ) oprtor.push("("); // '(' need to be done first
				else if ( split[i] == ")" ) { 
					while ( oprtor.top() != "(" )  { // do every thing between '(', ')'
						compute(result, oprtor.top());
						oprtor.pop();
					}
					oprtor.pop();
				} else { // operator !, &, |
					while ( oprtor.empty() == false && compOP(split[i], oprtor.top()) == true ) {
						compute(result, oprtor.top());
						oprtor.pop();
					}
					oprtor.push(split[i]);
				}
			}
		}
	}
	while ( oprtor.empty() == false ) { // do the rest operator (neccesary?)
		compute(result, oprtor.top());
		oprtor.pop();
	}
	return result.top();
}

void MailBox::mapSize() {
	printf("sizeof MailMap: %lu\n", mailVec.size());
	int count = 0;
	for ( auto p = fromState.begin(); p != fromState.end(); ++p ) count += p->second.id.size(); 
	printf("sizeof FromMap: %lu, %d\n", fromState.size(), count); 
	count = 0;
	for ( auto p = toState.begin(); p != toState.end(); ++p ) count += p->second.id.size(); 
	printf("sizeof ToMap: %lu, %d\n", toState.size(), count); 
	printf("sizeof CharCountMap: %d\n", charCountMap.size());
}
