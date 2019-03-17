#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

/*  */
class SectionComponent : public Component
{
public:
  SectionComponent();
  ~SectionComponent();
  
  /* Sets color of section */
  void paint (Graphics&) override;
  
  /* Sets cursor depending on the position of the mouse */
  void mouseMove(const MouseEvent& event) override;
  
  /* Calls parent mouseDown if right clicked to allow rythm component functions */
  void mouseDown(const MouseEvent& event) override;
  
  /* Calls parent mouseDrag() to move or resize this component */
  void mouseDrag(const MouseEvent& event) override;
  
  /* Calls parent mouseUp() to set variables */
  void mouseUp(const MouseEvent& event) override;
  
  /* Checks if mouse is on the right of the component */
  bool isMouseOnRight();
  
private:
  
  bool mouseOnRight;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SectionComponent)
};
