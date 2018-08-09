/*
 * Copyright 2018 Autodesk
 */

#pragma once

#include <ITrackable.hpp>

class CppCallRecorder;
enum Err;

// Root class making it possible to address the automation wrappers
class IServer : public ITrackable
{
   // Construction
public:
   IServer();
   virtual ~IServer();
};
