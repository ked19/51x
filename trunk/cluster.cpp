#include "cluster.h"
#include "texture.h"
extern Tex3D *pTexVal;

vector<Selecter> vSelTop;
vector<Selecter> vSelBot;

Selecter::Selecter()
	:m_vCtrlP0(Vect2D<DATA>(50.F, 50.F)),
	 m_vCtrlP1(Vect2D<DATA>(100.F, 50.F)),
	 m_vCtrlP2(Vect2D<DATA>(100.F, 100.F)),
	 m_vCtrlP3(Vect2D<DATA>(50.F, 100.F)), 
	 m_ctrlHLen(3.f), m_setCtrlNo(-1)
{
	for(unsigned i=0; i<4; i++)
	{
		m_aColorFr[i] = 0;
		m_aColorTo[i] = 0;
	}
}

Selecter::Selecter(DATA aaV[][2], DATA aCFr[], DATA aCTo[])
	:m_vCtrlP0(Vect2D<DATA>(aaV[0][0], aaV[0][1])),
	 m_vCtrlP1(Vect2D<DATA>(aaV[1][0], aaV[1][1])),
	 m_vCtrlP2(Vect2D<DATA>(aaV[2][0], aaV[2][1])),
	 m_vCtrlP3(Vect2D<DATA>(aaV[3][0], aaV[3][1])),
	 m_ctrlHLen(3.f), m_setCtrlNo(-1)
{
	for(unsigned i=0; i<4; i++)
	{
		m_aColorFr[i] = aCFr[i];
		m_aColorTo[i] = aCTo[i];
	}
}

Selecter::~Selecter()
{}

//***********************************************

void Selecter::MoveOrgX(DATA xDis)
{
	m_vCtrlP0.m_x += xDis;
	m_vCtrlP1.m_x += xDis;
	m_vCtrlP2.m_x += xDis;
	m_vCtrlP3.m_x += xDis;
}

void Selecter::MoveOrgY(DATA yDis)
{
	m_vCtrlP0.m_y += yDis;
	m_vCtrlP1.m_y += yDis;
	m_vCtrlP2.m_y += yDis;
	m_vCtrlP3.m_y += yDis;
}

//***********************************************

void MoveP(Vect2D<DATA> &p, DATA xDis, DATA yDis)
{
	p.m_x += xDis;
	p.m_y += yDis;
}

void Selecter::MoveP0(DATA xDis, DATA yDis)
{
	MoveP(m_vCtrlP0, xDis, yDis);
}

void Selecter::MoveP1(DATA xDis, DATA yDis)
{
	MoveP(m_vCtrlP1, xDis, yDis);
}

void Selecter::MoveP2(DATA xDis, DATA yDis)
{
	MoveP(m_vCtrlP2, xDis, yDis);
}

void Selecter::MoveP3(DATA xDis, DATA yDis)
{
	MoveP(m_vCtrlP3, xDis, yDis);
}

//***********************************************
/*
void SetPX(Vect2D<DATA> &p, DATA x)
{
	p.m_x = x;
}

void Selecter::SetP0X(DATA x)
{
	SetPX(m_vCtrlP0, x);
}

void Selecter::SetP1X(DATA x)
{
	SetPX(m_vCtrlP1, x);
}

void Selecter::SetP2X(DATA x)
{
	SetPX(m_vCtrlP2, x);
}

void Selecter::SetP3X(DATA x)
{
	SetPX(m_vCtrlP3, x);
}
*/
void Selecter::SetCtrlP(DATA aaV[][2])
{
	m_vCtrlP0.m_x = aaV[0][0];		m_vCtrlP0.m_y = aaV[0][1];
	m_vCtrlP1.m_x = aaV[1][0];		m_vCtrlP1.m_y = aaV[1][1];
	m_vCtrlP2.m_x = aaV[2][0];		m_vCtrlP2.m_y = aaV[2][1];
	m_vCtrlP3.m_x = aaV[3][0];		m_vCtrlP3.m_y = aaV[3][1];
}

//***********************************************

void Selecter::SetColorFr(DATA aColor[])
{
	m_aColorFr[0] = aColor[0];
	m_aColorFr[1] = aColor[1];
	m_aColorFr[2] = aColor[2];
	m_aColorFr[3] = aColor[3];
}
void Selecter::SetColorTo(DATA aColor[])
{
	m_aColorTo[0] = aColor[0];
	m_aColorTo[1] = aColor[1];
	m_aColorTo[2] = aColor[2];
	m_aColorTo[3] = aColor[3];
}

void Selecter::GetColorFr(DATA aColor[])
{
	aColor[0] = m_aColorFr[0];
	aColor[1] = m_aColorFr[1];
	aColor[2] = m_aColorFr[2];
	aColor[3] = m_aColorFr[3];
}
void Selecter::GetColorFr(float aColor[])
{
	aColor[0] = (float)m_aColorFr[0];
	aColor[1] = (float)m_aColorFr[1];
	aColor[2] = (float)m_aColorFr[2];
	aColor[3] = (float)m_aColorFr[3];
}
void Selecter::GetColorTo(DATA aColor[])
{
	aColor[0] = m_aColorTo[0];
	aColor[1] = m_aColorTo[1];
	aColor[2] = m_aColorTo[2];
	aColor[3] = m_aColorTo[3];
}
void Selecter::GetColorTo(float aColor[])
{
	aColor[0] = (float)m_aColorTo[0];
	aColor[1] = (float)m_aColorTo[1];
	aColor[2] = (float)m_aColorTo[2];
	aColor[3] = (float)m_aColorTo[3];
}

void Selecter::SetCtrlNo(int no)
{
	m_setCtrlNo = no;
}

int Selecter::GetSetCtrlNo()
{
	return m_setCtrlNo;
}

//***********************************************

// ax + by + c = 0
void FindFunc(DATA aFunc[], Vect2D<DATA> p0, Vect2D<DATA> p1)
{
	aFunc[0] = p0.m_y - p1.m_y;
	aFunc[1] = -(p0.m_x - p1.m_x);
	aFunc[2] = -(aFunc[0]*p0.m_x + aFunc[1]*p0.m_y);
}

bool IsSameSide(Vect2D<DATA> p0, Vect2D<DATA> p1, Vect2D<DATA> p2, DATA x, DATA y)
{
	DATA aFunc[3];
	FindFunc(aFunc, p0, p1);
	
	DATA p2Val = p2.m_x*aFunc[0] + p2.m_y*aFunc[1] + aFunc[2];
	DATA inVal = x*aFunc[0] + y*aFunc[1] + aFunc[2];

	DATA mul = p2Val * inVal;
	if(mul > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Selecter::IsInside(DATA x, DATA y)
{
	/*
	if(IsInCtrlP0(x, y) ||
	   IsInCtrlP1(x, y) ||
	   IsInCtrlP2(x, y) ||
	   IsInCtrlP3(x, y))
	{
		return true;
	}
	else {}
	*/

	if(IsSameSide(m_vCtrlP0, m_vCtrlP1, m_vCtrlP2, x, y) &&
	   IsSameSide(m_vCtrlP1, m_vCtrlP2, m_vCtrlP3, x, y) &&
	   IsSameSide(m_vCtrlP2, m_vCtrlP3, m_vCtrlP0, x, y) &&
	   IsSameSide(m_vCtrlP3, m_vCtrlP0, m_vCtrlP1, x, y))
	{
		return true;
	}
	else 
	{
		return false;
	}
}

bool Selecter::IsInsideX(DATA x)
{
	if(x>=m_vCtrlP0.m_x && x<=m_vCtrlP1.m_x)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Selecter::IsInsideY(DATA y)
{
	if(y>=m_vCtrlP0.m_y && y<=m_vCtrlP3.m_y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Selecter::GetCtrlP(float aaV[][2])
{
	aaV[0][0] = (float)m_vCtrlP0.m_x;		aaV[0][1] = (float)m_vCtrlP0.m_y;
	aaV[1][0] = (float)m_vCtrlP1.m_x;		aaV[1][1] = (float)m_vCtrlP1.m_y;
	aaV[2][0] = (float)m_vCtrlP2.m_x;		aaV[2][1] = (float)m_vCtrlP2.m_y;
	aaV[3][0] = (float)m_vCtrlP3.m_x;		aaV[3][1] = (float)m_vCtrlP3.m_y;
}
void Selecter::GetCtrlP(DATA aaV[][2])
{
	aaV[0][0] = m_vCtrlP0.m_x;		aaV[0][1] = m_vCtrlP0.m_y;
	aaV[1][0] = m_vCtrlP1.m_x;		aaV[1][1] = m_vCtrlP1.m_y;
	aaV[2][0] = m_vCtrlP2.m_x;		aaV[2][1] = m_vCtrlP2.m_y;
	aaV[3][0] = m_vCtrlP3.m_x;		aaV[3][1] = m_vCtrlP3.m_y;
}

/*
DATA GetCtrlPX(Vect2D<DATA> &p)
{
	return p.m_x;		
}

DATA Selecter::GetCtrlP0X()
{
	return GetCtrlPX(m_vCtrlP0);
}

DATA Selecter::GetCtrlP1X()
{
	return GetCtrlPX(m_vCtrlP1);
}

DATA Selecter::GetCtrlP2X()
{
	return GetCtrlPX(m_vCtrlP2);
}

DATA Selecter::GetCtrlP3X()
{
	return GetCtrlPX(m_vCtrlP3);
}
*/
//***********************************************

void GetCtrlBBox(Vect2D<DATA> p, DATA hLen, DATA &xMin, DATA &xMax, DATA &yMin, DATA &yMax)
{
	xMin = p.m_x - hLen;		xMax = p.m_x + hLen;
	yMin = p.m_y - hLen;		yMax = p.m_y + hLen;
}

void Selecter::GetCtrlBBoxP0(DATA &xMin, DATA &xMax, DATA &yMin, DATA &yMax)
{
	GetCtrlBBox(m_vCtrlP0, m_ctrlHLen, xMin, xMax, yMin, yMax);
}

void Selecter::GetCtrlBBoxP1(DATA &xMin, DATA &xMax, DATA &yMin, DATA &yMax)
{
	GetCtrlBBox(m_vCtrlP1, m_ctrlHLen, xMin, xMax, yMin, yMax);
}

void Selecter::GetCtrlBBoxP2(DATA &xMin, DATA &xMax, DATA &yMin, DATA &yMax)
{
	GetCtrlBBox(m_vCtrlP2, m_ctrlHLen, xMin, xMax, yMin, yMax);
}

void Selecter::GetCtrlBBoxP3(DATA &xMin, DATA &xMax, DATA &yMin, DATA &yMax)
{
	GetCtrlBBox(m_vCtrlP3, m_ctrlHLen, xMin, xMax, yMin, yMax);
}

//***********************************************

bool IsInCtrl(Vect2D<DATA> p, DATA hLen, DATA x, DATA y)
{
	DATA xMin, xMax;
	DATA yMin, yMax;
	GetCtrlBBox(p, hLen, xMin, xMax, yMin, yMax);

	if(x<xMin || x>xMax ||
	   y<yMin || y>yMax)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Selecter::IsInCtrlP0(DATA x, DATA y)
{
	return IsInCtrl(m_vCtrlP0, m_ctrlHLen, x, y);
}

bool Selecter::IsInCtrlP1(DATA x, DATA y)
{
	return IsInCtrl(m_vCtrlP1, m_ctrlHLen, x, y);
}

bool Selecter::IsInCtrlP2(DATA x, DATA y)
{
	return IsInCtrl(m_vCtrlP2, m_ctrlHLen, x, y);
}

bool Selecter::IsInCtrlP3(DATA x, DATA y)
{
	return IsInCtrl(m_vCtrlP3, m_ctrlHLen, x, y);
}

//*************************************************************************************************
//
//*************************************************************************************************

Cluster::Cluster(string f)
	:m_dataNum(0), m_pData(0), m_pVolF(0), m_fName(f)
{
	m_pVolF = new VolumeFile(f+".vol");
	Vect3D<unsigned> dimScl = m_pVolF->GetSclDim();
	Vect3D<DATA> stp = m_pVolF->GetSclStep();
	
	m_dataNum = dimScl.m_x * dimScl.m_y * dimScl.m_z;
	m_pData = new PPoint[m_dataNum];
	
	ifstream fs(f+".mdsj");
	const VolumeData& rVolScl = m_pVolF->GetSclVolVal();
	for(unsigned z=0; z<dimScl.m_z; z++)
	{
		cout << z << " ";
		for(unsigned y=0; y<dimScl.m_y; y++)
		{
			for(unsigned x=0; x<dimScl.m_x; x++)
			{
				unsigned loc = z*dimScl.m_x*dimScl.m_y + y*dimScl.m_x + x;
				
				fs >> m_pData[loc].m_x
				   >> m_pData[loc].m_y;
				//fs >> m_pData[loc].m_x;
				//m_pData[loc].m_y = 0;
				m_pData[loc].m_z = rVolScl.CellVal(x, y, z);
			}
		}
	}
	cout << endl;
	cout << "read mdsj ok." << endl;
	fs.close();

	m_xMin = 1e10;	m_xMax = -1e10;
	m_yMin = 1e10;	m_yMax = -1e10;
	m_zMin = 1e10;	m_zMax = -1e10;
	for(unsigned i=0; i<m_dataNum; i++)
	{
		if(m_xMin>m_pData[i].m_x)	m_xMin = m_pData[i].m_x;
		if(m_xMax<m_pData[i].m_x)	m_xMax = m_pData[i].m_x;

		if(m_yMin>m_pData[i].m_y)	m_yMin = m_pData[i].m_y;
		if(m_yMax<m_pData[i].m_y)	m_yMax = m_pData[i].m_y;

		if(m_zMin>m_pData[i].m_z)	m_zMin = m_pData[i].m_z;
		if(m_zMax<m_pData[i].m_z)	m_zMax = m_pData[i].m_z;
	}
	if(m_yMin == m_yMax) {
		m_yMin -= 0.5F;
		m_yMax += 0.5F;
	} else {}
	cout << "xMin: " << m_xMin << "\txMax: " << m_xMax << endl;
	cout << "yMin: " << m_yMin << "\tyMax: " << m_yMax << endl;
	cout << "zMin: " << m_zMin << "\tzMax: " << m_zMax << endl;

	DATA xLen = m_xMax - m_xMin;
	DATA yLen = m_yMax - m_yMin;
	DATA zLen = m_zMax - m_zMin;
	DATA maxLen = (xLen>yLen)? xLen: yLen;
}

Cluster::~Cluster()
{
	delete []m_pData;
	delete []m_pVolF;
}

//*************************************************************************************************

unsigned Cluster::GetNum()
{
	return m_dataNum;
}

void Cluster::GetBBox(DATA &xMin, DATA &xMax, DATA &yMin, DATA &yMax, DATA &zMin, DATA &zMax)
{
	xMin = m_xMin;	xMax = m_xMax;
	yMin = m_yMin;	yMax = m_yMax;
	zMin = m_zMin;	zMax = m_zMax;
}

void Cluster::GetLoc(DATA &x, DATA &y, DATA &z, unsigned no)
{
	x = m_pData[no].m_x;
	y = m_pData[no].m_y;
	z = m_pData[no].m_z;
}

const VolumeFile& Cluster::GetVolFileVal()
{
	return *m_pVolF;
}

string Cluster::GetFName()
{
	return m_fName;
}

//*************************************************************************************************

unsigned Trans(DATA xIn, DATA xScl, unsigned xLen)
{
	int xLoc = (int)(xIn*xScl + 0.5F);
	if(xLoc < 0)
	{
		xLoc = 0;
	}
	else {}
	if(xLoc >= (int)xLen)
	{
		xLoc = xLen - 1;
	}
	else {}

	return (unsigned)xLoc;
}

void SaveColormap(string fName, Vect2D<unsigned> dimImg, Vect2D<DATA> scl)
{
	/*
	Layer *pLyrColor = new Layer(dimImg.m_x, dimImg.m_y, 4);
	lyrOp.zero.Gen(*pLyrColor);

	for(unsigned i=0; i<vSel.size(); i++)
	{
		DATA xMin, xMax, yMin, yMax, zMin, zMax;
		vSel[i].GetBBox(xMin, xMax, yMin, yMax, zMin, zMax);
		unsigned xMinLoc = Trans(xMin, scl.m_x, dimImg.m_x);
		unsigned xMaxLoc = Trans(xMax, scl.m_x, dimImg.m_x);
		unsigned yMinLoc = Trans(yMin, scl.m_y, dimImg.m_y);
		unsigned yMaxLoc = Trans(yMax, scl.m_y, dimImg.m_y);

		for(unsigned y=yMinLoc; y<=yMaxLoc; y++)
		{
			for(unsigned x=xMinLoc; x<=xMaxLoc; x++)
			{
				DATA aColor[4];
				vSel[i].GetColor(aColor);
				for(unsigned c=0; c<4; c++)
				{
					pLyrColor->CellRef(x, y, c) = aColor[c] * 255.F;
				}
			}
		}
	}

	imgIO.Write(fName, MyImg(*pLyrColor));
	delete pLyrColor;
	*/
}

void Cluster::SaveResult(string f, Vect2D<unsigned> dimOut)
{
	DATA xMin, xMax, yMin, yMax, zMin, zMax;
	GetBBox(xMin, xMax, yMin, yMax, zMin, zMax);
	Vect2D<DATA> scl(dimOut.m_x/xMax, dimOut.m_y/yMax);

	SaveColormap("img.bmp", dimOut, scl);

	/*
	ofstream fs(f.c_str());
	for(unsigned i=0; i<m_dataNum; i++)
	{
		PPoint &p = m_pData[i];
		fs << (unsigned)p.m_orgX << "\t" << (unsigned)p.m_orgY << "\t" << (unsigned)p.m_orgZ << "\t"
			<< p.m_x*scl.m_x << "\t" << p.m_y*scl.m_y << endl;
	}
	fs.close();
	*/
}

//*************************************************************************************************

/*
float *Cluster::GenerateDistribution(unsigned xDim, unsigned yDim, unsigned xAxis, unsigned yAxis)
{
	unsigned size = xDim * yDim;
	
	unsigned *pCount = new unsigned[size];
	for(unsigned i=0; i<size; i++)
	{
		pCount[i] = 0;
	}

	DATA xMin, xMax;
	if(xAxis == 0)		{xMin = m_xMin;	xMax = m_xMax;}
	else if(xAxis == 1)	{xMin = m_yMin; xMax = m_yMax;}
	else if(xAxis == 2) {xMin = m_zMin; xMax = m_zMax;}
	else {assert(0);}

	DATA yMin, yMax;
	if(yAxis == 0)		{yMin = m_xMin; yMax = m_xMax;}
	else if(yAxis == 1) {yMin = m_yMin; yMax = m_yMax;}
	else if(yAxis == 2) {yMin = m_zMin; yMax = m_zMax;}
	else {assert(0);}

	DATA xLen = xMax - xMin;
	DATA yLen = yMax - yMin;
	DATA xRatio = (xDim - 1) / xLen;
	DATA yRatio = (yDim - 1) / yLen;
	for(unsigned i=0; i<m_dataNum; i++)
	{
		DATA xIn;
		if(xAxis == 0)		{xIn = m_pData[i].m_x;}
		else if(xAxis == 1) {xIn = m_pData[i].m_y;}
		else if(xAxis == 2) {xIn = m_pData[i].m_z;}

		DATA yIn;
		if(yAxis == 0)		{yIn = m_pData[i].m_x;}
		else if(yAxis == 1)	{yIn = m_pData[i].m_y;}
		else if(yAxis == 2) {yIn = m_pData[i].m_z;}

		unsigned xPos = (unsigned)((xIn - xMin) * xRatio + 0.5F);
		unsigned yPos = (unsigned)((yIn - yMin) * yRatio + 0.5F);

		unsigned countPos = yPos * xDim + xPos;
		pCount[countPos]++;
	}

	float *pData = new float[size];
	for(unsigned i=0; i<size; i++)
	{
		pData[i] = (float)(pCount[i]);
	}
	delete []pCount;
	return pData;
}
*/

float *Cluster::GenerateDistribution(unsigned xDim, unsigned yDim, unsigned xAxis, unsigned yAxis)
{
	unsigned size = xDim * yDim;
	
	unsigned *pCount = new unsigned[size];
	for(unsigned i=0; i<size; i++)
	{
		pCount[i] = 0;
	}

	DATA xMin, xMax;
	if(xAxis == 0)		{xMin = m_xMin;	xMax = m_xMax;}
	else if(xAxis == 1)	{xMin = m_yMin; xMax = m_yMax;}
	else if(xAxis == 2) {xMin = m_zMin; xMax = m_zMax;}
	else {assert(0);}

	DATA yMin, yMax;
	if(yAxis == 0)		{yMin = m_xMin; yMax = m_xMax;}
	else if(yAxis == 1) {yMin = m_yMin; yMax = m_yMax;}
	else if(yAxis == 2) {yMin = m_zMin; yMax = m_zMax;}
	else {assert(0);}

	DATA xLen = xMax - xMin;
	DATA yLen = yMax - yMin;
	DATA xRatio = (xDim-1) / xLen;
	DATA yRatio = (yDim-1) / yLen;	
	for(unsigned i=0; i<m_dataNum; i++)
	{
		DATA xIn;
		if(xAxis == 0)		{xIn = m_pData[i].m_x;}
		else if(xAxis == 1) {xIn = m_pData[i].m_y;}
		else if(xAxis == 2) {xIn = m_pData[i].m_z;}

		DATA yIn;
		if(yAxis == 0)		{yIn = m_pData[i].m_x;}
		else if(yAxis == 1)	{yIn = m_pData[i].m_y;}
		else if(yAxis == 2) {yIn = m_pData[i].m_z;}

		unsigned xPos = (unsigned)((xIn-xMin)*xRatio + 0.5F);
		unsigned yPos = (unsigned)((yIn-yMin)*yRatio + 0.5F);
		unsigned countPos = yPos*xDim + xPos;
		pCount[countPos]++;
	}

	float *pData = new float[size];
	for(unsigned i=0; i<size; i++)
	{
		pData[i] = (float)(pCount[i]);
	}
	delete []pCount;
	return pData;
}