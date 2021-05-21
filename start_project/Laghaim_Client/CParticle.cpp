#include "stdafx.h"
#include "D3DApp.h"
#include "D3DUtil.h"

#include "CParticle.h"
#include "CParticleSystem.h"

#define GRAVITY D3DVECTOR(0.0f, -9.8f, 0.0f)

CParticle::CParticle()
	: m_fLifetime(0)
	, m_fSizeDelta(0)
	, m_fAlpha(0)
	, m_fAlphaDelta(0)
	, m_fGravity(0)
	, m_fGravityDelta(0)
	, m_bOrbiting(0)
	, m_pParent(NULL)
{
	// should have used the newer C++ method of parameter initialization
	// but wanted to keep it simple for those new to C++
	m_fAge = 0.0f;
	m_fSize = 1.0f;
	m_d3dcColor.r = 1.0f;
	m_d3dcColor.g = 0.0f;
	m_d3dcColor.b = 0.0f;
	m_d3dcColor.a = 0.5f;
}

CParticle::~CParticle()
{}

//	Update the given particle with the passed in time delta
//  If the particle dies in the update, False is returned
bool CParticle::Update( float fTimeDelta )
{
	// Some systems may have particles with no Lifetime
	// i.e. they could have to collide with terrain
	// or they could just always be alive if the system
	// had a fixed # of particles alive
	if ( m_fAge + fTimeDelta >= m_fLifetime )
	{
		m_fAge = -1.0f;
		return FALSE;
	}
	else
	{
		// everything ages so add the time elapsed
		m_fAge += fTimeDelta;
		// Our current location will be our previous location next frame
		m_d3dvPrevLocation = m_d3dvLocation;
		// this moves the particle using the last known velocity and the time that has passed
		m_d3dvLocation += m_d3dvVelocity * fTimeDelta;
		// whether or not our parent system allows ground plane collision.
		// this really should be a per particle setting, not an attribute of the parent system
		// This isn't how you would normally do collision.  This snaps any particle below the
		// ground plane up to ground level.  Ground collision is just to show what a fountain
		// or other effects might look like
		if ( m_pParent && m_pParent->IsColliding() )
		{
			if ( m_d3dvLocation.y < -25.0f )
			{
				// if they are moving at a decent rate vertically reflect them
				if ( m_d3dvVelocity.y > -5.0f )  // -5.0f <= velocity.y <= 0.0f
				{
					m_d3dvLocation.y = -25.0f;
					m_d3dvVelocity.y = 0.0f;
				}
				// otherwise, let them rest on the ground plance
				else
				{
					m_d3dvLocation.y = m_d3dvPrevLocation.y;
					m_d3dvVelocity.y = -m_d3dvVelocity.y*0.5f;
				}
			}
		}

		// note that GRAVITY is hardcoded up above.  Gravity could be a vector per particle system
		// to simulate a crosswind that blew the particles sideways as well.
		m_d3dvVelocity += ( GRAVITY * m_fGravity * fTimeDelta );

		// if this particle's parent system supports Attraction
		if ( m_pParent && m_pParent->IsAttractive() )
		{
			D3DVECTOR AttractLocation;
			// Find out where our Parent is located so we can track it
			m_pParent->GetLocation(AttractLocation);
			// The attractors could be points in space to create some cool effects.
			//AttractLocation = D3DVECTOR(50,50,50);
			// calculate the vector between the particle and the attractor
			D3DVECTOR AttractDir = AttractLocation - m_d3dvLocation;
			// We can turn off attraction for certain axes
			// AttractDir.y = 0; // this would simulate a cyclone type effect

			// note that this is NOT accurate gravitation.   We don't even look at the distance
			// between the 2 locations !!!    But what can I say, it looks good. You can find the
			// formula for Gravitation in any good physics book.  It uses the masses of the 2 objects
			// the distance between them and the Gravitational constant

			// if you decide to use this simple method you really should use a variable multiplier
			// instead of a hardcoded value like 25.0f
			m_d3dvVelocity += Normalize(AttractDir) * 25.0f * fTimeDelta;
		}

		// adjust current Color from calculated Deltas and time elapsed.
		m_d3dcColor.r += m_d3dcColorDelta.r * fTimeDelta;
		m_d3dcColor.g += m_d3dcColorDelta.g * fTimeDelta;
		m_d3dcColor.b += m_d3dcColorDelta.b * fTimeDelta;

		// adjust current Alpha yadda yadda yadda
		m_fAlpha += m_fAlphaDelta * fTimeDelta;
		// adjust current Size
		m_fSize += m_fSizeDelta * fTimeDelta;
		// adjust current Gravity
		m_fGravity += m_fGravityDelta * fTimeDelta;

	}
	// if we got this far, everything is probably cool
	return TRUE;
}
