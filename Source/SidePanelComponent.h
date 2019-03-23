#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "MainAudioComponent.h"

/* Controls settings including playback, rhythm component, etc. */
class SidePanelComponent : public Component, private Timer
{
public:
  SidePanelComponent(MainAudioComponent& audioCompToUse);
  ~SidePanelComponent();
  
  void paint (Graphics&) override;
  void resized() override;
  
  /* Gives focus to parent component */
  void mouseDown(const MouseEvent& event) override;
  
  void setFileName(String name);
  
  double getBalance();
  
  bool isMono();
  bool isInverted();
  
private:
  
  /* Updates labels */
  void timerCallback() override;
  
  MainAudioComponent& audioComp;
  
  /* Info */
  String fileName;
  Label positionLabel;
  Label sectionLabel;
  
  /* Balance */
  Slider balanceSlider;
  ToggleButton monoButton;
  ToggleButton phaseButton;
  
  /* Rhythm */
  TextEditor numeratorEditor;
  TextEditor denominatorEditor;
  TextEditor repetitionEditor;
  TextEditor tempoEditor;
  
  
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SidePanelComponent)
};
