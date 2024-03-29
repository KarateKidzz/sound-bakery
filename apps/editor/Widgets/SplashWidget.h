#pragma once

#include "Widget.h"

class SplashWidget : public Widget
{
public:
    SplashWidget(WidgetSubsystem* parentSubsystem) : Widget(parentSubsystem) {}

public:
    void ShowSplashScreen();
    void CloseSplashScreen();

public:
    virtual void Tick(double deltaTime) override;
    virtual void Render() override;

protected:
    virtual void Start() override;

private:
    bool m_wantsToShow         = false;
    double m_timeShowingScreen = 0.0;
    std::string m_loadingText  = std::string("Loading");

    uint32_t m_splashImageID = 0;
    int m_splashImageWidth   = -1;
    int m_splashImageHeight  = -1;
};
