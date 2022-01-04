
#include "stdafx.h"
#include "egl/egl.h"

static EGLDisplay currentDisplay;
static EGLSurface currentSurface;
static EGLContext currentContext;

static HWND hWnd = NULL;
static HDC hDC = NULL;
static HGLRC hRC = NULL;
static HINSTANCE hInstance;

BOOL createGLWindow(TCHAR *title, int width, int height, BOOL fullscreenFlag);

EGLDisplay eglGetDisplay( HDC hdc)
{	
	hDC = hdc;
	return 0;
}
void eglInitialize( EGLDisplay d, int* majorVersion, int* minorVer )
{}
void eglBindAPI( int a )
{}
void eglChooseConfig( EGLDisplay d, int* i1, EGLConfig* c, int i2, int* i3  )
{}
int eglSwapBuffers( EGLDisplay d, EGLSurface s)
{	
	SwapBuffers( hDC );
	return 1;
}
int eglMakeCurrent( EGLDisplay d, EGLSurface s1, EGLSurface s2, EGLContext c )
{
	currentDisplay = d;
	currentContext = c;
	currentSurface = s1;
	wglMakeCurrent(hDC, hRC);
	return 1;
}
EGLContext eglCreateContext( EGLDisplay d, EGLConfig c, HWND hwnd, void* v2 )
{
	return currentContext;
}
EGLSurface eglCreateWindowSurface( EGLDisplay d, EGLConfig c, HWND hwnd, void* v )
{	
	RECT size;	
	EGLint width, height;

	hWnd = hwnd;
	GetClientRect(hwnd, &size);
	width = size.right - size.left;
	height = size.bottom - size.top;

	createGLWindow( NULL, width, height, FALSE ); 
		
	vgInitContextAM(width, height, VG_FALSE, VG_TRUE);	
	
	return currentSurface;
}
void eglDestroyContext( EGLDisplay d, EGLContext c )
{}
void eglDestroySurface( EGLDisplay d, EGLSurface s )
{}



BOOL wglExtensionSupported(const char *extension) {

	const size_t extlen = strlen(extension);
	const char *supported = NULL;
	const char *p;

	PROC wglGetExtString = wglGetProcAddress("wglGetExtensionsStringARB");

	if (wglGetExtString)
		supported = ((char*(__stdcall*)(HDC))wglGetExtString)(wglGetCurrentDC());

	if (supported == NULL)
		supported = (char*)glGetString(GL_EXTENSIONS);

	if (supported == NULL)
		return FALSE;

	for (p = supported; ; p++) {
		p = strstr(p, extension);
		if (p == NULL)
			return FALSE;
		if ((p == supported || p[-1] == ' ') && (p[extlen] == '\0' || p[extlen] == ' '))
			return TRUE;
	}
}

BOOL glExtensionSupported(const char *extension) {

	const size_t extlen = strlen(extension);
	const char *supported = NULL;
	const char *p;

	supported = (char*)glGetString(GL_EXTENSIONS);
	if (supported == NULL)
		return FALSE;
	for (p = supported; ; p++) {
		p = strstr(p, extension);
		if (p == NULL)
			return FALSE;
		if ((p == supported || p[-1] == ' ') && (p[extlen] == '\0' || p[extlen] == ' '))
			return TRUE;
	}
}

void killGLWindow(GLvoid) {

	if (hRC) {
		if (!wglMakeCurrent(NULL, NULL))
			MessageBox(NULL, TEXT("Release Of DC And RC Failed."), TEXT("SHUTDOWN ERROR"), MB_OK | MB_ICONINFORMATION);
		if (!wglDeleteContext(hRC))
			MessageBox(NULL, TEXT("Release Rendering Context Failed."), TEXT("SHUTDOWN ERROR"), MB_OK | MB_ICONINFORMATION);
		hRC = NULL;
	}
	if (hDC && !ReleaseDC(hWnd, hDC)) {
		MessageBox(NULL, TEXT("Release Device Context Failed."), TEXT("SHUTDOWN ERROR"), MB_OK | MB_ICONINFORMATION);
		hDC = NULL;
	}
	if (hWnd && !DestroyWindow(hWnd)) {
		MessageBox(NULL, TEXT("Could Not Release hWnd."), TEXT("SHUTDOWN ERROR"), MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;
	}
	if (!UnregisterClass(TEXT("OpenGL"), hInstance)) {
		MessageBox(NULL, TEXT("Could Not Unregister Class."), TEXT("SHUTDOWN ERROR"), MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;
	}
}

BOOL createGLWindow(TCHAR *title, int width, int height, BOOL fullscreenFlag) {


	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	//WNDCLASS	wc;						// Windows Class Structure
	//DWORD		dwExStyle;				// Window Extended Style
	//DWORD		dwStyle;				// Window Style
	//RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	static PIXELFORMATDESCRIPTOR pfd =	{			// pfd Tells Windows How We Want Things To Be
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
			1,										// Version Number
			PFD_DRAW_TO_WINDOW |					// Format Must Support Window
			PFD_SUPPORT_OPENGL |					// Format Must Support OpenGL
			PFD_DOUBLEBUFFER,						// Must Support Double Buffering
			PFD_TYPE_RGBA,							// Request An RGBA Format
			24,										// Select Our Color Depth
			0, 0, 0, 0, 0, 0,						// Color Bits Ignored
			8,										// 8bit Alpha Buffer
			0,										// Shift Bit Ignored
			0,										// No Accumulation Buffer
			0, 0, 0, 0,								// Accumulation Bits Ignored
			16,										// 16Bit Z-Buffer (Depth Buffer)  
			8,										// 8 bits Stencil Buffer
			0,										// No Auxiliary Buffer
			PFD_MAIN_PLANE,							// Main Drawing Layer
			0,										// Reserved
			0, 0, 0									// Layer Masks Ignored
	};

	/*
	WindowRect.left = (long)0;			// Set Left Value To 0
	WindowRect.right = (long)width;		// Set Right Value To Requested Width
	WindowRect.top = (long)0;			// Set Top Value To 0
	WindowRect.bottom = (long)height;	// Set Bottom Value To Requested Height

	
	fullscreen = fullscreenFlag;		// Set The Global Fullscreen Flag

	
	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= TEXT("OpenGL");								// Set The Class Name

	if (!RegisterClass(&wc)) {
		MessageBox(NULL, TEXT("Failed To Register The Window Class."), TEXT("ERROR"), MB_OK | MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}
	
	// Window Extended Style
	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	dwStyle = WS_TILEDWINDOW;

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	
	// Create The Window
	if (!(hWnd = CreateWindowEx(dwExStyle,							// Extended Style For The Window
								TEXT("OpenGL"),							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right - WindowRect.left,	// Calculate Window Width
								WindowRect.bottom - WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL))) {							// Don't Pass Anything To WM_CREATE

		killGLWindow();								// Reset The Display
		MessageBox(NULL, TEXT("Window Creation Error."), TEXT("ERROR"), MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hDC = GetDC(hWnd))) {
		killGLWindow();
		MessageBox(NULL, TEXT("Can't Create A GL Device Context."), TEXT("ERROR"), MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}
	
	if (!arbMultisampleSupported) {
		if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd))) {
			killGLWindow();
			MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}
	}
	else
		PixelFormat = arbMultisampleFormat;
*/
	
	PixelFormat = ChoosePixelFormat(hDC, &pfd);

	if (!SetPixelFormat(hDC, PixelFormat, &pfd)) {
		killGLWindow();
		MessageBox(NULL, TEXT("Can't Set The PixelFormat."), TEXT("ERROR"), MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!(hRC = wglCreateContext(hDC))) {
		killGLWindow();
		MessageBox(NULL, TEXT("Can't Create A GL Rendering Context."), TEXT("ERROR"), MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!wglMakeCurrent(hDC, hRC)) {
		killGLWindow();
		MessageBox(NULL, TEXT("Can't Activate The GL Rendering Context."), TEXT("ERROR"), MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	/*
	if (!arbMultisampleSupported) {
		
		if (initMultisample(pfd)) {
			killGLWindow();
			return createGLWindow(title, width, height, fullscreenFlag);
		}
	}
	

	//ShowWindow(hWnd, SW_SHOW);						// Show The Window
	//SetForegroundWindow(hWnd);						// Slightly Higher Priority
	//SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	*/
	
	return TRUE;									// Success
}
