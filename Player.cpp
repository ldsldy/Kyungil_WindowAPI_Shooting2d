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
    if (Image->GetLastStatus() != Gdiplus::Ok) //GdiPlus::Ok : ���� ����
    {
        //���������� �̹��� �ε� ����
        delete Image;
        Image = nullptr;
		OutputDebugString(L"�÷��̾� �̹��� ���� ����\n");
        MessageBox(g_hMainWindow, L"�÷��̾� �̹��� ���� ����", L"����", MB_OK | MB_ICONERROR); //���� �޽��� ���, MB_OK : Ȯ�� ��ư, MB_ICONERROR : ���� ������
    }
}
Player::~Player()
{
    if(Image)
    { 
        //�÷��̾� �̹��� ���� ����
        delete Image;
        Image = nullptr;
    }
}

void Player::Render(Gdiplus::Graphics* InGraphics)
{
    /*WCHAR buffer[256];
    swprintf_s(buffer, L"�÷��̾� ��ŸŸ�� ��: %.5f\n", DeltaTime);
    OutputDebugStringW(buffer);*/

    if (Image)
    {
        InGraphics->DrawImage(
            Image,  //�׷��� �̹���
            static_cast<int>(Position.X-PixelWidth*Pivot.X), 
            static_cast<int>(Position.Y-PixelHeight*Pivot.Y),       //�׷��� ��ġ
            PixelWidth, PixelHeight);  //�׷��� ������
    }
    else
    {
        //�÷��̾ ������ ������ ��ü
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