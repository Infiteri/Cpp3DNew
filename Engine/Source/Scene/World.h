#pragma once

#include "Base.h"
#include "Scene.h"

namespace Core
{
    class CE_API World
    {
    public:
        World() {};
        ~World() {};

        // -- Base class functions --

        static void Init();
        static void Shutdown();

        // --------------------------

        // -- Scene related ---------

        /// @brief Will create a new scene. Name must be unique.
        /// @param name The name of the scene.
        /// @return Pointer to a newly created scene.
        static Scene *CreateScene(const std::string &name);

        /// @brief Will activate a scene with this name. ActiveScene will be set to nullptr if there is no scene with the given name.
        /// @param name The name of the scene.
        static void ActivateScene(const std::string &name);

        /// @brief Will activate a scene instance, this scene is not saved in the World class, useful for editor (future).
        /// @note This function only sets the active scene to the 'scene' argument and it doesn't actually start it. (Call the StartActiveScene)
        /// @param scene The scene to activate.
        static void ActivateSceneInstance(Scene *scene);

        /// @brief Will return the active scene.
        /// @return Scene* or nullptr.
        static Scene *GetActiveScene();

        /// @brief Will deactivate the active scene. Nothing will happen if ActiveScene is nullptr.
        static void DeactivateActiveScene();

        /// @brief Will delete a scene, nothing happens if scene with this name doesn't exist.
        /// @param name The name of the scene.
        static void DeleteScene(const std::string& name);
        // --------------------------

        // -- Scene activation ------

        static void StartActiveScene();
        static void RenderActiveScene();
        static void UpdateActiveScene();
        static void StopActiveScene();

        // --------------------------
    };

}