/***************************************/
/* Transformacje w scenie Win32-OpenGL */
/***************************************/
// dodatkowa konfiguracja dla kompilatora MSVC
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
#include <iostream>
#include <vector>
// pliki naglowkowe z deklaracjami elementow biblioteki OpenGL:
#include <gl\gl.h>
#include <gl\glu.h>
using namespace std;
// opis klasy, okna i ekranu:
char nazwaKlasy[] = "Kostka";
char tytulOkna[] = "Album na kostce";
const char nazwy[6][256] = {
	"zdjecie1.bmp","zdjecie2.bmp","zdjecie3.bmp","zdjecie4.bmp","zdjecie5.bmp","zdjecie6.bmp"
};
static struct
{
	double X, Y, Z, A, B, C, U, V, W;
} widok = { 0.0 }; // parametry widoku sceny
static struct
{
	float kat1, kat2, kat3;
} ruch = { 0.0f }; // parametry ruchu w scenie
struct zdjecie {
	int szerokosc;
	int wysokosc;
	int rozmiar;
	unsigned char * obraz;
	zdjecie(int szer=0,int wys=0,int rozm=0,unsigned char * obr=nullptr):szerokosc(szer),wysokosc(wys),rozmiar(rozm),obraz(obr){}
};
BOOL pelnyEkran = FALSE;
//textury programu
GLuint textury[6];
//zdjecie zdjecia[6];
vector<zdjecie> zdjecia;
// deklaracja funkcji okna do obslugi zdarzen:
LRESULT CALLBACK funOkna(HWND okno, UINT komunikat, WPARAM wParam, LPARAM lParam);
// deklaracja funkcji programu do wykonywania obliczen:
BOOL funProg(HINSTANCE prog);
unsigned char* readBMP(const char* filename, int & szerokosc, int &wysokosc, int & rozmiar);
// glowny kod programu:
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
	if (!pelnyEkran)
		okno = CreateWindowEx(0, nazwaKlasy, tytulOkna, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, prog, NULL);
	else
	{
		DEVMODE ekran = { 0 }; // tryb i rozmiar ekranu
		int szerEkranu = 800;
		int wysEkranu = 600;
		ekran.dmSize = sizeof(DEVMODE);
		ekran.dmPelsWidth = szerEkranu;
		ekran.dmPelsHeight = wysEkranu;
		ekran.dmBitsPerPel = 16;
		ekran.dmDisplayFrequency = 75;
		ekran.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL | DM_DISPLAYFREQUENCY;
		ChangeDisplaySettings(&ekran, CDS_FULLSCREEN);
		okno = CreateWindowEx(0, nazwaKlasy, tytulOkna, WS_POPUP, 0, 0, szerEkranu, wysEkranu, NULL, NULL, prog, NULL);
	}
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
				BOOL zmiana = funProg(prog);
				if (zmiana)
					InvalidateRect(okno, NULL, FALSE); // odswiezenie zawartosci okna glownego
			}
		}

		// zakonczenie programu:
		return (int)komunikat.wParam;
	}
}
// deklaracja globalnych danych i procedur sceny do przetwarzania grafiki:
static HGLRC scena = NULL; // kontekst sceny z grafika GL
static void Rozpoczecie(HWND okno);
static void Zakonczenie(HWND okno);
static void Dopasowanie(HDC graf, int szer, int wys);
static void Wyswietlanie(HDC graf, int szer, int wys);
static void Renderowanie(double asp);
static void Modelowanie(void);
static BOOL Oddzialywanie(double pozm, double pion, double pros, double kret);
static BOOL Obliczenia(void);
// funkcja okna do obslugi zdarzen:
LRESULT CALLBACK funOkna(HWND okno, UINT komunikat, WPARAM wParam, LPARAM lParam)
{
	static int kursX = -1, kursY = -1; // biezace polozenie kursora myszki
	switch (komunikat)
	{
	case WM_CREATE: // utworzenie okna
	{
		Rozpoczecie(okno);
		{ // ukrycie kursora myszki i zapamietanie jego polozenia w oknie
			POINT kurs;
			ShowCursor(FALSE);
			GetCursorPos(&kurs);         // pobranie globalnego polozenia kursora wzgledem ekranu
			ScreenToClient(okno, &kurs); // obliczenie polozenia lokalnego wzgledem okna
			kursX = kurs.x;
			kursY = kurs.y;
		}
		Oddzialywanie(0, 0, 0, 0); // ustawienie stanu poczatkowego widoku sceny
		return 0;
	}
	case WM_DESTROY: // zniszczenie okna
	{
		ShowCursor(TRUE);
		Zakonczenie(okno);
		PostQuitMessage(0); // wyjscie z glownej petli programu
		return 0;
	}
	case WM_CLOSE: // zamkniecia okna
	{
		ShowCursor(TRUE);
		if (MessageBox(okno, "Czy zako�czy� program?", "Grafika WinGL", MB_YESNOCANCEL) == IDYES)
			DestroyWindow(okno); // zniszczenie okna glownego i wyjscie z programu
		else
			ShowCursor(FALSE);
		return 0;
	}
	case WM_SIZE: // ustawienie rozmiaru okna
	{
		int szer = LOWORD(lParam); // nowa szerokosc okna
		int wys = HIWORD(lParam);  // nowa wysokosc okna
		HDC grafika = GetDC(okno); // kontekst wyswietlania grafiki w oknie
		Dopasowanie(grafika, szer, wys);
		ReleaseDC(okno, grafika);
		return 0;
	}
	case WM_PAINT: // rysowanie zawartosci okna
	{
		RECT obszar; // obszar rysowania okna
		GetClientRect(okno, &obszar);
		{
			HDC grafika;         // kontekst wyswietlania grafiki w oknie
			PAINTSTRUCT rysunek; // struktura rysunku
			grafika = BeginPaint(okno, &rysunek);
			Wyswietlanie(grafika, obszar.right, obszar.bottom);
			EndPaint(okno, &rysunek);
		}
		return 0;
	}
	//case WM_MOUSEMOVE: // przesuwanie myszki w oknie
	//{
	//	POINT kurs = { LOWORD(lParam), HIWORD(lParam) }; // loklane polozenie kursora myszki
	//	int szer, wys;                                 // rozmiary obszaru okna
	//	{
	//		RECT obszar;
	//		GetClientRect(okno, &obszar);
	//		szer = obszar.right;
	//		wys = obszar.bottom;
	//	}
	//	if (szer > 0 && wys > 0 && kurs.x >= 0 && kurs.x < szer && kurs.y >= 0 && kurs.y < wys)
	//	{
	//		BOOL zmiana = Oddzialywanie(1.0 * (kurs.x - kursX) / szer, 1.0 * (kurs.y - kursY) / wys, 0.0, 0.0);
	//		if (zmiana)
	//			InvalidateRect(okno, NULL, FALSE); // odswiezenie zawartosci okna
	//	}
	//	kursX = kurs.x;
	//	kursY = kurs.y; // poprzednie polozenie kursora
	//	return 0;
	//}
	case WM_KEYDOWN: // nacisniecie klawiatury w oknie
	{
		int klawisz = (int)(wParam); // kod biezacego klawisza klawiatury
		int wert = 0;                // zwrot klawisza ze strzalka wertykalna
		int horz = 0;                // zwrot klawisza ze strzalka horyzontalna
		if (klawisz == VK_UP)
			wert = +10;
		else if (klawisz == VK_DOWN)
			wert = -10;
		else if (klawisz == VK_RIGHT)
			horz = +10;
		else if (klawisz == VK_LEFT)
			horz = -10;
		{
			BOOL zmiana = Oddzialywanie(0.0, 0.0, 0.1 * wert, 0.01 * horz);
			if (zmiana)
				InvalidateRect(okno, NULL, FALSE); // odswiezenie zawartosci okna
		}
		return 0;
	}
	case WM_CHAR: // wprowadzenie znaku do okna
	{
		if ((int)wParam == VK_ESCAPE)
			DestroyWindow(okno); // zniszczenie okna glownego i wyjscie z programu
		return 0;
	}
	default: // domyslna obsluga komunikatu
	{
		return DefWindowProc(okno, komunikat, wParam, lParam);
	}
	}
}
// funkcja programu do wykonywania obliczen:
BOOL funProg(HINSTANCE prog)
{
	return Obliczenia();
}
// procedura rozpoczecia przetwarzania grafiki GL w oknie Win32:
void Rozpoczecie(HWND okno)
{
	HDC grafika = GetDC(okno); // tymczasowe pobranie kontekstu wyswietlania grafiki w podanym oknie
	{                          // ustawienie formatu pikseli w grafice
		static PIXELFORMATDESCRIPTOR opisFormatu =
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, // rysowanie grafiki bedzie w oknie ze wsparciem dla OpenGL oraz z podwojnym buforowaniem obrazu
			PFD_TYPE_RGBA,                                              // kolory grafiki definiowane beda przez podanie skladnikow R,G,B i parametru alfa
			32,
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			16,
			0,
			0,
			PFD_MAIN_PLANE,
			0,
			0, 0, 0 };
		int format = ChoosePixelFormat(grafika, &opisFormatu);
		SetPixelFormat(grafika, format, &opisFormatu);
	}
	scena = wglCreateContext(grafika); // utworzenie globalnego kontekstu renderowania sceny
	wglMakeCurrent(grafika, scena);    // wyroznienie kontekstu sceny z grafika 3D (GL) w kontekscie grafiki 2D (GDI)
	ReleaseDC(okno, grafika);          // zwolnienie kontekstu grafiki podanego okna
	{                                  // wstepna konfiguracja srodowiska GL
		glEnable(GL_DEPTH_TEST);       // wlaczenie bufora do testowania glebi w renderowaniu sceny 3D
		glShadeModel(GL_SMOOTH);       // wybranie trybu cieniowania poprzez interpolacje kolorow na powierzchni figury
	}
	for (int i = 0; i < 6; i++) {
		int szerokosc, wysokosc, rozmiar;
		unsigned char * obraz = readBMP(nazwy[i], szerokosc, wysokosc, rozmiar);
		zdjecia.push_back(zdjecie(szerokosc, wysokosc, rozmiar, obraz));
	}
}
// procedura zakonczenia przetwarzania grafiki GL w oknie Win32:
void Zakonczenie(HWND okno){
	HDC grafika = GetDC(okno);     // tymczasowe pobranie kontekstu wyswietlania grafiki w podanym oknie
	wglMakeCurrent(grafika, NULL); // anulowanie kontekstu sceny w kontekscie grafiki
	wglDeleteContext(scena);       // usuniecie kontekstu renderowania sceny
	scena = NULL;
	ReleaseDC(okno, grafika); // zwolnienie kontekstu grafiki podanego okna
	glDeleteTextures(6, textury);
	for (auto&x : zdjecia)
		delete[]x.obraz;
}
// procedura dopasowanie grafiki GL po zmianie rozmiaru okna:
void Dopasowanie(HDC graf, int szer, int wys){
	if (szer <= 0 || wys <= 0)
		return;
	glViewport(0, 0, szer, wys); // ustawienie portu widokowego bedacego obszarem p/n rzutowania sceny w obrebie okna
	glMatrixMode(GL_PROJECTION); // wyroznienie transformacji rzutowania sceny 3D na plaszczyzne widokowej
	glLoadIdentity();            // przypisanie macierzy jednostkowej do p/w transformacji
	gluPerspective(60.0, (double)szer / wys, 1.0, 1000.0); // przypisanie macierzy perspektywy do p/w transformacji
	glMatrixMode(GL_MODELVIEW); // wyroznienie transformacji modelu i/lub widoku w scenie 3D
}
// procedura wyswietlania grafiki GL w oknie:
void Wyswietlanie(HDC graf, int szer, int wys)
{
	if (szer <= 0 || wys <= 0)
		return;
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);               // ustawienie koloru RGBA dla tla sceny
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // poczatkowe wpisanie koloru tla do bufora z kolorami pikseli na obrazie sceny oraz wyzerowanie bufora glebi
	glLoadIdentity();                                   // przypisanie macierzy jednostkowej do p/w transformacji model-widok (zresetowanie stanu transformacji)
	//Zadanie: zakomentuj p/w operacje, aby zachowacpoprzednia macierz w transformacji model-widok
	Renderowanie((double)szer / wys); // ponowne renderowanie sceny po odswiezeniu zawartosci okna o biezacym aspekcie geometrycznym
	SwapBuffers(graf);                // po utworzeniu obrazu sceny na tylnim buforze zamiana buforow obrazu miejscami
}
// procedura odtworzenia modelu sceny w postaci grafiki GL:
void Renderowanie(double asp){
	if (asp > 1)
		asp = 1;
	gluLookAt(                                       // przypisanie macierzy obserwacji do p/w transformacji
		widok.X / asp, widok.Y / asp, widok.Z / asp, // polozenie miejsca obserwacji
		widok.A, widok.B, widok.C,                   // punkt skupienia obserwacji - srodek sceny
		widok.U, widok.V, widok.W                    // kierunek pionu obserwacji
	);
	Modelowanie(); // modelowanie zawartosci graficznej w scenie
	glFlush();     // natychmiastowe utworzenie obrazu sceny na ekranie lub w buforze i powrot do wyswietlania grafiki
}
// procedura podstawowego modelowania graficznego w scenie:
unsigned char* readBMP(const char* filename, int & szerokosc, int &wysokosc, int & rozmiar) {
	int i;
	FILE* f = fopen(filename, "rb");
	unsigned char info[54];
	// read the 54-byte header
	fread(info, sizeof(unsigned char), 54, f);
	// extract image height and width from header
	szerokosc = *(int*)&info[18];
	wysokosc = *(int*)&info[22];
	// allocate 3 bytes per pixel
	rozmiar = 3 * szerokosc*wysokosc;
	unsigned char* data = new unsigned char[rozmiar];
	// read the rest of the data at once
	fread(data, sizeof(unsigned char), rozmiar, f);
	fclose(f);
	return data;
}
void Modelowanie(void)
{
	glEnable(GL_TEXTURE_2D);
	glPushMatrix(); // zapamietanie lokalnej macierzy dla p/w transformacji model-widok
	{
		glGenTextures(6, textury);
		for (int i = 0; i < 6; i++) {
			//ładowanie obrazka
			glBindTexture(GL_TEXTURE_2D, textury[i]);
			gluBuild2DMipmaps(
				GL_TEXTURE_2D,//wybor textury plaskiej
				GL_RGB,//paleta rgb
				zdjecia[i].szerokosc,
				zdjecia[i].wysokosc,
				GL_BGR_EXT,
				GL_UNSIGNED_BYTE,
				zdjecia[i].obraz
			);
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		for (int i = 0; i < 6; i++)
		{
			glBindTexture(GL_TEXTURE_2D, textury[i]);
			glBegin(GL_QUADS); // definicja gornej sciany szescianu nad srodkiem sceny
			{
				glTexCoord2f(0.0, 0.0);
				glColor3f(1.0f, 1.0f, 1.0f);
				glVertex3f(-1.0f, 1.0f, 1.0f);
				glTexCoord2f(1.0, 0.0);
				glVertex3f(-1.0f, -1.0f, 1.0f);
				glTexCoord2f(0.0, 1.0);
				glVertex3f(1.0f, -1.0f, 1.0f);
				glTexCoord2f(1.0, 1.0);
				glVertex3f(1.0f, 1.0f, 1.0f);
			}
			glEnd();
			if (i % 2 == 0)
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // pomocniczy obrot p/w sciany wokol OX lub OY tak aby otrzymac pozostale sciany w szescianie
			else
				glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
		}
	}
	glPopMatrix(); // zresetowanie p/w transformacji model-widok do macierzy lokalnej
}
// procedura oddzialywania uzytkownika na widok sceny:
BOOL Oddzialywanie(double pozm, double pion, double pros, double kret)
{
#define Ro 10.0
	static double teta = M_PI_2, fi = 0.0, prom = Ro, psi = M_PI_2; // biezace parametry stanu dla widoku sceny
	teta += pion * M_PI;
	if (teta < 0.0)
		teta = 0.0;
	else if (teta > M_PI)
		teta = M_PI;
	fi += pozm * M_2PI;
	fi = fmod(fi, M_2PI);
	prom += pros * Ro;
	if (prom < 0.01 * Ro)
		prom = 0.01 * Ro;
	else if (prom > 100.0 * Ro)
		prom = 100.0 * Ro;
	psi += kret * M_2PI;
	psi = fmod(psi, M_2PI);
	widok.Z = prom * sin(teta) * cos(fi);
	widok.X = prom * sin(teta) * sin(fi);
	widok.Y = prom * cos(teta);
	widok.U = cos(psi);
	widok.V = sin(psi);
	widok.W = widok.A = widok.B = widok.C = 0.0;
#undef Ro
	return TRUE; // wyswietlenie grafiki GL w oknie
}
// procedura iteracyjnych obliczen dla modelu graficznego:
BOOL Obliczenia(void){
	ruch.kat1 += 1.0f; // nastepne katy obrotu figur w modelu sceny
	ruch.kat2 -= 2.0f;
	ruch.kat3 += 3.0f;
	Sleep(10);   // wstrzymanie programu przez podana liczbe milisekund
	return TRUE; // wyswietlenie grafiki GL w oknie
}