/*
* Copyright 2018 Autodesk
*/

#pragma once

// ArgsWriter.hpp

class CppCallMapEntry;
class CppCallStream;
class IHFDMtrackable;
class PropertyId;

class ArgsWriter final
{
   CppCallStream&                            m_callStream;
   const CppCallMapEntry*                    m_pEntry;
   std::shared_ptr<const IHFDMtrackable>     m_pThisTarget;
   int                                       m_nArgsPushedSoFar;

public:
   explicit ArgsWriter(CppCallStream& callStream);
   ~ArgsWriter();

protected:
   CppCallStream& cppCallStream() const
   {
      return m_callStream;
   }
   const CppCallMapEntry& entry()
   {
      return *m_pEntry;
   }
   std::string api();
   std::shared_ptr<const IHFDMtrackable> getThisTarget()
   {
      return m_pThisTarget;
   }

public:
   ArgsWriter& pushHeader(std::string methodname, std::shared_ptr<const IHFDMtrackable> pThisTarget);
   std::string pushTrackable(std::shared_ptr<const IHFDMtrackable> pTrackable);

   // Record each argument according to its type
   ArgsWriter& pushArg(int64_t nIntItem);
   ArgsWriter& pushArg(int nIntItem);
   ArgsWriter& pushArg(unsigned int nIntItem);
   ArgsWriter& pushArg(std::string sStringItem);
   ArgsWriter& pushArg(double dDoubleItem);
   ArgsWriter& pushArg(std::shared_ptr<const IHFDMtrackable> pTrackable);
   ArgsWriter& pushArg(const PropertyId& id);
   ArgsWriter& pushArg(const std::vector<std::string>& vStrings);

   ArgsWriter& pushCall();

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
