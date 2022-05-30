#pragma once

#include "smsdk_ext.h"

class CNavArea;
class CNavLadder;
class CFuncElevator;

class CSurvivorBotNavAvoidFix : public SDKExtension, public IConCommandBaseAccessor
{
public:
	/**
	 * @brief This is called after the initial loading sequence has been processed.
	 *
	 * @param error		Error message buffer.
	 * @param maxlen	Size of error message buffer.
	 * @param late		Whether or not the module was loaded after map load.
	 * @return			True to succeed loading, false to fail.
	 */
	virtual bool SDK_OnLoad(char *error, size_t maxlen, bool late) override;

	/**
	 * @brief This is called once all known extensions have been loaded.
	 * Note: It is is a good idea to add natives here, if any are provided.
	 */
	// virtual void SDK_OnAllLoaded() override;

	/**
	 * @brief This is called right before the extension is unloaded.
	 */
	virtual void SDK_OnUnload() override;
#ifdef SMEXT_CONF_METAMOD
	/**
	 * @brief Called when Metamod is attached, before the extension version is called.
	 *
	 * @param error			Error buffer.
	 * @param maxlen		Maximum size of error buffer.
	 * @param late			Whether or not Metamod considers this a late load.
	 * @return				True to succeed, false to fail.
	 */
	virtual bool SDK_OnMetamodLoad(ISmmAPI *ismm, char *error, size_t maxlen, bool late) override;
#endif
	// Flags is a combination of FCVAR flags in cvar.h.
	// hOut is filled in with a handle to the variable.
	virtual bool RegisterConCommandBase(ConCommandBase *pVar) override;

private:
	void *m_pfn_SurvivorBotPathCost_vtable;

	int m_nSHookID;
	int m_nOffset_NextBot_player_pointer;

	float Hook_SurvivorBotPathCost_FnCallOp_Post(CNavArea *pArea, CNavArea *pFromArea, const CNavLadder *pLadder, const CFuncElevator *pElevator, float flLength);

public:
	IForward *m_pFwd_CalcSurvivorBotPathCost = nullptr;
};