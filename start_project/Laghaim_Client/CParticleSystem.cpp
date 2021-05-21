#include "stdafx.h"
//--------------------------------------------------------------------------
//  File:       CParticleSystem.cpp
//  Contents:   Implementation of CParticleSystem
//	To do:		Variance is not accessible through GUI.
//				Don't forget the importance of the variance variables
//---------------------------------------------------------------------------
#include "CParticleSystem.h"
#include "D3DApp.h"
#include "D3DUtil.h"
#include "D3DMath.h"
#include "main.h"

//-----------------------------------------------------------------------------
//	CParticleSystem::CParticleSystem
//-----------------------------------------------------------------------------
CParticleSystem::CParticleSystem()
{
	//should have used the new C++ initialization methods for members (which are faster), but I wanted to keep
	// the code readable to those new to C++
	m_fAge = 0.0f;
	m_fTimeLastUpdate = 0.0f;
	m_fEmissionResidue =  0.0f;
	m_d3dvDirection = D3DVECTOR(0,1,0);

	// since we have a previous location , we have to set that to the current location, otherwise
	// our first update could have erroneous data in the PrevLocation field
	m_d3dvLocation = m_d3dvPrevLocation = D3DVECTOR( 0.0f, 0.0f, 0.0f);

	// Initialize the 4 vertices of our mesh to some acceptable values
	// D3DVECTOR(-1.0f,-1.0f, 0.0f ) is the location of the vertex in local space
	// 0x40ffffff is the D3DCOLOR value. ( which doesn't matter much since we recalculate it every frame.
	//     basically it's   0xaarrggbb where aa = alpha, rr = red...  so this is white with 0.25 opacity (0x40/0xff) = 64/255 ~= 0.25
	// 0 is the specular value which we don't use
	// 0.0f, 1.0f are the UV coordinates for the vertex.  this tells the renderer how to map the texture onto triangles we render
	m_Mesh[0] = D3DLVERTEX( D3DVECTOR(-1.0f,-1.0f, 0.0f ), 0x40ffffff, 0, 0.0f, 1.0f );
	m_Mesh[1] = D3DLVERTEX( D3DVECTOR(-1.0f, 1.0f, 0.0f ), 0x40ffffff, 0, 0.0f, 0.0f );
	m_Mesh[2] = D3DLVERTEX( D3DVECTOR( 1.0f,-1.0f, 0.0f ), 0x40ffffff, 0, 1.0f, 1.0f );
	m_Mesh[3] = D3DLVERTEX( D3DVECTOR( 1.0f, 1.0f, 0.0f ), 0x40ffffff, 0, 1.0f, 0.0f );
}


//-----------------------------------------------------------------------------
//	CParticleSystem::~CParticleSystem
//-----------------------------------------------------------------------------
CParticleSystem::~CParticleSystem()
{}

//-----------------------------------------------------------------------------
//	CParticleSystem::Update
//	Always returns TRUE for our version, but there are various reasons why
//  you may want to bail out and return false.
//-----------------------------------------------------------------------------
bool CParticleSystem::Update(float fTime)
{
	// fTime is absolute time.  So we need to calculate the relative time displacement or fTimeDelta that has elapesed
	float fTimeDelta = fTime - m_fTimeLastUpdate;

	m_fTimeLastUpdate = fTime;

	// sort of a hack (or nice screen saver effect) to test out a moving particle system
	if (m_bIsMoving)
	{
		static float fThetaHoriz = 0.0f;
		static float fThetaVert = 180.0f;

		fThetaHoriz += 50.0f * fTimeDelta;
		fThetaVert += 30.0f * fTimeDelta;

		if ( fThetaHoriz > 359.0f )
			fThetaHoriz = 0.0f;

		if ( fThetaVert > 359.0f )
			fThetaVert = 0.0f;

		m_d3dvLocation.x = 30.0f * (float)cos( fThetaHoriz * 0.01745f );
		m_d3dvLocation.y = 50.0f + 30.0f * (float)sin( fThetaVert * 0.01745f );
		m_d3dvLocation.z = 30.0f * (float)sin( fThetaHoriz * 0.01745f );

	}
	else
	{
		// this will reset it every frame, but most importantly it will reset it to the origin if it isn't Moving
		m_d3dvLocation.x = 0.0f;
		m_d3dvLocation.y = 10.0f;
		m_d3dvLocation.z = 0.0f;

	}

	// clear our counter variable before counting how many particles are alive
	m_uParticlesAlive = 0;

	// update all particles that we own
	for ( int i=0; i<MAX_PARTICLES; i++ )
	{
		if (m_rParticles[i].m_fAge >= 0.0f )
			if ( m_rParticles[i].Update( fTimeDelta )) // Note that the update function returns FALSE if the particle died
				m_uParticlesAlive++;
	}

	// calculate how many particles we should create from ParticlesPerSec and time elapsed taking the
	// previous frame's EmissionResidue into account.
	float fParticlesNeeded = m_uParticlesPerSec * fTimeDelta + m_fEmissionResidue;

	// cast the float fparticlesNeeded to a INT to see how many particles we really need to create.
	unsigned int uParticlesCreated = (unsigned int)fParticlesNeeded;

	if ( !m_bIsSuppressed )
		// remember the difference between what we wanted to create and how many we created.
		m_fEmissionResidue = fParticlesNeeded - uParticlesCreated;
	else
	{
		m_fEmissionResidue = fParticlesNeeded;
		uParticlesCreated = 0;
	}

	// see if actually have any to create
	if ( uParticlesCreated > 0 )
	{
		// loop through all the particles to see if any are available
		for ( int i=0; i<MAX_PARTICLES; i++ )
		{
			// if we have created enough particles, this value will be 0 and we can skip the rest
			// alternatively you could mantain a list of free particles and used particles
			if ( !uParticlesCreated )
				break;

			// of the age is -1.0f then this particle is not in use
			if ( m_rParticles[i].m_fAge < 0.0f )
			{
				// New particle so it's age is 0.0f
				// Actually should be some randomization of fTimeDelta since in a perfect world
				// particles would have been created between our last update and now.
				m_rParticles[i].m_fAge = 0.0f;
				m_rParticles[i].m_fLifetime = m_fLife + RANDOM_NUM * m_fLifeVar;
				Clamp( m_rParticles[i].m_fLifetime, MIN_LIFETIME, MAX_LIFETIME );

				// our start color is going to be the System's StartColor + the System's color variation
				m_rParticles[i].m_d3dcColor.r = m_d3dcColorStart.r + RANDOM_NUM * m_d3dcColorVar.r;
				m_rParticles[i].m_d3dcColor.g = m_d3dcColorStart.g + RANDOM_NUM * m_d3dcColorVar.g;
				m_rParticles[i].m_d3dcColor.b = m_d3dcColorStart.b + RANDOM_NUM * m_d3dcColorVar.b;
				// we don't use the alpha value of the color here
				m_rParticles[i].m_d3dcColor.a = 1.0f;

				// with variation, the Color could overflow
				Clamp( m_rParticles[i].m_d3dcColor.r, 0.0f, 1.0f );
				Clamp( m_rParticles[i].m_d3dcColor.g, 0.0f, 1.0f );
				Clamp( m_rParticles[i].m_d3dcColor.b, 0.0f, 1.0f );

				// calculate a delta so that by the time the particle dies, it will have reached it's "ColorEnd"
				m_rParticles[i].m_d3dcColorDelta.r = (m_d3dcColorEnd.r - m_rParticles[i].m_d3dcColor.r) / m_rParticles[i].m_fLifetime;
				m_rParticles[i].m_d3dcColorDelta.g = (m_d3dcColorEnd.g - m_rParticles[i].m_d3dcColor.g) / m_rParticles[i].m_fLifetime;
				m_rParticles[i].m_d3dcColorDelta.b = (m_d3dcColorEnd.b - m_rParticles[i].m_d3dcColor.b) / m_rParticles[i].m_fLifetime;

				// Calculate our Alpha from the system's AlphaStart and AlphaVar
				m_rParticles[i].m_fAlpha = m_fAlphaStart + RANDOM_NUM * m_fAlphaVar;
				// clamp any overflow
				Clamp( m_rParticles[i].m_fAlpha, MIN_ALPHA, MAX_ALPHA );
				// calculate a delta so that by the time the particle dies, it will have reached it's "AlphaEnd"
				m_rParticles[i].m_fAlphaDelta = (m_fAlphaEnd - m_rParticles[i].m_fAlpha) / m_rParticles[i].m_fLifetime;


				m_rParticles[i].m_fSize = m_fSizeStart + RANDOM_NUM * m_fSizeVar;
				Clamp( m_rParticles[i].m_fSize, MIN_SIZE, MAX_SIZE );
				m_rParticles[i].m_fSizeDelta = (m_fSizeEnd - m_rParticles[i].m_fSize) / m_rParticles[i].m_fLifetime;


				m_rParticles[i].m_fGravity = m_fGravityStart*9.8f + RANDOM_NUM * m_fGravityVar*9.8f;
				// yes this is a lot of repetitive calculation ( * 9.8f ), but it keeps the meaning of m_fgravity clear.
				// It's a percentage of normal gravity.
				Clamp( m_rParticles[i].m_fGravity, MIN_GRAVITY*9.8f, MAX_GRAVITY*9.8f );
				m_rParticles[i].m_fGravityDelta = ( m_fGravityEnd*9.8f - m_rParticles[i].m_fGravity ) / m_rParticles[i].m_fLifetime;

				// since Velocity is a change in position over time, we calculate a Velocity the particle would have to
				// have travelled to move from PrevLocation to Location in fTimeDelta seconds.
				// Then we can calculate points in that linear path by passing in different Time deltas (note the 'RANDOM_NUM * fTimeDelta' below)
				D3DVECTOR tempVelocity = (m_d3dvLocation - m_d3dvPrevLocation)/fTimeDelta;

				// Emit the particles from a location between the last known location and the current location
				m_rParticles[i].m_d3dvLocation = m_d3dvPrevLocation + tempVelocity * RANDOM_NUM * fTimeDelta;

				// This is a simple way of randomizing the location a little
				// You could also emit particles from a source primitive like a Sphere or a Plane instead of a point
				/*
				m_rParticles[i].m_d3dvLocation.x += 2.0f - RANDOM_NUM * 4.0f;
				m_rParticles[i].m_d3dvLocation.y += 2.0f - RANDOM_NUM * 4.0f;
				m_rParticles[i].m_d3dvLocation.z += 2.0f - RANDOM_NUM * 4.0f;
				*/

				// Update the previous location so the next update we can remember where we were
				m_rParticles[i].m_d3dvPrevLocation = m_d3dvLocation;

				// The emitter has a Direction.  This code adds some randomness to that direction so
				// that we don't emit a Line of particles.  For the demo I always assume a randomess of
				// 360 degrees in the Yaw direction.  In your code, you really should allow this to be modified
				// separately from the Pitch.  Normally I would define PI 3.14159 and have a DEG_2_RAD macro
				// but it is used so briefly in this app I didn't
				float RandomYaw = RANDOM_NUM * 3.14159f * 2.0f;
				float RandomPitch = RANDOM_NUM * m_fTheta * 3.14159f / 180.0f ;

				//this uses spherical coordinates to randomize the velocity vector ( or the direction ) of the particle
				m_rParticles[i].m_d3dvVelocity.y = cosf( RandomPitch );
				m_rParticles[i].m_d3dvVelocity.x = sinf(RandomPitch) * cosf(RandomYaw);
				m_rParticles[i].m_d3dvVelocity.z = sinf(RandomPitch) * sinf(RandomYaw);

				// Velocity at this point is just a direction (normalized vector ) and needs to be multiplied by
				// the speed component to be a true velocity
				float fNewSpeed = m_fSpeed + RANDOM_NUM * m_fSpeedVar;
				Clamp( fNewSpeed, MIN_SPEED, MAX_SPEED );
				// Luckily D3D knows how to multiply a Vector by a Scalar
				m_rParticles[i].m_d3dvVelocity *= fNewSpeed ;

				// let the particle know who it's Daddy is
				m_rParticles[i].SetParent(this);

				// decrement the number of needed particles
				uParticlesCreated--;
			}
		}
	}

	///set prevlocation to current location so next frame we know where we were
	m_d3dvPrevLocation = m_d3dvLocation;

	return TRUE;
}


//-----------------------------------------------------------------------------
//	CParticleSystem::Draw
//	Renders all active particles as billboards
//-----------------------------------------------------------------------------
void CParticleSystem::Draw(D3DVECTOR &vPos)
{
	if( !GET_D3DDEVICE() )
		return;

	// Render the particles
	for( DWORD i=0; i<MAX_PARTICLES; i++ )
	{

		// if particle is alive, it's age will be nonZero
		if ( m_rParticles[i].m_fAge >= 0.0f )
		{

			// set the mesh's color to the current particles RGBA value
			m_Mesh[0].color =
				m_Mesh[1].color =
					m_Mesh[2].color =
						m_Mesh[3].color = D3DRGBA(	m_rParticles[i].m_d3dcColor.r,
													m_rParticles[i].m_d3dcColor.g,
													m_rParticles[i].m_d3dcColor.b,
													m_rParticles[i].m_fAlpha);

			D3DMATRIX ViewMatrix;
			// this guarantees a clean Matrix before doing operations with it
			D3DUtil_SetIdentityMatrix( ViewMatrix );
			// get the ViewMatrix from D3D
			GET_D3DDEVICE()->GetTransform( D3DTRANSFORMSTATE_VIEW, &ViewMatrix );

			D3DMATRIX InvViewMatrix;
			// this guarantees a clean Matrix before doing operations with it
			D3DUtil_SetIdentityMatrix( InvViewMatrix );
			// get the Inverse of the ViewMatrix, kinda like your reflection looking at you in the mirror
			D3DMath_MatrixInvert( InvViewMatrix, ViewMatrix );

			// set the translation portion of the InvViewMatrix to the location of the particles in world space
			InvViewMatrix._41 = vPos.x + m_rParticles[i].m_d3dvLocation.x;
			InvViewMatrix._42 = vPos.y + m_rParticles[i].m_d3dvLocation.y;
			InvViewMatrix._43 = vPos.z + m_rParticles[i].m_d3dvLocation.z;

			// set the scale of the particles in local space so when they are transformed they will be the right size
			m_Mesh[0].dvX = -m_rParticles[i].m_fSize;
			m_Mesh[0].dvY = -m_rParticles[i].m_fSize;

			m_Mesh[1].dvX = -m_rParticles[i].m_fSize;
			m_Mesh[1].dvY =  m_rParticles[i].m_fSize;

			m_Mesh[2].dvX =  m_rParticles[i].m_fSize;
			m_Mesh[2].dvY = -m_rParticles[i].m_fSize;

			m_Mesh[3].dvX =  m_rParticles[i].m_fSize;
			m_Mesh[3].dvY =  m_rParticles[i].m_fSize;

			// Set the new world transform and render the particle
			GET_D3DDEVICE()->SetTransform( D3DTRANSFORMSTATE_WORLD, &InvViewMatrix );

			// Note: Should accumulate polygons in an array and then pass them all at once as a D3DPT_TRIANGLELIST
			GET_D3DDEVICE()->DrawPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, m_Mesh, 4, 0 );

		}
	}

}


void CParticleSystem::Move(D3DVECTOR vLocation)
{
	m_d3dvPrevLocation = m_d3dvLocation;
	m_d3dvLocation = vLocation;
}


void CParticleSystem::ChangeParams(const BYTE Keys[])
{

	// Color changes
	if ( Keys[VK_INSERT] )
	{
		if ( Keys[VK_SHIFT] )
			m_d3dcColorEnd.r += 0.05f;
		else
			m_d3dcColorStart.r += 0.05f;
	}

	if ( Keys[VK_DELETE] )
	{
		if ( Keys[VK_SHIFT] )
			m_d3dcColorEnd.r -= 0.05f;
		else
			m_d3dcColorStart.r -= 0.05f;
	}

	if ( Keys[VK_HOME] )
	{
		if ( Keys[VK_SHIFT] )
			m_d3dcColorEnd.g += 0.05f;
		else
			m_d3dcColorStart.g += 0.05f;
	}

	if ( Keys[VK_END] )
	{
		if ( Keys[VK_SHIFT] )
			m_d3dcColorEnd.g -= 0.05f;
		else
			m_d3dcColorStart.g -= 0.05f;
	}


	if ( Keys[VK_PRIOR] )
	{
		if ( Keys[VK_SHIFT] )
			m_d3dcColorEnd.b += 0.05f;
		else
			m_d3dcColorStart.b += 0.05f;
	}

	if ( Keys[VK_NEXT] )
	{
		if ( Keys[VK_SHIFT] )
			m_d3dcColorEnd.b -= 0.05f;
		else
			m_d3dcColorStart.b -= 0.05f;
	}

	// Make sure that any adjustments that we made
	// stay within valid color limits
	Clamp( m_d3dcColorStart.r, 0.0f, 1.0f );
	Clamp( m_d3dcColorStart.g, 0.0f, 1.0f );
	Clamp( m_d3dcColorStart.b, 0.0f, 1.0f );

	Clamp( m_d3dcColorEnd.r, 0.0f, 1.0f );
	Clamp( m_d3dcColorEnd.g, 0.0f, 1.0f );
	Clamp( m_d3dcColorEnd.b, 0.0f, 1.0f );
}


bool CParticleSystem::IsMoving()
{
	return m_bIsMoving;
}


bool CParticleSystem::IsColliding()
{
	return m_bIsColliding;
}


bool CParticleSystem::IsAttractive()
{
	return m_bIsAttractive;
}


void CParticleSystem::GetLocation(D3DVECTOR& vLocation)
{
	vLocation = m_d3dvLocation;
}


unsigned int CParticleSystem::NumParticles()
{
	return m_uParticlesAlive;
}


HRESULT CParticleSystem::LoadCurrentTexture()
{
	return TRUE;
}

