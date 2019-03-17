#include "../JuceLibraryCode/JuceHeader.h"
#include "TimelineComponent.h"

TimelineComponent::TimelineComponent() : tooltip(this, 0)
{
  trackLength = 0;
  markerX = 0;
  tooltip.addToDesktop(0);
}

TimelineComponent::~TimelineComponent() { }

void TimelineComponent::paint (Graphics& g)
{
  
  // Background of timeline
  g.fillAll(Colour(55, 60, 67));
  
  // Background of markers
  g.setColour(Colour(35, 34, 35));
  g.fillRect(0, 0, getWidth(), 15);
  
  // Dividing lines
  g.setColour(Colour(80, 90, 100));
  g.fillRect(0, 14, getWidth(), 1);
  g.fillRect(0, 29, getWidth(), 1);
  
  g.setColour(Colour(175, 185, 185));
  
  // Drawing if track is loaded
  if(trackLength > 0) {
    
    double interval = 43;
    double time = (43.0 / getWidth()) * trackLength;
    
    // Different drawing methods based on whether the time
    // is less than 1 or greater than 1
    if(time < 1) {
      
      interval = ((1.0 / trackLength) * getWidth());
      time = 0;
      
      for(int x = 0; x < getWidth(); x += (int) interval) {
        g.fillRect(x, 15, 1, 14);
        g.drawFittedText(secondsToMinutesText(time), x + 2, 15, 50, 14, Justification::centredLeft, 1);
        time += 1;
      }
      
    }
    else {
      
      for(int x = 0; x < getWidth(); x += 43) {
        time = ((double) x / getWidth()) * trackLength;
        g.fillRect(x, 15, 1, 14);
        g.drawFittedText(secondsToMinutesText(time), x + 2, 15, 50, 14, Justification::centredLeft, 1);
      }
      
    }
    
  }
  
}

void TimelineComponent::resized() {
  
  if (trackLength > 0) {
    
    MarkerComponent* current;
    
    for (int i = 0; i < markers.size(); i++) {
      current = markers[i];
      double percent = current->getPosition() / trackLength;
      current->setBounds(getWidth() * percent, 0, 16, 30);
    }
    
  }
  
}

void TimelineComponent::setLength(double length) {
  trackLength = length;
  
  // Resetting markers if the track length equals 0
  if(!length) {
    markers.clear();
  }
  
  repaint();
  resized();
}

void TimelineComponent::addMarker(double time, String label, String text) {
  MarkerComponent* markerToAdd = new MarkerComponent(time, label, text);
  addAndMakeVisible(markerToAdd);
  markers.add(markerToAdd);
  markerToAdd->setTooltip(text);
}

void TimelineComponent::mouseDown(const MouseEvent &event) {
  
  auto comp = event.originalComponent;
  
  // Calling popup menu or dragging marker
  if(event.mods.isPopupMenu() && trackLength) {
    
    PopupMenu pop;
    int result;
    
    // Changing popup menu items depending on whether or not a marker is clicked
    if(comp == this) {
      pop.addItem(1, "Add Marker");
      result = pop.show();
    }
    else {
      pop.addItem(2, "Edit Marker");
      pop.addItem(3, "Remove Marker");
      result = pop.showAt(comp);
    }
    
    // Adding, deleting, or editing marker based on input result
    if(result == 1) {
      addMarker((event.position.x / getWidth()) * trackLength, "A", "");
      resized();
    }
    else if (result == 2) {
      EditMarkerComponent* popupcomp = new EditMarkerComponent((MarkerComponent*) comp, trackLength);
      popupcomp->setSize(400, 135);
      
      DialogWindow::LaunchOptions options;
      options.dialogTitle = "Edit Marker";
      options.resizable = false;
      options.content.set(popupcomp, true);
      options.launchAsync();
    }
    else if (result == 3) {
      markers.removeObject((MarkerComponent*) comp);
    }
    
  }
  else if(event.mods.isLeftButtonDown() && comp != this) {
    markerX = comp->getX();
  }
  
}

void TimelineComponent::mouseDrag(const MouseEvent &event) {
  
  auto comp = event.originalComponent;
  
  if(comp != this) {
    
    int x = markerX + event.getDistanceFromDragStartX();
    
    // Limiting position of marker
    if(x < 0) {
      x = 0;
    }
    else if (x >= getWidth()) {
      x = getWidth() - 1;
    }
    
    comp->setBounds(x, 0, 16, 30);
    
  }
  
}

void TimelineComponent::mouseUp(const MouseEvent &event) {
  
  auto comp = event.originalComponent;
  
  if(comp != this) {
    MarkerComponent* marker;
    marker = (MarkerComponent*) comp;
    marker->setPosition( ( (double) marker->getX() / getWidth() ) * trackLength);
  }
  
}

String TimelineComponent::secondsToMinutesText(double time) {
  
  // Converting time to an integer
  int seconds = (int) time;
  
  // Converting seconds to minutes
  int minutes = (int) (seconds / 60);
  
  // Getting the number of seconds remaining after removing minutes
  seconds = seconds % 60;
  
  // Converting to strings
  String minText = String(minutes);
  String secText = String(seconds);
  
  // Formatting text for minutes
  if (minutes < 10) {
    minText = " " + minText;
  }
  
  // Formatting text for seconds
  if (seconds == 0) {
    secText = "00";
  } else if (seconds < 10) {
    secText = "0" + secText;
  }
  
  // Combining strings
  return minText + String(":") + secText;
}
