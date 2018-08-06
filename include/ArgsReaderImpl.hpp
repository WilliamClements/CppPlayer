/*
* Copyright 2018 Autodesk
*/

#pragma once

// ArgsReaderImpl.hpp
#include <ArgsReader.hpp>

#include <CppCallStream.hpp>
#include <ITrackable.hpp>

ArgsReader::ArgsReader(CppCallStream& callStream)
   : m_callStream(callStream)
   , m_pEntry()
   , m_pThisTarget()
   , m_nArgsPoppedSoFar()
{}
ArgsReader::~ArgsReader()
{}

std::string ArgsReader::api() const
{
   return entry().api();
}

ArgsReader& ArgsReader::popHeader()
{
   m_pEntry = &cppCallMap().lookupMethod(popString());
   TRACE2("CppCallPlayer calling %ld method=\"%hs\".................\n", cppCallStream().callsCounter(), entry().api().c_str());
   // Defer error reporting on target not found
   m_pThisTarget = unaliasTrackableSafely(popString());
   return *this;
}

int64_t ArgsReader::popInt() const
{
   ++m_nArgsPoppedSoFar;
   return cppCallStream().io().popInt();
}

std::string ArgsReader::popString() const
{
   ++m_nArgsPoppedSoFar;
   return cppCallStream().io().popString();
}

double ArgsReader::popDouble() const
{
   ++m_nArgsPoppedSoFar;
   return cppCallStream().io().popDouble();
}

#if WOX
PropertyId ArgsReader::popPropertyId() const
{
   ++m_nArgsPoppedSoFar;
   std::string stringlist = cppCallStream().io().popString();
   return PropertyId{ cppCallStream().recomposeStringVector(stringlist) };
}
#endif

void ArgsReader::callCpp(int nFields) const
{
   ++cppCallStream().callsCounter();
   // Invoke wrapper which will call the target api
   entry().call()(*this);
   numberOfArgsMustMatch(m_nArgsPoppedSoFar, nFields);
   fulfillReserved();
}

void ArgsReader::aliasTrackable(std::string ssTrackable, std::shared_ptr<ITrackable> pTrackable) const
{
   // Associate guid from file with corresponding live object just created.
   cppCallStream().aliased()[ssTrackable] = pTrackable;
}

std::shared_ptr<ITrackable> ArgsReader::unaliasTrackable(std::string ssMemorexId) const
{
   // Get live object given guid from file.
   auto ret = unaliasTrackableSafely(ssMemorexId);
   failUnlessFound(!!ret, CppCallError_NoSuchTrackable);
   return ret;
}

std::shared_ptr<ITrackable> ArgsReader::unaliasTrackableSafely(std::string ssMemorexId) const
{
   // Get live object given guid from file. Don't throw. Let error be forced by caller.
   std::shared_ptr<ITrackable> ret;
   auto live = cppCallStream().aliased().find(ssMemorexId);
   if (cppCallStream().aliased().end() != live)
      ret = live->second;
   return ret;
}

void ArgsReader::aliasURN(std::string ssLiveURN, std::string ssMemorexURN) const
{
   // empty string maps to empty string
   if (!ssLiveURN.empty())
   {
      // If create a repository on playback, branch guid will not be the same. Account for that.
      cppCallStream().aliasedURNs()[ssMemorexURN] = ssLiveURN;
   }
}

std::string ArgsReader::unaliasURN(std::string ssMemorexURN) const
{
   // empty string maps to empty string
   if (ssMemorexURN.empty())
      return ssMemorexURN;
   // We need the live branch guid given original branch guid from file.
   auto live = cppCallStream().aliasedURNs().find(ssMemorexURN);
   failUnlessFound(cppCallStream().aliasedURNs().end() != live, CppCallError_NoSuchURN);
   return live->second;
}

void ArgsReader::reserveZero() const
{
   std::string toReserve = popString();
   failUnlessPredicate(
      toReserve == "nullptr"
      , CppCallError_TrackableMustBeNull);
}
void ArgsReader::reserveOne() const
{
   std::string toReserve = popString();
   failUnlessPredicate(
      toReserve != "nullptr"
      , CppCallError_NoSuchTrackable);
   m_reserve.push_back(toReserve);
}
void ArgsReader::reserve() const
{
   std::string toReserve = popString();
   if (toReserve != "nullptr")
      m_reserve.push_back(toReserve);
}
void ArgsReader::fulfill(std::shared_ptr<ITrackable> pTrackable) const
{
   if (pTrackable)
      m_fulfill.push_back(pTrackable);
}
void ArgsReader::fulfillReserved() const
{
   failUnlessPredicate(
      m_reserve.size() == m_fulfill.size()
      , CppCallError_UnmatchedFulfillVersusReserve);
   for (int ii = 0; ii < m_reserve.size(); ii++)
      aliasTrackable(m_reserve[ii], m_fulfill[ii]);
}
