#pragma once
#include "Transitor.h"

class FadeTransitor : public Transitor
{
public:
	FadeTransitor();
	~FadeTransitor();

	void Update()override;
	void Draw()override;

private:

};