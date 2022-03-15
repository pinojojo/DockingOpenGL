#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>

class ViewerLayerer : public GLCore::ImGuiLayer 
{
public:
	ViewerLayerer();
	virtual ~ViewerLayerer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(GLCore::Event& event) override;
	virtual void OnUpdate(GLCore::Timestep ts) override;
	virtual void OnImGuiRender() override;
private:
	void CreateVAO();
	void CreateFBO(int width, int height);
	void ResizeFBO(int width, int height);
	void CreateShader();
	void UpdateShader();
	void DrawCube(int width, int height);

	void CreateExampleCubeVAO();


private:
	int frame_loop_id_ = 0;

	GLuint vao_ = 0;
	GLuint vbo_ = 0;
	GLuint fbo_ = 0;
	GLuint aa_fbo_=0;
	GLuint tex_ = 0;
	int fbo_width_ = 1000;
	int fbo_height_ = 1000;
	int viewer_panel_width_ = 1000;
	int viewer_panel_height_ = 1000;
	
	GLCore::Utils::Shader* shader_;

	GLCore::Utils::PerspectiveCamera camera_;
	glm::vec3 camera_position_ = glm::vec3(5.f);
	float fovy_=45;
	glm::vec3 front_ = glm::vec3(1, 0, 0);


};
