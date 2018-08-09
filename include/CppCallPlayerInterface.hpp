/*
 * Copyright 2018 Autodesk
 */

#pragma once

// CppCallPlayerInterface.hpp

#include <CRSError.hpp>

void playbackCppCalls(std::string filename, std::function<Err()> uiLoginFun = nullptr);
