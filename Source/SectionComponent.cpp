#include "../JuceLibraryCode/JuceHeader.h"
#include "SectionComponent.h"

SectionComponent::SectionComponent() {
  mouseOnRight = false;
}

SectionComponent::~SectionComponent() {
}

void SectionComponent::paint (Graphics& g) {
  g.fillAll(Colour((uint8) 190, (uint8) 190, (uint8) 190, 0.3f));
}

void SectionComponent::mouseMove(const MouseEvent& event) {
  int x = event.x;
  
  // Setting mouse cursor to a resize cursor if the mouse is currently on the edges of the
  // component and to a drag cursor if it is in the middle of the component
  if(x < 10 || x > getWidth() - 10) {
    setMouseCursor(MouseCursor::StandardCursorType::LeftRightResizeCursor);
  }
  else {
    setMouseCursor(MouseCursor::StandardCursorType::DraggingHandCursor);
  }
  
}

void SectionComponent::mouseDown(const MouseEvent &event) {
  
  // Setting mouseOnRight if the mouse is on the outer edge of the component
  if(event.x > getWidth() - 10) {
    mouseOnRight = true;
  }
  else {
    mouseOnRight = false;
  }
  
  if(event.mods.isRightButtonDown()) {
    getParentComponent()->mouseDown(event);
  }
  
}

void SectionComponent::mouseDrag(const MouseEvent &event) {
  getParentComponent()->mouseDrag(event);
}

void SectionComponent::mouseUp(const MouseEvent& event) {
  getParentComponent()->mouseUp(event);
}

bool SectionComponent::isMouseOnRight() {
  return mouseOnRight;
}
