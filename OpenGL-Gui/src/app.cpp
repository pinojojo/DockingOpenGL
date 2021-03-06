#include "GLCore.h"
#include "ContainerLayer.h"

#include "ViewerLayerer.h"
using namespace GLCore;

class MyGUI : public Application
{
public:
	MyGUI()
		:Application("new")
	{
		PushLayer(new ContainerLayer());
		
		PushLayer(new ViewerLayerer());
	}
};

int main()
{
	std::unique_ptr<MyGUI> app = std::make_unique<MyGUI>();
	app->Run();
}