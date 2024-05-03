#include "Core.h"
#include "Core/Launch/EntryPoint.h"

namespace Core
{
    class Editor : public Application
    {
    public:
        Editor(){};
        ~Editor(){};
    };

    Application *CreateApplication()
    {
        return new Editor();
    }

}
