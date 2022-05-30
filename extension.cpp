#include "extension.h"
#include "nav_ladder.h"
#include "nav_area.h"
#include "survivorbotpathcost.h"

CSurvivorBotNavAvoidFix g_SurvivorBotNavAvoidFix;

SMEXT_LINK(&g_SurvivorBotNavAvoidFix);

SH_DECL_MANUALHOOK5(MHook_SurvivorBotPathCost_FnCallOp, 0, 0, 0, float, CNavArea *, CNavArea *, const CNavLadder *, const CFuncElevator *, float);

int CNavArea::m_nOffset_m_center = -1;
int CNavArea::m_nOffset_m_attributeFlags = -1;

ICvar *g_pCVar = nullptr;

ConVar SurvivorBotPathAvoidPenalty("sb_path_avoid_penalty", "20.0", FCVAR_CHEAT);

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

	if (!pGameConfig->GetOffset("NextBot player pointer", &m_nOffset_NextBot_player_pointer))
	{
		ke::SafeStrcpy(error, maxlen, "Unable to find gamedata offset entry for \"NextBot player pointer\"");

		gameconfs->CloseGameConfigFile(pGameConfig);

		return false;
	}

	if (!pGameConfig->GetAddress("SurvivorBotPathCost vtable", &m_pfn_SurvivorBotPathCost_vtable))
	{
		ke::SafeStrcpy(error, maxlen, "Unable to find gamedata address entry for \"SurvivorBotPathCost vtable\"");

		gameconfs->CloseGameConfigFile(pGameConfig);

		return false;
	}

	if (m_pfn_SurvivorBotPathCost_vtable == nullptr)
	{
		ke::SafeStrcpy(error, maxlen, "Unable to find signature in binary for gamedata entry \"SurvivorBotPathCost vtable\"");

		gameconfs->CloseGameConfigFile(pGameConfig);

		return false;
	}

	gameconfs->CloseGameConfigFile(pGameConfig);

	m_nSHookID = SH_ADD_MANUALDVPHOOK(MHook_SurvivorBotPathCost_FnCallOp, m_pfn_SurvivorBotPathCost_vtable, SH_MEMBER(this, &CSurvivorBotNavAvoidFix::Hook_SurvivorBotPathCost_FnCallOp_Post), true);

	ConVar_Register(0, this);

	sharesys->RegisterLibrary(myself, "survivor_bot_nav_avoid_fix");

	m_pFwd_CalcSurvivorBotPathCost = forwards->CreateForward("L4D_OnCalcSurvivorBotPathCost", ET_Event, 6, NULL, Param_Cell, Param_Cell, Param_Cell, Param_Float, Param_Cell, Param_FloatByRef);

	return true;
}

void CSurvivorBotNavAvoidFix::SDK_OnUnload()
{
	SH_REMOVE_HOOK_ID(m_nSHookID);

	ConVar_Unregister();

	forwards->ReleaseForward(m_pFwd_CalcSurvivorBotPathCost);
}

bool CSurvivorBotNavAvoidFix::SDK_OnMetamodLoad(ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	GET_V_IFACE_CURRENT(GetEngineFactory, g_pCVar, ICvar, CVAR_INTERFACE_VERSION);

	return true;
}

bool CSurvivorBotNavAvoidFix::RegisterConCommandBase(ConCommandBase *pVar)
{
	/* Always call META_REGCVAR instead of going through the engine. */
	return META_REGCVAR(pVar);
}

float CSurvivorBotNavAvoidFix::Hook_SurvivorBotPathCost_FnCallOp_Post(CNavArea *pArea, CNavArea *pFromArea, const CNavLadder *pLadder, const CFuncElevator *pElevator, float flLength)
{
	SurvivorBotPathCost *_this = META_IFACEPTR(SurvivorBotPathCost);

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

		// SurvivorBot *pBot = static_cast<SurvivorBot *>(m_me);
		CBaseEntity *pBot = reinterpret_cast<CBaseEntity *>(reinterpret_cast<char *>(_this->m_me) - m_nOffset_NextBot_player_pointer);

		float flOrigCost = flCost;

		cell_t nResult = Pl_Continue;

		g_SurvivorBotNavAvoidFix.m_pFwd_CalcSurvivorBotPathCost->PushCell(reinterpret_cast<cell_t>(_this));
		g_SurvivorBotNavAvoidFix.m_pFwd_CalcSurvivorBotPathCost->PushCell(gamehelpers->EntityToBCompatRef(pBot));
		g_SurvivorBotNavAvoidFix.m_pFwd_CalcSurvivorBotPathCost->PushCell(reinterpret_cast<cell_t>(pArea));
		g_SurvivorBotNavAvoidFix.m_pFwd_CalcSurvivorBotPathCost->PushFloat(flDist);
		g_SurvivorBotNavAvoidFix.m_pFwd_CalcSurvivorBotPathCost->PushCell(pArea->GetAttributes());
		g_SurvivorBotNavAvoidFix.m_pFwd_CalcSurvivorBotPathCost->PushFloatByRef(&flCost);
		g_SurvivorBotNavAvoidFix.m_pFwd_CalcSurvivorBotPathCost->Execute(&nResult);

		switch (nResult)
		{
			case Pl_Continue:
				break;
			case Pl_Changed:
				flOrigCost = flCost;
				break;
			case Pl_Handled:
			case Pl_Stop:
				RETURN_META_VALUE(MRES_SUPERCEDE, flCost);
		}

		if (!_this->m_ignorePenalties)
		{
			// if this is an area to avoid, add penalty
			if (pArea->GetAttributes() & NAV_MESH_AVOID)
			{
				const float flAvoidPenalty = SurvivorBotPathAvoidPenalty.GetFloat();
				flOrigCost += flAvoidPenalty * flDist;
			}
		}

		RETURN_META_VALUE(MRES_SUPERCEDE, flOrigCost);
	}

	RETURN_META_VALUE(MRES_IGNORED, 0.0f);
}