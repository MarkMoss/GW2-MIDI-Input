# GW2-MIDI-Input
Simple tool to convert MIDI NoteOn commands to USB keyboard key presses.

By default, the tool converts the following notes to key presses, meant for using a simple 2-octave MIDI keyboard with the Guild Wars 2 "Bell Chior" activity:

* C3 - F3 to keys 1 - 4.
* G3 - C4 to keys 5 - 9.
* D4 - F4 to keys 1 - 4.
* G4 - C5 to keys 5 - 9.


This can be customized by editing a table in KBHandler.cpp.

## Pre-Requisites

* An Arduino board capable of emulating a USB keyboard (tested with an Arduino Leonardo).
* An Arduino MIDI shield.
* Some sort of MIDI device (tested with a cheap 2-octave MIDI keybard).
