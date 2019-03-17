#include "../JuceLibraryCode/JuceHeader.h"
#include "PositionOverlay.h"

PositionOverlay::PositionOverlay(AudioTransportSource& transportSourceToUse) :
transportSource (transportSourceToUse)
{
  startTimer(33);
}

PositionOverlay::~PositionOverlay() {
}

void PositionOverlay::paint(Graphics& g) {
  
  auto duration = transportSource.getLengthInSeconds();
  
  if (duration > 0.0)
  {
    auto audioPosition = transportSource.getCurrentPosition();
    auto drawPosition = (audioPosition / duration) * getWidth();
    
    g.setColour(Colour(200, 200, 200));
    g.fillRect(drawPosition, 0, 1, getHeight());
  }
  
}

void PositionOverlay::timerCallback() {
  repaint();
}
