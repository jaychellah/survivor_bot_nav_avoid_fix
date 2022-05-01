#pragma once

#define SMEXT_CONF_NAME			    "[L4D/2] Survivor Bot Nav Avoid Fix"
#define SMEXT_CONF_DESCRIPTION	    "Fixes an issue where survivor bots would not respect areas marked as AVOID"
#define SMEXT_CONF_VERSION		    "1.1.0"
#define SMEXT_CONF_AUTHOR		    "Justin \"Jay\" Chellah"
#define SMEXT_CONF_URL			    "https://github.com/jchellah/survivor_bot_nav_avoid_fix"
#define SMEXT_CONF_LOGTAG		    "SBNAF"
#define SMEXT_CONF_LICENSE		    "GPL"
#define SMEXT_CONF_DATESTRING	    __DATE__
#define SMEXT_CONF_GAMEDATA_FILE	"survivor_bot_nav_avoid_fix"

#define SMEXT_LINK(name) SDKExtension *g_pExtensionIface = name;

#define SMEXT_CONF_METAMOD

#define SMEXT_ENABLE_GAMECONF
#define SMEXT_ENABLE_GAMEHELPERS