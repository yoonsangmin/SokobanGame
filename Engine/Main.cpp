#include "PreCompiledHeader.h"
#include <iostream>

#include "Engine/Engine.h"
#include "Level/Level.h"

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Engine engine;
    engine.LoadLevel(new Level());
	engine.Run();
}   