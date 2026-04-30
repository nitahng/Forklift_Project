#pragma once
#include <opencv2/opencv.hpp>
#include "server.h"

class CCommunication
{

private:

	CServer server;

public:

	CCommunication();
	~CCommunication();

	void sendimage(cv::Mat frame);
	bool confirmcommand(char &cmd);

};

