#Proto Pedal Test Plan.

## Prologue

Assemble the protopedal by soldering the PTH components to the PCB.  If you're intending to interface with a case, it's best to use the case as an assembly jig, to hold the TRS jacks and stomp switch.  Be sure the following have been connected:

* 2x TRS jacks
* 9V battery snap with regard for color coding.
* Barrel jack.


## Background

*The stuff it's easy to forget when testing this device*

Power circuits are completed by connecting a 1/4" TS jack  to the input terminal.  The sleeve of the jack bridges the ring terminal to ground, completing the power circuit.

The barrel jack is sleeve-positive, tip-negative, as per the Roland/Boss standard.

The barrel jack should disconnect the 9V battery when it's connected.  We'll test this below!


## Materials

* Multimeter with continuity, VDC and IDC modes.
* Adjustable DC power supply with alligator clip wires and Ammeter display. 
	* If not stated in test, configure supply for 9VDC output, with 25 mA current limit.   
* Barrel jack to raw wire pigtail, with *tip* and *barrel* clearly marked.
* 1/4" TS jack (this can be a loose jack, or on a TS cable).  *Be certain that it is not a TRS jack!*




## Power Related Tests

1. Sanity check against obvious shorts
	* Measure continuity between VCC and ground pads.
	* **Criteria:** should read open-circuit from VCC to Ground. 

2. Verify input jack form ground connection
	* Measure continuity from TP4 (input ring, adjacent to input test point) to GND.
	* **Criteria:**  should measure open.
	* Connect TS jack to input.
	* Measure continuity from TP4 to GND.
	* **Criteria:** Should measure short.

3. Verify VAUX is floating
	* Measure continuity from Vaux to VCC
	* **Criteria:** should measure open
	* Measure continuity from Vaux to GND
	* **Criteria:** should measure open 

3. Verify battery snap polarity.
	* Connect power supply to 9V battery snap.  
		* The larger terminal is +V
		* The smaller terminal is ground
	* Set power supply to current limit at 25 mA.
	* Switch supply on. 
	* **Criteria:** Supply should indicate no current flowing.
	* Connect 1/4" TS jack to input.
	* Measure voltage between VCC and GND.
	* **Criteria:** Should measure supply voltage between VCC and GND
	* Switch the stomp switch.
	* **Criteria:** SMT LED should toggle on and off
	* **Criteria:** While LED is on, supply current should read ~5 mA.
	* Turn power supply off, remove input jack, disconnect battery snap.

4.  Verify 9V battery reverse polarity protection
	* Connect power supply to 9V battery snap, now reversed.  
		* The larger terminal is Ground
		* The smaller terminal is +V
	* Set power supply to current limit at 25 mA.
	* Switch supply on. 
	* **Criteria:** Supply should indicate no current flowing.
	* Connect 1/4" TS jack to input.
	* Measure voltage between VCC and GND.
	* **Criteria:** Should measure at most 10 mV DC, supply indicating no measurable current.
	* Switch the stomp switch.
	* **Criteria:** SMT LED should remain dark
	* Turn power supply off, remove input jack, disconnect battery snap.

5. Verify barrel jack polarity.
	* Connect power supply to barrel jack with pigtails  
		* Barrel is +V
		* Tip is ground
	* Set power supply to current limit at 25 mA.
	* Switch supply on. 
	* **Criteria:** Supply should indicate no current flowing.
	* Connect 1/4" TS jack to input.
	* Measure voltage between VCC and GND.
	* **Criteria:** Should measure supply voltage between VCC and GND
	* Switch the stomp switch.
	* **Criteria:** SMT LED should toggle on and off
	* **Criteria:** While LED is on, supply current should read ~5 mA.
	* Turn power supply off, remove input jack, disconnect battery snap.

6. Verify barrel jack reverse polarity protection
	* Connect power supply to barrel jack with pigtails  
		* Barrel is ground
		* Tip is +V
	* Set power supply to current limit at 25 mA.
	* Switch supply on. 
	* **Criteria:** Supply should indicate no current flowing.
	* Connect 1/4" TS jack to input.
	* Measure voltage between VCC and GND.
	* **Criteria:** Should measure at most 10 mV DC, supply indicating no measurable current.
	* Switch the stomp switch.
	* **Criteria:** SMT LED should remain dark
	* Turn power supply off, remove input jack, disconnect battery snap.


7. Verify Barrel Jack Defeats battery. 
	* Connect power supply to 9V battery snap.  
		* The larger terminal is +V
		* The smaller terminal is ground
	* Disconnect barrel jack pigtails from all connections 
	* Insert TS jack into input.
	* Toggle stomp switch until SMT LED comes on.
	* **Criteria:** Supply should indicate ~5 mA DC draw.
	* Insert disconnected barrel into socket.
	* **Criteria:** Supply should indicate 0 mA, LED should go dark.
	* Remove barrel from socket.
	* **Criteria:** LED should illuminate, current returns to ~5 mA. 
 
8. Verify Power Input Range
	* Connect power supply to barrel jack with pigtails  
		* Barrel is +V
		* Tip is ground
	* Plug TS into input.
	* Turn on power supply at 9 VDC.
	* Toggle stomp switch until LED comes on.
	* Slowly ramp PSU up to 18 VDC.
	* **Criteria:** No catastrophic failure, excess heat or excess current draw (typical current draw will be about ~15 mA).  
	
## Switching Related Tests.

1. Prerequesite configuration
	* These teses assume that the DUT has already passed the power section tests.  They also share a common setup:
		* TS connector plugged into intput jack.
		* 9VDC connected via barrel connector
			* barrel is +9VDC
			* tip is ground
		* These tests will call for DUT to switch between active and bypass states.  The SMT LED is illuminated when the DUT is in the active state. 

1. Verify Bypass state is functional
	* Set switch to bypass. SMT LED will be off.
	* Measure resistance from input tip connection to left side of C1 (at lower left corner of PCB) 
	* **Criteria:** External side of C1 is floating WRT input tip
	* Measure resistance from input tip to output tip.
	* **Criteria:** tip-to-tip shows continuity.
	* Measure resistance from left side of C1 to ground
	* **Criteria:** LHS of C1 continuous with ground
	* Measure RHS of C2  (adjacent to battery leads) to tip of output
	* **Criteria:** RHS of C2 is floating WRT output tip
	* Measure RHS of C2 to ground
	* **Criteria:** RHS of C2 is 1Meg to ground.  
	* 
2. Verify active state is functional
	* Set switch to active state.  SMT LED will illuminate
	* Measure voltage between RHS of R4 (PTH resistor marked "BRT") and ground
	* **Criteria:** Vcc (~9VDC) present at RHS of R4.
	* Measure resistance from input tip to ground.
	* **Criteria:** Input tip is is 1Meg to ground.  
	* Measure resistance from input tip to LHS C1.
	* **Criteria:** Input tip is contonuous with LHS C1
	* Measure resistance input tip to output tip
	* **Criteria:** Resistance tip-to-tip is 2 Meg.
	* Measure resistance RHS of C2 to output tip
	* **Criteria:** Output tip is contonuous with RHS C2
	* Measure resistance output tip to ground 
	* **Criteria:** Output tip is is 1Meg to ground.
3. Verify SJ1 defeats SMT LED.
	* Remove power.
	* Clear solder jumper SJ1 
