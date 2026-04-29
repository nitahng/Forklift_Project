// Short Main Function

#include "CCar.h"
#include "stdafx.h"
#include <opencv2/opencv.hpp>

int main()
{
	CCar forklift;

	std::cout << "Welcome to the FORKLFT operator\n";
	std::cout << "Enter MODE...\n";
	std::cout << "Manual (1)\n";
	std::cout << "Auto (2)\n\n\n";
	std::cout << "Enter Mode: ";

	
	forklift.drive();

	return 0;

}

