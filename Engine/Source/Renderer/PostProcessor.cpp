#include "PostProcessor.h"

namespace Core
{
    PostProcessor::PostProcessor() {}

    PostProcessor::~PostProcessor()
    {
        for (auto &shader : shaders)
            delete shader.shd;
    }

    void PostProcessor::Add(const std::string &shaderName, bool enabled)
    {
        for (const auto &shader : shaders)
        {
            if (shader.shd->GetName() == shaderName)
                return;
        }

        PostProcessShader pps;
        pps.shd = new Shader(shaderName);
        pps.enabled = enabled;
        shaders.push_back(pps);
    }

    void PostProcessor::Enable(const std::string &shaderName)
    {
        for (auto &shader : shaders)
        {
            if (shader.shd->GetName() == shaderName)
            {
                shader.enabled = true;
                return;
            }
        }
    }

    void PostProcessor::Disable(const std::string &shaderName)
    {
        for (auto &shader : shaders)
        {
            if (shader.shd->GetName() == shaderName)
            {
                shader.enabled = false;
                return;
            }
        }
    }

    void PostProcessor::Remove(const std::string &shaderName)
    {
        for (auto it = shaders.begin(); it != shaders.end(); ++it)
        {
            if (it->shd->GetName() == shaderName)
            {
                delete it->shd;
                shaders.erase(it); // Remove the shader from the vector
                return;
            }
        }
    }

    void PostProcessor::Rearrange(std::vector<std::string> shaderNames)
    {
        std::vector<PostProcessShader> newOrder;

        // Reorder shaders based on shaderNames vector
        for (const auto &name : shaderNames)
        {
            for (const auto &shader : shaders)
            {
                if (shader.shd->GetName() == name)
                {
                    newOrder.push_back(shader);
                    break;
                }
            }
        }

        shaders = newOrder;
    }

    std::vector<Shader *> PostProcessor::GetEnabledShaders()
    {
        std::vector<Shader *> enabledShaders;

        for (const auto &shader : shaders)
        {
            if (shader.enabled)
            {
                enabledShaders.push_back(shader.shd);
            }
        }

        return enabledShaders;
    }
}
