#if defined _survivor_bot_nav_avoid_fix_ext_included
 #endinput
#endif
#define _survivor_bot_nav_avoid_fix_ext_included

enum NavAttributeType
{
	NAV_MESH_INVALID		= 0,
	NAV_MESH_CROUCH			= 0x0000001,				// must crouch to use this node/area
	NAV_MESH_JUMP			= 0x0000002,				// must jump to traverse this area (only used during generation)
	NAV_MESH_PRECISE		= 0x0000004,				// do not adjust for obstacles, just move along area
	NAV_MESH_NO_JUMP		= 0x0000008,				// inhibit discontinuity jumping
	NAV_MESH_STOP			= 0x0000010,				// must stop when entering this area
	NAV_MESH_RUN			= 0x0000020,				// must run to traverse this area
	NAV_MESH_WALK			= 0x0000040,				// must walk to traverse this area
	NAV_MESH_AVOID			= 0x0000080,				// avoid this area unless alternatives are too dangerous
	NAV_MESH_TRANSIENT		= 0x0000100,				// area may become blocked, and should be periodically checked
	NAV_MESH_DONT_HIDE		= 0x0000200,				// area should not be considered for hiding spot generation
	NAV_MESH_STAND			= 0x0000400,				// bots hiding in this area should stand
	NAV_MESH_NO_HOSTAGES	= 0x0000800,				// hostages shouldn't use this area
	NAV_MESH_STAIRS			= 0x0001000,				// this area represents stairs, do not attempt to climb or jump them - just walk up
	NAV_MESH_NO_MERGE		= 0x0002000,				// don't merge this area with adjacent areas
	NAV_MESH_OBSTACLE_TOP	= 0x0004000,				// this nav area is the climb point on the tip of an obstacle
	NAV_MESH_CLIFF			= 0x0008000,				// this nav area is adjacent to a drop of at least CliffHeight

	NAV_MESH_FIRST_CUSTOM	= 0x00010000,				// apps may define custom app-specific bits starting with this value
	NAV_MESH_LAST_CUSTOM	= 0x04000000,				// apps must not define custom app-specific bits higher than with this value

	NAV_MESH_BLOCKED_PROPDOOR	= 0x10000000,				// area is blocked by prop_door_rotating

	NAV_MESH_HAS_ELEVATOR	= 0x40000000,				// area is in an elevator's path
	NAV_MESH_NAV_BLOCKER	= 0x80000000				// area is blocked by nav blocker ( Alas, needed to hijack a bit in the attributes to get within a cache line [7/24/2008 tom])
};

methodmap SurvivorBotPathCost
{
	property Address m_me
	{
		public get()
		{
			return view_as< Address >( LoadFromAddress( view_as< Address >( this ) + view_as< Address >( 4 ), NumberType_Int32 ) );
		}
	}

	property Address m_survivorLocomotion
	{
		public get()
		{
			return view_as< Address >( LoadFromAddress( view_as< Address >( this ) + view_as< Address >( 8 ), NumberType_Int32 ) );
		}
	}

	// Whether to allow traversal of damaging areas, if not ignoring penalties
	property bool m_allowDamagingAreas
	{
		public get()
		{
			return view_as< bool >( LoadFromAddress( view_as< Address >( this ) + view_as< Address >( 12 ), NumberType_Int8 ) );
		}
	}

	// Whether to return cost, without any penalties
	property bool m_ignorePenalties
	{
		public get()
		{
			return view_as< bool >( LoadFromAddress( view_as< Address >( this ) + view_as< Address >( 13 ), NumberType_Int8 ) );
		}
	}
};

/**
 * Called whenever survivor bot is building a path to go somewhere.
 *
 * @param adrThis				This pointer address.
 * @param adrArea				Area to navigate to.
 * @param flDist				Calculated distance traveled along path so far or length, if specified.
 * @param fAreaAttributes		Attributes of area to navigate to.
 * @param flCost				Calculated cost so far.
 *
 * @return						Plugin_Changed to override cost,
 *								Plugin_Handled or Plugin_Stop to block AVOID attribute check and override cost,
 *								Plugin_Continue otherwise.
 */
forward Action L4D_2_OnCalcSurvivorBotPathCost( SurvivorBotPathCost adrThis, Address adrArea, float flDist, NavAttributeType fAreaAttributes, float& flCost );

/**
 * Do not edit below this line!
 */
public Extension __ext_survivor_bot_nav_avoid_fix =
{
	name = "survivor_bot_nav_avoid_fix",
	file = "survivor_bot_nav_avoid_fix.ext",
#if defined AUTOLOAD_EXTENSIONS
	autoload = 1,
#else
	autoload = 0,
#endif
#if defined REQUIRE_EXTENSIONS
	required = 1,
#else
	required = 0,
#endif
};