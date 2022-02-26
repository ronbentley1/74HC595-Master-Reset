# 74HC595-Master-Reset
// Objective of the sketch
// ^^^^^^^^^^^^^^^^^^^^^^^
// To show the operation of the 74HC595 shift register master reset (MR)
// function.
//
// Approach
// ^^^^^^^^
// To achieve our objective, we need to bring the SIPO IC's master reset pin
// (MR, but often labelled SRCLR) under programmatic control. 
//
// For this, we allocate a digital microcontroller pin and connect this to the
// MR (SRCLR) pin of the SIPO IC.
//
// By lowering and raising the signal level of this pin (LOW/HIGH, 0/+5v)
// we are able to produce a complete SIPO IC reset as necessary.
//
// To show this operation the sketch uses a simply connected button switch
// to toggle the SIPO IC's reset function and noramal SIPO IC updates.
//
