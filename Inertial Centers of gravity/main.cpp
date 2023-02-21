//  A bar rotating about one end is represented animatedly.



          //  Include the headers used by the program
#include <windows.h>
#include <gl/gl.h>
#include <math.h>
#include <stdio.h>



          //  Function "WinProc"
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



          //  Function "EnableOpenGL"
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



          //  Function "DisableOpenGL"
VOID DisableOpenGL( HWND hWnd, HDC hDC, HGLRC hRC )
{
  wglMakeCurrent( NULL, NULL );
  wglDeleteContext( hRC );
  ReleaseDC( hWnd, hDC );
}



          //  Function "line"
void line(float x1, float y1, float x2, float y2)
{
  glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
  glEnd();
}



          //  Function "circle"
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



          //  Function "WinMain"
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int iCmdShow)
{
  WNDCLASS wc;
  HWND hWnd;
  HDC hDC;
  HGLRC hRC;
  MSG msg;
  BOOL bQuit = FALSE;
  int width = 800, height = 600, i, n=25;
  float widthscale, heightscale;
  float teta = 0.0f, omega = 1.0f, dt = 0.0002f, l = 0.4f, xA, yA, xB, yB, xD, yD, lam, xI, yI, r=2*l, xC, yC, xD1, yD1, xD2, yD2, k=0.2f, k1=0.2f;

          //  Determine the scale factors
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

          //  OpenGL animation code goes here

          glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
          glClear(GL_COLOR_BUFFER_BIT);
          glPushMatrix();
          glScalef(widthscale, heightscale, 1.0f);
          glRotatef(0.0f, 0.0f, 0.0f, 1.0f);



          //  Draw a bar in rotation
          xA = 2*l*sin(teta);
          yA = 0.0f;
          xB = 0.0f;
          yB = 2*l*cos(teta);
          xI=xA;
          yI=yB;
          glLineWidth(4.0f);
          glColor3f(1.0f, 1.0f, 1.0f);
          line(xA, yA,xB, yB);
          line(-2.25*l, 0,2.25*l, 0);
          line(0, -2.25*l,0, 2.25*l);
          line(xA, yA,xI, yI);
          line(xB, yB,xI, yI);
          circle(0,0,r);
          xC=xA/2;
          yC=yB/2;
          circle(xC,yC,l);
          for(i=0;i<=n;i++){
          	lam=(2*i*l)/n;
          	xD=(2*l-lam)*sin(teta);
          	yD=lam*cos(teta);
          	xD1=(2*l-lam)*omega*cos(teta);
          	yD1=-lam*omega*sin(teta);
          	xD2= xD+k*xD1;
          	yD2= yD+k*yD1;
          	glColor3f(0.0f,1.0f,0.0f);
          	line(xD,yD,xD2,yD2);
		  }
		  for(i=0;i<=n;i++){
          	lam=(2*i*l)/n;
          	xD=(2*l-lam)*sin(teta);
          	yD=lam*cos(teta);
          	xD1=-(2*l-lam)*omega*omega*sin(teta);
          	yD1=-lam*omega*omega*cos(teta);
          	xD2= xD+k1*xD1;
          	yD2= yD+k1*yD1;
          	glColor3f(0.9f,1.0f,0.09f);
          	line(xD,yD,xD2,yD2);
          }



          glPopMatrix();
          SwapBuffers( hDC );

          teta += omega*dt;
        }
  }
          // Shutdown OpenGL
  DisableOpenGL( hWnd, hDC, hRC );

          // Destroy the window explicitly
  DestroyWindow( hWnd );
  return msg.wParam;
}
