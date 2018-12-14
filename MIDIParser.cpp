#include "MIDIParser.h"

#define DebugConsole Serial

MIDIParser::Status MIDIParser::AddByte(uint8_t b){
  MIDIParser::Status status;
  // Check if this is a status byte (msb set) or a data byte (msb clear).
  if(b > 0x7F){
    // Status byte.  This indicates the start of a new command.
    // If there is already an unhandled command in the buffer, attempt to 
    // handle it.
    if(message_type_ != kNone){
      Parse(true);
    }
    //Clear the data buffer.
    command_length_ = 0;
    
    //  Determine if this is a channel command (high nibble 0x8 
    //  through 0xE) or if this is a system command (high nibble 0xF).
    if(b > 0xEF) {
      // System command.
      //
      // Command code is the entire byte.  Channel is 0.
      message_type_ = b;
      channel_ = 0;
      Parse();
    } else {
      // Channel command.
      //
      // Command code is the high nibble.  Channel is the low nibble.
      message_type_ = b & 0xF0;
      channel_ = b & 0x0F;
      Parse();
    }
  } else {
    // Data byte.
    // Check if there is space in the buffer
    if(command_length_ < command_buffer_length_){
      // Space available, add the data byte.
      command_buffer_[command_length_] = b;
      command_length_++;
      //Attempt to parse the command.
      return Parse();
    } else {
      return BufferFull;
    }
  }
}


MIDIParser::Status MIDIParser::Parse(boolean end_of_message = false){
  MIDIParser::Status status;
  //Note off message.
  //First data byte is the note, second is the velocity.
  if(message_type_ == kNoteOff && command_length_ == 2){
    status = OnNoteOff(channel_, command_buffer_[0], command_buffer_[1]);
    Reset();
    return status;
  }
  //Note on message.
  //First data byte is the note, second is the velocity.
  if(message_type_ == kNoteOn && command_length_ == 2){
    status = OnNoteOn(channel_, command_buffer_[0], command_buffer_[1]);
    Reset();
    return status;
  }
  //Polyphonic key pressure message.
  //First data byte is the note, second is the pressure.
  if(message_type_ == kPolyphonicKeyPressure && command_length_ == 2){
    status = OnPolyphonicKeyPressure(channel_, command_buffer_[0], command_buffer_[1]);
    Reset();
    return status;
  }
  //Control change message.
  //First data byte is control number, second is control value.
  if(message_type_ ==kControlChange && command_length_ == 2){
    status = OnControlChange(channel_, command_buffer_[0], command_buffer_[1]);
    Reset();
    return status;
  }
  //Program change message.
  //Single data byte contains the program number.
  if(message_type_ == kProgramChange && command_length_ == 1){
    status = OnProgramChange(channel_, command_buffer_[0]);
    Reset();
    return status;
  }
  //Channel pressure message.
  //The single data byte contains the pressure to apply to all notes.
  if(message_type_ == kChannelPressure && command_length_ == 1){
    status = OnChannelPressure(channel_, command_buffer_[0]);
    Reset();
    return status;
  }
  // Pitch bend message.
  // The two data bytes contain a 14 bit-value with the seven least-significant bits 
  // in the first data byte and the seven most significant bits in the second byte. 
  // In both bytes, the data is right-justified.
  if(message_type_ == kPitchBend && command_length_ == 2){
    
    status = OnPitchBend(channel_, 
                         (uint16_t)command_buffer_[1] << 7 ||
                         (uint16_t)command_buffer_[0]);
    Reset();
    return status;
  }
  //System exclusive message.
  //Data is variable length with the end delimited by an "end of exclusive" message.
  if(message_type_ == kSystemExclusive & end_of_message == true){
    status = OnSystemExclusive();
    Reset();
    return status;
  }
  // Time code.
  // The single data byte consists of a three-bit message field in the most-
  // significant nibble and a 4-bit value in the least-significant nibble.
  if(message_type_ == kTimeCode && command_length_ == 1){
    status = OnTimeCode((command_buffer_[0] & 0x70) >> 3,
                        (command_buffer_[0] & 0x0F)      );
    Reset();
    return status;
  }
  //Song position
  // The two data bytes contain a 14 bit-value with the seven least-significant bits 
  // in the first data byte and the seven most significant bits in the second byte. 
  // In both bytes, the data is right-justified.
  if(message_type_ == kSongPosition && command_length_ == 2){
    // Song position is a 14 bit-value with the seven least-significant bits
    // in the first data byte and the seven most significant bits in the 
    // second byte.  In both bytes, the data is right-justified.
    status = OnSongPosition((uint16_t)command_buffer_[1] << 7 ||
                            (uint16_t)command_buffer_[0]
                           );
    Reset();
    return status;
  }
  // Song select.
  // The single data byte contains the song.
  if(message_type_ == kSongSelect && command_length_ == 1){
    status = OnSongSelect(command_buffer_[0]);
    Reset();
    return status;
  }
  // Reserved messages
  // Since the data length is unknown, don't process unless/until end_of_message is true.
  if(end_of_message &&
        (message_type_ == kReserved4 ||
         message_type_ == kReserved5 ||
         message_type_ == kReserved9 ||
         message_type_ == kReservedD)){
    status = OnReserved(message_type_);
    Reset();
    return status;
  }
  // TuneRequest
  // No data.
  if(message_type_ == kTuneRequest){
    status = OnTuneRequest();
    Reset();
    return status;
  }
  // Timing clock
  // No data.
  if(message_type_ == kTimingClock){
    status = OnTimingClock();
    Reset();
    return status;
  }
  // Start
  // No data.
  if(message_type_ == kStart){
    status = OnStart();
    Reset();
    return status;
  }
  // Continue
  // No data.
  if(message_type_ == kContinue){
    status = OnContinue();
    Reset();
    return status;
  }
  // Stop
  // No data.
  if(message_type_ == kStop){
    status = OnStop();
    Reset();
    return status;
  }
  // Reset
  // No data.
  if(message_type_ == kReset){
    status = OnReset();
    Reset();
    return status;
  } 
}


void MIDIParser::Reset(){
  message_type_ = kNone;
  command_length_ = 0;
}


MIDIParser::Status MIDIParser::OnNoteOff(uint8_t channel, uint8_t note, uint8_t velocity){
  return Success;
}

MIDIParser::Status MIDIParser::OnNoteOn(uint8_t channel, uint8_t note, uint8_t velocity){
  return Success;
}

MIDIParser::Status MIDIParser::OnPolyphonicKeyPressure(uint8_t channel, uint8_t note, uint8_t pressure){
  return Success;
}

MIDIParser::Status MIDIParser::OnControlChange(uint8_t channel, uint8_t control, uint8_t value){
  return Success;
}

MIDIParser::Status MIDIParser::OnProgramChange(uint8_t channel, uint8_t program){
  return Success;
}

MIDIParser::Status MIDIParser::OnChannelPressure(uint8_t channel, uint8_t pressure){
  return Success;
}

MIDIParser::Status MIDIParser::OnPitchBend(uint8_t channel, uint16_t pitchbend){
  return Success;
}

MIDIParser::Status MIDIParser::OnSystemExclusive(){
  return Success;
}

MIDIParser::Status MIDIParser::OnTimeCode(uint8_t type, uint8_t value){
  return Success;
}

MIDIParser::Status MIDIParser::OnSongPosition(uint16_t position){
  return Success;
}

MIDIParser::Status MIDIParser::OnSongSelect(uint8_t song){
  return Success;
}

MIDIParser::Status MIDIParser::OnReserved(uint8_t command){
  return Success;
}

MIDIParser::Status MIDIParser::OnTuneRequest(void){
  return Success;
}

MIDIParser::Status MIDIParser::OnTimingClock(void){
  return Success;
}

MIDIParser::Status MIDIParser::OnStart(void){
  return Success;
}

MIDIParser::Status MIDIParser::OnContinue(void){
  return Success;
}

MIDIParser::Status MIDIParser::OnStop(void){
  return Success;
}

MIDIParser::Status MIDIParser::OnActiveSensing(void){
  return Success;
}

MIDIParser::Status MIDIParser::OnReset(void){
  return Success;
}



