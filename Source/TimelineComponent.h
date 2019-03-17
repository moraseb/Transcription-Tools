#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "MarkerComponent.h"
#include "EditMarkerComponent.h"

/* Class that draws a timeline and markers */
class TimelineComponent :
public Component
{
public:
  TimelineComponent();
  ~TimelineComponent();
  
  /* Draws lines and text for timeline */
  void paint (Graphics&) override;
  
  /* Places makers on the component */
  void resized() override;
  
  /* Sets track length, which determines the where the lines are drawn */
  void setLength(double length);
  
  /* Creates and adds marker at given time */
  void addMarker(double time, String label, String text);
  
  /* Launches popup menu to edit marker */
  void mouseDown(const MouseEvent &event) override;
  
  /* Drags selected marker to new position */
  void mouseDrag(const MouseEvent &event) override;
  
  /* Sets new marker position */
  void mouseUp(const MouseEvent &event) override;
  
  /* Converts seconds (represented by a double) to a text of minutes and seconds */
  static String secondsToMinutesText(double time);
  
private:
  
  OwnedArray<MarkerComponent> markers;
  TooltipWindow tooltip;
  double trackLength;
  int markerX;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TimelineComponent)
};
