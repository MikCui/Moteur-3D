#include "Console.h"

int    CConsole::m_NbInstance = 0;
HANDLE CConsole::m_hConsole = NULL;

CConsole::CConsole(void)
{
	m_NbInstance++;
	if(m_NbInstance == 1)
	{
		AllocConsole();
		SetConsoleTitle("Debug Console");
		SetScreenBufferSize(80, 100);
		SetCursorPosition(0, 0);
		m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	}
}

CConsole::~CConsole(void)
{
	m_NbInstance--;
    if(m_NbInstance == 0)         
    {        
        FreeConsole();        
        CloseHandle(m_hConsole);  
		m_hConsole = NULL;
    }
}

void CConsole::Printf(const char *szfmt, ...)
{
	if(m_hConsole)
    {
		char s[300];
		va_list argptr;
		int cnt;
	    
		va_start(argptr, szfmt);
		cnt = vsprintf(s, szfmt, argptr);
		va_end(argptr);
	    
		DWORD cCharsWritten;
        WriteConsole(m_hConsole, s, (DWORD)strlen(s), &cCharsWritten, NULL);
    }
}

void CConsole::SetScreenBufferSize(short X, short Y)
{
	if(m_hConsole)
	{
		COORD co = {X, Y };
		SetConsoleScreenBufferSize(m_hConsole, co);
	}
}

void CConsole::SetCursorPosition(short X, short Y)
{
	if(m_hConsole)
	{
		COORD co = {X, Y };
		SetConsoleCursorPosition(m_hConsole,co);
	}
}

int  CConsole::GetNbInstance()
{
	return m_NbInstance;
}

CConsole& CConsole::operator << (int i)
{
	if(m_hConsole)
    {
		char s[50];
		sprintf(s, "%d", i);

		DWORD cCharsWritten;
        WriteConsole(m_hConsole, s, (DWORD)strlen(s), &cCharsWritten, NULL);
    }
	return *this;
}

CConsole& CConsole::operator << (double f)
{
	if(m_hConsole)
    {
		char s[50];
		sprintf(s, "%f", f);

		DWORD cCharsWritten;
        WriteConsole(m_hConsole, s, (DWORD)strlen(s), &cCharsWritten, NULL);
    }
	return *this;
}

CConsole& CConsole::operator << (char* s)
{
    if(m_hConsole)
    {
		DWORD cCharsWritten;
        WriteConsole(m_hConsole, s, (DWORD)strlen(s), &cCharsWritten, NULL);
    }
	return *this;
}

CConsole& CConsole::operator << (char c)
{
	if(m_hConsole)
    {
		char s[5];
		sprintf(s, "%c", c);

		DWORD cCharsWritten;
        WriteConsole(m_hConsole, s, (DWORD)strlen(s), &cCharsWritten, NULL);
    }
	return *this;
}

CConsole& CConsole::operator << (unsigned int d)
{
	if(m_hConsole)
    {
		char s[50];
		sprintf(s, "%u", d);

		DWORD cCharsWritten;
        WriteConsole(m_hConsole, s, (DWORD)strlen(s), &cCharsWritten, NULL);
    }
	return *this;
}

CConsole& CConsole::operator << (RECT rc)
{
	*this << endl;
	*this << "RECT.left = "   << rc.left   << endl;
	*this << "RECT.top = "    << rc.top    << endl;
	*this << "RECT.right = "  << rc.right  << endl;
	*this << "RECT.bottom = " << rc.bottom << endl;
	*this << endl;
	return *this;
}

