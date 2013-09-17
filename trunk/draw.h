#ifndef _DRAW_H
#define _DRAW_H

#include "texture.h"
#include "cluster.h"
#include "define.h"

#include <GL/glew.h>

void DrawWin();
void DrawLle(Tex2D *pTexLle, unsigned w, unsigned h);
void DrawSlice(Tex3D *pTexVal, unsigned sliceNo, unsigned axis, unsigned w, unsigned h);
void MapContour(Tex2D &texContour, unsigned w, unsigned h);
void DrawContour(float aContPntNow[], Tex3D *pTexVal, Tex2D &texContour, unsigned axis, unsigned w, unsigned h);
void DrawCrossSection(Tex3D *pTexVal, unsigned xSliceNo, unsigned ySliceNo, unsigned zSliceNo);
void DrawBotSelecter(Selecter &sel, unsigned w, unsigned h, bool bColor, bool bEdt);
void DrawTopSelecter(Selecter &sel, unsigned w, unsigned h);
void DrawRedLevel(float redLevel, unsigned w, unsigned h);

#endif