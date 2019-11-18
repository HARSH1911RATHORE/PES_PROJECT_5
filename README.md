# PES_PROJECT_5- Principles of Embedded Software 

## Contributors - Utkarsh Dviwedi and Harsh Rathore
 
# Problem Statement 
 UART Communications with PC 
The project will have you connect to the KL25Z USB port to send and receive data from a PC-based terminal program. This setup will allow you to exercise more advanced firmware topics, including UART communication device drivers, circular buffers, and more on interrupts and timers. You will also capture oscilloscope or logic analyzer output and develop an FMEA document for the project.
This project is targeted to run on the KL25Z only – the program should (as in project 4) be capable of running in three modes:
1) In debug mode, with detailed debug messages.
2) In normal mode, with normal status messages.
3) In test mode, with detailed test messages.
You should also use a #DEFINE variable to select from blocking or non-blocking UART device driver use (see below), and a #DEFINE variable to select echo or application mode (see below).

## Part 1. (80 points) UART-based Communications with PC Terminal Program
Circular Buffer Functions
Develop a set of circular buffer interface functions for characters. These functions should be capable of working with multiple circular buffers by referencing a provided pointer to a circular buffer structure. You will likely have individual circular buffers active for the transmit queue and the receive queue.
• Define structure and dynamic memory allocation method for circular buffers
o Structure should include buffer pointer, head, tail, length, count
o Buffer functions should update these values as required
o Note that the maximum size of the buffer should equal the maximum number of elements – we will not use an empty byte as a wrap-around flag
• Define an enum for circular buffer errors (buffer functions should provide this as a return value)
• Function to add a new item to a buffer (should error if buffer is full)
• Function to remove oldest item from a buffer (should error if buffer is empty)
• Function to check is buffer full
• Function to check is buffer empty
• Function to verify buffer is initialized
• Function to verify the buffer pointer is valid
• Function to initialize a buffer
• Function to destroy a buffer
UART Device Driver 1 – Blocking/Polled
Develop a set of functions to implement polling of UART hardware. These functions should only use the predefined definitions in MKL25Z4.h (do not use higher-level functions provided by fsl*.h or other SDK includes). Note, for both UART drivers, use UART0 to have access to the full UART function set.
• Function for UART hardware initialization including baud rate and serial message format (parity, start/stop bits)
• Function to check whether the transmitter is available to accept a new character for transmission
• Function to transmit a character assuming transmitter is available.
• Function using these two functions to wait (block) for the transmitter to be available and then once available transmit a character and return
• Function to check whether the receiver has a new character to receive
• Function to receive the character assuming receiver has data
• Function using these two functions to wait (block) for the receiver to receive a new character and then return that character
• An echo function that uses the above functions to echo received characters one at a time back to the PC-based sender
UART Device Driver 2 – Non-blocking/Interrupt
Develop a set of functions for using interrupts to detect the completion of a send or receive operation. You are encouraged to share/reuse code from the Blocking/Polled version above. These functions should only use the predefined definitions in MKL25Z4.h (do not use higher-level functions provided by fsl*.h or other SDK includes).
• Function for UART hardware initialization as above with addition of interrupt configuration and enable (configure the KL25Z so the UART generates an interrupt when ready to transmit a character, when a character has been received, or when there is an error)
• An interrupt service routine (ISR) function to handle the interrupts. This function should handle transmit and receive interrupts as well as errors.
• Function to check whether the transmitter is available to accept a new character for transmission
• Function to transmit a character assuming transmitter is available
• Function called by the ISR to supply the next character for transmit (i.e. on the IRQ, check the transmit interrupt is enabled and the transmit buffer is empty before placing the next character into the data register)
• Function called by the ISR for receiving a character (i.e. on the IRQ, verify a character was received and read it from the data register)
• An echo function that uses the above functions to echo received characters one at a time back to the PC-based sender
Remember, you should not do any processing in the interrupt handler. The interrupt routine should be as short as possible. Each interrupt should clear associated flags when completed but only if they were set. Any processing should be done outside the interrupt context.
Echo Mode and Application Mode
If the #DEFINE for echo operation is in place, use the echo function in the device driver to send any received characters back to the PC terminal program.
If the #DEFINE for application operation is in place, run the following application:
Create an application that maintains a count of the unique characters that have been received by the UART device driver since the beginning of program execution. The report will show all characters received along with counts. Whenever the UART is able to transmit, send a formatted report to the PC terminal program. This report, and its associated data structure, should continue to accumulate characters and character counts for as long as a single execution is running.
For example, if the PC sent the characters in the word “Character”, the resulting report would look like (with data in ASCII value order):
C – 1; a – 2; c – 1; e – 1; h – 1; r – 2; s – 1; t – 1
Note that this report may be interrupted by incoming data from the PC, but once transmit is clear, the report should restart and attempt to send the report data to the PC again.
It is up to you to decided what triggers the printing of the report; perhaps, when some fixed number of characters has been processed, or perhaps if a particular character is processed. Your approach to this functionality needs to be documented appropriately in your software via comments and in the README observations or execution report.
Unit Testing
Using uCUnit, create a test suite to test the circular buffer code thoroughly. Examples:
• Create a buffer
• Test data access
• Verify wrap remove (test that your buffer tail point can wrap around the edge boundary when a remove occurs at the boundary)
• Verify wrap add (test that your buffer can wrap around the edge boundary and add to the front)
• Overfill (test that your buffer fails when too many items are added)
• Over empty (test that your buffer fails to remove an item when empty)
• Destroy the buffer
• Etc.
This test suite should be run anytime the program is executed in test mode.
Identify Critical Sections (operations that should not be interrupted)
Define two macro functions to disable and enable interrupts in any elements of the code you believe should be atomic, that is, should run without being interrupted.
#define START_CRITICAL()
#define END_CRITICAL()
Logging – add timestamps
Use the logging code implemented in Project 4 with the following extension:
• Modify enums as needed to track new modules/functions
• Add a timestamp to all logged messages
o Timestamps will be character strings in the form: “HH:MM:SS.n” where H = Hours, M = Minutes, S = Seconds, n = tenths of a second
o The timestamp will be based on time elapsed since program start
• Use the SysTick timer set to 10 Hz to provide a .1 second counter as a time reference; this timer should be enabled at the start of any program execution

• Once your UART drivers are working, use the UART drivers for logging instead of API functions. (This can be a good method to debug and verify UART driver 1.)
LEDs
• You may reuse your LED control function from any earlier projects
• The LED should be set as follows
o initialize to blue
o set to blue when the program moves to receiving or waiting to receive
o set to green when the program moves to transmitting or waiting to transmit
o red if there is any error condition detected
General
• For this project, you will develop using the MCUXpresso IDE environment. See the SAs for any assistance you need in your development environment.
• Your code should follow the ESE C Style Guide as closely as possible.
• When compiling use -Wall and -Werror compiler flags. Your code should have no compilation errors or warnings.
• You should use modular design for your code – UART drivers in one module, application code in another, etc.
Extra Credit (10 points) – Extending Circular Buffers
• Whether you’re using circular buffers to hold the report out data counts or transmit and receive data, you can have situations where the buffer overflows. In the code above, we simply throw an error when this occurs.
• For 10 points extra credit
o Create function(s) to add (reallocate) memory to resize a circular buffer
o Use this function in a situation (such as the report storage) when an overfill error occurs
o Include test of this function in the uCUnit test case suite

## Part 2 (10 points) – Capture Oscilloscope trace of UART traffic between KL25Z and the PC
You will need to capture a character transaction while in Echo mode showing the receive and transmit operations between the PC and the KL25Z.
Any clear image of the two transactions is fine. You should be able to annotate the image of the transaction to show key fields (start, data, stop, etc.) being transferred in your PDF submission.
If you have access to a logic analyzer, you could use that as an alternate for capturing the transaction waveforms.

## Part 3 (10 points) – FMEA (Failure Modes and Effects Analysis) document for the project
Create an Excel (or other) spreadsheet with the following columns:
• Potential Failure Mode (how could the failure occur)
• Potential Failure Effect (what is the impact of the failure)

• Severity (1-10) (how severe is the issue, 10=most severe)
• Potential Cause (what could make this happen)
• Occurrence (1-10) (how frequently would this happen, 10=most frequent)
• Controls (What is in place to prevent either the cause or the failure)
• Detection (1-10) (how hard is this error to detect, 10 = hardest)
• Risk Priority Number = Severity * Occurrence * Detection
Pick six error modes you can think of in the interactions between the PC and the KL25Z. Fill out the columns with your observations on the error mode and determine the Risk Priority Number for the risk.

## Description of Repository contents

 1. **UART_TEST.c**   --Contains the main function , UART, Functions and Initializations
 2. **UART.h**  --Contains defined macros, include files and function declarations
 3. **logger.c**   --Contains the Logger functions 
 4. **UART.c**  --Contains all functions needed by UART driver
 5. **UART.h** --Contains the declarations for UART code
 6. **system.c**   -- Unit test Library file
 7. **system.h**  -- Unit test Library file
 8. **uCUnit.h**  -- Unit test Library file
 9. **CircularBuffer.c** --Contains all functions needed to implement circular buffer
 10. **CircularBuffer.c** --Contains the declarations for circular buffer code
 11. **LED.c** --Contains all the functions and initialisation requried for the LED to function
 12. **LED.h** --Contains all declarations for led.c

## OBSERVATIONS

 1. **Circular Buffer** -The Circular buffer code ran perfectly when tested seperately for it. When integarted with the Uart code, it seemed to throw a lot of errors together with garbage values on the terminal host.
 2. **Logger** - Implementing the logger function to print the status of test, debug and status mode turned out to be very time consuming as all the functions messages were called by a single logger function with the use of loggers for different states. Also ha d to define #if for each finction to print the messages of them in different modes as needed by the user.
 3. **Uart** - The uart throwed minimal warnings but did not seem to work at the start because of a different baud rate and because the clock frequency applied was two times it was required for smm=ooth uart communication. Changing the systick clock in the baud rate calculation to half helped in getting proper results.



## Installation and Execution Notes
 1. **LED INIT and MUX** - We need to initialize the GPIO of the LEDs and directions of PTB18(Red LED) ,PTB19(Green LED) and PTBD1(Blue LED)
 2. **Set the Unit test and logger Mode flags** There are global Flags for logger and Unit test , Set the debug flag to 1 to run it in the debug mode and the unit test flag to 1 to run the unit tests.
 3. **Logger messagez** The logger messages are printed using the uart driver functions
 4. **Defines** #defines and #if are used at various places to run a particular mode and to print messages for a particular state.
