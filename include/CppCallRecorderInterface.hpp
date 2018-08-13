/*
 * Copyright 2018 Autodesk
 */

#pragma once
#pragma message("WOX -> CppCallRecorderInterface")

// CppCallRecorderInterface.hpp

#include <Aliases.hpp>
#include <ArgsWriter.hpp>
#include <CppCallStream.hpp>

class CppCallRecorderInterface
{
   mutable CppCallStream                  m_callStream;

public:
   CppCallRecorderInterface()
   {}
   virtual ~CppCallRecorderInterface() = 0
   {}

   CppCallStream& cppCallStream() const
   {
      return m_callStream;
   }
   bool streaming() const
   {
      return cppCallStream().streaming();
   }

   template<class ITarget, typename... Args>
   void recordOne(std::string methodname, std::shared_ptr<const ITarget> pTrackable, Args... args)
   {
      ArgsWriter aw(cppCallStream());
      // Push the api name, then the object
      aw.pushHeader(methodname, pTrackable);
      // Push the arguments, in order
      aw.pushArgs(args...);
      aw.pushCall();
   }
};

#pragma message("WOX <- CppCallRecorderInterface")