#pragma once
#include "Actor.h"

class Background : public Actor
{
public:
	Background() = delete;
	Background(ResourceID InID);

	virtual void OnTick(float InDeltaTime) override;
	virtual void OnRender(Gdiplus::Graphics* InGraphics) override;

private:
	int MapWidth;
	int MapHeight;

	//��ũ�� �ӵ�
	//float ScrollSpeed = 50.0f;
	
	//��ũ�� ������ ��
	//float Offset = 0.0f;
};

