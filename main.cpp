#include "mail.h"

MailBox mailbox; // the main storage for object Mail.

void execute(string operation) {
	if ( operation == "add" ) {
		string filePath;
		cin >> filePath;
		mailbox.add(filePath);	
	} else if ( operation == "remove" ) {
		int target_id;
		cin >> target_id;
		mailbox.remove(target_id);
	} else if ( operation == "longest" ) {
		mailbox.longest();
	} else {
		// the query operation.	
	}
}

int main(void) {
	string operation; 
	while ( std::getline(cin, operation) ) {
		execute(operation);	
	}
	return 0;
}
