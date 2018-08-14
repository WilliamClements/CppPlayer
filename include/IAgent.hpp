/*
 * Copyright 2018 Autodesk
 */

#pragma once

// IAgent.hpp

#include "CppCallStream.hpp"

class IAgent
{
   mutable CppCallStream                  m_callStream;

public:
   IAgent()
   {}
   virtual ~IAgent() = 0
   {}

   CppCallStream& cppCallStream() const
   {
      return m_callStream;
   }
   bool streaming() const
   {
      return cppCallStream().streaming();
   }
};
