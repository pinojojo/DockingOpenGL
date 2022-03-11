#pragma once
#include <GLCore.h>
#include <GLCoreUtils.h>


class Floor
{
public:
	void Init();

private:
	void CreateFBO();
	void CreateShader();
	GLuint tex_ms_=0; // multi-sampled
	GLuint fbo_ms_=0; // multi-sampled

	GLCore::Utils::Shader* shader_; 

	int width_;
	int height_;
	int samples_ms_ = 4;

	glm::mat4 mat_view_;
	glm::mat4 mat_proj_;
	

};

