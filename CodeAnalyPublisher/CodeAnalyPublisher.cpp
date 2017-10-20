/////////////////////////////////////////////////////////////////////////////
//  CodeAnalyPublisher.cpp - code analyzer and publisher                   //
//  Language:     C++, VS 2015                                             //
//  Platform:     SurfaceBook, Windows 10 Pro                              //
//  Application:  Project1 for CSE687 - Object Oriented Design             //
//  Author:       Weijun Cai      										   //
/////////////////////////////////////////////////////////////////////////////


#include "CodeAnalyPublisher.h"

using Rslt = Logging::StaticLogger<0>;  // use for application results
using Demo = Logging::StaticLogger<1>;  // use for demonstrations of processing
using Dbug = Logging::StaticLogger<2>;  // use for debug output


//----< helper: is text a substring of str? >--------------------

bool contains(const std::string& str, const std::string& text)
{
	if (str.find(text) < str.length())
		return true;
	return false;
}

CodeAnalyPublisher::CodeAnalyPublisher()
{
	pParser_ = configure_.Build();
	if (pParser_ == nullptr)
	{
		throw std::exception("couldn't create parser");
	}
	patterns_.push_back("*.cpp");
	patterns_.push_back("*.h");
	pRepo_ = CodeAnalysis::Repository::getInstance();
}

std::string & CodeAnalyPublisher::TargetSrcPath()
{
	return path_;
}


std::string & CodeAnalyPublisher::PublishPath()
{
	return publishPath;
}

std::string & CodeAnalyPublisher::StylePath()
{
	return stylePath;
}

std::string & CodeAnalyPublisher::JsPath()
{
	return jsPath;
}

std::vector<CodeAnalyPublisher::File>& CodeAnalyPublisher::cppHeaderFiles()
{
	cppHeaderFiles_.clear();
	for (auto item : fileMap_)
	{
		if (contains(item.first, "*.h"))
		{
			for (auto file : item.second)
				cppHeaderFiles_.push_back(file);
		}
	}
	return cppHeaderFiles_;
}

std::vector<CodeAnalyPublisher::File>& CodeAnalyPublisher::cppImplemFiles()
{
	cppImplemFiles_.clear();
	for (auto item : fileMap_)
	{
		if (contains(item.first, "*.cpp"))
		{
			for (auto file : item.second)
				cppImplemFiles_.push_back(file);
		}
	}
	return cppImplemFiles_;
}


void CodeAnalyPublisher::getSourceFiles()
{
	RepoFileMgr fm(path_, fileMap_);
	for (auto patt : patterns_)
		fm.addPattern(patt);
	fm.search();
}

void CodeAnalyPublisher::processSourceCode()
{
	processH();
	processCPP();

	depAnaly.AddFileRoot(pRepo_->getGlobalScope());

	analyzeH();
	analyzeCPP();
}

void CodeAnalyPublisher::openFile(const std::string& path)
{
	std::string path_ = FileSystem::Path::getFullFileSpec(path);
	std::string command("start \"\" \"" + path_ + "\"");
	// the command is: [ start "" "..\test files\page.html" ]
	// using [ start ..\text files\page.html ] will fail to start
	// using [ start "..\test files\page.html" ] will only start new command window with the current directory
	std::system(command.c_str());
}

void CodeAnalyPublisher::demonstrateHTMLs()
{
	for (auto & file : publisher.getHTMLfiles())
		openFile(file);
}

void CodeAnalyPublisher::processCPP()
{
	for (auto file : cppImplemFiles())
	{
		pRepo_->package() = FileSystem::Path::getName(file);

		if (!configure_.Attach(file))
		{
			std::ostringstream out;
			out << "\n  could not open file " << file << "\n";
			Rslt::write(out.str());
			Rslt::flush();
			continue;
		}
		// parse file

		if (!Rslt::running())
			Demo::write("\n\n  opening file \"" + pRepo_->package() + "\"");
		if (!Demo::running() && !Rslt::running())
			Dbug::write("\n\n  opening file \"" + pRepo_->package() + "\"");
		pRepo_->language() = CodeAnalysis::Language::Cpp;
		pRepo_->currentPath() = file;
		while (pParser_->next())
			pParser_->parse();

		depAnaly.AddNewFile(file, "C++ implementation file", "package");

		Slocs slocs = pRepo_->Toker()->currentLineCount();
		slocMap_[pRepo_->package()] = slocs;
	}
}

void CodeAnalyPublisher::processH()
{
	for (auto file : cppHeaderFiles())
	{
		pRepo_->package() = FileSystem::Path::getName(file);

		if (!configure_.Attach(file))
		{
			std::ostringstream out;
			out << "\n  could not open file " << file << "\n";
			Rslt::write(out.str());
			Rslt::flush();
			continue;
		}
		// parse file
		Rslt::flush();
		Demo::flush();
		Dbug::flush();

		if (!Rslt::running())
			Demo::write("\n\n  opening file \"" + pRepo_->package() + "\"");
		if (!Demo::running() && !Rslt::running())
			Dbug::write("\n\n  opening file \"" + pRepo_->package() + "\"");
		pRepo_->language() = CodeAnalysis::Language::Cpp;
		pRepo_->currentPath() = file;
		while (pParser_->next())
		{
			pParser_->parse();
		}

		depAnaly.AddNewFile(file, "C++ header file", "package");

		Slocs slocs = pRepo_->Toker()->currentLineCount();
		slocMap_[pRepo_->package()] = slocs;
	}
}

void CodeAnalyPublisher::analyzeCPP()
{
	for (auto file : cppImplemFiles())
	{
		pRepo_->package() = FileSystem::Path::getName(file);

		if (!configure_.Attach(file))
		{
			std::ostringstream out;
			out << "\n  could not open file " << file << "\n";
			Rslt::write(out.str());
			Rslt::flush();
			continue;
		}
		// parse file

		pRepo_->language() = CodeAnalysis::Language::Cpp;
		pRepo_->currentPath() = file;
		while (pParser_->next())
			depAnaly.AnalyzeStatement(file, pParser_->getTokCollec());
	}
}

void CodeAnalyPublisher::analyzeH()
{
	for (auto file : cppHeaderFiles())
	{

		pRepo_->package() = FileSystem::Path::getName(file);

		if (!configure_.Attach(file))
		{
			std::ostringstream out;
			out << "\n  could not open file " << file << "\n";
			Rslt::write(out.str());
			Rslt::flush();
			continue;
		}
		// parse file
		pRepo_->language() = CodeAnalysis::Language::Cpp;
		pRepo_->currentPath() = file;
		while (pParser_->next())
			depAnaly.AnalyzeStatement(file, pParser_->getTokCollec());
	}
}

void CodeAnalyPublisher::PublishFiles(const std::string& publishPath)
{
	NoSqlDb<std::string>& fileDepInfo = depAnaly.GetAnalyResult();

	if (!FileSystem::Directory::exists(publishPath))
	{
		if (FileSystem::Directory::create(publishPath))
			throw std::exception("\n\nspecified publish path cannot be created.\n");
	}
	publisher.setPublishPath(publishPath);
	publisher.setStylePath(stylePath);
	publisher.setFileInfo(&fileDepInfo);
	publisher.setJavaScriptPath(jsPath);

	if (filesToPub.size() == 0)
		publisher.PublishAll();
	else
	{
		for (auto & file : filesToPub)
		{
			if (fileDepInfo.find(file))
			{
				publisher.PublishFile(fileDepInfo[file]);
			}
		}
	}
}

void CodeAnalyPublisher::startLogger(std::ostream & out)
{
	Rslt::attach(&out);
	Demo::attach(&out);
	Dbug::attach(&out);
}

void CodeAnalyPublisher::flushLogger()
{
	Rslt::flush();
	Demo::flush();
	Dbug::flush();
}

void CodeAnalyPublisher::stopLogger()
{
	Rslt::flush();
	Demo::flush();
	Dbug::flush();
	Rslt::stop();
	Demo::stop();
	Dbug::stop();
}

RepoFileMgr::RepoFileMgr(const Path & path, FileMap & fileMap) : FileMgr(path), fileMap_(fileMap){}

void RepoFileMgr::file(const File & f)
{
	File fqf = d_ + "\\" + f;
	Ext ext = FileSystem::Path::getExt(fqf);
	Pattern p = "*." + ext;
	fileMap_[p].push_back(fqf);
}

void RepoFileMgr::dir(const Dir & d)
{
	d_ = d;
}

void RepoFileMgr::done()
{
}


#ifdef TEST_ANALYPUBLISHER



void main()
{
	CodeAnalyPublisher exec;
	exec.TargetSrcPath() = "C:\\Users\\cgarr\\Desktop\\P4\\TestCase";
	exec.startLogger(std::cout);
	exec.getSourceFiles();
	exec.processSourceCode();
	exec.PublishFiles();
	exec.flushLogger();

}

#endif // TEST_ANALYPUBLISHER