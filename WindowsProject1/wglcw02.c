///**********************************************/
///* Badanie wyswietlania w scenie Win32-OpenGL */     
///**********************************************/
//
// dodatkowa konfiguracja dla kompilatora MSVC
//#pragma warning(disable:4244)
//#pragma warning(disable:4996 4005)
//#define _CRT_SECURE_NO_WARNINGS
//#define _USE_MATH_DEFINES
//#ifdef UNICODE
//# undef UNICODE
//# define _MBCS
//#endif
//
// pliki biblioteki standardowej jezyka C:
//#include <stdlib.h>
//
// plik naglowkowy z deklaracjami elementow interfejsu Win32 API:
//#include <windows.h>
//
// pliki naglowkowe z deklaracjami elementow biblioteki OpenGL:
//#include <gl\gl.h>
//#include <gl\glu.h>
//
// opis klasy, okna i ekranu:
//char nazwaKlasy[] = "OknoSceny";
//char tytulOkna[] = "Wyswietlanie w WinGL";
//BOOL pelnyEkran = FALSE;
//
// deklaracja funkcji okna do obslugi zdarzen:
//LRESULT CALLBACK funOkna(HWND okno, UINT komunikat, WPARAM wParam, LPARAM lParam);
//
// deklaracja funkcji programu do wykonywania obliczen:
//BOOL funProg(HINSTANCE prog);
//
// glowny kod programu:
//int WINAPI WinMain(HINSTANCE prog, HINSTANCE _, LPSTR __, int trybOkna)
//{
// WNDCLASS klasa; // klasa okna
// HWND okno; // okno glowne
//
//  utworzenie i rejestrowanie nowej klasy okna:
// klasa.style = CS_HREDRAW | CS_VREDRAW;
// klasa.hInstance = prog;
// klasa.lpszClassName = nazwaKlasy;
// klasa.lpfnWndProc = funOkna;
// klasa.hCursor = LoadCursor(NULL, IDC_ARROW);
// klasa.hIcon = LoadIcon(NULL, IDI_APPLICATION);
// klasa.hbrBackground = NULL; // brak automatycznie rysowanego tla okna
// klasa.lpszMenuName = NULL;
// klasa.cbClsExtra = 0;
// klasa.cbWndExtra = 0;
// if (!RegisterClass(&klasa)) return 0;
//
//  utworzenie, wyswietlanie i aktualizacja okna glownego:
// if (!pelnyEkran) okno = CreateWindowEx(0, nazwaKlasy, tytulOkna, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, prog, NULL);
// else {
//  DEVMODE ekran = { 0 }; // tryb i rozmiar ekranu
//  int szerEkranu = 800;
//  int wysEkranu = 600;
//  ekran.dmSize = sizeof(DEVMODE);
//  ekran.dmPelsWidth = szerEkranu;
//  ekran.dmPelsHeight = wysEkranu;
//  ekran.dmBitsPerPel = 16;
//  ekran.dmDisplayFrequency = 75;
//  ekran.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL | DM_DISPLAYFREQUENCY;
//  ChangeDisplaySettings(&ekran, CDS_FULLSCREEN);
//  okno = CreateWindowEx(0, nazwaKlasy, tytulOkna, WS_POPUP, 0, 0, szerEkranu, wysEkranu, NULL, NULL, prog, NULL);
// }
// ShowWindow(okno, trybOkna);
// UpdateWindow(okno);
//
//  petla glowna programu do obslugi zdarzen oraz wykonywania obliczen:
// {
//  MSG komunikat; // komunikat dot. zdarzenia w systemie okienkowym
//  while (TRUE)
//  {
//   if (PeekMessage(&komunikat, 0, 0, 0, PM_REMOVE)) // obsluga zdarzen
//   {
//    if (komunikat.message == WM_QUIT) break; // przerwanie petli
//    TranslateMessage(&komunikat);
//    DispatchMessage(&komunikat);
//   }
//   else // obliczenia programu
//   {
//    BOOL zmiana = funProg(prog);
//    if (zmiana) InvalidateRect(okno,NULL,FALSE); // odswiezenie zawartosci okna glownego
//   }
//  }
//
//   zakonczenie programu:
//  return (int)komunikat.wParam;
// }
//}
//
// deklaracja globalnych danych i procedur sceny do przetwarzania grafiki:
//static HGLRC scena = NULL; // kontekst sceny z grafika GL
//static void Rozpoczecie(HWND okno);
//static void Zakonczenie(HWND okno);
//static void Dopasowanie(HDC graf, int szer, int wys);
//static void Wyswietlanie(HDC graf, int szer, int wys);
//static void Renderowanie1(double asp);
//static void Renderowanie2(double asp);
//static void Renderowanie3(double asp);
//static void Modelowanie(void);
//
// funkcja okna do obslugi zdarzen:
//LRESULT CALLBACK funOkna(HWND okno, UINT komunikat, WPARAM wParam, LPARAM lParam)
//{
// switch (komunikat)
// {
//  case WM_CREATE: // utworzenie okna
//  {
//   Rozpoczecie(okno);
//   return 0;
//  }
//  case WM_DESTROY: // zniszczenie okna
//  {
//   Zakonczenie(okno);
//   PostQuitMessage(0); // wyjscie z glownej petli programu  
//   return 0;
//  }
//  case WM_CLOSE: // zamkniecia okna
//  {
//   if (MessageBox(okno, "Czy zakoñczyæ program?", "Grafika WinGL", MB_YESNOCANCEL) == IDYES) 
//   DestroyWindow(okno); // zniszczenie okna glownego i wyjscie z programu  
//   return 0;  
//  }
//  case WM_SIZE: // ustawienie rozmiaru okna
//  {
//   int szer = LOWORD(lParam); // nowa szerokosc okna
//   int wys = HIWORD(lParam); // nowa wysokosc okna
//   HDC grafika = GetDC(okno); // kontekst wyswietlania grafiki w oknie 
//   Dopasowanie(grafika, szer, wys);
//   ReleaseDC(okno,grafika);
//   return 0;
//  }
//  case WM_PAINT: // rysowanie zawartosci okna
//  {
//   RECT obszar; // obszar rysowania okna
//   GetClientRect(okno,&obszar);
//   {
//    HDC grafika; // kontekst wyswietlania grafiki w oknie 
//    PAINTSTRUCT rysunek; // struktura rysunku
//    grafika = BeginPaint(okno, &rysunek); 
//    Wyswietlanie(grafika,obszar.right,obszar.bottom);
//    EndPaint(okno, &rysunek); 
//   }
//   return 0; 
//  }
//  default: // domyslna obsluga komunikatu
//  {
//   return DefWindowProc(okno, komunikat, wParam, lParam);
//  }
// }
//}
//
// funkcja programu do wykonywania obliczen:
//BOOL funProg(HINSTANCE prog)
//{
// return FALSE;
//}
//
// procedura rozpoczecia przetwarzania grafiki GL w oknie Win32:
//void Rozpoczecie(HWND okno)
//{
// HDC grafika = GetDC(okno); // tymczasowe pobranie kontekstu wyswietlania grafiki w podanym oknie
// { // ustawienie formatu pikseli w grafice
//  static PIXELFORMATDESCRIPTOR opisFormatu =
//  {
//   sizeof(PIXELFORMATDESCRIPTOR),
//   1,
//   PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL, // rysowanie grafiki bedzie w oknie ze wsparciem dla OpenGL oraz bez podwojnego buforowania obrazu
//   PFD_TYPE_RGBA, // kolory grafiki definiowane beda przez podanie skladnikow R,G,B i parametru alfa
//   32,
//   0, 0, 0, 0, 0, 0,
//   0,
//   0,
//   0,
//   0, 0, 0, 0,
//   16,
//   0,
//   0,
//   PFD_MAIN_PLANE,
//   0,
//   0, 0, 0
//  };
//  int format = ChoosePixelFormat(grafika, &opisFormatu);
//  SetPixelFormat(grafika, format, &opisFormatu);
// }
// scena = wglCreateContext(grafika); // utworzenie globalnego kontekstu renderowania sceny
// wglMakeCurrent(grafika,scena); // wyroznienie kontekstu sceny z grafika 3D (GL) w kontekscie grafiki 2D (GDI)
// ReleaseDC(okno,grafika); // zwolnienie kontekstu grafiki podanego okna
// { // wstepna konfiguracja srodowiska GL 
//  glShadeModel(GL_SMOOTH); // wybranie trybu cieniowania poprzez interpolacje kolorow na powierzchni figury
// }
//}
//
// procedura zakonczenia przetwarzania grafiki GL w oknie Win32:
//void Zakonczenie(HWND okno)
//{
// HDC grafika = GetDC(okno); // tymczasowe pobranie kontekstu wyswietlania grafiki w podanym oknie
// wglMakeCurrent(grafika,NULL); // anulowanie kontekstu sceny w kontekscie grafiki
// wglDeleteContext(scena); // usuniecie kontekstu renderowania sceny
// scena = NULL;
// ReleaseDC(okno,grafika); // zwolnienie kontekstu grafiki podanego okna
//}
//
// procedura dopasowanie grafiki GL po zmianie rozmiaru okna: 
//void Dopasowanie(HDC graf, int szer, int wys)
//{
// if (szer<=0 || wys<=0) return; 
//
// glViewport(0, 0, szer, wys); // ustawienie portu widokowego bedacego obszarem p/n rzutowania sceny w obrebie okna
//
// glMatrixMode(GL_PROJECTION); // wyroznienie transformacji rzutowania sceny 3D na plaszczyzne widokowej
// glLoadIdentity(); // przypisanie macierzy jednostkowej do p/w transformacji
// 
// gluPerspective(60.0, (double)szer/wys, 1.0, 1000.0); // przypisanie macierzy perspektywy do p/w transformacji
//
// glMatrixMode(GL_MODELVIEW); // wyroznienie transformacji modelu i/lub widoku w scenie 3D
//}
//
// procedura wyswietlania grafiki GL w oknie: 
//void Wyswietlanie(HDC graf, int szer, int wys)
//{
// if (szer<=0 || wys<=0) return; 
//
// glClearColor(0.2f, 0.2f, 0.2f, 1.0f); // ustawienie koloru RGBA dla tla sceny
// glClear(GL_COLOR_BUFFER_BIT); // poczatkowe wpisanie koloru tla do bufora z kolorami pikseli na obrazie sceny
// { // wielokrotne resetowanie transformacji model-widok i ponowne renderowanie sceny wraz z wyswietlaniem wynikowej grafiki GL w tym samym oknie Win32 
//  double aspekt = (double)szer/wys;
//  glLoadIdentity(); 
//  Renderowanie1(aspekt); 
//  glLoadIdentity(); 
//  Renderowanie2(aspekt); 
//  glLoadIdentity(); 
//  Renderowanie3(aspekt); 
// }
//}
//
// procedury odtworzenia modelu 3D sceny w postaci obrazu 2D z uzyciem grafiki GL: 
//void Renderowanie1(double asp)
//{
// if (asp>1) asp=1; // ustawienie widoku sceny poprzez przypisanie macierzy obserwacji
// gluLookAt(0.0, 0.0, 5.0/asp, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); 
//
// Modelowanie(); // modelowanie zawartosci graficznej w scenie
//
// if (MessageBox(NULL, "Czy wyœwietliæ grafikê?", "Grafika WinGL", MB_YESNO) == IDYES) 
// glFlush(); // utworzenie na podstawie modelu i jego widoku obrazu sceny i natychmiastowe wyswietlenie go na ekranie
//}
//void Renderowanie2(double asp)
//{
// if (asp>1) asp=1; // ustawienie widoku sceny poprzez przypisanie macierzy obserwacji
// gluLookAt(0.0, 0.0, 5.0/asp, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0); 
//
// Modelowanie(); // modelowanie zawartosci graficznej w scenie
//
// if (MessageBox(NULL, "Czy wyœwietliæ grafikê?", "Grafika WinGL", MB_YESNO) == IDYES) 
// glFlush(); // utworzenie na podstawie modelu i jego widoku obrazu sceny i natychmiastowe wyswietlenie go na ekranie
//}
//void Renderowanie3(double asp)
//{
// if (asp>1) asp=1; // ustawienie widoku sceny poprzez przypisanie macierzy obserwacji
// gluLookAt(0.0, 0.0, 5.0/asp, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0); 
//
// Modelowanie(); // modelowanie zawartosci graficznej w scenie
//
// if (MessageBox(NULL, "Czy wyœwietliæ grafikê?", "Grafika WinGL", MB_YESNO) == IDYES) 
// glFlush(); // utworzenie na podstawie modelu i jego widoku obrazu sceny i natychmiastowe wyswietlenie go na ekranie
//}
//
// procedura podstawowego modelowania graficznego w scenie: 
//void Modelowanie(void)
//{
// glBegin(GL_TRIANGLES); // poczatek definicji modelu graficznego
// { // podanie kolorow RGB i wspolrzednych XYZ punktow (werteksow) dla kolejnych rogow rysowanego trojkata w scenie 3D
//  glColor3f(  1.0f,  0.0f, 0.0f);
//  glVertex3f(-1.0f, -1.0f, 0.0f);
//  glColor3f(  0.0f,  1.0f, 0.0f);
//  glVertex3f( 1.0f, -1.0f, 0.0f);
//  glColor3f(  0.0f,  0.0f, 1.0f);
//  glVertex3f( 0.0f,  1.0f, 0.0f);
// }
// glEnd(); // koniec definicji modelu graficznego
//}
//
