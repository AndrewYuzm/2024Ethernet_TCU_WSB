# UWFE Online Telemetry System V1 Quick Start -- May 25, 2024

## Author: Andrew Yu

This document will guide you to set up the Telemetry System and GUI on your laptop, and troubleshooting.

## 1. Setup

### Preparation:
We need:
- TCU mounted on the car (with CAN bus working)
- A router with power adapter, a SIM card, an antenna
- An ethernet cable
- Your computer
- Make sure the NUC is powered and able to work.

### Steps:
1. Connect the TCU with the router by an ethernet cable (free to use any port LAN1 or LAN2), insert the SIM card to the router SIM 1 slot, connect the antenna to the “cellular 1” port, then connect the power adapter to the router to start it. It will take a few minutes to connect to the cellular network.

<img width="196" alt="image" src="https://github.com/AndrewYuzm/2024Ethernet_TCU_WSB/assets/118711557/e2325a3a-a6ed-4b58-8b73-a1be93fc882f">
<img width="196" alt="image" src="https://github.com/AndrewYuzm/2024Ethernet_TCU_WSB/assets/118711557/c035ce20-726d-4883-bead-f20afc4b7984">

    ![Figure 1.1 switch on the top, router on the bottom]

    ![Figure 1.2 GUI and connections]

2. Visit [UWaterloo-Formula-Electric/telemetry_interface](https://github.com/UWaterloo-Formula-Electric/telemetry_interface), download the code and follow the instructions to run the GUI designed by Addison Chen.
3. Or there is an alternative option to receive the data, please see 2.1).

## 2. Check the NCU server

### Problem: Cannot get any data on your laptop, or cannot create TCP connection.

First, make sure the NCU is powered! The fastest and easiest way to check if the server is working or getting data:

1. Create a TCP client connection to the server:
   - For Windows, use this in your cmd window:
     ```sh
     telnet 208.68.36.87 2333
     ```
   - For macOS terminal:
     ```sh
     nc 208.68.36.87 2333
     ```
2. Type something randomly and press enter/return button (This means to send something to the server).
3. If the same string is sent back from the server, that means the server is working and the TCP service code is running.
   
   If nothing happens, or even you are not able to create the TCP connection, please ask firmware members or lead (whoever with access to NCU) to solve it, or send a message to @Andrew Yu on Slack.

### Note for NCU Admin to solve the problem:

1. `cd AndrewsServ/`
2. `php test.php start -d` (run the code in daemon mode)

Do everything starting from 2.1) to test if it’s working.

## 3. Check the router

Use another ethernet cable to connect the router to your PC, following the sticker on the router to access the manage page, to check the internet connection.

---

To be updated...
