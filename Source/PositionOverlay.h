#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

/* Component that marks the playback position of a track */
class PositionOverlay :
public Component,
private Timer
{
public:
  
  /* Constructor/Destructor */
  PositionOverlay(AudioTransportSource& transportSourceToUse);
  ~PositionOverlay();
  
  /* Draws line at playback Position */
  void paint (Graphics&) override;
  
private:
  
  /* Calls to repaint line */
  void timerCallback() override;
  
  AudioTransportSource& transportSource;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PositionOverlay)
};
