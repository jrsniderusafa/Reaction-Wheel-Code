# Reaction-Wheel-Code
Code to command maxon motor through command library functions on EPOS4 controller
The code when connected to a power supply, maxon motor, and EPOS4 controller will spin based on an input command of wheel speed in RPM
The code will continue to run at that speed for 5 seconds and will then prompt the user to input a new speed
After each update of a new input, the average velocity and current of the last 5 seconds will be printed on the screen
After the application is closed the code will automically collect the wheel speed, current, and time stamp of the entire run and output that to a .csv file
