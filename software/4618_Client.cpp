///////////////////////////////////////////////////////////////
// ELEX 4618 Client Template project for BCIT
// Created Oct 5, 2016 by Craig Hennessey
// Last updated April 2022
////////////////////////////////////////////////////////////////
#include "stdafx.h"

#define WIN32_LEAN_AND_MEAN
#define _WINSOCKAPI_   // stops Windows.h from including old winsock.h
#include <windows.h>

#include <string>
#include <iostream>
#include <thread>
#include <conio.h>

#include "Client.h"

#define MASK 0x8000

std::string server_ip = "192.168.137.123"; 
//
//at home 10.0.0.33

int server_port = 4012;
int camera_port = 5012;

float timeout_start;

void print_menu()
{
	std::cout << "\n***********************************";
	std::cout << "\n Fork-Lift Operation";
	std::cout << "\n***********************************";
	std::cout << "\nControl with WSAD keys to move. E to lift Fork. Q to lower Fork.";
	std::cout << "\nPress U for Autonomous";
	std::cout << "\n(0) Exit\n";


	std::cout << "\nCMD> ";
}


void get_image(CClient& client) {

	cv::Mat im;
	if (client.rx_im(im) == true) //If image received
	{
		timeout_start = cv::getTickCount();

		if (im.empty() == false) //If image is present (not empty), then show image
		{

			cv::imshow("rx", im);
			cv::waitKey(1);
		}
	}

	else //If not present, try connecting again 
	{
		if ((cv::getTickCount() - timeout_start) / cv::getTickFrequency() > 5.0)
		{
			timeout_start = cv::getTickCount();
			client.close_socket();
			client.connect_socket(server_ip, camera_port);
		}
	}
	

}

int main(int argc, char* argv[])
{
	CClient control_client;
	CClient cam_client;

	timeout_start = cv::getTickCount();

	
	control_client.connect_socket(server_ip, server_port);
	cam_client.connect_socket(server_ip, camera_port);


	print_menu();


	char last_cmd = 'X';

	while (true)
	{

		char cmd = 'X';

		if (GetAsyncKeyState('W') & MASK) //AND MASK returns either 1:pressed or 0:not pressed 
			cmd = 'W';
		else if (GetAsyncKeyState('S') & MASK)
			cmd = 'S';
		else if (GetAsyncKeyState('A') & MASK)
			cmd = 'A';
		else if (GetAsyncKeyState('D') & MASK)
			cmd = 'D';
		else if (GetAsyncKeyState('E') & MASK)//servos down
			cmd = 'E';
		else if (GetAsyncKeyState('Q') & MASK)//servos up
			cmd = 'Q';
		else if (GetAsyncKeyState('U') & MASK)//servos up
			cmd = 'U';
		else if (GetAsyncKeyState('C') & MASK) {
			control_client.close_socket();
			cam_client.close_socket();
			Sleep(1000);
			control_client.connect_socket(server_ip, server_port);
			cam_client.connect_socket(server_ip, camera_port);
		}

		if (cmd != last_cmd)
		{
			std::string msg;
			msg += cmd;
			msg += "\n";

			control_client.tx_str(msg);
			last_cmd = cmd;
		}


		static double last_img_time = 0;
		double now = cv::getTickCount() / cv::getTickFrequency();

		if (now - last_img_time > 0.05)
		{
			get_image(cam_client);
			last_img_time = now;
		}

		cv::waitKey(1);
		Sleep(5);

	}

	
}