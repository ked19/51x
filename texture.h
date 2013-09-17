#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "denseMatrix.h"
#include "layer.h"

#include <iostream>
#include <GL/glew.h>
#include <cassert>

class Tex1D
{
public:
	Tex1D(float *pData, unsigned xDim, unsigned cDim, bool bNormal=false, bool bF32=false, bool bLinear=true);
	~Tex1D();

	unsigned GetDim();
	GLuint GetTexID();

	//void Update(int xOffset, unsigned xDim);

private:
	void SetTexture();

	const unsigned m_xDim;
	const unsigned m_cDim;

	const bool m_bF32;
	const bool m_bLinear;

	GLuint m_tex;
	float* const m_pCell;
};

class Tex2D
{
public:
	Tex2D(float *pData, unsigned xDim, unsigned yDim, unsigned cDim, bool bNormal=false, bool bF32=false, bool bLinear=true);
	Tex2D(unsigned xDim, unsigned yDim, unsigned cDim, bool bF32=false, bool bLinear=true);
	~Tex2D();

	void DrawRec3D(float aaSpaceCoord[][3]);
	void DrawRec2D(float aLowCoord[], float w, float h, bool bNormal);
	void SetTexCoord(float aaTexCoord[][2], float bnd=0);

	void FindDrawRange(float aNewLow[], float aNewHigh[], float aLowCoord[], float w, float h);

	void SetCell(float val, unsigned x, unsigned y, unsigned c);
	float GetCell(unsigned x, unsigned y, unsigned c);
	Vect2D<unsigned> GetDim();
	GLuint GetTexID();

	void Update();

	void ReadFrame(int aLow[]);
	void Read(MyImg &img);
	void GetCell(Layer &lyr);

private:
	void SetTexture();

	const unsigned m_xDim;
	const unsigned m_yDim;
	const unsigned m_cDim;

	const bool m_bF32;
	const bool m_bLinear;

	GLuint m_tex;
	float *m_pCell;
};

class Tex3D
{
public:
	Tex3D(float *pData, unsigned xDim, unsigned yDim, unsigned zDim, unsigned cDim, bool bNormal=false, bool bF32=false, bool bLinear=true);
	~Tex3D();

	void DrawOrthSlice3D(float aaSpaceCoord[][3], unsigned sliceNo, unsigned axis);
	void DrawOrthSlice3D(unsigned sliceNo, unsigned axis);
	void DrawOrthSlice3DBoundary(unsigned sliceNo, unsigned axis);

	void DrawOrthSlice2D(float aLowCoord[], float w, float h, unsigned sliceNo, unsigned axis, bool bNormal);
	void UnprojOrthSlice2D(float aOut[], float aIn[], float aLowCoord[], float w, float h, unsigned axis, bool bNormal);
	void SetTexCoord(float aaTexCoord[][3], unsigned sliceNo, unsigned axis, float bnd=0);

	Vect3D<unsigned> GetDim();

	GLuint GetTexID();

	//void CellRef(unsigned x, unsigned y, unsigned z)
	void Update(int xOffset, int yOffset, int zOffset, unsigned xDim, unsigned yDim, unsigned zDim);

private:
	void SetTexture();
	void ComputeProj(float &xOrg, float &yOrg, float &xLen, float &yLen, float aLowCoord[], float w, float h, unsigned axis, bool bNormal);

	const unsigned m_xDim;
	const unsigned m_yDim;
	const unsigned m_zDim;
	const unsigned m_cDim;

	const bool m_bF32;
	const bool m_bLinear;

	GLuint m_tex;
	float* const m_pCell;
};

#endif