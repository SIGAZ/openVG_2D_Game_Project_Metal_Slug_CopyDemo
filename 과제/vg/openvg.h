/****************************************************************************
** File: vg/openvg.h
**
** Authors: Matteo Muratori, Michele Fabbri.
**
** Description: implementation of OpenVG 1.0.
**
** Official website: http://www.amanithvg.com
**
** Copyright (C) 2004-2007 Mazatech S.r.l. All rights reserved.
**
** This file is part of AmanithVG OpenVG 1.0 implementation.
** Khronos and OpenVG are trademarks of The Khronos Group Inc.
** OpenGL is a registered trademark and OpenGL ES is a trademark of
** Silicon Graphics, Inc.
**
** This file is distributed under the terms of Mazatech End-User License
** Agreement for Evaluation Purposes only as defined by Mazatech S.r.l. of
** Italy and appearing in the file LICENSE.TXT included in the packaging
** of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** For any informations, please contact info@mazatech.com
**
****************************************************************************/

#ifndef _OPENVG_H
#define _OPENVG_H

#ifdef __cplusplus
extern "C" {
#endif

#define OPENVG_VERSION_1_0 1

/* AmanithVG extensions */
#define OVG_AM_separable_cap_style          1
#define OVG_AM_color_ramp_interpolation     1
#define OVG_AM_paint_opacity                1
#define OVG_AM_conical_gradient             1
#define OVG_AM_advanced_blend_modes         1
#define OVG_AM_separable_blend_modes        1

#if defined(OVG_STATISTICS)
  #define OVG_AM_statistics                 1
#endif

typedef float VGfloat;
typedef signed char VGbyte;
typedef unsigned char VGubyte;
typedef short VGshort;
typedef int VGint;
typedef unsigned int VGuint;
typedef unsigned int VGbitfield;

typedef enum {
  VG_FALSE = 0,
  VG_TRUE  = 1
} VGboolean;

#define VG_MAXSHORT ((VGshort)((~((unsigned)0)) >> 1))
#define VG_MAXINT ((VGint)((~((unsigned)0)) >> 1))
#define VG_MAXFLOAT 3.402823466e+38f

typedef void* VGHandle;

#define VG_INVALID_HANDLE ((VGHandle)0)

typedef enum {
  VG_NO_ERROR                                 = 0,
  VG_BAD_HANDLE_ERROR                         = 0x1000,
  VG_ILLEGAL_ARGUMENT_ERROR                   = 0x1001,
  VG_OUT_OF_MEMORY_ERROR                      = 0x1002,
  VG_PATH_CAPABILITY_ERROR                    = 0x1003,
  VG_UNSUPPORTED_IMAGE_FORMAT_ERROR           = 0x1004,
  VG_UNSUPPORTED_PATH_FORMAT_ERROR            = 0x1005,
  VG_IMAGE_IN_USE_ERROR                       = 0x1006
} VGErrorCode;

typedef enum {
  /* Mode settings */
  VG_MATRIX_MODE                              = 0x1100,
  VG_FILL_RULE                                = 0x1101,
  VG_IMAGE_QUALITY                            = 0x1102,
  VG_RENDERING_QUALITY                        = 0x1103,
  VG_BLEND_MODE                               = 0x1104,
  // AmanithVG extension, different blend modes, one for fill and one for stroke
#ifdef OVG_AM_separable_blend_modes
  VG_STROKE_BLEND_MODE                        = 0x116A,
  VG_FILL_BLEND_MODE                          = 0x116B,
#endif

  VG_IMAGE_MODE                               = 0x1105,

  /* Scissoring rectangles */
  VG_SCISSOR_RECTS                            = 0x1106,

  /* Stroke parameters */
  VG_STROKE_LINE_WIDTH                        = 0x1110,
  VG_STROKE_CAP_STYLE                         = 0x1111,
  VG_STROKE_JOIN_STYLE                        = 0x1112,
  VG_STROKE_MITER_LIMIT                       = 0x1113,
  VG_STROKE_DASH_PATTERN                      = 0x1114,
  VG_STROKE_DASH_PHASE                        = 0x1115,

  // AmanithVG extension, start and end cap styles
#ifdef OVG_AM_separable_cap_style
  VG_STROKE_START_CAP_STYLE                   = 0x1116,
  VG_STROKE_END_CAP_STYLE                     = 0x1117,
#endif

  /* Edge fill color for VG_TILE_FILL tiling mode */
  VG_TILE_FILL_COLOR                          = 0x1120,

  /* Color for vgClear */
  VG_CLEAR_COLOR                              = 0x1121,

  /* Enable/disable alpha masking and scissoring */
  VG_MASKING                                  = 0x1130,
  VG_SCISSORING                               = 0x1131,

  /* Pixel layout hint information */
  VG_PIXEL_LAYOUT                             = 0x1140,

  /* Source format selection for image filters */
  VG_FILTER_FORMAT_LINEAR                     = 0x1150,
  VG_FILTER_FORMAT_PREMULTIPLIED              = 0x1151,

  /* Destination write enable mask for image filters */
  VG_FILTER_CHANNEL_MASK                      = 0x1152,

  /* Implementation limits (read-only) */
  VG_MAX_SCISSOR_RECTS                        = 0x1160,
  VG_MAX_DASH_COUNT                           = 0x1161,
  VG_MAX_KERNEL_SIZE                          = 0x1162,
  VG_MAX_SEPARABLE_KERNEL_SIZE                = 0x1163,
  VG_MAX_COLOR_RAMP_STOPS                     = 0x1164,
  VG_MAX_IMAGE_WIDTH                          = 0x1165,
  VG_MAX_IMAGE_HEIGHT                         = 0x1166,
  VG_MAX_IMAGE_PIXELS                         = 0x1167,
  VG_MAX_IMAGE_BYTES                          = 0x1168,
  VG_MAX_FLOAT                                = 0x1169
} VGParamType;

typedef enum {
  VG_RENDERING_QUALITY_NONANTIALIASED         = 0x1200,
  VG_RENDERING_QUALITY_FASTER                 = 0x1201,
  VG_RENDERING_QUALITY_BETTER                 = 0x1202  /* Default */
} VGRenderingQuality;

typedef enum {
  VG_PIXEL_LAYOUT_UNKNOWN                     = 0x1300,
  VG_PIXEL_LAYOUT_RGB_VERTICAL                = 0x1301,
  VG_PIXEL_LAYOUT_BGR_VERTICAL                = 0x1302,
  VG_PIXEL_LAYOUT_RGB_HORIZONTAL              = 0x1303,
  VG_PIXEL_LAYOUT_BGR_HORIZONTAL              = 0x1304
} VGPixelLayout;

typedef enum {
  VG_MATRIX_PATH_USER_TO_SURFACE              = 0x1400,
  VG_MATRIX_IMAGE_USER_TO_SURFACE             = 0x1401,
  VG_MATRIX_FILL_PAINT_TO_USER                = 0x1402,
  VG_MATRIX_STROKE_PAINT_TO_USER              = 0x1403
} VGMatrixMode;

typedef enum {
  VG_CLEAR_MASK                               = 0x1500,
  VG_FILL_MASK                                = 0x1501,
  VG_SET_MASK                                 = 0x1502,
  VG_UNION_MASK                               = 0x1503,
  VG_INTERSECT_MASK                           = 0x1504,
  VG_SUBTRACT_MASK                            = 0x1505
} VGMaskOperation;

#define VG_PATH_FORMAT_STANDARD 0

typedef enum {
  VG_PATH_DATATYPE_INVALID                    = -1,
  VG_PATH_DATATYPE_S_8                        =  0,
  VG_PATH_DATATYPE_S_16                       =  1,
  VG_PATH_DATATYPE_S_32                       =  2,
  VG_PATH_DATATYPE_F                          =  3
} VGPathDatatype;

typedef enum {
  VG_ABSOLUTE                                 = 0,
  VG_RELATIVE                                 = 1
} VGPathAbsRel;

typedef enum {
  VG_CLOSE_PATH                               = ( 0 << 1),
  VG_MOVE_TO                                  = ( 1 << 1),
  VG_LINE_TO                                  = ( 2 << 1),
  VG_HLINE_TO                                 = ( 3 << 1),
  VG_VLINE_TO                                 = ( 4 << 1),
  VG_QUAD_TO                                  = ( 5 << 1),
  VG_CUBIC_TO                                 = ( 6 << 1),
  VG_SQUAD_TO                                 = ( 7 << 1),
  VG_SCUBIC_TO                                = ( 8 << 1),
  VG_SCCWARC_TO                               = ( 9 << 1),
  VG_SCWARC_TO                                = (10 << 1),
  VG_LCCWARC_TO                               = (11 << 1),
  VG_LCWARC_TO                                = (12 << 1)
} VGPathSegment;

typedef enum {
  VG_MOVE_TO_ABS                              = VG_MOVE_TO    | VG_ABSOLUTE,
  VG_MOVE_TO_REL                              = VG_MOVE_TO    | VG_RELATIVE,
  VG_LINE_TO_ABS                              = VG_LINE_TO    | VG_ABSOLUTE,
  VG_LINE_TO_REL                              = VG_LINE_TO    | VG_RELATIVE,
  VG_HLINE_TO_ABS                             = VG_HLINE_TO   | VG_ABSOLUTE,
  VG_HLINE_TO_REL                             = VG_HLINE_TO   | VG_RELATIVE,
  VG_VLINE_TO_ABS                             = VG_VLINE_TO   | VG_ABSOLUTE,
  VG_VLINE_TO_REL                             = VG_VLINE_TO   | VG_RELATIVE,
  VG_QUAD_TO_ABS                              = VG_QUAD_TO    | VG_ABSOLUTE,
  VG_QUAD_TO_REL                              = VG_QUAD_TO    | VG_RELATIVE,
  VG_CUBIC_TO_ABS                             = VG_CUBIC_TO   | VG_ABSOLUTE,
  VG_CUBIC_TO_REL                             = VG_CUBIC_TO   | VG_RELATIVE,
  VG_SQUAD_TO_ABS                             = VG_SQUAD_TO   | VG_ABSOLUTE,
  VG_SQUAD_TO_REL                             = VG_SQUAD_TO   | VG_RELATIVE,
  VG_SCUBIC_TO_ABS                            = VG_SCUBIC_TO  | VG_ABSOLUTE,
  VG_SCUBIC_TO_REL                            = VG_SCUBIC_TO  | VG_RELATIVE,
  VG_SCCWARC_TO_ABS                           = VG_SCCWARC_TO | VG_ABSOLUTE,
  VG_SCCWARC_TO_REL                           = VG_SCCWARC_TO | VG_RELATIVE,
  VG_SCWARC_TO_ABS                            = VG_SCWARC_TO  | VG_ABSOLUTE,
  VG_SCWARC_TO_REL                            = VG_SCWARC_TO  | VG_RELATIVE,
  VG_LCCWARC_TO_ABS                           = VG_LCCWARC_TO | VG_ABSOLUTE,
  VG_LCCWARC_TO_REL                           = VG_LCCWARC_TO | VG_RELATIVE,
  VG_LCWARC_TO_ABS                            = VG_LCWARC_TO  | VG_ABSOLUTE,
  VG_LCWARC_TO_REL                            = VG_LCWARC_TO  | VG_RELATIVE
} VGPathCommand;

typedef VGHandle VGPath;

typedef enum {
  VG_PATH_CAPABILITY_APPEND_FROM              = (1 <<  0),
  VG_PATH_CAPABILITY_APPEND_TO                = (1 <<  1),
  VG_PATH_CAPABILITY_MODIFY                   = (1 <<  2),
  VG_PATH_CAPABILITY_TRANSFORM_FROM           = (1 <<  3),
  VG_PATH_CAPABILITY_TRANSFORM_TO             = (1 <<  4),
  VG_PATH_CAPABILITY_INTERPOLATE_FROM         = (1 <<  5),
  VG_PATH_CAPABILITY_INTERPOLATE_TO           = (1 <<  6),
  VG_PATH_CAPABILITY_PATH_LENGTH              = (1 <<  7),
  VG_PATH_CAPABILITY_POINT_ALONG_PATH         = (1 <<  8),
  VG_PATH_CAPABILITY_TANGENT_ALONG_PATH       = (1 <<  9),
  VG_PATH_CAPABILITY_PATH_BOUNDS              = (1 << 10),
  VG_PATH_CAPABILITY_PATH_TRANSFORMED_BOUNDS  = (1 << 11),
  VG_PATH_CAPABILITY_ALL                      = ((1 << 12) - 1)
} VGPathCapabilities;

typedef enum {
  VG_PATH_FORMAT                              = 0x1600,
  VG_PATH_DATATYPE                            = 0x1601,
  VG_PATH_SCALE                               = 0x1602,
  VG_PATH_BIAS                                = 0x1603,
  VG_PATH_NUM_SEGMENTS                        = 0x1604,
  VG_PATH_NUM_COORDS                          = 0x1605
} VGPathParamType;

typedef enum {
  VG_CAP_BUTT                                 = 0x1700,
  VG_CAP_ROUND                                = 0x1701,
  VG_CAP_SQUARE                               = 0x1702
} VGCapStyle;

typedef enum {
  VG_JOIN_MITER                               = 0x1800,
  VG_JOIN_ROUND                               = 0x1801,
  VG_JOIN_BEVEL                               = 0x1802
} VGJoinStyle;

typedef enum {
  VG_EVEN_ODD                                 = 0x1900,
  VG_NON_ZERO                                 = 0x1901
} VGFillRule;

typedef enum {
  VG_STROKE_PATH                              = (1 << 0),
  VG_FILL_PATH                                = (1 << 1)
} VGPaintMode;

typedef VGHandle VGPaint;

typedef enum {
  /* Color paint parameters */
  VG_PAINT_TYPE                               = 0x1A00,
  VG_PAINT_COLOR                              = 0x1A01,
  VG_PAINT_COLOR_RAMP_SPREAD_MODE             = 0x1A02,
  VG_PAINT_COLOR_RAMP_STOPS                   = 0x1A03,

  /* Linear gradient paint parameters */
  VG_PAINT_LINEAR_GRADIENT                    = 0x1A04,

  /* Radial gradient paint parameters */
  VG_PAINT_RADIAL_GRADIENT                    = 0x1A05,

  /* Pattern paint parameters */
  VG_PAINT_PATTERN_TILING_MODE                = 0x1A06

  // AmanithVG extension, conical gradient paint parameters
#ifdef OVG_AM_conical_gradient
  ,
  VG_PAINT_CONICAL_GRADIENT                   = 0x1A07
#endif

  // AmanithVG extension, smooth color ramp interpolation
#ifdef OVG_AM_color_ramp_interpolation
  ,
  VG_PAINT_COLOR_RAMP_INTERPOLATION_TYPE      = 0x1A08
#endif

  // AmanithVG extension, paint opacity
#ifdef OVG_AM_paint_opacity
  ,
  VG_PAINT_OPACITY                            = 0x1A09
#endif

} VGPaintParamType;

typedef enum {
  VG_PAINT_TYPE_COLOR                         = 0x1B00,
  VG_PAINT_TYPE_LINEAR_GRADIENT               = 0x1B01,
  VG_PAINT_TYPE_RADIAL_GRADIENT               = 0x1B02,
  VG_PAINT_TYPE_PATTERN                       = 0x1B03,
  // AmanithVG extension, conical gradient paint type
#ifdef OVG_AM_conical_gradient
  VG_PAINT_TYPE_CONICAL_GRADIENT              = 0x1B04
#endif
} VGPaintType;

typedef enum {
  VG_COLOR_RAMP_SPREAD_PAD                    = 0x1C00,
  VG_COLOR_RAMP_SPREAD_REPEAT                 = 0x1C01,
  VG_COLOR_RAMP_SPREAD_REFLECT                = 0x1C02
} VGColorRampSpreadMode;

// AmanithVG extension, smooth color ramp interpolation
#ifdef OVG_AM_color_ramp_interpolation
typedef enum {
  VG_COLOR_RAMP_INTERPOLATION_LINEAR          = 0x1C80,
  VG_COLOR_RAMP_INTERPOLATION_SMOOTH          = 0x1C81
} VGColorRampInterpolationType;
#endif

typedef enum {
  VG_TILE_FILL                                = 0x1D00,
  VG_TILE_PAD                                 = 0x1D01,
  VG_TILE_REPEAT                              = 0x1D02,
  VG_TILE_REFLECT                             = 0x1D03
} VGTilingMode;

typedef enum {
  VG_IMAGE_FORMAT_INVALID                     = -1,
  VG_sRGBX_8888                               =  0,
  VG_sRGBA_8888                               =  1,
  VG_sRGBA_8888_PRE                           =  2,
  VG_sRGB_565                                 =  3,
  VG_sRGBA_5551                               =  4,
  VG_sRGBA_4444                               =  5,
  VG_sL_8                                     =  6,
  VG_lRGBX_8888                               =  7,
  VG_lRGBA_8888                               =  8,
  VG_lRGBA_8888_PRE                           =  9,
  VG_lL_8                                     = 10,
  VG_A_8                                      = 11,
  VG_BW_1                                     = 12
} VGImageFormat;

typedef VGHandle VGImage;

typedef enum {
  VG_IMAGE_QUALITY_NONANTIALIASED             = (1 << 0),
  VG_IMAGE_QUALITY_FASTER                     = (1 << 1),
  VG_IMAGE_QUALITY_BETTER                     = (1 << 2)
} VGImageQuality;

typedef enum {
  VG_IMAGE_FORMAT                             = 0x1E00,
  VG_IMAGE_WIDTH                              = 0x1E01,
  VG_IMAGE_HEIGHT                             = 0x1E02
} VGImageParamType;

typedef enum {
  VG_DRAW_IMAGE_NORMAL                        = 0x1F00,
  VG_DRAW_IMAGE_MULTIPLY                      = 0x1F01,
  VG_DRAW_IMAGE_STENCIL                       = 0x1F02
} VGImageMode;

typedef enum {
  VG_RED                                      = (1 << 3),
  VG_GREEN                                    = (1 << 2),
  VG_BLUE                                     = (1 << 1),
  VG_ALPHA                                    = (1 << 0)
} VGImageChannel;

typedef enum {
  VG_BLEND_SRC                                = 0x2000,
  VG_BLEND_SRC_OVER                           = 0x2001,
  VG_BLEND_DST_OVER                           = 0x2002,
  VG_BLEND_SRC_IN                             = 0x2003,
  VG_BLEND_DST_IN                             = 0x2004,
  VG_BLEND_MULTIPLY                           = 0x2005,
  VG_BLEND_SCREEN                             = 0x2006,
  VG_BLEND_DARKEN                             = 0x2007,
  VG_BLEND_LIGHTEN                            = 0x2008,
  VG_BLEND_ADDITIVE                           = 0x2009,
  // AmanithVG extension, new blend modes
#ifdef OVG_AM_advanced_blend_modes
  VG_BLEND_CLEAR                              = 0x200A,
  VG_BLEND_DST                                = 0x200B,
  VG_BLEND_SRC_OUT                            = 0x200C,
  VG_BLEND_DST_OUT                            = 0x200D,
  VG_BLEND_SRC_ATOP                           = 0x200E,
  VG_BLEND_DST_ATOP                           = 0x200F,
  VG_BLEND_XOR                                = 0x2010,
  VG_BLEND_OVERLAY                            = 0x2011,
  VG_BLEND_COLOR_DODGE                        = 0x2012,
  VG_BLEND_COLOR_BURN                         = 0x2013,
  VG_BLEND_HARD_LIGHT                         = 0x2014,
  VG_BLEND_SOFT_LIGHT                         = 0x2015,
  VG_BLEND_DIFFERENCE                         = 0x2016,
  VG_BLEND_EXCLUSION                          = 0x2017
#endif
} VGBlendMode;

typedef enum {
  VG_IMAGE_FORMAT_QUERY                       = 0x2100,
  VG_PATH_DATATYPE_QUERY                      = 0x2101
} VGHardwareQueryType;

typedef enum {
  VG_HARDWARE_ACCELERATED                     = 0x2200,
  VG_HARDWARE_UNACCELERATED                   = 0x2201
} VGHardwareQueryResult;

typedef enum {
  VG_VENDOR                                   = 0x2300,
  VG_RENDERER                                 = 0x2301,
  VG_VERSION                                  = 0x2302,
  VG_EXTENSIONS                               = 0x2303
} VGStringID;

#ifdef OVG_AM_statistics
typedef enum {
  VG_STATISTIC_FLATTENING_TOTAL_PTS                                  = (1 << 0),
  VG_STATISTIC_FLATTENING_FILTERED_PTS                               = (1 << 1),
  VG_STATISTIC_FLATTENING_TOTAL_MS                                   = (1 << 2),
  VG_STATISTIC_FLATTENING_CALCULUS_MS                                = (1 << 3),
  VG_STATISTIC_FLATTENING_STORAGE_MS                                 = (1 << 4),
  VG_STATISTIC_FLATTENING_FILTER_MS                                  = (1 << 5),
  VG_STATISTIC_TRI_FILLGEN_MS                                        = (1 << 6),
  VG_STATISTIC_TRI_STROKEGEN_MS                                      = (1 << 7),
  VG_STATISTIC_STROKE_TRI                                            = (1 << 8),
  VG_STATISTIC_FILL_TRI                                              = (1 << 9),
  VG_STATISTIC_BOUNDING_BOX_CALCULUS_MS                              = (1 << 10),
  VG_STATISTIC_TESS_TRIANGULATIONS_COUNT                             = (1 << 11),
  VG_STATISTIC_TESS_TRIANGULATIONS_PTS                               = (1 << 12),
  VG_STATISTIC_TESS_DESCRIPTOR_INITIALIZATION_MS                     = (1 << 13),
  VG_STATISTIC_TESS_CONTOURS_MESH_INSERTION_MS                       = (1 << 14),
  VG_STATISTIC_TESS_DICTIONARY_AND_PRIORITYQUEUE_INITIALIZATION_MS   = (1 << 15),
  VG_STATISTIC_TESS_MONOTONE_REGION_DECOMPOSITION_MS                 = (1 << 16),
  VG_STATISTIC_TESS_PURGE_REGIONS_MS                                 = (1 << 17),
  VG_STATISTIC_TESS_MONOTONE_REGIONS_TRINAGULATION_MS                = (1 << 18),
  VG_STATISTIC_TESS_FREE_TESSELLATION_MS                             = (1 << 19),
  VG_STATISTIC_ALL                                                   = ((1 << 20) - 1)
} VGStatisticInfo;
#endif

// Codewarrior doesn't define any Windows symbols until some headers are included
#if defined(__MWERKS__)
	#include <stddef.h>
#endif

// Brew
#if defined(__BREW__)
	#define VG_OS_BREW
// Symbian
#elif defined(__SYMBIAN32__)
	#define VG_OS_SYMBIAN
// WindowsCE
#elif defined(_WIN32_WCE) || defined(WINCE)
	#define VG_OS_WINCE
// BeOS
#elif defined(__BEOS__)
	#define VG_OS_BEOS
// Darwin OS (Without Mac OS X)
#elif defined(__DARWIN_X11__)
	#define VG_OS_DARWIN
// MacOS X
#elif defined(__APPLE__) && defined(__GNUC__)
	#define VG_OS_MACX
#elif defined(__MACOSX__)
	#define VG_OS_MACX
// MacOS 9
#elif defined(macintosh)
	#define VG_OS_MAC9
// Free BSD
#elif defined(__FreeBSD__)
	#define VG_OS_FREEBSD
// Net BSD
#elif defined(__NetBSD__)
	#define VG_OS_NETBSD
// Open BSD
#elif defined(__OpenBSD__)
	#define VG_OS_OPENBSD
// BSD / OS
#elif defined(__bsdi__)
	#define VG_OS_BSDI
// SGI Irix
#elif defined(__sgi)
	#define VG_OS_IRIX
// IBM AIX
#elif defined(_AIX)
	#define VG_OS_AIX
// Win64 core
#elif !defined(SAG_COM) && (defined(WIN64) || defined(_WIN64) || defined(__WIN64__))
	#define VG_OS_WIN32
	#define VG_OS_WIN64
// Win32 core (Windows 95/98/ME/NT/2000/XP)
#elif !defined(SAG_COM) && (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
	#define VG_OS_WIN32
	// some old libraries/application test for _WIN32 instead of the standard WIN32
	#ifndef _WIN32
		#define _WIN32
	#endif
// Linux
#elif defined(__LINUX__) || defined(__linux__) || defined(__linux)
	#define VG_OS_LINUX
// Sun SOLARIS
#elif defined(__sun) || defined(sun) || defined(__SUN__)
	#define VG_OS_SOLARIS
// HP-UX (the standard macro is __hpux)
#elif defined(__hpux) || defined(__hpux__)
	#define VG_OS_HPUX
#endif
// merge macros for Windows and Mac
#if (defined(VG_OS_WIN32) || defined(VG_OS_WIN64)) && !defined(VG_OS_WINCE)
	#define VG_OS_WIN
	// impose that minimum requirements are Windows 2000 for NT kernels, and Windows 98 for 9x kernels
	#ifndef _WIN32_WINNT
		#define _WIN32_WINNT 0x0500
	#endif
	#ifndef _WIN32_WINDOWS
		#define _WIN32_WINDOWS 0x0410
	#endif
	// take care of Visual Studio .Net 2005 C deprecations
	#if _MSC_VER >= 1400
		#ifndef _CRT_SECURE_NO_DEPRECATE
			#define _CRT_SECURE_NO_DEPRECATE 1
		#endif
		#ifndef _CRT_NONSTDC_NO_DEPRECATE
			#define _CRT_NONSTDC_NO_DEPRECATE 1
		#endif
		#pragma warning (disable:4996)
	#endif
#endif
#if defined(VG_OS_MAC9) || defined(VG_OS_MACX)
	#define VG_OS_MAC
#endif
// UNIX like system
#if defined(__UNIX__) || defined(__unix) || defined(__unix__) || defined(VG_OS_LINUX) || defined(VG_OS_IRIX) || defined(VG_OS_HPUX) || defined(VG_OS_SOLARIS) || defined(VG_OS_AIX) ||  defined(VG_OS_BEOS)
	#define VG_OS_UNIXLIKE
#endif
// BSD systems
#if defined(VG_OS_FREEBSD) || defined(VG_OS_NETBSD) || defined(VG_OS_OPENBSD) || defined(VG_OS_BSDI)
	#define VG_OS_BSDLIKE
#endif

#if defined(OPENVG_STATIC_LIBRARY)
	#define VG_API_CALL
	#undef OPENVG_MAKE_DLL
#else
	// Win32 (Windows 95/98/ME/NT/2000/XP/CE) using MSVC
	#if (defined(VG_OS_WIN) || defined(VG_OS_WINCE)) && !defined(__CYGWIN__)
		#if defined(OPENVG_MAKE_DLL)
			#define VG_API_CALL __declspec(dllexport)
		#else
			#define VG_API_CALL __declspec(dllimport)
		#endif
	// MacOS X
	#elif defined(VG_OS_MACX)
		#if defined(OPENVG_MAKE_DLL)
			#define VG_API_CALL __attribute__((visibility("default"))) extern
		#else
			#define VG_API_CALL extern
		#endif
	// Brew with Windows simulator
	#elif defined(VG_OS_BREW) && defined(AEE_SIMULATOR)
		#if defined(OPENVG_MAKE_DLL)
			#define VG_API_CALL __declspec(dllexport)
		#else
			#define VG_API_CALL __declspec(dllimport)
		#endif
	// Symbian CodeWarrior on Win32 (WINSCW builds)
	#elif defined(VG_OS_SYMBIAN) && (defined(__MWERKS__) || defined(__CW32__))
		#if defined(OPENVG_MAKE_DLL)
			#define VG_API_CALL __declspec(dllexport)
		#else
			#define VG_API_CALL __declspec(dllimport)
		#endif
	// Symbian GCC
	#elif defined(VG_OS_SYMBIAN) && (defined (__GCC32__) || defined(__GCCE__))
		#if defined(OPENVG_MAKE_DLL)
			#define VG_API_CALL __declspec(dllexport)
		#else
			#define VG_API_CALL __declspec(dllimport)
		#endif
	// Symbian RVCT MakMake fix
	#elif defined(VG_OS_SYMBIAN) && defined (__ARMCC_2__)
		#define VG_API_CALL
	// Linux/BSD/Unix
	#else
		#if defined(OPENVG_MAKE_DLL)
			#define VG_API_CALL __attribute__((visibility("default"))) extern
		#else
			#define VG_API_CALL extern
		#endif
	#endif
#endif
#ifndef VG_API_CALL
	#error VG_API_CALL must be defined
#endif

VG_API_CALL VGErrorCode vgGetError(void);

VG_API_CALL void vgFlush(void);
VG_API_CALL void vgFinish(void);

/* Getters and Setters */
VG_API_CALL void vgSetf (VGParamType type, VGfloat value);
VG_API_CALL void vgSeti (VGParamType type, VGint value);
VG_API_CALL void vgSetfv(VGParamType type, VGint count,
                         const VGfloat * values);
VG_API_CALL void vgSetiv(VGParamType type, VGint count,
                         const VGint * values);

VG_API_CALL VGfloat vgGetf(VGParamType type);
VG_API_CALL VGint   vgGeti(VGParamType type);
VG_API_CALL VGint   vgGetVectorSize(VGParamType type);
VG_API_CALL void    vgGetfv(VGParamType type, VGint count, VGfloat * values);
VG_API_CALL void    vgGetiv(VGParamType type, VGint count, VGint * values);

VG_API_CALL void vgSetParameterf(VGHandle object,
                                 VGint paramType,
                                 VGfloat value);
VG_API_CALL void vgSetParameteri(VGHandle object,
                                 VGint paramType,
                                 VGint value);
VG_API_CALL void vgSetParameterfv(VGHandle object,
                                  VGint paramType,
                                  VGint count, const VGfloat * values);
VG_API_CALL void vgSetParameteriv(VGHandle object,
                                  VGint paramType,
                                  VGint count, const VGint * values);

VG_API_CALL VGfloat vgGetParameterf(VGHandle object,
                                    VGint paramType);
VG_API_CALL VGint vgGetParameteri(VGHandle object,
                                  VGint paramType);
VG_API_CALL VGint vgGetParameterVectorSize(VGHandle object,
                                           VGint paramType);
VG_API_CALL void vgGetParameterfv(VGHandle object,
                                  VGint paramType,
                                  VGint count, VGfloat * values);
VG_API_CALL void vgGetParameteriv(VGHandle object,
                                  VGint paramType,
                                  VGint count, VGint * values);

/* Matrix Manipulation */
VG_API_CALL void vgLoadIdentity(void);
VG_API_CALL void vgLoadMatrix(const VGfloat * m);
VG_API_CALL void vgGetMatrix(VGfloat * m);
VG_API_CALL void vgMultMatrix(const VGfloat * m);
VG_API_CALL void vgTranslate(VGfloat tx, VGfloat ty);
VG_API_CALL void vgScale(VGfloat sx, VGfloat sy);
VG_API_CALL void vgShear(VGfloat shx, VGfloat shy);
VG_API_CALL void vgRotate(VGfloat angle);

/* Masking and Clearing */
VG_API_CALL void vgMask(VGImage mask, VGMaskOperation operation,
                        VGint x, VGint y, VGint width, VGint height);
VG_API_CALL void vgClear(VGint x, VGint y, VGint width, VGint height);

/* Paths */
VG_API_CALL VGPath vgCreatePath(VGint pathFormat,
                                VGPathDatatype datatype,
                                VGfloat scale, VGfloat bias,
                                VGint segmentCapacityHint,
                                VGint coordCapacityHint,
                                VGbitfield capabilities);
VG_API_CALL void vgClearPath(VGPath path, VGbitfield capabilities);
VG_API_CALL void vgDestroyPath(VGPath path);
VG_API_CALL void vgRemovePathCapabilities(VGPath path,
                                          VGbitfield capabilities);
VG_API_CALL VGbitfield vgGetPathCapabilities(VGPath path);
VG_API_CALL void vgAppendPath(VGPath dstPath, VGPath srcPath);
VG_API_CALL void vgAppendPathData(VGPath dstPath,
                                  VGint numSegments,
                                  const VGubyte * pathSegments,
                                  const void * pathData);
VG_API_CALL void vgModifyPathCoords(VGPath dstPath, VGint startIndex,
                                    VGint numSegments,
                                    const void * pathData);
VG_API_CALL void vgTransformPath(VGPath dstPath, VGPath srcPath);
VG_API_CALL VGboolean vgInterpolatePath(VGPath dstPath,
                                        VGPath startPath,
                                        VGPath endPath,
                                        VGfloat amount);
VG_API_CALL VGfloat vgPathLength(VGPath path,
                                 VGint startSegment, VGint numSegments);
VG_API_CALL void vgPointAlongPath(VGPath path,
                                  VGint startSegment, VGint numSegments,
                                  VGfloat distance,
                                  VGfloat * x, VGfloat * y,
                                  VGfloat * tangentX, VGfloat * tangentY);
VG_API_CALL void vgPathBounds(VGPath path,
                              VGfloat * minX, VGfloat * minY,
                              VGfloat * width, VGfloat * height);
VG_API_CALL void vgPathTransformedBounds(VGPath path,
                                         VGfloat * minX, VGfloat * minY,
                                         VGfloat * width, VGfloat * height);
VG_API_CALL void vgDrawPath(VGPath path, VGbitfield paintModes);

/* Paint */
VG_API_CALL VGPaint vgCreatePaint(void);
VG_API_CALL void vgDestroyPaint(VGPaint paint);
VG_API_CALL void vgSetPaint(VGPaint paint, VGbitfield paintModes);
VG_API_CALL VGPaint vgGetPaint(VGPaintMode paintMode);
VG_API_CALL void vgSetColor(VGPaint paint, VGuint rgba);
VG_API_CALL VGuint vgGetColor(VGPaint paint);
VG_API_CALL void vgPaintPattern(VGPaint paint, VGImage pattern);

/* Images */
VG_API_CALL VGImage vgCreateImage(VGImageFormat format,
                                  VGint width, VGint height,
                                  VGbitfield allowedQuality);
VG_API_CALL void vgDestroyImage(VGImage image);
VG_API_CALL void vgClearImage(VGImage image,
                              VGint x, VGint y, VGint width, VGint height);
VG_API_CALL void vgImageSubData(VGImage image,
                                const void * data, VGint dataStride,
                                VGImageFormat dataFormat,
                                VGint x, VGint y, VGint width, VGint height);
VG_API_CALL void vgGetImageSubData(VGImage image,
                                   void * data, VGint dataStride,
                                   VGImageFormat dataFormat,
                                   VGint x, VGint y,
                                   VGint width, VGint height);
VG_API_CALL VGImage vgChildImage(VGImage parent,
                                 VGint x, VGint y, VGint width, VGint height);
VG_API_CALL VGImage vgGetParent(VGImage image);
VG_API_CALL void vgCopyImage(VGImage dst, VGint dx, VGint dy,
                             VGImage src, VGint sx, VGint sy,
                             VGint width, VGint height,
                             VGboolean dither);
VG_API_CALL void vgDrawImage(VGImage image);
VG_API_CALL void vgSetPixels(VGint dx, VGint dy,
                             VGImage src, VGint sx, VGint sy,
                             VGint width, VGint height);
VG_API_CALL void vgWritePixels(const void * data, VGint dataStride,
                               VGImageFormat dataFormat,
                               VGint dx, VGint dy,
                               VGint width, VGint height);
VG_API_CALL void vgGetPixels(VGImage dst, VGint dx, VGint dy,
                             VGint sx, VGint sy,
                             VGint width, VGint height);
VG_API_CALL void vgReadPixels(void * data, VGint dataStride,
                              VGImageFormat dataFormat,
                              VGint sx, VGint sy,
                              VGint width, VGint height);
VG_API_CALL void vgCopyPixels(VGint dx, VGint dy,
                              VGint sx, VGint sy,
                              VGint width, VGint height);

/* Image Filters */
VG_API_CALL void vgColorMatrix(VGImage dst, VGImage src,
                               const VGfloat * matrix);
VG_API_CALL void vgConvolve(VGImage dst, VGImage src,
                            VGint kernelWidth, VGint kernelHeight,
                            VGint shiftX, VGint shiftY,
                            const VGshort * kernel,
                            VGfloat scale,
                            VGfloat bias,
                            VGTilingMode tilingMode);
VG_API_CALL void vgSeparableConvolve(VGImage dst, VGImage src,
                                     VGint kernelWidth,
                                     VGint kernelHeight,
                                     VGint shiftX, VGint shiftY,
                                     const VGshort * kernelX,
                                     const VGshort * kernelY,
                                     VGfloat scale,
                                     VGfloat bias,
                                     VGTilingMode tilingMode);
VG_API_CALL void vgGaussianBlur(VGImage dst, VGImage src,
                                VGfloat stdDeviationX,
                                VGfloat stdDeviationY,
                                VGTilingMode tilingMode);
VG_API_CALL void vgLookup(VGImage dst, VGImage src,
                          const VGubyte * redLUT,
                          const VGubyte * greenLUT,
                          const VGubyte * blueLUT,
                          const VGubyte * alphaLUT,
                          VGboolean outputLinear,
                          VGboolean outputPremultiplied);
VG_API_CALL void vgLookupSingle(VGImage dst, VGImage src,
                                const VGuint * lookupTable,
                                VGImageChannel sourceChannel,
                                VGboolean outputLinear,
                                VGboolean outputPremultiplied);

/* Hardware Queries */
VG_API_CALL VGHardwareQueryResult vgHardwareQuery(VGHardwareQueryType key,
                                                  VGint setting);

/* Renderer and Extension Information */
VG_API_CALL const VGubyte * vgGetString(VGStringID name);

/* AmanithVG proprietary calls */
VG_API_CALL VGboolean vgInitContextAM(const VGint windowWidth, const VGint windowHeight,
									  const VGboolean surfaceLinearColorSpace,
									  const VGboolean surfaceAlphaPremultiplied);
VG_API_CALL void vgDestroyContextAM(void);
VG_API_CALL void vgResizeSurfaceAM(const VGint windowWidth, const VGint windowHeight);

/* AmanithVG extensions */
#ifdef OVG_AM_statistics

/*
In the statistics build, this resets statistics counters.

Errors
------
VG_ILLEGAL_ARGUMENT_ERROR: if statistics is not a valid bitwise OR of values from the VGStatisticInfo enumaration

*/
VG_API_CALL void vgResetStatisticsAM(const VGbitfield statistics);
/*

In the statistics build, this returns statistics gathered since the last vgResetStatisticsAM.

Errors
------
VG_ILLEGAL_ARGUMENT_ERROR: if statistic is not one of the values from VGStatisticInfo enumaration

*/
VG_API_CALL VGint vgGetStatisticiAM(const VGStatisticInfo statistic);

#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _OPENVG_H */
