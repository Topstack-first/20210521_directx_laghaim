#ifndef _LH_Decal_H_
#define _LH_Decal_H_

#pragma once

#include "main.h"

class LH_Decal
{
public:
	bool m_initialized;

	D3DVERTEX * m_vertices;
	WORD * m_indices;

	int m_num_vertex;
	int m_num_index;
	int m_half_size;
	float m_size;
	float m_gap;

	float m_x;
	float m_z;

public:
	void Initial();
	void Release();



public:
	void Init(float x, float z, float half_size, int gap);
	void ReInit(float x, float z, float half_size, int gap);
	void UpdateTransform();


public:
	void Render(float dt);

public:
	LH_Decal(void);
	LH_Decal(float x, float z, float half_size, int gap);
	virtual ~LH_Decal(void);
};
#endif // _LH_Decal_H_