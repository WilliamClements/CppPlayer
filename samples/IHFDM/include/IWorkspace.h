#pragma once
#include <functional>
#include <IHFDMtrackable.h>

enum Err;
class GUIDvalue;
class IHFDMserver;
class IPropertySet;
class IRootProperty;
class PropertyId;
namespace Lynx
{
   namespace PropertySets
   {
      class BranchNode;
      class CommitNode;
   }
}

// Interface to a Lynx Workspace.
class IWorkspace : public IHFDMtrackable
{
public:
   FORCEINLINE IWorkspace()
   { }
   virtual ~IWorkspace()
   { }

   virtual bool initialize(std::string strURN, std::string* newURN) = 0;
   virtual void onBranchHeadMoved_connect(
      std::function<void(
         std::shared_ptr<Lynx::PropertySets::BranchNode>
         , std::shared_ptr<Lynx::PropertySets::CommitNode>)>) = 0;
   virtual void processEventQueue() = 0;
   virtual std::shared_ptr<IRootProperty> getRootProperty() const = 0;
   virtual bool createChild(const PropertyId& id, const std::string& typeId) = 0;
   virtual std::shared_ptr<IPropertySet> factory(const std::string& typeId) = 0;
   virtual std::vector<std::string> getIds() const = 0;
   virtual void pushNotificationDelayScope() = 0;
   virtual void popNotificationDelayScope() = 0;
   virtual void commit(
      const std::string& msg
      , std::function<void(std::shared_ptr<std::exception>, std::shared_ptr<Lynx::PropertySets::CommitNode>)>) = 0;
   virtual void robotic_commit(const std::string& msg) = 0;
   virtual Err rebase(std::string) = 0;

   virtual GUIDvalue getCurrentBranch() const = 0;
   virtual GUIDvalue getCurrentCommit() const = 0;
};

typedef std::function<void(std::shared_ptr<IWorkspace>)> IWorkspaceOperation;