#include <Keyboard.h>

#include "KBHandler.h"

#define DebugConsole Serial
#define MidiPort Serial1
#define MidiPortBaud 32500

KBHandler Parser;

void setup() {
  DebugConsole.begin(115200);
  Keyboard.begin();
  MidiPort.begin(MidiPortBaud);
  
}

void loop() {
  /*  These variables keep track whether we have received a byte since last 
   *  resetting parser, and if so, at what time.
   */
  static boolean       msg_rx = false;
  static unsigned long msg_rx_at = 0;

  /*  Check if a new byte is available on the MIDI port. */
  if(MidiPort.available()){
    /* A byte is available.  Read it and update the message status variables.
     */
    uint8_t byte = (uint8_t)MidiPort.read();
    Parser.AddByte(byte);
    msg_rx = true;
    msg_rx_at = micros();
  } else if(msg_rx) {
    /* Reset parser if idle for more that 40 bit periods */
    //if((micros() - msg_rx_at) > (1000000*40/MidiPortBaud)){
    //  Parser.Reset();
    //  msg_rx = false;
    //}
  }
}



