///************************************************************************/
///* Prosta wielookienkowa aplikacja C++ z GUI w Win32 i grafika w OpenGL */     
///************************************************************************/
//
////////////// zbior klas 
//
//#include "xwglcwA.h"
//
////////////// kod glowny
//
//class MojModel : public ProgBaza {
//  int krok_;
//  float kat_;
// public:
//  explicit MojModel(Aplikacja &modul): ProgBaza(modul) 
//  {
//   krok_=0;
//   kat_=0.0f;
//  }
//  float KatObrotu() const 
//  { return kat_; }
// protected:
//  bool Obliczanie()
//  {
//   krok_++;
//   if (krok_<100000) return false;
//   else {
//    krok_=0;
//    kat_ += 1.0f; 
//    Sleep(20);
//    return true;
//   } 
//  }
//};
//
//class MojPanel : public OknoBaza {
// public:
//  explicit MojPanel(Aplikacja &modul): OknoBaza(modul,"Panel glowny aplikacji GUI z grafika w Win32-OpenGL",600,400,2) 
//  {}
// protected:
//  void Utworzenie(HINSTANCE aplik, HWND okno) 
//  {
//   CreateWindowEx(0,"BUTTON","Nowe",
//                  WS_VISIBLE | WS_CHILD,
//                  400,150,150,60,okno,
//                  (HMENU)1000,aplik,NULL); 
//  }
//  bool Sterowanie(HWND okno, UINT polec, HWND nadaw);
//};
//
//class MojeOkno : public OknoBaza {
// public:
//  explicit MojeOkno(MojPanel &panel): OknoBaza(panel,"Okno podrzedne z podgladem grafki",400,400) 
//  {}
// protected:
//  void Utworzenie(HINSTANCE aplik, HWND okno) 
//  {
//   CreateWindowEx(0,"BUTTON","Dalej",
//                  WS_VISIBLE | WS_CHILD,
//                  10,10,90,30,okno,
//                  (HMENU)1001,aplik,NULL); 
//  }
//  bool Sterowanie(HWND okno, UINT polec, HWND nadaw)
//  {
//   switch (polec)
//   {
//    case 1001: 
//    {
//     return true; 
//    }
//    default: return false;
//   } 
//  }
//};
//
//class MojWidok : public ScenBaza {
// public:
//  explicit MojWidok(Podzespol &pulpit): ScenBaza(pulpit,10,10,363,343) 
//  {}
// protected:
//  void Renderowanie(double asp)
//  {
//   const MojModel &model = dynamic_cast<const MojModel &>(Aplikacja::Model());
//   glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
//   glClear(GL_COLOR_BUFFER_BIT);
//   glLoadIdentity();
//   glRotatef(model.KatObrotu(), 0.0f, 0.0f, 1.0f);
//   glScalef(0.5f, 0.5f, 1.0f);
//   glBegin(GL_TRIANGLES);
//   {
//    glColor3f(  1.0f,  0.0f, 0.0f);
//    glVertex3f(-1.0f, -1.0f, 0.0f);
//    glColor3f(  0.0f,  1.0f, 0.0f);
//    glVertex3f( 1.0f, -1.0f, 0.0f);
//    glColor3f(  0.0f,  0.0f, 1.0f);
//    glVertex3f( 0.0f,  1.0f, 0.0f);
//   }
//   glEnd();
//   glFlush();
//  }
//};
//
//bool MojPanel::Sterowanie(HWND okno, UINT polec, HWND nadaw)
//{
// switch (polec)
// {
//  case 1000: 
//  {
//   MojeOkno *okno = new MojeOkno(*this);
//   MojWidok *wglad = new MojWidok(*okno);
//   okno->Pokaz();
//   return true; 
//  }
//  default: return false;
// } 
//}
//
//int WINAPI WinMain(HINSTANCE uchwyt, HINSTANCE, LPSTR, int tryb)
//{
// Aplikacja aplikacja(uchwyt,tryb);
// MojModel model(aplikacja);
// // MojWidok ekran(aplikacja);
// MojPanel panel(aplikacja);
// MojWidok widok(panel);
// MojeOkno okno(panel);
// MojWidok wglad(okno);
// return aplikacja.Bieg();
//}
///*
// Zadanie: Odkomentuj p/w definicje widoku pelnoekranowego
// */
//
