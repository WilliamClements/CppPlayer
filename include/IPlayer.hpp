/*
 * Copyright 2018 Autodesk
 */

#pragma once

// IPlayer.hpp - methods, if any, for a playing type of IAgent

#include "IAgent.hpp"

class IPlayer : public IAgent
{
public:
   IPlayer(CallMap& callMap)
      : IAgent{ callMap }
   {}
   virtual ~IPlayer() = 0
   {}
};


