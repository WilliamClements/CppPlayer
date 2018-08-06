/*
$CODE_OWNER William_Clements
$SECONDARY_OWNERS Dmitry_Brumberg David_Becroft
$ACCESS_RESTRICTED No
*/

#include <stdafx.h>
// ArgsWriter.cpp
#include <ArgsWriter.h>

#include <CppCallMap.h>
#include <CppCallStream.h>
#include <IHFDMtrackable.h>
#include <PropertyId.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
namespace ArgsWriterFile
{
   static char THIS_FILE[] = __FILE__;
}
#define THIS_FILE ArgsWriterFile::THIS_FILE
#endif
#define NEW_IS_REDEFINED 1

ArgsWriter::ArgsWriter(CppCallStream& callStream)
   : m_callStream(callStream)
   , m_pEntry()
   , m_pThisTarget()
   , m_nArgsPushedSoFar()
{}
ArgsWriter::~ArgsWriter()
{}

std::string ArgsWriter::api()
{
   return entry().api();
}

ArgsWriter& ArgsWriter::pushHeader(std::string methodname, std::shared_ptr<const IHFDMtrackable> pThisTarget)
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

std::string ArgsWriter::pushTrackable(std::shared_ptr<const IHFDMtrackable> pTrackable)
{
   // Recording guid to identify object just created
   return pTrackable->m_objectKey;
}

ArgsWriter& ArgsWriter::pushArg(int64_t nIntItem)
{
   cppCallStream().io().pushInt(nIntItem);
   ++m_nArgsPushedSoFar;
   return *this;
}
ArgsWriter& ArgsWriter::pushArg(int nIntItem)
{
   cppCallStream().io().pushInt(nIntItem);
   ++m_nArgsPushedSoFar;
   return *this;
}
ArgsWriter& ArgsWriter::pushArg(unsigned int nIntItem)
{
   cppCallStream().io().pushInt(nIntItem);
   ++m_nArgsPushedSoFar;
   return *this;
}
ArgsWriter& ArgsWriter::pushArg(std::string sStringItem)
{
   cppCallStream().io().pushString(sStringItem);
   ++m_nArgsPushedSoFar;
   return *this;
}
ArgsWriter& ArgsWriter::pushArg(double dDoubleItem)
{
   cppCallStream().io().pushDouble(dDoubleItem);
   ++m_nArgsPushedSoFar;
   return *this;
}
ArgsWriter& ArgsWriter::pushArg(std::shared_ptr<const IHFDMtrackable> pTrackable)
{
   return pushArg(pTrackable ? pushTrackable(pTrackable) : "nullptr");
}
ArgsWriter& ArgsWriter::pushArg(const PropertyId& id)
{
   return pushArg(id.getAbsolutePath());
}
ArgsWriter& ArgsWriter::pushArg(const std::vector<std::string>& vStrings)
{
   return pushArg(cppCallStream().breakStringVector(vStrings));
}

ArgsWriter& ArgsWriter::pushCall()
{
   failUnlessPredicate(entry().numExpectedFields() == m_nArgsPushedSoFar, CppCallError_WrongNumberOfFields, m_pEntry);
   cppCallStream().io().pushCurrentCall();
   ++cppCallStream().callsCounter();
   return *this;
}

