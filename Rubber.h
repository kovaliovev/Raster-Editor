#pragma once

#include "Tool.h"

class Rubber : public virtual Tool {
public:
	Rubber();
	void SetColor(COLORREF) override;
};