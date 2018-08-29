#pragma once

#include <Assertions.hpp>
#include <functional>
#include <future>
#include <IHFDMtrackable.h>
#include <IMain.hpp>
#include <unordered_map>
#include <CppRecorder.hpp>
#include <CppPlayer.hpp>

enum Err;
class IHFDMserver;

// Interface to a singleton to oversee IHFDMservers
class IHFDMmain : public IMain
{
public:
   IHFDMmain();

   void registerDiagnosticCallback(IHFDMDiagnosticCallback);
   void registerLoginCallback(IHFDMLoginCallback);

   virtual ~IHFDMmain();
   virtual void registerTemplate(std::string);
   virtual void registerTemplatesFromFolder(std::string);
   virtual void registerTemplateFromJSON(std::string);
   virtual std::shared_ptr<IHFDMserver> getOrCreateServer(std::string);
   virtual Err disconnectAll();

   std::shared_ptr<IHFDMserver> createServer(std::string url);

private:
   std::unordered_map<std::string, std::shared_ptr<IHFDMserver>, std::hash<std::string>> m_servermap;
   IHFDMLoginCallback m_uiLoginFun;
   IHFDMDiagnosticCallback m_diagnosticCallback;

public:
   static CallMap& libraryCallMap();

   // CppPlayer doubles
public:
   void playback_registerTemplate           (const ArgsReader&);
   void playback_registerTemplatesFromFolder(const ArgsReader&);
   void playback_registerTemplatesFromJSON  (const ArgsReader&);
   void playback_getOrCreateServer          (const ArgsReader&);
   void playback_disconnectAll              (const ArgsReader&);

private:
   AddToMap(IHFDMmain, registerTemplate);
   AddToMap(IHFDMmain, registerTemplatesFromFolder);
   AddToMap(IHFDMmain, registerTemplatesFromJSON);
   AddToMap(IHFDMmain, getOrCreateServer);
   AddToMap(IHFDMmain, disconnectAll);
};