///**************************************************************/
///* Zlozona graficzna aplikacja C/C++ z uzyciem Win32 - OpenGL */     
///**************************************************************/
//
//// dodatkowa konfiguracja dla kompilatora MSVC
//#pragma warning(disable:4244)
//#pragma warning(disable:4996 4005)
//#define _CRT_SECURE_NO_WARNINGS
//#define _USE_MATH_DEFINES
//#ifdef UNICODE
//# undef UNICODE
//# define _MBCS
//#endif
//
//// pliki naglowkowe biblioteki standardowej jezyka ISO-C99 lub ISO-C++98:
//#ifndef __cplusplus
//#include <math.h>
//#include <stdlib.h>
//#include <stdbool.h>
//#else // zamiast dla jezyka C wykonaj rownowazna kompilacje dla jezyka C++
//#include <cmath>
//#include <cstdlib>
//using namespace std;
//#endif
//#define M_2PI (2.0*M_PI)
//
//// plik naglowkowy z deklaracjami elementow interfejsu Win32 API:
//#include <windows.h>
//
//// pliki naglowkowe z deklaracjami elementow biblioteki OpenGL:
//#include <gl\gl.h>
//#include <gl\glu.h>
//
//// opis klasy, okna i ekranu:
//const char nazwaKlasy[] = "OknoSceny";
//const char tytulOkna[] = "Symulacje w WinGL";
//const bool pelnyEkran = false;
//
//// deklaracja funkcji okna do obslugi zdarzen:
//LRESULT CALLBACK funOkna(HWND okno, UINT komunikat, WPARAM wParam, LPARAM lParam);
//
//// deklaracja funkcji programu do wykonywania obliczen:
//bool funProg(HINSTANCE prog);
//
//// glowny kod programu:
//int WINAPI WinMain(HINSTANCE prog, HINSTANCE _, LPSTR __, int trybOkna)
//{
// WNDCLASS klasa; // klasa okna
// HWND okno; // okno glowne
//
// // utworzenie i rejestrowanie nowej klasy okna:
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
// // utworzenie, wyswietlanie i aktualizacja okna glownego:
// if (!pelnyEkran) okno = CreateWindowEx(0, nazwaKlasy, tytulOkna, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, prog, NULL);
// else {
//  DEVMODE ekran = { 0 }; // tryb i rozmiar ekranu
//  const int szerEkranu = 800;
//  const int wysEkranu = 600;
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
// // petla glowna programu do obslugi zdarzen oraz wykonywania obliczen:
// {
//  MSG komunikat; // komunikat dot. zdarzenia w systemie okienkowym
//  while (true)
//  {
//   if (PeekMessage(&komunikat, 0, 0, 0, PM_REMOVE)) // obsluga zdarzen
//   {
//    if (komunikat.message == WM_QUIT) break; // przerwanie petli
//    TranslateMessage(&komunikat);
//    DispatchMessage(&komunikat);
//   }
//   else // obliczenia programu
//   {
//    bool zmiana = funProg(prog);
//    if (zmiana) InvalidateRect(okno,NULL,FALSE); // odswiezenie zawartosci okna glownego
//   }
//  }
//
//  // zakonczenie programu:
//  return (int)komunikat.wParam;
// }
//}
//
//// deklaracja globalnych danych i procedur sceny do przetwarzania grafiki:
//static HGLRC scena = NULL; // kontekst sceny z grafika GL
//static struct { double X,Y,Z, A,B,C, U,V,W; } widok = {0.0}; // parametry widoku sceny
//static struct { double alfa,beta,gama; } ruch = {0.0}; // parametry ruchu w scenie
//static void Rozpoczecie(HWND okno);
//static void Zakonczenie(HWND okno);
//static void Dopasowanie(HDC graf, int szer, int wys);
//static void Wyswietlanie(HDC graf, int szer, int wys);
//static void Renderowanie(double asp);
//static void Oswietlanie();
//static void Obserwowanie(double asp);
//static void Modelowanie();
//static bool Oddzialywanie(double pozm, double pion, double pros, double kret);
//static bool Obliczenia();
//
//// funkcja okna do obslugi zdarzen:
//LRESULT CALLBACK funOkna(HWND okno, UINT komunikat, WPARAM wParam, LPARAM lParam)
//{
// static int kursX = -1, kursY = -1; // biezace polozenie kursora myszki
// switch (komunikat)
// {
//  case WM_CREATE: // utworzenie okna
//  {
//   Rozpoczecie(okno);
//   { // ukrycie kursora myszki i zapamietanie jego polozenia w oknie
//    POINT kurs;
//    ShowCursor(FALSE);
//    GetCursorPos(&kurs); // pobranie globalnego polozenia kursora wzgledem ekranu     
//    ScreenToClient(okno,&kurs); // obliczenie polozenia lokalnego wzgledem okna
//    kursX = kurs.x; kursY = kurs.y;
//   }
//   return 0;
//  }
//  case WM_DESTROY: // zniszczenie okna
//  {
//   ShowCursor(TRUE);
//   Zakonczenie(okno);
//   PostQuitMessage(0); // wyjscie z glownej petli programu  
//   return 0;
//  }
//  case WM_CLOSE: // zamkniecia okna
//  {
//   ShowCursor(TRUE);
//   if (MessageBox(okno, "Czy zakoñczyæ program?", "Grafika WinGL", MB_YESNOCANCEL) == IDYES) 
//    DestroyWindow(okno); // zniszczenie okna glownego i wyjscie z programu  
//   else ShowCursor(FALSE);
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
//  case WM_MOUSEMOVE: // przesuwanie myszki w oknie
//  {
//   POINT kurs = {LOWORD(lParam),HIWORD(lParam)}; // loklane polozenie kursora myszki
//   int szer, wys; // rozmiary obszaru okna
//   {
//    RECT obszar;
//    GetClientRect(okno,&obszar); 
//    szer = obszar.right; wys = obszar.bottom;
//   }
//   if (szer>0 && wys>0 && kurs.x>=0 && kurs.x<szer && kurs.y>=0 && kurs.y<wys) {
//    bool zmiana = Oddzialywanie(1.0*(kurs.x-kursX)/szer,1.0*(kurs.y-kursY)/wys,0.0,0.0);
//    if (zmiana) InvalidateRect(okno,NULL,FALSE); // odswiezenie zawartosci okna
//   }
//   kursX = kurs.x; kursY = kurs.y; // poprzednie polozenie kursora
//   return 0; 
//  }
//  case WM_KEYDOWN: // nacisniecie klawiatury w oknie
//  {
//   int klawisz = (int)(wParam); // kod biezacego klawisza klawiatury
//   int wert = 0; // zwrot klawisza ze strzalka wertykalna
//   int horz = 0; // zwrot klawisza ze strzalka horyzontalna
//   if (klawisz==VK_UP) wert = +1;
//   else if (klawisz==VK_DOWN) wert = -1;
//   else if (klawisz==VK_RIGHT) horz = +1;
//   else if (klawisz==VK_LEFT) horz = -1;
//   {
//    bool zmiana = Oddzialywanie(0.0,0.0,0.1*wert,0.01*horz);
//    if (zmiana) InvalidateRect(okno,NULL,FALSE); // odswiezenie zawartosci okna
//   }
//   return 0; 
//  }
//  case WM_CHAR: // wprowadzenie znaku do okna
//  {
//   if ((int)wParam == VK_ESCAPE) DestroyWindow(okno); // zniszczenie okna glownego i wyjscie z programu
//   return 0;
//  }
//  default: // domyslna obsluga komunikatu
//  {
//   return DefWindowProc(okno, komunikat, wParam, lParam);
//  }
// }
//}
//
//// funkcja programu do wykonywania obliczen:
//bool funProg(HINSTANCE prog)
//{
// return Obliczenia();
//}
//
//// procedura rozpoczecia przetwarzania grafiki GL w oknie Win32:
//void Rozpoczecie(HWND okno)
//{
// HDC grafika = GetDC(okno); // tymczasowe pobranie kontekstu wyswietlania grafiki w podanym oknie
// { // ustawienie formatu pikseli w grafice
//  static PIXELFORMATDESCRIPTOR opisFormatu =
//  {
//   sizeof(PIXELFORMATDESCRIPTOR),
//   1,
//   PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, // rysowanie grafiki bedzie w oknie ze wsparciem dla OpenGL oraz z podwojnym buforowaniem obrazu
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
//  glEnable(GL_DEPTH_TEST); // wlaczenie bufora do testowania glebi w renderowaniu sceny 3D
//  glShadeModel(GL_SMOOTH); // wybranie trybu cieniowania poprzez interpolacje kolorow
//  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); // wybranie sposobu reprezentowania wielokata w postaci pelnej figury
//  glEnable(GL_LIGHTING); // wlaczenie oswietlenia w secnie
//  glEnable(GL_LIGHT0);  // wybor zrodla swiatla numer zero
//  glEnable(GL_LIGHT1);  // wybor zrodla swiatla numer jeden
//  { // uwspolnienie konfiguracji obydwu zrodel
//   GLenum Q[] = { GL_AMBIENT, GL_DIFFUSE, GL_SPECULAR, GL_POSITION, GL_SPOT_DIRECTION, GL_SPOT_EXPONENT, GL_SPOT_CUTOFF, GL_CONSTANT_ATTENUATION, GL_LINEAR_ATTENUATION, GL_QUADRATIC_ATTENUATION };
//   int n = (sizeof Q)/sizeof(GLenum);
//   GLfloat X[10];
//   for (int i=0; i<n; i++) {
//    glGetLightfv(GL_LIGHT0,Q[i],X);
//    glLightfv(GL_LIGHT1,Q[i],X);
//   }
//  }
//  glEnable(GL_NORMALIZE); // normalizacja wektorow normalnych do pow. figur niezbedna w algorytmie obliczania oswietlenia
//  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,TRUE); // wlaczenie obliczania oswietlenia dla obydwu stron pow. figur
//  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,TRUE); // wlaczenie realistycznej metody obliczania odbic swiatla na pow. figur
//  glEnable(GL_COLOR_MATERIAL); // ustawianie koloru materialu na pow. figury w alg. oswietlenia odbywa sie na podstawie jawnie podanych kolorow werteksow
//  glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE); // wybor skladowych otoczeniowej i dyfuzyjnej swiatla do jednoczesnego ustawiania jawnego koloru materialu jedynie dla przedniej strony pow. figur
//  glHint(GL_POINT_SMOOTH, GL_NICEST); // wybor najlepszego algorytmu antyaliasingu
//  glHint(GL_LINE_SMOOTH, GL_NICEST);
//  glHint(GL_POLYGON_SMOOTH, GL_NICEST);
//  glEnable(GL_LINE_SMOOTH); // wlaczenie antyaliasingu dla linii
//  glEnable(GL_POLYGON_SMOOTH); // wlaczenie antyaliasingu dla wielokata
// }
// Oddzialywanie(0,0,0,0); // ustawienie stanu poczatkowego widoku sceny
//}
//
//// procedura zakonczenia przetwarzania grafiki GL w oknie Win32:
//void Zakonczenie(HWND okno)
//{
// HDC grafika = GetDC(okno); // tymczasowe pobranie kontekstu wyswietlania grafiki w podanym oknie
// wglMakeCurrent(grafika,NULL); // anulowanie kontekstu sceny w kontekscie grafiki
// wglDeleteContext(scena); // usuniecie kontekstu renderowania sceny
// scena = NULL;
// ReleaseDC(okno,grafika); // zwolnienie kontekstu grafiki podanego okna
//}
//
//// procedura dopasowanie grafiki GL po zmianie rozmiaru okna: 
//void Dopasowanie(HDC graf, int szer, int wys)
//{
// if (szer<=0 || wys<=0) return; 
//
// glViewport(0, 0, szer, wys); // ustawienie portu widokowego bedacego obszarem p/n rzutowania sceny w obrebie okna
//
// glMatrixMode(GL_PROJECTION); // wyroznienie transformacji rzutowania sceny 3D na plaszczyzne widokowej
// glLoadIdentity(); // przypisanie macierzy jednostkowej do p/w transformacji
// 
// gluPerspective(60.0, (double)szer/wys, 1.5, 1000.0); // przypisanie macierzy perspektywy do p/w transformacji
//
// glMatrixMode(GL_MODELVIEW); // wyroznienie transformacji modelu i/lub widoku w scenie 3D
//}
//
//// procedura wyswietlania grafiki GL w oknie: 
//void Wyswietlanie(HDC graf, int szer, int wys)
//{
// if (szer<=0 || wys<=0) return; 
//
// { // ustawienie koloru RGBA dla poswiaty od tla sceny powstajacej na powierzchni figur  
//  GLfloat poswiata[] = {0.4f, 0.4f, 0.4f, 1.0f};
//  glLightModelfv(GL_LIGHT_MODEL_AMBIENT,poswiata);
// } 
// glClearColor(0.5f, 0.3f, 0.8f, 1.0f); // ustawienie osobnego koloru RGBA dla tla sceny
// glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // poczatkowe wpisanie koloru tla do bufora z kolorami pikseli na obrazie sceny oraz wyzerowanie bufora glebi
//
// glLoadIdentity(); // przypisanie macierzy jednostkowej do p/w transformacji model-widok
// Renderowanie((double)szer/wys); // ponowne renderowanie sceny po odswiezeniu zawartosci okna o biezacym aspekcie geometrycznym
// SwapBuffers(graf); // po utworzeniu obrazu sceny na tylnim buforze zamiana buforow obrazu miejscami
//}
//
//// procedura odtworzenia modelu sceny w postaci grafiki GL: 
//void Renderowanie(double asp)
//{
// Oswietlanie(); // oswietlanie modelu sceny
// Obserwowanie(asp); // obserwowanie modelu sceny
// Modelowanie(); // modelowanie zawartosci graficznej w scenie
//
// glFlush(); // wykonanie renderingu na podstawie p/w opisu sceny i natychmiastowe utworzenie koncowego obrazu sceny
//}
//
//// pomocnicza procedura z definicja tylnego zrodla swiatla: 
//static void SwiatloTylne_()
//{
// glPushMatrix(); // zapamietanie globalnej macierzy dla p/w transformacji model-widok
// {
//  GLfloat zrodlo[] = {0.0f, 0.0f, 50.0f, 1.0f}; // zrodlo swiatla, (x,y,z,w): w=1 - swiatlo pozycyjne, w=0 - swiatlo kierunkowe
//  GLfloat strumien[] = {0.0, 0.0, -1.0}; // strumien swiatla pozycyjnego, (x,y,z)
//  glTranslatef(0.0f, +2.5f, 0.0f); // zmiana p/w pozycji i kierunku zrodla swiatla 
//  glRotatef(+10.0f, 0.0, 1.0, 0.0);
//  glRotatef(-15.0f, 1.0, 0.0, 0.0);  
//  glLightfv(GL_LIGHT0, GL_POSITION, zrodlo); // ustawienie zrodla swiatla
//  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, strumien); // kierunek strumienia swiatla
//  glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 90.0);// polowa kata rozwarcia stozka strumienia swiatla
//  glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 100.0);// stopien skoncentrowania rozkladu swiatla w stozku strumieniu
// }
// glPopMatrix(); // zresetowanie p/w transformacji model-widok do macierzy globalnej
//}
//
//// pomocnicza procedura z definicja przedniego zrodla swiatla: 
//static void SwiatloPrzednie_()
//{
// glPushMatrix(); // zapamietanie globalnej macierzy dla p/w transformacji model-widok
// {
//  GLfloat zrodlo[] = {0.0f, 0.0f, -40.0f, 1.0f}; // zrodlo swiatla, (x,y,z,w): w=1 - swiatlo pozycyjne, w=0 - swiatlo kierunkowe
//  GLfloat strumien[] = {0.0, 0.0, 1.0}; // strumien swiatla pozycyjnego, (x,y,z)
//  glTranslatef(0.0f, +1.5f, 0.0f); // zmiana p/w pozycji i kierunku zrodla swiatla 
//  glRotatef(+20.0f, 0.0, 1.0, 0.0);
//  glRotatef(+35.0f, 1.0, 0.0, 0.0);  
//  glLightfv(GL_LIGHT1, GL_POSITION, zrodlo); // ustawienie zrodla swiatla
//  glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, strumien); // kierunek strumienia swiatla
//  glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90.0);// polowa kata rozwarcia stozka strumienia swiatla
//  glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 10.0);// stopien skoncentrowania rozkladu swiatla w stozku strumieniu
// }
// glPopMatrix(); // zresetowanie p/w transformacji model-widok do macierzy globalnej
//}
//
//// procedura ustawienia zrodel i materialu w oswietleniu sceny: 
//void Oswietlanie()
//{
// SwiatloTylne_(); // zdefiniowanie zrodel swiatla
// SwiatloPrzednie_();
// { // zdefiniowanie wlasciwosci materialnych
//  GLfloat material[] = {0.5f, 0.5f, 0.5f, 1.0f}; // wlasciwy kolor materialu na pow. figur widoczny w oswietleniu
//  GLfloat matowka[] = {0.2f, 0.2f, 0.2f, 1.0f}; // dodatkowy kolor materialu dla powierzchni matowej figur
//  GLfloat odblask[] = {0.7f, 0.7f, 0.7f, 1.0f}; // dodatkowy kolor materialu dla powierzchni odblaskowej figur
//  glMaterialf(GL_BACK, GL_SHININESS, 10.0); // ustawienie stopnia polysku materialu na pow. figur powstajacego w wyniku oswietlenia 
//  glMaterialf(GL_FRONT, GL_SHININESS, 128.0);
//  glMaterialfv(GL_BACK, GL_DIFFUSE, material); // ustawienie wspolnego koloru materialu w swietle dyfuzyjnym jedynie dla tylnej strony pow. wszystkich figur
//  glMaterialfv(GL_BACK, GL_SPECULAR, matowka); // ustawienie wspolnego koloru materialu w swiatle odbitym od tylnej strony pow. wszystkich figur 
//  glMaterialfv(GL_FRONT, GL_SPECULAR, odblask); // ustawienie wspolnego koloru materialu w swiatle odbitym od przedniej strony pow. wszystkich figur 
// }
//}
//
//// procedura ustawienia widoku w obserwacji sceny: 
//void Obserwowanie(double asp)
//{
// if (asp>1) asp=1;
// gluLookAt( // przypisanie macierzy obserwacji do p/w transformacji
//  widok.X/asp, widok.Y/asp, widok.Z/asp, // polozenie miejsca obserwacji
//  widok.A, widok.B, widok.C, // punkt skupienia obserwacji - srodek sceny
//  widok.U, widok.V, widok.W // kierunek pionu obserwacji
// );
//}
//
//// pomocnicza procedura z definicja figury graficznej w postaci kwadratu 
//static void DefKwadrat_(double Ho, int Nx, int Ny, double Zo)
//{
// register double dx = 2*Ho/Nx;
// register double dy = 2*Ho/Ny;
// glNormal3d(0.0,0.0,1.0);
// for (int i=0; i<Nx; i++) {
//  register double x = -Ho+i*dx;
//  register double xx = x+dx;
//  for (int j=0; j<Ny; j++) {
//   register double y = -Ho+j*dy;
//   register double yy = y+dy;
//   glBegin(GL_QUADS);
//   {
//    glVertex3d(xx, y, Zo);
//    glVertex3d(xx, yy, Zo);
//    glVertex3d(x, yy, Zo);
//    glVertex3d(x, y, Zo);
//   }
//   glEnd();
//  }
// }
//}
//
//// pomocnicza procedura z definicja figury graficznej w postaci szescianu 
//static void DefSzescian_(double Ho, int Na, int Nb)
//{
// glPushMatrix(); // zapamietanie lokalnej macierzy dla p/w transformacji model-widok
// for (int i=0; i<6; i++) {
//  DefKwadrat_(Ho,Na,Nb,Ho); // definicja figury w postaci gornej sciany szescianu
//  if (i%2==0) glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // pomocniczy obrot p/w sciany wokol OX lub OY tak aby otrzymac pozostale sciany w szescianie
//  else glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
// } 
// glPopMatrix(); // zresetowanie p/w transformacji model-widok do macierzy lokalnej
//}
//
//// pomocnicza procedura z definicja figury graficznej w postaci torusa 
//static void DefTorus_(double Ro, double ro, int Ns, int Nt)
//{
// for (int i=0; i<Ns; i++) { // petla po malym kole torusa
//  glBegin(GL_QUAD_STRIP); // figura wokol osi OZ i srodka sceny w postaci zbioru polaczonych czworokatow zlozonych z werteksow podawanych parami
//  { // kolejnosc werteksow w czworokacie okresla kolejnosc werteksow w drugiej parze
//   register double cos_s, sin_s, cos_t, sin_t;
//   register double x, y, z, u, v, w;
//   for (int j=0; j<=Nt; j++) { // petla po duzym kole torusa
//    int t = j%Nt;
//    for (int k=1; k>=0; k--) { // petla po werteksach biezacej pary
//     int s = (i+k)%Ns;
//     cos_s = cos(s*M_2PI/Ns);
//     sin_s = sin(s*M_2PI/Ns);
//     cos_t = cos(t*M_2PI/Nt);
//     sin_t = sin(t*M_2PI/Nt);
//     x = (Ro+ro*cos_s)*cos_t;
//     y = (Ro+ro*cos_s)*sin_t;
//     z = ro*sin_s;
//     u = cos_s*cos_t;
//     v = cos_s*sin_t;
//     w = sin_s;
//     glNormal3d(u,v,w);
//     glVertex3d(x,y,z);
//    }
//   }
//  }
//  glEnd();
// }
//}
//
//// pomocnicza procedura z definicja modelu graficznego ruchomych kol Eulera 
//static void ModelKola_()
//{
// GLUquadricObj* kwadryka = gluNewQuadric(); 
// glPushMatrix(); 
// { 
//  glTranslatef(0.0f, 2.5f, 0.0f); 
//  glRotated(ruch.gama*180.0/M_PI, 0.0, 1.0, 0.0);
//  glPushMatrix();
//  {  
//   glTranslatef(0.0f, -2.8f, 0.0f);
//   glRotated(+90.0, 1.0, 0.0, 0.0);
//   gluCylinder(kwadryka,0.2,0.2,0.9,10,10);
//  }  
//  glPopMatrix(); 
//  DefTorus_(3.0,0.25,40,90); 
//  glRotated(ruch.beta*180.0/M_PI, 1.0, 0.0, 0.0);
//  glPushMatrix();
//  {  
//   glTranslatef(+2.3f, 0.0f, 0.0f);
//   glRotated(+90.0, 0.0, 1.0, 0.0);
//   gluCylinder(kwadryka,0.2,0.2,0.9,10,10);
//   glRotated(-90.0, 0.0, 1.0, 0.0);
//   glTranslatef(-4.6f, 0.0f, 0.0f);
//   glRotated(-90.0, 0.0, 1.0, 0.0);
//   gluCylinder(kwadryka,0.2,0.2,0.9,10,10);
//  }  
//  glPopMatrix(); 
//  DefTorus_(2.5,0.25,40,90); 
//  glRotated(ruch.alfa*180.0/M_PI, 0.0, 1.0, 0.0);
//  glPushMatrix();
//  {  
//   glTranslatef(0.0f, +1.8f, 0.0f);
//   glRotated(-90.0, 1.0, 0.0, 0.0);
//   gluCylinder(kwadryka,0.2,0.2,0.9,10,10);
//   glRotated(+90.0, 1.0, 0.0, 0.0);
//   glTranslatef(0.0f, -3.6f, 0.0f);
//   glRotated(+90.0, 1.0, 0.0, 0.0);
//   gluCylinder(kwadryka,0.2,0.2,0.9,10,10);
//  }  
//  glPopMatrix(); 
//  DefTorus_(2.0,0.25,40,90); 
// }
// glPopMatrix(); 
// gluDeleteQuadric(kwadryka); 
//}
//
//// pomocnicza procedura z definicja modelu graficznego stalego cokolu 
//static void ModelCokol_()
//{
// glPushMatrix(); // zapamietanie globalnej macierzy dla p/w transformacji model-widok
// { 
//  glTranslatef(0.0f, -3.001f, 0.0f); // dolaczenie do p/w transformacji macierzy skalowania i przesuniecia dla p/n figury
//  glScalef(4.0f,2.0f,4.0f);
//  DefSzescian_(1.0,150,150); // definiowanie figury w srodku sceny
// }
// glPopMatrix(); // zresetowanie p/w transformacji model-widok do macierzy globalnej
//}
//
//// pomocnicza procedura z definicja modelu graficznego stalej podlogi 
//static void ModelPodloga_()
//{
// glPushMatrix(); // zapamietanie globalnej macierzy dla p/w transformacji model-widok
// { 
//  glTranslatef(0.0f, -5.0f, 0.0f); // dolaczenie do p/w transformacji macierzy obrotu i przesuniecia dla p/n figury
//  glRotated(-90.0, 1.0, 0.0, 0.0);
//  DefKwadrat_(25.0,50,50,0.0); // definiowanie figury w srodku sceny
// }
// glPopMatrix(); // zresetowanie p/w transformacji model-widok do macierzy globalnej
//}
//
//// procedura podstawowego modelowania graficznego w scenie: 
//void Modelowanie()
//{ // ustawnienie koloru dla materialu na pow. p/n modelu widocznego w swietle dyfuzyjnym i zdefiniowanie modelu
// glColor3f(1.0f, 0.8f, 0.1f);
// ModelKola_();
// glColor3f(0.6f, 0.6f, 0.9f);
// ModelCokol_();
// glColor3f(0.1f, 1.0f, 0.8f);
// ModelPodloga_();
//}
//
//// procedura oddzialywania uzytkownika na widok sceny: 
//bool Oddzialywanie(double pozm, double pion, double pros, double kret)
//{
// if (fabs(pozm)>0.5 || fabs(pion)>0.5 || fabs(pros)>50.0 || fabs(kret)>0.5) return false;
// const double Po = M_PI; // stale i biezace parametry stanu dla widoku sceny
// const double Qo = M_2PI;
// const double Ro = 12.0;
// const double So = M_2PI;
// static double tetp = 0.40, fp = 0.00, prmp = 1.00, psp = 0.25;
// tetp += pion; if (tetp<0.001) tetp = 0.001; else if (tetp>0.999) tetp = 0.999;
// fp += pozm; fp = fmod(fp,1.0);
// prmp += pros; if (prmp<0.01) prmp = 0.01; else if (prmp>100.0) prmp = 100.0;
// psp += kret; psp = fmod(psp,1.0);
// double teta = tetp*Po;
// double fi = fp*Qo;
// double prom = prmp*Ro;
// double psi = psp*So;
// widok.Z = prom*sin(teta)*cos(fi);
// widok.X = prom*sin(teta)*sin(fi);
// widok.Y = prom*cos(teta);
// widok.U = cos(psi);
// widok.V = sin(psi);
// widok.W = widok.A = widok.B = widok.C = 0.0;
// return true; // wyswietlenie grafiki GL w oknie
//}
//
//// procedura iteracyjnych obliczen dla modelu graficznego:
//static double _losuj_(int no) // pomocnicza funkcja do nieskorelowanego generowania liczb pseudolosowych 
//{
// while (--no>0) rand();
// return 1.0*rand()/(RAND_MAX+1);
//} 
//inline double losuj_() 
//{ return _losuj_(10); }
//bool Obliczenia() // kod symulacji w oparciu o rownanie Eulera z losowa sila periodyczna i numeryczna sila dyfuzyji
//{
// static double tbieg = 0.0;
// static double tfaza1 = 0.0;
// static double tfaza2 = 0.0;
// static double tfaza3 = 0.0;
// static double omega1 = 0.0;
// static double omega2 = 0.0;
// static double omega3 = 0.0;
// const double sila = 2.5;
// const double opor = 0.04;
// const double delta = 0.03;
// const double okres = 50.0;
// 
// if (tbieg<=0.0) {
//  tfaza1 = losuj_()*okres;
//  tfaza2 = losuj_()*okres;
//  tfaza3 = losuj_()*okres;
// } 
// double tau1 = losuj_()*sin(M_2PI*(tbieg-tfaza1)/okres); // implementacja rownania ruchu
// double tau2 = losuj_()*sin(M_2PI*(tbieg-tfaza2)/okres);
// double tau3 = losuj_()*sin(M_2PI*(tbieg-tfaza3)/okres);
// omega1 += sila*tau1*delta-opor*omega1;
// omega2 += sila*tau2*delta-opor*omega2;
// omega3 += sila*tau3*delta-opor*omega3;
// tbieg += delta;
//
// ruch.alfa += omega1*delta; // nastepne katy w radianach dla obrotu figury w scenie 
// ruch.beta += omega2*delta;
// ruch.gama += omega3*delta;
//
// return true; // wyswietlenie grafiki GL w oknie
//}