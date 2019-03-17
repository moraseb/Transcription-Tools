#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ThumbnailComponent.h"
#include "PositionOverlay.h"
#include "TimelineComponent.h"
#include "MainAudioComponent.h"
#include "SidePanelComponent.h"

/*
 This component lives inside our window, and this is where you should put all
 your controls and content.
 */
class MainComponent : public AudioAppComponent, public ChangeListener
{
public:
  /* Constructor/Desctructor */
  MainComponent();
  ~MainComponent();
  
  /* Calls to start playback */
  void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
  
  /* Gets data for playback */
  void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
  
  /* Releases playback resources */
  void releaseResources() override;
  
  /* Colors the main window */
  void paint (Graphics&) override;
  
  /* Determines size and placement of components */
  void resized() override;
  
  /* Calls TransportSourceChanged */
  void changeListenerCallback (ChangeBroadcaster* source) override;
  
  /* Changes playback based on keyboard presses */
  bool keyPressed (const KeyPress& key) override;
  
private:
  
  /* Enumerator for the various playback states */
  enum TransportState
  {
    Stopped,
    Starting,
    Playing,
    Stopping
  };
  
  /* Changes playback state */
  void changeState (TransportState newState);
  
  /* Checks if the transportSource has changed */
  void transportSourceChanged();
  
  /* Closes file and resets thumbnail */
  void closeButtonClicked();
  
  /* Opens file */
  void openButtonClicked();
  
  /* Skips to start of track */
  void skipStartButtonClicked();
  
  /* Starts playback */
  void playButtonClicked();
  
  /* Stops playback */
  void stopButtonClicked();
  
  /* Skips to end of track */
  void skipEndButtonClicked();
  
  /* Opens side panel that contains additional features */
  void fxButtonClicked();
  
  /* Toolbar */
  
  // File
  TextButton closeButton;
  TextButton openButton;
  TextButton saveButton;
  
  // Playback
  TextButton skipStartButton;
  TextButton playButton;
  TextButton pauseButton;
  TextButton stopButton;
  TextButton skipEndButton;
  TextButton loopButton;
  TextButton fxButton;
  Slider gainSlider;
  Slider zoomSlider;
  
  /* SidePanel */
  // FX
  ToggleButton speedButton;
  Slider speedSlider;
  
  TextButton pitchButton;
  Slider pitchSlider;
  
  // Tempo
  ComboBox leftClickBox;
  ComboBox timeSigNumBox;
  ComboBox timeSigDenBox;
  ComboBox repetitionBox;
  
  TextButton bpmButton;
  
  // Mix
  ToggleButton mixButton;
  ToggleButton monoButton;
  ToggleButton phaseButton;
  Slider balanceSlider;
  
  AudioFormatManager formatManager;
  std::unique_ptr<AudioFormatReaderSource> readerSource;
  AudioTransportSource transportSource;
  TransportState state;
  AudioThumbnailCache thumbnailCache;
  ThumbnailComponent thumbnailComp;
  MainAudioComponent audioComp;
  Viewport audioViewport;
  SidePanelComponent sidePanel;
  Viewport sidePanelViewport;
  
  // Members for playback
  double sampleRate;
  bool stopClicked;
  bool fxClicked;
  
  bool invertPhase;
  
  /* Colors */
  Colour grey = Colour(55, 60, 67);
  Colour darkgrey = Colour(35, 34, 35);
  Colour lightgrey = Colour(80, 90, 100);
  Colour blue = Colour(85, 135, 215);
  Colour darkblue = Colour(35, 85, 165);
  
  /* Keyboard */
  MidiKeyboardState keyState;
  MidiKeyboardComponent keyboard;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
