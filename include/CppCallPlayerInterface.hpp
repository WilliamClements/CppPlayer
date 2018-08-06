/*
$CODE_OWNER William_Clements
$SECONDARY_OWNERS Dmitry_Brumberg David_Becroft
$ACCESS_RESTRICTED No
*/

#pragma once

#include <CRSError.h>

#if defined(IHFDM_HOME)
IHFDMAPI
#endif
void playbackCppCalls(std::string filename, std::function<Err()> uiLoginFun = nullptr);
