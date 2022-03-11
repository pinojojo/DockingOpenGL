#include "Floor.h"
using namespace GLCore;
using namespace GLCore::Utils;
void Floor::CreateFBO()
{
    glGenFramebuffers(1, &fbo_ms_);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_ms_);
    // create a multisampled color attachment texture
    glGenTextures(1, &tex_ms_);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, tex_ms_);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples_ms_, GL_RGB, width_, height_, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, tex_ms_, 0);
    // create a (also multisampled) renderbuffer object for depth and stencil attachments
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples_ms_, GL_DEPTH24_STENCIL8, width_, height_);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
void Floor::CreateShader()
{
	shader_ = Shader::FromGLSLTextFiles(
		"assets/shaders/floor.vert.glsl",
		"assets/shaders/floor.frag.glsl"
	);

    glUseProgram(shader_->GetRendererID());





    glUseProgram(0);
}
