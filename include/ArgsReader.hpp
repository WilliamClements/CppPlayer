/*
 * Copyright 2018 Autodesk
 */

#pragma once

// ArgsReader.hpp

#include "Assertions.hpp"
#include "CallMap.hpp"
#include "CallStream.hpp"
#include <vector>

class ArgsReader final
{
private:
   CallStream&                         m_callStream;
   CallMap&                            m_callMap;
   TypeErased*                         m_ucall;
   std::shared_ptr<ITrackable>         m_pThisTarget;
   mutable std::vector<std::string>    m_reserve;
   mutable std::vector<
      std::shared_ptr<ITrackable>>     m_fulfill;
   mutable int                         m_nArgsPoppedSoFar;

public:
   explicit ArgsReader(CallStream& callStream)
      : m_callStream(callStream)
      , m_callMap(callStream.m_callMap)
      , m_ucall()
      , m_pThisTarget()
      , m_nArgsPoppedSoFar()
   {}
   ~ArgsReader()
   {}

protected:
   CallStream& callStream() const
   {
      return m_callStream;
   }

public:
   std::shared_ptr<ITrackable> getThisTarget() const
   {
      return m_pThisTarget;
   }
   ArgsReader& popHeader()
   {
      m_ucall = &m_callMap.lookupMethod(popString());
      // Defer error reporting on target not found
      m_pThisTarget = unaliasTrackableSafely(popString());
      return *this;
   }

public:
   int64_t popInt() const
   {
      ++m_nArgsPoppedSoFar;
      return callStream().io().popInt();
   }
   std::string popString() const
   {
      ++m_nArgsPoppedSoFar;
      return callStream().io().popString();
   }
   double popDouble() const
   {
      ++m_nArgsPoppedSoFar;
      return callStream().io().popDouble();
   }
   std::vector<std::string> popStringVector() const
   {
      ++m_nArgsPoppedSoFar;
      std::string stringlist = callStream().io().popString();
      return callStream().recomposeStringVector(stringlist);
   }
   void invokeTargetApi() const
   {
      ++callStream().callsCounter();
      // Invoke wrapper which will call the target api
      (*m_ucall)(*this);
      fulfillReserved();
   }
   void aliasTrackable(std::string objectKey, std::shared_ptr<ITrackable> pTrackable) const
   {
      // Associate key from file with corresponding live object just created.
      callStream().aliased()[objectKey] = pTrackable;
   }
   std::shared_ptr<ITrackable> unaliasTrackable(std::string aliasedKey) const
   {
      // Get live object given key from file.
      auto ret = unaliasTrackableSafely(aliasedKey);
      Assert(!!ret, Assertions_NoSuchTrackable);
      return ret;
   }
   std::shared_ptr<ITrackable> unaliasTrackableSafely(std::string aliasedKey) const
   {
      // Get live object given key from file. Don't throw. Let error be forced by caller.
      std::shared_ptr<ITrackable> ret;
      auto live = callStream().aliased().find(aliasedKey);
      if (callStream().aliased().end() != live)
         ret = live->second;
      return ret;
   }
   void aliasURN(std::string ssLiveURN, std::string ssMemorexURN) const
   {
      // empty string maps to empty string
      if (!ssLiveURN.empty())
      {
         // If create a repository on playback, branch URN will not be the same. Account for that.
         callStream().aliasedURNs()[ssMemorexURN] = ssLiveURN;
      }
   }
   std::string unaliasURN(std::string ssMemorexURN) const
   {
      // empty string maps to empty string
      if (ssMemorexURN.empty())
         return ssMemorexURN;
      // We need the live branch URN given original branch URN from file.
      auto live = callStream().aliasedURNs().find(ssMemorexURN);
      Assert(callStream().aliasedURNs().end() != live, Assertions_NoSuchURN);
      return live->second;
   }
   void functionReturn(std::string) const
   {}
   void functionReturn(int64_t) const
   {}
   void functionReturn(Err) const
   {}
   void functionReturn(std::shared_ptr<ITrackable> pTrackable) const
   {}
   void functionReturn(std::vector<std::string>) const
   {}
   void fulfillReserved() const
   {
      Assert(
         m_reserve.size() == m_fulfill.size()
         , Assertions_UnmatchedFulfillVersusReserve);
      for (size_t ii = 0; ii < m_reserve.size(); ii++)
         aliasTrackable(m_reserve[ii], m_fulfill[ii]);
   }
#if WOX
   void compareBeforeAndAfter(bool bMemorex, bool bLive) const
   {
      failUnlessPredicate(bMemorex == bLive, Assertions_UnequalReturnResult, m_pEntry);
   }
   void compareBeforeAndAfter(Err sMemorex, Err sLive) const
   {
      failUnlessPredicate(sMemorex == sLive, Assertions_UnequalReturnResult, m_pEntry);
   }
   void mustBeNull(ITrackable* pTrackable) const
   {
      failUnlessPredicate(!pTrackable, Assertions_OutOfSequence, m_pEntry);
   }
   void mustNotBeNull(ITrackable* pTrackable) const
   {
      failUnlessPredicate(!!pTrackable, Assertions_OutOfSequence, m_pEntry);
   }
   void mustNotBeEmpty(std::string objectKey) const
   {
      failUnlessPredicate(!objectKey.empty(), Assertions_OutOfSequence, m_pEntry);
   }
   void failUnlessFound(bool bFound, Assertions err) const
   {
      failUnlessPredicate(bFound, err, m_pEntry);
   }
#endif
};
