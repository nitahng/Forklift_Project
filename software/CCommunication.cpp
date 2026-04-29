#include "stdafx.h"
#include "CCommunication.h"
#include "server.h"
#include <opencv2/opencv.hpp>
#include <vector>
#include <chrono>



CCommunication::CCommunication() {


}

CCommunication::~CCommunication() {



}

//Transmission from Pi to PC (Client)
void CCommunication::sendimage(cv::Mat frame) {

	server.set_txin(frame);

}

//Confirm command is sent from PC to Pi
bool CCommunication::confirmcommand(char &cmd) {

	std::vector<std::string> cmds;
	server.get_cmd(cmds);
	
	if (cmds.size() > 0 && cmds[0].size() > 0) {
		cmd = cmds[0][0];//returns first character of first string
		return true;
	}

	else {
		return false;
	}

}

