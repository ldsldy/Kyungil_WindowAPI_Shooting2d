#include "Player.h"
#include "string"

Player::Player(const wchar_t* InImagePath)
{
    Position.X = 300.0f;
    Position.Y = 700.0f;
    
    KeyWasPressedMap[InputDirection::Left] = false;
    KeyWasPressedMap[InputDirection::Right] = false;
    KeyWasPressedMap[InputDirection::Up] = false;
    KeyWasPressedMap[InputDirection::Down] = false;
    
    Image = new Gdiplus::Bitmap(InImagePath);
    if (Image->GetLastStatus() != Gdiplus::Ok) //GdiPlus::Ok : 정상 상태
    {
        //정상적으로 이미지 로드 실패
        delete Image;
        Image = nullptr;
		OutputDebugString(L"플레이어 이미지 생성 실패\n");
        MessageBox(g_hMainWindow, L"플레이어 이미지 생성 실패", L"오류", MB_OK | MB_ICONERROR); //오류 메시지 출력, MB_OK : 확인 버튼, MB_ICONERROR : 오류 아이콘
    }
}
Player::~Player()
{
    if(Image)
    { 
        //플레이어 이미지 버퍼 삭제
        delete Image;
        Image = nullptr;
    }
}

void Player::Render(Gdiplus::Graphics* InGraphics)
{
    /*WCHAR buffer[256];
    swprintf_s(buffer, L"플레이어 델타타임 값: %.5f\n", DeltaTime);
    OutputDebugStringW(buffer);*/

    if (Image)
    {
        InGraphics->DrawImage(
            Image,  //그려질 이미지
            static_cast<int>(Position.X-PixelWidth*Pivot.X), 
            static_cast<int>(Position.Y-PixelHeight*Pivot.Y),       //그려질 위치
            PixelWidth, PixelHeight);  //그려질 사이즈
    }
    else
    {
        //플레이어가 없으면 원으로 대체
        Gdiplus::SolidBrush RedBrush(Gdiplus::Color(255, 255, 0, 0));
        InGraphics->FillEllipse(&RedBrush, 
            static_cast<int>(Position.X - PixelWidth * Pivot.X),
            static_cast<int>(Position.Y - PixelHeight * Pivot.Y), 
            PixelWidth, PixelHeight);
    }
}

void Player::HandleKeyState(WPARAM InKey, bool InIsPressed)
{
    InputDirection Direction;
    if (InKey == VK_LEFT || InKey == VK_RIGHT)
    {
        KeyWasPressedMap[static_cast<InputDirection>(InKey)] = InIsPressed;

        if (InKey == VK_LEFT)
        {
            Position.X -= Speed * DeltaTime;
            if (Position.X < PixelWidth)
            {
                Position.X = static_cast<float>(g_ScreenSize.X - PixelWidth);
                //Position.X = static_cast<float>(0 + PixelWidth);
            }
        }
        else if(InKey == VK_RIGHT)
        {
            Position.X += Speed * DeltaTime;
            if (g_ScreenSize.X - PixelWidth < Position.X)
            {
                Position.X = static_cast <float>(PixelWidth);
                //Position.X = static_cast <float>(g_ScreenSize.X - PixelWidth);
            }
		}
    }
}