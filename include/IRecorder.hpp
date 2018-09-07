/*
 * Copyright 2018 Autodesk
 */

#pragma once

// IRecorder.hpp - methods, if any, for a recording type of IAgent

#include "ArgsWriter.hpp"
#include "IAgent.hpp"
#include "NamespaceAliases.hpp"

class IRecorder : public IAgent
{
public:
   IRecorder(CallMap& callMap)
      : IAgent{ callMap }
   {}
   virtual ~IRecorder() = 0
   {}

   template<class ITarget, typename ReturnType, typename... Args>
   void recordCall(
      std::string api
      , bool returnsValue
      , ReturnType retValue
      , std::shared_ptr<const ITarget> pTrackable
      , Args... args)
   {
      ArgsWriter aw{ callStream() };
      // Push the api name, whether function or method, then the target object
      aw.pushHeader(api, returnsValue, retValue, pTrackable);
      // Push the arguments, in order
      aw.pushArgs(args...);
      callStream().pushCall();
   }
};

