#include "LayerStack.h"
#include <vector>

namespace Core
{
    static std::vector<Layer *> layers;

    void LayerStack::Init()
    {
    }

    void LayerStack::Shutdown()
    { 
        for (auto l : layers)
        {
            l->OnDetach();
            delete l;
        }

        layers.clear();
    }

    void LayerStack::Update()
    {
        for (auto l : layers)
            l->OnUpdate();
    }

    void LayerStack::RenderImGui()
    {
        for (auto l : layers)
            l->OnImGuiRender();
    }

    void LayerStack::PushLayer(Layer *layer)
    {
        layers.push_back(layer);
        layer->OnAttach();
    }
}