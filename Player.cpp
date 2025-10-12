#include "Player.h"
#include "Common.h"
#include "GameManager.h"
#include "Key.h"
#include <string>

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
 //   if(KeyWasPressedMap[InputDirection::Down])
 //   {
 //       
	//}

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
	Key* key = dynamic_cast<Key*>(InOther);
    if (key && InOther != this)
    {
        PickupKey(key);
    }
    // �ٸ� �����۰� ������ �� ó��
}

void Player::PickupKey(Key* key)
{
    if (key)
    {
		int KeyId = key->GetKeyId();
		key->OnPlayerGetKey(); //���� ȹ�� ó��
		AddKey(KeyId); //�÷��̾ ���踦 �����ϰ� ��

		//����� �޽��� ���
        std::wstring message = L"Ű " + std::to_wstring(key->GetKeyId()) + L" ȹ��.\n";
        OutputDebugString(message.c_str());
    }
}

void Player::AddKey(int KeyId)
{
    for (int ownedKey : OwnedKeys)
    {
        if (ownedKey == KeyId)
        {
            OutputDebugString(L"����!, �̹� ������ Ű�Դϴ�.\n");
			return; //�̹� ������ ����� ����
        }
    }

	//���ο� ���� �߰�
	OwnedKeys.push_back(KeyId);
}

bool Player::HasKey(int KeyId)
{
    for(int ownedKey : OwnedKeys)
    {
        if (ownedKey == KeyId)
        {
            return true; //������ ����� true ��ȯ
        }
	}
    return false;
}

void Player::ResetKeys()
{
    OwnedKeys.clear();
}

void Player::HandleKeyState(WPARAM InKey, bool InIsPressed)
{
    if (InKey == VK_LEFT || InKey == VK_RIGHT||InKey == VK_UP||InKey == VK_DOWN)
    {
        KeyWasPressedMap[static_cast<InputDirection>(InKey)] = InIsPressed;
    }
}