#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace PhysiXal {

	class Camera {
	public:
		void SetOrthographicProjection(float left, float right, float top, float bottom, float znear, float zfar);
		void SetPerspectiveProjection(float fovy, float aspect, float znear, float zfar);

		void SetViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3{ 0.f, -1.f, 0.f });
		void SetViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3{ 0.f, -1.f, 0.f });
		void SetViewYXZ(glm::vec3 position, glm::vec3 rotation);

		static glm::mat4& GetProjection() { return m_ProjectionMatrix; }
		static glm::mat4& GetView() { return m_ViewMatrix; }
	private:
		static inline glm::mat4 m_ProjectionMatrix{ 1.f };
		static inline glm::mat4 m_ViewMatrix{ 1.f };

		glm::vec3 m_Movement;
	};
}