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
   bool                                m_bReturnsValue;
   ReturnValue                         m_ReturnValue;
   TypeErased*                         m_ucall;
   std::shared_ptr<ITrackable>         m_pThisTarget;
   mutable int                         m_nArgsPoppedSoFar;

public:
   explicit ArgsReader(CallStream& callStream)
      : m_callStream(callStream)
      , m_callMap(callStream.m_callMap)
      , m_bReturnsValue(false)
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
      m_bReturnsValue = !!popInt();
      if (m_bReturnsValue)
         m_ReturnValue = popVariant();
      // Defer error reporting on target not found
      m_pThisTarget = unswizzle(popString());
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
   ReturnValue popVariant() const
   {
      ++m_nArgsPoppedSoFar;
      return callStream().io().popVariant();
   }
   std::shared_ptr<ITrackable> popTrackable() const
   {
      return unswizzle(popString());
   }
   void invoke() const
   {
      ++callStream().callsCounter();
      // Go through the type-erased functor
      (*m_ucall)(*this);
      // WOX fulfillReserved();
   }
   void swizzle(std::string objectKey, std::shared_ptr<ITrackable> pTrackable) const
   {
      // Associate key from file with corresponding live object just created.
      callStream().swizzled()[objectKey] = pTrackable;
   }
   std::shared_ptr<ITrackable> unswizzle(std::string swizzleKey) const
   {
      // Get live object given key from file. Don't throw. Let error be forced by caller.
      std::shared_ptr<ITrackable> ret;
      auto live = callStream().swizzled().find(swizzleKey);
      if (callStream().swizzled().end() != live)
         ret = live->second;
      return ret;
   }
   void bindURN(std::string ssLiveURN, std::string ssMemorexURN) const
   {
      // empty string maps to empty string
      if (!ssLiveURN.empty())
      {
         // If create a repository on playback, branch URN will not be the same. Account for that.
         callStream().urnBindings()[ssMemorexURN] = ssLiveURN;
      }
   }
   std::string unbindURN(std::string ssMemorexURN) const
   {
      // empty string maps to empty string
      if (ssMemorexURN.empty())
         return ssMemorexURN;
      // We need the live branch URN given original branch URN from file.
      auto live = callStream().urnBindings().find(ssMemorexURN);
      Assert(callStream().urnBindings().end() != live, Assertions_NoSuchURN);
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
