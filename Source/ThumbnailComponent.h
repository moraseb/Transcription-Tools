#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

/* Component that holds the waveform of the file */
class ThumbnailComponent :
public Component,
private ChangeListener
{
public:
  
  /* Constructor/Destructor */
  ThumbnailComponent(int sourceSamplesPerThumbnailSample,
                     AudioFormatManager& formatManager,
                     AudioThumbnailCache& cache);
  ~ThumbnailComponent();
  
  /* Sets file to draw */
  void setFile (const File& file);
  
  /* Draws the waveform */
  void paint (Graphics&) override;
  
  /* Draws message if no file has been loaded */
  void paintIfNoFileLoaded (Graphics& g);
  
  /* Draws waveform if file is loaded */
  void paintIfFileLoaded (Graphics& g);
  
  /* Calls to redraw waveform if new file is loaded */
  void changeListenerCallback (ChangeBroadcaster* source) override;
  
private:
  
  /* Calls to repaint new thumbnail */
  void thumbnailChanged();
  
  // Audio waveform
  AudioThumbnail thumbnail;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThumbnailComponent)
};
