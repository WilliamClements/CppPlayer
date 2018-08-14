/*
 * Copyright 2018 Autodesk
 */

#pragma once
#pragma message("WOX -> CppCallPlayerInterface")

// CppCallPlayerInterface.hpp

#include <CppCallStream.hpp>

class CppCallPlayerInterface
{
   mutable CppCallStream                  m_callStream;

public:
   CppCallPlayerInterface()
   {}
   virtual ~CppCallPlayerInterface() = 0
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

#pragma message("WOX <- CppCallPlayerInterface")

