#pragma once

// General usage constants
inline constexpr float EPSILON = .000001f;

// Window constants
inline constexpr int SCR_WIDTH = 1280;
inline constexpr int SCR_HEIGHT = 720;
inline constexpr const char* APP_NAME = "Fluid Simulator";

// camera constants
inline constexpr float YAW = -90.0f;
inline constexpr float PITCH = 0.0f;
inline constexpr float SPEED = 0.1f;
inline constexpr float SENSITIVITY = 0.09f;
inline constexpr float ROTATION_SENSITIVITY = 0.7f;
inline constexpr float ZOOM = 45.0f;
inline constexpr float CAMERA_VIEWPORT = 2.0f;
inline constexpr float NEAR_CAMERA = 0.01f;
inline constexpr float FAR_CAMERA = 1000.0f;
inline constexpr float MOUSE_OBJECT_SHIFT_COEFF = 0.05f;

// Visual constants
inline constexpr float PARTICLE_RADIUS_VIS = 0.1f; 

// Simulation constants
inline constexpr unsigned int PARTICLES_COUNT = 10000;
inline static constexpr float TIME_STEP = .01f;   
inline static constexpr float GRAVITY = 9.81f;
inline static constexpr float VELOCITY_COLLISION_DAMPING = .8f;
