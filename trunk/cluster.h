#ifndef _CLUSTER_H
#define _CLUSTER_H

#include <windows.h>
#include "define.h"
#include "layer.h"
#include "layerOperation.h"
#include "imageIO.h"
#include "volumeData.h"

#include <iostream>
#include <fstream>

using namespace std;

class PPoint
{
public:
	DATA m_x;
	DATA m_y;
	DATA m_z;

	//unsigned m_orgX;
	//unsigned m_orgY;
	//unsigned m_orgZ;
};

class Cluster
{
public:
	Cluster(string f);
	~Cluster();

	void GetBBox(DATA &xMin, DATA &xMax, DATA &yMin, DATA &yMax, DATA &zMin, DATA &zMax);
	unsigned GetNum();
	void GetLoc(DATA &x, DATA &y, DATA &z, unsigned no);
	void SaveResult(string f, Vect2D<unsigned> dimOut);

	float *GenerateDistribution(unsigned xDim, unsigned yDim, unsigned xAxis, unsigned yAxis);
	const VolumeFile& GetVolFileVal();
	string GetFName();

private:
	VolumeFile *m_pVolF;

	unsigned m_dataNum;
	PPoint *m_pData;
	string m_fName;
	DATA m_xMin;	DATA m_xMax;
	DATA m_yMin;	DATA m_yMax;
	DATA m_zMin;	DATA m_zMax;
};

//*************************************************************************************************

class Selecter
{  
//  p3---------p2  
//  |          |  
//	|          |
//  p0---------p1
public:
	Selecter();
	Selecter(DATA aaV[][2], DATA aCFr[], DATA aCTo[]);
	~Selecter();

	void MoveOrgX(DATA xDis);
	void MoveOrgY(DATA yDis);

	void MoveP0(DATA xDis, DATA yDis);
	void MoveP1(DATA xDis, DATA yDis);
	void MoveP2(DATA xDis, DATA yDis);
	void MoveP3(DATA xDis, DATA yDis);

	//void SetP0X(DATA x);
	//void SetP1X(DATA x);
	//void SetP2X(DATA x);
	//void SetP3X(DATA x);
	void SetCtrlP(DATA aaV[][2]);

	void SetColorFr(DATA aColor[]);
	void SetColorTo(DATA acolor[]);
	
	void GetColorFr(DATA aColor[]);
	void GetColorFr(float aColor[]);
	void GetColorTo(DATA aColor[]);
	void GetColorTo(float aColor[]);

	void SetCtrlNo(int no);
	int GetSetCtrlNo();

	bool IsInside(DATA x, DATA y);
	bool IsInsideX(DATA x);
	bool IsInsideY(DATA y);
	void GetCtrlP(float aaV[][2]);
	void GetCtrlP(DATA aaV[][2]);

	void GetCtrlBBoxP0(DATA &xMin, DATA &xMax, DATA &yMin, DATA &yMax);
	void GetCtrlBBoxP1(DATA &xMin, DATA &xMax, DATA &yMin, DATA &yMax);
	void GetCtrlBBoxP2(DATA &xMin, DATA &xMax, DATA &yMin, DATA &yMax);
	void GetCtrlBBoxP3(DATA &xMin, DATA &xMax, DATA &yMin, DATA &yMax);

	bool IsInCtrlP0(DATA x, DATA y);
	bool IsInCtrlP1(DATA x, DATA y);
	bool IsInCtrlP2(DATA x, DATA y);
	bool IsInCtrlP3(DATA x, DATA y);

private:
	Vect2D<DATA> m_vCtrlP0, m_vCtrlP1, m_vCtrlP2, m_vCtrlP3; 
	//Vect3D<DATA> m_vLen;
	DATA m_aColorFr[4];
	DATA m_aColorTo[4];
	DATA m_ctrlHLen;
	int m_setCtrlNo;
};

#endif