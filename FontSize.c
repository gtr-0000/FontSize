#include <stdio.h>
#include <string.h>
#include <windows.h>

// #ref: https://docs.microsoft.com/en-us/windows/console/setcurrentconsolefontex
typedef struct _CONSOLE_FONT_INFOEX
{
	ULONG	cbSize;
	DWORD	nFont;
	COORD	dwFontSize;
	UINT	FontFamily;
	UINT	FontWeight;
	WCHAR	FaceName[LF_FACESIZE];
} CONSOLE_FONT_INFOEX, *PCONSOLE_FONT_INFOEX;

BOOL WINAPI SetCurrentConsoleFontEx
	(
		HANDLE			hConsoleOutput,
		BOOL			bMaximumWindow,
		PCONSOLE_FONT_INFOEX	lpConsoleCurrentFontEx
	);

// #ref: https://blog.csdn.net/hilavergil/article/details/78449291
WCHAR *charToWCHAR(char *s)
{
	int	w_nlen = MultiByteToWideChar(CP_ACP, 0, s, -1, NULL, 0);
	WCHAR	*ret = (WCHAR *) malloc(sizeof(WCHAR) * w_nlen);
	memset(ret, 0, sizeof(ret));
	MultiByteToWideChar(CP_ACP, 0, s, -1, ret, w_nlen);
	return ret;
}

int main(int argc, char *argv[])
{
	CONSOLE_FONT_INFOEX	cfi;
	HANDLE			soh;
	int			wid, hei;
	WCHAR			*fac = L"Terminal";

	if(argc <= 2 || argc > 4) goto help;

	wid = atoi(argv[1]), hei = atoi(argv[2]);
	if(wid <= 0 || hei <= 0) goto help;
	if(argc == 4) fac = charToWCHAR(argv[3]);

	memset(&cfi, 0, sizeof cfi);
	cfi.cbSize = sizeof cfi;
	cfi.dwFontSize.X = wid;
	cfi.dwFontSize.Y = hei;
	if(fac != NULL) wcscpy(cfi.FaceName, fac);

	soh = GetStdHandle(STD_OUTPUT_HANDLE);
	if(!SetCurrentConsoleFontEx(soh, FALSE, &cfi))
	{
		fprintf(stderr, "Failed to set console font.\n");
		return 1;
	}
	else
		return 0;

help:
	printf("Usage: FontSize <Width> <Height> [FontName(default:Terminal)]\n");
	printf("用法: FontSize <宽度> <高度> [字体名称(默认字体:Terminal)]\n");
	return 0;
}
