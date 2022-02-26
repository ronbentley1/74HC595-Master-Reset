//
// 74HC595 Programmatic Master Reset
//
// Ron Bentley, Stafford UK
// February 2022
//
// This example and code is in the public domain and may be used without
// restriction and without warranty.
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Example sketch - Demonstration of Programmatic Control of the 
//                  74HC595 Shift Register Master Reset(MR) Function
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Objective of the sketch
// ^^^^^^^^^^^^^^^^^^^^^^^
// To show the operation of the 74HC595 shift register master reset (MR)
// function.
//
// Approach
// ^^^^^^^^
// To achieve our objective, we need to bring the SIPO IC's master reset pin
// (MR, but often labelled SRCLR)under programmatic control. 
// For this, we allocate a digital microcontroller pin and connect this to the
// MR (SRCLR) pin of the SIPO IC.
//
// By lowering and raising the signal level of this pin (LOW/HIGH, 0/+5v)
// we are able to produce a complete SIPO IC reset as necessary.
// To show this operation the sketch uses a simply connected button switch
// to toggle the SIPO IC's reset function and noramal SIPO IC updates.
//
// The sketch uses two libraries:
// 1. the ez_switch_lib library to define and control a simply connected
//    button switch, and
// 2. the ez_SIPO8_lib library to define and control a single 74HC595
//    shift register.
// These libraries may be accessed and downloaded via the ARDUINO Project Hub:
//
// 1. ez_switch_lib library-
//    https://create.arduino.cc/projecthub/ronbentley1/a-switch-library-for-arduino-dfbe40?ref=user&ref_id=1455180&offset=17
//
// 2. ez_SIPO8_li library-
//    https://create.arduino.cc/projecthub/ronbentley1/command-control-of-multiple-serial-in-parallel-out-ics-3e0b1a?ref=user&ref_id=1455180&offset=16
//
// If you wish to read a primer for SIPO ICs/shift registers then have a look at the link below:
//    https://lastminuteengineers.com/74hc595-shift-register-arduino-tutorial/
//
// ****
// declare SIPO IC data 
#include <ez_SIPO8_lib.h>

SIPO8 my_SIPOs(1, 0); // configure for one SIPO IC, no timers for this sketch

uint16_t bank_id;

#define data_pin     8
#define latch_pin    9
#define clock_pin   10
#define IC_MR_pin   11

bool IC_MR_status;

// ****
// declare switch data
#include <ez_switch_lib.h>

Switches my_switch(1); // establish just one switch for this sketch

int switch_id;

#define my_switch_pin 12

void setup() {
  // ****
  // set up switch - a button switch, simply wired without a pulldown resistor
  switch_id = my_switch.add_switch(button_switch, my_switch_pin, circuit_C2); // configure the switch
  
  // ****
  // deal with initialisation of the MR reset pin
  pinMode(IC_MR_pin, OUTPUT);
  IC_MR_status = HIGH; // start with a HIGH signal, ie IC active status, ready for data
  digitalWrite(IC_MR_pin, IC_MR_status);

  // ****
  // set up the SIPO - one SIPO IC in this bank on these control pins:
  bank_id = my_SIPOs.create_bank(data_pin, clock_pin, latch_pin, 1);
  // now set all of the IC's output pins HIGH (LEDs on).
  my_SIPOs.set_all_array_pins(HIGH);
  my_SIPOs.xfer_array(MSBFIRST);
}

void loop() {
  do {
    // only process if the button switch has been toggled:
    // alternative switch presses perform IC resets and IC updates, respectively
    if (my_switch.read_switch(switch_id) == switched) {
      IC_MR_status = !IC_MR_status;    // invert current IC_MR_status and process
      if (IC_MR_status == LOW) {
        // request to reset the SIPO IC via the MR pin, so reset the IC and
        // ensure output ports also reflect cleared status by toggling latch pin
        digitalWrite(IC_MR_pin, LOW);  // reset IC - drop MR reset pin +5v signal
        digitalWrite(latch_pin, LOW);  // inform IC to latch data from the register to the output ports
        digitalWrite(latch_pin, HIGH);
        digitalWrite(IC_MR_pin, HIGH); // set IC back to active status
      } else {
        // update the SIPO with all outputs raised HIGH
        my_SIPOs.set_all_array_pins(HIGH);
        my_SIPOs.xfer_array(MSBFIRST); // send update to all ouput ports
      }
    }
  } while (true);
}
