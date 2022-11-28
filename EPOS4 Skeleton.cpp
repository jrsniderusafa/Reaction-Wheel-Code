// EPOS4 Skeleton.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "Definitions.h"
#include <iostream>
#include <time.h>
#include <Windows.h>
#include <dos.h>
#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>

using namespace std;
using std::cout; using std::endl;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
////using std chrono::system_clock;

//Configure EPOS4 parameters 
HANDLE keyHandle = 0;
char CommStack[] = ("MAXON SERIAL V2");
char Interface[] = ("USB");
char Port[] = ("USB0");
char DeviceName[] = ("EPOS4");
DWORD errorCode = 0;
WORD NodeId = 1;
BOOL IsInFault = 0;
BOOL Executed = false;
int velocity_input = 1; //// initialize variable for velocity user input
long pVelocityIs = 0; //// initialize variable that is recognized by the VCS get average velocity operator
short pCurrentIs = 0;
long pVelocity = 0;
short pCurrent = 0;

using std::cout; using std::endl;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

int main()
{
	////Create file
	std::ofstream myFile("velocity.csv");
	myFile << "Velocity (rpm)" << "," << "Current (mA)" << "," << "Time (ms)" << "\n";
	//Establish communication with the EPOS4
	while (keyHandle == 0) {
		keyHandle = VCS_OpenDeviceDlg(&errorCode);
		//keyHandle = VCS_OpenDevice(DeviceName, CommStack, Interface, Port, &errorCode);	//Get keyHandle for EPOS4
		if (keyHandle != 0) {
			cout << "Communication Established\n";
		}
		else if (keyHandle == 0) {
			cout << "Failed to establish communication, trying again...\n";
			keyHandle = VCS_OpenDeviceDlg(&errorCode);
		}
	}

	//Enable the motor power stage on the EPOS4
	Executed = VCS_SetEnableState(keyHandle, NodeId, &errorCode);		//Enable the EPOS4 controller
	while (Executed == false) {
		Executed = VCS_SetEnableState(keyHandle, NodeId, &errorCode);
	}
	Executed = false;

	/////////Demo code in this section, delete and add your own code/////////////////////////////////
	////C3C Joe Olson's changes are made with four slashes

	VCS_ActivateProfileVelocityMode(keyHandle, NodeId, &errorCode);		////Activate profile velcoity mode
	VCS_SetVelocityProfile(keyHandle, NodeId, 2500, 2500, &errorCode);	////Set velcoity profile with 2500rpm/s accel & decel


	cout << "Input velocity for velocity move in rpm: "; ////prompts user to input a number gets user input for velocity value
	cin >> velocity_input; ////stores user input as a variable

	while (velocity_input != 0) { ////as long as the input is not 0 rpm
		VCS_GetVelocityIsAveraged(keyHandle, NodeId, &pVelocityIs, &errorCode); //// gets velocity values from sensor
		cout << "Current average velocity is " << pVelocityIs << "\n"; //// prints average velocity value at the time of the command
		VCS_GetCurrentIsAveraged(keyHandle, NodeId, &pCurrentIs, &errorCode);
		cout << "Current average current is " << pCurrentIs << "\n";
			if (velocity_input != 0) {
				Executed = VCS_MoveWithVelocity(keyHandle, NodeId, velocity_input, &errorCode);	////Start velocity move of desired value measured in rpm
				cout << "Executing velocity move\n\n"; ////prints that velocity move command is working

				auto starttime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
				time_t timer;
				time_t countdown;
				//time_t basetime;
				timer = time(NULL);
				//basetime = time(NULL);
				countdown = time(NULL) + 5;
				while (countdown > timer) {
					
					VCS_GetVelocityIs(keyHandle, NodeId, &pVelocity, &errorCode);
					VCS_GetCurrentIs(keyHandle, NodeId, &pCurrent, &errorCode);
					myFile << pVelocity << "," << pCurrent << "," << duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() - starttime << "\n";
					timer = time(NULL);
					
				}
				while (Executed == false) {
					cout << "Failed to execute velocity move, trying again...\n"; ////prints that velocity command is not working
					Executed = VCS_MoveWithVelocity(keyHandle, NodeId, velocity_input, &errorCode);
				}
				Executed = false;
				cout << "Input new velocity for velocity move in rpm \nInput the previous value entered to display current average velocity \nInput 0 to stop program (It is advised to slow down the wheel before stopping by entering a small rpm value such as 10, then 0 to stop the wheel) \nNew input: ";
				cin >> velocity_input; ////stores new input as the velocity variable, loop starts again if input is not 0
			}

			////The following code is to be included if the motor is needed to turn under a time constraint	
			//time_t timer;
			//time_t countdown;
			//timer = time(NULL);
			//countdown = time(NULL) + 10;
			//while (countdown > timer)		//set a timer for 10 seconds before the drive is disabled
			//{
				//timer = time(NULL);
			//}
				//Executed = false;
		}
		Executed = false;

		Executed = VCS_HaltVelocityMovement(keyHandle, NodeId, &errorCode);		//Stop profile veolcity move 
		cout << "Stopping velocity move\n"; ////prints that velocity move is stopping
		while (Executed == false) {
			cout << "Failed to stop velocity move, trying again...\n"; ////prints that the move failed to stop
			Executed = VCS_HaltVelocityMovement(keyHandle, NodeId, &errorCode);
		}
		cout << "Velocity move successfully stopped\n";
		///////////////////////////////////////////////////////////////////////////////////////////////////////

		Executed = VCS_SetDisableState(keyHandle, NodeId, &errorCode);		//disble the drive
		while (Executed == false) {
			Executed = VCS_SetDisableState(keyHandle, NodeId, &errorCode);
		}
		Executed = false;

		Executed = VCS_CloseDevice(keyHandle, &errorCode);				//Close communication port
		while (Executed == false) {
			Executed = VCS_CloseDevice(keyHandle, &errorCode);
		}

		cout << "\nProgram Finished\n";
		myFile.close();
		return 0;
	}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu


