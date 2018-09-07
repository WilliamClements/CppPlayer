/*
 * Copyright 2018 Autodesk
 */

#pragma once

// ArgsWriter.hpp - process one call into recording stream

#include "CallMap.hpp"
#include "CallStream.hpp"
#include "ITrackable.hpp"

class ArgsWriter final
{
   CallStream&                               m_callStream;
   CallMap&                                  m_callMap;
   TypeErased*                               m_ucall;
   std::shared_ptr<const ITrackable>         m_pThisTarget;
   int                                       m_nArgsPushedSoFar;

public:
   explicit ArgsWriter(CallStream& callStream)
      : m_callStream(callStream)
      , m_callMap(callStream.m_callMap)
      , m_ucall()
      , m_pThisTarget()
      , m_nArgsPushedSoFar()
   {}
   ~ArgsWriter()
   {}

   template<typename ReturnType>
   ArgsWriter& pushHeader(
      std::string api
      , bool bReturnsValue
      , ReturnType retValue
      , std::shared_ptr<const ITrackable> pThisTarget)
   {
      // Finish initializing object
      m_ucall = &m_callMap.lookupMethod(api);
      m_pThisTarget = pThisTarget;
      // Start recording call
      m_callStream.io().pushHeader();
      // Stream out api and target
      pushArgs(api, int(bReturnsValue));
      if (bReturnsValue)
         pushArgs(retValue);
      pushArgs(m_pThisTarget);
      return *this;
   }

   // Record each argument according to its type
   ArgsWriter& pushArg(int64_t nIntItem)
   {
      m_callStream.io().pushInt(nIntItem);
      ++m_nArgsPushedSoFar;
      return *this;
   }
   ArgsWriter& pushArg(int nIntItem)
   {
      m_callStream.io().pushInt(nIntItem);
      ++m_nArgsPushedSoFar;
      return *this;
   }
   ArgsWriter& pushArg(unsigned int nIntItem)
   {
      m_callStream.io().pushInt(nIntItem);
      ++m_nArgsPushedSoFar;
      return *this;
   }
   ArgsWriter& pushArg(std::string sStringItem)
   {
      m_callStream.io().pushString(sStringItem);
      ++m_nArgsPushedSoFar;
      return *this;
   }
   ArgsWriter& pushArg(double dDoubleItem)
   {
      m_callStream.io().pushDouble(dDoubleItem);
      ++m_nArgsPushedSoFar;
      return *this;
   }
   ArgsWriter& pushArg(std::shared_ptr<const ITrackable> pTrackable)
   {
      return pushArg(pTrackable ? pTrackable->objectKey : "nullptr");
   }
   ArgsWriter& pushArg(const std::vector<std::string>& vStrings)
   {
      return pushArg(m_callStream.breakStringVector(vStrings));
   }
   ArgsWriter& pushArgs()
   {
      return *this;
   }

   template<class Arg>
   ArgsWriter& pushArgs(Arg arg)
   {
      pushArg(arg);
      return *this;
   }

   template<class Head, class... Tail>
   ArgsWriter& pushArgs(Head const& head, Tail const&... tails)
   {
      pushArgs(head);
      pushArgs(tails...);
      return *this;
   }
};
