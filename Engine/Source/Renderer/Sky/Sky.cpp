#include "Sky.h"
#include "Renderer/Camera/CameraSystem.h"
#include "Renderer/Shader/ShaderSystem.h"
#include "Math/Matrix4.h"
#include "Resources/Loaders/CubeMapLoader.h"
#include <vector>

namespace Core
{
    void Sky::DestroyCubeMap()
    {
        if (cubeTexture)
            delete cubeTexture;

        cubeTexture = nullptr;
    }

    void Sky::DestroyShader()
    {
        if (shader)
            delete shader;

        shader = nullptr;
    }

    void Sky::DestroyFromMode()
    {
        switch (mode)
        {
        case CubeMapMode:
            DestroyCubeMap();
            break;

        case ShaderMode:
            DestroyShader();
            break;
        }
    }

    void Sky::ReloadNewModeWithPreviousData()
    {
        switch (mode)
        {
        case CubeMapMode:
        {
            // c++ pros dont kill me for copying the code below pls 🙏🙏🙏 i swear i will refactor it someday rn i got better things to do then worry this, i cant call SetModeToCubeMap.
            // Due to the fact that i calls back in SetMode, which will call this functions again, which calls SetModeToCubeMap, repeating this cycle over and over and the app never continues
            // Call stack exceeds or something, but im 100% it wont work
            // also it will call into destroy from mode which is prob. not safe to do
            // it does check weather or not its safe to delete the object that has to be deleted but still im no professional c++ programmer so ill stay safe from calling DestroyFromMode twice for a while
            // TODO: Fix this bs
            CubeMapTexture::Configuration c;
            CubeMapLoader l;
            l.Deserialize(cubeMapPath, &c);
            cubeTexture = new CubeMapTexture(c);
        }
        break;

        case ShaderMode:
        {
            shader = new Shader(shaderName);
        }
        break;
        }
    }

    static float cubeVertices[] = {
        // Front face
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,

        // Back face
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        // Left face
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,

        // Right face
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,

        // Top face
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        // Bottom face
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f};

    Sky::Sky()
    {
        color = {255, 255, 255, 255};
        shader = nullptr;
        mode = ColorMode;
        // Generate vertex array
        array = new VertexArray();
        array->GenVertexBuffer(cubeVertices, sizeof(cubeVertices));
        array->GetVertexBuffer()->AddLayout(0, 0, 3);
        array->GetVertexBuffer()->Bind();
    }

    Sky::~Sky()
    {
        delete array;

        DestroyCubeMap();
        DestroyShader();
    }

    void Sky::Render()
    {
        array->Bind();
        array->GetVertexBuffer()->Bind();

        auto camera = CameraSystem::GetPerspectiveActive();

        switch (mode)
        {
        case CubeMapMode:
        {
            auto cubeMapShader = ShaderSystem::GetFromEngineResource("CubeMap");

            if (camera && cubeTexture && cubeMapShader && cubeMapShader->GetValid())
            {
                cubeMapShader->Use();
                camera->UpdateProjection();
                camera->UpdateView();
                cubeMapShader->Mat4(camera->GetProjection(), "uProjection");
                cubeMapShader->Mat4(camera->GetViewInverted(), "uView"); // todo: inverted?
                cubeMapShader->Mat4(Matrix4::Translate(camera->GetPosition()), "uModel");

                cubeTexture->Use();
                cubeMapShader->Vec4(color, "uTint");

                cubeMapShader->Int(cubeTexture->GetGeneration(), "uSkybox");
            }
            break;
        }

        case ShaderMode:
        {
            if (shaderName.empty())
                break;

            if (!shader || (shader && !shader->GetValid()))
                return;

            shader->Use();

            for (auto data : shaderData.GetSet())
            {
                switch (data->GetType())
                {

                case CeData::DataVec2:
                {
                    shader->Vec2(data->As<Vector2>(), data->GetName().c_str());
                }

                case CeData::DataVec3:
                {
                    shader->Vec3(data->As<Vector3>(), data->GetName().c_str());
                }
                break;

                case CeData::DataColor:
                {
                    Color *c = (Color *)data->GetData();
                    shader->Vec4(*c, data->GetName().c_str());
                }
                break;
                }
            }

            shader->Mat4(camera->GetProjection(), "uProjection");
            shader->Mat4(camera->GetViewInverted(), "uView");
            shader->Mat4(Matrix4::Translate(camera->GetPosition()), "uModel");

            break;
        }

        case ColorMode:
            // NOTE: By the renderer
            break;
        }

        array->GetVertexBuffer()->Bind();
        array->GetVertexBuffer()->Draw();
    }

    void Sky::SetMode(Mode newMode)
    {
        DestroyFromMode();
        mode = newMode;
        ReloadNewModeWithPreviousData();
    }

    void Sky::SetModeToColor(const Color &c)
    {
        DestroyFromMode();
        SetMode(ColorMode);
        color = c;
    }

    void Sky::SetModeToCubeMap(CubeMapTexture::Configuration &config)
    {
        DestroyFromMode();
        SetMode(CubeMapMode);
        cubeTexture = new CubeMapTexture(config);
    }

    void Sky::SetModeToCubeMap(const std::string &configPath)
    {
        CubeMapTexture::Configuration c;
        CubeMapLoader l;
        cubeMapPath = configPath;
        l.Deserialize(configPath, &c);
        SetModeToCubeMap(c);
    }

    void Sky::SetModeToShader(const std::string &shaderFile)
    {
        DestroyFromMode();
        SetMode(ShaderMode);
        shaderName = shaderFile;
        shader = new Shader(shaderName);
    }

    void Sky::SetColor(const Color &color)
    {
        this->color.Set(color);
    }

    void Sky::From(Sky *other)
    {
        SetMode(other->GetMode());
        color.Set(other->GetColor());

        if (other->GetMode() == CubeMapMode)
            SetModeToCubeMap(other->cubeTexture->GetConfig());
        else if (other->GetMode() == ShaderMode)
            shaderName = other->shaderName;

        //? Copy over the shader data
        for (auto o : other->shaderData.GetSet())
        {
            CeData *newSet = new CeData(o);
            shaderData.Add(newSet);
        }
    }
}