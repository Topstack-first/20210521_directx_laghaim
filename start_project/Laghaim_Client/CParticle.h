#ifndef _CPARTICLE_H_
#define _CPARTICLE_H_

#include "Config.h"

#include "D3DTypes.h"


class CParticle
{

	friend class CParticleSystem;
public:

	// usual suspects, Constructor and Destructor
	CParticle();
	~CParticle();

	// Update method with TimeDelta for time-based movement
	bool Update( float fTimeDelta );

	// Location of particle at last update
	D3DVECTOR		m_d3dvPrevLocation;

	// Current location of particle
	D3DVECTOR		m_d3dvLocation;

	// Current velocity of particle
	D3DVECTOR		m_d3dvVelocity;

	// Current Color and how much color to add over time
	D3DCOLORVALUE	m_d3dcColor;
	D3DCOLORVALUE	m_d3dcColorDelta;

	// Age of particle in seconds
	float		m_fAge;

	// Age at which particle dies
	float		m_fLifetime;

	// size of particle in world units and delta to add over time
	float		m_fSize;
	float		m_fSizeDelta;

	// translucency and delta to add over time
	// this may seem counter-intuitive, but 0.0f Alpha is transparent
	float		m_fAlpha;
	float		m_fAlphaDelta;

	// gravity ratio and delta to add over time
	// this is a proportion of normal gravity and can go negative
	float		m_fGravity;
	float		m_fGravityDelta;

	//fix not used anymore
	float		m_bOrbiting;

	// Set ParticleSystem as parent of particle
	void		SetParent(CParticleSystem* Parent)
	{
		m_pParent = Parent;
	}

private:
	// our parent
	CParticleSystem* m_pParent;


};

#endif // _CPARTICLE_H_