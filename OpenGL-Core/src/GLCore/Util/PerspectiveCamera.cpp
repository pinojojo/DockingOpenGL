#include "glpch.h"
#include "PerspectiveCamera.h"



GLCore::Utils::PerspectiveCamera::PerspectiveCamera()
{
}

void GLCore::Utils::PerspectiveCamera::Update(glm::vec3 position, glm::vec3 front, float fov_y, float aspect_ratio)
{
	position_ = position;
	front_ = glm::normalize(front);
	fov_y_ = fov_y;
	aspect_ratio_ = aspect_ratio;

	// calc vec
	world_up_ = glm::vec3(0, 0, 1);
	right_ = glm::normalize(glm::cross(front_, world_up_));
	up_ = glm::normalize(glm::cross(right_, front_));

	// calc mat
	mat_view_ = glm::lookAt(position_, position_ + front_, up_);
	mat_projection_ = glm::perspective(fov_y_, aspect_ratio_, z_near_, z_far_);
	mat_view_projection_ = mat_projection_ * mat_view_;


}


