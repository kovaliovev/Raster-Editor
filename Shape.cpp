#include "framework.h"
#include "Shape.h"

Shape::~Shape() {}

void Shape::SetStartCords(int new_x_start, int new_y_start) {
	shape_x_1 = new_x_start;
	shape_y_1 = new_y_start;
}

void Shape::SetEndCords(int new_x_end, int new_y_end) {
	shape_x_2 = new_x_end;
	shape_y_2 = new_y_end;
}

void Shape::SetColor(COLORREF new_color) {
	shape_color = new_color;
}

void Shape::SetThick(int new_thick) {
	shape_thick = new_thick;
}