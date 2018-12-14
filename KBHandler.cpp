#include <Keyboard.h>
#include "KBHandler.h"

const KBHandler::NoteTableEntry KBHandler::note_table_[] = {{0, 48, '1'},
                                                            {0, 50, '2'},
                                                            {0, 52, '3'},
                                                            {0, 53, '4'},
                                                            {0, 55, '6'},
                                                            {0, 57, '7'},
                                                            {0, 59, '8'},
                                                            {0, 60, '9'},
                                                            {0, 62, '2'},
                                                            {0, 64, '3'},
                                                            {0, 65, '4'},
                                                            {0, 67, '6'},
                                                            {0, 69, '7'},
                                                            {0, 71, '8'},
                                                            {0, 72, '9'},
                                                            {1, 48, '1'},
                                                            {1, 50, '2'},
                                                            {1, 52, '3'},
                                                            {1, 53, '4'},
                                                            {1, 55, '5'},
                                                            {1, 57, '6'},
                                                            {1, 59, '7'},
                                                            {1, 60, '8'},
                                                           };

MIDIParser::Status KBHandler::OnNoteOn(uint8_t channel, uint8_t note, uint8_t velocity){
  char key;
  for(int i=0; i<note_table_size_; i++){
    if(note_table_[i].channel_ == channel && note_table_[i].note_ == note){
      Keyboard.write(note_table_[i].key_);
    }
  }
  
  return Success;
}


MIDIParser::Status KBHandler::OnNoteOff(uint8_t channel, uint8_t note, uint8_t velocity){
  //char key;
  //for(int i=0; i<note_table_size_; i++){
  //  if(note_table_[i].channel_ == channel && note_table_[i].note_ == note){
  //    Keyboard.release(note_table_[i].key_);
  //  }
  //}
  
  return Success;
}


