# Circuit design

The microwave transmitter consists of following blocks:


* Synthesizer (VCO) based on MAX2871 chip and accompanying passives. This guy is able to synthesize frequencies from 25 MHz to 6 GHz.
* Filter bank that makes sure no harmonics are generated in GHz range. There are 4 low pass filters in ranges of 1, 2, 4, and 6 GHz and these are selected via two SP4T switches.
* Amplification block that uses programmable attenuator for the gain control and power amplifier TRF37A75 that should be able to deliver 12dBm output power. I've left some room for matching capacitors at the output to optimize PA's performance.
* Coupler and RSSI detectors that measure emitted and reflected power, of course. Coupler is modified version of Henrik's coupler and I took it as it is, no prior FEM simulation performed.
* Teensy 3.2 that controls the synthesizer, selects filter switches, controls the attenuation and AD converts measured power from the RSSI detectors and sends all of data to PC. Henrik originally used a bit more powerful LPC1765 microcontroller but I went for Teensy because it's easier to use.
* Power supply that scales 12V input to 5V, 3.6V, 3.3V and 1.8V. 5V supply is used exclusively by power amplifier while all other devices use 3.3V only. 3.6V is used just for additional 3.3V low noise LDO regulator that feeds the RF part of MAX2871. 1.8V source is meant for reference clock generator only. It is worth noting that no power is taken from the Teensy board.

# Board layout

## Layer stackup

Two-layer PCB won't be enought for the circuit of this complexity. Luckily, OSH Park where I plan to do the fabrication, offers 4 layer PCBs with the following characteristics:

| **Thickness**	| Material	| Usage                |      **Layer** |
|---------------|---------------|----------------------|---------------|
| 1.4mil        |	1oz Cu  |  signaling and power | top layer (L1)|
|  6.7mil       |prepreg| | |	
|  0.7mil	|0.5oz Cu | solid signal ground | 1st inner layer (L2)|
|   47mil	|core| | |	
|  0.7mil	|0.5oz Cu|	signaling,coupler and 3V3 power plane|	2nd inner layer (L3)|
|  6.7mil       | prepreg | | |	
|  1.4mil	|1oz Cu | power plane / ground |   bottom layer (L4)|

Both core and prepreg are FR408, with a dielectric constant of 3.66 at 1GHz. Total board thickness is 1.6mm.

## Routing

The most sensitive part of PCB layout are microwave traces. These need to be routed with special care and well shielded. I decided to go for the so called planar waveguide topology on the top layer and used Saturn PCB calculator for dimensioning. Knowing that layer thickness between the L1 and L2 (GND plane) is 6.7mil, to get 50 Ω line I'd need 14mil wide conductor with 10mil gap. It is important to have solid noninterrupted return (GND) plane beneath the RF tracks on the L2. I did via stitching at the PCB edge and next to the waveguide to achieve the best possible shielding. 

The coupler is designed in the stripline topology, meaning that it consists of the 2 coupled lines on the L3 that are ground-shielded between top and bottom layers. There is a gap in the L2 layer just above the coupler structure. This makes a rather asymmetrical situation as the stripline has 47+6.7=53.7mil of dielectric above the structure and only 6.7mil of dielectric below. But,if it has worked for Henrik, I see no reason why it won't work for me as well.

Power tracks should be as short as possible and far away from the sensitive RF and signaling tracks. It would be the best if the power tracks would also have noninterrupted return path. The return paths of power and digital signals should not interweave in any case. I used power plane for 3.3V on the L3 and L4 and one smaller 5V plane on the L1. All other planes on L1, L2 and L4 are dedicated to the GND. 