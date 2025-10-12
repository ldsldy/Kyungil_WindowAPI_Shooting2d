#include "KeyInventoryUI.h"
#include "GameManager.h"
#include "ResourceManager.h"

void KeyInventoryUI::OnInitialize()
{
	Actor::OnInitialize();

	SetSize(KeyIconSize); //크기 설정
	anchoredPosition = { -20,20 };			//앵커를 기준으로한 위치 설정
	anchor = { 1.0f,0.0f };					//앵커 포인트를 오른쪽 상단으로 설정
	Pivot = { 1.0f,0.0f };					//피벗 포인트를 오른쪽 상단으로 설정
	width = Size * 4 + 40;
	height = Size;
	
	UpdatePosition();
}

void KeyInventoryUI::OnTick(float deltaTime)
{
	Actor::OnTick(deltaTime);
}

void KeyInventoryUI::OnRender(Gdiplus::Graphics* graphics)
{
	if (!graphics)
	{
		OutputDebugString(L"KeyInventoryUI::OnRender - graphics is null\n");
		return;
	}

	//UI 영역 확인
	/*float PanelWidth = width;
	float PanelHeight = height;
	Gdiplus::SolidBrush BackgroundBrush(Gdiplus::Color(150, 0, 0, 0));
	graphics->FillRectangle(&BackgroundBrush, Position.X, Position.Y, PanelWidth, PanelHeight);*/

	//열쇠 아이콘 그리기
	for (int KeyIndex = 0; KeyIndex < 4; KeyIndex++)
	{
		float xPos = Position.X + KeyIndex * (KeyIconSize + KeyIconSpacing);
		float yPos = Position.Y;

		int keyId = KeyIndex + 1; //1부터 시작하는 열쇠 ID
		bool hasKey = std::find(CurrentKeys.begin(), CurrentKeys.end(), keyId) != CurrentKeys.end();
		
		////UI 영역 확인
		//Gdiplus::SolidBrush BackgroundBrush2(Gdiplus::Color(150, 100, 40*KeyIndex, 0));
		//graphics->FillRectangle(&BackgroundBrush2, xPos, yPos, KeyIconSize, KeyIconSize);

		ResourceID KeyResourceID = GetKeyResourceID(keyId);
		if(KeyResourceID != ResourceID::None)
		{
			//열쇠 이미지 가져오기
			Gdiplus::Bitmap* keyImage = ResourceManager::Get().GetImage(KeyResourceID);
			if (keyImage)
			{
				if(hasKey)
					graphics->DrawImage(keyImage, static_cast<int>(xPos), static_cast<int>(yPos), static_cast<int>(KeyIconSize), static_cast<int>(KeyIconSize));
				else
				{
					//열쇠가 없으면 반투명으로 표시
					Gdiplus::ColorMatrix colorMatrix = {
						1.0f, 0.0f, 0.0f, 0.0f, 0.0f, //Red
						0.0f, 1.0f, 0.0f, 0.0f, 0.0f, //Green
						0.0f, 0.0f, 1.0f, 0.0f, 0.0f, //Blue
						0.0f, 0.0f, 0.0f, 0.3f, 0.0f, //Alpha채널(알파값을 낮춤)
						0.0f, 0.0f, 0.0f, 0.0f, 1.0f  //변환 행렬
					};

					Gdiplus::ImageAttributes ImageAttr;
					ImageAttr.SetColorMatrix(&colorMatrix);

					Gdiplus::Rect destRect(static_cast<int>(xPos), static_cast<int>(yPos), static_cast<int>(KeyIconSize), static_cast<int>(KeyIconSize));
					graphics->DrawImage(keyImage, destRect, 0, 0, keyImage->GetWidth(), keyImage->GetHeight(), Gdiplus::UnitPixel, &ImageAttr);
				}
			}
		}
	}
}

void KeyInventoryUI::UpdateInventory(const std::vector<int>& ownedKeys)
{
	CurrentKeys = ownedKeys;
}

ResourceID KeyInventoryUI::GetKeyResourceID(int keyId)
{
	switch (keyId)
	{
	case 1:
		return ResourceID::Key_Blue;
	case 2:
		return ResourceID::Key_Red;
	case 3:
		return ResourceID::Key_Green;
	case 4:
		return ResourceID::Key_Yellow;
	default:
		return ResourceID::None; //알 수 없는 열쇠 ID
	}
}