#pragma once

#include "Core.h"
#include "Panel.h"

namespace Core
{
    class SceneHierarchyPanel : public Panel
    {
    public:
        Actor *selectionContext;

        SceneHierarchyPanel();
        ~SceneHierarchyPanel();

        virtual void OnImGuiRender(PanelInformation *information) override;

        void RenderActor(Actor *a);

        void RenderActorComponents(Actor* a);
    };

}