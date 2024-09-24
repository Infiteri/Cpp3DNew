#pragma once

#include "Base.h"
#include "Actor.h"
#include <vector>
#include <string>

namespace Core
{
    class CE_API Scene
    {
    private:
        ActorList actors;
        std::string name;

    public:
        Scene();
        ~Scene();

        void SetName(const std::string &name) { this->name = name; };
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
        Actor *GetActorByName(const std::string &name);

        /// @brief Will return any actor with the same UUID.
        /// @param uuid The UUID of the actor.
        /// @return Pointer to an actor, nullptr if no actor exists with this UUID.
        Actor *GetActorByUUID(const UUID &uuid);

        inline ActorList GetActors() { return actors; };
    };
}