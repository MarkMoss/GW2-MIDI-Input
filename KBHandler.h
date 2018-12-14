#ifndef KBHandler_H
#define KBHandler_H

#include "MIDIParser.h"

class KBHandler : public MIDIParser {
  public:
    typedef struct NoteTableEntry 
    {
      uint8_t channel_;
      uint8_t note_;
      char key_;
    };
    static const int note_table_size_ = 23;
    static const NoteTableEntry note_table_[];
  protected:
    
    
    MIDIParser::Status OnNoteOn(uint8_t channel, uint8_t note, uint8_t velocity) override;
    MIDIParser::Status OnNoteOff(uint8_t channel, uint8_t note, uint8_t velocity) override;
};
                     
                      

#endif //#ifndef KBHandler_H
