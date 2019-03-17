#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "MarkerComponent.h"
#include "TimelineComponent.h"

/* Content component for the popup window to edit a marker */
class EditMarkerComponent : public Component
{
public:
  EditMarkerComponent(MarkerComponent* markerToEdit, double length);
  ~EditMarkerComponent();
  
  /* Draws text */
  void paint (Graphics&) override;
  
  /* Places child components */
  void resized() override;
  
  /* Unfocuses chidren */
  void mouseDown(const MouseEvent &event) override;
  
private:
  
  MarkerComponent* marker;
  double trackLength;
  TextEditor labelEditor;
  TextEditor descriptionEditor;
  Slider positionSlider;
  TextButton cancelButton;
  TextButton okButton;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditMarkerComponent)
};
