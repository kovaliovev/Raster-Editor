#pragma once

class File {
public:
	void OnNew(HWND);
	void OnSaveAs(HWND, LPCWSTR);
	void OnSave(HWND);
	void OnOpen(HWND, LPCWSTR);
};