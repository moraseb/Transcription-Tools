#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "TimelineComponent.h"
#include "ThumbnailComponent.h"
#include "RhythmComponent.h"
#include "PositionOverlay.h"
#include "SectionComponent.h"

/* Main component that contains audio visualization compnents as children */
class MainAudioComponent : public Component
{
public:
  
  AudioTransportSource& transportSource;
  TimelineComponent timelineComp;
  ThumbnailComponent& thumbnail;
  RhythmComponent rhythmComp;
  SectionComponent sectionComp;
  PositionOverlay positionOverlay;
  
  MainAudioComponent(AudioTransportSource& transportSourceToUse, ThumbnailComponent& thumbnailToUse);
  ~MainAudioComponent();
  
  /**/
  void resized() override;
  
  /* Sets audio position when left clicked and rhythm position when right clicked */
  void mouseDown(const MouseEvent& event) override;
  
  /* Chenges position of section component or changes size based on where the section is clicked */
  void mouseDrag(const MouseEvent& event) override;
  
  /* Sets section start and width after section is dragged */
  void mouseUp(const MouseEvent& event) override;
  
  /* Changes visibility of rhythm comppnent */
  void setRhythmVisible(bool isVisible);
  
  /* Setters/getters */
  void setStartPosition(double position);
  void setEndPosition(double position);
  void setSectionWidth(int width);
  double getStartPosition();
  double getEndPosition();
  
  /* Resets all members */
  void reset();
  
private:
  
  // Variables for playback
  double startPosition;
  double endPosition;
  
  // Variables for section component
  int sectionX;
  int sectionWidth;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainAudioComponent)
};
