#include "KeyInventoryUI.h"
#include "GameManager.h"
#include "ResourceManager.h"

void KeyInventoryUI::OnInitialize()
{
	Actor::OnInitialize();

	SetSize(KeyIconSize); //ũ�� ����
	anchoredPosition = { -20,20 };			//��Ŀ�� ���������� ��ġ ����
	anchor = { 1.0f,0.0f };					//��Ŀ ����Ʈ�� ������ ������� ����
	Pivot = { 1.0f,0.0f };					//�ǹ� ����Ʈ�� ������ ������� ����
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

	//UI ���� Ȯ��
	/*float PanelWidth = width;
	float PanelHeight = height;
	Gdiplus::SolidBrush BackgroundBrush(Gdiplus::Color(150, 0, 0, 0));
	graphics->FillRectangle(&BackgroundBrush, Position.X, Position.Y, PanelWidth, PanelHeight);*/

	//���� ������ �׸���
	for (int KeyIndex = 0; KeyIndex < 4; KeyIndex++)
	{
		float xPos = Position.X + KeyIndex * (KeyIconSize + KeyIconSpacing);
		float yPos = Position.Y;

		int keyId = KeyIndex + 1; //1���� �����ϴ� ���� ID
		bool hasKey = std::find(CurrentKeys.begin(), CurrentKeys.end(), keyId) != CurrentKeys.end();
		
		////UI ���� Ȯ��
		//Gdiplus::SolidBrush BackgroundBrush2(Gdiplus::Color(150, 100, 40*KeyIndex, 0));
		//graphics->FillRectangle(&BackgroundBrush2, xPos, yPos, KeyIconSize, KeyIconSize);

		ResourceID KeyResourceID = GetKeyResourceID(keyId);
		if(KeyResourceID != ResourceID::None)
		{
			//���� �̹��� ��������
			Gdiplus::Bitmap* keyImage = ResourceManager::Get().GetImage(KeyResourceID);
			if (keyImage)
			{
				if(hasKey)
					graphics->DrawImage(keyImage, static_cast<int>(xPos), static_cast<int>(yPos), static_cast<int>(KeyIconSize), static_cast<int>(KeyIconSize));
				else
				{
					//���谡 ������ ���������� ǥ��
					Gdiplus::ColorMatrix colorMatrix = {
						1.0f, 0.0f, 0.0f, 0.0f, 0.0f, //Red
						0.0f, 1.0f, 0.0f, 0.0f, 0.0f, //Green
						0.0f, 0.0f, 1.0f, 0.0f, 0.0f, //Blue
						0.0f, 0.0f, 0.0f, 0.3f, 0.0f, //Alphaä��(���İ��� ����)
						0.0f, 0.0f, 0.0f, 0.0f, 1.0f  //��ȯ ���
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
		return ResourceID::None; //�� �� ���� ���� ID
	}
}