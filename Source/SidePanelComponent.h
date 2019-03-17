#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "MainAudioComponent.h"

/* Controls settings including playback, rhythm component, etc. */
class SidePanelComponent : public Component
{
public:
  SidePanelComponent(MainAudioComponent& audioCompToUse);
  ~SidePanelComponent();
  
  void paint (Graphics&) override;
  void resized() override;
  
  /* Gives focus to parent component */
  void mouseDown(const MouseEvent& event) override;
  
private:
  
  MainAudioComponent& audioComp;
  
  /* Rhythm Section */
  TextEditor numeratorEditor;
  TextEditor denominatorEditor;
  TextEditor repetitionEditor;
  TextEditor tempoEditor;
  
  
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SidePanelComponent)
};
