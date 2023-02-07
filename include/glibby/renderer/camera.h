#pragma once

#include "input.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace glibby
{
	class Camera
	{
	public:
		Camera();

		void Update(InputState& inputState, float dt);

		glm::mat4 GetPerspective(uint32_t screenWidth, uint32_t screenHeight) const;
		glm::mat4 GetView() const;
	private:
		float speed = 30.0f;
		float pitch = 0.0f;
		float yaw = 90.0f;
		float fov = 60.0f;
        float sensitivity = 0.1f;

		glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -2.0f);
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	};
}