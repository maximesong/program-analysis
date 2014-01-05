#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <fstream>
#include <vector>
#include <assert.h>

using namespace std;

class SourceCodeLocation {
public:
	SourceCodeLocation(int line = 0, int column = 0) : line(line), column(column) {}
	int line;
	int column;
};

class TextReader {
public:
	static string readRange(string filename, int start_line, int end_line) {
		ifstream in(filename);
		string content;
		string line;
		for (int i = 1; i != start_line; ++i) {
			getline(in, line);
		}
		for (int i = start_line; i <= end_line; ++i) {
			getline(in, line);
			content += line;
		}
		return content;
	}

	static string readRange(string filename, int start_line, int start_column,
			int end_line, int end_column)
	{
		ifstream in(filename);
		string content;
		string line;
		for (int i = 1; i <= start_line; ++i) {
			getline(in, line);
		}
		line = line.substr(start_column - 1);
		for (int i = start_line + 1; i <= end_line; ++i) {
			content += line;
			getline(in, line);
		}
		content += line.substr(0, end_column);
		return content;
	}
};

class SourceCodeRange {
public:
	SourceCodeLocation start;
	SourceCodeLocation end;
	string filename;
	
	string getCode() 
	{
		return TextReader::readRange(filename, start.line, start.column,
				end.line, end.column);
	}

	string getLineCode() 
	{
		return TextReader::readRange(filename, start.line, end.line);
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

class DirSettings {
public:
  static void setWorkingDir(string dir);


  static string getWorkingDir();

private:
	static string working_dir;
};

#endif
