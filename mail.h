#include <iostream>
#include <string>
#include <vector>

class Mail {
	public:
		Mail(std::string _from, std::string _to, std::string _date, int _id, std::string _subject) : from(_from), to(_to), date(_date), id(_id), subject(_subject);
		add(std::string path);
		remove(int target_id);
		longest();
		setContent(std::string readin); // split the original content into clean-words. (also set char_count)

	private:
		std::string from;
		std::string to;
		std::string date;
		int id;
		std::string subject;
		std::vector<std::string> content;
		int char_count;
};
