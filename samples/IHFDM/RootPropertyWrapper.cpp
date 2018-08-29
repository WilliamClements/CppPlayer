#include <stdafx.h>
#include <RootPropertyWrapper.h>

#include <CppPlayer.hpp>
#include <CppRecorder.hpp>
#include <CRSError.h>
#include <PropertySetWrapper.h>

/////////////////////////////////////////////////////////////////////////////
// IRootProperty::getChild + record/playback

std::shared_ptr<IPropertySet> RootPropertyWrapper::getChild(const PropertyId& id)
{
   auto ret = std::make_shared<PropertySetWrapper>(this->rootProperty->get(id.getId()));
   recordFunction(capture_getChild, ret, id.getAbsolutePath());
   return ret;
}
void RootPropertyWrapper::playback_getChild(const ArgsReader& ar)
{
   ar.functionReturn(
      getChild(PropertyId{ ar.popStringVector() }));
};

/////////////////////////////////////////////////////////////////////////////
// IRootProperty::appendChild + record/playback

std::shared_ptr<IPropertySet> RootPropertyWrapper::appendChild(const std::string& typeId, const PropertyId& id)
{
   // execute
   auto parentProperty = this->rootProperty;

   for (auto pathSegment : id.getParentPath())
   {
      if (!(parentProperty = parentProperty->get<Lynx::PropertySets::NodeProperty>(pathSegment)))
         break;
   }

   std::shared_ptr<IPropertySet> ret;

   if (parentProperty)
   {
      auto childProperty = Lynx::PropertySets::GetPropertyFactory().createProperty(typeId);
      parentProperty->insert(id.getId(), childProperty);
      ret = std::make_shared<PropertySetWrapper>(childProperty);
   }
   // log
   recordFunction(capture_appendChild, ret, typeId, id.getAbsolutePath());
   return ret;
}
void RootPropertyWrapper::playback_appendChild(const ArgsReader& ar)
{
   ar.functionReturn(
      appendChild(ar.popString(), PropertyId{ ar.popStringVector() }));
}

/////////////////////////////////////////////////////////////////////////////
// IRootProperty::resolvePath + record/playback

std::shared_ptr<IPropertySet> RootPropertyWrapper::resolvePath(const PropertyId& id)
{
   // execute
   std::shared_ptr<Lynx::PropertySets::BaseProperty> targetProperty = this->rootProperty;
   for (auto pathSegment : id.getAbsolutePath())
   {
      if (!(targetProperty = targetProperty->get(pathSegment)))
         break;
   }

   std::shared_ptr<IPropertySet> ret;
   if (targetProperty)
      ret = std::make_shared<PropertySetWrapper>(targetProperty);
   // pre-assert
   // WOX Assert(!recording() || !id.getId().empty(), Assertions_InvalidPath, "IRootProperty::resolvePath");
   // log
   recordFunction(capture_rootResolvePath, ret, id.getAbsolutePath());
   return ret;
}
void RootPropertyWrapper::playback_rootResolvePath(const ArgsReader& ar)
{
   ar.functionReturn(
      resolvePath(PropertyId{ ar.popStringVector() }));
}

/////////////////////////////////////////////////////////////////////////////
// IRootProperty::getDynamicIds + record/playback

std::vector<std::string> RootPropertyWrapper::getDynamicIds() const
{
   // execute
   auto ret = this->rootProperty->getDynamicIds();
   // side-effect ??
   // log
   // ZOOX it is a function, you goof
   recordMethod(capture_getDynamicIds, ret);
   return ret;
}
void RootPropertyWrapper::playback_getDynamicIds(const ArgsReader& ar)
{
   // scan
   auto pTrackable = ar.popString();
   // execute
   ar.functionReturn(getDynamicIds());
} 