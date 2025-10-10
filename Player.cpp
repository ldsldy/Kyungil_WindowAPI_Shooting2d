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
	//�����ϸ� Ellipse�� �ٲٱ�
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
    //y�� 0�Ʒ��� ������� �߰�
}
void Player::OnRender(Gdiplus::Graphics* InGraphics)
{
    if (!InGraphics) return;
    if (!Image) return;

    //���� ��ȯ ���� ����
    Gdiplus::Matrix oldTransform;
    InGraphics->GetTransform(&oldTransform);

    //ȸ�� �߽����� ��ü�� �߽����� ����
    InGraphics->TranslateTransform(Position.X, Position.Y);

    //������ ������ŭ ȸ��
    InGraphics->RotateTransform(Angle);

    InGraphics->TranslateTransform(-Position.X, -Position.Y); //���� ��ġ�� �ٽ� �̵�

    //Image�� �ε��Ǿ� �ִ�.
    InGraphics->DrawImage(
        Image,
        static_cast<int>(Position.X - WidthSize * Pivot.X), //�׷��� ��ġ
        static_cast<int>(Position.Y - HeightSize * Pivot.Y),
        WidthSize, HeightSize); //�׷��� ������

    //���� ��ȯ ���·� ����
    InGraphics->SetTransform(&oldTransform);
    
    //�θ��� OnRender���� �߰� ����
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
        //���� ���� ó�� -> hp-1�� ����(����)
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