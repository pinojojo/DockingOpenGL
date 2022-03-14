#include "ContainerLayer.h"

ContainerLayer::ContainerLayer()
{
}

ContainerLayer::~ContainerLayer()
{
}

void ContainerLayer::OnAttach()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   

}

void ContainerLayer::OnDetach()
{
}

void ContainerLayer::OnEvent(GLCore::Event& event)
{
}

void ContainerLayer::OnUpdate(GLCore::Timestep ts)
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ContainerLayer::OnImGuiRender()
{

    DrawDockSpace();



}


void ContainerLayer::DrawDockSpace()
{
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());










}
