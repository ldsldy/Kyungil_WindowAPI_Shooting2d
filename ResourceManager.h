#pragma once
#include <unordered_map>
#include "Common.h"
#include "Singleton.h"

class ResourceManager : public Singleton<ResourceManager>
{
	friend class Singleton<ResourceManager>;

public:
	/// <summary>
	/// 사용할 모든 이미지를 미리 로드
	/// </summary>
	void Initialize();
	void Destroy();

	Gdiplus::Bitmap* GetImage(ResourceID InID);

private:
	ResourceManager() = default;
	virtual ~ResourceManager() = default;

	/// <summary>
	/// 파일에서	리소스를 로딩하는 함수
	/// </summary>
	/// <param name="InID">이미지에 매핍 시킬 ID</param>
	/// <param name="path">이미지의 경로</param>
	/// <returns></returns>
	bool LoadResourceFromFile(ResourceID InID, const wchar_t* path); //파일에서 리소스를 로딩하는 함수

	std::unordered_map<ResourceID, Gdiplus::Bitmap*> Resources; // 리소스 ID와 Gdiplus::Bitmap 포인터를 매핑하는 맵
};