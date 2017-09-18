#pragma once
#include "Controller.h"
#include <engine/Input.h>

namespace Logic
{
    class CameraController : public Controller
    {
    public:
        ~CameraController();

        enum class ECameraMode
        {
            Free = 0,
            Static = 1,
            FirstPerson = 2,
            ThirdPerson = 3,
            Viewer  // name is open to change
        };

        struct CameraSettings
        {
            /**
             * Settings for camera mode "free"
             */
            struct
            {
                Engine::Action* actionMoveForward;
                Engine::Action* actionMoveRight;
                Engine::Action* actionMoveUp;
                Engine::Action* actionLookHorizontal;
                Engine::Action* actionLookVertical;
            } freeCameraSettings;

            /**
             * Settings for camera mode "firstperson"
             */
            struct
            {
                Engine::Action* actionMoveForward;
                Engine::Action* actionMoveRight;
                Engine::Action* actionLookHorizontal;
                Engine::Action* actionLookVertical;
            } firstPersonCameraSettings;

            /**
             * Settings for camera mode "thirdperson"
             */
            struct
            {
                Engine::Action* actionWheel;
                Engine::Action* actionLookVertical;
                Engine::Action* actionLookHorizontal;

                Math::float3 currentLookAt;
                Math::float3 currentOffsetDirection;
                float zoomExponent;
                // vertical angle of the camera: 0° = behind player horizontal. 90° = looking from above (-y direction)
                float pitch;
                // angle between camera's look-at-point and rotation center. 0° = camera looks at rotation center
                float cameraElevation;
                // rotation around vertical axis (y) to be done on next camera update
                float deltaPhi;
            } thirdPersonCameraSettings;

            struct
            {
                float yaw, pitch;
                Math::float3 position;
                Math::float3 up, right, forward;
            } floatingCameraSettings;

            /**
             * Settings for camera mode "viewer"
             */
            struct
            {
                Engine::Action* actionViewHorizontal;
                Engine::Action* actionViewVertical;
                Engine::Action* actionPan;
                Engine::Action* actionZoom;
                Engine::Action* actionRotate;
                Engine::Action* actionClick;
                Engine::Action* actionWheel;

                bool isPanModifier;
                bool isZoomModifier;
                bool isRotateModifier;

                Math::float3 lookAt;
                float mouseWheel;
                Math::float3 up, right, in;
                float yaw, pitch, zoom;
            } viewerCameraSettings;
        };

        /**
         * @param world World of the underlaying entity
         * @param entity Entity owning this controller
         */
        CameraController(World::WorldInstance& world, Handle::EntityHandle entity);

        /**
         * @return The type of this class. If you are adding a new base controller, be sure to add it to ControllerTypes.h
         */
        virtual EControllerType getControllerType() { return EControllerType::CameraController; }
        /**
         * Called on game-tick
         */
        void onUpdateExplicit(float deltaTime);

        /**
         * @brief Sets how the camera should behave
         */
        void setCameraMode(ECameraMode mode);

        /**
         * @brief Sets whether this controller should read input
         */
        void setActive(bool active)
        {
            m_Active = active;
        }

        /**
         * Sets the entity to follow in the first/third person camera modes
         */
        void followEntity(Handle::EntityHandle h)
        {
            m_FollowedEntity = h;
        }

        /**
         * @return access to the settings of this camera
         */
        CameraSettings& getCameraSettings()
        {
            return m_CameraSettings;
        }

        /**
         * Sets the transform of this camera
         */
        void setTransforms(const Math::float3& position, float yaw = 0.0f, float pitch = 0.0f);

        void setDebugMoveSpeed(float moveSpeedMultiplier) { m_moveSpeedMultiplier = moveSpeedMultiplier; }

    protected:
        /**
         * registers a binding
         */
        template <class Functor>
        Engine::Action* registerBinding(ECameraMode cameraMode, Engine::ActionType actionType, Functor functor)
        {
            auto action = Engine::Input::RegisterAction(actionType, functor);
            m_KeyBindings[cameraMode].push_back(std::make_pair(actionType, action));
            return action;
        }

        /**
         * clears all bindings for camera steering
         */
        void clearBindings();

        /**
         * Transforms the given yaw/pitch into the corresponding direction vectors
         * @return pair of (forward, right)
         */
        std::pair<Math::float3, Math::float3> getDirectionVectors(float yaw, float pitch);

        /**
         * Whether this controller should read player input
         */
        bool m_Active;

        void disableActions();

        /**
         * How the camera should behave regarding the followed entity
         */
        ECameraMode m_CameraMode;

        /**
         * Entity this is attached to
         */
        Handle::EntityHandle m_FollowedEntity;

        /**
         * Settings for the different camera modes
         */
        CameraSettings m_CameraSettings;

        /**
         * stored bindings
         */
        std::map<ECameraMode, std::vector<std::pair<Engine::ActionType, Engine::Action*>>> m_KeyBindings;

        /**
         * Current view-matrix
         */
        Math::Matrix m_ViewMatrix;

        //Math::float2 m_LastMousePosition;

        /**
         * Debug
         */
        float m_moveSpeedMultiplier;

        /**
         * Direction to use during locked camera while using mobs
         */
        Math::float3 m_savedPdir;
    };
}
