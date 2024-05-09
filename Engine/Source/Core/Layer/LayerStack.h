#pragma once

#include "Base.h"
#include "Layer.h"

namespace Core
{
    class CE_API LayerStack
    {
    public:
        LayerStack() {}
        ~LayerStack() {}

        static void Init();
        static void Shutdown();

        static void Update();

        static void PushLayer(Layer *layer);
    };
}