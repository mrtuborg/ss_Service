using namespace std;

class cronTaskMeta {
public:
	cronTaskMeta();
	~cronTaskMeta();

	void Init(unsigned long Id,
			  unsigned long nextId,
			  unsigned length_sec);

	size_t Size();



	friend ostream &operator<<(ostream &stream, const cronTaskMeta &meta);
	friend istream &operator>>(istream &stream, cronTaskMeta &meta);

	friend fstream &operator<<(fstream &stream, const cronTaskMeta &meta);
	//friend ifstream &operator>>(ifstream &stream, cronTaskMeta &meta);

private:
	unsigned long objId;
	unsigned long nextObjId;
	unsigned length_sec;
};

class cronTask{
public:
	cronTask(std::string &textLine);

	cronTask(unsigned hh,
			 unsigned mm,
			 unsigned mday,
			 unsigned month,
			 unsigned wday,
			 unsigned long objId,
			 unsigned long nextObjId,
			 unsigned length_sec,
			 std::string &command);

	~cronTask();

	friend ostream &operator<<(ostream &stream, const cronTask &task);
	friend istream &operator>>(istream &stream, cronTask &task);

	friend fstream &operator<<(fstream &stream, const cronTask &task);
	//friend ifstream &operator>>(ifstream &stream, cronTask &task);


private:
		cronTaskMeta meta;
		int hour, minute, mday, month, wday;
		string command;
};
