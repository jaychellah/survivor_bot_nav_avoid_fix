# [L4D/2] Survivor Bot Nav Avoid Fix
SourceMod extension which allows level designers to actually make use of **AVOID** attribute in their **Navmeshes** for survivor bots. This extension can be useful for you if you're planning to prevent bots from taking certain routes, provided bots have others to chose from.

# CVars
- `sb_path_avoid_penalty` (set to `20.0` by default)
  - Adjust this CVar based on your needs. The higher, the more likely bots are to avoid given area

# Docs
- [Navigation Areas and Costs (Unity)]( https://docs.unity3d.com/Manual/nav-AreasAndCosts.html)
- [Custom Pathing Behavior (Unreal Engine)]( https://unrealcommunity.wiki/ai-custom-pathing-how-to-use-nav-modifiers-query-filters-z7fbdsb6)

# Supported Platforms
- Linux (not built yet)
- Windows
