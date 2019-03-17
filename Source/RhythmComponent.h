#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

/* Component that draws the lines for the tempo and time signature 
 based on the parameters
 */
class RhythmComponent : public Component
{
public:
  RhythmComponent();
  ~RhythmComponent();
  
  /* Draws lines based on parameters */
  void paint (Graphics&) override;

  /* Setters/Getters */
  void setNumerator(int newNumerator);
  void setDenominator(int newDenominator);
  void setRepetitions(int newValue);
  void setStartPosition(double newPosition);
  void setTempo(double newTempo);
  void setTrackLength(double newLength);
  
  int getNumerator();
  int getDenominator();
  int getRepetitions();
  double getTempo();
  
private:
  
  int numerator;
  int denominator;
  int repetitions;
  double startPosition;
  double tempo;
  double trackLength;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RhythmComponent)
};
