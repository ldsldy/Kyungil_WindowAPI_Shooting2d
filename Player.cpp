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

	// 키 입력 상태 초기화
    KeyWasPressedMap[InputDirection::Up] = false;
    KeyWasPressedMap[InputDirection::Down] = false;
    KeyWasPressedMap[InputDirection::Left] = false;
    KeyWasPressedMap[InputDirection::Right] = false;

	//물리 컴포넌트 추가
    PhysicsComponent* physicsComponent = new PhysicsComponent(this, CollisionType::Circle, PhysicsLayer::Player);
	//가능하면 Ellipse로 바꾸기
    physicsComponent->SetRadius(static_cast<float>(HeightSize * 0.5f));
    AddComponent(physicsComponent);

	//이동 컴포넌트 추가
	MoveComponent* moveComponent = new MoveComponent(this);
	moveComponent->SetGravity(980.0f); //중력 설정
    moveComponent->SetIsOnGround(false);
	AddComponent(moveComponent);
}

void Player::OnTick(float InDeltaTime)
{
    HandleInput();

    UpdateMovement(InDeltaTime);
}

void Player::HandleInput()
{
	MoveComponent* moveComp = GetComponent<MoveComponent>();
    if (!moveComp) return;

	//수평 이동
    if (KeyWasPressedMap[InputDirection::Left])
    {
        moveComp->MoveHorizontal(-Speed);
    }
    if (KeyWasPressedMap[InputDirection::Right])
    {
        moveComp->MoveHorizontal(+Speed);
    }
    else
    {
		moveComp->MoveHorizontal(0.0f);
    }

	bool IsPressedUp = KeyWasPressedMap[InputDirection::Up];

    if (IsPressedUp)
    {
		moveComp->AddVeriticalImpulse(JumpImpulse);
    }
}

void Player::UpdateMovement(float InDeltaTime)
{
	MoveComponent* moveComp = GetComponent<MoveComponent>();
    if (!moveComp) return;

	PointF NextPos = moveComp->CalculateNextPosition(InDeltaTime);
	PointF CurrentVel = moveComp->GetVelocity();

    // Floor_Block과의 충돌 검사
    bool IsGrounded = false;
    float GroundY = NextPos.Y;

	//GameManager에서 모든 Floor_Block 액터를 가져와서 충돌 검사
	PhysicsComponent* playerPhysics = GetComponent<PhysicsComponent>();
    if (playerPhysics)
    {

    }


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
	Key* key = dynamic_cast<Key*>(InOther);
    if (key && InOther != this)
    {
        PickupKey(key);
    }
    // 다른 아이템과 겹쳤을 때 처리
}

void Player::OnGroundCollision()
{
    MoveComponent* moveComp = GetComponent<MoveComponent>();
    if (moveComp)
    {
        moveComp->SetIsOnGround(true);
		moveComp->ResetVerticalVelocity();
	}
}

void Player::OnGroundLeft()
{
    MoveComponent* moveComp = GetComponent<MoveComponent>();
    if (moveComp)
    {
        moveComp->SetIsOnGround(false);
	}
}
void Player::PickupKey(Key* key)
{
    if (key)
    {
		int KeyId = key->GetKeyId();
		key->OnPlayerGetKey(); //열쇠 획득 처리
		AddKey(KeyId); //플레이어가 열쇠를 소유하게 함

		//디버그 메시지 출력
        std::wstring message = L"키 " + std::to_wstring(key->GetKeyId()) + L" 획득.\n";
        OutputDebugString(message.c_str());
    }
}

void Player::AddKey(int KeyId)
{
    for (int ownedKey : OwnedKeys)
    {
        if (ownedKey == KeyId)
        {
            OutputDebugString(L"오류!, 이미 소유한 키입니다.\n");
			return; //이미 소유한 열쇠면 오류
        }
    }
	//새로운 열쇠 추가
	OwnedKeys.push_back(KeyId);

	GameManager::Get().UpdatePalyerKeyInventory();
}

bool Player::HasKey(int KeyId)
{
    for(int ownedKey : OwnedKeys)
    {
        if (ownedKey == KeyId)
        {
            return true; //소유한 열쇠면 true 반환
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