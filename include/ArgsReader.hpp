/*
 * Copyright 2018 Autodesk
 */

#pragma once

#include <vector>

#include <CppCallError.hpp>
#include <CppCallMap.hpp>
#include <CppCallStream.hpp>
#include <CRSError.hpp>

class ArgsReader final
{
private:
   CppCallStream&                      m_callStream;
   const CppCallMapEntry*              m_pEntry;
   std::shared_ptr<ITrackable>         m_pThisTarget;
   mutable std::vector<std::string>    m_reserve;
   mutable std::vector<std::shared_ptr<ITrackable>>
                                       m_fulfill;
   mutable int                         m_nArgsPoppedSoFar;

public:
   explicit ArgsReader(CppCallStream& callStream)
      : m_callStream(callStream)
      , m_pEntry()
      , m_pThisTarget()
      , m_nArgsPoppedSoFar()
   {}
   ~ArgsReader()
   {}

protected:
   CppCallStream& cppCallStream() const
   {
      return m_callStream;
   }
   const CppCallMapEntry& entry() const
   {
      return *m_pEntry;
   }
   std::string api() const
   {
      return entry().api();
   }

public:
   std::shared_ptr<ITrackable> getThisTarget() const
   {
      return m_pThisTarget;
   }
   ArgsReader& popHeader()
   {
      m_pEntry = &cppCallMap().lookupMethod(popString());
      // Defer error reporting on target not found
      m_pThisTarget = unaliasTrackableSafely(popString());
      return *this;
   }

public:
   int64_t popInt() const
   {
      ++m_nArgsPoppedSoFar;
      return cppCallStream().io().popInt();
   }
   std::string popString() const
   {
      ++m_nArgsPoppedSoFar;
      return cppCallStream().io().popString();
   }
   double popDouble() const
   {
      ++m_nArgsPoppedSoFar;
      return cppCallStream().io().popDouble();
   }
   std::vector<std::string> popStringVector() const
   {
      ++m_nArgsPoppedSoFar;
      std::string stringlist = cppCallStream().io().popString();
      return cppCallStream().recomposeStringVector(stringlist);
   }
   void callCpp(int nFields) const
   {
      ++cppCallStream().callsCounter();
      // Invoke wrapper which will call the target api
      entry().call()(*this);
      numberOfArgsMustMatch(m_nArgsPoppedSoFar, nFields);
      fulfillReserved();
   }
   void aliasTrackable(std::string ssTrackable, std::shared_ptr<ITrackable> pTrackable) const
   {
      // Associate guid from file with corresponding live object just created.
      cppCallStream().aliased()[ssTrackable] = pTrackable;
   }
   std::shared_ptr<ITrackable> unaliasTrackable(std::string ssMemorexId) const
   {
      // Get live object given guid from file.
      auto ret = unaliasTrackableSafely(ssMemorexId);
      failUnlessFound(!!ret, CppCallError_NoSuchTrackable);
      return ret;
   }
   std::shared_ptr<ITrackable> unaliasTrackableSafely(std::string ssMemorexId) const
   {
      // Get live object given guid from file. Don't throw. Let error be forced by caller.
      std::shared_ptr<ITrackable> ret;
      auto live = cppCallStream().aliased().find(ssMemorexId);
      if (cppCallStream().aliased().end() != live)
         ret = live->second;
      return ret;
   }
   void aliasURN(std::string ssLiveURN, std::string ssMemorexURN) const
   {
      // empty string maps to empty string
      if (!ssLiveURN.empty())
      {
         // If create a repository on playback, branch guid will not be the same. Account for that.
         cppCallStream().aliasedURNs()[ssMemorexURN] = ssLiveURN;
      }
   }
   std::string unaliasURN(std::string ssMemorexURN) const
   {
      // empty string maps to empty string
      if (ssMemorexURN.empty())
         return ssMemorexURN;
      // We need the live branch guid given original branch guid from file.
      auto live = cppCallStream().aliasedURNs().find(ssMemorexURN);
      failUnlessFound(cppCallStream().aliasedURNs().end() != live, CppCallError_NoSuchURN);
      return live->second;
   }
   void reserveZero() const
   {
      std::string toReserve = popString();
      failUnlessPredicate(
         toReserve == "nullptr"
         , CppCallError_TrackableMustBeNull);
   }
   void reserveOne() const
   {
      std::string toReserve = popString();
      failUnlessPredicate(
         toReserve != "nullptr"
         , CppCallError_NoSuchTrackable);
      m_reserve.push_back(toReserve);
   }
   void reserve() const
   {
      std::string toReserve = popString();
      if (toReserve != "nullptr")
         m_reserve.push_back(toReserve);
   }
   void fulfill(std::shared_ptr<ITrackable> pTrackable) const
   {
      if (pTrackable)
         m_fulfill.push_back(pTrackable);
   }
   void fulfillReserved() const
   {
      failUnlessPredicate(
         m_reserve.size() == m_fulfill.size()
         , CppCallError_UnmatchedFulfillVersusReserve);
      for (size_t ii = 0; ii < m_reserve.size(); ii++)
         aliasTrackable(m_reserve[ii], m_fulfill[ii]);
   }
   void compareBeforeAndAfter(bool bMemorex, bool bLive) const
   {
      failUnlessPredicate(bMemorex == bLive, CppCallError_UnequalReturnResult, m_pEntry);
   }
   void compareBeforeAndAfter(Err sMemorex, Err sLive) const
   {
      failUnlessPredicate(sMemorex == sLive, CppCallError_UnequalReturnResult, m_pEntry);
   }
   void mustBeNull(ITrackable* pTrackable) const
   {
      failUnlessPredicate(!pTrackable, CppCallError_OutOfSequence, m_pEntry);
   }
   void mustNotBeNull(ITrackable* pTrackable) const
   {
      failUnlessPredicate(!!pTrackable, CppCallError_OutOfSequence, m_pEntry);
   }
   void mustNotBeEmpty(std::string ssTrackable) const
   {
      failUnlessPredicate(!ssTrackable.empty(), CppCallError_OutOfSequence, m_pEntry);
   }
   void numberOfArgsMustMatch(int nMemorex, int nLive) const
   {
      failUnlessPredicate(nMemorex == nLive, CppCallError_WrongNumberOfFields, m_pEntry);
   }
   void failUnlessFound(bool bFound, CppCallError err) const
   {
      failUnlessPredicate(bFound, err, m_pEntry);
   }
};