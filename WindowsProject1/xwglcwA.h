/****************************************************************************************/
/* Zbior klas dla podstawowych jednostek aplikacji C++ GUI-3D z uzyciem  Win32 - OpenGL */     
/****************************************************************************************/

// dodatkowa konfiguracja dla kompilatora MSVC
#pragma warning(disable:4244)
#pragma warning(disable:4996 4005)
#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#ifdef UNICODE
# undef UNICODE
# define _MBCS
#endif

//////////// pliki naglowkowe i biblioteczne

// pliki biblioteki standardowej jezyka C/C++:
#include <cstring>
#include <ctime>
#include <cmath>
#include <cassert>
#include <cstdlib>
#include <map>
#include <list>
#include <algorithm>
#include <typeinfo>
#include <stdexcept>
#define M_2PI (2.0*M_PI)
using namespace std;

// plik naglowkowy z deklaracjami elementow interfejsu Win32 API:
#include <windows.h>
#define MK_LEFT -1
#define MK_RIGHT -2

// pliki naglowkowe z deklaracjami elementow biblioteki OpenGL:
#include <gl\gl.h>
#include <gl\glu.h>

//////////// lista klas w kodzie zrodlowym
class Jednostka;
class Podzespol;
class ProgBaza;
class FormBaza;
class ScenBaza;
class OknoBaza;
class Aplikacja;

//////////// klasa jednostki funkcjonalnej aplikacji
class Jednostka { // zbiorcza
 private:
  Jednostka(const Jednostka &);
  void operator=(const Jednostka &);
 protected:
// zainicjowanie obiektu jednostki:
  Jednostka()
  {}
  
// sfinalizowanie obiektu jednostki:
  virtual ~Jednostka()
  {}

// podstawowe dane dla jednostki Win32: 
  static HINSTANCE UchwytAplikacji();
  static int TrybAplikacji();
  
 public:
// nadrzedny kontener z jednostka: 
  static const Aplikacja &Kontener();
};

//////////// klasa podzespolu jednostek aplikacji
class Podzespol : public virtual Jednostka { // zbiorcza
 protected:
  list<Jednostka*> komponenty_;
 protected:
// zainicjowanie obiektu podzespolu:
  Podzespol();

// sfinalizowanie obiektu podzespolu:
  ~Podzespol();
  
// wstawienie/usuniecie skladnikow podzespolu:
  void Sklad(const Jednostka &komponent, bool usun);

 public:
// podstawowe dane podzespolu Win32: 
  int Licznosc() const { return komponenty_.size(); }
};

//////////// klasa glownego kodu sterujacego aplikacji
class Aplikacja : public Podzespol { // finalna
  friend class Jednostka;
  HINSTANCE uchwyt_;
  int tryb_;
  OknoBaza *panel_;
  ScenBaza *widok_;
  ProgBaza *model_;
  static Aplikacja *obiekt_;
 public:
// zainicjowanie obiektu aplikacji:
  Aplikacja(HINSTANCE uchywt, int tryb);
  
// sfinalizowanie obiektu aplikacji:
  ~Aplikacja();
  
// wstawienie/usuniecie skladnikow aplikacji:
  void Sklad(ProgBaza &model, bool usun=false);
  void Sklad(ScenBaza &widok, bool usun=false);
  void Sklad(OknoBaza &panel, bool usun=false);
  
// uruchomienie przebiegu aplikacji:
  virtual int Bieg();

// podstawowe dane aplikacji Win32: 
  static HINSTANCE Uchwyt() { assert(obiekt_!=NULL); return obiekt_->uchwyt_; }
  static int Tryb() { assert(obiekt_!=NULL); return obiekt_->tryb_; }

// podstawowe jednostki architektury MVP: 
  static const ProgBaza &Model() { assert(obiekt_!=NULL && obiekt_->model_!=NULL); return *obiekt_->model_; }
  static const ScenBaza &Widok() { assert(obiekt_!=NULL && obiekt_->widok_!=NULL); return *obiekt_->widok_; }
  static const OknoBaza &Panel() { assert(obiekt_!=NULL && obiekt_->panel_!=NULL); return *obiekt_->panel_; }
};

//////////// klasa programu obliczeniowego aplikacji
class ProgBaza : public Jednostka { // abstrakcyjna
 protected:
  Aplikacja *modul_;
 public:
// zainicjowanie obiektu programu:
  explicit ProgBaza(Aplikacja &modul);

// sfinalizowanie obiektu programu:
  ~ProgBaza();
       
// wykonanie programu w petli:
  virtual bool Postep();

// nadrzedny modul z programem: 
  const Aplikacja &Modul() const { return *this->modul_; }

// procedura wykonywania obliczen w programie:
 protected:
  virtual bool Obliczanie() = 0;
};

//////////// klasa formy graficznej aplikacji
class FormBaza : public virtual Jednostka { // abstrakcyjna
 protected:
  char klasa_[256];
  int szerokosc_, wysokosc_;
  HWND uchwyt_;
  HWND zaczep_;
  static FormBaza *biezacy_;
  static map<HWND,FormBaza*> obiekty_;
  static FormBaza *obiekt_(HWND okno);
  static LRESULT CALLBACK funkcja_(HWND, UINT, WPARAM, LPARAM);  
 public:
// zainicjowanie obiektu formy:
  FormBaza(int szerokosc, int wysokosc);

// sfinalizowanie obiektu formy:
  ~FormBaza();

// utworzenie formy na ekranie:
  virtual void Pokaz();

// podstawowe dane formy Win32: 
  const char *Klasa() const { return this->klasa_; }
  HWND Uchwyt() const { return this->uchwyt_; }
  HWND Zaczep() const { return this->zaczep_; }
  int Szerokosc() const { return this->szerokosc_; }
  int Wysokosc() const { return this->wysokosc_; }

// procedury oblugi zdarzen w formie:
 protected:
  virtual void Utworzenie(HINSTANCE aplik, HWND okno) {}
  virtual void Zniszczenie(HINSTANCE aplik, HWND okno) {}
  virtual bool Obsluzenie(HWND okno, UINT kom, WPARAM wpar, LPARAM lpar) { return true; }  
  virtual void Dopasowanie(HDC kont, int szer, int wys) {}
  virtual void Wyswietlanie(HDC kont, int szer, int wys) = 0;
  virtual bool Wykonanie(HWND okno, UINT polec, HWND nadaw) { return false; }
  virtual bool Dzialanie(HWND okno, INT klaw, int kol, int wier) { return false; }
};

//////////// klasa okna sterowania aplikacji
class OknoBaza : public FormBaza , public Podzespol { // finalna
 protected:
  Jednostka *rama_;
  list<FormBaza*> ramki_;
  char tytul_[256];
  int barwa_;
 public:
// zainicjowanie obiektu okna:
  OknoBaza(Jednostka &rama, const char *tytul, int szerokosc=-1, int wysokosc=-1, int barwa=-1);

// sfinalizowanie obiektu okna:
  ~OknoBaza();

// wstawienie/usuniecie skladnikow okna:
  private: void sklad(FormBaza &ramka, bool usun); public:
  void Sklad(OknoBaza &panel, bool usun=false);
  void Sklad(ScenBaza &widok, bool usun=false);

// utworzenie okna na ekranie:
  void Pokaz();

// nadrzedna rama z oknem jako aplikacja lub inne okno: 
  const Jednostka &Rama() const { return *this->rama_; }
       
// podstawowe dane okna Win32: 
  const char *Tytul() const { return this->tytul_; }
  int Barwa() const { return this->barwa_; }

// jednostki aplikacji powiazane z oknem: 
  list<FormBaza*>::const_iterator Ramki() const { return ramki_.begin(); }

// procedury oblugi zdarzen w oknie:
 protected:
  void Wyswietlanie(HDC kont, int szer, int wys) {}
  bool Wykonanie(HWND okno, UINT polec, HWND nadaw) { return Sterowanie(okno,polec,nadaw); }
  virtual bool Sterowanie(HWND okno, UINT polec, HWND nadaw) = 0;
};

//////////// klasa sceny renderowania aplikacji
class ScenBaza : public FormBaza { // abstrakcyjna
 protected:
  Podzespol *pulpit_;
  int kolumna_, wiersz_;
  HGLRC osrodek_;
 public:
// zainicjowanie obiektu sceny:
  ScenBaza(Podzespol &pulpit, int kolumna, int wiersz, int szerokosc=-1, int wysokosc=-1);

// sfinalizowanie obiektu sceny:
  ~ScenBaza();

// utworzenie sceny na ekranie:
  void Pokaz();

// podstawowe dane sceny GL: 
  HGLRC Osrodek() const { return this->osrodek_; }
  int Kolumna() const { return this->kolumna_; }
  int Wiersz() const { return this->wiersz_; }

// nadrzedny pulpit ze scena: 
  const Podzespol &Pulpit() const { return *this->pulpit_; }

// procedury oblugi zdarzen w scenie:
 protected:
  void Utworzenie(HINSTANCE aplik, HWND okno);
  void Zniszczenie(HINSTANCE aplik, HWND okno);
  void Dopasowanie(HDC kont, int szer, int wys);
  void Wyswietlanie(HDC kont, int szer, int wys);
  virtual void Inicjowanie(HDC kont) {}
  virtual void Finalizowanie(HDC kont) {}
  virtual void Projektowanie(int szer, int wys) {}
  virtual void Renderowanie(double asp) = 0;
};

//////////// definicje metod i danych klasy jednostki

HINSTANCE Jednostka::UchwytAplikacji()
{
 return Aplikacja::Uchwyt();
}

int Jednostka::TrybAplikacji()
{
 return Aplikacja::Tryb();
}

const Aplikacja &Jednostka::Kontener() 
{ 
 assert(Aplikacja::obiekt_!=NULL); return *Aplikacja::obiekt_; 
}

//////////// definicje metod i danych klasy podzespolu

Podzespol::Podzespol()
{}

Podzespol::~Podzespol()
{
 komponenty_.clear();
}
  
void Podzespol::Sklad(const Jednostka &komp, bool usun)
{
 Jednostka &komponent = const_cast<Jednostka&>(komp);
 if (!usun) {
  assert(find(komponenty_.begin(),komponenty_.end(),&komponent)==komponenty_.end());
  komponenty_.push_back(&komponent);
 } else {
  assert(find(komponenty_.begin(),komponenty_.end(),&komponent)!=komponenty_.end());
  komponenty_.remove(&komponent);
 } 
}

//////////// definicje metod i danych klasy aplikacji

Aplikacja *Aplikacja::obiekt_ = NULL;

Aplikacja::Aplikacja(HINSTANCE uchwyt, int tryb): 
 uchwyt_(uchwyt), tryb_(tryb), 
 model_(NULL), widok_(NULL), panel_(NULL)
{
 assert(obiekt_==NULL);
 obiekt_ = this;
}

Aplikacja::~Aplikacja()
{
 obiekt_ = NULL;
 assert(panel_==NULL);
 assert(widok_==NULL);
 assert(model_==NULL);
 uchwyt_ = NULL;
}

void Aplikacja::Sklad(ProgBaza &model, bool usun)
{
 assert(&model.Modul()==this);
 if (!usun) { 
  assert(model_==NULL);
  model_=&model;
 } else {
  assert(&model==model_);
  model_=NULL;
 }
 Podzespol::Sklad(model,usun); 
}

void Aplikacja::Sklad(OknoBaza &panel, bool usun)
{
 assert(&panel.Rama()==this);
 if (!usun) { 
  assert(panel_==NULL);
  panel_=&panel;
 } else {
  assert(&panel==panel_);
  panel_=NULL;
 }
 Podzespol::Sklad(panel,usun); 
}

void Aplikacja::Sklad(ScenBaza &widok, bool usun)
{
 assert(&widok.Pulpit()==this);
 if (!usun) { 
  assert(widok_==NULL);
  widok_=&widok;
 } else {
  assert(&widok==widok_);
  widok_=NULL;
 }
 Podzespol::Sklad(widok,usun); 
}

int Aplikacja::Bieg()
{
 FormBaza *widok = widok_!=NULL ? (FormBaza*)widok_ : (FormBaza*)panel_;
 assert(widok!=NULL);
 widok->Pokaz();
 UpdateWindow(widok->Uchwyt());
 if (model_==NULL) 
 {
  MSG komunikat;
  
  while (GetMessage(&komunikat, NULL, 0, 0)) 
  {
   TranslateMessage(&komunikat); 
   DispatchMessage(&komunikat); 
  }
  
  return (int)komunikat.wParam;
 } 
 else 
 {
  MSG komunikat;
  
  while (TRUE)
  {
   if (PeekMessage(&komunikat, 0, 0, 0, PM_REMOVE)) 
   {
    if (komunikat.message == WM_QUIT) break; 
    TranslateMessage(&komunikat);
    DispatchMessage(&komunikat);
   }
   else 
   {
    bool zmiana = model_->Postep();
    if (zmiana) InvalidateRect(widok->Uchwyt(),NULL,FALSE);
   }
  }
  
  return (int)komunikat.wParam;
 } 
}

//////////// definicje metod i danych klasy programu

ProgBaza::ProgBaza(Aplikacja &modul):
 modul_(&modul)
{
 modul_->Sklad(*this);
}

ProgBaza::~ProgBaza()
{
 if (modul_!=NULL) modul_->Sklad(*this,true);
 modul_ = NULL;
}

bool ProgBaza::Postep()
{
 return this->Obliczanie();
}

//////////// definicje metod i danych klasy formy

FormBaza *FormBaza::biezacy_ = NULL;
map<HWND,FormBaza*> FormBaza::obiekty_;

FormBaza::FormBaza(int szerokosc, int wysokosc):
 szerokosc_(szerokosc<0 ? CW_USEDEFAULT : szerokosc), 
 wysokosc_(wysokosc<0 ? CW_USEDEFAULT : wysokosc)
{
 strncpy(klasa_,"",256);
 uchwyt_ = NULL;
 zaczep_ = NULL;
}

FormBaza::~FormBaza()
{
 obiekty_[uchwyt_] = NULL;
 zaczep_ = NULL;
 uchwyt_ = NULL;
}

void FormBaza::Pokaz()
{
 if (uchwyt_!=NULL) ShowWindow(uchwyt_,TrybAplikacji());
}

FormBaza *FormBaza::obiekt_(HWND okno)
{
 if (okno==NULL) return NULL;
 if (biezacy_!=NULL) {
  obiekty_[okno] = biezacy_;
  biezacy_ = NULL;
 }
 return obiekty_[okno]; 
}

LRESULT CALLBACK FormBaza::funkcja_(HWND okno, UINT komunikat, WPARAM wParam, LPARAM lParam)
{
 FormBaza *this_ = obiekt_(okno);
 switch (komunikat)
 {
  case WM_CREATE:
  {
   HINSTANCE aplikacja = UchwytAplikacji();
   assert(aplikacja==(HINSTANCE)GetWindowLong(okno, GWL_HINSTANCE));
   this_->Utworzenie(aplikacja,okno);
   return 0;
  }
  case WM_SIZE:
  {
   HDC kontekst = GetDC(okno);
   RECT obszar;
   GetWindowRect(okno,&obszar);
   this_->szerokosc_ = obszar.right-obszar.left;
   this_->wysokosc_ = obszar.bottom-obszar.top;
   this_->Dopasowanie(kontekst,LOWORD(lParam),HIWORD(lParam));
   ReleaseDC(okno,kontekst);
   return 0;
  }
  case WM_CLOSE:
  {
   DestroyWindow(okno);
   return 0;
  }
  case WM_CHAR:
  {
   if ((INT)wParam == VK_ESCAPE) DestroyWindow(okno); 
   return 0;
  }
  case WM_DESTROY:
  {
   HINSTANCE aplikacja = UchwytAplikacji();
   assert(aplikacja==(HINSTANCE)GetWindowLong(okno, GWL_HINSTANCE));
   this_->Zniszczenie(aplikacja,okno);
   if (this_->Zaczep()==NULL) PostQuitMessage(0);
   return 0;
  }
  case WM_PAINT:
  {
   HDC kontekst;
   PAINTSTRUCT zarys;
   RECT obszar;
   GetClientRect(okno,&obszar);
   kontekst = BeginPaint(okno,&zarys);
   this_->Wyswietlanie(kontekst,obszar.right,obszar.bottom);
   EndPaint(okno,&zarys);
   return 0;
  }
  case WM_COMMAND:
  {
   UINT polecenie = LOWORD(wParam);
   HWND nadawca = (HWND)lParam;
   bool zmiana = this_->Wykonanie(okno,polecenie,nadawca);
   if (zmiana) InvalidateRect(okno,NULL,FALSE);
   return 0;
  }
  case WM_VSCROLL:
  {
   UINT polecenie = LOWORD(wParam);
   HWND nadawca = (HWND)lParam;
   int pozycja = GetScrollPos(nadawca,SB_CTL);
   int wiersz = HIWORD(wParam);
   switch (polecenie) 
   { 
    case SB_PAGEUP: pozycja -= 5; break; 
    case SB_PAGEDOWN: pozycja += 5; break; 
    case SB_LINEUP: pozycja -= 1; break; 
    case SB_LINEDOWN: pozycja += 1; break; 
    case SB_THUMBTRACK: pozycja = wiersz; break; 
   } 
   SetScrollPos(nadawca,SB_CTL,pozycja,FALSE);
   polecenie =  GetWindowLong(nadawca,GWL_ID);
   bool zmiana = this_->Wykonanie(okno,polecenie,nadawca);
   if (zmiana) InvalidateRect(okno,NULL,FALSE);
   return 0;
  }
  case WM_HSCROLL:
  {
   UINT polecenie = LOWORD(wParam);
   HWND nadawca = (HWND)lParam;
   int pozycja = GetScrollPos(nadawca,SB_CTL);
   int wiersz = HIWORD(wParam);
   switch (polecenie) 
   { 
    case SB_PAGELEFT: pozycja -= 5; break; 
    case SB_PAGERIGHT: pozycja += 5; break; 
    case SB_LINELEFT: pozycja -= 1; break; 
    case SB_LINERIGHT: pozycja += 1; break; 
    case SB_THUMBTRACK: pozycja = wiersz; break; 
   } 
   SetScrollPos(nadawca,SB_CTL,pozycja,FALSE);
   polecenie =  GetWindowLong(nadawca,GWL_ID);
   bool zmiana = this_->Wykonanie(okno,polecenie,nadawca);
   if (zmiana) InvalidateRect(okno,NULL,FALSE);
   return 0;
  }
  case WM_KEYDOWN: 
  {
   INT klawisz = (INT)wParam; 
   POINT kursor;
   GetCursorPos(&kursor); 
   ScreenToClient(okno,&kursor); 
   bool zmiana = this_->Dzialanie(okno,klawisz,kursor.x,kursor.y);
   if (zmiana) InvalidateRect(okno,NULL,FALSE); 
   return 0; 
  }
  case WM_LBUTTONDOWN:
  {
   bool zmiana = this_->Dzialanie(okno,MK_LEFT,LOWORD(lParam),HIWORD(lParam));
   if (zmiana) InvalidateRect(okno,NULL,FALSE); 
   return 0; 
  }
  case WM_RBUTTONDOWN:
  {
   bool zmiana = this_->Dzialanie(okno,MK_RIGHT,LOWORD(lParam),HIWORD(lParam));
   if (zmiana) InvalidateRect(okno,NULL,FALSE); 
   return 0; 
  }
  default: 
  {
   bool przekaz = this_->Obsluzenie(okno,komunikat,wParam,lParam);
   if (przekaz) return DefWindowProc(okno,komunikat,wParam,lParam);
   return 0;
  }
 }
}

//////////// definicje metod i danych klasy panelu

OknoBaza::OknoBaza(Jednostka &rama, const char *tytul, int szerokosc, int wysokosc, int barwa):
 FormBaza(szerokosc,wysokosc),
 rama_(&rama),
 barwa_(barwa<0 ? COLOR_WINDOW : barwa)
{
 strncpy(tytul_,tytul,256);
 Aplikacja *modul = dynamic_cast<Aplikacja*>(rama_);
 if (modul!=NULL) modul->Sklad(*this);
 else {
  OknoBaza *panel = dynamic_cast<OknoBaza*>(rama_);
  if (panel!=NULL) panel->Sklad(*this); 
  else throw runtime_error("B³¹d kompozycji obiektów w aplikacji !");
 }
}

OknoBaza::~OknoBaza()
{
 ramki_.clear();
 Aplikacja *modul = dynamic_cast<Aplikacja*>(rama_);
 if (modul!=NULL) modul->Sklad(*this,true);
 else {
  OknoBaza *panel = dynamic_cast<OknoBaza*>(rama_);
  if (panel!=NULL) panel->Sklad(*this,true); 
 }
 rama_ = NULL;
}

void OknoBaza::sklad(FormBaza &ramka, bool usun)
{
 if (!usun) {
  assert(find(ramki_.begin(),ramki_.end(),&ramka)==ramki_.end());
  ramki_.push_back(&ramka);
 } else {
  assert(find(ramki_.begin(),ramki_.end(),&ramka)!=ramki_.end());
  ramki_.remove(&ramka);
 } 
 Podzespol::Sklad(ramka,usun);
}

void OknoBaza::Sklad(OknoBaza &panel, bool usun)
{
 assert(&panel.Rama()==this);
 sklad(panel,usun);
}

void OknoBaza::Sklad(ScenBaza &widok, bool usun)
{
 assert(&widok.Pulpit()==this);
 sklad(widok,usun);
}

void OknoBaza::Pokaz()
{ 
 if (uchwyt_==NULL) {
  if (strlen(klasa_)==0) {
   strncpy(klasa_,typeid(*this).name(),256);
   WNDCLASS info = {0};
   if (!GetClassInfo(UchwytAplikacji(),klasa_,&info)) {
    WNDCLASS rekord;
    rekord.style = CS_HREDRAW | CS_VREDRAW; 
    rekord.hInstance = UchwytAplikacji();  
    rekord.lpszClassName = klasa_;
    rekord.lpfnWndProc = (WNDPROC)&funkcja_;  
    rekord.hIcon = LoadIcon(NULL,IDI_APPLICATION); 
    rekord.hCursor = LoadCursor(NULL,IDC_ARROW); 
    rekord.lpszMenuName = NULL; 
    rekord.cbClsExtra = 0; 
    rekord.cbWndExtra = 0; 
    rekord.hbrBackground = barwa_==COLOR_APPWORKSPACE ? NULL : (HBRUSH)(barwa_+1); 
    if (!RegisterClass(&rekord)) throw runtime_error("B³¹d rejestracji klasy Win32 !");
    assert(GetClassInfo(UchwytAplikacji(),klasa_,&info));
   }
  }
  OknoBaza *panel = dynamic_cast<OknoBaza*>(rama_);
  if (panel!=NULL) zaczep_ = panel->Uchwyt();
  assert(biezacy_==NULL);
  biezacy_ = this;
  uchwyt_ = CreateWindowEx(0,klasa_,tytul_,WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME,CW_USEDEFAULT,CW_USEDEFAULT,szerokosc_,wysokosc_,zaczep_,NULL,UchwytAplikacji(),NULL);
  assert(biezacy_==NULL && obiekty_[uchwyt_]==this);
 }
 FormBaza::Pokaz(); 
 for (list<FormBaza*>::iterator ramka=ramki_.begin(); ramka!=ramki_.end(); ramka++)
 (*ramka)->Pokaz(); 
}

//////////// definicje metod i danych klasy sceny

ScenBaza::ScenBaza(Podzespol &pulpit, int kolumna, int wiersz, int szerokosc, int wysokosc):
 FormBaza(szerokosc,wysokosc),
 pulpit_(&pulpit),
 kolumna_(kolumna<0 ? 0 : kolumna), 
 wiersz_(wiersz<0 ? 0 : wiersz)
{
 osrodek_ = NULL;
 Aplikacja *modul = dynamic_cast<Aplikacja*>(pulpit_);
 if (modul!=NULL) modul->Sklad(*this);
 else {
  OknoBaza *panel = dynamic_cast<OknoBaza*>(pulpit_);
  if (panel!=NULL) panel->Sklad(*this); 
  else throw runtime_error("B³¹d kompozycji obiektów w aplikacji !");
 }
}

ScenBaza::~ScenBaza()
{
 Aplikacja *modul = dynamic_cast<Aplikacja*>(pulpit_);
 if (modul!=NULL) modul->Sklad(*this,true);
 else {
  OknoBaza *panel = dynamic_cast<OknoBaza*>(pulpit_);
  if (panel!=NULL) panel->Sklad(*this,true); 
 }
 pulpit_ = NULL;
 osrodek_ = NULL;
}

void ScenBaza::Pokaz()
{ 
 if (uchwyt_==NULL) {
  if (strlen(klasa_)==0) {
   strncpy(klasa_,typeid(*this).name(),256);
   WNDCLASS info = {0};
   if (!GetClassInfo(UchwytAplikacji(),klasa_,&info)) {
    WNDCLASS rekord;
    rekord.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; 
    rekord.hInstance = UchwytAplikacji();  
    rekord.lpszClassName = klasa_;
    rekord.lpfnWndProc = (WNDPROC)&funkcja_;  
    rekord.hIcon = LoadIcon(NULL,IDI_APPLICATION); 
    rekord.hCursor = LoadCursor(NULL,IDC_ARROW); 
    rekord.lpszMenuName = NULL; 
    rekord.cbClsExtra = 0; 
    rekord.cbWndExtra = 0; 
    rekord.hbrBackground = NULL; 
    if (!RegisterClass(&rekord)) throw runtime_error("B³¹d rejestracji klasy Win32 !");
    assert(GetClassInfo(UchwytAplikacji(),klasa_,&info));
   }
  }
  OknoBaza *panel = dynamic_cast<OknoBaza*>(pulpit_);
  if (panel!=NULL) zaczep_ = panel->Uchwyt();
  assert(biezacy_==NULL);
  biezacy_ = this;
  if (zaczep_!=NULL) uchwyt_ = CreateWindowEx(0,klasa_,NULL,WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,kolumna_,wiersz_,szerokosc_,wysokosc_,zaczep_,NULL,UchwytAplikacji(),NULL);
  else {
   DEVMODE ekran = { 0 };
   int szer = szerokosc_+2*kolumna_;
   int wys = wysokosc_+2*wiersz_;
   ekran.dmSize = sizeof(DEVMODE);
   ekran.dmPelsWidth = szer;
   ekran.dmPelsHeight = wys;
   ekran.dmBitsPerPel = 16;
   ekran.dmDisplayFrequency = 75;
   ekran.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL | DM_DISPLAYFREQUENCY;
   ChangeDisplaySettings(&ekran, CDS_FULLSCREEN);
   uchwyt_ = CreateWindowEx(0,klasa_,NULL,WS_POPUP,0,0,szer,wys,NULL,NULL,UchwytAplikacji(),NULL);
  }
  assert(biezacy_==NULL && obiekty_[uchwyt_]==this);
 }
 FormBaza::Pokaz(); 
}

void ScenBaza::Utworzenie(HINSTANCE aplik, HWND okno)
{
 HDC kontekst = GetDC(okno); 
 {
  static PIXELFORMATDESCRIPTOR opisFormatu =
  {
   sizeof(PIXELFORMATDESCRIPTOR),
   1,
   PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, 
   PFD_TYPE_RGBA, 
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
   0, 0, 0
  };
  int format = ChoosePixelFormat(kontekst, &opisFormatu);
  SetPixelFormat(kontekst, format, &opisFormatu);
 }
 osrodek_ = wglCreateContext(kontekst); 
 wglMakeCurrent(kontekst,osrodek_); 
 Inicjowanie(kontekst);
 ReleaseDC(okno,kontekst); 
}

void ScenBaza::Zniszczenie(HINSTANCE aplik, HWND okno)
{
 HDC kontekst = GetDC(okno); 
 wglMakeCurrent(kontekst,osrodek_); 
 Finalizowanie(kontekst);
 wglMakeCurrent(kontekst,NULL); 
 wglDeleteContext(osrodek_); 
 ReleaseDC(okno,kontekst); 
}

void ScenBaza::Dopasowanie(HDC kont, int szer, int wys)
{
 if (szer<=0 || wys<=0) return; 
 wglMakeCurrent(kont,osrodek_); 
 glViewport(0, 0, szer, wys); 
 Projektowanie(szer,wys); 
}

void ScenBaza::Wyswietlanie(HDC kont, int szer, int wys)
{
 if (szer<=0 || wys<=0) return; 
 wglMakeCurrent(kont,osrodek_); 
 Renderowanie((double)szer/wys); 
 SwapBuffers(kont); 
}
