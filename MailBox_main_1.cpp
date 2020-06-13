#include "MailBox.hpp"

/*
Mail addMail(string path){
    map<string, int> month2int;
    month2int["January"] = 1; month2int["February"] = 2; month2int["March"] = 3; month2int["April"] = 4; month2int["May"] = 5; month2int["June"] = 6; month2int["July"] = 7; month2int["August"] = 8; month2int["September"] = 9; month2int["October"] = 10; month2int["November"] = 11; month2int["December"] = 12;
    string from, to;
	int* date = (int*)malloc(sizeof(int)*4);
	int id = 0;
	int char_count = 0;
	unordered_set<string> _words;

    processInput(path, from, to, date, id, char_count, _words);

    Mail mail(path, from, to, date, id, char_count);
    mail.kwords = _words;

    return mail;
}
*/


int main(void){
    map<int, Mail> mails;
    unordered_set<string> pathed;
    map<string, int> month2int;
    month2int["January"] = 1; month2int["February"] = 2; month2int["March"] = 3; month2int["April"] = 4; month2int["May"] = 5; month2int["June"] = 6;
    month2int["July"] = 7; month2int["August"] = 8; month2int["September"] = 9; month2int["October"] = 10; month2int["November"] = 11; month2int["December"] = 12;

    string operation; 
	while ( cin >> operation ) {
		if ( operation == "add" ) {
			string filePath;
			cin >> filePath;
            if (pathed.find(filePath) == pathed.end()) {
                pathed.insert(filePath);
                cout << pathed.size() << '\n';
			    // Mail m;
                // m = addMail(filePath);
                // mails[m.id] = m;

                string from, to;
                int* date = (int*)malloc(sizeof(int)*4);
                int id = 0;
                int char_count = 0;
                unordered_set<string> _words;

                processInput(filePath, from, to, date, id, char_count, _words);

                Mail mail(filePath, from, to, date, id, char_count);
                mail.kwords = _words;

                mails.insert(pair<int, Mail>(id, mail));

                // mail.mailInfo();

            } else {
                cout << "-\n";
            }
		} else if ( operation == "remove" ) {
			int target_id;
			cin >> target_id;
            map<int, Mail>::iterator m = mails.find(target_id);
            if (mails.find(target_id) == mails.end()) {
                cout << "-\n";
            } else {
                pathed.erase(m->second.path);
                mails.erase(target_id);
                cout << pathed.size() << '\n';
            }
		} else if ( operation == "longest" ) {
            if (mails.size() == 0){
                cout << "-\n";
            } else {
                int len = 0, lid;
                for (map<int, Mail>::const_iterator mlz = mails.begin(); mlz != mails.end(); ++mlz) {
                    if (mlz->second.char_count > len) {
                        len = mlz->second.char_count;
                        lid = mlz->first;
                    }
                }
                cout << lid << ' ' << len << '\n';
            }
		} else if ( operation == "query" ) {
			string input;
			string from = "", to = "";
			int start[4] = { -1, -1, -1, -1 };
			int end[4] = { -1, -1, -1, -1 };
            vector<string> split;
			getline(cin, input);
			processQuery(input, from, to, start, end, split);

            bool first = true;
            for (map<int, Mail>::iterator mlz = mails.begin(); mlz != mails.end(); ++mlz) {
                if (from != "") //9651
                    if (mlz->second.from != from) continue;
                if (to != "")
                    if (mlz->second.to != to) continue;
                if (start[0] != -1)
                    if (dateComp(mlz->second.date, start) == false) continue;
                if (end[0] != -1)
                    if (dateComp(end, mlz->second.date) == false) continue;
                if (exps(mlz->second.kwords, split) == false) continue;
                if (first) {
                    first = false;
                    cout << mlz->first;
                } else {
                    cout << ' ' << mlz->first;
                }
            }
            if (first == true) cout << "-\n";
            else cout << '\n';
		}
	}
    return 0;
}