# Needed hardware and software
Hardware Needed (Those in parenthesis are what was used in testing):
- ESP32 (ESP-WROOM-32)
- Servo (SG90 Micro servo)
- Power Supply for Servo (Arduino Nano 5V)

Other Components used:
- 3D Printed servo holder
- Popsicle Stick for holding ESP32
- Zip ties
- Blue Tack

Software Needed (Where the software should be installed):
- Arduino IDE (Desktop)
- MATLAB Application (Desktop) *Cannot run on MATLAB online
- nRF Connect (Android Mobile Phone) *Does not work with Apple devices as they do not let you set a custom UUID

# Setup
Setting up the nRF Connect Application:
1. Go to any browser and search 'UUID Generator'
2. Generate and copy the UUID 
3. Download the nRF Connect Application
4. Open application and go to Advertiser
5. Press the '+' icon on the bottom right
6. Set a display name (any works, this is for your own convenience)
7. Press 'Add Record' and select 'Service UUID'
8. Paste the generated UUID and press 'OK'
9. Under Options, select 'Connectable' and ensure 'Discoverable' is selected
10. Click 'OK'
11. Activate it by pressing on the slider in the upper right until it turns from gray to blue

Setting up the hardware components:
1. Connect the servo to the ESP32 *pin 22 is the default in the program but can be changed at 'servo1.attach(22)'
2. Connect the servo to the dedicated power source
3. Mount the ESP32 onto the servo, ensuring that the ESP32 is pointed horizontally

Setting up the desktop software components:
1. Download the Arduino program and MATLAB program from this repository
2. Open both the Arduino and MATLAB program
3. Connect the ESP32 to the desktop and upload the Arduino program
4. Double-check if the IP address displayed in the Serial Monitor of the Arduino IDE is the same as the one in MATLAB. If not, change it to match.
5. Disconnect from your current Wifi network and connect to the network named 'ESP32-Access-Point' with the password '123456789' *This is the default and can be altered in the program

# Running the main MATLAB application
Running the MATLAB program:
1. Click the 'Run' icon
2. Once the program is done, Signal Analyzer should open with the graphs and the Command Window should display the direction it chose
