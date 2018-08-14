/*
 * Copyright 2018 Autodesk
 */

#pragma once

// IAgent.hpp

#include "CallStream.hpp"

class IAgent
{
   mutable CallStream                  m_callStream;

public:
   IAgent()
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
