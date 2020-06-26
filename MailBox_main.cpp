#include "MailBox.hpp"

MailBox mailbox; // the main storage for object Mail.
vector<string> split;

int main(void) {
    ios_base::sync_with_stdio(false);
	string operation; 
	while ( cin >> operation ) {
		if ( operation == "add" ) {
			string filePath, tag;
			processAdd(filePath, tag);	
			if ( filePath == "all" ) {
				for ( int i = 1; i <= 10000; ++i ) {
					filePath = "MailData/mail" + to_string(i);
					mailbox.add(filePath, tag);
				}
			 } else if ( filePath == "half" ) {
				 for ( int i = 1; i <= 5000; ++i ) {
					filePath = "MailData/mail" + to_string(i);
					mailbox.add(filePath, tag);
				 }
			 } else mailbox.add(filePath, tag);	
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
			vector<int> IDs;
			string tag, description;
			processTag(IDs, tag, description);
			if ( IDs.size() != 0 ) {
				for ( int i = 0; i < IDs.size(); i++ )
					mailbox.mailInfo(IDs[i]);
			} else if ( tag != "" ) {
				mailbox.tagInfo(tag);
			} else {
				puts("[info usage]");
				puts("$ info [id] [id] ...  [id] := positive integer");
				puts("$ info [tag name]     [tag name] := alphabetic string");
			}
		}  else if ( operation == "tag" ) {
			vector<int> IDs;
			string tag, description;
			processTag(IDs, tag, description);
			if ( IDs.size() != 0 ) {
				mailbox.addTag(IDs, tag);
			} else if ( description != "" ) {
				mailbox.addtagDescription(tag, description);
			}
		} else if ( operation == "google" ) {
		    int id;
			cin >> id;
			mailbox.google(id);
		}
	}
	return 0;
}
