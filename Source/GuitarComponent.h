#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

/*
*/
class GuitarComponent    : public Component
{
public:
    GuitarComponent();
    ~GuitarComponent();

    void paint (Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GuitarComponent)
};
