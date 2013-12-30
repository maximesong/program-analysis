#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <fstream>

using namespace std;

class SourceCodeLocation {
public:
	SourceCodeLocation(int line = 0, int column = 0) : line(line), column(column) {}
	int line;
	int column;
};

class SourceCodeRange {
public:
	SourceCodeLocation start;
	SourceCodeLocation end;
	string filename;
	
	string getCode() 
	{
		ifstream in(filename);
		string content;
		string line;
		for (int i = 0; i != start.line; ++i) {
			getline(in, line);
		}
		line = line.substr(start.column - 1);
		for (int i = start.line; i != end.line; ++i) {
			content += line;
			getline(in, line);
		}
		line = line.substr(0, end.column + 1);
		content += line;
		return content;
	}

	string getLineCode() 
	{
		ifstream in(filename);
		string content;
		string line;
		for (int i = 0; i != start.line; ++i) {
			getline(in, line);
		}
		for (int i = start.line; i != end.line; ++i) {
			content += line;
			getline(in, line);
		}
		content += line;
		return content;
	}

	static SourceCodeRange parse(string startLocation, string endLocation) {
		SourceCodeRange range;
		vector<string> startInfo = split(startLocation);
		assert(startInfo.size() == 3);
		vector<string> endInfo = split(endLocation);
		assert(endInfo.size() == 3);
		range.filename = startInfo[0];
		range.start.line = stoi(startInfo[1]);
		range.start.column = stoi(startInfo[2]);
		range.end.line = stoi(endInfo[1]);
		range.end.column = stoi(endInfo[2]);
		return range;
	}

	static vector<string> split(string location) {
		int pos = 0;
		vector<string> result;
		while (pos != string::npos) {
			int last = location.find(":", pos);
			result.push_back(location.substr(pos, last));
			if (last != string::npos) {
				pos = last + 1;
			} else {
				pos = last;
			}
		}
		return result;
	}
};

#endif
