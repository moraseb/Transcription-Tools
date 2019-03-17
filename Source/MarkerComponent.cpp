#include "../JuceLibraryCode/JuceHeader.h"
#include "MarkerComponent.h"

MarkerComponent::MarkerComponent(double time, String lbl, String text) : Button("")
{
  position = time;
  label = lbl;
  description = text;
  setTooltip(description);
}

MarkerComponent::~MarkerComponent() {}

void MarkerComponent::paintButton(Graphics &g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) {
  
  // Rounded Rectangle
  g.setColour(Colour(140, 0, 0));
  g.fillRoundedRectangle(0, 0, 16, 14, 10);
  
  // Rectangle outline
  g.setColour(Colour(150, 130, 130));
  g.drawRoundedRectangle(0, 0, 16, 14, 10, 1);
  
  // Label
  g.setColour(Colours::lightgrey);
  g.drawText(label, 0, 0, 16, 14, Justification::centred);
  
  // Position line
  g.setColour(Colour(200, 0, 0));
  g.fillRect(0, 15, 1, 14);
  
}

void MarkerComponent::setPosition(double time) { position = time; }

void MarkerComponent::setLabel(String text) {
  label = text;
  repaint();
}

void MarkerComponent::setDescription(String text) {
  description = text;
  setTooltip(description);
}

double MarkerComponent::getPosition() { return position; }

String MarkerComponent::getLabel() { return label; }

String MarkerComponent::getDescription() { return description; }

void MarkerComponent::mouseDown(const MouseEvent &event) {
  getParentComponent()->mouseDown(event);
}

void MarkerComponent::mouseDrag(const MouseEvent &event) {
  getParentComponent()->mouseDrag(event);
}

void MarkerComponent::mouseUp(const MouseEvent &event) {
  getParentComponent()->mouseUp(event);
}
