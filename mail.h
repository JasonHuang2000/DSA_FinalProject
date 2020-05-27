#include <iostream>
#include <string>
#include <vector>
using std::string;
using std::vector;

class MailBox;
class Mail;

class MailBox : public Mail {

	public:
		MailBox() : database(0); 
		void add(string path);
		void remove(int target_id);
		void longest();
	
	private:
		vector<Mail> database;
};

class Mail {

	public:
		Mail(string _from, string _to, string _date, int _id, string _subject) : from(_from), to(_to), date(_date), id(_id), subject(_subject);
		void setContent(string readin); // split the original content into clean-words. (also set char_count)

	protected:
		string from;
		string to;
		string date;
		string subject;
		int id;
		int char_count;
		vector<string> content;
};
