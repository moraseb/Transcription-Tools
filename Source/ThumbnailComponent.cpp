#include "../JuceLibraryCode/JuceHeader.h"
#include "ThumbnailComponent.h"

ThumbnailComponent::ThumbnailComponent(int sourceSamplesPerThumbnailSample,
                                       AudioFormatManager& formatManager,
                                       AudioThumbnailCache& cache) :
thumbnail(sourceSamplesPerThumbnailSample, formatManager, cache)
{
  thumbnail.addChangeListener(this);
}

ThumbnailComponent::~ThumbnailComponent()
{
}

void ThumbnailComponent::setFile(const File& file)
{
  thumbnail.setSource(new FileInputSource (file));
}


void ThumbnailComponent::paint (Graphics& g)
{
  if (thumbnail.getNumChannels() == 0)
    paintIfNoFileLoaded (g);
  else
    paintIfFileLoaded (g);
}

void ThumbnailComponent::paintIfNoFileLoaded(Graphics& g)
{
  g.fillAll(Colour(35, 34, 35));
  g.setColour(Colours::whitesmoke);
  g.drawFittedText("No File Loaded", getLocalBounds(), Justification::centred, 1.0f);
}

void ThumbnailComponent::paintIfFileLoaded(Graphics& g)
{
  g.fillAll(Colour(35, 34, 35));
  g.setColour(Colour(85, 135, 215));
  thumbnail.drawChannels (g, getLocalBounds(), 0.0, thumbnail.getTotalLength(), 1.0f);
  
  g.setColour(Colour(80, 90, 100));
  g.fillRect(0, getHeight()/2, getWidth(), 1);
}

void ThumbnailComponent::changeListenerCallback(ChangeBroadcaster* source)
{
  if (source == &thumbnail)
    thumbnailChanged();
}

void ThumbnailComponent::thumbnailChanged()
{
  repaint();
}
