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

std::string server_ip = "10.0.0.33"; 
int server_port = 4618;

float timeout_start;

void print_menu()
{
	std::cout << "\n***********************************";
	std::cout << "\n Fork-Lift Operation";
	std::cout << "\n***********************************";
	std::cout << "\n(1) Manual Mode ";
	std::cout << "\n(2) Auto Mode";
	std::cout << "\n(0) Exit\n";


	std::cout << "\nCMD> ";
}

//This Function sends a command from PC to PI. 
void send_command(CClient& client, std::string cmd)
{
	//Sends command to PI
	std::string str;
	client.tx_str(cmd);
	std::cout << "\nClient Tx: " << cmd;

	
	if (cmd == "im")//if "im" Make Image commands to return an image
	{
		cv::Mat im; 
		if (client.rx_im(im) == true) //If image received
		{
			timeout_start = cv::getTickCount();

			if (im.empty() == false) //If image is present (not empty), then show image
			{
				std::cout << "\nClient Rx: Image received";
				cv::imshow("rx", im);
				cv::waitKey(10);
			}
		}
		else //If not present, try connecting again 
		{
			if ((cv::getTickCount() - timeout_start) / cv::getTickFrequency() > 1000)
			{
				timeout_start = cv::getTickCount();
				client.close_socket();
				client.connect_socket(server_ip, server_port);
			}
		}
	}

	else //get text rely from server then print, if no response then reconnect 
	{
		if (client.rx_str(str) == true)
		{
			timeout_start = cv::getTickCount();
			std::cout << "\nClient Rx: " << str;
		}
		else
		{
			if ((cv::getTickCount() - timeout_start) / cv::getTickFrequency() > 1000)
			{
				timeout_start = cv::getTickCount();
				client.close_socket();
				client.connect_socket(server_ip, server_port);
			}
		}
	}
}

int main(int argc, char* argv[])
{
	CClient client;

	timeout_start = cv::getTickCount();
	client.connect_socket(server_ip, server_port);

	print_menu();

	char last_cmd = 'X';

	while (1)
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
		else if (GetAsyncKeyState('E') & MASK)//servos up
			cmd = 'E';
		else if (GetAsyncKeyState('Q') & MASK)//servos down
			cmd = 'Q';

		if (cmd != last_cmd)
		{
			std::string msg;
			msg += cmd;
			msg += "\n";

			client.tx_str(msg);
			last_cmd = cmd;
		}


	}

	
}