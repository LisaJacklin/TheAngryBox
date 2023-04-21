# TheAngryBox/ Reverse Bop-it
### EE 444: Embedded Systems

##### Team Members: 
- Audrey Eikenberry
- Jessica Pennock
- Lisa Jacklin

#### Background
Using the MSP430F5438A boards including several different sensors that we have chosen from to create, the angry box. Using CrossStudio for MSP430, we will be programming and incorporating the additional peripherals including, the LCD screen on the MSP430, accelorator, photocell, potentially audio capabilities, as well as the gyroscope for angle motion. Using all of these items, we will create an embedded system that will respond to the user in terms of the LCD screen and potentially even audio when ever motion occurs.

#### Potential Utilzations of the Angry Box
This embedded system could come in handle with applicances that must remain in a single place for a task, consider a laser leveler or perhaps an item that must only be stored right side up. This system could warn the user of motion and tell them to restore the original location or orientation to allow the main device to continue performing its assigned task.


### Tasks and Expectations
- [] Setting up the Accelerometer 
- [x] Transmit and recieve data from the accelerometer and the board
- [] Read and record data before moving on to the next ISR or task
- [x] Setting up the LCD screen to display strings
- [x] adjust the LCD screen based on the ISR reached and the response that was given via the Accel. and User
- [x] Set the button to display a message to the LCD screen based on button press.
- [ ] setting up battery power
- [ ] calculate how long the program lasts on battery power
- [x] creating a single master program including the LCD and accelerator program


##### Weekly Breakdown and Progress
 - [x] Week 1: planning out project and beginning research on peripheral sensors and the msp430f5438a LCD screen and header file included.
 - [x] Week 2: Got the LCD screen to operate both alone and with the inclusion of button ISR. No working program for the MSP430 and accelerometer together. Experiencing issues with I^2C communication.
 - [x] Week 3: LCd is ready to integrate in with the acclerator program. Main program for both the LCd and accelerator code has been created. The accelerator connection through I2C is still not operational.
 - [x] Week 4: LCD is working and a single project file has been created to move all accelerometer code as well as the required LCD code into the respected areas. This week several hiccups occured between transmission and recieving a single bit, where the response from the Oscilliscope was not reciprocated on CrossStudio. There is must still to be done, including creating the ISRs for reading the Z-axis at minimum as well as allowing data to be taken for more than a single byte at a type. Another item that must be addressed is being able to sample a signal from an accelerometer axis and translating that to a ISR depending on the range of variety.

#### Obstacles and Issues
- transmission lines do not line up between the oscilliscope measurements and CrossStudio; we get the correct output within the registers and the program, but there is at minimum difficulty viewing the transmissions for Start, Address, Read/Write, acknowledgement, and other following transmissions when considering only a single bit.
- 

### Learning, Changes, and Future
- adding a photocell to allow for different backlighting and contrast on the LCD screen based on the light level reported by the photocell.
- Organization and understanding of the Accelerometer chip
- Understanding how I2C transmissions work and which modules to use for each component.
- Learning how to check for the correct pin connections and when wires are dead.
- Adding the ability to draw power from a battery source and optimizing it.


