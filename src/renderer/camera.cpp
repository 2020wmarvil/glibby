#include "camera.h"

namespace glibby
{
	Camera::Camera()
	{}

	void Camera::Update(InputState& inputState, float dt)
	{
		float scaledSpeed = speed * dt;

		if (inputState.bIsWPressed) cameraPos += scaledSpeed * cameraFront;
		if (inputState.bIsSPressed) cameraPos -= scaledSpeed * cameraFront;
		if (inputState.bIsAPressed) cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * scaledSpeed;
		if (inputState.bIsDPressed) cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * scaledSpeed;
		if (inputState.bIsQPressed) cameraPos -= cameraUp * scaledSpeed;
		if (inputState.bIsEPressed) cameraPos += cameraUp * scaledSpeed;

        float xoffset = inputState.currentX - inputState.lastX;
        float yoffset = inputState.lastY - inputState.currentY;

        inputState.lastX = inputState.currentX;
        inputState.lastY = inputState.currentY;

        xoffset *= sensitivity;
        yoffset *= sensitivity;
        
        yaw += xoffset;
        pitch += yoffset;
        
        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
        
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(front);
    }

	glm::mat4 Camera::GetPerspective(uint32_t screenWidth, uint32_t screenHeight) const
	{
		return glm::perspective(glm::radians(fov), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
	}

	glm::mat4 Camera::GetView() const
	{
		return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}
}