#include "../JuceLibraryCode/JuceHeader.h"
#include "KeyboardComponent.h"

KeyboardComponent::KeyboardComponent() :
keyboardState(), keyboard(keyboardState, MidiKeyboardComponent::Orientation::horizontalKeyboard)
{
  keyboard.setAvailableRange(21, 108);
  
}

KeyboardComponent::~KeyboardComponent()
{
}

