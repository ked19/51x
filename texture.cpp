#include "texture.h"

GLint SetFilterPara(bool bLinear)
{
	if(bLinear)
	{
		return GL_LINEAR;
	}
	else
	{
		return GL_NEAREST;
	}
}

GLint SetInterFormat(bool bF32, unsigned cDim)
{
	if(bF32)
	{
		if(cDim == 1)
		{
			return GL_R32F;
		}
		else if(cDim == 3)
		{
			return GL_RGB32F;
		}
		else if(cDim == 4)
		{
			return GL_RGBA32F;
		}
		else
		{
			assert(0);
		}
	}
	else
	{
		if(cDim == 1)
		{
			return GL_LUMINANCE;
		}
		else if(cDim == 3)
		{
			return GL_RGB;
		}
		else if(cDim == 4)
		{
			return GL_RGBA;
		}
		else 
		{
			assert(0);
		}
	}
	return 0;
}

GLenum SetFormat(unsigned cDim)
{
	if(cDim == 1)
	{
		return GL_LUMINANCE;
	}
	else if(cDim == 3)
	{
		return GL_RGB;
	}
	else if(cDim == 4)
	{
		return GL_RGBA;
	}
	else
	{
		assert(0);
	}
	return 0;
}

//*************************************************************************************************

void Tex1D::SetTexture()
{
	glEnable(GL_TEXTURE_1D);
	glGenTextures(1, &m_tex);
	glBindTexture(GL_TEXTURE_1D, m_tex);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	GLint filterPara = SetFilterPara(m_bLinear);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, filterPara);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, filterPara);

	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	GLint interFormat = SetInterFormat(m_bF32, m_cDim);
	GLenum format = SetFormat(m_cDim);
	glTexImage1D(GL_TEXTURE_1D, 0, interFormat, m_xDim, 0, format, GL_FLOAT, m_pCell);

	glDisable(GL_TEXTURE_1D);
}

Tex1D::Tex1D(float *pData, unsigned xDim, unsigned cDim, bool bNormal, bool bF32, bool bLinear)
	:m_xDim(xDim), m_cDim(cDim)
	,m_bF32(bF32), m_bLinear(bLinear), m_pCell(pData)
{
	unsigned size = xDim * cDim;
	
	if(bNormal)
	{
		float maxCell = -1e10;
		for(unsigned i=0; i<size; i++)
		{
			if(m_pCell[i] > maxCell)
			{
				maxCell = m_pCell[i];
			}
			else {}
		}

		if(maxCell != 0)
		{
			for(unsigned i=0; i<size; i++)
			{
				m_pCell[i] /= maxCell;
			}
		}
		else {}
	}
	else {}

	SetTexture();
}

Tex1D::~Tex1D()
{}

//*************************************************************************************************

GLuint Tex1D::GetTexID()
{
	return m_tex;
}

unsigned Tex1D::GetDim()
{
	return m_xDim;
}

//*************************************************************************************************
//
//*************************************************************************************************

void Tex3D::SetTexture()
{
	glEnable(GL_TEXTURE_3D);
	glGenTextures(1, &m_tex);
	glBindTexture(GL_TEXTURE_3D, m_tex);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	GLint filterPara = SetFilterPara(m_bLinear);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, filterPara);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, filterPara);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	GLint interFormat = SetInterFormat(m_bF32, m_cDim);
	GLenum format = SetFormat(m_cDim);
	glTexImage3D(GL_TEXTURE_3D, 0, interFormat, m_xDim, m_yDim, m_zDim, 0, format, GL_FLOAT, m_pCell);

	glDisable(GL_TEXTURE_3D);
}

Tex3D::Tex3D(float *pData, unsigned xDim, unsigned yDim, unsigned zDim, unsigned cDim, bool bNormal, bool bF32, bool bLinear)
	:m_xDim(xDim), m_yDim(yDim), m_zDim(zDim), m_cDim(cDim)
	,m_bF32(bF32), m_bLinear(bLinear), m_pCell(pData)
{
	unsigned size = xDim * yDim * zDim * cDim;
	
	if(bNormal)
	{
		float maxCell = -1e10;
		for(unsigned i=0; i<size; i++)
		{
			if(m_pCell[i] > maxCell)
			{
				maxCell = m_pCell[i];
			}
			else {}
		}

		if(maxCell != 0)
		{
			for(unsigned i=0; i<size; i++)
			{
				m_pCell[i] /= maxCell;
			}
		}
		else {}
	}
	else {}

	SetTexture();
}

Tex3D::~Tex3D()
{}

//*************************************************************************************************

GLuint Tex3D::GetTexID()
{
	return m_tex;
}

Vect3D<unsigned> Tex3D::GetDim()
{
	Vect3D<unsigned> dim(m_xDim, m_yDim, m_zDim);
	return dim;
}

void Tex3D::SetTexCoord(float aaTexCoord[][3], unsigned sliceNo, unsigned axis, float bnd)
{
	if(axis == 0)
	{
		float xR = (float)sliceNo / (m_xDim - 1);
		aaTexCoord[0][0] = xR;	aaTexCoord[0][1] = 0-bnd;	 aaTexCoord[0][2] = 1.f+bnd;
		aaTexCoord[1][0] = xR;  aaTexCoord[1][1] = 1.f+bnd;  aaTexCoord[1][2] = 1.f+bnd;
		aaTexCoord[2][0] = xR;  aaTexCoord[2][1] = 1.f+bnd;  aaTexCoord[2][2] = 0-bnd;
		aaTexCoord[3][0] = xR;  aaTexCoord[3][1] = 0-bnd;    aaTexCoord[3][2] = 0-bnd;
	}
	else if(axis == 1)
	{
		float yR = (float)sliceNo / (m_yDim - 1);
		aaTexCoord[0][0] = 0-bnd;	 aaTexCoord[0][1] = yR;	 aaTexCoord[0][2] = 1.f+bnd;
		aaTexCoord[1][0] = 1.f+bnd;  aaTexCoord[1][1] = yR;  aaTexCoord[1][2] = 1.f+bnd;
		aaTexCoord[2][0] = 1.f+bnd;  aaTexCoord[2][1] = yR;  aaTexCoord[2][2] = 0-bnd;
		aaTexCoord[3][0] = 0-bnd;    aaTexCoord[3][1] = yR;  aaTexCoord[3][2] = 0-bnd;
	}
	else if(axis == 2)
	{
		float zR = (float)sliceNo / (m_zDim - 1);
		aaTexCoord[0][0] = 0-bnd;	 aaTexCoord[0][1] = 0-bnd;	  aaTexCoord[0][2] = zR;
		aaTexCoord[1][0] = 1.f+bnd;  aaTexCoord[1][1] = 0-bnd;    aaTexCoord[1][2] = zR;
		aaTexCoord[2][0] = 1.f+bnd;  aaTexCoord[2][1] = 1.f+bnd;  aaTexCoord[2][2] = zR;
		aaTexCoord[3][0] = 0-bnd;    aaTexCoord[3][1] = 1.f+bnd;  aaTexCoord[3][2] = zR;
	}
	else 
	{
		assert(0);
	}
}

void Tex3D::DrawOrthSlice3D(float aaSpaceCoord[][3], unsigned sliceNo, unsigned axis)
{
	float aaTexCoord[4][3];
	SetTexCoord(aaTexCoord, sliceNo, axis);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_TEXTURE_3D);
	glBindTexture(GL_TEXTURE_3D, m_tex);
	glBegin(GL_QUADS);
		glTexCoord3fv(aaTexCoord[0]);			glVertex3fv(aaSpaceCoord[0]);
		glTexCoord3fv(aaTexCoord[1]);			glVertex3fv(aaSpaceCoord[1]);
		glTexCoord3fv(aaTexCoord[2]);			glVertex3fv(aaSpaceCoord[2]);
		glTexCoord3fv(aaTexCoord[3]);			glVertex3fv(aaSpaceCoord[3]);
	glEnd();
	glDisable(GL_TEXTURE_3D);
}

void Tex3D::DrawOrthSlice3D(unsigned sliceNo, unsigned axis)
{
	float aaSpaceCoord[4][3];
	SetTexCoord(aaSpaceCoord, sliceNo, axis);
	DrawOrthSlice3D(aaSpaceCoord, sliceNo, axis);
}

void Tex3D::DrawOrthSlice3DBoundary(unsigned sliceNo, unsigned axis)
{
	float aaSpaceCoord[4][3];
	SetTexCoord(aaSpaceCoord, sliceNo, axis, 0.01f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
		glVertex3fv(aaSpaceCoord[0]);			
		glVertex3fv(aaSpaceCoord[1]);			
		glVertex3fv(aaSpaceCoord[2]);			
		glVertex3fv(aaSpaceCoord[3]);
	glEnd();
}

void Tex3D::ComputeProj(float &xOrg, float &yOrg, float &xLen, float &yLen, float aLowCoord[], float w, float h, unsigned axis, bool bNormal)
{
	if(bNormal)
	{
		unsigned xDim, yDim;
		if(axis == 0)
		{
			xDim = m_yDim;
			yDim = m_zDim;
		}
		else if(axis == 1)
		{
			xDim = m_xDim;
			yDim = m_zDim;
		}
		else if(axis == 2)
		{
			xDim = m_xDim;
			yDim = m_yDim;
		}
		else
		{
			assert(0);
		}

		float xScl = w / xDim;
		float yScl = h / yDim;
		float scl = xScl<yScl? xScl: yScl;

		xLen = xDim * scl;
		yLen = yDim * scl;
		xOrg = (w - xLen) * 0.5f + aLowCoord[0];
		yOrg = (h - yLen) * 0.5f + aLowCoord[1];
	}
	else
	{
		xLen = w;
		yLen = h;
		xOrg = aLowCoord[0];
		yOrg = aLowCoord[1];
	}
}

void Tex3D::DrawOrthSlice2D(float aLowCoord[], float w, float h, unsigned sliceNo, unsigned axis, bool bNormal)
{
	float xOrg, yOrg;
	float xLen, yLen;
	ComputeProj(xOrg, yOrg, xLen, yLen, aLowCoord, w, h, axis, bNormal);

	float xEnd = xOrg + xLen;// - 1.f;
	float yEnd = yOrg + yLen;// - 1.f;
	float aaSpaceCoord[][3] = {{xOrg, yOrg, 0}, {xEnd, yOrg, 0}, {xEnd, yEnd, 0}, {xOrg, yEnd, 0}};
	DrawOrthSlice3D(aaSpaceCoord, sliceNo, axis);
}

void Tex3D::UnprojOrthSlice2D(float aOut[], float aIn[], float aLowCoord[], float w, float h, unsigned axis, bool bNormal)
{
	float xOrg, yOrg;
	float xLen, yLen;
	ComputeProj(xOrg, yOrg, xLen, yLen, aLowCoord, w, h, axis, bNormal);

	unsigned xDim, yDim;
	if(axis == 0)
	{
		xDim = m_yDim;
		yDim = m_zDim;
	}
	else if(axis == 1)
	{
		xDim = m_xDim;
		yDim = m_zDim;
	}
	else if(axis == 2)
	{
		xDim = m_xDim;
		yDim = m_yDim;
	}
	else
	{
		assert(0);
	}
	aOut[0] = (aIn[0] - xOrg) * xDim / xLen;
	aOut[1] = (aIn[1] - yOrg) * yDim / yLen;
}

void Tex3D::Update(int xOffset, int yOffset, int zOffset, unsigned xDim, unsigned yDim, unsigned zDim)
{
	glBindTexture(GL_TEXTURE_3D, m_tex);
	GLenum format = SetFormat(m_cDim);
	glTexSubImage3D(GL_TEXTURE_3D, 0, xOffset, yOffset, zOffset, 
		m_xDim, m_yDim, m_zDim, format, GL_FLOAT, m_pCell);
}

//*************************************************************************************************
//
//*************************************************************************************************

void Tex2D::SetTexture()
{
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &m_tex);
	glBindTexture(GL_TEXTURE_2D, m_tex);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	GLint filterPara = SetFilterPara(m_bLinear);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterPara);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterPara);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	GLint interFormat = SetInterFormat(m_bF32, m_cDim);
	GLenum format = SetFormat(m_cDim);
	glTexImage2D(GL_TEXTURE_2D, 0, interFormat, m_xDim, m_yDim, 0, format, GL_FLOAT, m_pCell);

	glDisable(GL_TEXTURE_2D);
}


Tex2D::Tex2D(unsigned xDim, unsigned yDim, unsigned cDim, bool bF32, bool bLinear)
	:m_xDim(xDim), m_yDim(yDim), m_cDim(cDim)
	,m_bF32(bF32), m_bLinear(bLinear), m_pCell(0)
{
	unsigned size = xDim * yDim * cDim;

	delete []m_pCell;
	m_pCell = new float[size];
	for(unsigned i=0; i<size; i++)
	{
		m_pCell[i] = 0;
	}

	SetTexture();
}

Tex2D::Tex2D(float *pData, unsigned xDim, unsigned yDim, unsigned cDim, bool bNormal, bool bF32, bool bLinear)
	:m_xDim(xDim), m_yDim(yDim), m_cDim(cDim)
	,m_bF32(bF32), m_bLinear(bLinear), m_pCell(pData)
{
	unsigned size = xDim * yDim * cDim;

	//delete []m_pCell;
	//m_pCell = pData;

	if(bNormal)
	{
		float maxCell = -1e10;
		for(unsigned i=0; i<size; i++)
		{
			if(m_pCell[i] > maxCell)
			{
				maxCell = m_pCell[i];
			}
			else {}
		}

		if(maxCell != 0)
		{
			for(unsigned i=0; i<size; i++)
			{
				m_pCell[i] /= maxCell;
			}
		}
		else {}
	}
	else {}

	SetTexture();
}

Tex2D::~Tex2D()
{
	delete []m_pCell;
}

//*************************************************************************************************

void Tex2D::SetTexCoord(float aaTexCoord[][2], float bnd)
{	
	aaTexCoord[0][0] = 0-bnd;	 aaTexCoord[0][1] = 0-bnd;	  
	aaTexCoord[1][0] = 1.f+bnd;  aaTexCoord[1][1] = 0-bnd;    
	aaTexCoord[2][0] = 1.f+bnd;  aaTexCoord[2][1] = 1.f+bnd; 
	aaTexCoord[3][0] = 0-bnd;    aaTexCoord[3][1] = 1.f+bnd; 
}

void Tex2D::DrawRec3D(float aaSpaceCoord[][3])
{
	float aaTexCoord[4][2];
	SetTexCoord(aaTexCoord);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_tex);
	glBegin(GL_QUADS);
		glTexCoord2fv(aaTexCoord[0]);			glVertex3fv(aaSpaceCoord[0]);
		glTexCoord2fv(aaTexCoord[1]);			glVertex3fv(aaSpaceCoord[1]);
		glTexCoord2fv(aaTexCoord[2]);			glVertex3fv(aaSpaceCoord[2]);
		glTexCoord2fv(aaTexCoord[3]);			glVertex3fv(aaSpaceCoord[3]);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void Tex2D::FindDrawRange(float aNewLow[], float aNewHigh[], float aLowCoord[], float w, float h)
{
		float xScl = w / m_xDim;
		float yScl = h / m_yDim;
		float scl = xScl<yScl? xScl: yScl;

		float xLen = m_xDim * scl;
		float yLen = m_yDim * scl;
		float xOrg = (w - xLen) * 0.5f + aLowCoord[0];
		float yOrg = (h - yLen) * 0.5f + aLowCoord[1];
		float xEnd = xOrg + xLen;// - 1.f;
		float yEnd = yOrg + yLen;// - 1.f;

		aNewLow[0] = xOrg;		aNewLow[1] = yOrg;
		aNewHigh[0] = xEnd;		aNewHigh[1] = yEnd;
}

void Tex2D::DrawRec2D(float aLowCoord[], float w, float h, bool bNormal)
{
	float xOrg, yOrg;
	float xLen, yLen;
	if(bNormal)
	{
		float xScl = w / m_xDim;
		float yScl = h / m_yDim;
		float scl = xScl<yScl? xScl: yScl;

		xLen = m_xDim * scl;
		yLen = m_yDim * scl;
		xOrg = (w - xLen) * 0.5f + aLowCoord[0];
		yOrg = (h - yLen) * 0.5f + aLowCoord[1];
	}
	else
	{
		xLen = w;
		yLen = h;
		xOrg = aLowCoord[0];
		yOrg = aLowCoord[1];
	}

	float xEnd = xOrg + xLen; //- 1.f;
	float yEnd = yOrg + yLen; //- 1.f;
	float aaSpaceCoord[][3] = {{xOrg, yOrg, 0}, {xEnd, yOrg, 0}, {xEnd, yEnd, 0}, {xOrg, yEnd, 0}};
	DrawRec3D(aaSpaceCoord);
}

void Tex2D::SetCell(float val, unsigned x, unsigned y, unsigned c)
{
	unsigned pos = (y*m_xDim + x) * m_cDim + c;
	m_pCell[pos] = val;
}

float Tex2D::GetCell(unsigned x, unsigned y, unsigned c)
{
	unsigned pos = (y*m_xDim + x) * m_cDim + c;
	return m_pCell[pos];
}

Vect2D<unsigned> Tex2D::GetDim()
{
	Vect2D<unsigned> dim(m_xDim, m_yDim);
	return dim;
}

void Tex2D::Update()
{	
	glEnable(GL_TEXTURE_2D);

	//glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_tex);
	//glActiveTexture(GL_TEXTURE1);

	//GLint interFormat = SetInterFormat(m_bF32, m_cDim);
	//GLenum format = SetFormat(m_cDim);
	//glTexImage2D(GL_TEXTURE_2D, 0, interFormat, m_xDim, m_yDim, 0, format, GL_FLOAT, m_pCell);


	GLenum format = SetFormat(m_cDim);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_xDim, m_yDim, format, GL_FLOAT, m_pCell);

	glDisable(GL_TEXTURE_2D);
}

GLuint Tex2D::GetTexID()
{
	return m_tex;
}

//***********************************************

GLenum DecideFormat(unsigned cDim)
{
	if(cDim == 1)
	{
		return GL_RED;
	}
	else if(cDim == 3)
	{
		return GL_RGB;
	}
	else if(cDim == 4)
	{
		return GL_RGBA;
	}
	else
	{
		assert(0);
		return 0;
	}
}

void Tex2D::ReadFrame(int aLow[])
{
	GLenum format = DecideFormat(m_cDim);
	glReadPixels(aLow[0], aLow[1], m_xDim, m_yDim, format, GL_FLOAT, m_pCell);
}

void Tex2D::Read(MyImg &img)
{
	Vect3D<unsigned> dimImg = img.GetDim();
	assert(
		dimImg.m_x==m_xDim &&
		dimImg.m_y==m_yDim &&
		dimImg.m_z==m_cDim);
	
	for(unsigned y=0; y<m_yDim; y++)
	{
		for(unsigned x=0; x<m_xDim; x++)
		{
			for(unsigned c=0; c<m_cDim; c++)
			{
				float val = (float)img.CellVal(x, y, c);
				SetCell(val, x, y, c);
			}
		}
	}
}

void Tex2D::GetCell(Layer &lyr)
{
	Vect3D<unsigned> dimLyr = lyr.GetDim();
	assert(dimLyr.m_x==m_xDim &&
		   dimLyr.m_y==m_yDim &&
		   dimLyr.m_z==m_cDim);

	lyr.CopyFrom_zFirst(m_pCell);
}