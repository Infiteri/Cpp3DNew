#include "Core.h"
#include "Core/Launch/EntryPoint.h"

#include "EditorLayer.h"

namespace Core
{
    class Editor : public Application
    {
    public:
        Editor(){};
        ~Editor(){};

        void Init()
        {
            LayerStack::PushLayer(new EditorLayer());
        }
    };

    Application *CreateApplication()
    {
        return new Editor();
    }

}
