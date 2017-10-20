#pragma once

/////////////////////////////////////////////////////////////////////////////
//  CodeAnalyPublisher.h - code analyzer and publisher                     //
//  Language:     C++, VS 2015                                             //
//  Platform:     SurfaceBook, Windows 10 Pro                              //
//  Application:  Project1 for CSE687 - Object Oriented Design             //
//  Author:       Weijun Cai      										   //
/////////////////////////////////////////////////////////////////////////////
/*
*   Module Operations
*   -----------------
*   This module contains the implementaion of code analyzer which analyze the
*	dependencies for a collctions of source files and later publish them into
*	HTML files
*
*/
/*
*   Build Process
*   -------------
*   - Required files: FileSystem.h, FileSystem.cpp
*                     Parser.h. Parser.cpp
*					  FileMgr.h, FileMgr.cpp
*					  CodePublisher.h, CodePublisher.cpp
*					  AbstractSyntaxTree.h, AbstractSyntaxTree.cpp
*					  CodeAnalyPublisher.h, CodeAnalyPublisher.cpp
*
*
*   Maintenance History
*   -------------------
*   ver 1.0 : 28 April 2017
*/

#include "../Parser/Parser.h"
#include "../FileMgr/FileMgr.h"
#include "../Parser/ConfigureParser.h"
#include "../Utilities/Utilities.h"
#include "../DependencyAnalyzer/DependencyAnalyzer.h"
#include "../StrongComponent/StrongComponent.h"
#include "../CodePublisher/CodePublisher.h"
#include "../AbstractSyntaxTree/AbstrSynTree.h"



class RepoFileMgr : public FileManager::FileMgr
{
public:
	using Path = std::string;
	using File = std::string;
	using Files = std::vector<File>;
	using Pattern = std::string;
	using Ext = std::string;
	using FileMap = std::unordered_map<Pattern, Files>;

	RepoFileMgr(const Path& path, FileMap& fileMap);
	virtual void file(const File& f);
	virtual void dir(const Dir& d);
	virtual void done();

private:
	Path d_;
	FileMap& fileMap_;
};


class CodeAnalyPublisher
{
public:
	using Path = std::string;
	using Pattern = std::string;
	using Patterns = std::vector<Pattern>;
	using File = std::string;
	using Files = std::vector<File>;
	using Ext = std::string;
	using Options = std::vector<char>;
	using FileMap = std::unordered_map<Pattern, Files>;
	using FileNodes = std::vector<std::pair<File, CodeAnalysis::ASTNode*>>;
	using Slocs = size_t;
	using SlocMap = std::unordered_map<File, Slocs>;


public:
	CodeAnalyPublisher();

	std::string& TargetSrcPath();
	std::string& PublishPath();
	std::string& StylePath();
	std::string& JsPath();

	std::vector<File>& cppHeaderFiles();
	std::vector<File>& cppImplemFiles();

	// get source files for analysis
	void getSourceFiles();
	// analyze dependency info for specified files
	void processSourceCode();
	// publish files into web pages
	void PublishFiles(const std::string& publishPath = "C:\\Users\\cgarr\\Desktop\\PublishedFiles");
	// open published HTML files in browser
	void demonstrateHTMLs();

	void startLogger(std::ostream& out);
	void flushLogger();
	void stopLogger();

private:

	void processCPP();
	void processH();

	void analyzeCPP();
	void analyzeH();

	void openFile(const std::string& path);
private:
	CodeAnalysis::Parser* pParser_;
	CodeAnalysis::ConfigParseForCodeAnal configure_;
	CodeAnalysis::Repository* pRepo_;
	std::string path_;
	Patterns patterns_;
	FileMap fileMap_;
	FileNodes fileNodes_;
	std::vector<File> cppHeaderFiles_;
	std::vector<File> cppImplemFiles_;
	SlocMap slocMap_;
	std::ofstream* pLogStrm_ = nullptr;

	////////////////////////////////////////////////////////
	DependencyAnalyzer depAnaly;
	CodePublisher publisher;

	std::vector<std::string> filesToPub;
	std::string publishPath = "RemoteCodeRepository\\PublishedFiles";
	std::string stylePath = "RemoteCodeRepository\\PublishedFiles\\style.css";
	std::string jsPath = "RemoteCodeRepository\\PublishedFiles\\collapse.js";
};