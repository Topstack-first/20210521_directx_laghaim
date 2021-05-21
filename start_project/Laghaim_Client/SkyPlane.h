#ifndef _SKY_PLANE_H_
#define _SKY_PLANE_H_

#include "Config.h"

#define SKYPLANE_DIV	20
#define SKYPLANE_TIME	50000

class IndexedTexture;

class CSkyPlane
{
public:
	CSkyPlane();
	CSkyPlane(IndexedTexture *pIndexedTexture);
	virtual ~CSkyPlane();

	float m_PlanetRadius;
	float m_AtmosphereRadius;
	float m_HTileFactor;
	float m_VTileFactor;

	D3DLVERTEX m_Vertices[(SKYPLANE_DIV+1)*(SKYPLANE_DIV+1)];
	unsigned short m_Indices[SKYPLANE_DIV*SKYPLANE_DIV*6];
	float m_tU[(SKYPLANE_DIV+1)*(SKYPLANE_DIV+1)];

	BOOL Init(float planetRadius, float atmosphereRadius, float hTilefactor, float vTilefactor, float scale);
	void SetDevice() {}
	BOOL SetupVertices(float scale);
	BOOL SetupIndices();
	void Render(float x, float y, float z, BOOL bFog=TRUE);
	void SetFog(BOOL bFog);
};

#endif // _SKY_PLANE_H_
