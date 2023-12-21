#include "framework.h"
#include "resource.h"
#include "Rubber.h"

Rubber::Rubber() {
	SetColor(COL_WHITE);
	SetThick(1);
}

void Rubber::SetColor(COLORREF new_color) {
	Tool::SetColor(COL_WHITE);
}
