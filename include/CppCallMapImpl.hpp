/*
$CODE_OWNER William_Clements
$SECONDARY_OWNERS Dmitry_Brumberg David_Becroft
$ACCESS_RESTRICTED No
*/

#include <stdafx.h>
// CppCallMap.cpp
#include <CppCallMap.h>

#include <CppCallError.h>
#include <CRSError.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
namespace CppCallMapFile
{
   static char THIS_FILE[] = __FILE__;
}
#define THIS_FILE CppCallMapFile::THIS_FILE
#endif
#define NEW_IS_REDEFINED 1

CppCallMapEntry::CppCallMapEntry(std::string api, PlaybackCall call, bool bReturnsValue, int numArgs)
   : m_api(api)
   , m_call(call)
   , m_bReturnsValue(bReturnsValue)
   , m_numArgs(numArgs)
{}
CppCallMapEntry::~CppCallMapEntry()
{}

CppCallMap::CppCallMap()
   : m_theMap()
{}
CppCallMap::~CppCallMap()
{}

CppCallMapEntry& CppCallMap::emplaceMethod(std::unique_ptr<CppCallMapEntry> pEntry)
{
   auto result = m_theMap.emplace(pEntry->api(), std::move(pEntry));
   auto* pRet = (result.first->second).get();
   failUnlessPredicate(result.second, CppCallError_DuplicateAPINames, pRet);
   return *pRet;
}

CppCallMapEntry& CppCallMap::lookupMethod(std::string apiname)
{
   auto findResult = m_theMap.find(apiname);
   failUnlessPredicate(m_theMap.end() != findResult, CppCallError_NoSuchAPIName, apiname);
   return *(findResult->second.get());
}

CppCallMap& cppCallMap()
{
   static CppCallMap s_CppCallMap;
   return s_CppCallMap;
}
