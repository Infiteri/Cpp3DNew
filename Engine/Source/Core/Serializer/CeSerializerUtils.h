#pragma once

#include <yaml-cpp/yaml.h>
#include "Math/Vectors.h"
#include "Renderer/Color.h"

// #define CE_YAML_OPERATORS                                                                      \
//     YAML::Emitter &operator<<(YAML::Emitter &out, Vector2 *v)                                  \
//     {                                                                                          \
//         out << YAML::Flow;                                                                     \
//         out << YAML::BeginSeq << v->x << v->y << YAML::EndSeq;                                 \
//         return out;                                                                            \
//     }                                                                                          \
//                                                                                                \
//     YAML::Emitter &operator<<(YAML::Emitter &out, Vector3 *v)                                  \
//     {                                                                                          \
//         out << YAML::Flow;                                                                     \
//         out << YAML::BeginSeq << v->x << v->y << v->z << YAML::EndSeq;                         \
//         return out;                                                                            \
//     }                                                                                          \
//                                                                                                \
//     YAML::Emitter &operator<<(YAML::Emitter &out, Color *color)                                \
//     {                                                                                          \
//         out << YAML::Flow;                                                                     \
//         out << YAML::BeginSeq << color->r << color->g << color->b << color->a << YAML::EndSeq; \
//         return out;                                                                            \
//     }

namespace Core
{
    namespace SerializerUtils
    {
        CE_API void Vector3ToYAML(YAML::Node node, Vector3 *vec);
        CE_API void YAMLToVector3(YAML::Emitter &out, const char *field, Vector3 *vec);
        CE_API void YAMLToColor(YAML::Node node, Color *vec);
        CE_API void ColorToYAML(YAML::Emitter &out, const char *field, Color *vec);
    }
}