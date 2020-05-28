#pragma warning(disable : 4244)
#pragma warning(disable : 4996 4005)
#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#ifdef UNICODE
#undef UNICODE
#define _MBCS
#endif
// pliki biblioteki standardowej jezyka C:
#include <math.h>
#include <stdlib.h>
#define M_2PI (2.0 * M_PI)
// plik naglowkowy z deklaracjami elementow interfejsu Win32 API:
#include <windows.h>
// pliki naglowkowe z deklaracjami elementow biblioteki OpenGL:
#include <gl\gl.h>
#include <gl\glu.h>
char nazwaKlasy[] = "Kostka";
char tytulOkna[] = "Album na kostce";
LRESULT CALLBACK funOkna(HWND okno, UINT komunikat, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE prog, HINSTANCE _, LPSTR __, int trybOkna) {
	WNDCLASS klasa; // klasa okna
	HWND okno;      // okno glowne
	// utworzenie i rejestrowanie nowej klasy okna:
	klasa.style = CS_HREDRAW | CS_VREDRAW;
	klasa.hInstance = prog;
	klasa.lpszClassName = nazwaKlasy;
	klasa.lpfnWndProc = funOkna;
	klasa.hCursor = LoadCursor(NULL, IDC_ARROW);
	klasa.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	klasa.hbrBackground = NULL; // brak automatycznie rysowanego tla okna
	klasa.lpszMenuName = NULL;
	klasa.cbClsExtra = 0;
	klasa.cbWndExtra = 0;
	if (!RegisterClass(&klasa))
		return 0;
	// utworzenie, wyswietlanie i aktualizacja okna glownego:
	okno = CreateWindowEx(0, nazwaKlasy, tytulOkna, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, prog, NULL);
	ShowWindow(okno, trybOkna);
	UpdateWindow(okno);
	// petla glowna programu do obslugi zdarzen oraz wykonywania obliczen:
	{
		MSG komunikat; // komunikat dot. zdarzenia w systemie okienkowym
		while (TRUE)
		{
			if (PeekMessage(&komunikat, 0, 0, 0, PM_REMOVE)) // obsluga zdarzen
			{
				if (komunikat.message == WM_QUIT)
					break; // przerwanie petli
				TranslateMessage(&komunikat);
				DispatchMessage(&komunikat);
			}
			else // obliczenia programu
			{
				//BOOL zmiana = funProg(prog);
				//if (zmiana)
				//	InvalidateRect(okno, NULL, FALSE); // odswiezenie zawartosci okna glownego
			}
		}
		// zakonczenie programu:
		return (int)komunikat.wParam;
	}
}
LRESULT CALLBACK funOkna(HWND okno, UINT komunikat, WPARAM wParam, LPARAM lParam){
	return 0;
}