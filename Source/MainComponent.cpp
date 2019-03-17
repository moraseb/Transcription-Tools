#include "MainComponent.h"

MainComponent::MainComponent() :
state (Stopped),
thumbnailCache(5),
thumbnailComp(256, formatManager, thumbnailCache),
audioComp(transportSource, thumbnailComp), audioViewport(),
sidePanel(audioComp), sidePanelViewport(),
keyState(),
keyboard(keyState, MidiKeyboardComponent::horizontalKeyboard)
{
  sampleRate = 0;
  stopClicked = false;
  fxClicked = false;
  invertPhase = false;
  
  /* File */
  addAndMakeVisible(&closeButton);
  closeButton.setButtonText ("X");
  closeButton.setColour(TextButton::textColourOnId, Colours::red);
  closeButton.setColour(TextButton::textColourOffId, Colours::red);
  closeButton.onClick = [this] { closeButtonClicked(); };
  closeButton.setEnabled(false);
  
  addAndMakeVisible(&openButton);
  openButton.setButtonText ("Open");
  openButton.onClick = [this] { openButtonClicked(); };
  
  /* Playback */
  addAndMakeVisible (&skipStartButton);
  skipStartButton.setButtonText ("|<");
  skipStartButton.onClick = [this] { skipStartButtonClicked(); };
  skipStartButton.setEnabled (false);
  
  addAndMakeVisible(&playButton);
  playButton.setButtonText (">");
  playButton.onClick = [this] { playButtonClicked(); };
  playButton.setColour (TextButton::buttonColourId, Colours::forestgreen);
  playButton.setEnabled (false);
  
  addAndMakeVisible(&stopButton);
  stopButton.setButtonText ("||");
  stopButton.onClick = [this] { stopButtonClicked(); };
  stopButton.setColour (TextButton::buttonColourId, Colours::red);
  stopButton.setEnabled (false);
  
  addAndMakeVisible(&skipEndButton);
  skipEndButton.setButtonText (">|");
  skipEndButton.onClick = [this] { skipEndButtonClicked(); };
  skipEndButton.setEnabled (false);
  
  addAndMakeVisible(&gainSlider);
  gainSlider.setRange(0, 500, 0.01);
  gainSlider.setSkewFactorFromMidPoint(100);
  gainSlider.setValue(100);
  gainSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
  gainSlider.setDoubleClickReturnValue(true, 100);
  gainSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
  gainSlider.setPopupDisplayEnabled(false, false, nullptr);
  gainSlider.setTextValueSuffix("%");
  gainSlider.setColour(Slider::thumbColourId, darkblue);
  gainSlider.setColour(Slider::trackColourId, lightgrey);
  gainSlider.setColour(Slider::backgroundColourId, darkgrey);
  gainSlider.setEnabled(false);
  
  addAndMakeVisible(&zoomSlider);
  zoomSlider.setRange(100, 1000, 1);
  zoomSlider.setValue(100);
  zoomSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
  zoomSlider.setDoubleClickReturnValue(true, 100);
  zoomSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
  zoomSlider.setTextValueSuffix("%");
  zoomSlider.setColour(Slider::thumbColourId, darkblue);
  zoomSlider.setColour(Slider::trackColourId, lightgrey);
  zoomSlider.setColour(Slider::backgroundColourId, darkgrey);
  zoomSlider.setEnabled(false);
  
  zoomSlider.onValueChange = [this] {
    audioComp.setSize((getWidth() - 2) * (zoomSlider.getValue() / 100), audioComp.getHeight());
  };
  
  /* FX */
  addAndMakeVisible(&fxButton);
  fxButton.setEnabled(true);
  fxButton.setButtonText("FX");
  fxButton.onClick = [this] { fxButtonClicked(); };
  
  /* Visual audio component */
  addAndMakeVisible(&audioViewport);
  audioViewport.setViewedComponent(&audioComp, false);
  audioViewport.setScrollBarPosition(true, true);
  audioViewport.setScrollBarsShown(false, true);
  ScrollBar* horizontalBar;
  horizontalBar = &audioViewport.getHorizontalScrollBar();
  horizontalBar->setColour(ScrollBar::ColourIds::thumbColourId, blue);
  horizontalBar->setAutoHide(false);
  
  audioComp.setWantsKeyboardFocus(false);
  
  /* Side Panel */
  addAndMakeVisible(&sidePanelViewport);
  sidePanelViewport.setViewedComponent(&sidePanel, false);
  sidePanelViewport.setScrollBarPosition(false, true);
  sidePanelViewport.setScrollBarsShown(true, false);
  ScrollBar* verticalBar;
  verticalBar = &sidePanelViewport.getVerticalScrollBar();
  verticalBar->setColour(ScrollBar::ColourIds::thumbColourId, blue);
  sidePanel.setSize(300 - sidePanelViewport.getVerticalScrollBar().getWidth(), 1000);
  
  /* Keyboard */
  addAndMakeVisible(&keyboard);
  keyboard.setWantsKeyboardFocus(false);
  keyboard.setOctaveForMiddleC(4);
  keyboard.setAvailableRange(21, 108); // from A0 to C8
  keyboard.setMidiChannel(1);
  keyboard.setWantsKeyboardFocus(false);
  keyboard.setScrollButtonsVisible(false);
  
  // Size of MainComponent
  setSize(800, 500);
  setWantsKeyboardFocus(true);
  
  formatManager.registerBasicFormats();
  transportSource.addChangeListener (this);
  
  // Number of input/output channels
  setAudioChannels(2, 2);
  
}

MainComponent::~MainComponent() {
  shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
  transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) {
  
  if (readerSource.get() == nullptr) {
    bufferToFill.clearActiveBufferRegion();
  }
  else {
    
    // Looping
    if(transportSource.getCurrentPosition() < audioComp.getStartPosition()) {
      transportSource.setPosition(audioComp.getStartPosition());
    }
    else if(transportSource.getCurrentPosition() >= audioComp.getEndPosition()) {
      transportSource.setPosition(audioComp.getStartPosition());
    }
    
    // Updating buffer
    transportSource.getNextAudioBlock(bufferToFill);
    
    //===================================== AUDIO PROCESSING GOES HERE ==================================//
    auto* leftInBuffer = bufferToFill.buffer->getReadPointer(0, bufferToFill.startSample);
    auto* leftOutBuffer = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    auto* rightInBuffer = bufferToFill.buffer->getReadPointer(1, bufferToFill.startSample);
    auto* rightOutBuffer = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
    
    // Invert Phase
    if (invertPhase) {
      for(auto sample = 0; sample < bufferToFill.numSamples; sample++) {
        leftOutBuffer[sample] = leftInBuffer[sample] - rightInBuffer[sample];
        rightOutBuffer[sample] = leftOutBuffer[sample];
      }
    }
    
    // Gain
    transportSource.setGain(gainSlider.getValue() / 100);
    
  }
}

void MainComponent::releaseResources() {
  transportSource.releaseResources();
}

void MainComponent::paint (Graphics& g) {
  
  // Sets background color
  g.fillAll(grey);
  
  // Colors bakground for dividing lines
  g.setColour(lightgrey);
  g.fillRect(0, 50, getWidth(), getHeight() - 50);
}

void MainComponent::resized() {
  
  // UI components
  closeButton.setBounds(12, 12, 30, 25);
  openButton.setBounds (42, 12, 60, 25);
  playButton.setBounds (getWidth()/2 - 30, 12, 30, 25);
  stopButton.setBounds (getWidth()/2, 12, 30, 25);
  skipStartButton.setBounds(getWidth()/2 - 60, 12, 30, 25);
  skipEndButton.setBounds(getWidth()/2 + 30, 12, 30, 25);
  gainSlider.setBounds(getWidth() * 0.19, 14, getWidth() * 0.17, 20);
  zoomSlider.setBounds(getWidth() * 0.69, 14, getWidth() * 0.17, 20);
  fxButton.setBounds(getWidth() - 42, 12, 30, 25);
  
  // Side panel and audio viewport
  if (fxClicked) {
    audioViewport.setBounds(1, 51, getWidth() - 302, getHeight() - 252);
    sidePanelViewport.setBounds(getWidth() - 301, 51, 302, getHeight() - 252);
  }
  else {
    audioViewport.setBounds(1, 51, getWidth() - 2, getHeight() - 252);
    sidePanelViewport.setBounds(getWidth(), 51, 0, getHeight() - 252);
  }
  
  // Audio compnent size
  int width = (getWidth() - 2) * (zoomSlider.getValue() / 100);
  int height = audioViewport.getHeight() - audioViewport.getHorizontalScrollBar().getHeight();
  audioComp.setSize(width, height);
  
  // Keyboard
  keyboard.setBounds(0, getHeight() - 200, getWidth(), 200);
  keyboard.setKeyWidth( ( (float) getWidth() ) / 52 );
  
}

void MainComponent::changeListenerCallback(ChangeBroadcaster* source) {
  if (source == &transportSource)
    transportSourceChanged();
}

bool MainComponent::keyPressed(const KeyPress& key) {
  
  if(transportSource.getLengthInSeconds() <= 0) {
    return true;
  }
  
  // Play/Stop when spacebar is pressed
  if(key.isKeyCode(KeyPress::spaceKey)) {
    if(transportSource.isPlaying())
      stopButtonClicked();
    else
      playButtonClicked();
  }
  else if(key.isKeyCode('M')) {
    audioComp.timelineComp.addMarker(transportSource.getCurrentPosition(), "A", "");
    audioComp.timelineComp.resized();
  }
  
  return true;
}

void MainComponent::changeState(TransportState newState) {
  if (state != newState)
  {
    state = newState;
    
    switch (state)
    {
      case Stopped:
        stopButton.setEnabled (false);
        playButton.setEnabled (true);
        closeButton.setEnabled(true);
        openButton.setEnabled (true);
        skipEndButton.setEnabled(true);
        break;
        
      case Starting:
        playButton.setEnabled (false);
        openButton.setEnabled (false);
        closeButton.setEnabled(false);
        transportSource.start();
        break;
        
      case Playing:
        stopButton.setEnabled (true);
        skipEndButton.setEnabled(false);
        break;
        
      case Stopping:
        transportSource.stop();
        break;
        
      default:
        jassertfalse;
        break;
    }
  }
}

void MainComponent::transportSourceChanged() {
  if (transportSource.isPlaying()) {
    changeState (Playing);
  }
  else {
    changeState (Stopped);
    
    // Looping if the stop button was not clicked
    if (!stopClicked){
      changeState (Starting);
    }
    
  }
}

void MainComponent::closeButtonClicked() {
  
  // Resetting File
  transportSource.setSource(nullptr, 0, nullptr, 0);
  thumbnailComp.setFile(File());
  openButton.setEnabled(true);
  
  // Disabling other buttons
  playButton.setEnabled(false);
  skipStartButton.setEnabled(false);
  skipEndButton.setEnabled(false);
  stopButton.setEnabled(false);
  closeButton.setEnabled(false);
  gainSlider.setEnabled(false);
  zoomSlider.setEnabled(false);
  
  gainSlider.setValue(100);
  gainSlider.setColour(Slider::thumbColourId, blue);
  gainSlider.setPopupDisplayEnabled(true, true, nullptr, -1);
  
  zoomSlider.setValue(0);
  zoomSlider.setColour(Slider::thumbColourId, blue);
  zoomSlider.setPopupDisplayEnabled(true, true, nullptr, -1);
  
  audioComp.reset();
  resized();
}

void MainComponent::openButtonClicked() {
  FileChooser chooser ("Select a file to play...", File(),
                       "*.wav; *.flac; *.mp3; *.aif; *.aiff, *.ogg");
  
  if (chooser.browseForFileToOpen())
  {
    File file = chooser.getResult();
    
    if (auto* reader = formatManager.createReaderFor (file))
    {
      std::unique_ptr<AudioFormatReaderSource> newSource (new AudioFormatReaderSource (reader, true));
      transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);
      sampleRate = reader->sampleRate;
      closeButton.setEnabled(true);
      playButton.setEnabled (true);
      skipStartButton.setEnabled(true);
      skipEndButton.setEnabled(true);
      thumbnailComp.setFile(file);
      readerSource.reset (newSource.release());
      audioComp.timelineComp.setLength(transportSource.getLengthInSeconds());
      audioComp.rhythmComp.setTrackLength(transportSource.getLengthInSeconds());
      gainSlider.setEnabled(true);
      gainSlider.setPopupDisplayEnabled(true, true, nullptr, -1);
      gainSlider.setColour(Slider::thumbColourId, blue);
      zoomSlider.setEnabled(true);
      zoomSlider.setPopupDisplayEnabled(true, true, nullptr, -1);
      zoomSlider.setColour(Slider::thumbColourId, blue);
      audioComp.setStartPosition(0);
      audioComp.setEndPosition(transportSource.getLengthInSeconds());
    }
  }
}

void MainComponent::skipStartButtonClicked() {
  transportSource.setPosition(0.0);
  audioComp.setStartPosition(0);
  audioComp.setEndPosition(transportSource.getLengthInSeconds());
  audioComp.setSectionWidth(0);
}

void MainComponent::playButtonClicked() {
  transportSource.setPosition(audioComp.getStartPosition());
  stopClicked = false;
  changeState (Starting);
}

void MainComponent::stopButtonClicked() {
  stopClicked = true;
  changeState (Stopping);
}

void MainComponent::skipEndButtonClicked() {
  transportSource.setPosition(transportSource.getLengthInSeconds());
  audioComp.setStartPosition(transportSource.getLengthInSeconds());
  audioComp.setEndPosition(transportSource.getLengthInSeconds());
  audioComp.setSectionWidth(0);
}

void MainComponent::fxButtonClicked() {
  fxClicked = !fxClicked;
  resized();
}
