#include "ResourceManager.h"
#include "GameManager.h"

void ResourceManager::Initialize()
{
	Resources[ResourceID::None] = nullptr;
	LoadResourceFromFile(ResourceID::Player, L"./Images/Player/Player.png");
	LoadResourceFromFile(ResourceID::Background, L"./Images/Background/Background.png");
	LoadResourceFromFile(ResourceID::Bomb, L"./Images/Interactable/Bomb.png");
	LoadResourceFromFile(ResourceID::Key_Blue, L"./Images/Interactable/key_blue.png");
	LoadResourceFromFile(ResourceID::Key_Green, L"./Images/Interactable/key_green.png");
	LoadResourceFromFile(ResourceID::Key_Red, L"./Images/Interactable/key_red.png");
	LoadResourceFromFile(ResourceID::Key_Yellow, L"./Images/Interactable/key_yellow.png");
	LoadResourceFromFile(ResourceID::Number0, L"./Images/UI/hud0.png");
	LoadResourceFromFile(ResourceID::Number1, L"./Images/UI/hud1.png");
	LoadResourceFromFile(ResourceID::Number2, L"./Images/UI/hud2.png");
	LoadResourceFromFile(ResourceID::Number3, L"./Images/UI/hud3.png");
	LoadResourceFromFile(ResourceID::Number4, L"./Images/UI/hud4.png");
	LoadResourceFromFile(ResourceID::Number5, L"./Images/UI/hud5.png");
	LoadResourceFromFile(ResourceID::Number6, L"./Images/UI/hud6.png");
	LoadResourceFromFile(ResourceID::Number7, L"./Images/UI/hud7.png");
	LoadResourceFromFile(ResourceID::Number8, L"./Images/UI/hud8.png");
	LoadResourceFromFile(ResourceID::Number9, L"./Images/UI/hud9.png");
	LoadResourceFromFile(ResourceID::Dot, L"./Images/UI/hudX.png");
	LoadResourceFromFile(ResourceID::UI_Key_Blue, L"./Images/UI/hud_key_blue.png");
	LoadResourceFromFile(ResourceID::UI_Key_Green, L"./Images/UI/hud_key_green.png");
	LoadResourceFromFile(ResourceID::UI_Key_Red, L"./Images/UI/hud_key_red.png");
	LoadResourceFromFile(ResourceID::UI_Key_Yellow, L"./Images/UI/hud_key_yellow.png");
}

void ResourceManager::Destroy()
{
	for (auto& pair : Resources)
	{
		delete pair.second;  //로드한 비트맵 모두 지우기
	}
	Resources.clear();
}

Gdiplus::Bitmap* ResourceManager::GetImage(ResourceID InID)
{
	auto iter = Resources.find(InID);
	if (iter != Resources.end())
	{
		return iter->second;
	}
	return nullptr;
}

bool ResourceManager::LoadResourceFromFile(ResourceID InID, const wchar_t* path)
{
	bool Result = false;
	if (path)
	{
		Gdiplus::Bitmap* Image = new Gdiplus::Bitmap(path); //이미지 로딩
		if (Image->GetLastStatus() != Gdiplus::Ok)
		{
			//이미지 로딩 실패
			delete Image; //바로 메모리 해제
			Image = nullptr;
			OutputDebugString(L"이미지 로드 실패");
			MessageBox(GameManager::Get().GetMainWindowHandle(),
				L"이미지 로드 실패", L"오류", MB_OK | MB_ICONERROR); //오류 메시지 박스 출력
		}
		else
		{
			Resources[InID] = Image;
			Result = true;
		}
	}

	return Result;
}
