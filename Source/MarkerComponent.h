#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

/* Class that creates a marker with a red line on its left edga */
class MarkerComponent :
public Button
{
public:
  
  MarkerComponent(double time, String lbl, String text);
  ~MarkerComponent();
  
  /* Draws component as rectangle with rounded edges and label */
  void paintButton (Graphics &g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
  
  /* Setters/Getters */
  void setPosition(double time);
  void setLabel(String text);
  void setDescription(String text);
  double getPosition();
  String getLabel();
  String getDescription();
  
  /* Calls parent's (TimelineComponent) mouse methods */
  void mouseDown(const MouseEvent &event) override;
  void mouseDrag(const MouseEvent &event) override;
  void mouseUp(const MouseEvent &event) override;
  
private:
  
  double position;
  String label;
  String description;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MarkerComponent)
};
