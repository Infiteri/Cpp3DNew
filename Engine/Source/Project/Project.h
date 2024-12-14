#pragma once

#include "Base.h"
#include "Platform/Platform.h"

#include <string>

namespace Core
{
    /// @brief Representation of a project configuration.
    struct ProjectConfiguration
    {
        /// @brief Project name.
        std::string Name = "";

        /// @brief The path of the starting scene.
        /// @note Relative to the AssetDirectory (AssetDirectory/StartScenePath)
        std::string StartScenePath = "";

        /// @brief The directory / folder where all assets are stored, any kinds of assets stored outside this directory / folder may not work.
        std::string AssetDirectory = "";

        /// @brief The name of the library that holds the base game code, with no extension.
        std::string ScriptLibraryPath = "";

        /// @brief The path relative to the AssetDirectory that represents the script which is called in order to build the script library.
        std::string ScriptBuildTask = "";

        inline std::string GetStartScenePathFormatted() { return AssetDirectory + "/" + StartScenePath; };
        inline std::string GetScriptBuildTaskFormatted() { return AssetDirectory + "/" + ScriptBuildTask; };
        inline std::string GetScriptLibPathFormatted(bool withExtension = true) { return AssetDirectory + "/" + ScriptLibraryPath + (withExtension ? Platform::GetExtensionFormat(Platform::ExtensionLibrary, true) : ""); };
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
            EMPTY_CONFIG.Name = "";
            if (!Project::activeProject)
                return EMPTY_CONFIG; // TODO: Fix? Kinda bad of an approach

            return Project::activeProject->config;
        };
    };
}
