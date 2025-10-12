#include "UserInterface.h"
#include "GameManager.h"

void UserInterface::UpdatePosition()
{
	//��Ŀ�� �Ǻ��� ����� ��ġ�� ������Ʈ
	GameManager& gameManager = GameManager::Get();
	Position.X = anchoredPosition.x + (anchor.X * GameManager::ScreenWidth) - (Pivot.X * width);
	Position.Y = anchoredPosition.y + (anchor.Y * GameManager::ScreenHeight) - (Pivot.Y * height);
}
