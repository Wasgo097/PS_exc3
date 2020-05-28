///*************************************/
///* Oswietlenie w scenie Win32-OpenGL */     
///*************************************/
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
//// pliki biblioteki standardowej jezyka C:
//#include <math.h>
//#include <stdlib.h>
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
//char nazwaKlasy[] = "OknoSceny";
//char tytulOkna[] = "Oswietlenie w WinGL";
//BOOL pelnyEkran = FALSE;
//
//// deklaracja funkcji okna do obslugi zdarzen:
//LRESULT CALLBACK funOkna(HWND okno, UINT komunikat, WPARAM wParam, LPARAM lParam);
//
//// deklaracja funkcji programu do wykonywania obliczen:
//BOOL funProg(HINSTANCE prog);
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
// // petla glowna programu do obslugi zdarzen oraz wykonywania obliczen:
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
//  // zakonczenie programu:
//  return (int)komunikat.wParam;
// }
//}
//
//// deklaracja globalnych danych i procedur sceny do przetwarzania grafiki:
//static HGLRC scena = NULL; // kontekst sceny z grafika GL
//static struct { double X,Y,Z, A,B,C, U,V,W; } widok = {0.0}; // parametry widoku sceny
//static void Rozpoczecie(HWND okno);
//static void Zakonczenie(HWND okno);
//static void Dopasowanie(HDC graf, int szer, int wys);
//static void Wyswietlanie(HDC graf, int szer, int wys);
//static void Renderowanie(double asp);
//static void Oswietlanie(void);
//static void Obserwowanie(double asp);
//static void Modelowanie(void);
//static BOOL Oddzialywanie(double pozm, double pion, double pros, double kret);
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
//   Oddzialywanie(0,0,0,0); // ustawienie stanu poczatkowego widoku sceny
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
//    BOOL zmiana = Oddzialywanie(1.0*(kurs.x-kursX)/szer,1.0*(kurs.y-kursY)/wys,0.0,0.0);
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
//    BOOL zmiana = Oddzialywanie(0.0,0.0,0.1*wert,0.01*horz);
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
//BOOL funProg(HINSTANCE prog)
//{
// return FALSE;
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
//
//  glShadeModel(GL_SMOOTH); // wybranie trybu cieniowania poprzez interpolacje kolorow na powierzchni figury
//  // glShadeModel(GL_FLAT); // wybranie trybu cieniowania poprzez powielanie koloru ostatniego werteksa na powierzchni figury
//  /*
//   Zadanie: zmien miejscami komentarz w p/w dwoch liniach, 
//            aby zmienic tryb cieniowania kolorow powstalych
//            na pow. figur w wyniku oswietlenia sceny
//  */
//
//  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); // wybranie sposobu reprezentowania wielokata w postaci pelnej figury
//  // glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); // wybranie sposobu reprezentowania wielokata w postaci siatki linii
//  /*
//   Zadanie: zmien miejscami komentarz w p/w dwoch liniach 
//            w celu zmiany sposobu reprezentowania figur w 
//            modelu sceny
//  */
//  
//  glEnable(GL_LIGHTING); // wlaczenie oswietlenia figur w scenie
//  glEnable(GL_LIGHT0);  // wybor zrodla swiatla numer zero
//
//  glEnable(GL_NORMALIZE); // normalizacja wektorow normalnych do pow. figur niezbedna w algorytmie obliczania oswietlenia
//  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,TRUE); // wlaczenie obliczania oswietlenia dla obydwu stron pow. figur a nie tylko przedniej
//  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,TRUE); // wlaczenie realistycznej metody obliczania odbic swiatla na pow. figur
//  /*
//   Zadanie: zakomentuj jedna z p/w linii w celu zmiany 
//            kodu w algorytmie obliczania oswietlenia
//   */
//   
//  glEnable(GL_COLOR_MATERIAL); // ustawianie koloru materialu na pow. figury w alg. oswietlenia odbywa sie na podstawie jawnie podanych kolorow werteksow
//  glColorMaterial(GL_FRONT,GL_DIFFUSE); // wybor skladowej dyfuzyjnej swiatla do jawnego ustawiania koloru materialu jedynie dla przedniej strony pow. figur
//  /*
//   Zadanie: zmien skladowa swiatla ze dyfuzyjnego GL_DIFFUSE 
//            na otoczenia GL_AMBIENT lub odbitego GL_SPECULAR
//   */
// }
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
// gluPerspective(60.0, (double)szer/wys, 1.0, 1000.0); // przypisanie macierzy perspektywy do p/w transformacji
//
// glMatrixMode(GL_MODELVIEW); // wyroznienie transformacji modelu i/lub widoku w scenie 3D
//}
//
//// procedura wyswietlania grafiki GL w oknie: 
//void Wyswietlanie(HDC graf, int szer, int wys)
//{
// if (szer<=0 || wys<=0) return; 
//
// { // ustawienie jednego koloru RGBA dla tla sceny i poswiaty tla powstajacej na powierzchni figur  
//  GLfloat poswiata[] = {0.2f, 0.5f, 0.2f, 1.0f};
//  // glLightModelfv(GL_LIGHT_MODEL_AMBIENT,poswiata);
//  /*
//   Zadanie: odkomentuj p/w operacje i zmien kolor poswiaty
//   */
//  glClearColor(poswiata[0], poswiata[1], poswiata[2], poswiata[3]);
// } 
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
// Modelowanie(); // wlasciwe modelowanie zawartosci graficznej w scenie
// /*
//  Zadanie: Przeniesc operacje oswietlenia za operacje obserwowania sceny
//  */
//
// glFlush(); // wykonanie renderingu na podstawie p/w opisu sceny i natychmiastowe utworzenie koncowego obrazu sceny
//}
//
//// procedura ustawienia zrodel i materialu w oswietleniu sceny: 
//void Oswietlanie(void)
//{
// glPushMatrix(); // zapamietanie globalnej macierzy dla p/w transformacji model-widok
// {
//  GLfloat zrodlo[] = {0.0f, 0.0f, 35.0f, 1.0f}; // zrodlo swiatla, (x,y,z,w): w=1 - swiatlo pozycyjne, w=0 - swiatlo kierunkowe
//  GLfloat strumien[] = {0.0, 0.0, -1.0}; // strumien swiatla pozycyjnego, (x,y,z)
//  GLfloat material[] = {0.5f, 0.5f, 0.5f, 1.0f}; // wlasciwy kolor materialu na pow. figur widoczny w oswietleniu
//  GLfloat matowka[] = {0.2f, 0.2f, 0.2f, 1.0f}; // dodatkowy kolor materialu dla powierzchni matowej figur
//  GLfloat odblask[] = {0.7f, 0.7f, 0.7f, 1.0f}; // dodatkowy kolor materialu dla powierzchni odblaskowej figur
//  glTranslatef(0.0f, +1.25f, 0.0f); // zmiana p/w pozycji i kierunku zrodla swiatla
//  glRotatef(-7.0f, 0.0, 1.0, 0.0);
//  glRotatef(+10.0f, 1.0, 0.0, 0.0);  
//  glLightfv(GL_LIGHT0, GL_POSITION, zrodlo); // ustawienie zrodla swiatla
//  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, strumien); // kierunek strumienia swiatla
//  glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 90.0);// polowa kata rozwarcia stozka strumienia swiatla
//  glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 128.0);// stopien skoncentrowania rozkladu swiatla w stozku strumieniu
//  glMaterialf(GL_BACK, GL_SHININESS, 10.0); // ustawienie stopnia polysku materialu na pow. figur powstajacego w wyniku oswietlenia 
//  glMaterialf(GL_FRONT, GL_SHININESS, 128.0);
//  glMaterialfv(GL_BACK, GL_DIFFUSE, material); // ustawienie wspolnego koloru materialu w swietle dyfuzyjnym jedynie dla tylnej strony pow. wszystkich figur
//  glMaterialfv(GL_BACK, GL_SPECULAR, matowka); // ustawienie wspolnego koloru materialu w swiatle odbitym od tylnej strony pow. wszystkich figur 
//  glMaterialfv(GL_FRONT, GL_SPECULAR, odblask); // ustawienie wspolnego koloru materialu w swiatle odbitym od przedniej strony pow. wszystkich figur 
//  /*
//   Zadanie: zmien parametry oswietlenia i materialu w p/w kodzie
//   */
// }
// glPopMatrix(); // zresetowanie p/w transformacji model-widok do macierzy globalnej
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
//// pomocnicza procedura z definicja figury graficznej w postaci zlozonego kwadratu o zadanej polowie boku, gestosci siatki werteksow oraz wysokosci  
//static void DefKwadrat_(double Ho, int Nx, int Ny, double Zo)
//{
// double dx = 2*Ho/Nx;
// double dy = 2*Ho/Ny;
// int i,j;
// glNormal3d(0.0,0.0,1.0); // normalny wektor do powierzchni figury niezbedny w oswietleniu sceny
// for (i=0; i<Nx; i++) {
//  double x = -Ho+i*dx;
//  double xx = x+dx;
//  for (j=0; j<Ny; j++) {
//   double y = -Ho+j*dy;
//   double yy = y+dy;
//   glBegin(GL_QUADS); // figura wokol osi OZ nad srodkiem sceny w postaci zbioru czworokatow zlozonych z 4 danych werteksow
//   { // kolejnosc werteksow zgodna z regula prawej sroby wzgledem kierunku p/w normalnej definiuje przednia strone figury
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
//// procedura podstawowego modelowania graficznego w scenie: 
//void Modelowanie(void)
//{
// glColor3f(1.0f, 0.9f, 0.1f); // ustawnienie koloru dla materialu na pow. p/n figur w swietle dyfuzyjnym
// glPushMatrix(); // zapamietanie globalnej macierzy dla p/w transformacji model-widok
// { // zdefiniowanie standardowych figur w srodku sceny i ich lokalne przemiszczanie
//  GLUquadricObj* kwadryka = gluNewQuadric(); // utworzenie rekordu dla figury w postaci uogolnionej kwadryki
//  glTranslatef(0.0f, 0.0f, 1.0f);  
//  gluSphere(kwadryka,1.5,100,100); // definicja zlozonej sfery o danym promieniu i gestosci siatki werteksow
//  glTranslatef(0.0f, 0.0f, -2.0f);  
//  gluCylinder(kwadryka,0.75,0.75,1.0,100,100); // definicja zlozonego walca o danych promieniach, wysokosci i gestosci siatki werteksow 
//  glTranslatef(0.0f, 0.0f, 0.01f);  
//  gluDisk(kwadryka,0.75,2.5,100,100); // definicja zlozonego dysku o danych promieniach i gestosci siatki werteksow
//  glTranslatef(0.0f, 0.0f, -0.02f);
//  glRotatef(180.0f, 1.0f, 0.0f, 0.0f);  
//  gluDisk(kwadryka,0.0,2.5,100,100); 
//  gluDeleteQuadric(kwadryka); // usuniecie rekordu standardowej kwadryki
// }
// glPopMatrix(); // zresetowanie p/w transformacji model-widok do macierzy globalnej
// glColor3f(0.8f, 0.8f, 1.0f); // ustawnienie koloru dla materialu na pow. p/n figur w swietle dyfuzyjnym
// DefKwadrat_(5.0,100,100,-1.0); // zdefiniowanie wlasnej figury pod srodkiem sceny
// /*
//  Zadanie: zmien parametry gestosci siatki 
//           werteksow w aproksymacji p/w figur
//  */
//}                                
//
//// procedura oddzialywania uzytkownika na widok sceny: 
//BOOL Oddzialywanie(double pozm, double pion, double pros, double kret)
//{
//#define Ro 12.0
// static double teta = M_PI_2, fi = 0.0, prom = Ro, psi = M_PI_2; // biezace parametry stanu dla widoku sceny
// teta += pion*M_PI; if (teta<0.0) teta = 0.0; else if (teta>M_PI) teta = M_PI;
// fi += pozm*M_2PI; fi = fmod(fi,M_2PI);
// prom += pros*Ro; if (prom<0.01*Ro) prom = 0.01*Ro; else if (prom>100.0*Ro) prom = 100.0*Ro;
// psi += kret*M_2PI; psi = fmod(psi,M_2PI);
// widok.Z = prom*sin(teta)*cos(fi);
// widok.X = prom*sin(teta)*sin(fi);
// widok.Y = prom*cos(teta);
// widok.U = cos(psi);
// widok.V = sin(psi);
// widok.W = widok.A = widok.B = widok.C = 0.0;
//#undef Ro
// return TRUE; // wyswietlenie grafiki GL w oknie
//}
