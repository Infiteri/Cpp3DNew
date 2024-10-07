#pragma once

#include "Core.h"
#include "Panel.h"

namespace Core
{
    class SceneHierarchyPanel : public Panel
    {
    public:
        Actor *selectionContext;
        int actorIndex;

        SceneHierarchyPanel();
        ~SceneHierarchyPanel();

        virtual void OnImGuiRender(PanelInformation *information) override;

        void RenderActor(Actor *a, Actor *parent, bool parentNodeOpen);
        void RenderActorComponents(Actor *a);
    };

}