#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>

class FileCollec
{
public:

	using Path = std::string;
	using File = std::string;
	using Files = std::vector<File>;
	using Pattern = std::string;
	using FileMap = std::unordered_map<Pattern, Files>;
	using Iterator = std::vector<std::string>::iterator;

	FileCollec() {}
	FileCollec(FileMap& fileMap);

	Iterator begin();
	Iterator end();


private:
	std::vector<std::string> srcfiles;


};
