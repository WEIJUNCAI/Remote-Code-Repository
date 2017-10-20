#pragma once

/////////////////////////////////////////////////////////////////////////////
//  IComm.h - interface for sender and receiver                            //
//  Language:     C++, VS 2015                                             //
//  Platform:     SurfaceBook, Windows 10 Pro                              //
//  Application:  Project1 for CSE687 - Object Oriented Design             //
//  Author:       Jim Fawcett											   //
//				  Weijun Cai                                               //
/////////////////////////////////////////////////////////////////////////////

#include <string>
#include "../HttpMessage/HttpMessage.h"
using Message = HttpMessage;

struct ISendr
{
	virtual void postMessage(const Message& msg) = 0;
};

struct IRecvr
{
	virtual Message getMessage() = 0;
};