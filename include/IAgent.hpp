/*
 * Copyright 2018 Autodesk
 */

#pragma once

// IAgent.hpp - owns a stream (IPlayer or IRecorder)

#include "CallStream.hpp"

class IAgent
{
   mutable CallStream                  m_callStream;

public:
   IAgent(CallMap& callMap)
      : m_callStream(callMap)
   {}
   virtual ~IAgent() = 0
   {}

   CallStream& callStream() const
   {
      return m_callStream;
   }
   bool streaming() const
   {
      return callStream().streaming();
   }
};
