///////////////////////////////////////////////////////////////
// ELEX 4618 Client Template project for BCIT
// Created Oct 5, 2016 by Craig Hennessey
// Last updated April 2022
////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include <string>
#include <iostream>
#include <thread>
#include <conio.h>

#include "Client.h"

std::string server_ip = "192.168.137.123"; 
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
	
	while (1) {
		if (_kbhit()) {

			char cmd = _getch();


			switch (cmd)
			{

			case 'w':
			case 'W': send_command(client, "W\n");
				break;

			case 'd':
			case 'D': send_command(client, "D\n"); \
				break;

			case 'a':
			case 'A': send_command(client, "A\n");
				break;

			case 's':
			case 'S': send_command(client, "S\n");
				break;

			case 'e':
			case 'E':

				while (true)
				{
					send_command(client, "im");

					if (cv::waitKey(30) == 'q')
						break;
				}
				break;

			}


		}

	}



	
}