#pragma once

/////////////////////////////////////////////////////////////////////////////
//  MessageHandler.h - handles incoming http messages                      //
//  Language:     C++, VS 2015                                             //
//  Platform:     SurfaceBook, Windows 10 Pro                              //
//  Application:  Project1 for CSE687 - Object Oriented Design             //
//  Author:       Weijun Cai											   //
/////////////////////////////////////////////////////////////////////////////
/*
*   Module Operations
*   -----------------
*   This module contains the implementaion of message handler, which processes
*	Http message and performs different actions depending on the types of messages
*	this handler is specific for repository, clients have different handlers
*
*/
/*
*   Build Process
*   -------------
*   - Required files: MessageHandler.h, MessageHandler.cpp
*					  HttpMessage.h, HttpMessage.cpp
*					  Cpp11-BlockingQueue.h, Cpp11-BlockingQueue.cpp
*
*   Maintenance History
*   -------------------
*   ver 1.0 : 28 April 2017
*/

#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include "../HttpMessage/HttpMessage.h"
#include "../Logger/Logger.h"


class MessageHandler
{

public:
	MessageHandler(const std::string rootPath, Async::BlockingQueue<HttpMessage>& analy_Q);
	// the method called by other classes to hanlde message
	HttpMessage HandleHessage(const HttpMessage& reqMsg);

private:
	// process file uploading sent from peer
	HttpMessage handleFileUploadReq(const HttpMessage& reqMsg);
	// process file download request from peer
	HttpMessage handleFileDownloadReq(const HttpMessage& reqMsg);
	// process get all categories query
	HttpMessage handleGetAllCategoryReq(const HttpMessage& reqMsg);
	// handles query file names in a category
	HttpMessage handleGetFilesInCategoryReq(const HttpMessage& reqMsg);
	// handle file delete request
	HttpMessage handleDeleteFileInCategoryReq(const HttpMessage& reqMsg);
	// handle source file dependency and publish request from client
	HttpMessage handleCodeAnalyAndPubReq(const HttpMessage& reqMsg);

private:
	std::string rootFileDir;
	Async::BlockingQueue<HttpMessage>& analyzer_msgQ;
};