#include "ViewerLayerer.h"

using namespace GLCore;
using namespace GLCore::Utils;

ViewerLayerer::ViewerLayerer()
{
}

ViewerLayerer::~ViewerLayerer()
{
}

void ViewerLayerer::OnAttach()
{
    CreateVAO();
    CreateFBO(viewer_width_, viewer_height_);
    CreateShader();
}

void ViewerLayerer::OnDetach()
{
}

void ViewerLayerer::OnEvent(GLCore::Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<MouseButtonPressedEvent>(
		[&](MouseButtonPressedEvent& e)
		{
			std::cout << "viewer pressed" << std::endl;
			return false;
		});
}

void ViewerLayerer::OnUpdate(GLCore::Timestep ts)
{
    if (frame_loop_id_++>1000)
    {
        frame_loop_id_ = 0;
    }
    DrawCube(1024,1024);
}

void ViewerLayerer::OnImGuiRender()
{
   
	// viewer 
	ImGui::Begin("viewer");
    if (ImGui::Button("save"))
    {
        GLCore::Utils::SaveTexture2PNG(tex_, "ds.png", 4);
    }

    ImVec2 wsize = ImGui::GetWindowSize();

    new_viewer_width_ = wsize.x;
    new_viewer_height_ = wsize.y;
   

        
    ImGui::Image((void*)(intptr_t)tex_, ImVec2(viewer_width_,viewer_height_));

	ImGui::End();

    //ImGui::ShowDemoWindow();
}

void ViewerLayerer::CreateVAO()
{
    CreateExampleCubeVAO();

}

void ViewerLayerer::CreateFBO(int width, int height)
{
    glGenFramebuffers(1, &fbo_);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    // create a color attachment texture
    glGenTextures(1, &tex_);
    glBindTexture(GL_TEXTURE_2D, fbo_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, viewer_width_, viewer_height_, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_, 0);  // we only need a color buffer

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: infill framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ViewerLayerer::ResizeFBO(int width, int height)
{
    glDeleteTextures(1, &tex_);
    glDeleteFramebuffers(1, &fbo_);

    CreateFBO(width, height);
    viewer_width_ = width;
    viewer_height_ = height;
}



void ViewerLayerer::CreateShader()
{

    shader_ = Shader::FromGLSLTextFiles(
        "assets/shaders/viewer.vert.glsl",
        "assets/shaders/viewer.frag.glsl"
    );
}

void ViewerLayerer::UpdateShader()
{
    glUseProgram(shader_->GetRendererID());

    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)viewer_width_ / (float)viewer_height_, 0.1f, 100.0f);
   
    glUniformMatrix4fv(glGetUniformLocation(shader_->GetRendererID(), "projection"), 1, GL_FALSE, &projection[0][0]);
    

    // camera/view transformation
    glm::mat4 view = glm::lookAt(glm::vec3(5.0,5.0,5.0), glm::vec3(0), glm::vec3(0,1,0));
    glUniformMatrix4fv(glGetUniformLocation(shader_->GetRendererID(), "view"), 1, GL_FALSE, &view[0][0]);


    glm::mat4 model = glm::mat4(1.0);
    glUniformMatrix4fv(glGetUniformLocation(shader_->GetRendererID(), "model"), 1, GL_FALSE, &model[0][0]);



   
}



void ViewerLayerer::DrawCube(int width, int height)
{
    if ((width!=viewer_width_)||(height!=viewer_height_))
    {
        ResizeFBO(width, height);
    }


    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, viewer_width_, viewer_height_);

    shader_->Use();
    UpdateShader();
    glBindVertexArray(vao_);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void ViewerLayerer::CreateExampleCubeVAO()
{
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.f,
                                         
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.f,
                                         
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 2.f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 2.f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 2.f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 2.f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 2.f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 2.f,
                                         
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 3.f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 3.f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 3.f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 3.f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 3.f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 3.f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 4.f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 4.f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 4.f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 4.f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 4.f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 4.f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 5.f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 5.f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 5.f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 5.f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 5.f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 5.f,
    };

    // create vao
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // face id 
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    

}

