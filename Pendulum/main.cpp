//  Se reprezinta grafic animat o bara ce se roteste in jurul unui capat.



          //  Includerea headerelor utilizate de program
#include <windows.h>
#include <gl/gl.h>
#include <math.h>



          //  Functia "WinProc"
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message)
    {
      case WM_CREATE:
        return 0;
      case WM_CLOSE:
        PostQuitMessage( 0 );
        return 0;
      case WM_DESTROY:
        return 0;
      case WM_KEYDOWN:
        switch (wParam)
        {
          case VK_ESCAPE:
            PostQuitMessage( 0 );
            return 0;
        }
        return 0;
        default:
          return DefWindowProc(hWnd, message, wParam, lParam);
  }
}



          //  Functia "EnableOpenGL"
VOID EnableOpenGL( HWND hWnd, HDC * hDC, HGLRC * hRC )
{
  PIXELFORMATDESCRIPTOR pfd;
  int iFormat;

          //  Get the device context (DC)
  *hDC = GetDC( hWnd );

          //  Set the pixel format for the DC
  ZeroMemory( &pfd, sizeof( pfd ) );
  pfd.nSize = sizeof( pfd );
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DRAW_TO_WINDOW |
  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 24;
  pfd.cDepthBits = 16;
  pfd.iLayerType = PFD_MAIN_PLANE;
  iFormat = ChoosePixelFormat( *hDC, &pfd );
  SetPixelFormat( *hDC, iFormat, &pfd );

          //  Create and enable the render context (RC)
  *hRC = wglCreateContext( *hDC );
  wglMakeCurrent( *hDC, *hRC );
}



          //  Functia "DisableOpenGL"
VOID DisableOpenGL( HWND hWnd, HDC hDC, HGLRC hRC )
{
  wglMakeCurrent( NULL, NULL );
  wglDeleteContext( hRC );
  ReleaseDC( hWnd, hDC );
}



          //  Functia "line"
void line(float x1, float y1, float x2, float y2)
{
  glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
  glEnd();
}



          //  Functia "circle"
void circle(float x, float y, float radius)
{
  int n = 100;
  float teta;
  glBegin(GL_LINE_LOOP);
    for (int i=0; i<=n; i++)
    {
	  teta = i*6.283185f/n;
      glVertex2f(x + cos(teta)*radius, y + sin(teta)*radius);
    }
  glEnd();
}



          //  Functia "WinMain"
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int iCmdShow)
{
  WNDCLASS wc;
  HWND hWnd;
  HDC hDC;
  HGLRC hRC;
  MSG msg;
  BOOL bQuit = FALSE;
  int width = 800, height = 600;
  float widthscale, heightscale;
  float teta1 = -0.3f, teta2 = 0.3f, omega1 =.0f, omega2=.0f, dt = 0.01f, s1, c1, s2, c2, L = 1.0f, d = 1.0f;
  float J, S, A, B;
  float g=9.81, l=.5f, a=.1f, b=.2f, M=.5f, m=.1f, k=40.f, c=2.f;


          //  Determinarea factorilor de scara
  if (width < height)
  {
  	heightscale = (float)width/(float)height;
    widthscale = 1.0f;
  }
  else
  {
    widthscale = (float)height/(float)width;
  	heightscale = 1.0f;
  }
          //  Register window class
  wc.style = CS_OWNDC;
  wc.lpfnWndProc = WndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = hInstance;
  wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
  wc.hCursor = LoadCursor( NULL, IDC_ARROW );
  wc.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );
  wc.lpszMenuName = NULL;
  wc.lpszClassName = "GLSample";
  RegisterClass( &wc );

          //  Create main window
  hWnd = CreateWindow("GLSample", "OpenGL Sample",
  WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,
  0, 0, width, height, NULL, NULL, hInstance, NULL);

          //  Enable OpenGL for the window
  EnableOpenGL( hWnd, &hDC, &hRC );

          //  Program main loop
  while (!bQuit)
    {
          // Check for messages
      if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {

          //  Handle or dispatch messages
          if (msg.message == WM_QUIT)
            {
              bQuit = TRUE;
            }
          else
            {
              TranslateMessage(&msg);
              DispatchMessage(&msg);
            }
         }
      else
        {

          //  Inceputul zonei de instructiuni pentru animatia OpenGL

          glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
          glClear(GL_COLOR_BUFFER_BIT);
          glPushMatrix();
          glScalef(widthscale, heightscale, 1.0f);
          glRotatef(0.0f, 0.0f, 0.0f, 1.0f);



          //  Desenarea a doi penduli in rotatie
          s1 = sin(teta1);
          s2 = sin(teta2);
          c1 = cos(teta1);
          c2 = cos(teta2);

          // Rezolvare
		J=(M+(m/3))*(l*l);
		S=(M+(m/2))*g*l;
		A=k*a*a;
		B=c*b*b;
		/*omega1=omega1;
		omega2=omega2;
		s1=sin(teta1);
		s2=sin(teta2);
		c1=cos(teta1);
		c2=cos(teta2);*/

		omega1=omega1+((-S*s1+A*(s2-s1)*c1-B*omega1*(c1*c1))/J)*dt;
		omega2=omega2+((-S*s2-A*(s2-s1)*c2-B*omega2*(c2*c2))/J)*dt;
		teta1=teta1+omega1*dt;
		teta2=teta2+omega2*dt;
		/*(line(-d/2, L/2, -d/2+L*sin(teta1), L/2-L*cos(teta1));
		line(d/2,L/2,d/2+L*sin(teta2),L/2-cos(teta2));
		circle(-d/2+L*sin(teta1), L/2-L*cos(teta1), 0.1*L);
		circle(d/2+L*sin(teta2),L/2-L*cos(teta1),0.1*L);*/






          glLineWidth(4.0f);
          glColor3f(1.0f, 1.0f, 1.0f);

          line(-d/2., L/2., -d/2 + L*s1, L/2. - L*c1);
          line( d/2., L/2.,  d/2 + L*s2, L/2. - L*c2);
          circle(-d/2 + L*s1, L/2. - L*c1, .1*L);
          circle( d/2 + L*s2, L/2. - L*c2, .1*L);

          glPopMatrix();
          SwapBuffers( hDC );

        }
  }
          // Shutdown OpenGL
  DisableOpenGL( hWnd, hDC, hRC );

          // Destroy the window explicitly
  DestroyWindow( hWnd );
  return msg.wParam;
}
