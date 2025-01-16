#include "GameLevel.h"
#include "Engine/Engine.h"

#include "Actor/Wall.h"
#include "Actor/Ground.h"
#include "Actor/Box.h"
#include "Actor/Target.h"
#include "Actor/Player.h"

#include "Engine/Timer.h"

GameLevel::GameLevel()
{
    // 커서 감추기.
    Engine::Get().SetCursorType(CursorType::NoCursor);

    // 맵 파일 불러와 레벨 로드.
    // 파일 열기.
    FILE* file = nullptr;
    fopen_s(&file, "../Assets/Maps/Stage1.txt", "rb");

    // 파일 처리.
    if (file == nullptr)
    {
        std::cout << "맵 파일 열기 실패.\n";
        __debugbreak();
        return;
    }

    // 파일 읽기.
    // 끝 위치로 이동.
    fseek(file, 0, SEEK_END);

    // 이동한 위치의 FP 가져오기.
    size_t readSize = ftell(file);

    // FP 원 위치.
    //fseek(file, 0, SEEK_SET);
    rewind(file);

    // 파일 읽어서 버퍼에 담기.
    char* buffer = new char[readSize + 1];
    size_t bytesRead = fread(buffer, 1, readSize, file);

    if (readSize != bytesRead)
    {
        std::cout << "읽어온 크기가 다름\n";
        __debugbreak();
        return;
    }

    buffer[readSize - 1] = '\0';

    // 파일 읽을 때 사용할 인덱스.
    int index = 0;

    // 좌표 계산을 위한 변수 선언.
    int xPosition = 0;
    int yPosition = 0;

    // 해석 (파싱-Parsing).
    while (index < (int)bytesRead)
    {
        // 한 문자씩 읽기.
        char mapChar = buffer[index++];

        // 개행 문자인 경우 처리.
        if (mapChar == '\n')
        {
            ++yPosition;
            xPosition = 0;
            continue;
        }

        // 맵 문자가 1이면 Wall 액터 생성.
        if (mapChar == '1')
        {
            Wall* wall = new Wall(Vector2(xPosition, yPosition));
            actors.PushBack(wall);
            map.PushBack(wall);
        }

        // 맵 문자가 .이면 그라운드 액터 생성.
        else if (mapChar == '.')
        {
            Ground* ground = new Ground(Vector2(xPosition, yPosition));
            actors.PushBack(ground);
            map.PushBack(ground);
        }

        // 맵 문자가 b이면 박스 액터 생성.
        else if (mapChar == 'b')
        {
            Ground* ground = new Ground(Vector2(xPosition, yPosition));
            actors.PushBack(ground);
            map.PushBack(ground);

            Box* box = new Box(Vector2(xPosition, yPosition));
            actors.PushBack(box);
            boxes.PushBack(box);
        }

        // 맵 문자가 t이면 타겟 액터 생성.
        else if (mapChar == 't')
        {
            Target* target = new Target(Vector2(xPosition, yPosition));
            actors.PushBack(target);
            map.PushBack(target);
            targets.PushBack(target);
        }

        // 맵 문자가 p이면 플레이어 액터 생성.
        else if (mapChar == 'p')
        {
            Ground* ground = new Ground(Vector2(xPosition, yPosition));
            actors.PushBack(ground);
            map.PushBack(ground);

            player = new Player(
                Vector2(xPosition, yPosition),
                this
            );
            actors.PushBack(player);
        }

        ++xPosition;
    }

    // 버퍼 삭제.
    delete[] buffer;

    // 파일 닫기.
    fclose(file);
}

void GameLevel::Update(float deltaTime)
{
    Super::Update(deltaTime);

    // 게임이 클리어됐으면, 게임 종료 처리.
    if (isGameClear)
    {
        // 대략 한 프레임 정도의 시간 대기.
        //static float elapsedTime = 0.0f;
        //elapsedTime += deltaTime;
        //if (elapsedTime < 0.1f)
        //{
        //    return;
        //}
        
        // 타이머.
        static Timer timer(0.1f);
        timer.Update(deltaTime);
        if (!timer.IsTimeOut())
        {
            return;
        }

        // 커서 이동.
        Engine::Get().SetCursorPosition(0, Engine::Get().ScreenSize().y);

        // 메시지 출력.
        Log("Game Clear!");

        // 쓰레드 정지.
        Sleep(2000);

        // 게임 종료 처리.
        Engine::Get().QuitGame();
    }
}

void GameLevel::Draw()
{
    // 맵 그리기.
    for (auto* actor : map)
    {
        // 플레이어의 위치 확인.
        if (actor->Position() == player->Position())
        {
            continue;
        }
        
        // 박스 위치 확인.
        bool shouldDraw = true;
        for (auto* box : boxes)
        {
            if (actor->Position() == box->Position())
            {
                shouldDraw = false;
                break;
            }
        }

        // 맵 액터 그리기.
        if (shouldDraw)
        {
            actor->Draw();
        }
    }

    // 타겟 그리기.
    for (auto* target : targets)
    {
        // 플레이어의 위치 확인.
        if (target->Position() == player->Position())
        {
            continue;
        }

        // 박스 위치 확인.
        bool shouldDraw = true;
        for (auto* box : boxes)
        {
            if (target->Position() == box->Position())
            {
                shouldDraw = false;
                break;
            }
        }

        // 타겟 액터 그리기.
        if (shouldDraw)
        {
            target->Draw();
        }
    }

    // 박스 그리기.
    for (auto* box : boxes)
    {
        box->Draw();
    }

    // 플레이어 그리기.
    player->Draw();
}

bool GameLevel::CanPlayerMove(const Vector2& position)
{
    // 게임이 클리어된 경우 바로 종료.
    if (isGameClear)
    {
        return false;
    }

    // 박스 검색.
    Box* searchedBox = nullptr;
    for (auto* box : boxes)
    {
        if (box->Position() == position)
        {
            searchedBox = box;
            break;
        }
    }

    // 박스가 있을 때.
    if (searchedBox)
    {
        // 이동 방향.
        int directionX = position.x - player->Position().x;
        int directionY = position.y - player->Position().y;

        // 박스가 이동할 새 위치.
        Vector2 newPosition = searchedBox->Position() + Vector2(directionX, directionY);

        // 추가 검색 (박스).
        for (auto* box : boxes)
        {
            // 예외 처리.
            if (box == searchedBox)
            {
                continue;
            }

            // 이동할 위치에 다른 박스가 있다면 이동 불가.
            if (box->Position() == newPosition)
            {
                return false;
            }
        }


        // 추가 검색 (맵).
        for (auto* actor : map)
        {
            // 이동하려는 위치에 있는 액터 검색.
            if (actor->Position() == newPosition)
            {
                // 형변환을 통해 물체의 타입 확인.

                // 이동하려는 위치에 벽이 있으면 이동 불가.
                if (actor->As<Wall>())
                {
                    return false;
                }

                // 땅이나 타겟이면 이동 가능.
                if (actor->As<Ground>() || actor->As<Target>())
                {
                    // 박스 이동 처리.
                    searchedBox->SetPosition(newPosition);
                    
                    // 게임 클리어 여부 확인.
                    isGameClear = CheckGameClear();

                    return true;
                }
            }
        }
    }

    // 이동하려는 위치에 벽이 있는지 확인.
    DrawableActor* searchedActor = nullptr;

    // 먼저 이동하려는 위치의 액터 찾기.
    for (auto* actor : map)
    {
        if (actor->Position() == position)
        {
            searchedActor = actor;
            break;
        }
    }

    // 검색한 액터가 벽인지 확인.
    if (searchedActor->As<Wall>())
    {
        return false;
    }

    // 검색한 액터가 이동 가능한 액터(땅/타겟)인지 확인.
    if (searchedActor->As<Ground>() || searchedActor->As<Target>())
    {
        return true;
    }

    return false;
}

bool GameLevel::CheckGameClear()
{
    // 점수 확인을 위한 변수.
    int currentScore = 0;
    int targetScore = targets.Size();

    // 타겟 위치에 배치된 박스 개수 세기.
    for (auto* box : boxes)
    {
        for (auto* target : targets)
        {
            // 점수 확인.
            if (box->Position() == target->Position())
            {
                ++currentScore;
                continue;
            }
        }
    }

    // 획득한 점수가 목표 점수와 같은지 비교.
    return currentScore == targetScore;
}
