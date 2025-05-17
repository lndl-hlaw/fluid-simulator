#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../global/constants.hpp"
#include "glm/ext/matrix_clip_space.hpp"

namespace graphics
{
	// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
	enum Camera_Movement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	// Default camera values


	// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
	class camera
	{
	public:
		// camera Attributes
		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;

		// euler Angles
		float Yaw;
		float Pitch;

		// camera options
		float MovementSpeed;
		float MouseSensitivity;
		float RotationSensitivity;
		float Zoom;

		// constants for ray casting
		float viewport_height;
		float viewport_width;
		glm::vec3 horizontal;
		glm::vec3 vertical;
		glm::vec3 lower_left_corner;

		glm::mat4 projection;
		glm::mat4 view;
		glm::mat4 invPorojection;
		glm::mat4 invView;

		// constructor with vectors
		camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3 lookAt = glm::vec3(0, 0, 0), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
			: Front(glm::normalize(lookAt - position)), RotationSensitivity(ROTATION_SENSITIVITY), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
		{
			Position = position;
			WorldUp = up;
			Right = glm::normalize(glm::cross(Front, WorldUp));
			Up = glm::normalize(glm::cross(Right, Front));
			Yaw = yaw;
			Pitch = pitch;

			updateViewport(static_cast<float>(SCR_WIDTH)/ static_cast<float>(SCR_HEIGHT));

			updateCameraVectors();
			updateCameraViewport(false);
		}

		float getFov() { return Zoom * 2; }

		glm::mat4 GetViewMatrix()
		{
			return view;
		}

		glm::mat4 getProjectionMatrix()
		{
			return projection;
		}

		glm::mat4 getInverseProjectionMatrix()
		{
			return invPorojection;
		}

		glm::mat4 getInverseViewMatrix()
		{
			return invView;
		}

		void updateViewport(float aspect_ratio)
		{
			projection = glm::perspective(glm::radians(getFov()), aspect_ratio, NEAR_CAMERA, FAR_CAMERA);
			invPorojection = glm::inverse(projection);
			auto theta = glm::radians(getFov());
			auto h = tan(theta / CAMERA_VIEWPORT);
			viewport_height = CAMERA_VIEWPORT * h;
			viewport_width = aspect_ratio * viewport_height;
			updateCameraViewport();
		}

		// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
		void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
		{
			xoffset *= 1.2f * RotationSensitivity;
			yoffset *= 1.2f * RotationSensitivity;

			Yaw += xoffset;
			Pitch += yoffset;

			// make sure that when pitch is out of bounds, screen doesn't get flipped
			if (constrainPitch)
			{
				if (Pitch > 89.0f)
					Pitch = 89.0f;
				if (Pitch < -89.0f)
					Pitch = -89.0f;
			}

			// update Front, Right and Up Vectors using the updated Euler angles
			updateCameraVectors();
			updateCameraViewport(false);

		}

		void moveLeft()
		{
			Position -= Right * MovementSpeed;
			updateCameraViewport();
		}

		void moveRight()
		{
			Position += Right * MovementSpeed;
			updateCameraViewport();
		}

		void moveForward()
		{
			Position += Front * MovementSpeed;
			updateCameraViewport();
		}

		void moveBack()
		{
			Position -= Front * MovementSpeed;
			updateCameraViewport();
		}

		void ascend()
		{
			Position += Up * MovementSpeed;
			updateCameraViewport();
		}

		void descend()
		{
			Position -= Up * MovementSpeed;
			updateCameraViewport();
		}

		void rotateLeft()
		{
			Yaw -= RotationSensitivity;
			updateCameraVectors();
			updateCameraViewport(false);
		}

		void rotateRight()
		{
			Yaw += RotationSensitivity;
			updateCameraVectors();
			updateCameraViewport(false);
		}

		void rotateUp()
		{
			Pitch += RotationSensitivity;
			updateCameraVectors();
			updateCameraViewport(false);
		}

		void rotateDown()
		{
			Pitch -= RotationSensitivity;
			updateCameraVectors();
			updateCameraViewport(false);
		}

	private:
		// calculates the front vector from the Camera's (updated) Euler Angles
		void updateCameraVectors(bool triggerState = true)
		{
			// calculate the new Front vector
			glm::vec3 front;
			front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
			front.y = sin(glm::radians(Pitch));
			front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
			Front = glm::normalize(front); // also re-calculate the Right and Up vector
			Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
			Up = glm::normalize(glm::cross(Right, Front));

			updateView();

		}

		void updateCameraViewport(bool triggerState = true)
		{
			horizontal = viewport_width * Right;
			vertical = viewport_height * Up;
			lower_left_corner = Position - horizontal / 2.0f - vertical / 2.0f + Front;

		}

		void updateView()
		{
			view = glm::lookAt(Position, Position + Front, Up);
			// glm::vec3 const f(normalize(Front));
			// glm::vec3 const s(normalize(cross(f, Up)));
			// glm::vec3 const u(cross(s, f));
			//
			// view = glm::mat4(1);
			// view[0][0] = s.x;
			// view[1][0] = s.y;
			// view[2][0] = s.z;
			// view[0][1] = u.x;
			// view[1][1] = u.y;
			// view[2][1] = u.z;
			// view[0][2] = -f.x;
			// view[1][2] = -f.y;
			// view[2][2] = -f.z;
			// view[3][0] = -dot(s, Position);
			// view[3][1] = -dot(u, Position);
			// view[3][2] = dot(f, Position);
			// invView = glm::inverse(view);
		}
	};
}
