#include <stdafx.h>
// IHFDMmain.cpp
#include <IHFDMmain.h>

#include <CRSError.h>
#include <IHFDMserver.h>
#include <CppPlayer.hpp>
#include <CppRecorder.hpp>

#undef max
#undef SYNCHRONIZE
#include <propertysets/hfdm.h>
#include <propertysets/property_factory.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
namespace IHFDMmainFile
{
   static char THIS_FILE[] = __FILE__;
}
#define THIS_FILE IHFDMmainFile::THIS_FILE
#endif
#define NEW_IS_REDEFINED 1

IHFDMmain::IHFDMmain()
   : IMain{ IHFDMtrackable::libraryCallMap() }
   , m_servermap()
{}
IHFDMmain::~IHFDMmain()
{}
void IHFDMmain::registerDiagnosticCallback(IHFDMDiagnosticCallback dcb)
{
   m_diagnosticCallback = dcb;
}
void IHFDMmain::registerLoginCallback(IHFDMLoginCallback uiLoginFun)
{
   m_uiLoginFun = uiLoginFun;
}

/////////////////////////////////////////////////////////////////////////////
// IHFDMmain::registerTemplate + record/playback

void IHFDMmain::registerTemplate(std::string propertyTemplate)
{
   Lynx::PropertySets::GetPropertyFactory().registerTemplateFromJSON(propertyTemplate);
   recordMethod(capture_registerTemplate, propertyTemplate);
}
void IHFDMmain::playback_registerTemplate(const ArgsReader& ar)
{
   // TODO: methodReturn
   registerTemplate(ar.popString());
}

void IHFDMmain::registerTemplatesFromFolder(std::string folder)
{
   // execute
   Lynx::PropertySets::GetPropertyFactory().registerTemplatesFromFolder(folder);
   // log
   recordMethod(capture_registerTemplatesFromFolder, folder);
}
void IHFDMmain::playback_registerTemplatesFromFolder(const ArgsReader& ar)
{
   // TODO: methodReturn
   registerTemplatesFromFolder(ar.popString());
}

void IHFDMmain::registerTemplateFromJSON(std::string propertyTemplate)
{
   // execute
   Lynx::PropertySets::GetPropertyFactory().registerTemplateFromJSON(propertyTemplate);
   // log
   recordMethod(capture_registerTemplatesFromJSON, propertyTemplate);
}
void IHFDMmain::playback_registerTemplatesFromJSON(const ArgsReader& ar)
{
   // TODO: methodReturn
   registerTemplateFromJSON(ar.popString());
}

/////////////////////////////////////////////////////////////////////////////
// IHFDMmain::createServer

std::shared_ptr<IHFDMserver> IHFDMmain::createServer(std::string url)
{
   auto* pRawHFDM = Lynx::PropertySets::CreateHFDM();
   std::shared_ptr<Lynx::PropertySets::HFDM> pHFDM;
   pHFDM.reset(pRawHFDM);
   return std::make_shared<HFDMserverWrapper>(pHFDM, url);
}

/////////////////////////////////////////////////////////////////////////////
// IHFDMmain::getOrCreateServer + record/playback

std::shared_ptr<IHFDMserver> IHFDMmain::getOrCreateServer(std::string url)
{
   // execute
   auto ipair = m_servermap.emplace(url, std::shared_ptr<IHFDMserver>());
   auto& pServer = ipair.first->second;
   if (ipair.second)
   {
      // The url is new to the map ... so initialize it. 
      pServer = createServer(url);
   }
   // log
   recordFunction(capture_getOrCreateServer, pServer, url);
   return pServer;
}
void IHFDMmain::playback_getOrCreateServer(const ArgsReader& ar)
{
   ar.functionReturn(
      getOrCreateServer(ar.popString()));
}

/////////////////////////////////////////////////////////////////////////////
// IHFDMmain::disconnectAll + record/playback

Err IHFDMmain::disconnectAll()
{
   // execute
   for (auto& ipair : m_servermap)
   {
      // Catch exceptions while disconnecting from servers
      auto& pServer = ipair.second;
      if (pServer)
      {
         if (pServer->isConnected())
         {
            try
            {
               pServer->disconnect();
            }
            catch (...)
            {
               CppCallInfo::reportInfo(ERR_SUCCESS, "Should not get an exception from HFDM on the way out\n");
            }
         }
      }
   }

   // Now just initiate batch destruction
   try
   {
      m_servermap.clear();
   }
   catch (...)
   {
      CppCallInfo::reportInfo(ERR_SUCCESS, "Should not get an exception from HFDM on the way out\n");
   }

   Err sStatus = ERR_SUCCESS;
   // log
   recordFunction(capture_disconnectAll, sStatus);
   return sStatus;
}
void IHFDMmain::playback_disconnectAll(const ArgsReader& ar)
{
   // WOX
   // scan
   Err sMemorex = Err(ar.popInt());
   // execute
   // INTENTIONALLY COMMENTED OUT pThis.disconnectAll();
   sMemorex;
}

/* static */
CallMap& IHFDMmain::libraryCallMap()
{
   return IHFDMtrackable::libraryCallMap();
}

