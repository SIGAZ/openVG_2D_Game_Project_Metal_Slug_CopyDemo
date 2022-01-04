#ifndef _EGL_H_
#define _EGL_H_

#include <windows.h>
#include "../vg/openvg.h"
#include <gl/GL.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "wglext.h"

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "libopenvg.lib")

typedef struct
{
	unsigned char*	buffer;
	int				stride;
	int				width;
	int				height;
	int	surface;
} EGL_surface_;

typedef int				EGLContext;
typedef int             EGLDisplay;
typedef EGL_surface_*   EGLSurface;
typedef int             EGLConfig;

typedef float			EGLfloat;
typedef int				EGLint;

#define EGL_RED_SIZE		0
#define EGL_BLUE_SIZE		0
#define EGL_GREEN_SIZE		0
#define EGL_ALPHA_SIZE		0
#define EGL_SURFACE_TYPE 	0
#define EGL_RENDERABLE_TYPE 0
#define EGL_PIXMAP_BIT 		0
#define EGL_WINDOW_BIT 		0
#define EGL_NONE 			0
#define EGL_COLORSPACE		0
#define EGL_COLORSPACE_sRGB	0
#define EGL_ALPHA_FORMAT	0
#define EGL_ALPHA_FORMAT_PRE 0

#define EGL_DEFAULT_DISPLAY 0
#define EGL_FALSE 0
#define EGL_TRUE 1
#define EGL_OPENVG_API 0
#define EGL_OPENVG_BIT 0

extern EGLDisplay eglGetDisplay( HDC );
extern void eglInitialize( EGLDisplay, int* majorVersion, int* minorVer );
extern void eglBindAPI( int );
extern void eglChooseConfig( EGLDisplay, int*, EGLConfig*, int, int* );
extern int eglSwapBuffers( EGLDisplay, EGLSurface );
extern int eglMakeCurrent( EGLDisplay, EGLSurface, EGLSurface, EGLContext );
extern EGLContext eglCreateContext( EGLDisplay, EGLConfig, HWND hwnd, void* );
extern EGLSurface eglCreateWindowSurface( EGLDisplay, EGLConfig, HWND hwnd, void* );
extern void eglDestroyContext( EGLDisplay, EGLContext );
extern void eglDestroySurface( EGLDisplay, EGLSurface );

#endif
