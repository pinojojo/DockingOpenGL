#pragma once

#include "GLCore/Core/Layer.h"

#include "GLCore/Events/ApplicationEvent.h"
#include "GLCore/Events/KeyEvent.h"
#include "GLCore/Events/MouseEvent.h"

namespace GLCore {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		void Begin();
		void End();

		virtual void ImGuiLayer::OnEvent(Event& event);

		bool ImGuiLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool ImGuiLayer::OnWindowResizePressed(WindowResizeEvent& e);


		int win_width_;
		int win_height_;

	private:
		float m_Time = 0.0f;
		
	

	};

}