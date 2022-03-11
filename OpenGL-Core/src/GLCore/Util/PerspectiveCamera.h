#pragma once

#include <glm/glm.hpp>

namespace GLCore::Utils {

	class PerspectiveCamera
	{
	public:
		PerspectiveCamera();

		void SetProjection(float fvo_y,float aspect_ratio,float z_near,float z_far);
		void SetView(glm::vec3 camera_position,glm::vec3 look_at,glm::vec3 camera_up);

		

		
		const glm::mat4& GetProjectionMatrix() const { return mat_projection_; }
		const glm::mat4& GetViewMatrix() const { return mat_view_; }
		const glm::mat4& GetViewProjectionMatrix() const { return mat_view_projection_; }


	private:
		void RecalculateViewMatrix();

		glm::mat4 mat_projection_;
		glm::mat4 mat_view_;
		glm::mat4 mat_view_projection_;

		glm::vec3 camera_position_;
		glm::vec3 look_at_;
		glm::vec3 camera_up_;
		
		float fov_y_;
		float aspect_ratio;
		float z_near_;
		float z_far_;

		
		


	};


}




