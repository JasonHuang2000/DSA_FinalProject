#include "MailBox.hpp"

MailBox mailbox; // the main storage for object Mail.
vector<string> split;

int main(void) {
    ios_base::sync_with_stdio(false);
	string operation; 
	while ( cin >> operation ) {
		if ( operation == "add" ) {
			string filePath;
			cin >> filePath;
			if ( filePath == "all" ) {
				for ( int i = 1; i <= 10000; ++i ) {
					filePath = "MailData/mail" + to_string(i);
					mailbox.add(filePath);
				}
			 } else if ( filePath == "half" ) {
				 for ( int i = 1; i <= 5000; ++i ) {
					filePath = "MailData/mail" + to_string(i);
					mailbox.add(filePath);
				 }
			 } else mailbox.add(filePath);	
		} else if ( operation == "remove" ) {
			int target_id;
			cin >> target_id;
			/* cout << "remove " << target_id << endl; */
			mailbox.remove(target_id);
		} else if ( operation == "longest" ) {
			/* cout << "longest" << endl; */
			mailbox.longest();
		} else if ( operation == "query" ) {
			string input;
			string from = "", to = "";
			int64_t start = 0, end = 0;
			getline(cin, input);
			processQuery(input, from, to, start, end, split);
			mailbox.query(from, to, start, end, split);
			split.clear();
		} else if ( operation == "info" ) {
			string token;
			while ( cin >> token ) {
				if ( isdigit(token[0]) ) {
					mailbox.mailInfo(stoi(token));
				}
			}
		}
	}
	return 0;
}
