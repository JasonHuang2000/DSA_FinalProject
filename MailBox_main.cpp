#include "MailBox.hpp"

MailBox mailbox; // the main storage for object Mail.

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
		// the query operation.	
	}
}

int main(void) {
	string operation; 
	while ( cin >> operation ) {
		execute(operation);	
	}
	return 0;
}
