#pragma once
#include <IWorkspace.h>
#include <CppRecorder.hpp>

namespace Lynx
{
   namespace PropertySets
   {
      class Workspace;
   }
}

// Wrapper for Lynx Workspace.
class WorkspaceWrapper final : public IWorkspace
{
public:
   WorkspaceWrapper(std::shared_ptr<Lynx::PropertySets::Workspace>);
   ~WorkspaceWrapper();

   bool initialize(std::string strURN, std::string* newURN) override;
   void onBranchHeadMoved_connect(
      std::function<void(
         std::shared_ptr<Lynx::PropertySets::BranchNode>
         , std::shared_ptr<Lynx::PropertySets::CommitNode>)>) override;
   void processEventQueue() override;
   std::shared_ptr<IRootProperty> getRootProperty() const override;
   bool createChild(const PropertyId& id, const std::string& typeId) override;
   std::shared_ptr<IPropertySet> factory(const std::string& typeId) override;
   std::vector<std::string> getIds() const override;

   void pushNotificationDelayScope() override;
   void popNotificationDelayScope() override;

   void commit(
      const std::string& msg
      , std::function<void(std::shared_ptr<std::exception>, std::shared_ptr<Lynx::PropertySets::CommitNode>)>) override;
   void robotic_commit(const std::string& msg) override;
   Err rebase(std::string) override;

   GUIDvalue getCurrentBranch() const override;
   GUIDvalue getCurrentCommit() const override;

public:
   std::shared_ptr<Lynx::PropertySets::Workspace> m_pNativeWorkspace;

   // CppPlayer doubles
public:
   void playback_initialize                 (const ArgsReader&);
   void playback_processEventQueue          (const ArgsReader&);
   void playback_getRootProperty            (const ArgsReader&);
   void playback_createChild                (const ArgsReader&);
   void playback_getIds                     (const ArgsReader&);
   void playback_pushNotificationDelayScope (const ArgsReader&);
   void playback_popNotificationDelayScope  (const ArgsReader&);
   void playback_commit                     (const ArgsReader&);
   void playback_rebase                     (const ArgsReader&);

private:
   AddToMap(WorkspaceWrapper, initialize);
   AddToMap(WorkspaceWrapper, processEventQueue);
   AddToMap(WorkspaceWrapper, getRootProperty);
   AddToMap(WorkspaceWrapper, createChild);
   AddToMap(WorkspaceWrapper, getIds);
   AddToMap(WorkspaceWrapper, pushNotificationDelayScope);
   AddToMap(WorkspaceWrapper, popNotificationDelayScope);
   AddToMap(WorkspaceWrapper, commit);
   AddToMap(WorkspaceWrapper, rebase);
};
