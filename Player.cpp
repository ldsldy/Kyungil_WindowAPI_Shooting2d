#include "Player.h"
#include "Common.h"
#include "GameManager.h"

void Player::OnInitialize()
{
    Position.X = 300.0f;
    Position.Y = 700.0f;

    WidthSize = (Image->GetWidth())/3;
	HeightSize = (Image->GetHeight())/3;

    KeyWasPressedMap[InputDirection::Up] = false;
    KeyWasPressedMap[InputDirection::Down] = false;
    KeyWasPressedMap[InputDirection::Left] = false;
    KeyWasPressedMap[InputDirection::Right] = false;

    PhysicsComponent* physicsComponent = new PhysicsComponent(this, CollisionType::Circle, PhysicsLayer::Player);
	//가능하면 Ellipse로 바꾸기
    physicsComponent->SetRadius(static_cast<float>(HeightSize * 0.5f));
    AddComponent(physicsComponent);
}

void Player::OnTick(float InDeltaTime)
{
    float MoveDistance = InDeltaTime * Speed;
    if (KeyWasPressedMap[InputDirection::Left])
    {
        Position.X -= MoveDistance;
    }
    if (KeyWasPressedMap[InputDirection::Right])
    {
        Position.X += MoveDistance;
	}
    if (KeyWasPressedMap[InputDirection::Up])
    {
        Position.Y -= MoveDistance;
    }

    if (Position.X < (0 - WidthSize * 0.5f))
    {
        Position.X = 0;
    }
    else if ((GameManager::ScreenWidth + WidthSize * 0.5f) < Position.X)
    {
        Position.X= GameManager::ScreenWidth + WidthSize * 0.5f;
    }
    //y가 0아래면 사망판정 추가
}
void Player::OnRender(Gdiplus::Graphics* InGraphics)
{
    if (!InGraphics) return;
    if (!Image) return;

    //현재 변환 상태 저장
    Gdiplus::Matrix oldTransform;
    InGraphics->GetTransform(&oldTransform);

    //회전 중심점을 객체의 중심으로 설정
    InGraphics->TranslateTransform(Position.X, Position.Y);

    //지정된 각도만큼 회전
    InGraphics->RotateTransform(Angle);

    InGraphics->TranslateTransform(-Position.X, -Position.Y); //원래 위치로 다시 이동

    //Image가 로딩되어 있다.
    InGraphics->DrawImage(
        Image,
        static_cast<int>(Position.X - WidthSize * Pivot.X), //그려질 위치
        static_cast<int>(Position.Y - HeightSize * Pivot.Y),
        WidthSize, HeightSize); //그려질 사이즈

    //이전 변환 상태로 복원
    InGraphics->SetTransform(&oldTransform);
    
    //부모의 OnRender이후 추가 동작
    if (!Image)
    {
        Gdiplus::SolidBrush RedBrush(Gdiplus::Color(255, 255, 0, 0));
        InGraphics->FillEllipse(
            &RedBrush,
            static_cast<int>(Position.X - WidthSize * Pivot.X),
            static_cast<int>(Position.Y - HeightSize * Pivot.Y),
            WidthSize, HeightSize);
    }
}

void Player::OnOverlap(Actor* InOther)
{
    OutputDebugString(L"Player::OnOverlap called\n");
    if (InOther && InOther != this)
    {
        //게임 오버 처리 -> hp-1로 변경(예정)
        GameManager::Get().SetGameState(GameState::GameOver);
    }
}


void Player::HandleKeyState(WPARAM InKey, bool InIsPressed)
{
    if (InKey == VK_LEFT || InKey == VK_RIGHT||InKey == VK_UP)
    {
        KeyWasPressedMap[static_cast<InputDirection>(InKey)] = InIsPressed;
    }
}