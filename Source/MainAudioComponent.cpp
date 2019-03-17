#include "../JuceLibraryCode/JuceHeader.h"
#include "MainAudioComponent.h"

MainAudioComponent::MainAudioComponent(AudioTransportSource& transportSourceToUse, ThumbnailComponent& thumbnailToUse) :
transportSource(transportSourceToUse), timelineComp(), thumbnail(thumbnailToUse), rhythmComp(),
sectionComp(), positionOverlay(transportSourceToUse)
{
  startPosition = 0;
  endPosition = 0;
  sectionX = 0;
  sectionWidth = 0;
  
  addAndMakeVisible(&timelineComp);
  addAndMakeVisible(&thumbnail);
  addAndMakeVisible(&rhythmComp);
  addAndMakeVisible(&sectionComp);
  addAndMakeVisible(&positionOverlay);
  
  thumbnail.setInterceptsMouseClicks(false, false);
  rhythmComp.setInterceptsMouseClicks(false, false);
  positionOverlay.setInterceptsMouseClicks(false, false);
}

MainAudioComponent::~MainAudioComponent()
{
}

void MainAudioComponent::resized()
{
  // Timeline bounds
  timelineComp.setBounds(0, 0, getWidth(), 30);
  
  // Bounds for non interactive components
  Rectangle<int> bounds = Rectangle<int>(0, 30, getWidth(), getHeight() - 30);
  thumbnail.setBounds(bounds);
  rhythmComp.setBounds(bounds);
  positionOverlay.setBounds(bounds);
  
  // Bounds for loop section
  auto duration = transportSource.getLengthInSeconds();
  if(duration > 0 && sectionComp.getWidth() > 0) {
    auto startX = (startPosition / duration) * getWidth();
    auto width = ((endPosition - startPosition) / duration) * getWidth();
    sectionX = startX;
    sectionWidth = width;
    sectionComp.setBounds(startX, 30, width, getHeight() - 30);
  }
  
}

void MainAudioComponent::mouseDown(const MouseEvent& event) {
  
  auto duration = transportSource.getLengthInSeconds();
  
  // Doing nothing if no file is loaded
  if(duration > 0) {
    
    auto pos = (event.position.x / getWidth()) * duration;
    
    // Setting transport position if left click and rhythm position
    // if right click
    if(event.mods.isLeftButtonDown()) {
      
      transportSource.setPosition(pos);
      startPosition = pos;
      endPosition = transportSource.getLengthInSeconds();
      sectionComp.setBounds(0, 0, 0, 0);
      
    }
    else if(event.mods.isRightButtonDown()) {
      
      // Setting correct position for rhythm component
      if(event.originalComponent == this) {
        rhythmComp.setStartPosition(pos);
      }
      else {
        pos = ((sectionX + event.position.x) / getWidth()) * duration;
        rhythmComp.setStartPosition(pos);
      }
      
      rhythmComp.repaint();
      
    }
    
  }
  
}

void MainAudioComponent::mouseDrag(const MouseEvent& event) {
  
  double duration = transportSource.getLengthInSeconds();
  int distance = event.getDistanceFromDragStartX();
  int startX, endX;
  
  // Returning if the x value has not changed or file is not loaded
  if(!distance || !duration) {
    return;
  }
  
  // Changing starting position of rhythm component
  if(event.mods.isRightButtonDown()) {
    
    auto pos = (event.position.x / getWidth()) * duration;
    
    // Setting startX to the correct position
    if (event.originalComponent != this) {
      pos = ((event.position.x + sectionX) / getWidth()) * duration;
    }
    
    // Limiting the start and end positions of the rhythm component
    if(pos < 0) {
      rhythmComp.setStartPosition(0);
    }
    else if (pos >= duration) {
      rhythmComp.setStartPosition(duration);
    }
    else {
      rhythmComp.setStartPosition(pos);
    }
    
    rhythmComp.repaint();
    return;
    
  }
  
  // Creating new section if this component is clicked
  // and changing existing section if sectionComp was clicked
  if(event.originalComponent == this) {
    
    startX = event.getMouseDownX();
    endX = event.x;
    
    // Swapping start and end points if drag distance is negative
    if(distance < 0) {
      distance = -distance;
      startX = event.x;
      endX = event.getMouseDownX();
    }
    
    // Limiting the position of the section to start at 0 and end at the width
    if(startX < 0) {
      startX = 0;
      distance = event.getMouseDownX();
    }
    else if (endX >= getWidth()) {
      distance = getWidth() - startX;
      endX = getWidth();
    }
    
    // Setting bounds and playback positions
    sectionComp.setBounds(startX, 30, distance, getHeight() - 30);
    startPosition = ((double) startX / getWidth()) * duration;
    endPosition = ((double) endX / getWidth() ) * duration;
    
  }
  else {
    
    // Checking if mouse is meant to drag section or resize
    if(sectionComp.getMouseCursor() == MouseCursor::DraggingHandCursor) {
      
      // Getting width of section component
      int width = sectionWidth;
      startX = sectionX + distance;
      
      // Limiting the position of the section to start at 0 and end at the width
      if(startX < 0) {
        startX = 0;
      }
      else if (startX + width >= getWidth()) {
        startX = getWidth() - width;
      }
      
      // Setting bounds and playback positions
      sectionComp.setBounds(startX, 30, width, getHeight() - 30);
      startPosition = ((double) startX / getWidth()) * duration;
      endPosition = ((double) (startX + width) / getWidth() ) * duration;
      
    }
    else {
      
      int anchor, width;
      
      // Switching anchor and width based on where the resize is
      if(sectionComp.isMouseOnRight()) {
        anchor = sectionX;
        width = sectionWidth + distance;
      }
      else {
        anchor = sectionX + distance;
        width = sectionWidth - distance;
      }
      
      // Setting anchor and width variables to corret values
      if(width < 0) {
        anchor = anchor + width;
        width = -width;
      }
      
      // Limiting the position of the section to start at 0 and end at the width
      if(anchor < 0) {
        width = width + anchor;
        anchor = 0;
      }
      else if (anchor + width >= getWidth()) {
        width = getWidth() - anchor;
      }
      
      // Setting bounds and playback positions
      sectionComp.setBounds(anchor, 30, width, getHeight() - 30);
      startPosition = ((double) anchor / getWidth()) * duration;
      endPosition = ((double) (anchor + width) / getWidth() ) * duration;
      
    }
    
  }
  
}

void MainAudioComponent::mouseUp(const MouseEvent& event) {
  sectionX = sectionComp.getX();
  sectionWidth = sectionComp.getWidth();
}

void MainAudioComponent::setRhythmVisible(bool isVisible) {
  rhythmComp.setVisible(isVisible);
}

void MainAudioComponent::setStartPosition(double position) {
  startPosition = position;
}

void MainAudioComponent::setEndPosition(double position) {
  endPosition = position;
}

void MainAudioComponent::setSectionWidth(int width) {
  sectionWidth = width;
  sectionComp.setBounds(sectionX, 30, sectionWidth, getHeight() - 30);
}

double MainAudioComponent::getStartPosition() {
  return startPosition;
}

double MainAudioComponent::getEndPosition() {
  return endPosition;
}

void MainAudioComponent::reset() {
  
  startPosition = 0;
  endPosition = 0;
  sectionX = 0;
  sectionWidth = 0;
  
  timelineComp.setLength(0);
  rhythmComp.setNumerator(4);
  rhythmComp.setDenominator(4);
  rhythmComp.setRepetitions(0);
  rhythmComp.setStartPosition(0);
  rhythmComp.setTempo(120);
  rhythmComp.setTrackLength(0);
  rhythmComp.repaint();
  sectionComp.setBounds(0, 30, 0, getHeight() - 30);
  
}
