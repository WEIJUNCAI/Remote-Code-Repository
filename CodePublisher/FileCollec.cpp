#include "FileCollec.h"

FileCollec::FileCollec(FileMap& fileMap)
{
	for (auto & pair : fileMap)
	{
		for (auto & file : pair.second)
			srcfiles.push_back(file);
	}
}

FileCollec::Iterator FileCollec::begin()
{
	return srcfiles.begin();
}

FileCollec::Iterator FileCollec::end()
{
	return srcfiles.end();
}




