
#include "i3DEngine.hpp"

int main()
{
	i3DEngine * engine = get3DEngine();

	engine->init();
  
	while (engine->render())
	{

	}

	return 0;
}