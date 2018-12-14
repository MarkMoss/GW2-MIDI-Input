#ifndef MIDIParser_h
#define MIDIParser_h

#include "Arduino.h"

class MIDIParser
{
  public:
    enum Status : uint8_t {
      Success = 0,
      BufferFull,
      InvalidCommandCode,
      InvalidChannel
    };
    enum CommandCode : uint8_t {
      kNoteOff               = 0x80,
      kNoteOn                = 0x90,
      kPolyphonicKeyPressure = 0xA0,
      kControlChange         = 0xB0,
      kProgramChange         = 0xC0,
      kChannelPressure       = 0xD0,
      kPitchBend             = 0xE0,
      kSystemExclusive       = 0xF0,
      kTimeCode              = 0xF1,
      kSongPosition          = 0xF2,
      kSongSelect            = 0xF3,
      kReserved4             = 0xF4,
      kReserved5             = 0xF5,
      kTuneRequest           = 0xF6,
      kEndOfExclusive        = 0xF7,
      kTimingClock           = 0xF8,
      kReserved9             = 0xF9,
      kStart                 = 0xFA,
      kContinue              = 0xFB,
      kStop                  = 0xFC,
      kReservedD             = 0xFD,
      kActiveSensing         = 0xFE,
      kReset                 = 0xFF,
      kNone                  = 0x00,
      kUnknown               = 0x01
    };
   
    // Adds one byte (presumably from the MIDI stream) to the parser.
    MIDIParser::Status AddByte(const uint8_t b);

    void Reset(void);

  protected:
    virtual MIDIParser::Status OnNoteOff(uint8_t channel, uint8_t note, uint8_t velocity);
    virtual MIDIParser::Status OnNoteOn(uint8_t channel, uint8_t note, uint8_t velocity);
    virtual MIDIParser::Status OnPolyphonicKeyPressure(uint8_t channel, uint8_t note, uint8_t pressure);
    virtual MIDIParser::Status OnControlChange(uint8_t channel, uint8_t control, uint8_t value);
    virtual MIDIParser::Status OnProgramChange(uint8_t channel, uint8_t program);
    virtual MIDIParser::Status OnChannelPressure(uint8_t channel, uint8_t pressure);
    virtual MIDIParser::Status OnPitchBend(uint8_t channel, uint16_t pitchbend);
    virtual MIDIParser::Status OnSystemExclusive();
    virtual MIDIParser::Status OnTimeCode(uint8_t type, uint8_t value);
    virtual MIDIParser::Status OnSongPosition(uint16_t position);
    virtual MIDIParser::Status OnSongSelect(uint8_t song);
    virtual MIDIParser::Status OnReserved(uint8_t command);
    virtual MIDIParser::Status OnTuneRequest(void);
    virtual MIDIParser::Status OnTimingClock(void);
    virtual MIDIParser::Status OnStart(void);
    virtual MIDIParser::Status OnContinue(void);
    virtual MIDIParser::Status OnStop(void);
    virtual MIDIParser::Status OnActiveSensing(void);
    virtual MIDIParser::Status OnReset(void);
    
  private:
    // message_type_ and channel_ store the fields of the MIDI status 
    // (command) byte.
    MIDIParser::CommandCode message_type_;
    uint8_t channel_;

    // command_buffer_ contains the data bytes of the command currently being 
    //    received.
    // 
    // command_buffer_length indicates the maximum number of data bytes which
    //    the buffer can hold.
    //
    // command_length indicates the number of data bytes which have been 
    //    placed in the buffer.
    static const uint8_t command_buffer_length_ = 2;
    uint8_t command_buffer_[command_buffer_length_];
    uint8_t command_length_;

    MIDIParser::Status Parse(bool end_of_message = false);

    
    
};

#endif //#ifndef MIDIParser_h
