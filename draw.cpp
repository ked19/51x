#include "draw.h"

void DrawWin()
{
	glMatrixMode(GL_PROJECTION);	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);		glLoadIdentity();

	glLineWidth(3.0);
	//glColor4f(0.4f, 0.3f, 0.2f, 1.f);
	glColor4f(1.f, 1.f, 1.f, 1.f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
		glVertex2f(-1.f, -1.f);
		glVertex2f(1.f,  -1.f);
		glVertex2f(1.f,  1.f);
		glVertex2f(-1.f, 1.f);
	glEnd();
	glLineWidth(1.0);
}

//***********************************************

void DrawLle(Tex2D *pTexLle, unsigned w, unsigned h)
{
	glMatrixMode(GL_PROJECTION);	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);		glLoadIdentity();
	gluOrtho2D(0, w-1, 0, h-1);

	float aLowCoord[] = {0, 0};
	pTexLle->DrawRec2D(aLowCoord, (float)w, (float)h, false);
}

//***********************************************

void DrawSlice(Tex3D *pTexVal, unsigned sliceNo, unsigned axis, unsigned w, unsigned h)
{
	glMatrixMode(GL_PROJECTION);	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);		glLoadIdentity();
	gluOrtho2D(0, w-1, 0, h-1);

	float aLowCoord[] = {0, 0};
	pTexVal->DrawOrthSlice2D(aLowCoord, (float)w, (float)h, sliceNo, axis, false);
}

//***********************************************

void MapContour(Tex2D &texContour, unsigned w, unsigned h)
{
	glMatrixMode(GL_PROJECTION);	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);		glLoadIdentity();
	gluOrtho2D(0, w-1, 0, h-1);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	float aLowCoord[] = {0, 0};
	texContour.DrawRec2D(aLowCoord, (float)w, (float)h, false);
	glDisable(GL_BLEND);
}

//***********************************************

void DrawContour(float aContPntNow[], Tex3D *pTexVal, Tex2D &texContour, unsigned axis, unsigned w, unsigned h)
{
	glMatrixMode(GL_PROJECTION);	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);		glLoadIdentity();
	gluOrtho2D(0, w-1, 0, h-1);

	double aMProj[16];
	double aMView[16];
    int aVPort[4];
	double aObjPntNow[3];
    glGetDoublev(GL_PROJECTION_MATRIX, aMProj);
    glGetDoublev(GL_MODELVIEW_MATRIX, aMView);
    glGetIntegerv(GL_VIEWPORT, aVPort);
	gluUnProject(aContPntNow[0], aContPntNow[1], 0, aMView, aMProj, aVPort, aObjPntNow, aObjPntNow+1, aObjPntNow+2);

	Vect3D<unsigned> dimTexVal = pTexVal->GetDim();
	unsigned xDim, yDim;
	if(axis == 0)
	{
		xDim = dimTexVal.m_y;
		yDim = dimTexVal.m_z;
	}
	else if(axis == 1)
	{
		xDim = dimTexVal.m_x;
		yDim = dimTexVal.m_z;
	}
	else if(axis == 2)
	{
		xDim = dimTexVal.m_x;
		yDim = dimTexVal.m_y;
	}
	else
	{
		assert(0);
	}

	float aOutPosNow[2];
	float aInPosNow[] = {(float)aObjPntNow[0], (float)aObjPntNow[1]};
	float aLowCoord[] = {0, 0};
	pTexVal->UnprojOrthSlice2D(aOutPosNow, aInPosNow, aLowCoord, (float)w, (float)h, axis, true);

	int x = (int)(aOutPosNow[0] + 0.5f);
	int y = (int)(aOutPosNow[1] + 0.5f);
	if(x>=0 && x<=(int)xDim-1 &&
	   y>=0 && y<=(int)yDim-1)
	{
		texContour.SetCell(1.f, x, y, 0);
		texContour.SetCell(0,   x, y, 1);
		texContour.SetCell(0,   x, y, 2);
		texContour.SetCell(1.f, x, y, 3);
		texContour.Update();
	}
	else {}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	//float aLowCoord[] = {0, 0};
	texContour.DrawRec2D(aLowCoord, (float)w, (float)h, false);
	glDisable(GL_BLEND);
}

//***********************************************

void DrawCrossSection(Tex3D *pTexVal, unsigned xSliceNo, unsigned ySliceNo, unsigned zSliceNo)
{
	glMatrixMode(GL_PROJECTION);	glLoadIdentity();
	glOrtho(-0.75F, 0.75F, -0.75F, 0.75F, 0, 3.F);

	glMatrixMode(GL_MODELVIEW);		glLoadIdentity();
	gluLookAt(0.F, 0.F, 0.F, 0.5F, 0.5F, 0.5F, 1.F, 1.F, 0);

	unsigned aSliceNo[] = {xSliceNo, ySliceNo, zSliceNo};
	//glColor4f(1.f, 1.f, 1.f, 1.f);
	glColor4f(0.4f, 0.3f, 0.2f, 1.f);
	glEnable(GL_DEPTH_TEST);
	for(unsigned i=0; i<3; i++)
	{
		pTexVal->DrawOrthSlice3D(aSliceNo[i], i);
		pTexVal->DrawOrthSlice3DBoundary(aSliceNo[i], i);
	}
}

//***********************************************

void DrawBotSelecter(Selecter &sel, unsigned w, unsigned h, bool bColor, bool bEdt)
{
	glMatrixMode(GL_PROJECTION);	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);		glLoadIdentity();
	gluOrtho2D(0, w-1, 0, h-1);

	float aaV[4][2];
	sel.GetCtrlP(aaV);

	if(bColor)
	{
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		float aColorFr[4];
		float aColorTo[4];
		sel.GetColorFr(aColorFr);
		sel.GetColorTo(aColorTo);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBegin(GL_QUADS);
			glColor4fv(aColorFr);
			glVertex2fv(aaV[0]);
			glColor4fv(aColorFr);
			glVertex2fv(aaV[1]);
			glColor4fv(aColorTo);
			glVertex2fv(aaV[2]);
			glColor4fv(aColorTo);
			glVertex2fv(aaV[3]);
		glEnd();
	}
	else {}
	glDisable(GL_BLEND);

	glColor4f(0, 0.4f, 0.2f, 1.f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
		glVertex2fv(aaV[0]);
		glVertex2fv(aaV[1]);
		glVertex2fv(aaV[2]);
		glVertex2fv(aaV[3]);
	glEnd();
	if(bEdt)
	{
		glBegin(GL_LINES);
			glVertex2f(aaV[0][0], 0);
			glVertex2f(aaV[3][0], h-1.f);
			glVertex2f(aaV[1][0], 0);
			glVertex2f(aaV[2][0], h-1.f);
		glEnd();
	}
	else {}

	DATA xCMin[4], xCMax[4], yCMin[4], yCMax[4];
	sel.GetCtrlBBoxP0(xCMin[0], xCMax[0], yCMin[0], yCMax[0]);
	sel.GetCtrlBBoxP1(xCMin[1], xCMax[1], yCMin[1], yCMax[1]);
	sel.GetCtrlBBoxP2(xCMin[2], xCMax[2], yCMin[2], yCMax[2]);
	sel.GetCtrlBBoxP3(xCMin[3], xCMax[3], yCMin[3], yCMax[3]);

	float aaaCV[4][4][2];
	for(unsigned i=0; i<4; i++)
	{
		aaaCV[i][0][0] = (float)xCMin[i];	aaaCV[i][1][0] = (float)xCMax[i];	aaaCV[i][2][0] = (float)xCMax[i];	aaaCV[i][3][0] = (float)xCMin[i];
		aaaCV[i][0][1] = (float)yCMin[i];	aaaCV[i][1][1] = (float)yCMin[i];	aaaCV[i][2][1] = (float)yCMax[i];	aaaCV[i][3][1] = (float)yCMax[i];	
	}

	for(unsigned i=0; i<4; i++)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBegin(GL_QUADS);
			glVertex2fv(aaaCV[i][0]);
			glVertex2fv(aaaCV[i][1]);
			glVertex2fv(aaaCV[i][2]);
			glVertex2fv(aaaCV[i][3]);
		glEnd();
	}
}

void DrawTopSelecter(Selecter &sel, unsigned w, unsigned h)
{
	glMatrixMode(GL_PROJECTION);	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);		glLoadIdentity();
	gluOrtho2D(0, w-1, 0, h-1);

	float aaV[4][2];
	sel.GetCtrlP(aaV);

	glColor4f(0, 0.4f, 0.2f, 1.f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glBegin(GL_LINES);
	glBegin(GL_QUADS);
		glVertex2fv(aaV[0]);
		glVertex2fv(aaV[1]);
		glVertex2fv(aaV[2]);
		glVertex2fv(aaV[3]);
		//glVertex2f(aaV[0][0], 0);
		//glVertex2f(aaV[3][0], h-1.f);
		//glVertex2f(aaV[1][0], 0);
		//glVertex2f(aaV[2][0], h-1.f);
	glEnd();

	DATA xCMin[4], xCMax[4], yCMin[4], yCMax[4];
	sel.GetCtrlBBoxP0(xCMin[0], xCMax[0], yCMin[0], yCMax[0]);
	sel.GetCtrlBBoxP1(xCMin[1], xCMax[1], yCMin[1], yCMax[1]);
	sel.GetCtrlBBoxP2(xCMin[2], xCMax[2], yCMin[2], yCMax[2]);
	sel.GetCtrlBBoxP3(xCMin[3], xCMax[3], yCMin[3], yCMax[3]);

	float aaaCV[4][4][2];
	for(unsigned i=0; i<4; i++)
	{
		aaaCV[i][0][0] = (float)xCMin[i];	aaaCV[i][1][0] = (float)xCMax[i];	aaaCV[i][2][0] = (float)xCMax[i];	aaaCV[i][3][0] = (float)xCMin[i];
		aaaCV[i][0][1] = (float)yCMin[i];	aaaCV[i][1][1] = (float)yCMin[i];	aaaCV[i][2][1] = (float)yCMax[i];	aaaCV[i][3][1] = (float)yCMax[i];	
	}

	for(unsigned i=0; i<4; i++)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBegin(GL_QUADS);
			glVertex2fv(aaaCV[i][0]);
			glVertex2fv(aaaCV[i][1]);
			glVertex2fv(aaaCV[i][2]);
			glVertex2fv(aaaCV[i][3]);
		glEnd();
	}
}

//***********************************************

void DrawRedLevel(float redLevel, unsigned w, unsigned h)
{
	glMatrixMode(GL_PROJECTION);	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);		glLoadIdentity();
	gluOrtho2D(0, w-1, 0, h-1);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	glColor4f(1.f, 0, 0, 1.f);
	glBegin(GL_LINES);
		glVertex2f(0, redLevel);
		glVertex2f(w-1.f, redLevel);
	glEnd();
}