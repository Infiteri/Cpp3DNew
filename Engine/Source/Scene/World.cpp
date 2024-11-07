#include "World.h"
#include "Core/Logger.h"
#include "Serializer/SceneSerializer.h"

#include <unordered_map>
#include <string>

namespace Core
{

    static std::unordered_map<std::string, Scene *> scenes;
    static Scene *activeScene;

    void World::EDITOR_CopyToActive(Scene *scene)
    {
        if (activeScene)
            delete activeScene;

        activeScene = Scene::From(scene);
    }

    void World::Init()
    {
        CE_DEFINE_LOG_CATEGORY("CE_WORLD", "World");
    }

    void World::Shutdown()
    {
    }

    Scene *World::CreateScene(const std::string &name)
    {
        CE_LOG("CE_WORLD", Debug, "Creating new scene: %s.", name.c_str());
        Scene *scene = new Scene();
        SceneSerializer ser(scene);
        scene->SetName(name);
        ser.Deserialize(name);
        scenes[name] = scene;
        return scene;
    }

    Scene *World::CreateScene(const std::string &name, const std::string &filepath)
    {
        CE_LOG("CE_WORLD", Debug, "Creating new scene: %s.", name.c_str());
        Scene *scene = new Scene();
        scene->SetName(name);
        SceneSerializer s(scene);
        s.Deserialize(filepath);
        scenes[name] = scene;
        return scene;
    }

    void World::ActivateScene(const std::string &name)
    {
        Scene *scene = scenes.find(name)->second;

        if (!scene)
        {
            CE_LOG("CE_WORLD", Error, "Scene '%s' doesn't exist, cannot activate.", name.c_str());
            return;
        }

        DeactivateActiveScene();
        ActivateSceneInstance(scene);
    }

    void World::ActivateSceneInstance(Scene *scene)
    {
        activeScene = scene;
    }

    Scene *World::GetActiveScene()
    {
        return activeScene;
    }

    void World::DeactivateActiveScene()
    {
        if (!activeScene)
            return;

        StopActiveScene();

        // NOTE: The activeScene is set to nullptr after stop, its not deleted from the 'scenes' map because it just deactivates it, not destroy it.
        activeScene = nullptr;
    }

    void World::DeleteScene(const std::string &name)
    {
        Scene *scene = scenes.find(name)->second;
        if (!scene)
            return;

        scene->Stop();
        delete scene;
        scenes.erase(name);
    }

    bool World::Exists(const std::string &name)
    {
        return scenes.find(name) != scenes.end();
    }

    void World::StartActiveScene()
    {
        if (!activeScene)
            return;

        activeScene->Start();
    }

    void World::RenderActiveScene()
    {
        if (!activeScene)
            return;

        activeScene->Render();
    }

    void World::UpdateActiveScene()
    {
        if (!activeScene)
            return;

        activeScene->Update();
    }

    void World::StopActiveScene()
    {
        if (!activeScene)
            return;

        activeScene->Stop();
    }
}