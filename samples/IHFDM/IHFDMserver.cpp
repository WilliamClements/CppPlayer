#include <stdafx.h>
// IHFDMserver.cpp
#include <IHFDMserver.h>

#include <BString.h>
#include <CppPlayer.hpp>
#include <CppRecorder.hpp>
#include <CRSError.h>
#include <WorkspaceWrapper.h>

#undef max
#include <propertysets/hfdm.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
namespace IHFDMserverFile
{
   static char THIS_FILE[] = __FILE__;
}
#define THIS_FILE IHFDMserverFile::THIS_FILE
#endif
#define NEW_IS_REDEFINED 1

HFDMserverWrapper::HFDMserverWrapper(std::shared_ptr<Lynx::PropertySets::HFDM> pHFDM, std::string url)
   : m_pHFDM(pHFDM)
   , m_url(url)
   , m_sConnectStatus(ERR_CONTINUE)
{
   std::string msg("ctor HFDMserverWrapper");
   msg += m_url;
   msg += "\n";
   CppCallInfo::reportInfo(ERR_SUCCESS, msg);
}
HFDMserverWrapper::~HFDMserverWrapper()
{
   std::string msg("dtor ~HFDMserverWrapper");
   msg += m_url;
   msg += "=url\n";
   CppCallInfo::reportInfo(ERR_SUCCESS, msg);

   m_pHFDM.reset();
   CppCallInfo::reportInfo(ERR_SUCCESS, "that was that!\n");
}

/////////////////////////////////////////////////////////////////////////////
// IHFDMserver::isConnected + record/playback

bool HFDMserverWrapper::isConnected() const
{
   CppCallInfo::reportInfo(ERR_SUCCESS, "reached HFDMserverWrapper::isConnected\n");

   // execute
   bool bRet = m_pHFDM.get() && m_pHFDM->isConnected();
   // log
   recordFunction(capture_isConnected, bRet);
   return bRet;
}
void HFDMserverWrapper::playback_isConnected(const ArgsReader& ar)
{
   ar.functionReturn(
      isConnected());
}

/////////////////////////////////////////////////////////////////////////////
// IHFDMserver::disconnect + record/playback

void HFDMserverWrapper::disconnect()
{
   CppCallInfo::reportInfo(ERR_SUCCESS, "reached HFDMserverWrapper::disconnect\n");

   if (m_pHFDM)
   {
      // execute
      m_pHFDM->disconnect();
      // log
      recordMethod(capture_disconnect);
   }
}
void HFDMserverWrapper::playback_disconnect(const ArgsReader&)
{
   // TODO: methodReturn
   disconnect();
}

/////////////////////////////////////////////////////////////////////////////
// IHFDMserver::createWorkspace + record/playback

std::shared_ptr<IWorkspace> HFDMserverWrapper::createWorkspace()
{
   // execute
   auto ws = std::make_shared<WorkspaceWrapper>(this->m_pHFDM->createWorkspace());
   m_pHFDM->onEventQueued.connect(
      [this, ws]()
      {
         ws->processEventQueue();
      }
   );

   // log
   recordFunction(capture_createWorkspace, ws);
   return ws;
}
void HFDMserverWrapper::playback_createWorkspace(const ArgsReader& ar)
{
   ar.functionReturn(
      createWorkspace());
}

void HFDMserverWrapper::destroyWorkspace(std::shared_ptr<IWorkspace> workspace)
{
   m_pHFDM->destroyWorkspace(std::dynamic_pointer_cast<WorkspaceWrapper>(workspace)->m_pNativeWorkspace.get());
}

void HFDMserverWrapper::shareUrn(std::string urn)
{
   std::vector<std::string> urns{ urn };
   std::vector<std::string> userIds{ "*" };
   std::vector<std::string> groupIds{};
   std::vector<std::string> actions{};
   m_pHFDM->share(urns, userIds, groupIds, actions);
}
