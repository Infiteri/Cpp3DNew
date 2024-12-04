#pragma once

#include "Base.h"
#include "Actor.h"
#include "SceneEnvironment.h"

#include <vector>
#include <unordered_map>
#include <string>

namespace Core
{
    class CE_API Scene
    {
    private:
        ActorList actors;
        SceneEnvironment environemnt;
        std::string name;

        friend class TextureSystem;

        enum State
        {
            Initialized,
            Started,
            Running,
            Stopped,
        };

        State state;

        void LoadRuntimeComponent(Actor *a);

        void SetName(const std::string &name) { this->name = name; };

        friend class EditorLayer; // Note: For editor
        friend class World;
        friend class SceneSerializer;

        void _SetSkyInstance();

    public:
        Scene();
        ~Scene();

        static Scene *From(Scene *scene);

        // TODO: Maybe keep name to the path?
        inline std::string GetName() { return name; };

        void Start();
        void Render();
        void Update();
        void Stop();

        void AddActor(Actor *actor);
        Actor *SpawnActor();

        /// @brief Will return any actor with the same name, if more actors have the same name it will return the first on in the list.
        /// @param name The name of the actor.
        /// @return Pointer to an actor, nullptr if no actor exists with this name.
        Actor *GetActor(const std::string &name);

        /// @brief Will return any actor with the same UUID.
        /// @param uuid The UUID of the actor.
        /// @return Pointer to an actor, nullptr if no actor exists with this UUID.
        Actor *GetActor(const UUID &uuid);

        Actor *GetActorByTag(const std::string &tagValue);

        inline ActorList GetActors() { return actors; };

        Actor *GetActorInHierarchy(const UUID &uuid);

        void RemoveActor(const std::string &name);
        void RemoveActor(const UUID &uid);

        void MoveChildInHierarchy(const UUID &uid, int newIndex);

        inline SceneEnvironment *GetEnvironment() { return &environemnt; };

        // -- Scene camera functions --
        void SetPrimaryCameraToNone();
        void ActivateSceneCamera();
        CameraComponent *GetPrimaryCameraComponent();
        CameraComponent *GetFirstCameraComponent();

        // ----------------------------
    };
}