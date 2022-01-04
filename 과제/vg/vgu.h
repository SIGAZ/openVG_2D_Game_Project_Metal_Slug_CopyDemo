/****************************************************************************
** File: vg/vgu.h
**
** Authors: Matteo Muratori, Michele Fabbri.
**
** Description: implementation of VGU 1.0.
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

#ifndef VGU_H
#define VGU_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef OPENVG_MAKE_DLL
	#include "vg/openvg.h"
#else
	#include <vg/openvg.h>
#endif

#define VGU_VERSION_1_0 1

#ifndef VGU_API_CALL
#define VGU_API_CALL VG_API_CALL
#endif

/* AmanithVG SVG extensions */
#define OVG_AM_vgu_svg_support					1

typedef enum {
  VGU_NO_ERROR                                 = 0,
  VGU_BAD_HANDLE_ERROR                         = 0xF000,
  VGU_ILLEGAL_ARGUMENT_ERROR                   = 0xF001,
  VGU_OUT_OF_MEMORY_ERROR                      = 0xF002,
  VGU_PATH_CAPABILITY_ERROR                    = 0xF003,
  VGU_BAD_WARP_ERROR                           = 0xF004
} VGUErrorCode;

typedef enum {
  VGU_ARC_OPEN                                 = 0xF100,
  VGU_ARC_CHORD                                = 0xF101,
  VGU_ARC_PIE                                  = 0xF102
} VGUArcType;


#ifdef OVG_AM_vgu_svg_support
typedef enum {
	/* Degrees */
	VGU_ANGLE_DEG                              = 0xF800,
	/* Grads */
	VGU_ANGLE_GRAD                             = 0xF801,
	/* Radians */
	VGU_ANGLE_RAD                              = 0xF802
} VGUAngleType;
#endif

VGU_API_CALL VGUErrorCode vguLine(VGPath path,
                  VGfloat x0, VGfloat y0,
                  VGfloat x1, VGfloat y1);

VGU_API_CALL VGUErrorCode vguPolygon(VGPath path,
                    const VGfloat * points, VGint count,
                    VGboolean closed);

VGU_API_CALL VGUErrorCode vguRect(VGPath path,
                 VGfloat x, VGfloat y,
                 VGfloat width, VGfloat height);

VGU_API_CALL VGUErrorCode vguRoundRect(VGPath path,
                      VGfloat x, VGfloat y,
                      VGfloat width, VGfloat height,
                      VGfloat arcWidth, VGfloat arcHeight);

VGU_API_CALL VGUErrorCode vguEllipse(VGPath path,
                    VGfloat cx, VGfloat cy,
                    VGfloat width, VGfloat height);

VGU_API_CALL VGUErrorCode vguArc(VGPath path,
                VGfloat x, VGfloat y,
                VGfloat width, VGfloat height,
                VGfloat startAngle, VGfloat angleExtent,
                VGUArcType arcType);

VGU_API_CALL VGUErrorCode vguComputeWarpQuadToSquare(VGfloat sx0, VGfloat sy0,
                                                    VGfloat sx1, VGfloat sy1,
                                                    VGfloat sx2, VGfloat sy2,
                                                    VGfloat sx3, VGfloat sy3,
                                                    VGfloat * matrix);

VGU_API_CALL VGUErrorCode vguComputeWarpSquareToQuad(VGfloat dx0, VGfloat dy0,
                                                    VGfloat dx1, VGfloat dy1,
                                                    VGfloat dx2, VGfloat dy2,
                                                    VGfloat dx3, VGfloat dy3,
                                                    VGfloat * matrix);

VGU_API_CALL VGUErrorCode vguComputeWarpQuadToQuad(VGfloat sx0, VGfloat sy0,
                                                  VGfloat sx1, VGfloat sy1,
                                                  VGfloat sx2, VGfloat sy2,
                                                  VGfloat sx3, VGfloat sy3,
                                                  VGfloat dx0, VGfloat dy0,
                                                  VGfloat dx1, VGfloat dy1,
                                                  VGfloat dx2, VGfloat dy2,
                                                  VGfloat dx3, VGfloat dy3,
                                                  VGfloat * matrix);

#ifdef OVG_AM_vgu_svg_support
/*!
	Given an UTF-8 formatted ASCIIZ string, containing the SVG color name according to the specifications
	from W3C (http://www.w3.org/TR/SVG/types.html#ColorKeywords), it returns the rgb components in the [0; 1] range
	compatible with OpenVG.
	NB: the comparison is case-insensitive, so for example "AliceBLUE" will be equal to "aliceblue"

	Errors
	------
	VGU_ILLEGAL_ARGUMENT_ERROR
	- if one of the four supplied parameters is NULL
	- if svgColorName is not a valid SVG color name, according to the specifications from W3C (http://www.w3.org/TR/SVG/types.html#ColorKeywords)
*/
VGU_API_CALL VGUErrorCode vguSVGColorByName(const char *svgColorName, VGfloat *red, VGfloat *green, VGfloat *blue);

/*!
	Given an UTF-8 formatted ASCIIZ string, containing the SVG path 'd' attribute (http://www.w3.org/TR/SVG/paths.html#PathData), it
	appends the corresponding segments and data to the OpenVG path.
	angleType is used to specify the angle measure units for the elliptical arcs rotation angle.

	Errors
	------
	VGU_BAD_HANDLE_ERROR
	- if path is not a valid path handle, or is not shared with the current context

	VGU_ILLEGAL_ARGUMENT_ERROR
	- if svgPath is NULL or has a length of 0
	- if angleType is not a valid value form the VGUAngleType enumeration
*/
VGU_API_CALL VGUErrorCode vguSVGPath(VGPath path, const char *svgPath, VGUAngleType angleType);

#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* #ifndef VGU_H */
