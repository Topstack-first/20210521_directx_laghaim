#include "stdafx.h"
#include "LHUI_Base.h"

LHUI_Base::LHUI_Base(void)
	:	m_visible(true)
	,	m_disable(false)
	,	m_x(0)
	,	m_y(0)
	,	m_w(0)
	,	m_h(0)
{
	_UpdateTransform();
}

LHUI_Base::~LHUI_Base(void)
{
}
