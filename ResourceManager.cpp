#include "ResourceManager.h"
#include "GameManager.h"

void ResourceManager::Initialize()
{
	Resources[ResourceID::None] = nullptr;
	LoadResourceFromFile(ResourceID::Player, L"./Images/Player/Player.png");
	LoadResourceFromFile(ResourceID::Background, L"./Images/Background/Background.png");
	LoadResourceFromFile(ResourceID::Bomb, L"./Images/Interactable/Bomb.png");
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
}

void ResourceManager::Destroy()
{
	for (auto& pair : Resources)
	{
		delete pair.second;  //�ε��� ��Ʈ�� ��� �����
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
		Gdiplus::Bitmap* Image = new Gdiplus::Bitmap(path); //�̹��� �ε�
		if (Image->GetLastStatus() != Gdiplus::Ok)
		{
			//�̹��� �ε� ����
			delete Image; //�ٷ� �޸� ����
			Image = nullptr;
			OutputDebugString(L"�̹��� �ε� ����");
			MessageBox(GameManager::Get().GetMainWindowHandle(),
				L"�̹��� �ε� ����", L"����", MB_OK | MB_ICONERROR); //���� �޽��� �ڽ� ���
		}
		else
		{
			Resources[InID] = Image;
			Result = true;
		}
	}

	return Result;
}
