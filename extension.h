#pragma once

#include "smsdk_ext.h"

class CNavArea;
class CNavLadder;
class CFuncElevator;

#define TEAM_UNASSIGNED 0
#define TEAM_SURVIVOR 2

class CSurvivorBotNavAvoidFix : public SDKExtension
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
    // virtual void SDK_OnAllLoaded(void);

	/**
	 * @brief This is called right before the extension is unloaded.
	 */
	virtual void SDK_OnUnload(void) override;
#ifdef SMEXT_CONF_METAMOD
	/**
	 * @brief Called when Metamod is attached, before the extension version is called.
	 *
	 * @param error			Error buffer.
	 * @param maxlen		Maximum size of error buffer.
	 * @param late			Whether or not Metamod considers this a late load.
	 * @return				True to succeed, false to fail.
	 */
	// virtual bool SDK_OnMetamodLoad(ISmmAPI *ismm, char *error, size_t maxlen, bool late) override;
#endif
private:
	void *m_pFn_SurvivorBotPathCost_vtable;

	int m_nSHookID;

	float Hook_SurvivorBotPathCost_FnCallOp_Post(CNavArea *pArea, CNavArea *pFromArea, const CNavLadder *pLadder, const CFuncElevator *pElevator, float flLength);
};