#include "MailBox.hpp"
#include <fstream>
#include <sstream>
#include <cctype>

unordered_map<string, int> month2int;
char op[5] = { '(', ')', '!', '&', '|' };
unordered_map<string, vector<int>> kwordmap;
unordered_map<string, bitset<MAXMAILNUM>> bwords;

void processInput(string& path, string& from, string& to, int64_t& date_ll, int& id, int& char_count) {
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

				auto ff = bwords.find(word);
				if ( ff == bwords.end() ){ // word is not added yet
					bitset<MAXMAILNUM> temp;
					temp[id] = true;
					bwords.insert(pair<string, bitset<MAXMAILNUM>>(word, temp));
				} else {
					ff->second[id] = true;
				}

				word.clear();
			}
		}
		if ( word.empty() == false ) {

			auto ff = bwords.find(word);
			if ( ff == bwords.end() ){ // word is not added yet
				bitset<MAXMAILNUM> temp;
				temp[id] = true;
				bwords.insert(pair<string, bitset<MAXMAILNUM>>(word, temp));
			} else {
				ff->second[id] = true;
			}

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

					auto ff = bwords.find(word);
					if ( ff == bwords.end() ){ // word is not added yet
						bitset<MAXMAILNUM> temp;
						temp[id] = true;
						bwords.insert(pair<string, bitset<MAXMAILNUM>>(word, temp));
					} else {
						ff->second[id] = true;
					}

					word.clear();
				}
			}
			if ( word.empty() == false ) {

				auto ff = bwords.find(word);
				if (ff == bwords.end()){ // word is not added yet
					bitset<MAXMAILNUM> temp;
					temp[id] = true;
					bwords.insert(pair<string, bitset<MAXMAILNUM>>(word, temp));
				} else {
					ff->second[id] = true;
				}

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
			int idx = 0;
			for ( int i = 0; i < token.size(); i++ ) {
				/* cout << i << ": " << token[i] << endl; */ 
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

	// if ( IDState.find(id) == IDState.end() ) { // if mail is not in the box right now
	if ( idstate[id] == false ) { // if mail is not in the box right now
		int char_count = 0;
		if ( met[id] == false ) { // if mail hasn't been added

			string from, to;
			met[id] = true;	
			int64_t date;

			processInput(path, from, to, date, id, char_count);

			Mail mail(from, to, date, id, char_count);
			mailVec[id] = mail;

		} else {
			char_count = mailVec[id].char_count;
		}

		charCountMap.insert(char_count, id);
		
		idstate[id] = true;
		cout << idstate.count() << '\n';

	} else cout << "-\n"; 
}

void MailBox::remove(int target_id) {

	if ( idstate[target_id] == true ) {
		int char_count = mailVec[target_id].char_count;
		charCountMap.erase(char_count, target_id);
		idstate[target_id] = false;
		cout << idstate.count() << '\n';
	} else cout << "-\n";
}

void MailBox::longest() {
	charCountMap.longest();
}

void MailBox::query(string& from, string& to, int64_t& start, int64_t& end, vector<string>& split) {

	bitset<MAXMAILNUM> results;
	bsetexps(results, split);
	results &= idstate;

	if ( results.count() == 0 ) {
		cout << "-\n";
		return;
	} else {
		int c = results.count();
		bool first = true;
		for ( int i = 1; i < MAXMAILNUM && c != 0; i++ ) {
			if ( results[i] ){
				if ( from != "" && mailVec[i].from != from ) continue;
				if ( to != "" && mailVec[i].to  != to ) continue;
				if ( start != 0 && mailVec[i].date < start ) continue;
				if ( end != 0 && mailVec[i].date > end ) continue;

				if ( first ) {
					cout << i;
					c--;
					first = false;
				} else {
					cout << ' ' << i;
					c--;
				}
			}
		}
		if ( first ) cout << "-\n";
		else cout << '\n';
	}
	return;
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

void bsetcompute(stack<bitset<MAXMAILNUM>>& wordzz, string& oprtor) {
	bitset<MAXMAILNUM> temp;

	if ( oprtor == "!" ) {
		temp = ~(wordzz.top());
		wordzz.pop();
		wordzz.push(temp);
	} else if ( oprtor == "|" ) {
		bitset<MAXMAILNUM> temp2;
		temp |= wordzz.top(); wordzz.pop();
		temp2 |= wordzz.top(); wordzz.pop();
		wordzz.push(temp | temp2);
	} else if ( oprtor == "&" ) {
		bitset<MAXMAILNUM> temp2;
		temp |= wordzz.top(); wordzz.pop();
		temp2 |= wordzz.top(); wordzz.pop();
		wordzz.push(temp & temp2);
	}

	return;
}

void bsetexps(bitset<MAXMAILNUM>& results, vector<string>& split) {
	stack<string> oprtor;
	stack<bitset<MAXMAILNUM>> wordzz;
	for ( int i = 0; i < split.size(); ++i ) {
		if ( isalnum(split[i][0]) ) {
			auto ff = bwords.find(split[i]);
			if ( ff == bwords.end() ) { // if keyword not exist
				bitset<MAXMAILNUM> temp;
				wordzz.push(temp);
			} else {
				wordzz.push(bwords[split[i]]);
			}
		} else { // if is an operator
			if ( oprtor.empty() == true ) oprtor.push(split[i]);
			else {
				if ( split[i] == "(" ) oprtor.push("("); // '(' need to be done first
				else if ( split[i] == ")" ) {
					while ( oprtor.top() != "(" )  { // do every thing between '(', ')'
						bsetcompute(wordzz, oprtor.top());
						oprtor.pop();
					}
					oprtor.pop();
				} else { // operator !, &, |
					while ( oprtor.empty() == false && compOP(split[i], oprtor.top()) == true ) {
						bsetcompute(wordzz, oprtor.top());
						oprtor.pop();
					}
					oprtor.push(split[i]);
				}
			}
		}
	}
	while ( oprtor.empty() == false ) { // do the rest operator (neccesary?)
		bsetcompute(wordzz, oprtor.top());
		oprtor.pop();
	}
	results |= wordzz.top();
	return;
}
