#pragma once
#include "../Singleton/SingletonBase.h"

class ShadowMap : public SingletonBase<ShadowMap>
{
public:
	ShadowMap();
	~ShadowMap();

	void Update()override;
	void Draw()override;

	void Init();

private:


	friend class SingletonBase<ShadowMap>;
};