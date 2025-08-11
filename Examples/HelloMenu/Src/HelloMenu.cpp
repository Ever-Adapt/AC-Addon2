#include "APIEnvir.h"
#include "ACAPinc.h"

#define HELLO_ADDON_NAME        32000
#define HELLO_MENU_STRINGS      32500

static GSErrCode __ACENV_CALL MenuCommandHandler (const API_MenuParams* params)
{
    ACAPI_WriteReport ("Hello, API world!", false);
    return NoError;
}

API_AddonType CheckEnvironment (API_EnvirParams* envir)
{
    RSGetIndString (&envir->addOnInfo.name,        HELLO_ADDON_NAME, 1, ACAPI_GetOwnResModule ());
    RSGetIndString (&envir->addOnInfo.description, HELLO_ADDON_NAME, 2, ACAPI_GetOwnResModule ());
    return APIAddon_Preload;
}

GSErrCode RegisterInterface (void)
{
    return ACAPI_MenuItem_RegisterMenu (HELLO_MENU_STRINGS, 0, MenuCode_UserDef, MenuFlag_Default);
}

GSErrCode Initialize (void)
{
    return ACAPI_MenuItem_InstallMenuHandler (HELLO_MENU_STRINGS, MenuCommandHandler);
}

GSErrCode FreeData (void)
{
    return NoError;
}
