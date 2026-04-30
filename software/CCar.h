#pragma once

#include "server.h"
#include "CMotor.h"
#include "CGuidance.h"
#include "CCar.h"
#include "CCommunication.h"

class CCar
{
private:
	CServer _server;
	CCommunication _comm;
	CMotor _motors;
	CGuidance _guidance;
	static void serverthrd(CCar* ptr);
	static void imagethrd(CCar* ptr);
public:
	CCar();
	~CCar();
	void drive();
	int mode;
};