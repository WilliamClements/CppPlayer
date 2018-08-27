/*
 * Copyright 2018 Autodesk
 */

#pragma once

// IPlayer.hpp

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


