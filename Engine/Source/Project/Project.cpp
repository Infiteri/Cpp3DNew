#include "Project.h"
#include "ProjectSerializer.h"

namespace Core
{
    Project::Project()
    {
    }

    Project::~Project()
    {
    }

    Project *Project::New()
    {
        if (Project::activeProject != nullptr)
            delete Project::activeProject;

        Project::activeProject = new Project();

        return Project::activeProject;
    }

    Project *Project::Load(const std::string &projectPath)
    {
        New();
        ProjectSerializer ser(Project::activeProject);
        ser.Deserialize(projectPath);
        return Project::activeProject;
    }

    void Project::SaveActive(const std::string &projectPath)
    {
        ProjectSerializer ser(Project::activeProject);
        ser.Serialize(projectPath);
    }
}