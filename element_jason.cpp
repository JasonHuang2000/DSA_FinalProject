#include "mail.h"

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
	printf("There are %lu keywords in the mail.\n", keywords.size());
	printf("--------------------------------------------------\n");
}

void RbElem::rbElemInfo() { 
	printf("--------------------------------------------------\n");
	printf("                 RbElem Info                      \n");
	printf("--------------------------------------------------\n");
	cout << "From      " << from << endl;
	cout << "Date      " << date[0] << '/' << date[1] << '/' << date[2] << ' ' << date[3] << endl;
	cout << "Mail-id   " << id << endl;
	printf("--------------------------------------------------\n");
	printf("There are %d alphanumeric chracters in the mail.\n", char_count);
	printf("--------------------------------------------------\n");
}

bool heap_comp(HeapElem& hp1, HeapElem& hp2) { return hp1.char_count < hp2.char_count; }
