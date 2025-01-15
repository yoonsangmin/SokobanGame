#include "PreCompiledHeader.h"
#include "Level.h"
// 프로젝트 설정 -> 추가 포함 디렉토리에 $(ProjectDir)\ 추가
//#include "../Actor/Actor.h"
#include "Actor/Actor.h"

#include <iostream>

Level::Level()
{
}

Level::~Level()
{
	// 메모리 해제.
	//for (int ix = 0; ix < actors.Size(); ++ix)
	//{
	//	// 액터 삭제.
	//	delete actors[ix];
	//}

    // 메모리 해제.
    for (Actor* actor : actors)
    {
        delete actor;
    }
}

void Level::AddActor(Actor* newActor)
{
    //actors.PushBack(newActor);
    addRequestedActor = newActor;
}

void Level::ProcessAddedAndDestroyedActor()
{
    // 액터 순회 후 삭제 요청된 액터를 처리.
    for (int ix = 0; ix < actors.Size();)
    {
        if (actors[ix]->isExpired)
        {
            delete actors[ix];
            actors[ix] = nullptr;
            actors.Erase(ix);
            continue;
        }

        ++ix;
    }

    // 추가 요청된 액터 처리.
    if (addRequestedActor)
    {
        actors.PushBack(addRequestedActor);
        addRequestedActor = nullptr;
    }
}

void Level::Update(float deltaTime)
{
    //for (int ix = 0; ix < actors.Size(); ++ix)
    //{
    //    actors[ix]->Update(deltaTime);
    //}

    // 레벨에 포함된 액터를 순회하면서 Update 함수 호출.
    for (Actor* actor : actors)
    {
        // 액터가 비활성화 상태이거나, 삭제 요청된 경우 건너뛰기.
        if (!actor->isActive || actor->isExpired)
        {
            continue;
        }

        actor->Update(deltaTime);
    }
}

void Level::Draw()
{
    // 레벨에 포함된 액터를 순회하면서 Draw 함수 호출.
    //for (int ix = 0; ix < actors.Size(); ++ix)
    //{
    //    actors[ix]->Draw();
    //}
    
    for (Actor* actor : actors)
    {
        // 액터가 비활성화 상태이거나, 삭제 요청된 경우 건너뛰기.
        if (!actor->isActive || actor->isExpired)
        {
            continue;
        }

        actor->Draw();
    }
}
