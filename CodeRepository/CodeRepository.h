#pragma once

/////////////////////////////////////////////////////////////////////////////
//  CodeRepository.h - Code repository                                     //
//  Language:     C++, VS 2015                                             //
//  Platform:     SurfaceBook, Windows 10 Pro                              //
//  Application:  Project1 for CSE687 - Object Oriented Design             //
//  Author:       Weijun Cai											   //
/////////////////////////////////////////////////////////////////////////////
/*
*   Module Operations
*   -----------------
*   This module contains the implementaion of code repository class, which is able 
*	to store files in different categories, analyze dependency information among
*	collection of source files and publish them into web pages.
*
*/
/*
*   Build Process
*   -------------
*   - Required files: MessageHandler.h, MessageHandler.cpp
*					  CommChannel.h, CommChannel.cpp
*					  CodeAnalyPublisher.h, CodeAnalyPublisher.cpp
*					  CodeRepository.h, CodeRepository.cpp
*
*   Maintenance History
*   -------------------
*   ver 1.0 : 28 April 2017
*/

#include "../CommChannel/CommChannel.h"
#include "../CodeAnalyPublisher/CodeAnalyPublisher.h"
#include "../MessageHandler/MessageHandler.h"

class CodeRepository
{
public:
	CodeRepository(const std::string& fileRootDir, size_t serverPort, Socket::IpVer ipv = Socket::IP6);
	~CodeRepository();
	// analyze and publish files
	void run_analy_publish();
	// start message listener for accepting messages from peers
	bool startReceiver();
	// start message sender to sending message
	bool startSender(const std::string& ip, size_t port);
	// get message from peer for handling
	HttpMessage getMessage();
	// put reply message to send back to peer
	void postMessage(const HttpMessage& msg);
	// handling incoming messages
	HttpMessage processMessage(const HttpMessage& msg);

private:
	HttpMessage generateAnalyReply(HttpMessage analyReq);

private:
	Sendr sender_;
	Recvr receiver_;
	CodeAnalyPublisher analyPublisher_;

	size_t port;
	std::string rootSrcFileDir_;
	Async::BlockingQueue<HttpMessage> analyzer_msgQ;
	MessageHandler msgHandler_;
	std::thread codeAnalyThread_;
		
};
