#include "myWinApp.h"

bool bSetTar = 0;

//**********************

Cluster *pCluster = 0;
Layer *pLyrDev = 0;
extern vector<Selecter> vSelTop;
extern vector<Selecter> vSelBot;

bool bEdtSel = false;
int selEdtNo = -1;
Vect2D<DATA> anchor(0, 0);

DATA reduceLevel = 0.05F;

//***********************************************

Tex3D *pTexVal = 0;
Tex2D *apTexContour[] = {0, 0, 0, 0, 0, 0}; 
Tex2D *pTexLleTop = 0;
Tex2D *pTexLleBot = 0;

int xSliceNo = 0;
int ySliceNo = 0;
int zSliceNo = 0;

//***********************************************

class MY_WIN
{
public:
	unsigned m_lowX, m_lowY;
	unsigned m_highX, m_highY;

	bool IsInside(float xIn, float yIn);
};
// 0 | 3 4
// 5 | 1 2 
MY_WIN aWin[6];
MY_WIN aVport[6];

bool aBClick[] = {false, false, false, false, false, false};
float aContPntNow[2];

//*************************************************************************************************

bool MY_WIN::IsInside(float xIn, float yIn)
{
	if(xIn>=m_lowX && xIn<=m_highX &&
	   yIn>=m_lowY && yIn<=m_highY)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//*************************************************************************************************
// target
//*************************************************************************************************

void SetTarget(Fl_Widget *w, void *v)
{
	Fl_Check_Button *pSetTarBttn = (Fl_Check_Button*)w;
	bSetTar = (pSetTarBttn->value()!=0)? true: false;
	pRender_glWin->redraw();
}

void DisplayTarget(Fl_Widget *w, void *v)
{
	Vect3D<unsigned> dimVal = pTexVal->GetDim();
	Vect2D<unsigned> dimLle = pTexLleTop->GetDim();
	for(unsigned i=1; i<=3; i++)
	{
		Tex2D *pContour = apTexContour[i];
		Vect2D<unsigned> dimContour = pContour->GetDim();
		for(unsigned y=0; y<dimContour.m_y; y++)
		{
			for(unsigned x=0; x<dimContour.m_x; x++)
			{
				float val = pContour->GetCell(x, y, 3);
				if(val != 0)
				{
					unsigned xPos, yPos, zPos;
					if(i == 1)		{xPos = x;			yPos = ySliceNo;	zPos = dimVal.m_z-1-y;}
					else if(i == 2)	{xPos = xSliceNo;	yPos = x;			zPos = dimVal.m_z-1-y;}
					else if(i == 3)	{xPos = x;			yPos = y;			zPos = zSliceNo;}
					else {}

					unsigned llePos = (zPos*dimVal.m_y + yPos) * dimVal.m_x + xPos;
					DATA xLle, yLle, zLle;
					pCluster->GetLoc(xLle, yLle, zLle, llePos);

					DATA xMin, xMax, yMin, yMax, zMin, zMax;
					pCluster->GetBBox(xMin, xMax, yMin, yMax, zMin, zMax);
					DATA xLen = xMax - xMin;	DATA xRatio = (dimLle.m_x-1) / xLen;
					DATA yLen = yMax - yMin;	DATA yRatio = (dimLle.m_y-1) / yLen;
					DATA zLen = zMax - zMin;	DATA zRatio = (dimLle.m_y-1) / zLen;
					unsigned xxPos = (unsigned)((xLle - xMin) * xRatio + 0.5F);
					unsigned yyPos = (unsigned)((yLle - yMin) * yRatio + 0.5F);
					unsigned zzPos = (unsigned)((zLle - zMin) * zRatio + 0.5F);
					
					apTexContour[0]->SetCell(1.f,	xxPos, yyPos, 0);
					apTexContour[0]->SetCell(0,		xxPos, yyPos, 1);
					apTexContour[0]->SetCell(0,		xxPos, yyPos, 2);
					apTexContour[0]->SetCell(1.f,	xxPos, yyPos, 3);

					apTexContour[5]->SetCell(1.f,	xxPos, zzPos, 0);
					apTexContour[5]->SetCell(0,		xxPos, zzPos, 1);
					apTexContour[5]->SetCell(0,		xxPos, zzPos, 2);
					apTexContour[5]->SetCell(1.f,	xxPos, zzPos, 3);
				}
				else {}
			}
		}
	}
	apTexContour[0]->Update();
	apTexContour[5]->Update();
	pRender_glWin->redraw();
}

void ClearTarget(Fl_Widget *w, void *v)
{
	for(unsigned i=0; i<6; i++)
	{
		aBClick[i] = false;
	}

	for(unsigned i=0; i<6; i++)
	{
		if(!apTexContour[i])
		{
			continue;
		}
		else {}

		Vect2D<unsigned> dimCont = apTexContour[i]->GetDim();
		for(unsigned y=0; y<dimCont.m_y; y++)
		{
			for(unsigned x=0; x<dimCont.m_x; x++)
			{
				for(unsigned c=0; c<4; c++)
				{
					apTexContour[i]->SetCell(0, x, y, c);
				}
			}
		}
		apTexContour[i]->Update();
	}

	pRender_glWin->redraw();
}

//*************************************************************************************************
// selecter
//*************************************************************************************************

void AddSelecter(Fl_Widget *w, void *v)
{
	if(!pCluster)
	{
		return;
	}
	else {}

	vSelTop.push_back(Selecter());
	vSelBot.push_back(Selecter());
	selEdtNo = (int)vSelTop.size() - 1;

	string cStr;
	stringstream ss;

	DATA aColor[4];
	vSelTop[selEdtNo].GetColorFr(aColor);
	for(unsigned i=0; i<4; i++)
	{
		ss.clear();
		ss << aColor[i];
		ss >> cStr;
		Fl_Input *pCInput = (Fl_Input*)pColorFr_group->child(i);
		pCInput->value(cStr.c_str());
	}

	vSelTop[selEdtNo].GetColorTo(aColor);
	for(unsigned i=0; i<4; i++)
	{
		ss.clear();
		ss << aColor[i];
		ss >> cStr;
		Fl_Input *pCInput = (Fl_Input*)pColorTo_group->child(i);
		pCInput->value(cStr.c_str());
	}


	pRender_glWin->redraw();
}

void EditSelecter(Fl_Widget *w, void *v)
{
	if(!pCluster)
	{
		return;
	}
	else {}

	Fl_Check_Button *pChBttn = (Fl_Check_Button*)w;
	bEdtSel = (pChBttn->value()!=0)? true: false;

	pRender_glWin->redraw();
}

void CopyColor(Fl_Widget *w, void *v)
{
	if(!bEdtSel)
	{
		return;
	}
	else {}

	string cStr;
	stringstream ss;

	//DATA aColor[4];
	for(unsigned i=0; i<4; i++)
	{
		Fl_Input *pCInput = (Fl_Input*)pColorFr_group->child(i);
		cStr = pCInput->value();

		Fl_Input *pCInput2 = (Fl_Input*)pColorTo_group->child(i);
		pCInput2->value(cStr.c_str());
	}
}

void SetColor(Fl_Widget *w, void *v)
{
	if(!bEdtSel)
	{
		return;
	}
	else {}

	string cStr;
	stringstream ss;

	DATA aColor[4];
	for(unsigned i=0; i<4; i++)
	{
		Fl_Input *pCInput = (Fl_Input*)pColorFr_group->child(i);
		cStr = pCInput->value();
		
		ss.clear();
		ss << cStr;
		ss >> aColor[i];
	}
	vSelTop[selEdtNo].SetColorFr(aColor);
	vSelBot[selEdtNo].SetColorFr(aColor);

	for(unsigned i=0; i<4; i++)
	{
		Fl_Input *pCInput = (Fl_Input*)pColorTo_group->child(i);
		cStr = pCInput->value();
		
		ss.clear();
		ss << cStr;
		ss >> aColor[i];
	}
	vSelTop[selEdtNo].SetColorTo(aColor);
	vSelBot[selEdtNo].SetColorTo(aColor);

	pRender_glWin->redraw();
}

void Truncate(Fl_Widget *w, void *v)
{
	if(vSelBot.size() == 0)
	{
		return;
	}
	else {}

	Vect2D<unsigned> dimLle = pTexLleBot->GetDim();

	DATA xMin, xMax, yMin, yMax, zMin, zMax;
	pCluster->GetBBox(xMin, xMax, yMin, yMax, zMin, zMax);
	DATA xLen = xMax - xMin;
	DATA yLen = yMax - yMin;
	DATA zLen = zMax - zMin;
	DATA xRatio = (dimLle.m_x-1) / xLen;
	DATA yRatio = (dimLle.m_y-1) / yLen;
	DATA zRatio = (dimLle.m_y-1) / zLen;

	for(unsigned y=0; y<dimLle.m_y; y++)
	{
		for(unsigned x=0; x<dimLle.m_x; x++)
		{
			pTexLleTop->SetCell(0, x, y, 0);
		}
	}

	float maxCount = -1e10;
	unsigned dataNum = pCluster->GetNum();
	for(unsigned i=0; i<dataNum; i++)
	{
		DATA xx, yy, zz;
		pCluster->GetLoc(xx, yy, zz, i);

		DATA xIn = (xx-xMin) * xRatio;
		DATA zIn = (zz-zMin) * zRatio;

		if( !vSelBot[selEdtNo].IsInside(xIn, zIn) )
		{
			continue;
		}
		else {}

		unsigned xOut = (unsigned)(xIn + 0.5F);
		unsigned yOut = (unsigned)((yy-yMin)*yRatio + 0.5F);
		float count = pTexLleTop->GetCell(xOut, yOut, 0);
		count = count + 1.f;
		pTexLleTop->SetCell(count, xOut, yOut, 0);

		if(count > maxCount)
		{
			maxCount = count;
		}
		else {}
	}

	for(unsigned y=0; y<dimLle.m_y; y++)
	{
		for(unsigned x=0; x<dimLle.m_x; x++)
		{
			float count = pTexLleTop->GetCell(x, y, 0);

			pTexLleTop->SetCell(log(count+1.f)/log(maxCount+1.f), x, y, 0);
		}
	}
	pTexLleTop->Update();
	pRender_glWin->redraw();
}

void ReduceLevel(Fl_Widget *w, void *v)
{
	reduceLevel = pLevelReduce_valScl->value();

	Vect2D<unsigned> dimLle = pTexLleBot->GetDim();

	DATA xMin, xMax, yMin, yMax, zMin, zMax;
	pCluster->GetBBox(xMin, xMax, yMin, yMax, zMin, zMax);
	DATA xLen = xMax - xMin;
	DATA yLen = yMax - yMin;
	DATA zLen = zMax - zMin;
	DATA xRatio = (dimLle.m_x-1) / xLen;
	DATA yRatio = (dimLle.m_y-1) / yLen;
	DATA zRatioNew = 1.F / zLen;

	for(unsigned y=0; y<dimLle.m_y; y++)
	{
		for(unsigned x=0; x<dimLle.m_x; x++)
		{
			pTexLleTop->SetCell(0, x, y, 0);
		}
	}

	float maxCount = -1e10;
	unsigned dataNum = pCluster->GetNum();
	for(unsigned i=0; i<dataNum; i++)
	{
		DATA xx, yy, zz;
		pCluster->GetLoc(xx, yy, zz, i);
		
		DATA zIn = (zz-zMin) * zRatioNew;
		if(zIn < reduceLevel)
		{
			continue;
		}
		else {}

		unsigned xOut = (unsigned)((xx-xMin)*xRatio + 0.5F);
		unsigned yOut = (unsigned)((yy-yMin)*yRatio + 0.5F);
		float count = pTexLleTop->GetCell(xOut, yOut, 0);
		count = count + 1.f;
		pTexLleTop->SetCell(count, xOut, yOut, 0);

		if(count > maxCount)
		{
			maxCount = count;
		}
		else {}
	}

	for(unsigned y=0; y<dimLle.m_y; y++)
	{
		for(unsigned x=0; x<dimLle.m_x; x++)
		{
			float count = pTexLleTop->GetCell(x, y, 0);

			pTexLleTop->SetCell(log(count+1.f)/log(maxCount+1.f), x, y, 0);
		}
	}
	pTexLleTop->Update();
	pRender_glWin->redraw();
}

//*************************************************************************************************
// interaction
//*************************************************************************************************

MyGlWindow::MyGlWindow(int x, int y, int w, int h, const char *l)
:Fl_Gl_Window(x, y, w, h, l)
{}

int MyGlWindow::handle(int evnt)
{	
	int xIn = Fl::event_x();
	int yIn = h()-1 - Fl::event_y();

	if(evnt == FL_PUSH)
	{
		int buttn = Fl::event_button();
		if(buttn == 1)
		{
			if(bSetTar)
			{
				for(unsigned i=0; i<6; i++)
				{
					aBClick[i] = false;
				}
				for(unsigned i=1; i<=3; i++)
				{
					if(aWin[i].IsInside((float)xIn, (float)yIn))
					{
						aBClick[i] = true;
						aContPntNow[0] = (float)xIn;		
						aContPntNow[1] = (float)yIn;
						redraw();
						return 1;
					}
					else {}
				}
			}
			else if(bEdtSel)
			{
				for(unsigned i=0; i<6; i++)
				{
					aBClick[i] = false;
				}

				for(unsigned i=0; i<=5; i+=5)
				{
					if(!aWin[i].IsInside((float)xIn, (float)yIn))
					{
						continue;
					}
					else {}

					aBClick[i] = true;
					Vect2D<unsigned>dimLle = apTexContour[i]->GetDim();
					DATA selX = (xIn-aVport[i].m_lowX) *( dimLle.m_x-1) / (aVport[i].m_highX-aVport[i].m_lowX);
					DATA selY = (yIn-aVport[i].m_lowY) * (dimLle.m_y-1) / (aVport[i].m_highY-aVport[i].m_lowY);

					if(i==0 && selEdtNo<0)
					{
						continue;
					}
					else {}
					unsigned vStr = (i==5)? 0: selEdtNo;
					unsigned vEnd = (i==5)? vSelTop.size()-1: selEdtNo;
					selEdtNo = -1;
					for(unsigned j=vStr; j<=vEnd; j++)
					//for(unsigned j=0; j<vSelTop.size(); j++)
					{
						if(i == 0)
						{
							if(! (vSelTop[j].IsInside(selX, selY) || vSelTop[j].IsInCtrlP0(selX, selY) ||
								vSelTop[j].IsInCtrlP1(selX, selY) || vSelTop[j].IsInCtrlP2(selX, selY) ||
								vSelTop[j].IsInCtrlP3(selX, selY)) )
							{
								continue;
							}
							else {}
						}
						else if(i == 5)
						{
							if(! (vSelBot[j].IsInside(selX, selY) || vSelBot[j].IsInCtrlP0(selX, selY) ||
								vSelBot[j].IsInCtrlP1(selX, selY) || vSelBot[j].IsInCtrlP2(selX, selY) ||
								vSelBot[j].IsInCtrlP3(selX, selY)) )
							{
								continue;
							}
							else {}
						}
						else {}

						if(i == 0)
						{
							if(vSelTop[j].IsInCtrlP0(selX, selY))			{vSelTop[j].SetCtrlNo(0);}
							else if(vSelTop[j].IsInCtrlP1(selX, selY))		{vSelTop[j].SetCtrlNo(1);}
							else if(vSelTop[j].IsInCtrlP2(selX, selY))		{vSelTop[j].SetCtrlNo(2);}
							else if(vSelTop[j].IsInCtrlP3(selX, selY))		{vSelTop[j].SetCtrlNo(3);}
							else {vSelTop[j].SetCtrlNo(-1);}
						}
						else if(i == 5)
						{
							if(vSelBot[j].IsInCtrlP0(selX, selY))			{vSelBot[j].SetCtrlNo(0);}
							else if(vSelBot[j].IsInCtrlP1(selX, selY))		{vSelBot[j].SetCtrlNo(1);}
							else if(vSelBot[j].IsInCtrlP2(selX, selY))		{vSelBot[j].SetCtrlNo(2);}
							else if(vSelBot[j].IsInCtrlP3(selX, selY))		{vSelBot[j].SetCtrlNo(3);}
							else {vSelBot[j].SetCtrlNo(-1);}
						}
						else {}

						selEdtNo = j;
						anchor.m_x = xIn;
						anchor.m_y = yIn;

						stringstream ss;
						string cStr;
						DATA aColor[4];
						vSelTop[selEdtNo].GetColorFr(aColor);
						for(unsigned j=0; j<4; j++)
						{
							ss.clear();
							ss << aColor[j];
							ss >> cStr;
							Fl_Input *pCInput = (Fl_Input*)pColorFr_group->child(j);
							pCInput->value(cStr.c_str());	
						}
						vSelTop[selEdtNo].GetColorTo(aColor);
						for(unsigned j=0; j<4; j++)
						{
							ss.clear();
							ss << aColor[j];
							ss >> cStr;
							Fl_Input *pCInput = (Fl_Input*)pColorTo_group->child(j);
							pCInput->value(cStr.c_str());	
						}
						redraw();
						return 1;
					}
				}
				return 0;
			}
			else {}
		}
		else {} // buttn
	}
	else if(evnt == FL_DRAG)
	{
		int buttn = Fl::event_button();
		if(buttn == 1)
		{
			if(bSetTar)
			{
				for(unsigned i=1; i<=3; i++)
				{
					if(aWin[i].IsInside((float)xIn, (float)yIn))
					{
						if(!aBClick[i])
						{
							for(unsigned j=1; j<=3; j++)
							{
								aBClick[j] = false;
							}
							return 0;
						}
						else 
						{
							aContPntNow[0] = (float)xIn;		
							aContPntNow[1] = (float)yIn;
						}
						redraw();
						return 1;
					}
					else {}
				}
			}
			else if(bEdtSel)
			{
				for(unsigned i=0; i<6; i++)
				{
					aBClick[i] = false;
				}

				if(selEdtNo < 0)
				{
					return 0;
				}
				else {}

				for(unsigned i=0; i<=5; i+=5)
				{
					if(!aWin[i].IsInside((float)xIn, (float)yIn))
					{
						continue;
					}
					else {}

					Vect2D<unsigned>dimLle = apTexContour[i]->GetDim();
					DATA xDis = (xIn-anchor.m_x) * (dimLle.m_x-1) / (aVport[i].m_highX-aVport[i].m_lowX);
					DATA yDis = (yIn-anchor.m_y) * (dimLle.m_y-1) / (aVport[i].m_highY-aVport[i].m_lowY);
					anchor.m_x = xIn;
					anchor.m_y = yIn;
				
					if(i == 0)
					{
						int setCtrlNo = vSelTop[selEdtNo].GetSetCtrlNo();
						if(setCtrlNo == 0)		
						{
							vSelTop[selEdtNo].MoveP0(xDis, yDis);	vSelTop[selEdtNo].MoveP3(xDis, 0);
							vSelBot[selEdtNo].MoveP0(xDis, 0);		vSelBot[selEdtNo].MoveP3(xDis, 0);
							vSelTop[selEdtNo].MoveP1(0, yDis);
						}
						else if(setCtrlNo == 1)	
						{
							vSelTop[selEdtNo].MoveP1(xDis, yDis);	vSelTop[selEdtNo].MoveP2(xDis, 0);
							vSelBot[selEdtNo].MoveP1(xDis, 0);		vSelBot[selEdtNo].MoveP2(xDis, 0);
							vSelTop[selEdtNo].MoveP0(0, yDis);
						}
						else if(setCtrlNo == 2) 
						{
							vSelTop[selEdtNo].MoveP2(xDis, yDis);	vSelTop[selEdtNo].MoveP1(xDis, 0);
							vSelBot[selEdtNo].MoveP2(xDis, 0);		vSelBot[selEdtNo].MoveP1(xDis, 0);
							vSelTop[selEdtNo].MoveP3(0, yDis);
						}
						else if(setCtrlNo == 3) 
						{
							vSelTop[selEdtNo].MoveP3(xDis, yDis);	vSelTop[selEdtNo].MoveP0(xDis, 0);
							vSelBot[selEdtNo].MoveP3(xDis, 0);		vSelBot[selEdtNo].MoveP0(xDis, 0);
							vSelTop[selEdtNo].MoveP2(0, yDis);
						}
						else
						{
							vSelTop[selEdtNo].MoveOrgX(xDis);
							vSelTop[selEdtNo].MoveOrgY(yDis);							
							vSelBot[selEdtNo].MoveOrgX(xDis);
						}
					}
					else
					{
						int setCtrlNo = vSelBot[selEdtNo].GetSetCtrlNo();
						if(setCtrlNo == 0)		
						{
							vSelBot[selEdtNo].MoveP0(xDis, yDis);	vSelBot[selEdtNo].MoveP3(xDis, 0);
							vSelTop[selEdtNo].MoveP0(xDis, 0);		vSelTop[selEdtNo].MoveP3(xDis, 0);
							vSelBot[selEdtNo].MoveP1(0, yDis);
						}
						else if(setCtrlNo == 1)	
						{
							vSelBot[selEdtNo].MoveP1(xDis, yDis);	vSelBot[selEdtNo].MoveP2(xDis, 0);
							vSelTop[selEdtNo].MoveP1(xDis, 0);		vSelTop[selEdtNo].MoveP2(xDis, 0);
							vSelBot[selEdtNo].MoveP0(0, yDis);
						}
						else if(setCtrlNo == 2) 
						{
							vSelBot[selEdtNo].MoveP2(xDis, yDis);	vSelBot[selEdtNo].MoveP1(xDis, 0);
							vSelTop[selEdtNo].MoveP2(xDis, 0);		vSelTop[selEdtNo].MoveP1(xDis, 0);
							vSelBot[selEdtNo].MoveP3(0, yDis);
						}
						else if(setCtrlNo == 3) 
						{
							vSelBot[selEdtNo].MoveP3(xDis, yDis);	vSelBot[selEdtNo].MoveP0(xDis, 0);
							vSelTop[selEdtNo].MoveP3(xDis, 0);		vSelTop[selEdtNo].MoveP0(xDis, 0);
							vSelBot[selEdtNo].MoveP2(0, yDis);
						}
						else
						{
							vSelBot[selEdtNo].MoveOrgX(xDis);
							vSelBot[selEdtNo].MoveOrgY(yDis);
							vSelTop[selEdtNo].MoveOrgX(xDis);
						}
					}
					redraw();
					return 1;
				}
				return 0;
			}
		}
	}
	else if(evnt == FL_ENTER)
	{
		if(bEdtSel)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if(evnt == FL_MOVE)
	{
		if(bEdtSel)
		{
			if(selEdtNo == -1)
			{
				return 1;
			}
			else {}

			int xIn = Fl::event_x();
			int yIn = h()-1 - Fl::event_y();

			GLdouble mView[16];
			GLdouble proj[16];
			GLint vPort[4];
			glGetDoublev(GL_MODELVIEW_MATRIX, mView);
			glGetDoublev(GL_PROJECTION_MATRIX, proj);
			glGetIntegerv(GL_VIEWPORT, vPort);
			GLdouble sel_x, sel_y, sel_z;
			gluUnProject((GLdouble)xIn, (GLdouble)yIn, 0, mView, proj, vPort, &sel_x, &sel_y, &sel_z);
			
			Selecter &sel = vSelTop[selEdtNo];
			//sel.SetLoc(sel_x, sel_y);

			redraw();
			return 1;
		}
		else {}
	}
	else if(evnt == FL_RELEASE)
	{}
	else if(evnt == FL_MOUSEWHEEL)
	{
		int xIn = Fl::event_x();
		int yIn = h()-1 - Fl::event_y();

		int *apSliceNo[] = {0, &ySliceNo, &xSliceNo, &zSliceNo};
		for(unsigned i=1; i<=3; i++)
		{
			if(aWin[i].IsInside((float)xIn, (float)yIn))
			{
				if(Fl::event_dy() > 0)
				{
					(*apSliceNo[i])++;
				
				}
				else
				{
					(*apSliceNo[i])--;
				}
				redraw();
			}
		}
		return 1;
	}
	else
	{
		return Fl_Widget::handle(evnt);
	}
	return 0;
}

//*************************************************************************************************
// draw
//*************************************************************************************************

void DivideSubWin(MY_WIN aMyWin[], unsigned w, unsigned h)
{
	DATA xR = 0.53F;

	aMyWin[0].m_lowX = 0;												aMyWin[0].m_lowY = 0;
	aMyWin[0].m_highX = (unsigned)((w-1) * xR);							aMyWin[0].m_highY = h - 1;

	aMyWin[1].m_lowX = aMyWin[0].m_highX + 1;							aMyWin[1].m_lowY = 0;
	aMyWin[1].m_highX = (unsigned)((w-1 + aMyWin[0].m_highX) * 0.5);	aMyWin[1].m_highY = (unsigned)((h-1) * 0.5);

	aMyWin[2].m_lowX = aMyWin[1].m_highX + 1;							aMyWin[2].m_lowY = 0;
	aMyWin[2].m_highX = w - 1;											aMyWin[2].m_highY = aMyWin[1].m_highY;

	aMyWin[3].m_lowX = aMyWin[1].m_lowX;								aMyWin[3].m_lowY = aMyWin[1].m_highY + 1;
	aMyWin[3].m_highX = aMyWin[1].m_highX;								aMyWin[3].m_highY = h - 1;

	aMyWin[4].m_lowX = aMyWin[2].m_lowX;								aMyWin[4].m_lowY = aMyWin[2].m_highY + 1;
	aMyWin[4].m_highX = w - 1;											aMyWin[4].m_highY = h - 1;

	aMyWin[5].m_lowX = aMyWin[0].m_lowX;								aMyWin[5].m_lowY = aMyWin[0].m_lowY;
	aMyWin[5].m_highX = aMyWin[0].m_highX;								aMyWin[5].m_highY = (aMyWin[0].m_lowY + aMyWin[0].m_highY) / 2;
	aMyWin[0].m_lowY = aMyWin[5].m_highY + 1;
}

void SetViewport(MY_WIN aMyVport[], MY_WIN aMyWin[])
{
	for(unsigned i=0; i<6; i++)
	{
		if(i == 4)
		{
			continue;
		}
		else {}

		unsigned xLen = aMyWin[i].m_highX - aMyWin[i].m_lowX + 1;
		unsigned yLen = aMyWin[i].m_highY - aMyWin[i].m_lowY + 1;
		float aLowIn[] = {(float)aMyWin[i].m_lowX, (float)aMyWin[i].m_lowY}; 
		float aLenIn[] = {(float)xLen, (float)yLen};

		float aLowNew[2];	float aHighNew[2];
		apTexContour[i]->FindDrawRange(aLowNew, aHighNew, aLowIn, aLenIn[0], aLenIn[1]);

		aMyVport[i].m_lowX = (int)(aLowNew[0] + 0.5f);		aMyVport[i].m_highX = (int)(aHighNew[0] + 0.5f);
		aMyVport[i].m_lowY = (int)(aLowNew[1] + 0.5f);		aMyVport[i].m_highY = (int)(aHighNew[1] + 0.5f);
	}
}

void MyGlWindow::draw()
{
	DATA vScl = 1.1F;

	static bool bInit = false;
	if(!bInit)
	{
		bInit = true;
		glewInit();

		DivideSubWin(aWin, w(), h());
	}
	else {}

	//glClearColor(0.25f, 0.f, 0.25f, 0);
	glClearColor(0, 0.2f, 0.4f, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(!pCluster)
	{
		for(unsigned i=0; i<6; i++)
		{
			unsigned xLen = aWin[i].m_highX - aWin[i].m_lowX + 1;
			unsigned yLen = aWin[i].m_highY - aWin[i].m_lowY + 1;
			glViewport(aWin[i].m_lowX, aWin[i].m_lowY, xLen, yLen);

			glDisable(GL_DEPTH_TEST);
			DrawWin();
		}
		glFlush();
		return;
	}
	else {}

	//*******************************************

	Tex2D *apTexLle[] = {pTexLleTop, 0, 0, 0, 0, pTexLleBot};
	for(unsigned i=0; i<=5; i+=5)
	{
		unsigned xLen = aVport[i].m_highX - aVport[i].m_lowX + 1;
		unsigned yLen = aVport[i].m_highY - aVport[i].m_lowY + 1;
		glViewport(aVport[i].m_lowX, aVport[i].m_lowY, xLen, yLen);

		Vect2D<unsigned> dim = apTexContour[i]->GetDim();
		DrawLle(apTexLle[i], dim.m_x, dim.m_y);
	}

	//*******************************************

	unsigned aAxis[] = {0, 1, 0, 2};
	unsigned aSliceNo[] = {0, ySliceNo, xSliceNo, zSliceNo};
	for(unsigned i=1; i<=3; i++)
	{
		unsigned xLen = aVport[i].m_highX - aVport[i].m_lowX + 1;
		unsigned yLen = aVport[i].m_highY - aVport[i].m_lowY + 1;
		glViewport(aVport[i].m_lowX, aVport[i].m_lowY, xLen, yLen);

		Vect2D<unsigned> dim = apTexContour[i]->GetDim();
		DrawSlice(pTexVal, aSliceNo[i], aAxis[i], dim.m_x, dim.m_y);
	}

	for(unsigned i=4; i<=4; i++)
	{
		unsigned xLen = aWin[i].m_highX - aWin[i].m_lowX + 1;
		unsigned yLen = aWin[i].m_highY - aWin[i].m_lowY + 1;
		glViewport(aWin[i].m_lowX, aWin[i].m_lowY, xLen, yLen);
		DrawCrossSection(pTexVal, xSliceNo, ySliceNo, zSliceNo);
	}

	//*******************************************

	for(unsigned i=0; i<=5; i+=5)
	{
		unsigned xLen = aVport[i].m_highX - aVport[i].m_lowX + 1;
		unsigned yLen = aVport[i].m_highY - aVport[i].m_lowY + 1;
		glViewport(aVport[i].m_lowX, aVport[i].m_lowY, xLen, yLen);

		Vect2D<unsigned> dim = apTexContour[i]->GetDim();
		MapContour(*apTexContour[i], xLen, yLen);	
	}

	for(unsigned i=1; i<=3; i++)
	{
		unsigned xLen = aVport[i].m_highX - aVport[i].m_lowX + 1;
		unsigned yLen = aVport[i].m_highY - aVport[i].m_lowY + 1;
		glViewport(aVport[i].m_lowX, aVport[i].m_lowY, xLen, yLen);

		Vect2D<unsigned> dim = apTexContour[i]->GetDim();
		DrawSlice(pTexVal, aSliceNo[i], aAxis[i], dim.m_x, dim.m_y);
		if(aBClick[i])
		{
			DrawContour(aContPntNow, pTexVal, *apTexContour[i], aAxis[i], dim.m_x, dim.m_y);
		}
		else 
		{
			MapContour(*apTexContour[i], dim.m_x, dim.m_y);
		}
	}

	//*******************************************

	/*
	for(unsigned i=0; i<=0; i++)
	{
		if(selEdtNo < 0)
		{
			continue;
		}
		else {}

		unsigned xLen = aVport[i].m_highX - aVport[i].m_lowX + 1;
		unsigned yLen = aVport[i].m_highY - aVport[i].m_lowY + 1;
		glViewport(aVport[i].m_lowX, aVport[i].m_lowY, xLen, yLen);

		vector<Selecter> &vSelDraw = (i==0)? vSelTop: vSelBot;
		Vect2D<unsigned> dim = apTexContour[i]->GetDim();
		DrawTopSelecter(vSelDraw[selEdtNo], dim.m_x, dim.m_y);
	}
	*/

	for(unsigned i=0; i<=5; i+=5)
	{
		unsigned xLen = aVport[i].m_highX - aVport[i].m_lowX + 1;
		unsigned yLen = aVport[i].m_highY - aVport[i].m_lowY + 1;
		glViewport(aVport[i].m_lowX, aVport[i].m_lowY, xLen, yLen);

		vector<Selecter> &vSelDraw = (i==0)? vSelTop: vSelBot;
		bool bColor = (i==0)? false: true;
		Vect2D<unsigned> dim = apTexContour[i]->GetDim();
		for(unsigned j=0; j<vSelTop.size(); j++)
		{
			bool bEdt = (j==selEdtNo)? true: false;
			DrawBotSelecter(vSelDraw[j], dim.m_x, dim.m_y, bColor, bEdt);
		}
	}

	//*******************************************

	for(unsigned i=5; i<=5; i++)
	{
		unsigned xLen = aVport[i].m_highX - aVport[i].m_lowX + 1;
		unsigned yLen = aVport[i].m_highY - aVport[i].m_lowY + 1;
		glViewport(aVport[i].m_lowX, aVport[i].m_lowY, xLen, yLen);

		Vect2D<unsigned> dim = apTexContour[i]->GetDim();
		DrawRedLevel((float)(reduceLevel*(dim.m_y-1)), dim.m_x, dim.m_y);
	}

	//*******************************************

	for(unsigned i=0; i<6; i++)
	{
		unsigned xLen = aWin[i].m_highX - aWin[i].m_lowX + 1;
		unsigned yLen = aWin[i].m_highY - aWin[i].m_lowY + 1;
		glViewport(aWin[i].m_lowX, aWin[i].m_lowY, xLen, yLen);
		glDisable(GL_DEPTH_TEST);
		DrawWin();
	}
	glFlush();
}

//*************************************************************************************************
// load data
//*************************************************************************************************

void LoadData(Fl_Widget *w, void *v)
{
	Fl_File_Chooser *pFChooser = new Fl_File_Chooser("./", "(*.{vol,mdsj,clst})", 
		Fl_File_Chooser::SINGLE, 0);
	pFChooser->show();
	while(pFChooser->shown())
	{
		Fl::wait();
	}
	if(!pFChooser->value())
	{
		return;
	}
	else {}

	string strAll = pFChooser->value();
	string strOrg = strAll.substr( strAll.find_last_of('/')+1 );
	string strExt = strOrg.substr( strOrg.find_last_of('.')+1 );
	string strName = strOrg.substr(0, strOrg.find_first_of('.'));

	delete pCluster;
	pCluster = new Cluster(strName);	
	vSelTop.clear();
	vSelBot.clear();

	if(!strExt.compare("vol"))
	{
		string funCall = "..\\myLle_weight\\Release\\myLle " + strAll;
		system(funCall.c_str());
	}
	else if(!strExt.compare("clst"))
	{
		ifstream ff(strAll);
		unsigned vS;
		ff.read((char*)&vS, sizeof(unsigned));

		for(unsigned i=0; i<vS; i++)
		{
			DATA aaVT[4][2];
			for(unsigned j=0; j<4; j++)
			{
				ff.read((char*)aaVT[j], sizeof(DATA)*2);
			}
			DATA aaVB[4][2];
			for(unsigned j=0; j<4; j++)
			{
				ff.read((char*)aaVB[j], sizeof(DATA)*2);
			}
			DATA aCFr[4];
			ff.read((char*)aCFr, sizeof(DATA)*4);
			DATA aCTo[4];
			ff.read((char*)aCTo, sizeof(DATA)*4);

			vSelTop.push_back( Selecter(aaVT, aCFr, aCTo) );
			vSelBot.push_back( Selecter(aaVB, aCFr, aCTo) );
		}
		ff.close();
	}
	else if(!strExt.compare("mdsj")) {}
	else
	{
		assert(0);
	}

	//*******************************************

	const VolumeData &rVolScl = pCluster->GetVolFileVal().GetSclVolVal();
	Vect3D<unsigned> dimData = rVolScl.GetDim();
	unsigned size = dimData.m_x * dimData.m_y * dimData.m_z;
	float *pData = new float[size];
	rVolScl.GetLyrVal().CopyTo_zLast(pData);
	
	delete pTexVal;
	pTexVal = new Tex3D(pData, dimData.m_x, dimData.m_y, dimData.m_z, 1, true);

	unsigned aXDim[] = {0, dimData.m_y, dimData.m_x, dimData.m_x, 0};
	unsigned aYDim[] = {0, dimData.m_z, dimData.m_z, dimData.m_y, 0};
	for(unsigned i=1; i<=3; i++)
	{
		delete apTexContour[i];
		apTexContour[i] = new Tex2D(aXDim[i], aYDim[i], 4, false, false);
	}

	//*******************************************

	unsigned texXDim = 512; //512;
	unsigned texYDim = 256; //300;

	float *pTexDataTop = pCluster->GenerateDistribution(texXDim, texYDim, 0, 1);
	float *pTexDataBot = pCluster->GenerateDistribution(texXDim, texYDim, 0, 2);

	unsigned texSize = texXDim * texYDim;
	float *apTexData[] = {pTexDataTop, pTexDataBot};
	for(unsigned no=0; no<2; no++)
	{
		float *pTexData = apTexData[no];
		float maxCount = -1e10;
		for(unsigned i=0; i<texSize; i++)
		{
			if(pTexData[i] > 0)
			{
				pTexData[i] = (float)log(pTexData[i]+1);
			}
			else 
			{
				pTexData[i] = 0;
			}
		
			if(pTexData[i] > maxCount)
			{
				maxCount = pTexData[i];
			}
			else {}		
		}
		for(unsigned i=0; i<texSize; i++)
		{
			pTexData[i] /= maxCount;
		}
	}

	delete pTexLleTop;		pTexLleTop = new Tex2D(pTexDataTop, texXDim, texYDim, 1);
	delete pTexLleBot;		pTexLleBot = new Tex2D(pTexDataBot, texXDim, texYDim, 1);

	delete apTexContour[0];		apTexContour[0] = new Tex2D(texXDim, texYDim, 4, false, false);
	delete apTexContour[5];		apTexContour[5] = new Tex2D(texXDim, texYDim, 4, false, false);

	SetViewport(aVport, aWin);

	MyGlWindow *pGlWin = (MyGlWindow*)v;
	pGlWin->redraw();
}

//*************************************************************************************************

void GenCluster(Fl_Widget *w, void *v)
{	
	Vect2D<unsigned> dimTop = pTexLleTop->GetDim();
	Vect2D<unsigned> dimBot = pTexLleBot->GetDim();
	unsigned dimMap = dimBot.m_y;

	float *pMap = new float[dimMap*4];	
	for(unsigned i=0; i<dimMap; i++)
	{
		for(unsigned j=0; j<4; j++)
		{
			pMap[i*4+j] = 0;
		}
	}

	float *pId = new float[dimMap];
	for(unsigned i=0; i<dimMap; i++)
	{
		pId[i] = 0;
	}

	for(unsigned i=0; i<vSelTop.size(); i++)
	{
		DATA aColorFr[4];
		DATA aColorTo[4];
		vSelBot[i].GetColorFr(aColorFr);
		vSelBot[i].GetColorTo(aColorTo);

		float aaV[4][2];
		vSelBot[i].GetCtrlP(aaV);
		unsigned zN = (unsigned)(aaV[0][1] + 0.5f);
		unsigned zF = (unsigned)(aaV[3][1] + 0.5f);
		for(unsigned z=zN; z<=zF; z++)
		{
			DATA fRatio = (DATA)(zF-z+1)/(zF-zN+1);
			for(unsigned j=0; j<4; j++)
			{
				pMap[z*4+j] = (float)myMath.Interpolate_linear(aColorFr[j], aColorTo[j], fRatio);
			}
			pId[z] = (i+1.f) * 0.1f;
		}
	}

	ofstream f(pCluster->GetFName()+".tf", ios::binary);
	f.write((char*)&dimMap, sizeof(unsigned));
	f.write((char*)pMap, sizeof(float)*dimMap*4);
	f.close();
	ofstream f2(pCluster->GetFName()+".id", ios::binary);
	f2.write((char*)&dimMap, sizeof(unsigned));
	f2.write((char*)pId, sizeof(float)*dimMap);
	f2.close();
	delete []pMap;
	delete []pId;

	//*******************************************

	const unsigned CLST_NUM = 10;

	//Vect3D<unsigned> dimOrg = pCluster->GetVolFileVal().GetOrgDim();
	Vect3D<unsigned> dimScl = pCluster->GetVolFileVal().GetSclDim();
	Layer **ppLyrMsk = new Layer*[CLST_NUM];
	for(unsigned i=0; i<CLST_NUM; i++)
	{
		ppLyrMsk[i] = new Layer(dimScl.m_x, dimScl.m_y, dimScl.m_z);
	}
	
	Vect3D<unsigned> dimG(5, 5, 5);
	//Vect3D<unsigned> dimG = pCluster->GetVolFileVal().GetDimStep();
	//Vect3D<DATA> stpG = pCluster->GetVolFileVal().GetSclStep();
	Vect3D<DATA> stpG(1.F, 1.F, 1.F);
	//DATA minStp = FindMin(stpG);
	//Mult(stpG, minStp);

	const unsigned lenV = 3;
	DATA sVar = 1.1F;
	DATA eVar = 2.F;
	VolumeData **ppVolG = new VolumeData*[lenV];
	for(unsigned i=0; i<lenV; i++)
	{
		DATA var = myMath.Interpolate_linear(sVar, eVar, (DATA)i/(lenV-1));

		ppVolG[i] = new VolumeData(dimG, stpG);
		lyrOp.Gauss3D.Gen(*ppVolG[i], var);
	}
	

	DATA xMin, xMax, yMin, yMax, zMin, zMax;
	pCluster->GetBBox(xMin, xMax, yMin, yMax, zMin, zMax);
	//cout << xMin << " " << xMax << " "
	//	 << yMin << " " << yMax << " "
	//	 << zMin << " " << zMax << endl;
	DATA xRat = (dimTop.m_x-1.F) / (xMax-xMin);
	DATA yRat = (dimTop.m_y-1.F) / (yMax-yMin);
	DATA zRat = (dimBot.m_y-1.F) / (zMax-zMin);

	for(unsigned i=0; i<CLST_NUM; i++)
	{
		//lyrOp.splt.New(*ppLyrMsk[i]);
		lyrOp.zero.Gen(*ppLyrMsk[i]);
	}
	Vect3D<unsigned> dimStp = pCluster->GetVolFileVal().GetDimStep();
	for(unsigned z=0; z<dimScl.m_z; z++)
	{
		unsigned zOrg = z * dimStp.m_z;
		for(unsigned y=0; y<dimScl.m_y; y++)
		{
			unsigned yOrg = y * dimStp.m_y;
			for(unsigned x=0; x<dimScl.m_x; x++)
			{
				unsigned xOrg = x * dimStp.m_x;

				unsigned no = z*dimScl.m_x*dimScl.m_y + y*dimScl.m_x + x;
				DATA ptX, ptY, ptZ;
				pCluster->GetLoc(ptX, ptY, ptZ, no);

				unsigned uX = (unsigned)((ptX-xMin) * xRat + 0.5F);
				unsigned uY = (unsigned)((ptY-yMin) * yRat + 0.5F);
				unsigned uZ = (unsigned)((ptZ-zMin) * zRat + 0.5F);

				for(unsigned i=0; i<vSelTop.size(); i++)
				{
					float aaV[4][2];
					vSelBot[i].GetCtrlP(aaV);
					unsigned zN = (unsigned)(aaV[0][1] + 0.5f);	
					unsigned zF = (unsigned)(aaV[3][1] + 0.5f);			
					if(uZ<zN || uZ>zF)	continue;	else {}			
			
					vSelTop[i].GetCtrlP(aaV);
					unsigned xL = (unsigned)(aaV[0][0] + 0.5f);	
					unsigned xR = (unsigned)(aaV[1][0] + 0.5f);
					unsigned yB = (unsigned)(aaV[0][1] + 0.5f);	
					unsigned yT = (unsigned)(aaV[3][1] + 0.5f);
					if(uX<xL || uX>xR || uY<yB || uY>yT)
					{	
						//lyrOp.splt.AddPoint(lyrMsk, 0, ppVolG[0]->GetLyrVal(), 
						//	xOrg, yOrg, zOrg);
						//lyrOp.splt.AddPoint(lyrMsk, 0, ppVolG[lenV-1]->GetLyrVal(), 
						//	xOrg, yOrg, zOrg);
						//lyrOp.splt.AddPoint(*ppLyrMsk[i], 0, ppVolG[0]->GetLyrVal(),
						//	x, y, z);

						for(unsigned zz=0; zz<3; zz++)
						{
							int zLoc = (int)z + zz - 3/2;
							if(!ppLyrMsk[i]->IsCInside(zLoc))
								continue;
							else {}
							for(unsigned yy=0; yy<3; yy++)
							{
								int yLoc = (int)y + yy - 3/2;
								//cout << yLoc << " ";
								if(!ppLyrMsk[i]->IsYInside(yLoc))
									continue;
								else {}
								for(unsigned xx=0; xx<3; xx++)
								{
									int xLoc = (int)x + xx - 3/2;
									if(!ppLyrMsk[i]->IsXInside(xLoc))
										continue;
									else {}

									//lyrMsk.CellRef(xLoc, yLoc, zLoc) = (i+1.F) * 0.1F;
									ppLyrMsk[i]->CellRef(xLoc, yLoc, zLoc) = 
										ppLyrMsk[i]->CellVal(xLoc, yLoc, zLoc) - 1.F;
								} // xx
							} // yy
						} // zz
					}
					else
					{
						//lyrOp.splt.AddPoint(*ppLyrMsk[i], 1.F, ppVolG[lenV-1]->GetLyrVal(),
						//	x, y, z);

						//lyrOp.splt.AddPoint(lyrMsk, 1.F, ppVolG[lenV-1]->GetLyrVal(), 
						//	xOrg, yOrg, zOrg);
						//lyrOp.splt.AddPoint(lyrMsk, 1.F, ppVolG[0]->GetLyrVal(), 
						//	xOrg, yOrg, zOrg);
						//lyrMsk.CellRef(x, y, z) = 1.F;
						//lyrMsk.CellRef(x, y, z) = (i+1.F) * 0.1F;

						
						for(unsigned zz=0; zz<3; zz++)
						{
							int zLoc = (int)z + zz - 3/2;
							if(!ppLyrMsk[i]->IsCInside(zLoc))
								continue;
							else {}
							for(unsigned yy=0; yy<3; yy++)
							{
								int yLoc = (int)y + yy - 3/2;
								if(!ppLyrMsk[i]->IsYInside(yLoc))
									continue;
								else {}
								for(unsigned xx=0; xx<3; xx++)
								{
									int xLoc = (int)x + xx - 3/2;
									if(!ppLyrMsk[i]->IsXInside(xLoc))
										continue;
									else {}

									//lyrMsk.CellRef(xLoc, yLoc, zLoc) = (i+1.F) * 0.1F;
									ppLyrMsk[i]->CellRef(xLoc, yLoc, zLoc) = 
										ppLyrMsk[i]->CellVal(xLoc, yLoc, zLoc) + 1.F;
								} // xx
							} // yy
						} // zz
						
					} // if
				} // i
			} // x
		} // y
	} // z
	for(unsigned i=0; i<vSelTop.size(); i++)
	{
		//lyrOp.splt.Gen(*ppLyrMsk[i]);
	}
	//lyrOp.splt.Gen(lyrMsk);	
	for(unsigned i=0; i<lenV; i++)
	{
		delete ppVolG[i];
	}
	delete []ppVolG;

	ofstream ff(pCluster->GetFName()+".msk", ios::binary);
	/*
	ff.write((char*)&dimOrg.m_x, sizeof(unsigned));
	ff.write((char*)&dimOrg.m_y, sizeof(unsigned));
	ff.write((char*)&dimOrg.m_z, sizeof(unsigned));
	for(unsigned z=0; z<dimOrg.m_z; z++)
	{
		for(unsigned y=0; y<dimOrg.m_y; y++)
		{
			for(unsigned x=0; x<dimOrg.m_x; x++)
			{
				float v = (float)lyrMsk.CellVal(x, y, z);
				ff.write((char*)&v, sizeof(float));
			}
		}
	}
	*/
	ff.write((char*)&dimScl.m_x, sizeof(unsigned));
	ff.write((char*)&dimScl.m_y, sizeof(unsigned));
	ff.write((char*)&dimScl.m_z, sizeof(unsigned));
	for(unsigned z=0; z<dimScl.m_z; z++)
	{
		for(unsigned y=0; y<dimScl.m_y; y++)
		{
			for(unsigned x=0; x<dimScl.m_x; x++)
			{
				//float v = (float)lyrMsk.CellVal(x, y, z);
				//ff.write((char*)&v, sizeof(float));
				//cout << v << " ";
				unsigned idMax = 0;
				unsigned idMin = 0;
				for(unsigned i=1; i<vSelTop.size(); i++)
				{
					if(ppLyrMsk[i]->CellVal(x, y, z) > ppLyrMsk[idMax]->CellVal(x, y, z))
					{
						idMax = i;
					}
					else {}
					if(ppLyrMsk[i]->CellVal(x, y, z) < ppLyrMsk[idMin]->CellVal(x, y, z))
					{
						idMin = i;
					}
					else {}
				}

				float v = 0.99f;
				if(ppLyrMsk[idMax]->CellVal(x, y, z) <= 0)
				{
					v = 0;
				}
				else if(ppLyrMsk[idMin]->CellVal(x, y, z) < 0)
				{
					v = (idMin+1) * 0.1f;
				}
				else {}
				ff.write((char*)&v, sizeof(float));
			}
		}
	}
	ff.close();

	//*******************************************

	ofstream cf(pCluster->GetFName()+".clst", ios::binary);
	unsigned vS = vSelTop.size();
	cf.write((char*)&vS, sizeof(unsigned));
	for(unsigned i=0; i<vS; i++)
	{
		DATA aaV[4][2];
		vSelTop[i].GetCtrlP(aaV);
		for(unsigned j=0; j<4; j++)
		{
			cf.write((char*)aaV[j], sizeof(DATA)*2);
		}
		vSelBot[i].GetCtrlP(aaV);
		for(unsigned j=0; j<4; j++)
		{
			cf.write((char*)aaV[j], sizeof(DATA)*2);
		}

		DATA aColor[4];
		vSelTop[i].GetColorFr(aColor);
		cf.write((char*)aColor, sizeof(DATA)*4);
		vSelTop[i].GetColorTo(aColor);
		cf.write((char*)aColor, sizeof(DATA)*4);
	}
	cf.close();
}

/*
void GenCluster(Fl_Widget *w, void *v)
{	
	Vect2D<unsigned> dimTop = pTexLleTop->GetDim();
	Vect2D<unsigned> dimBot = pTexLleBot->GetDim();
	unsigned dimMap = dimBot.m_y;

	float *pMap = new float[dimMap*4];	
	for(unsigned i=0; i<dimMap; i++)
	{
		for(unsigned j=0; j<4; j++)
		{
			pMap[i*4+j] = 0;
		}
	}

	for(unsigned i=0; i<vSelTop.size(); i++)
	{
		DATA aColorFr[4];
		DATA aColorTo[4];
		vSelBot[i].GetColorFr(aColorFr);
		vSelBot[i].GetColorTo(aColorTo);

		float aaV[4][2];
		vSelBot[i].GetCtrlP(aaV);
		unsigned zN = (unsigned)(aaV[0][1] + 0.5f);
		unsigned zF = (unsigned)(aaV[3][1] + 0.5f);
		for(unsigned z=zN; z<=zF; z++)
		{
			DATA fRatio = (DATA)(zF-z+1)/(zF-zN+1);
			for(unsigned j=0; j<4; j++)
			{
				pMap[z*4+j] = (float)myMath.Interpolate_linear(aColorFr[j], aColorTo[j], fRatio);
			}
		}
	}

	ofstream f(pCluster->GetFName()+".tf", ios::binary);
	f.write((char*)&dimMap, sizeof(unsigned));
	f.write((char*)pMap, sizeof(float)*dimMap*4);
	f.close();
	delete []pMap;

	//*******************************************

	Vect3D<unsigned> dimOrg = pCluster->GetVolFileVal().GetOrgDim();
	Vect3D<unsigned> dimScl = pCluster->GetVolFileVal().GetSclDim();
	Layer lyrMsk(dimOrg.m_x, dimOrg.m_y, dimOrg.m_z);
	//Layer lyrMsk(dimScl.m_x, dimScl.m_y, dimScl.m_z);

	Vect3D<unsigned> dimG(5, 5, 5);
	//Vect3D<unsigned> dimG = pCluster->GetVolFileVal().GetDimStep();
	Vect3D<DATA> stpG = pCluster->GetVolFileVal().GetSclStep();
	//DATA minStp = FindMin(stpG);
	//Mult(stpG, minStp);

	const unsigned lenV = 5;
	DATA sVar = 1.1F;
	DATA eVar = 2.F;
	VolumeData **ppVolG = new VolumeData*[lenV];
	for(unsigned i=0; i<lenV; i++)
	{
		DATA var = myMath.Interpolate_linear(sVar, eVar, (DATA)i/(lenV-1));

		ppVolG[i] = new VolumeData(dimG, stpG);
		lyrOp.Gauss3D.Gen(*ppVolG[i], var);
	}

	DATA xMin, xMax, yMin, yMax, zMin, zMax;
	pCluster->GetBBox(xMin, xMax, yMin, yMax, zMin, zMax);
	//cout << xMin << " " << xMax << " "
	//	 << yMin << " " << yMax << " "
	//	 << zMin << " " << zMax << endl;
	DATA xRat = (dimTop.m_x-1.F) / (xMax-xMin);
	DATA yRat = (dimTop.m_y-1.F) / (yMax-yMin);
	DATA zRat = (dimBot.m_y-1.F) / (zMax-zMin);

	//lyrOp.zero.Gen(lyrMsk);
	lyrOp.splt.New(lyrMsk);
	Vect3D<unsigned> dimStp = pCluster->GetVolFileVal().GetDimStep();
	for(unsigned z=0; z<dimScl.m_z; z++)
	{
		unsigned zOrg = z * dimStp.m_z;
		for(unsigned y=0; y<dimScl.m_y; y++)
		{
			unsigned yOrg = y * dimStp.m_y;
			for(unsigned x=0; x<dimScl.m_x; x++)
			{
				unsigned xOrg = x * dimStp.m_x;

				unsigned no = z*dimScl.m_x*dimScl.m_y + y*dimScl.m_x + x;
				DATA ptX, ptY, ptZ;
				pCluster->GetLoc(ptX, ptY, ptZ, no);

				unsigned uX = (unsigned)((ptX-xMin) * xRat + 0.5F);
				unsigned uY = (unsigned)((ptY-yMin) * yRat + 0.5F);
				unsigned uZ = (unsigned)((ptZ-zMin) * zRat + 0.5F);

				for(unsigned i=0; i<vSelTop.size(); i++)
				{
					float aaV[4][2];
					vSelBot[i].GetCtrlP(aaV);
					unsigned zN = (unsigned)(aaV[0][1] + 0.5f);	
					unsigned zF = (unsigned)(aaV[3][1] + 0.5f);			
					if(uZ<zN || uZ>zF)	continue;	else {}			
			
					vSelTop[i].GetCtrlP(aaV);
					unsigned xL = (unsigned)(aaV[0][0] + 0.5f);	
					unsigned xR = (unsigned)(aaV[1][0] + 0.5f);
					unsigned yB = (unsigned)(aaV[0][1] + 0.5f);	
					unsigned yT = (unsigned)(aaV[3][1] + 0.5f);
					if(uX<xL || uX>xR || uY<yB || uY>yT)
					{	
						//lyrOp.splt.AddPoint(lyrMsk, 0, ppVolG[0]->GetLyrVal(), 
						//	xOrg, yOrg, zOrg);
						lyrOp.splt.AddPoint(lyrMsk, 0, ppVolG[lenV-1]->GetLyrVal(), 
							xOrg, yOrg, zOrg);

					}
					else
					{
						//lyrOp.splt.AddPoint(lyrMsk, 1.F, ppVolG[lenV-1]->GetLyrVal(), 
						//	xOrg, yOrg, zOrg);
						lyrOp.splt.AddPoint(lyrMsk, 1.F, ppVolG[0]->GetLyrVal(), 
							xOrg, yOrg, zOrg);
						//lyrMsk.CellRef(x, y, z) = 1.F;
					}
				}
			}
		}
	}
	lyrOp.splt.Gen(lyrMsk);	
	for(unsigned i=0; i<lenV; i++)
	{
		delete ppVolG[i];
	}
	delete []ppVolG;

	ofstream ff(pCluster->GetFName()+".msk", ios::binary);
	ff.write((char*)&dimOrg.m_x, sizeof(unsigned));
	ff.write((char*)&dimOrg.m_y, sizeof(unsigned));
	ff.write((char*)&dimOrg.m_z, sizeof(unsigned));
	for(unsigned z=0; z<dimOrg.m_z; z++)
	{
		for(unsigned y=0; y<dimOrg.m_y; y++)
		{
			for(unsigned x=0; x<dimOrg.m_x; x++)
			{
				float v = (float)lyrMsk.CellVal(x, y, z);
				ff.write((char*)&v, sizeof(float));
			}
		}
	}
	ff.close();

	//*******************************************

	ofstream cf(pCluster->GetFName()+".clst", ios::binary);
	unsigned vS = vSelTop.size();
	cf.write((char*)&vS, sizeof(unsigned));
	for(unsigned i=0; i<vS; i++)
	{
		DATA aaV[4][2];
		vSelTop[i].GetCtrlP(aaV);
		for(unsigned j=0; j<4; j++)
		{
			cf.write((char*)aaV[j], sizeof(DATA)*2);
		}
		vSelBot[i].GetCtrlP(aaV);
		for(unsigned j=0; j<4; j++)
		{
			cf.write((char*)aaV[j], sizeof(DATA)*2);
		}

		DATA aColor[4];
		vSelTop[i].GetColorFr(aColor);
		cf.write((char*)aColor, sizeof(DATA)*4);
		vSelTop[i].GetColorTo(aColor);
		cf.write((char*)aColor, sizeof(DATA)*4);
	}
	cf.close();
}
*/