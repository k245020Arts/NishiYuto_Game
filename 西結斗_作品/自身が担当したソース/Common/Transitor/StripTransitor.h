#pragma once
#include "Transitor.h"

class StripTransitor : public Transitor
{
public:
	StripTransitor();
	~StripTransitor();

	void Update()override;
	void Draw()override;

private:

};
