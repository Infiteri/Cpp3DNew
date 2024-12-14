#pragma once

#include "Base.h"
#include "Core/Logger.h"
#include "Math/Vectors.h"
#include "Math/Quaternion.h"
#include "Physics/Collider/Collider.h"

namespace Core
{
    class Actor;
    class CE_API PhysicsBody
    {
    public:
        enum Type
        {
            None,
            Rigid,
            Static
        };

    protected:
        friend class Contact;
        static PhysMatrix3 _ComposeInertiaMatrix(Collider *collider, float mass)
        {
            PhysMatrix3 m;

            switch (collider->GetType())
            {
            case Collider::Box:
            {
                auto b = collider->As<AABBCollider>();
                m.SetBlockInertiaTensor(b->Size, mass);
            }
            break;

            case Collider::None:
                break;

            default:
                CE_CORE_WARN("Unknown collider type");
                break;
            }

            return m;
        }

        static void _TransformInertiaTensor(PhysMatrix3 &iitWorld,
                                            const Quaternion &q,
                                            const PhysMatrix3 &iitBody,
                                            const PhysMatrix4x3 &rotMat)
        {
            float t4 = rotMat.data[0] * iitBody.data[0] +
                       rotMat.data[1] * iitBody.data[3] +
                       rotMat.data[2] * iitBody.data[6];
            float t9 = rotMat.data[0] * iitBody.data[1] +
                       rotMat.data[1] * iitBody.data[4] +
                       rotMat.data[2] * iitBody.data[7];
            float t14 = rotMat.data[0] * iitBody.data[2] +
                        rotMat.data[1] * iitBody.data[5] +
                        rotMat.data[2] * iitBody.data[8];
            float t28 = rotMat.data[4] * iitBody.data[0] +
                        rotMat.data[5] * iitBody.data[3] +
                        rotMat.data[6] * iitBody.data[6];
            float t33 = rotMat.data[4] * iitBody.data[1] +
                        rotMat.data[5] * iitBody.data[4] +
                        rotMat.data[6] * iitBody.data[7];
            float t38 = rotMat.data[4] * iitBody.data[2] +
                        rotMat.data[5] * iitBody.data[5] +
                        rotMat.data[6] * iitBody.data[8];
            float t52 = rotMat.data[8] * iitBody.data[0] +
                        rotMat.data[9] * iitBody.data[3] +
                        rotMat.data[10] * iitBody.data[6];
            float t57 = rotMat.data[8] * iitBody.data[1] +
                        rotMat.data[9] * iitBody.data[4] +
                        rotMat.data[10] * iitBody.data[7];
            float t62 = rotMat.data[8] * iitBody.data[2] +
                        rotMat.data[9] * iitBody.data[5] +
                        rotMat.data[10] * iitBody.data[8];

            iitWorld.data[0] = t4 * rotMat.data[0] +
                               t9 * rotMat.data[1] +
                               t14 * rotMat.data[2];
            iitWorld.data[1] = t4 * rotMat.data[4] +
                               t9 * rotMat.data[5] +
                               t14 * rotMat.data[6];
            iitWorld.data[2] = t4 * rotMat.data[8] +
                               t9 * rotMat.data[9] +
                               t14 * rotMat.data[10];
            iitWorld.data[3] = t28 * rotMat.data[0] +
                               t33 * rotMat.data[1] +
                               t38 * rotMat.data[2];
            iitWorld.data[4] = t28 * rotMat.data[4] +
                               t33 * rotMat.data[5] +
                               t38 * rotMat.data[6];
            iitWorld.data[5] = t28 * rotMat.data[8] +
                               t33 * rotMat.data[9] +
                               t38 * rotMat.data[10];
            iitWorld.data[6] = t52 * rotMat.data[0] +
                               t57 * rotMat.data[1] +
                               t62 * rotMat.data[2];
            iitWorld.data[7] = t52 * rotMat.data[4] +
                               t57 * rotMat.data[5] +
                               t62 * rotMat.data[6];
            iitWorld.data[8] = t52 * rotMat.data[8] +
                               t57 * rotMat.data[9] +
                               t62 * rotMat.data[10];
        }

        static void _CalculateTransformMatrix(PhysMatrix4x3 &transformMatrix,
                                              const Vector3 &position,
                                              const Quaternion &orientation)
        {
            transformMatrix.data[0] = 1 - 2 * orientation.j * orientation.j -
                                      2 * orientation.k * orientation.k;
            transformMatrix.data[1] = 2 * orientation.i * orientation.j -
                                      2 * orientation.r * orientation.k;
            transformMatrix.data[2] = 2 * orientation.i * orientation.k +
                                      2 * orientation.r * orientation.j;
            transformMatrix.data[3] = position.x;

            transformMatrix.data[4] = 2 * orientation.i * orientation.j +
                                      2 * orientation.r * orientation.k;
            transformMatrix.data[5] = 1 - 2 * orientation.i * orientation.i -
                                      2 * orientation.k * orientation.k;
            transformMatrix.data[6] = 2 * orientation.j * orientation.k -
                                      2 * orientation.r * orientation.i;
            transformMatrix.data[7] = position.y;

            transformMatrix.data[8] = 2 * orientation.i * orientation.k -
                                      2 * orientation.r * orientation.j;
            transformMatrix.data[9] = 2 * orientation.j * orientation.k +
                                      2 * orientation.r * orientation.i;
            transformMatrix.data[10] = 1 - 2 * orientation.i * orientation.i -
                                       2 * orientation.j * orientation.j;
            transformMatrix.data[11] = position.z;
        }

        Type type;
        Actor *owner;
        PhysMatrix4x3 transformMatrix;
        Collider *collider;
        PhysMatrix3 inverseInertiaTensor;
        PhysMatrix3 inverseInertiaTensorWorld;
        Quaternion orientation;

        friend class Scene;
        friend class PhysicsEngine;

        virtual void _CalculateData() {};

    public:
        PhysicsBody();
        virtual ~PhysicsBody();

        inline Type GetType() { return type; };
        inline Actor *GetOwner() { return owner; };

        virtual void Update();
        virtual void UseConfiguration(void *config);

        Vector3 &GetPosition();
        Vector3 GetRotation(bool asRadians = false);
        inline PhysMatrix4x3 &GetTransformMatrix() { return transformMatrix; };

        inline Collider *GetCollider() { return collider; };
        void SetCollider(Collider *newCollider);

        /// @brief Will set the orientation and will change the body rotation to the respected values/
        /// @param quat The quaternion.
        void SetOrientation(const Quaternion &quat);

        inline PhysMatrix3 GetInverseInertiaTensorWorld() { return inverseInertiaTensorWorld; };
        inline PhysMatrix3 GetInverseInertiaTensor() { return inverseInertiaTensor; };
        void SetInertiaTensor(const PhysMatrix3 &matrix);
        inline Quaternion &GetOrientation() { return orientation; };

        template <typename T>
        T *As() { return (T *)this; };
    };
}