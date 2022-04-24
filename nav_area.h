#pragma once

#include "nav_ladder.h"

class CNavArea
{
public:
	static int m_nOffset_m_center;
	static int m_nOffset_m_attributeFlags;

	const Vector &GetCenter( void )
	{
		return *reinterpret_cast<Vector *>(reinterpret_cast<byte *>(this) + m_nOffset_m_center);
	}

	int GetAttributes( void )
	{
		return *reinterpret_cast<NavAttributeType *>(reinterpret_cast<byte *>(this) + m_nOffset_m_attributeFlags);
	}
};