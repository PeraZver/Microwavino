Firmware is based on Arduino/C++ code that can be found in SW/Testing folder.
 
There are multiple code examples in that folder, but the one you need is 'Basic' example. This example contains basic, user configurable, functionality of the transmitter. In it, the user can choose multiple options: adjust PLL paramters of MAX2871, filter, attenuation, etc. User can also read out settings of MAX2871 chip as well as RSSI detector output. There are two libraries written for this example, MAX2871.h that contains everything related to MAX2871 operations, and MWTX.h that configures other blocks on Microwave Transmitter, such as filter switches, attenuation and RSSI readout.

The firmware is made for Teensy 3.2 but is also compatible with Teensy 3.1 and partially with LC. I say partially because the only function that will not work properly is RSSI readout due to the fact that Teensy LC has only one AD converter. Everything else works fine. 

***
### Initialization
The code begins with initialization of SPI port and pin mode settings of the Teensy. It makes sure that that all output and input pins are set properly and all chip-enable signals active. Function `MAX2871_Init ()` initializes MAX2871 chip. Initialization of MAX2871 is achieved by writing registers 5 to 0, twice. Registers of MAX2871 are 32 bits long, so we need 4 times 8-bit SPI transmission for one register transmission. Function `MAX2871_SPI_tx(uint32_t data)` handles this transmission. 

Switches SKY and attenutator PE4311 do not need special initialization procedure. Function `MWInit()` initializes switches for filter 4 (the widest bandwidth) and attenuation for 0 dB. 

### User interface

When the initialization is completed, program enters the loop mode where it waits for user input from serial terminal. Depending on received character, various functions are executed. For instance, functions where only one character is required, like `e`, `d` or `g` will do simple tasks: enable/disable RF output and read out register content of MAX2871. Other function require more characters to proceed. For instance, `rv`, `rt`, `px`, or `fx`, where different ADC settings or power outputs are selected. And finally, there are functions that require numbers to be entered, such as `Nxa`, `Fxa`, `Mxa`, `Rxa`, and `Dx`. These functions are setting PLL parameters of MAX2871 and output divider. Since the number x can have between 1 to 5 digits, it is necessary to create serial buffer that will store the number from serial input and convert it to integer. This is done in function `String2Int()`. To avoid large buffers and memory space waste, it is necessary to terminate input with character 'a'. This is how function `String2Int()` knows user entered desired value. Keep in mind that this doesn't apply to function `Dx`. This is because divider can only have 1-digit input (from 0 to 7) and serial buffer is not needed there. 

### Sweep

If Microwave Transmitter is ever going to be used in some real application, frequency sweep functionality is must-have feature. Frequency is swept by two parameters, N and F. This is implemented in function `MAX2871_Sweep(char divider_type, uint16_t a, uint16_t b, uint16_t dt)`. Function takes arguments `divider_type` that may be 'N' or 'F', 'a' and 'b' that are lower and upper sweep values, and 'dt' that is time step in milliseconds. From user interface, this function is selected by command `s`. As before, values for 'a', 'b' and 'dt' must be terminated with character 'a'. For example to sweep paramter F from 100 to 200 in 10ms steps, user must enter 'sF100a200a10a'.

***

Other examples in the SW/Testing/ folder are testing some functionality of various blocks used in Microwave transmitter, such as SPI with MAX2871 and PE4311 as well as filter selection. I used them to build 'Basic' program and code examples. 
