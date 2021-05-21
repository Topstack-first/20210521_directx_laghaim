#ifndef _CHECKBOX_H_
#define _CHECKBOX_H_

class CSurface;

class CheckBox
{
public:
	CheckBox();
	~CheckBox();

public:
	void Init(const std::string& checked, const std::string& unchecked);
	void LoadRes();
	void DeleteRes();	
	HRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	void Draw();

public:
	void SetPosition(const int posX, const int posY);

	void SetChecked(const bool checked);
	bool GetChecked();

	void SetEnable(const bool enable);
	bool GetEnable();

	void SetVisible(const bool visible);
	bool GetVisible();

	void SetText(const std::string& text);
	const std::string& GetText();

private:
	bool isInside(int x, int y);

private:
	int posX;
	int posY;
	int width;
	int height;
	bool isChecked;
	bool isEnable;
	bool isVisible;

	std::string filenameChecked;
	std::string filenameUnchecked;

	CSurface* pSurChecked;
	CSurface* pSurUnchecked;
	CSurface* pSurText;

	std::string text;
};

#endif // _CHECKBOX_H_
