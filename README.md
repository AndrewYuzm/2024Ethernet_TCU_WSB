# 2024Ethernet_TCU_WSB
telemetry system and Ethernet connection for TCU and WSB designed in 2024
UWFE Online Telemetry System V1 Quick Start -- May 25, 2024
Author: Andrew Yu

This document will guide you to set up the Telemetry System and GUI on your laptop, and trouble shooting.

1.	Setup 
Preparation:
We need TCU mounted on the car (with CAN bus working)
a router with power adapter, a SIM card, an antenna
an ethernet cable
your computer
make sure the NUC is powered and able to work.

1)	Connect the TCU with the router by an ethernet cable (free to use any port LAN1 or LAN2), insert the SIM card to the router SIM 1 slot, connect the antenna to “cellular 1” port, then connect the power adapter to the router to start it. It will take a few minutes to connect to the cellular network.

  
Figure 1.1 switch on the top, router on the bottom
Figure 1.2 GUI and connections

2)	Visit https://github.com/UWaterloo-Formula-Electric/telemetry_interface, download the code and follow the instruction to run the GUI designed by Addison Chen. 
3)	Or there is an alternative option to receive the data, please see 2.1)

2.	Check the NCU server
Problem: Cannot get any data on your laptop, or cannot create TCP connection.
First, make sure the NCU is powered! 
The fastest and easiest way to check if the server is working or getting data:
1)	Create a TCP client connection to the server:
for windows, use this in your cmd window:
telnet 208.68.36.87 2333
For MacOS terminal:
nc 208.68.36.87 2333
2)	Type something randomly and press enter/return button (This means to send something to the server)
3)	If the same string send back from the server, that means the server is working and the TCP service code is running.
If nothing happened, or even you are not able to create the TCP connection, please ask firmware members or lead (whoever with access to NCU) to solve it, or send a message to @Andrew Yu on Slack.

* Note for NCU Admin to solve the problem:
1. cd AndrewsServ/
2. php test.php start -d (run the code in daemon mode)
Do everything start from 2.1) to test if it’s working

3.	Check the router
Use another ethernet cable to connect the router to your PC, following the sticker on the router to access the manage page, to check the internet connection.

to be updated...
![image](https://github.com/AndrewYuzm/2024Ethernet_TCU_WSB/assets/118711557/51491d32-1798-4230-ac20-15a80878d052)
