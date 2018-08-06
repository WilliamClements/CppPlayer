/*
 * Copyright 2018 Autodesk
 */

#pragma once

#include <CppCallError.hpp>
#include <CRSError.h>

class CppCallMapEntry;
class CppCallStream;
class ITrackable;
class PropertyId;

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
   explicit ArgsReader(CppCallStream& callStream);
   ~ArgsReader();

protected:
   CppCallStream& cppCallStream() const
   {
      return m_callStream;
   }
   const CppCallMapEntry& entry() const
   {
      return *m_pEntry;
   }
   std::string api() const;

public:
   std::shared_ptr<ITrackable> getThisTarget() const
   {
      return m_pThisTarget;
   }
   ArgsReader& popHeader();

public:
   int64_t                             popInt() const;
   std::string                         popString() const;
   double                              popDouble() const;
   PropertyId                          popPropertyId() const;
   void                                callCpp(int nFields) const;
   void                                aliasTrackable(std::string guid, std::shared_ptr<ITrackable> pTrackable) const;
   std::shared_ptr<ITrackable>         unaliasTrackable(std::string ssMemorexId) const;
   std::shared_ptr<ITrackable>         unaliasTrackableSafely(std::string ssMemorexId) const;
   void                                aliasURN(std::string ssLiveURN, std::string ssMemorexURN) const;
   std::string                         unaliasURN(std::string ssMemorexURN) const;

   void                                reserveZero() const;
   void                                reserveOne() const;
   void                                reserve() const;
   void                                fulfill(std::shared_ptr<ITrackable> pTrackable) const;
   void                                fulfillReserved() const;

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

#include <ArgsReaderImpl.hpp>