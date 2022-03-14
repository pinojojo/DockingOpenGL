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
	GLuint tex_ = 0;
	GLCore::Utils::Shader* shader_;
	GLCore::Utils::PerspectiveCameraController camera_controller_;
	
	int viewer_width_ = 1024;
	int viewer_height_ = 1024;
	int new_viewer_width_ = 1024;
	int new_viewer_height_ = 1024;
	
	

};
