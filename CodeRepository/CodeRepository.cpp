/////////////////////////////////////////////////////////////////////////////
//  CodeRepository.cpp - Code repository                                   //
//  Language:     C++, VS 2015                                             //
//  Platform:     SurfaceBook, Windows 10 Pro                              //
//  Application:  Project1 for CSE687 - Object Oriented Design             //
//  Author:       Weijun Cai											   //
/////////////////////////////////////////////////////////////////////////////

#include "CodeRepository.h"

CodeRepository::CodeRepository(const std::string& fileRootDir, size_t serverPort, Socket::IpVer ipv)
	: sender_(), 
	rootSrcFileDir_(fileRootDir),
	receiver_(fileRootDir, serverPort, ipv), 
	analyPublisher_(), 
	analyzer_msgQ(), 
	msgHandler_(fileRootDir, analyzer_msgQ)
{
	codeAnalyThread_ = std::thread(&CodeRepository::run_analy_publish, this);
}


CodeRepository::~CodeRepository()
{
	HttpMessage quitMsg;
	quitMsg.addAttribute(HttpMessage::Attribute("Command", "quit"));
	analyzer_msgQ.enQ(quitMsg);
	sender_.~Sendr();
	receiver_.~Recvr();
	codeAnalyThread_.join();
}

bool ProcessCommandLine(int argc, char * argv[], std::string& rootSrcFileDir, size_t& port)
{
	std::cout << "\n  Command line arguments:";
	std::cout << "\n   1 - Root path to store files;";
	std::cout << "\n   2 - Port which the receive socket is bound to." << std::endl;
	if (argc < 3)
	{
		std::cout << "Invalid command line arguments!" << std::endl;
		return false;
	}

	rootSrcFileDir = FileSystem::Path::getFullFileSpec(argv[1]);
	if (!FileSystem::Directory::exists(rootSrcFileDir))
	{
		std::cout << "\n\n Specified path \"" << rootSrcFileDir << "\" does not exist\n\n";
		return false;
	}

	port = Convert<size_t>::fromString(argv[2]);
	return true;
}



void CodeRepository::run_analy_publish()
{
	while (true)
	{
		auto analyReq = analyzer_msgQ.deQ();
		if (analyReq.findValue("Command") == "quit")
			break;
		std::string targetFolderPath;	
		if ((targetFolderPath = analyReq.findValue("Category")) != "")
		{
			CodeAnalyPublisher analyzer;
			analyzer.TargetSrcPath() = targetFolderPath;
			analyzer.getSourceFiles();
			analyzer.processSourceCode();
			std::string publishPath = analyReq.findValue("Category");

			analyzer.PublishFiles(publishPath);

			if (analyReq.findValue("Demonstration") == "ON")
				analyzer.demonstrateHTMLs();
			postMessage(generateAnalyReply(analyReq));
		}
	}
}

bool CodeRepository::startReceiver()
{
	return receiver_.startServer();
}

bool CodeRepository::startSender(const std::string & ip, size_t port)
{
	return sender_.connectServer(ip, port);
}

HttpMessage CodeRepository::getMessage()
{
	return receiver_.getMessage();
}

void CodeRepository::postMessage(const HttpMessage & msg)
{
	sender_.postMessage(msg);
}

HttpMessage CodeRepository::processMessage(const HttpMessage & msg)
{
	return msgHandler_.HandleHessage(msg);
}

HttpMessage CodeRepository::generateAnalyReply(HttpMessage analyReq)
{
	HttpMessage reply = analyReq;
	std::string categoryPath = analyReq.findValue("Category");
	auto pos = categoryPath.find_last_of('\\');
	std::string categoryName = categoryPath.substr(pos + 1);
	reply.removeAttribute("Command");
	reply.removeAttribute("Demonstration");
	reply.removeAttribute("Category");
	reply.addAttribute(HttpMessage::Attribute("Category", categoryName));
	reply.addAttribute(HttpMessage::Attribute("Command", "AnalyzeAck"));
	reply.addAttribute(HttpMessage::Attribute("Additional Message", "Dependency analysis for category \"" + categoryName + "\" is comptele." ));
	return reply;
}




#include <iostream>
void main(int argc, char* argv[])
{
	using show = Logging::StaticLogger<1>;
	std::cout << " //------------------------< Remote code repository >----------------------//\n\n" << std::endl;
	std::string rootPath;
	size_t port;
	if (!ProcessCommandLine(argc, argv, rootPath, port))
		return;
	CodeRepository repo(rootPath, port);
	auto start = repo.startReceiver();
	while (true)
	{
		show::flush();
		auto & msgGot = repo.getMessage();
		std::cout << "\n =================================\n" << std::endl;
		std::cout << "\n Received message from peer " << msgGot.findValue("Source") << "\n\n";
		std::cout << msgGot.headerString();
		auto replyMsg = repo.processMessage(msgGot);
		repo.postMessage(replyMsg);
	}

}