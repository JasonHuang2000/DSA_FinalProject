#include <iostream>
#include <string>
#include <vector>

class Mail {
	pulic:
		Mail(string _from, string _to, string _date, int _id, string _subject) : from(_from), to(_to), date(_date), id(_id), subject(_subject);
		add(string path);
		remove(int target_id);
		longest();
		setContent(string readin); // split the original content into clean-words. (also set char_count)

	private:
		string from;
		string to;
		string date;
		int id;
		string subject;
		vector<string> content;
		int char_count;
};
