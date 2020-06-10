#include "MailBox.hpp"

MailBox mailbox; // the main storage for object Mail.
vector<char> oprtor;
vector<string> keywords;

void execute(string operation) {
	if ( operation == "add" ) {
		string filePath;
		cin >> filePath;
		/* cout << "add " << filePath << endl; */
		mailbox.add(filePath);	
	} else if ( operation == "remove" ) {
		int target_id;
		cin >> target_id;
		/* cout << "remove " << target_id << endl; */
		mailbox.remove(target_id);
	} else if ( operation == "longest" ) {
		/* cout << "longest" << endl; */
		mailbox.longest();
	} else {
		string input;
		string from = "", to = "";
		int start[4] = { -1, -1, -1, -1 };
		int end[4] = { -1, -1, -1, -1 };
		getline(cin, input);
		processQuery(input, from, to, start, end, oprtor, keywords);

		oprtor.clear();
		keywords.clear();
	}
}

int main(void) {
	string operation; 
	while ( cin >> operation ) {
		execute(operation);	
	}
	return 0;
}
