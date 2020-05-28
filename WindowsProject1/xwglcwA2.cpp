///*********************************************************/
///* Aplikacja C++ ze sterowaniem grafiki w Win32 - OpenGL */     
///*********************************************************/
//
////////////// zbior klas 
//
//#include "xwglcwA.h"
//
////////////// kod glowny
//
//class MojPanel : public OknoBaza {
//  float kat_;
// public:
//  explicit MojPanel(Aplikacja &modul): OknoBaza(modul,"Aplikacja ze sterowaniem w Win32-OpenGL",700,500,2) 
//  {
//   kat_=0.0f;
//  }
//  float KatObrotu() const 
//  { return kat_; }
// protected:
//  void Utworzenie(HINSTANCE aplik, HWND okno) 
//  {
//   CreateWindowEx(0,"BUTTON",NULL,
//                  WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
//                  10,25,150,400,okno,
//                  NULL,aplik,NULL); 
//   CreateWindowEx(0,"BUTTON","Zamknij",
//                  WS_VISIBLE | WS_CHILD,
//                  40,370,90,30,okno,
//                  (HMENU)1000,aplik,NULL); 
//   HWND suwak = CreateWindowEx(0,"SCROLLBAR",NULL,
//                  WS_VISIBLE | WS_CHILD | SBS_VERT,
//                  65,50,40,300,okno,
//                  (HMENU)1001,aplik,NULL);
//   SetScrollRange(suwak,SB_CTL,0,360,FALSE);
//   SetScrollPos(suwak,SB_CTL,kat_,FALSE);
//  }
//  bool Sterowanie(HWND okno, UINT polec, HWND nadaw)
//  {
//   switch (polec)
//   {
//    case 1000: 
//    {
//     DestroyWindow(okno);
//     return true; 
//    }
//    case 1001: 
//    {
//     kat_ = GetScrollPos(nadaw,SB_CTL);
//     return true; 
//    }
//    default: return false;
//   } 
//  }
//};
//
//class MojWidok : public ScenBaza {
// public:
//  explicit MojWidok(MojPanel &panel): ScenBaza(panel,170,10,505,440) 
//  {}
// protected:
//  void Inicjowanie(HDC kont) 
//  {
//   glEnable(GL_DEPTH_TEST);
//   // ...ladowanie zasobow...
//  }
//  void Finalizowanie(HDC kont) 
//  {
//   // ...usuwanie zasobow...
//  }
//  void Projektowanie(int szer, int wys)
//  {
//   glMatrixMode(GL_PROJECTION);
//   glLoadIdentity();
//   gluPerspective(60.0f, (float)szer/(float)wys, 1.0f, 1000.0f);
//   glMatrixMode(GL_MODELVIEW);
//  }
//  void Renderowanie(double asp)
//  {
//   const MojPanel &panel = dynamic_cast<const MojPanel &>(Aplikacja::Panel());
//
//   glLoadIdentity();
//   gluLookAt(2.5f, 2.5f, 2.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
//   glRotatef(panel.KatObrotu(), 1.0f, 0.0f, 0.0f);
//
//   glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
//   glClearDepth(1.0);
//   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//   glColor3f(1.0f, 0.0f, 0.0f);
//   glBegin(GL_QUADS);
//   {
//    glVertex3f(-1.0f, -1.0f, 1.0f);
//    glVertex3f(1.0f, -1.0f, 1.0f);
//    glVertex3f(1.0f, 1.0f, 1.0f);
//    glVertex3f(-1.0f, 1.0f, 1.0f);
//   }
//   glEnd();
//
//   glColor3f(1.0f, 1.0f, 0.0f);
//   glBegin(GL_QUADS);
//   {
//    glVertex3f(1.0f, -1.0f, 1.0f);
//    glVertex3f(1.0f, -1.0f, -1.0f);
//    glVertex3f(1.0f, 1.0f, -1.0f);
//    glVertex3f(1.0f, 1.0f, 1.0f);
//   }
//   glEnd();
//
//   glColor3f(0.0f, 1.0f, 0.0f);
//   glBegin(GL_QUADS);
//   {
//    glVertex3f(-1.0f, -1.0f, -1.0f);
//    glVertex3f(1.0f, -1.0f, -1.0f);
//    glVertex3f(1.0f, 1.0f, -1.0f);
//    glVertex3f(-1.0f, 1.0f, -1.0f);
//   }
//   glEnd();
//
//   glColor3f(0.0f, 1.0f, 1.0f);
//   glBegin(GL_QUADS);
//   {
//    glVertex3f(-1.0f, -1.0f, 1.0f);
//    glVertex3f(-1.0f, -1.0f, -1.0f);
//    glVertex3f(-1.0f, 1.0f, -1.0f);
//    glVertex3f(-1.0f, 1.0f, 1.0f);
//   }
//   glEnd();
//
//   glColor3f(0.0f, 0.0f, 1.0f);
//   glBegin(GL_QUADS);
//   {
//    glVertex3f(-1.0f, 1.0f, 1.0f);
//    glVertex3f(1.0f, 1.0f, 1.0f);
//    glVertex3f(1.0f, 1.0f, -1.0f);
//    glVertex3f(-1.0f, 1.0f, -1.0f);
//   }
//   glEnd();
//
//   glColor3f(1.0f, 0.0f, 1.0f);
//   glBegin(GL_QUADS);
//   {
//    glVertex3f(-1.0f, -1.0f, 1.0f);
//    glVertex3f(1.0f, -1.0f, 1.0f);
//    glVertex3f(1.0f, -1.0f, -1.0f);
//    glVertex3f(-1.0f, -1.0f, -1.0f);
//   }
//   glEnd();
//   glFlush();
//  }
//};
//
//int WINAPI WinMain(HINSTANCE uchwyt, HINSTANCE, LPSTR, int tryb)
//{
// Aplikacja aplikacja(uchwyt,tryb);
// MojPanel panel(aplikacja);
// MojWidok widok(panel);
// return aplikacja.Bieg();
//}
