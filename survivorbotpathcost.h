#pragma once

class INextBot;
class ILocomotion;

struct SurvivorBotPathCost
{
	/* 0 */ void *__vptr;
	/* 4 */ INextBot *m_me;
	/* 8 */ ILocomotion *m_survivorLocomotion;
	/* 12 */ bool m_allowDamagingAreas;					// Whether to allow traversal of damaging areas, if not ignoring penalties
	/* 13 */ bool m_ignorePenalties;					// Whether to return cost, without any penalties
};