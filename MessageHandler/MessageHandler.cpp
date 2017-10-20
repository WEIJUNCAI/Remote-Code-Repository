/////////////////////////////////////////////////////////////////////////////
//  MessageHandler.cpp - handles incoming http messages                    //
//  Language:     C++, VS 2015                                             //
//  Platform:     SurfaceBook, Windows 10 Pro                              //
//  Application:  Project1 for CSE687 - Object Oriented Design             //
//  Author:       Weijun Cai											   //
/////////////////////////////////////////////////////////////////////////////

#include "MessageHandler.h"
#include "../FileSystem/FileSystem.h"

using Show = Logging::StaticLogger<1>;


MessageHandler::MessageHandler(const std::string rootPath, Async::BlockingQueue<HttpMessage>& analy_Q)
	: rootFileDir(rootPath), analyzer_msgQ(analy_Q) {}



HttpMessage MessageHandler::HandleHessage(const HttpMessage & reqMsg)
{
	HttpMessage reply;
	auto command = reqMsg.findValue("Command");
	if (command == "")
	{
		Show::write("\n\n Invalid message, dropped.");
		return reply;
	}
	// received file from remote peer, send ACK back
	if (command == "File Upload")
		return handleFileUploadReq(reqMsg);
	///////////////////////////////////////////////////
	// should subsequently send file to remote peer,
	// - resolve file name and category into local absolute path
	// -- if cannot find file, send a File ACK instead
	// -- if can find file, fill in the local path for sender thread to send
	if (command == "File Download")
		return handleFileDownloadReq(reqMsg);
	if (command == "Get All Categories")
		return handleGetAllCategoryReq(reqMsg);
	if (command == "Get Files In category")
		return handleGetFilesInCategoryReq(reqMsg);
	if (command == "Remove File")
		return handleDeleteFileInCategoryReq(reqMsg);
	//////////////////////////////////////////////////////////////////
	// resolve file names for analyzer to analyze and publish
	// - dependency analysis can take relatively a long time to finish
	// - instead of block waiting for analysis result and and send,
	//   message handler will drop a request in analyzer's queue
	//   and attend to other messages
	if (command == "Publish")
		return handleCodeAnalyAndPubReq(reqMsg);
	if (command == "FileAck")
	{
		auto srcInfo = reqMsg.findValue("Source");
		auto additionalMsg = reqMsg.findValue("Additional Message");
		Show::write("\n\n Received response from peer \"" + srcInfo + "\":");
		Show::write("\n " + additionalMsg);
	}
	if (command == "quit")
	{
		// TODO: add shut down logic
	}
	return HttpMessage();
}

HttpMessage MessageHandler::handleFileUploadReq(const HttpMessage & reqMsg)
{
	// copy the original request message to retain info that client put in request
	// client do this to remind itself what plan it has for this reply (e.g., updating GUI)
	// server does not need to understand those info, just put in reply and return to client
	// this happends because our communication model is asynchronous
	HttpMessage reply = reqMsg;
	reply.removeAttribute("Command");
	reply.addAttribute(HttpMessage::Attribute("Command", "FileAck"));
	std::string ackInfo;
	auto srcAddr = reqMsg.findValue("Destination");
	auto destAddr = reqMsg.findValue("Source");
	auto fileName = reqMsg.findValue("File Name");
	reply.removeAttribute("Destination");
	reply.removeAttribute("Source");
	reply.removeAttribute("File Name");
	reply.addAttribute(HttpMessage::Attribute("File Name", fileName));
	reply.addAttribute(HttpMessage::Attribute("Destination", destAddr));
	reply.addAttribute(HttpMessage::Attribute("Source", srcAddr));

	if (reqMsg.findValue("File Receive Result") == "Successful")
	{
		ackInfo = "File \"" + fileName + "\" Successfully uploaded.";
		reply.addAttribute(HttpMessage::Attribute("Additional Message", ackInfo));
	}
	else
	{
		ackInfo = reqMsg.findValue("Additional Message");
		reply.addAttribute(HttpMessage::Attribute("Additional Message", ackInfo));
	}
	return reply;
}


HttpMessage MessageHandler::handleFileDownloadReq(const HttpMessage & reqMsg)
{
	HttpMessage reply = reqMsg;
	std::string filePath;
	reply.removeAttribute("Command");
	reply.removeAttribute("Destination");
	reply.removeAttribute("Source");
	reply.removeAttribute("File Name");
	auto fileName = reqMsg.findValue("File Name");
	auto category = reqMsg.findValue("Category");
	auto srcAddr = reqMsg.findValue("Destination");
	auto destAddr = reqMsg.findValue("Source");
	reply.addAttribute(HttpMessage::Attribute("Destination", destAddr));
	reply.addAttribute(HttpMessage::Attribute("Source", srcAddr));

	if (category != "")
		filePath = rootFileDir + "\\" + category + "\\" + fileName;
	else
		filePath = rootFileDir + "\\" + fileName;
	if (FileSystem::File::exists(filePath))
	{
		reply.addAttribute(HttpMessage::Attribute("Command", "File Upload"));
		reply.addAttribute(HttpMessage::Attribute("File Name", filePath));
	}
	else
	{
		reply.addAttribute(HttpMessage::Attribute("Command", "FileAck"));
		reply.addAttribute(HttpMessage::Attribute("File Send Result", "Failed"));
		reply.addAttribute(HttpMessage::Attribute("Additional Message", "File \"" + fileName +
			"\" in category \"" + category + "\" does not exist."));
	}
	return reply;
}


HttpMessage MessageHandler::handleGetAllCategoryReq(const HttpMessage & reqMsg)
{
	HttpMessage reply = reqMsg;
	reply.removeAttribute("Command");
	reply.removeAttribute("Destination");
	reply.removeAttribute("Source");
	reply.addAttribute(HttpMessage::Attribute("Command", "Set All Categories"));
	auto srcAddr = reqMsg.findValue("Destination");
	auto destAddr = reqMsg.findValue("Source");
	reply.addAttribute(HttpMessage::Attribute("Destination", destAddr));
	reply.addAttribute(HttpMessage::Attribute("Source", srcAddr));

	auto categoryFolders = FileSystem::Directory::getDirectories(rootFileDir);
	for (auto & categoryFolder : categoryFolders)
	{
		if (categoryFolder.find('.') == std::string::npos)
			reply.addAttribute(HttpMessage::Attribute("Category", categoryFolder));
	}
	return reply;
}


HttpMessage MessageHandler::handleGetFilesInCategoryReq(const HttpMessage & reqMsg)
{
	HttpMessage reply = reqMsg;
	reply.removeAttribute("Command");
	reply.removeAttribute("Destination");
	reply.removeAttribute("Source");
	reply.addAttribute(HttpMessage::Attribute("Command", "Set Files In Category"));
	auto srcAddr = reqMsg.findValue("Destination");
	auto destAddr = reqMsg.findValue("Source");
	reply.addAttribute(HttpMessage::Attribute("Destination", destAddr));
	reply.addAttribute(HttpMessage::Attribute("Source", srcAddr));
	std::string category = reqMsg.findValue("Category");
	auto categoryPath = (category == "NULL") ? rootFileDir : rootFileDir + "\\" + category;

	auto filesInCategoryFolder = FileSystem::Directory::getFiles(categoryPath);
	for (auto & file : filesInCategoryFolder)
		reply.addAttribute(HttpMessage::Attribute("File Name", file));
	return reply;
}

HttpMessage MessageHandler::handleDeleteFileInCategoryReq(const HttpMessage & reqMsg)
{
	HttpMessage reply = reqMsg;
	reply.removeAttribute("Command");
	reply.removeAttribute("Destination");
	reply.removeAttribute("Source");
	reply.addAttribute(HttpMessage::Attribute("Command", "FileAck"));
	std::string ackInfo;
	auto srcAddr = reqMsg.findValue("Destination");
	auto destAddr = reqMsg.findValue("Source");
	auto fileName = reqMsg.findValue("File Name");
	auto category = reqMsg.findValue("Category");
	reply.addAttribute(HttpMessage::Attribute("Destination", destAddr));
	reply.addAttribute(HttpMessage::Attribute("Source", srcAddr));

	if (!FileSystem::File::remove(rootFileDir + "\\" + category + "\\" + fileName))
	{
		reply.addAttribute(HttpMessage::Attribute("File Delete Result", "Failed"));
		reply.addAttribute(HttpMessage::Attribute("Additional Message", "Cannot delete file \"" + fileName + "\"."));
	}
	else
	{
		FileSystem::File::remove(rootFileDir + "\\" + category + "\\" + fileName + ".htm");
		ackInfo = "File \"" + fileName + "\" successfully deleted from category \"" + category + "\".";
		reply.addAttribute(HttpMessage::Attribute("File Delete Result", "Successful"));
		reply.addAttribute(HttpMessage::Attribute("Additional Message", ackInfo));
	}
	return reply;
}


HttpMessage MessageHandler::handleCodeAnalyAndPubReq(const HttpMessage & reqMsg)
{
	HttpMessage reply = reqMsg;
	reply.removeAttribute("Command");
	reply.removeAttribute("Destination");
	reply.removeAttribute("Source");
	reply.addAttribute(HttpMessage::Attribute("Command", "Publish"));
	auto srcAddr = reqMsg.findValue("Destination");
	auto destAddr = reqMsg.findValue("Source");
	reply.addAttribute(HttpMessage::Attribute("Destination", destAddr));
	reply.addAttribute(HttpMessage::Attribute("Source", srcAddr));

	std::string category;
	if ((category = reqMsg.findValue("Category")) != "") // currently support analyze and publish files in a specific category folder
	{
		auto filePath = FileSystem::Path::getFullFileSpec(rootFileDir + "\\" + category);
		reply.removeAttribute("Category");
		reply.addAttribute(HttpMessage::Attribute("Category", filePath));
	}
	analyzer_msgQ.enQ(reply);
	return HttpMessage(); // empty message will be placed in send queue but not be sent
}
