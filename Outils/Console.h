#pragma once
#include <Windows.h>
#include <stdio.h>

const char endl = '\n';

class CConsole
{
public:
	CConsole(void);
	~CConsole(void);
	void Printf(const char *szfmt, ...);
	void SetScreenBufferSize(short X, short Y);
	void SetCursorPosition(short X, short Y);
	int  GetNbInstance();

	CConsole& operator << (int i);
	CConsole& operator << (double f);
	CConsole& operator << (char* s);
	CConsole& operator << (char c);
	CConsole& operator << (unsigned int d);
	CConsole& operator << (RECT rc);

private:
	static HANDLE	m_hConsole;
	static int		m_NbInstance;

};