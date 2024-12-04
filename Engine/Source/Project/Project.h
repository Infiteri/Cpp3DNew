#pragma once

#include "Base.h"

#include <string>

namespace Core
{
    /// @brief Representation of a project configuration.
    struct ProjectConfiguration
    {
        /// @brief Project name.
        std::string Name = "Project";

        /// @brief The path of the starting scene.
        /// @note Relative to the AssetDirectory (AssetDirectory/StartScenePath)
        std::string StartScenePath = "";

        /// @brief The directory / folder where all assets are stored, any kinds of assets stored outside this directory / folder may not work.
        std::string AssetDirectory = "";

        // TODO: Script system path? std::string ScriptPath = "/";

        inline std::string GetStartScenePathFormatted() { return AssetDirectory + "/" + StartScenePath; };
    };

    class CE_API Project
    {
    private:
        // NOTE: Only one project can be active at a time. Once opened, the editor needs to be reloaded in order to open a new project

        /// @brief Engine handled?
        static inline Project *activeProject;
        friend class Engine;
        friend class ProjectSerializer;

        ProjectConfiguration config;

    public:
        Project();
        ~Project();

        static Project *New();
        static Project *Load(const std::string &projectPath);
        static void SaveActive(const std::string &projectPath);

        static ProjectConfiguration &GetActiveConfiguration()
        {
            static ProjectConfiguration EMPTY_CONFIG;
            if (!Project::activeProject)
                return EMPTY_CONFIG; // TODO: Fix? Kinda bad of an approach

            return Project::activeProject->config;
        };
    };
}
