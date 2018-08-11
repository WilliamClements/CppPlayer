/*
 * Copyright 2018 Autodesk
 */

#pragma once

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

