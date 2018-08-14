/*
 * Copyright 2018 Autodesk
 */

#pragma once
#pragma message("WOX -> ArgsWriter")

// ArgsWriter.hpp

#include <CppCallStream.hpp>
#include <ITrackable.hpp>

class ArgsWriter final
{
   CppCallStream&                            m_callStream;
   const CppCallMapEntry*                    m_pEntry;
   std::shared_ptr<const ITrackable>         m_pThisTarget;
   int                                       m_nArgsPushedSoFar;

public:
   explicit ArgsWriter(CppCallStream& callStream)
      : m_callStream(callStream)
      , m_pEntry()
      , m_pThisTarget()
      , m_nArgsPushedSoFar()
   {}
   ~ArgsWriter()
   {}

protected:
   CppCallStream& cppCallStream() const
   {
      return m_callStream;
   }
   const CppCallMapEntry& entry()
   {
      return *m_pEntry;
   }
   std::string api()
   {
      return entry().api();
   }
   std::shared_ptr<const ITrackable> getThisTarget()
   {
      return m_pThisTarget;
   }

public:
   ArgsWriter& pushHeader(std::string methodname, std::shared_ptr<const ITrackable> pThisTarget)
   {
      // Finish initializing object
      m_pEntry = &cppCallMap().lookupMethod(methodname);
      m_pThisTarget = pThisTarget;
      // Start recording call
      cppCallStream().io().pushHeader();
      // Stream out api and target
      pushArgs(api(), getThisTarget());
      return *this;
   }

   // Record each argument according to its type
   ArgsWriter& pushArg(int64_t nIntItem)
   {
      cppCallStream().io().pushInt(nIntItem);
      ++m_nArgsPushedSoFar;
      return *this;
   }
   ArgsWriter& pushArg(int nIntItem)
   {
      cppCallStream().io().pushInt(nIntItem);
      ++m_nArgsPushedSoFar;
      return *this;
   }
   ArgsWriter& pushArg(unsigned int nIntItem)
   {
      cppCallStream().io().pushInt(nIntItem);
      ++m_nArgsPushedSoFar;
      return *this;
   }
   ArgsWriter& pushArg(std::string sStringItem)
   {
      cppCallStream().io().pushString(sStringItem);
      ++m_nArgsPushedSoFar;
      return *this;
   }
   ArgsWriter& pushArg(double dDoubleItem)
   {
      cppCallStream().io().pushDouble(dDoubleItem);
      ++m_nArgsPushedSoFar;
      return *this;
   }
   ArgsWriter& pushArg(std::shared_ptr<const ITrackable> pTrackable)
   {
      return pushArg(pTrackable ? pTrackable->objectKey : "nullptr");
   }
   ArgsWriter& pushArg(const std::vector<std::string>& vStrings)
   {
      return pushArg(cppCallStream().breakStringVector(vStrings));
   }
   ArgsWriter& pushCall()
   {
      failUnlessPredicate(entry().numExpectedFields() == m_nArgsPushedSoFar, CppCallError_WrongNumberOfFields, m_pEntry);
      cppCallStream().io().pushCurrentCall();
      ++cppCallStream().callsCounter();
      return *this;
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

#pragma message("WOX <- ArgsWriter")
