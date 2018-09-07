/*
 * Copyright 2018 Autodesk
 */

#pragma once

// ArgsReader.hpp - process one call from playback stream

#include "Assertions.hpp"
#include "CallMap.hpp"
#include "CallStream.hpp"
#include "ITrackable.hpp"
#include <vector>

class ArgsReader final
{
   CallStream&                         m_callStream;
   CallMap&                            m_callMap;
   std::string                         m_api;
   TypeErased*                         m_ucall;
   std::shared_ptr<ITrackable>         m_pThisTarget;
   mutable ReturnVariant               m_StreamVariant;
   mutable ReturnVariant               m_LiveVariant;
   mutable int                         m_nArgsPoppedSoFar;

public:
   explicit ArgsReader(CallStream& callStream)
      : m_callStream(callStream)
      , m_callMap(callStream.m_callMap)
      , m_api()
      , m_ucall()
      , m_pThisTarget()
      , m_StreamVariant()
      , m_LiveVariant()
      , m_nArgsPoppedSoFar()
   {}
   ~ArgsReader()
   {}

   std::shared_ptr<ITrackable> getThisTarget() const
   {
      return m_pThisTarget;
   }

   ArgsReader& popHeader()
   {
      m_ucall = &m_callMap.lookupMethod(m_api = popString());
      if (!!popInt())
         m_StreamVariant = popVariant();
      // Defer error reporting on target not found
      m_pThisTarget = m_callStream.swizzle(popString());
      return *this;
   }

   int64_t popInt() const
   {
      ++m_nArgsPoppedSoFar;
      return m_callStream.io().popInt();
   }
   std::string popString() const
   {
      ++m_nArgsPoppedSoFar;
      return m_callStream.io().popString();
   }
   double popDouble() const
   {
      ++m_nArgsPoppedSoFar;
      return m_callStream.io().popDouble();
   }
   std::vector<std::string> popStringVector() const
   {
      ++m_nArgsPoppedSoFar;
      std::string stringlist = m_callStream.io().popString();
      return m_callStream.recomposeStringVector(stringlist);
   }
   ReturnVariant popVariant() const
   {
      ++m_nArgsPoppedSoFar;
      return m_callStream.io().popVariant();
   }
   std::shared_ptr<ITrackable> popTrackable() const
   {
      return m_callStream.swizzle(popString());
   }
   void reconcileVariants() const
   {
      // Known cases where the function returned type from the stream is not a 
      // simple serialization of actual data type
      if (std::holds_alternative<std::shared_ptr<ITrackable>>(m_LiveVariant))
      {
         // Register function return's trackable
         m_callStream.unswizzle(
            std::get<std::string>(m_StreamVariant)
            , std::get<std::shared_ptr<ITrackable>>(m_LiveVariant));
         m_StreamVariant = m_LiveVariant;
      }
      else if (std::holds_alternative<std::vector<std::string>>(m_LiveVariant))
      {
         auto unpacked = std::get<std::string>(m_StreamVariant);
         m_StreamVariant = m_callStream.recomposeStringVector(unpacked);
      }
   }
   void invoke() const
   {
      logPlayerDiagnostic(m_api);
      // Go through the type-erased functor
      (*m_ucall)(*this);
      reconcileVariants();
      // Results same?
      Assert(m_LiveVariant == m_StreamVariant, Assertions_UnequalReturnResult);
   }
   void bindURN(std::string liveURN, std::string streamURN) const
   {
      return m_callStream.bindURN(liveURN, streamURN);
   }
   std::string unbindURN(std::string streamURN) const
   {
      return m_callStream.unbindURN(streamURN);
   }
   void functionReturn(std::string ss) const
   {
      m_LiveVariant = ss;
   }
   void functionReturn(int64_t n64) const
   {
      m_LiveVariant = n64;
   }
   void functionReturn(std::shared_ptr<ITrackable> pTrackable) const
   {
      m_LiveVariant = pTrackable;
   }
   void functionReturn(std::vector<std::string> vs) const
   {
      m_LiveVariant = vs;
   }
};
