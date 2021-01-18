#include "DoubleLayerDisplayer.hpp"

DoubleLayerDisplayer::DoubleLayerDisplayer(Func draw)
{
    this->draw = draw;
}

void DoubleLayerDisplayer::Start()
{
    if(!once)
    {
        layers = HgWAddDoubleLayer(0);
        once = true;
    }
}

void DoubleLayerDisplayer::FlameUpdate()
{
    hiddenLayerID = HgLSwitch(&layers);
    HgLClear(hiddenLayerID);

    draw(hiddenLayerID);
}

void DoubleLayerDisplayer::SetActive(bool enabled)
{
    EventHandler::SetActive(enabled);
    if(!enabled)
    {
        HgLClear(layers.display);
        HgLClear(layers.hidden);
    }
}
    
doubleLayer DoubleLayerDisplayer::GetDoubleLayer()
{
    return layers;
}