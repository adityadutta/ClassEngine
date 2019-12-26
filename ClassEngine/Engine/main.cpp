#include "Core/CoreEngine.h"
#include "../Game/Game0.h"


int main(int argc, char * argv[])
{
	CoreEngine::GetInstance()->SetGameInterface(new Game0());

	if (!CoreEngine::GetInstance()->OnCreate("ClassEngine v0.1", 800, 600))
	{
		Debug::FatalError("Engine failed to create!", __FILE__, __LINE__);
		return 0;
	}

	CoreEngine::GetInstance()->Run();

	return 0;
}
