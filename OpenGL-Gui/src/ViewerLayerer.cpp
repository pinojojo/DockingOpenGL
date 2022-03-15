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
    CreateFBO(fbo_width_, fbo_height_);
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
    DrawCube(viewer_panel_width_,viewer_panel_height_);
}

void ViewerLayerer::OnImGuiRender()
{
    // controler panel for viewer
    {
        ImGui::Begin("view settings");

        // camera
        static float vec4f[4] = { 5.0f, 5.0f, 5.0f, 0.44f };
        if (ImGui::DragFloat3("camera position", vec4f, 0.1f, 0.0f, 10.0f))
        {
            camera_position_.x = vec4f[0];
            camera_position_.y = vec4f[1];
            camera_position_.z = vec4f[2];
            
        }

        static float fovy = 45;
        if (ImGui::DragFloat("camera fovy", &fovy, 0.1, 0.0, 90.f))
        {
            fovy_ = fovy;
        }
       

       


        ImGui::End();
    }


    ImGui::ShowDemoWindow();

   
	// viewer panel 
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.,0 });
        ImGui::Begin("viewer");
        // get panel size
        ImVec2 viewer_panel_size = ImGui::GetContentRegionAvail();
        viewer_panel_width_ = viewer_panel_size.x;
        viewer_panel_height_ = viewer_panel_size.y;
        ImGui::Image((void*)(intptr_t)tex_, ImVec2(fbo_width_, fbo_height_), ImVec2{ 0,1 }, ImVec2{ 1, 0 });
        ImGui::End();
        ImGui::PopStyleVar();
    }
   
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_, 0);  // we only need a color buffer

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: infill framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ViewerLayerer::ResizeFBO(int width, int height)
{
    
    // clear old fbo
    glDeleteTextures(1, &tex_);
    glDeleteFramebuffers(1, &fbo_);

    // new
    CreateFBO(width, height);
    fbo_width_ = width;
    fbo_height_ = height;
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

    camera_.Update(camera_position_, glm::vec3(0) - camera_position_, glm::radians(fovy_), (float)fbo_width_ / (float)fbo_height_);

    glUseProgram(shader_->GetRendererID());

    glUniformMatrix4fv(glGetUniformLocation(shader_->GetRendererID(), "projection"), 1, GL_FALSE, &camera_.GetProjectionMatrix()[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader_->GetRendererID(), "view"), 1, GL_FALSE, &camera_.GetViewMatrix()[0][0]);

    glm::mat4 model = glm::mat4(1.0);
    glUniformMatrix4fv(glGetUniformLocation(shader_->GetRendererID(), "model"), 1, GL_FALSE, &model[0][0]);

}



void ViewerLayerer::DrawCube(int width, int height)
{
    if ((width!=fbo_width_)||(height!=fbo_height_))
    {
        ResizeFBO(width, height);
    }

    glDisable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    ////glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, fbo_width_, fbo_height_);

   


    shader_->Use();
    UpdateShader();
    glBindVertexArray(vao_);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void ViewerLayerer::CreateExampleCubeVAO()
{
    float vertices[] = {
        //
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,0.0f,0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,0.0f,0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,0.0f,0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,0.0f,0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,0.0f,0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,0.0f,0.0f,
        //                                 
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,1.0f,0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,1.0f,0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,1.0f,0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,1.0f,0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,1.0f,0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,1.0f,0.0f,
        //                                 
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,0.0f,1.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,0.0f,1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,0.0f,1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,0.0f,1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,0.0f,1.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,0.0f,1.0f,
         //                                
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,1.0f,0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,1.0f,0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,1.0f,0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,1.0f,0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,1.0f,0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,1.0f,0.0f,
         //
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,0.0f,1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,0.0f,1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,0.0f,1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,0.0f,1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,0.0f,1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,0.0f,1.0f,
        //
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,1.0f,1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,1.0f,1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,1.0f,1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,1.0f,1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,1.0f,1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,1.0f,1.0f,
    };

    // create vao
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8* sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // face id 
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    

}

