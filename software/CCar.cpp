#include "stdafx.h"
#include "CCar.h"

#define Manual 1
#define Auto 2

CCar::CCar() {

	mode = Manual;

}

CCar::~CCar() {



}


void CCar::drive() {


	while (1) {

		char cmd;

		if (_comm.confirmcommand(cmd) == true) {

			if (cmd == 'W') {
				_motors.forward(50000);
			}

		}
		

	}


}

