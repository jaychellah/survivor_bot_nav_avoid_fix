#include "extension.h"
#include "nav_ladder.h"
#include "nav_area.h"

CSurvivorBotNavAvoidFix g_SurvivorBotNavAvoidFix;

SMEXT_LINK(&g_SurvivorBotNavAvoidFix);

SH_DECL_MANUALHOOK5(MHook_SurvivorBotPathCost_FnCallOp, 0, 0, 0, float, CNavArea *, CNavArea *, const CNavLadder *, const CFuncElevator *, float);

int CNavArea::m_nOffset_m_center = -1;
int CNavArea::m_nOffset_m_attributeFlags = -1;

bool CSurvivorBotNavAvoidFix::SDK_OnLoad(char *error, size_t maxlen, bool late)
{
	IGameConfig *pGameConfig;

	if (!gameconfs->LoadGameConfigFile(SMEXT_CONF_GAMEDATA_FILE, &pGameConfig, error, sizeof(error)))
	{
		ke::SafeStrcpy(error, maxlen, "Unable to load gamedata file \"" SMEXT_CONF_GAMEDATA_FILE ".txt\"");

		return false;
	}

	if (!pGameConfig->GetOffset("CNavArea::m_center", &CNavArea::m_nOffset_m_center))
	{
		ke::SafeStrcpy(error, maxlen, "Unable to find gamedata offset entry for \"CNavArea::m_center\"");

		gameconfs->CloseGameConfigFile(pGameConfig);

		return false;
	}

	if (!pGameConfig->GetOffset("CNavArea::m_attributeFlags", &CNavArea::m_nOffset_m_attributeFlags))
	{
		ke::SafeStrcpy(error, maxlen, "Unable to find gamedata offset entry for \"CNavArea::m_attributeFlags\"");

		gameconfs->CloseGameConfigFile(pGameConfig);

		return false;
	}

	if (!pGameConfig->GetAddress("SurvivorBotPathCost vtable", &m_pFn_SurvivorBotPathCost_vtable))
	{
		ke::SafeStrcpy(error, maxlen, "Unable to find gamedata address entry for \"SurvivorBotPathCost vtable\"");

		gameconfs->CloseGameConfigFile(pGameConfig);

		return false;
	}

	if (m_pFn_SurvivorBotPathCost_vtable == nullptr)
	{
		ke::SafeStrcpy(error, maxlen, "Unable to find signature in binary for gamedata entry \"SurvivorBotPathCost vtable\"");

		gameconfs->CloseGameConfigFile(pGameConfig);

		return false;
	}

	gameconfs->CloseGameConfigFile(pGameConfig);

	m_nSHookID = SH_ADD_MANUALDVPHOOK(MHook_SurvivorBotPathCost_FnCallOp, m_pFn_SurvivorBotPathCost_vtable, SH_MEMBER(this, &CSurvivorBotNavAvoidFix::Hook_SurvivorBotPathCost_FnCallOp_Post), true);

	return true;
}

void CSurvivorBotNavAvoidFix::SDK_OnUnload()
{
	SH_REMOVE_HOOK_ID(m_nSHookID);
}

float CSurvivorBotNavAvoidFix::Hook_SurvivorBotPathCost_FnCallOp_Post(CNavArea *pArea, CNavArea *pFromArea, const CNavLadder *pLadder, const CFuncElevator *pElevator, float flLength)
{
	float flCost = META_RESULT_ORIG_RET(float);

	if (flCost > 0.0f)
	{
		// compute distance traveled along path so far
		float flDist;

		if (pLadder)
		{
			// ladders are slow to use
			const float flLadderPenalty = 2.0f; // 3.0f;
			flDist = flLadderPenalty * pLadder->m_length;
		}
		else if (flLength > 0.0)
		{
			flDist = flLength;
		}
		else
		{
			flDist = (pArea->GetCenter() - pFromArea->GetCenter()).Length();
		}

		// if this is an area to avoid, add penalty
		if (pArea->GetAttributes() & NAV_MESH_AVOID)
		{
			const float flAvoidPenalty = 20.0f;
			flCost += flAvoidPenalty * flDist;
		}

		RETURN_META_VALUE(MRES_SUPERCEDE, flCost);
	}

	RETURN_META_VALUE(MRES_IGNORED, 0.0f);
}