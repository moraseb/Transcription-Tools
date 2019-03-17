#include "../JuceLibraryCode/JuceHeader.h"
#include "RhythmComponent.h"

RhythmComponent::RhythmComponent()
{
  numerator = 4;
  denominator = 4;
  repetitions = 0;
  startPosition = 0;
  tempo = 120;
  trackLength = 0;
}

RhythmComponent::~RhythmComponent()
{
}

void RhythmComponent::paint(Graphics& g)
{
  
  // Doing nothing if file is not loaded
  if(!trackLength) {
    return;
  }
  
  // Getting length of one note and start position
  double length = 240 / (tempo * denominator); // length in seconds
  length = length / trackLength; // length in percent
  auto startX = (startPosition / trackLength) * getWidth();
    
  // Drawing lines for time signature
  for(int i = 0; i <= (repetitions * numerator); i++) {
    
    // Coloring start of time signature line lighter than
    // other lines for easier distinction
    if(i % numerator == 0) {
      g.setColour(Colour(160, 154, 255));
    }
    else {
      g.setColour(Colour(133, 124, 255));
    }
    
    // Getting position and drawing line
    auto pos = startX + ( i * length * getWidth() );
    g.fillRect(pos, 0, 1, getHeight());
    
  }
  
}

void RhythmComponent::setNumerator(int newNumerator) {
  numerator = newNumerator;
}

void RhythmComponent::setDenominator(int newDenominator) {
  denominator = newDenominator;
}

void RhythmComponent::setRepetitions(int newValue) {
  repetitions = newValue;
}

void RhythmComponent::setStartPosition(double newPosition) {
  startPosition = newPosition;
}

void RhythmComponent::setTempo(double newTempo) {
  tempo = newTempo;
}

void RhythmComponent::setTrackLength(double newLength) {
  trackLength = newLength;
}

int RhythmComponent::getNumerator() {
  return numerator;
}

int RhythmComponent::getDenominator() {
  return denominator;
}

int RhythmComponent::getRepetitions() {
  return repetitions;
}

double RhythmComponent::getTempo() {
  return tempo;
}
