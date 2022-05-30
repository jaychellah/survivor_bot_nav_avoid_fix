# [L4D/2] Survivor Bot Nav Avoid Fix
SourceMod extension which allows level designers to actually make use of **AVOID** attribute in their **Navmeshes** for survivor bots. This extension can be useful for you if you're planning to prevent bots from taking certain routes, provided bots have others to chose from.

On top of that, it provides an **API** so that you can specify your own rules for bot pathfinding.

# CVars
- `sb_path_avoid_penalty` (set to `20.0` by default)
  - Adjust this CVar based on your needs. The higher, the more likely bots are to avoid given area

# API
- Global Forward
  - `L4D_OnCalcSurvivorBotPathCost( SurvivorBotPathCost adrThis, int iBot, Address adrArea, float flDist, NavAttributeType fAreaAttributes, float& flCost )`

Example code:
```
public Action L4D_OnCalcSurvivorBotPathCost( SurvivorBotPathCost adrThis, int iBot, Address adrArea, float flDist, NavAttributeType fAreaAttributes, float& flCost )
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
```

This will tell the bot that areas marked as **JUMP** are too dangerous (since penalty is 100) to traverse and to avoid it, if possible.

**Note**: If you're going to prevent bot from ever going to an area, use `Plugin_Handled` or `Plugin_Stop`!

# Docs
- [Navigation Areas and Costs (Unity)](https://docs.unity3d.com/Manual/nav-AreasAndCosts.html)
- [Custom Pathing Behavior (Unreal Engine)](https://unrealcommunity.wiki/ai-custom-pathing-how-to-use-nav-modifiers-query-filters-z7fbdsb6)

# Supported Platforms
- Linux (not built yet)
- Windows