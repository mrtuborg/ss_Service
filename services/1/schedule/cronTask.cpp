#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include "cronTask.h"

using namespace std;


void Tokenize(const  string & str,
              vector<string>& tokens,
              const  string & delimiters = " ")
{
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}


cronTaskMeta::cronTaskMeta()
{

}

cronTaskMeta::~cronTaskMeta()
{

}

void cronTaskMeta::Init(unsigned long id, unsigned long nextId, unsigned length_sec)
{
	  this->objId		= id;
	  this->nextObjId	= nextId;
	  this->length_sec	= length_sec;
}


size_t cronTaskMeta::Size()
{
	return sizeof(objId)+sizeof(nextObjId)+sizeof(length_sec);
}

cronTask::cronTask(unsigned mm, unsigned hh, unsigned mday, unsigned month, unsigned wday, unsigned long objId, unsigned long nextObjId, unsigned length_sec, std::string &command):
 hour (hh), minute(mm), mday(mday), month(month), wday(wday), command(command)
{
	meta.Init(objId, nextObjId, length_sec);
}

cronTask::cronTask(std::string &textLine)
{
	vector<string> tokens;
	vector<string>::const_iterator iter;
	for (iter=tokens.begin(); iter < tokens.end(); iter++) cout << *iter << endl;

	Tokenize(textLine, tokens, "\t#,");

	minute	= atoi(tokens[0].data());
	hour		= atoi(tokens[1].data());
	mday		= atoi(tokens[2].data());
	month	= atoi(tokens[3].data());
	wday		= atoi(tokens[4].data());
	command = tokens[5];

	meta.Init(atol(tokens[6].data()), atol(tokens[7].data()), atoi(tokens[8].data()));

}

cronTask::~cronTask()
{

}

ostream &operator<<(ostream &stream, const cronTaskMeta &meta)
{
		stream << "Operation id = " << meta.objId << ", Next operation id = " << meta.nextObjId << ", operation lasts about " << meta.length_sec << " seconds.";
		return stream;
}

ostream &operator<<(ostream &stream, const cronTask &task)
{
		stream << "-------"   << endl;
		stream << task.hour   << ":" << task.minute << " on " << task.mday << "/" << task.month << " [" << task.wday << "]" << endl;
		stream << "meta: "    << task.meta << endl;
		stream << "command: " << task.command << endl;
		stream << "-------"   << endl;

		return stream;
}

istream &operator>>(istream &stream, cronTaskMeta &meta)
{
		stream >> meta.objId >> meta.nextObjId >> meta.length_sec;
		return stream;
}

istream &operator>>(istream &stream, cronTask &task)
{
		stream >> task.hour >> task.minute >> task.mday >> task.month >> task.wday >> task.meta >> task.command;
		return stream;
}

fstream &operator<<(fstream &stream, const cronTaskMeta &meta)
{
		stream << meta.objId << "," << meta.nextObjId << "," << meta.length_sec;
		return stream;
}

fstream &operator<<(fstream &stream, const cronTask &task)
{
		stream << task.hour << "\t" << task.minute << "\t" << task.mday << " \t"<< task.month << "\t*\t" << task.command << "#";
		stream << task.meta;
		return stream;
}
