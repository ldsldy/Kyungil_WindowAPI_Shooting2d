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

	//스크롤 속도
	//float ScrollSpeed = 50.0f;
	
	//스크롤 오프셋 값
	//float Offset = 0.0f;
};

