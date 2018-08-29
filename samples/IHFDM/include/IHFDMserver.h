#pragma once

#include <functional>
#include <future>
#include <IHFDMtrackable.h>
#include <CppRecorder.hpp>

enum Err;
namespace Lynx
{
   namespace PropertySets
   {
      class HFDM;
   }
}
class IWorkspace;

// Interface to HFDM object, and other main functions
class IHFDMserver : public IHFDMtrackable
{
   DECLARE_ASUPER(IHFDMserver, IHFDMtrackable);

public:
   FORCEINLINE IHFDMserver()
   { }
   virtual ~IHFDMserver()
   { }

   virtual bool isConnected() const = 0;
   virtual Err connect(IHFDMDefaultCallback) = 0;
   virtual void disconnect() = 0;
   virtual std::shared_ptr<IWorkspace> createWorkspace() = 0;
   virtual void destroyWorkspace(std::shared_ptr<IWorkspace> workspace) = 0;
   virtual void shareUrn(std::string urn) = 0;
};

class HFDMserverWrapper : public IHFDMserver
{
   DECLARE_ASUPER(HFDMserverWrapper, IHFDMserver);

public:
   HFDMserverWrapper(std::shared_ptr<Lynx::PropertySets::HFDM> pHFDM, std::string ss);
   virtual ~HFDMserverWrapper();

   bool isConnected() const override;
   Err connect(IHFDMDefaultCallback) override;
   void disconnect() override;
   std::shared_ptr<IWorkspace> createWorkspace() override;
   void destroyWorkspace(std::shared_ptr<IWorkspace> workspace) override;

   void shareUrn(std::string urn);

private:
   std::shared_ptr<Lynx::PropertySets::HFDM> m_pHFDM;
   std::string                               m_url;
   Err                                       m_sConnectStatus;

   // CppPlayer doubles
public:
   void playback_isConnected                (const ArgsReader&);
   void playback_connect                    (const ArgsReader&);
   void playback_disconnect                 (const ArgsReader&);
   void playback_createWorkspace            (const ArgsReader&);

private:
   AddToMap(HFDMserverWrapper, isConnected);
   AddToMap(HFDMserverWrapper, connect);
   AddToMap(HFDMserverWrapper, disconnect);
   AddToMap(HFDMserverWrapper, createWorkspace);
};
