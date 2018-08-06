/*
 * Copyright 2018 Autodesk
 */

#pragma once

// CppCallPlayerInterface.hpp

#include <CRSError.h>

#if defined(IHFDM_HOME)
IHFDMAPI
#endif
void playbackCppCalls(std::string filename, std::function<Err()> uiLoginFun = nullptr);
