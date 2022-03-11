#include "GLCore.h"
#include "ContainerLayer.h"
using namespace GLCore;

class MyGUI : public Application
{
public:
	MyGUI()
		:Application("new")
	{
		PushLayer(new ContainerLayer());
	}
};

int main()
{
	std::unique_ptr<MyGUI> app = std::make_unique<MyGUI>();
	app->Run();
}