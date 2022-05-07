#include <sourcemod>

#define REQUIRE_EXTENSIONS
#include <survivor_bot_nav_avoid_fix>

public Action L4D_2_OnCalcSurvivorBotPathCost( SurvivorBotPathCost adrThis, Address adrArea, float flDist, NavAttributeType fAreaAttributes, float& flCost )
{
    if ( adrThis.m_ignorePenalties )
    {
        return Plugin_Continue;
    }

    if ( fAreaAttributes & NAV_MESH_JUMP )
    {
        const float flJumpPenalty = 100.0;
        flCost += flJumpPenalty * flDist;

        return Plugin_Changed;
    }

    return Plugin_Continue;
}