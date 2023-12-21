#include "framework.h"
#include "resource.h"
#include "Rubber.h"

Rubber::Rubber() {
	SetColor(RGB(255, 255, 255));
	SetThick(1);
}

void Rubber::SetColor(COLORREF new_color) {
	Tool::SetColor(RGB(255, 255, 255));
}
