///////////////////////////////////////////////////////////////
// ELEX 4618 Client Template project for BCIT
// Created Oct 5, 2016 by Craig Hennessey
// Last updated April 2022
////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include <string>
#include <iostream>
#include <thread>

#include "Client.h"

std::string server_ip = "192.168.137.123";//rasppi ip
int server_port = 4618;

float timeout_start;

void print_menu()
{
	std::cout << "\n***********************************";
	std::cout << "\n* ELEX4618 Client Project";
	std::cout << "\n***********************************";
	std::cout << "\n(1) Turn system ON";
	std::cout << "\n(2) Turn system OFF";
	std::cout << "\n(3) Sort to BIN1";
	std::cout << "\n(4) Sort to BIN2";
	std::cout << "\n(5) Get system status";
	std::cout << "\n(6) Get BIN1 count";
	std::cout << "\n(7) Get BIN2 count";
	std::cout << "\n(8) Get image";
	std::cout << "\n(0) Exit";
	std::cout << "\nCMD> ";
}

void send_command(CClient& client, std::string cmd)
{
	std::string str;

	client.tx_str(cmd);
	std::cout << "\nClient Tx: " << cmd;

	if (cmd == "im")
	{
		cv::Mat im;
		if (client.rx_im(im) == true)
		{
			timeout_start = cv::getTickCount();
			if (im.empty() == false)
			{
				std::cout << "\nClient Rx: Image received";
				cv::imshow("rx", im);
				cv::waitKey(10);
			}
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
	else
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
	int cmd = -1;

	timeout_start = cv::getTickCount();
	client.connect_socket(server_ip, server_port);

	do
	{
		print_menu();
		std::cin >> cmd;

		switch (cmd)
		{
		case 1: send_command(client, "S 0 1\n"); break;
		case 2: send_command(client, "S 0 0\n"); break;
		case 3: send_command(client, "S 1 0\n"); break;
		case 4: send_command(client, "S 1 1\n"); break;
		case 5: send_command(client, "G 0\n"); break;
		case 6: send_command(client, "G 1 0\n"); break;
		case 7: send_command(client, "G 1 1\n"); break;

		case 8:

			while (true)
			{
				send_command(client, "im");

				if (cv::waitKey(30) == 'q')
					break;
			}
			break;

		}
	} while (cmd != 0);
}