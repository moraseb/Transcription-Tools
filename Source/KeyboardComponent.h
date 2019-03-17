#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

/* Class that creates a MIDI keyboard that plays
 * GM sounds and proves information when left clicked
 */
class KeyboardComponent : public Component
{
public:
  KeyboardComponent();
  ~KeyboardComponent();
  
private:
  
  MidiKeyboardState keyboardState;
  MidiKeyboardComponent keyboard;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeyboardComponent)
};
