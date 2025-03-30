#include "Sky.h"
#include "Renderer/Renderer.h"
#include "Renderer/Camera/CameraSystem.h"
#include "Renderer/Shader/ShaderSystem.h"
#include "Math/Matrix4.h"
#include "Resources/Loaders/CubeMapLoader.h"
#include <vector>

namespace Core
{
    static float cubeVertices[] = {
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f};

    void Sky::_DestroyCubeMap()
    {
        if (cubemap.texture)
            delete cubemap.texture;

        cubemap.texture = nullptr;
    }

    void Sky::_DestroyShader()
    {
        if (shader.shader)
            delete shader.shader;

        shader.shader = nullptr;
        shader.data.Clear();
    }

    void Sky::_DestroyFromMode()
    {
        switch (mode)
        {
        case CubeMapMode:
            _DestroyCubeMap();
            break;

        case ShaderMode:
            _DestroyShader();
            break;
        }
    }

    void Sky::_ReloadNewModeWithPreviousData()
    {
        switch (mode)
        {
        case CubeMapMode:
        {
            // c++ pros dont kill me for copying the code below pls 🙏🙏🙏 i swear i will refactor it someday rn i got better things to do then worry this, i cant call SetModeToCubeMap.
            // Due to the fact that it calls back in SetMode, which will call this functions again, which calls SetModeToCubeMap, repeating this cycle over and over and the app never continues
            // Call stack exceeds or something, but im 100% it wont work
            // also it will call into destroy from mode which is prob. not safe to do
            // it does check weather or not its safe to delete the object that has to be deleted but still im no professional c++ programmer so ill stay safe from calling DestroyFromMode twice for a while
            // TODO: Fix this bs
            CubeMapTexture::Configuration c;
            CubeMapLoader l;
            l.Deserialize(cubemap.path, &c);
            cubemap.texture = new CubeMapTexture(c);
        }
        break;

        case ShaderMode:
        {
            shader.shader = new Shader(shader.name);
        }
        break;
        }
    }

    Sky::Sky()
    {
        color = {255, 255, 255, 255};
        shader.shader = nullptr;
        cubemap.texture = nullptr;
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

        _DestroyCubeMap();
        _DestroyShader();
    }

    void Sky::Render()
    {
        auto camera = CameraSystem::GetPerspectiveActive();
        CE_VERIFY(camera);

        array->Bind();
        camera->UpdateView();

        switch (mode)
        {
        case CubeMapMode:
        {
            auto cubeMapShader = ShaderSystem::GetFromEngineResource("CubeMap");

            if (cubemap.texture && cubeMapShader)
            {
                cubeMapShader->Use();
                camera->UpdateProjection();
                camera->UpdateView();

                Renderer::UploadCameraToShader(cubeMapShader, camera);
                cubeMapShader->Mat4(Matrix4::Translate(camera->GetPosition()), "uModel");

                cubemap.texture->Use();
                cubeMapShader->Vec4(color, "uTint");
                cubeMapShader->Int(cubemap.texture->GetGeneration(), "uSkybox");
            }
            break;
        }

        case ShaderMode:
        {
            CE_VERIFY(shader.shader);
            CE_VERIFY(shader.shader->GetValid());
            shader.shader->Use();

            for (auto pair : shader.data.GetSet())
            {
                CeData *data = pair.second;
                switch (data->GetType())
                {
                case CeData::DataVec2:
                {
                    shader.shader->Vec2(data->As<Vector2>(), data->GetName().c_str());
                }
                break;

                case CeData::DataVec3:
                {
                    shader.shader->Vec3(data->As<Vector3>(), data->GetName().c_str());
                }
                break;

                case CeData::DataColor:
                {
                    Color *c = data->As<Color>();
                    shader.shader->Vec4(*c, data->GetName().c_str());
                }
                break;
                case CeData::DataFloat:
                {
                    CeData::FloatContainer *c = data->As<CeData::FloatContainer>();
                    shader.shader->Float(c->Value, data->GetName().c_str());
                }
                break;
                }
            }

            Renderer::UploadCameraToShader(shader.shader, camera);
            shader.shader->Mat4(Matrix4::Translate(camera->GetPosition()), "uModel");
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
        _DestroyFromMode();
        mode = newMode;
    }

    void Sky::SetModeToColor(const Color &c)
    {
        _DestroyFromMode();
        SetMode(ColorMode);
        color = c;
    }

    void Sky::SetModeToCubeMap(CubeMapTexture::Configuration &config)
    {
        _DestroyFromMode();
        SetMode(CubeMapMode);
        cubemap.texture = new CubeMapTexture(config);
    }

    void Sky::SetModeToCubeMap(const std::string &configPath)
    {
        CubeMapTexture::Configuration c;
        CubeMapLoader l;
        cubemap.path = configPath;
        l.Deserialize(configPath, &c);
        SetModeToCubeMap(c);
    }

    void Sky::SetModeToShader(const std::string &shaderFile)
    {
        _DestroyFromMode();
        SetMode(ShaderMode);
        shader.name = shaderFile;
        shader.shader = new Shader(shader.name);
    }

    void Sky::SetColor(const Color &color)
    {
        this->color.Set(color);
    }

    void Sky::From(Sky *other)
    {
        _DestroyFromMode();
        SetMode(other->GetMode());
        color.Set(other->GetColor());
        shader.name = other->shader.name;
        cubemap.path = other->cubemap.path;

        //? Copy over the shader data
        for (auto o : other->shader.data.GetSet())
        {
            CeData *newSet = new CeData(o.second);
            shader.data.Add(newSet);
        }

        // setup new sky >
        _ReloadNewModeWithPreviousData();
    }
}