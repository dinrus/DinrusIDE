//$ class РНЦП::ТопОкно {
public:
	virtual LRESULT  WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
	dword       style;
	dword       exstyle;
	HICON       ico, lico;

	void        удалиИконку();
	void        центрПрям(HWND owner, int center);

public:
	void       открой(HWND ownerhwnd);
	ТопОкно& Стиль(dword _style);
	dword      дайСтиль() const                       { return style; }
	ТопОкно& эксСтиль(dword _exstyle);
	dword      GetExStyle() const                     { return exstyle; }
//$ };