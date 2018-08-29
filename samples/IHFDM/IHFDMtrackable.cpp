#include <stdafx.h>
// IHFDMtrackable.cpp
#include <IHFDMtrackable.h>

#include <CRSError.h>
#include <GUIDvalue.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
namespace IHFDMtrackableFile
{
   static char THIS_FILE[] = __FILE__;
}
#define THIS_FILE IHFDMtrackableFile::THIS_FILE
#endif
#define NEW_IS_REDEFINED 1

IHFDMDiagnosticCallback     IHFDMtrackable::s_dcb;
unsigned int                IHFDMtrackable::s_startFlags = 0x0;
std::weak_ptr<CppCallRecorder> IHFDMtrackable::s_recorder;

IHFDMtrackable::IHFDMtrackable()
   : m_objectKey(GUIDvalue(GUIDnew).getGuid())
{}
IHFDMtrackable::~IHFDMtrackable()
{}

CppCallStream& IHFDMtrackable::cppCallStream()
{
   // The recorder pointer should be previously validated (obviously)
   return s_recorder.lock()->cppCallStream();
}

/* static */
void IHFDMtrackable::reportError(const char* operation, const std::shared_ptr<std::exception>& sException)
{
   const auto* pE = sException.get();
   if (!pE)
   {
      // This should never happen...but if we are crashing,
      // try not to make matters worse
      reportInfo(ERR_SUCCESS, "Lynx: reportError -- Why no std::exception object?\n");
   }
   else
   {
      reportError(operation, *pE);
   }
}

/* static */
void IHFDMtrackable::reportError(const char* operation, const std::exception& error)
{
   std::string ss;
   ss += "Lynx: Failed to ";
   ss += operation;
   ss += " because ";
   ss += error.what();
   ss += "\n";
   reportInfo(ERR_FAILURE, ss);
}

/* static */
void IHFDMtrackable::reportInfo(Err sStatus, std::string ss)
{
   ::OutputDebugStringA(ss.c_str());
   if (s_dcb)
      s_dcb(sStatus, ss);
}
