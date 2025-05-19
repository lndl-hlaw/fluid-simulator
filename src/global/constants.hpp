#pragma once

// General usage constants
inline constexpr float EPSILON = .000001f;
inline constexpr float PI = 3.1415926535f;
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
inline static constexpr float TIME_STEP = .005f;   
inline static constexpr float GRAVITY = 0.0;//981f;
inline static constexpr float VELOCITY_COLLISION_DAMPING = .95f;
inline static constexpr float NEIGH_RADIUS = 1.1f;
inline static constexpr float NEIGH_RADIUS_SQUARED = NEAR_CAMERA*NEIGH_RADIUS;
inline static constexpr float NEIGH_RADIUS_BIQ = NEIGH_RADIUS_SQUARED*NEIGH_RADIUS_SQUARED;
inline static constexpr float REST_DENSITY = 1.5f;
inline static constexpr float STIFFNESS_PRESSURE = 10;
inline static constexpr float MASS = 1.0f;

// Bounding BOX
inline constexpr float XMIN = -2.0f;
inline constexpr float YMIN = 2.0f;
inline constexpr float ZMIN = -2.0f;
inline constexpr float XMAX = 2.0f;
inline constexpr float YMAX = 4.0f;
inline constexpr float ZMAX = 2.0f;

