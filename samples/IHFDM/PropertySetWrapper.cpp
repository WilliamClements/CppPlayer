#include "PropertySetWrapper.h"
#include <CppPlayer.hpp>
#include <CppRecorder.hpp>
#include <CRSError.h>

namespace LPS = Lynx::PropertySets;

PropertySetWrapper::PropertySetWrapper(std::shared_ptr<Lynx::PropertySets::BaseProperty> propertySet) :
   propertySet(propertySet),
   index(0)
{}

PropertySetWrapper::~PropertySetWrapper()
{}

class PrimitiveArrayElement : public IPrimitiveArrayElement
{
public:
   PrimitiveArrayElement(std::shared_ptr<Lynx::PropertySets::StringArrayProperty> stringArray) : stringArray(stringArray) { }
   PrimitiveArrayElement(std::shared_ptr<Lynx::PropertySets::BoolArrayProperty> boolArray) : boolArray(boolArray) { }
   PrimitiveArrayElement(std::shared_ptr<Lynx::PropertySets::Uint8ArrayProperty> uint8Array) : uint8Array(uint8Array) { }
   PrimitiveArrayElement(std::shared_ptr<Lynx::PropertySets::Int16ArrayProperty> int16Array) : int16Array(int16Array) { }
   PrimitiveArrayElement(std::shared_ptr<Lynx::PropertySets::Int32ArrayProperty> int32Array) : int32Array(int32Array) { }
   PrimitiveArrayElement(std::shared_ptr<Lynx::PropertySets::Int64ArrayProperty> int64Array) : int64Array(int64Array) { }
   PrimitiveArrayElement(std::shared_ptr<Lynx::PropertySets::Float32ArrayProperty> floatArray) : floatArray(floatArray) { }
   PrimitiveArrayElement(std::shared_ptr<Lynx::PropertySets::Float64ArrayProperty> doubleArray) : doubleArray(doubleArray) { }

   // CppPlayer doubles
public:
   void playback_pushBackStringValue        (const ArgsReader& ar);
   void playback_pushBackBoolValue          (const ArgsReader& ar);
   void playback_pushBackUint8Value         (const ArgsReader& ar);
   void playback_pushBackInt16Value         (const ArgsReader& ar);
   void playback_pushBackInt32Value         (const ArgsReader& ar);
   void playback_pushBackInt64Value         (const ArgsReader& ar);
   void playback_pushBackFloatValue         (const ArgsReader& ar);
   void playback_pushBackDoubleValue        (const ArgsReader& ar);

private:
   AddToMap(PrimitiveArrayElement, pushBackStringValue);
   AddToMap(PrimitiveArrayElement, pushBackBoolValue);
   AddToMap(PrimitiveArrayElement, pushBackUint8Value);
   AddToMap(PrimitiveArrayElement, pushBackInt16Value);
   AddToMap(PrimitiveArrayElement, pushBackInt32Value);
   AddToMap(PrimitiveArrayElement, pushBackInt64Value);
   AddToMap(PrimitiveArrayElement, pushBackFloatValue);
   AddToMap(PrimitiveArrayElement, pushBackDoubleValue);

   virtual void pushBackStringValue(const std::string& value) override;
   virtual void pushBackBoolValue(const bool& value) override;
   virtual void pushBackUint8Value(const uint8_t& value) override;
   virtual void pushBackInt16Value(const int16_t& value) override;
   virtual void pushBackInt32Value(const int32_t& value) override;
   virtual void pushBackInt64Value(const int64_t& value) override;
   virtual void pushBackFloatValue(const float& value) override;
   virtual void pushBackDoubleValue(const double& value) override;

private:
   std::shared_ptr<Lynx::PropertySets::StringArrayProperty> stringArray;
   std::shared_ptr<Lynx::PropertySets::BoolArrayProperty> boolArray;
   std::shared_ptr<Lynx::PropertySets::Uint8ArrayProperty> uint8Array;
   std::shared_ptr<Lynx::PropertySets::Int16ArrayProperty> int16Array;
   std::shared_ptr<Lynx::PropertySets::Int32ArrayProperty> int32Array;
   std::shared_ptr<Lynx::PropertySets::Int64ArrayProperty> int64Array;
   std::shared_ptr<Lynx::PropertySets::Float32ArrayProperty> floatArray;
   std::shared_ptr<Lynx::PropertySets::Float64ArrayProperty> doubleArray;
};

void PrimitiveArrayElement::pushBackStringValue(const std::string& value)
{
   this->stringArray->push(value);
   recordMethod(capture_pushBackStringValue, value);
}
void PrimitiveArrayElement::pushBackBoolValue(const bool& value)
{
   this->boolArray->push(value);
   recordMethod(capture_pushBackBoolValue, value);
}
void PrimitiveArrayElement::pushBackUint8Value(const uint8_t& value)
{
   this->uint8Array->push(value);
   recordMethod(capture_pushBackUint8Value, value);
}
void PrimitiveArrayElement::pushBackInt16Value(const int16_t& value)
{
   this->int16Array->push(value);
   recordMethod(capture_pushBackInt16Value, value);
}
void PrimitiveArrayElement::pushBackInt32Value(const int32_t& value)
{
   this->int32Array->push(value);
   recordMethod(capture_pushBackInt32Value, value);
}
void PrimitiveArrayElement::pushBackInt64Value(const int64_t& value)
{
   this->int64Array->push(value);
   recordMethod(capture_pushBackInt64Value, value);
}
void PrimitiveArrayElement::pushBackFloatValue(const float& value)
{
   this->floatArray->push(value);
   recordMethod(capture_pushBackFloatValue, value);
}
void PrimitiveArrayElement::pushBackDoubleValue(const double& value)
{
   this->doubleArray->push(value);
   recordMethod(capture_pushBackDoubleValue, value);
}

void PrimitiveArrayElement::playback_pushBackStringValue(const ArgsReader& ar)
{
   // TODO: methodReturn
   pushBackStringValue(
      ar.popString());
}
void PrimitiveArrayElement::playback_pushBackBoolValue(const ArgsReader& ar)
{
   // TODO: methodReturn
   pushBackBoolValue(
      !!ar.popInt());
}
void PrimitiveArrayElement::playback_pushBackUint8Value(const ArgsReader& ar)
{
   // TODO: methodReturn
   pushBackUint8Value(
      ar.popInt());
}
void PrimitiveArrayElement::playback_pushBackInt16Value(const ArgsReader& ar)
{
   // TODO: methodReturn
   pushBackInt16Value(
      ar.popInt());
}
void PrimitiveArrayElement::playback_pushBackInt32Value(const ArgsReader& ar)
{
   // TODO: methodReturn
   pushBackInt32Value(
      ar.popInt());
}
void PrimitiveArrayElement::playback_pushBackInt64Value(const ArgsReader& ar)
{
   // TODO: methodReturn
   pushBackInt64Value(
      ar.popInt());
}
void PrimitiveArrayElement::playback_pushBackFloatValue(const ArgsReader& ar)
{
   // TODO: methodReturn
   pushBackFloatValue(
      (float)ar.popDouble());
}
void PrimitiveArrayElement::playback_pushBackDoubleValue(const ArgsReader& ar)
{
   // TODO: methodReturn
   pushBackDoubleValue(
      ar.popDouble());
}

/////////////////////////////////////////////////////////////////////////////
// IPropertySet::resolvePath + record/playback

std::shared_ptr<IPropertySet> PropertySetWrapper::resolvePath(const std::string& path)
{
   // execute
   std::shared_ptr<IPropertySet> ret;
   auto childProperty = this->propertySet->get(path);
   if (childProperty)
      ret = std::make_shared<PropertySetWrapper>(childProperty);
   // pre-assert
   // WOX Assert(!recording() || !path.empty(), Assertions_InvalidPath, "IPropertySet::resolvePath");
   // log
   recordFunction(capture_resolvePath, ret, path);
   return ret;
}
void PropertySetWrapper::playback_resolvePath(const ArgsReader& ar)
{
   // ZOOX needs to swizzle
   ar.functionReturn(
      resolvePath(ar.popString()));
}

/////////////////////////////////////////////////////////////////////////////
// IPropertySet::resolvePathToPrimitive + record/playback

std::shared_ptr<IPrimitiveProperty> PropertySetWrapper::resolvePathToPrimitive(const std::string& path)
{
   std::shared_ptr<IPrimitiveProperty> ret;
   auto childProperty = this->propertySet->get(path);
   if (childProperty)
      ret = std::make_shared<PrimitivePropertySet>(childProperty);
   recordFunction(capture_resolvePathToPrimitive, ret, path);
   return ret;
}
void PropertySetWrapper::playback_resolvePathToPrimitive(const ArgsReader& ar)
{
   // ZOOX need swizzle
   ar.functionReturn(
      resolvePathToPrimitive(ar.popString()));
}

/////////////////////////////////////////////////////////////////////////////
// IPropertySet::resolvePathToReference + record/playback

std::shared_ptr<IReferenceProperty> PropertySetWrapper::resolvePathToReference(const std::string& path)
{
   std::shared_ptr<IReferenceProperty> ret;
   auto childProperty = this->propertySet->get<Lynx::PropertySets::ReferenceProperty>(path);
   if (childProperty)
      ret = std::make_shared<ReferencePropertySet>(childProperty);
   recordFunction(capture_resolvePathToReference, ret, path);
   return ret;
}
void PropertySetWrapper::playback_resolvePathToReference(const ArgsReader& ar)
{
   // ZOOX need swizzle
   ar.functionReturn(
      resolvePathToReference(ar.popString()));
}

/////////////////////////////////////////////////////////////////////////////
// IPropertySet::resolvePathToArray + record/playback

std::shared_ptr<IArrayProperty> PropertySetWrapper::resolvePathToArray(const std::string& path)
{
   std::shared_ptr<IArrayProperty> ret;
   auto childProperty = this->propertySet->get<Lynx::PropertySets::BaseProperty>(path);
   ret = std::make_shared<ArrayPropertyWrapper>(childProperty);
   return ret;
}

/////////////////////////////////////////////////////////////////////////////
// IPropertySet::isNull

bool PropertySetWrapper::isNull() const
{
   return !accessPropertyToAssign();
}

/////////////////////////////////////////////////////////////////////////////
// IPropertySet::insert + record/playback

void PropertySetWrapper::insert(const std::string& key, std::shared_ptr<IPropertySet> property)
{
   DBG_ASSERT(property.get());
   std::dynamic_pointer_cast<LPS::MapProperty>(this->accessPropertyToAssign())->insert(
      key
      , std::dynamic_pointer_cast<PropertySetWrapper>(property)->accessPropertyToAssign());
}
void PropertySetWrapper::insertStringValue(const std::string& key, const std::string& value)
{
   std::dynamic_pointer_cast<LPS::MapPropertyBase<std::string>>(this->accessPropertyToAssign())->insertValue(
      key
      , value);
}
void PropertySetWrapper::insertBoolValue(const std::string& key, const bool& value)
{
   std::dynamic_pointer_cast<LPS::MapPropertyBase<bool>>(this->accessPropertyToAssign())->insertValue(
      key
      , value);
}
void PropertySetWrapper::insertUint8Value(const std::string& key, const uint8_t& value)
{
   std::dynamic_pointer_cast<LPS::MapPropertyBase<uint8_t>>(this->accessPropertyToAssign())->insertValue(
      key
      , value);
}
void PropertySetWrapper::insertInt16Value(const std::string& key, const int16_t& value)
{
   std::dynamic_pointer_cast<LPS::MapPropertyBase<int16_t>>(this->accessPropertyToAssign())->insertValue(
      key
      , value);
}
void PropertySetWrapper::insertInt32Value(const std::string& key, const int32_t& value)
{
   std::dynamic_pointer_cast<LPS::MapPropertyBase<int32_t>>(this->accessPropertyToAssign())->insertValue(
      key
      , value);
}
void PropertySetWrapper::insertInt64Value(const std::string& key, const int64_t& value)
{
   std::dynamic_pointer_cast<LPS::MapPropertyBase<int64_t>>(this->accessPropertyToAssign())->insertValue(
      key
      , value);
}
void PropertySetWrapper::insertFloatValue(const std::string& key, const float& value)
{
   std::dynamic_pointer_cast<LPS::MapPropertyBase<float>>(this->accessPropertyToAssign())->insertValue(
      key
      , value);
}
void PropertySetWrapper::insertDoubleValue(const std::string& key, const double& value)
{
   std::dynamic_pointer_cast<LPS::MapPropertyBase<double>>(this->accessPropertyToAssign())->insertValue(
      key
      , value);
}

VisitingContinuation PropertySetWrapper::visitCollection(const std::function<VisitingContinuation(std::shared_ptr<IPropertySet>)>& visitor)
{
   auto mapp = std::dynamic_pointer_cast<LPS::MapProperty>(this->accessPropertyToAssign());
   auto mapv = mapp->getAsArray();

   auto vc = VisitingContinue;
   for (auto& eachps : mapv)
   {
      auto each = std::make_shared<PropertySetWrapper>(eachps);
      vc = visitor(each);
      if (VisitingBreak == vc)
         break;
   }

   return vc;
}

/////////////////////////////////////////////////////////////////////////////
// IPropertySet::remove + record/playback

void PropertySetWrapper::remove(const std::string& childId)
{
   // execute
   std::dynamic_pointer_cast<Lynx::PropertySets::NodeProperty>(this->propertySet)->remove(childId);
   // log
   recordMethod(capture_remove, childId);
}
void PropertySetWrapper::playback_remove(const ArgsReader& ar)
{
   // scan
   std::string ssTrackable = ar.popString();
   std::string ssActual = ar.unaliasTrackable(ssTrackable)->objectKey;
   // execute
   remove(ssActual);
}

/////////////////////////////////////////////////////////////////////////////
// IPropertySet::clear<Thing>Array + record/playback

void PropertySetWrapper::clearStringArray()
{
   // execute
   this->stringArray = std::dynamic_pointer_cast<Lynx::PropertySets::StringArrayProperty>(this->propertySet);
   this->stringArray->clear();
   // log
   recordMethod(capture_clearStringArray);
}
void PropertySetWrapper::playback_clearStringArray(const ArgsReader&)
{
   // TODO: methodReturn
   clearStringArray();
}

void PropertySetWrapper::clearBoolArray()
{
   // execute
   this->boolArray = std::dynamic_pointer_cast<Lynx::PropertySets::BoolArrayProperty>(this->propertySet);
   this->boolArray->clear();
   // log
   recordMethod(capture_clearBoolArray);
}
void PropertySetWrapper::playback_clearBoolArray(const ArgsReader&)
{
   // TODO: methodReturn
   clearBoolArray();
}

void PropertySetWrapper::clearUint8Array()
{
   // execute
   this->uint8Array = std::dynamic_pointer_cast<Lynx::PropertySets::Uint8ArrayProperty>(this->propertySet);
   this->uint8Array->clear();
   // log
   recordMethod(capture_clearUint8Array);
}
void PropertySetWrapper::playback_clearUint8Array(const ArgsReader&)
{
   // TODO: methodReturn
   clearUint8Array();
}

void PropertySetWrapper::clearInt16Array()
{
   // execute
   this->int16Array = std::dynamic_pointer_cast<Lynx::PropertySets::Int16ArrayProperty>(this->propertySet);
   this->int16Array->clear();
   // log
   recordMethod(capture_clearInt16Array);
}
void PropertySetWrapper::playback_clearInt16Array(const ArgsReader&)
{
   // TODO: methodReturn
   clearInt16Array();
}

void PropertySetWrapper::clearInt32Array()
{
   // execute
   this->int32Array = std::dynamic_pointer_cast<Lynx::PropertySets::Int32ArrayProperty>(this->propertySet);
   this->int32Array->clear();
   // log
   recordMethod(capture_clearInt32Array);
}
void PropertySetWrapper::playback_clearInt32Array(const ArgsReader&)
{
   // TODO: methodReturn
   clearInt32Array();
}

void PropertySetWrapper::clearInt64Array()
{
   // execute
   this->int64Array = std::dynamic_pointer_cast<Lynx::PropertySets::Int64ArrayProperty>(this->propertySet);
   this->int64Array->clear();
   // log
   recordMethod(capture_clearInt64Array);
}
void PropertySetWrapper::playback_clearInt64Array(const ArgsReader&)
{
   // TODO: methodReturn
   clearInt64Array();
}

void PropertySetWrapper::clearFloatArray()
{
   // execute
   this->floatArray = std::dynamic_pointer_cast<Lynx::PropertySets::Float32ArrayProperty>(this->propertySet);
   this->floatArray->clear();
   // log
   recordMethod(capture_clearFloatArray);
}
void PropertySetWrapper::playback_clearFloatArray(const ArgsReader&)
{
   // TODO: methodReturn
   clearFloatArray();
}

void PropertySetWrapper::clearDoubleArray()
{
   // execute
   this->doubleArray = std::dynamic_pointer_cast<Lynx::PropertySets::Float64ArrayProperty>(this->propertySet);
   this->doubleArray->clear();
   // log
   recordMethod(capture_clearDoubleArray);
}
void PropertySetWrapper::playback_clearDoubleArray(const ArgsReader&)
{
   // TODO: methodReturn
   clearDoubleArray();
}

void PropertySetWrapper::clearReferenceArray()
{
   // execute
   this->referenceArray = std::dynamic_pointer_cast<Lynx::PropertySets::ReferenceArrayProperty>(this->propertySet);
   this->referenceArray->clear();
   // log
   recordMethod(capture_clearReferenceArray);
}
void PropertySetWrapper::playback_clearReferenceArray(const ArgsReader&)
{
   // TODO: methodReturn
   clearReferenceArray();
}

void PropertySetWrapper::clearPropertyArray()
{
   // execute
   this->propertyArray = std::dynamic_pointer_cast<Lynx::PropertySets::ArrayProperty>(this->propertySet);
   this->propertyArray->clear();
   // log
   recordMethod(capture_clearPropertyArray);
}
void PropertySetWrapper::playback_clearPropertyArray(const ArgsReader&)
{
   // TODO: methodReturn
   clearPropertyArray();
}

/////////////////////////////////////////////////////////////////////////////
// IPropertySet::pushArrayValue + record/playback

void PropertySetWrapper::pushArrayValue(std::string typeId)
{
   // execute
   this->propertyArray->push(Lynx::PropertySets::GetPropertyFactory().createProperty(typeId));
   // log
   recordMethod(capture_pushArrayValue, typeId);
}
void PropertySetWrapper::playback_pushArrayValue(const ArgsReader& ar)
{
   // TODO: methodReturn
   // WOX string??
   pushArrayValue(ar.popString());
}

/////////////////////////////////////////////////////////////////////////////
// IPropertySet::getArrayValue + record/playback

std::shared_ptr<IPropertySet> PropertySetWrapper::getArrayValue(unsigned int index)
{
   // execute
   std::shared_ptr<IPropertySet> ret = std::make_shared<PropertySetWrapper>(this->propertyArray->getValue(index));
   // log
   recordFunction(capture_getArrayValue, ret, index);
   return ret;
}
void PropertySetWrapper::playback_getArrayValue(const ArgsReader& ar)
{
   ar.functionReturn(
      getArrayValue(ar.popInt()));
}

/////////////////////////////////////////////////////////////////////////////
// IPropertySet::createNextPrimitive + record/playback

std::shared_ptr<IPrimitiveArrayElement> PropertySetWrapper::createNextPrimitive()
{
   // execute
   std::shared_ptr<IPrimitiveArrayElement> ret;
   if (this->stringArray) ret = std::make_shared<PrimitiveArrayElement>(this->stringArray);
   else if (this->boolArray) ret = std::make_shared<PrimitiveArrayElement>(this->boolArray);
   else if (this->uint8Array) ret = std::make_shared<PrimitiveArrayElement>(this->uint8Array);
   else if (this->int16Array) ret = std::make_shared<PrimitiveArrayElement>(this->int16Array);
   else if (this->int32Array) ret = std::make_shared<PrimitiveArrayElement>(this->int32Array);
   else if (this->int64Array) ret = std::make_shared<PrimitiveArrayElement>(this->int64Array);
   else if (this->floatArray) ret = std::make_shared<PrimitiveArrayElement>(this->floatArray);
   else if (this->doubleArray) ret = std::make_shared<PrimitiveArrayElement>(this->doubleArray);
   // WOX else ret = std::make_shared<PrimitivePropertySet>(this->propertySet);
   // log
   recordFunction(capture_createNextPrimitive, ret);
   return ret;
}
void PropertySetWrapper::playback_createNextPrimitive(const ArgsReader& ar)
{
   ar.functionReturn(
      createNextPrimitive());
}

/////////////////////////////////////////////////////////////////////////////
// IPropertySet::createNextReference + record/playback

std::shared_ptr<IReferenceProperty> PropertySetWrapper::createNextReference()
{
   // execute
   std::shared_ptr<IReferenceProperty> ret;
   if (this->referenceArray)
   {
      this->referenceArray->push(std::shared_ptr<Lynx::PropertySets::BaseProperty>());
      auto reference = std::make_shared<ReferenceArrayElement>(referenceArray, this->index);
      this->index++;
      ret = reference;
   }
   else
   {
      ret = std::make_shared<ReferencePropertySet>(std::dynamic_pointer_cast<Lynx::PropertySets::ReferenceProperty>(this->propertySet));
   }
   // log
   recordFunction(capture_createNextReference, ret);
   return ret;
}
void PropertySetWrapper::playback_createNextReference(const ArgsReader& ar)
{
   ar.functionReturn(
      createNextReference());
}

std::string PropertySetWrapper::getTypeId() const
{
   auto ret = this->propertySet->getTypeId();
   return ret;
}

PropertyId PropertySetWrapper::getId() const
{
   std::vector<std::string> absolutePath;
   std::vector<std::string> pathStack;

   auto parent = this->propertySet;
   while (parent && !parent->isRoot())
   {
      pathStack.push_back(parent->getId());
      parent = parent->getParent();
   }

   while (!pathStack.empty())
   {
      absolutePath.push_back(pathStack.back());
      pathStack.pop_back();
   }

   auto ret = PropertyId{ absolutePath };
   return ret;
}

const ARuntimeClass* PropertySetWrapper::getARuntimeClass() const
{
#if !defined(IHFDM_HOME)
   return lynxCollaboration().getARuntimeClass(propertySet->getTypeId());
#else
   return nullptr;
#endif
}

std::shared_ptr<IPropertySet> PropertySetWrapper::getReferencedProperty() const
{
   std::shared_ptr<IPropertySet> ret;
   auto referentProperty = std::dynamic_pointer_cast<Lynx::PropertySets::ReferenceProperty>(this->propertySet)->get();
   if (referentProperty)
      ret = std::make_shared<PropertySetWrapper>(referentProperty);
   return ret;
}

std::string PropertySetWrapper::getForeignClassName() const
{
   return typeid(*this->propertySet.get()).name();
}

std::string PropertySetWrapper::getStringValue() const
{
   auto ret = std::dynamic_pointer_cast<Lynx::PropertySets::StringProperty>(this->propertySet)->getValue();
   return ret;
}

bool PropertySetWrapper::getBoolValue() const
{
   auto ret = std::dynamic_pointer_cast<Lynx::PropertySets::BoolProperty>(this->propertySet)->getValue();
   return ret;
}

uint8_t PropertySetWrapper::getUint8Value() const
{
   auto ret = std::dynamic_pointer_cast<Lynx::PropertySets::Uint8Property>(this->propertySet)->getValue();
   return ret;
}

int16_t PropertySetWrapper::getInt16Value() const
{
   auto ret = std::dynamic_pointer_cast<Lynx::PropertySets::Int16Property>(this->propertySet)->getValue();
   return ret;
}

int32_t PropertySetWrapper::getInt32Value() const
{
   auto ret = std::dynamic_pointer_cast<Lynx::PropertySets::Int32Property>(this->propertySet)->getValue();
   return ret;
}

int64_t PropertySetWrapper::getInt64Value() const
{
   auto ret = std::dynamic_pointer_cast<Lynx::PropertySets::Int64Property>(this->propertySet)->getValue();
   return ret;
}

float PropertySetWrapper::getFloatValue() const
{
   auto ret = std::dynamic_pointer_cast<Lynx::PropertySets::Float32Property>(this->propertySet)->getValue();
   return (float)ret;
}

double PropertySetWrapper::getDoubleValue() const
{
   auto ret = std::dynamic_pointer_cast<Lynx::PropertySets::Float64Property>(this->propertySet)->getValue();
   return ret;
}

ArrayPropertyWrapper::ArrayPropertyWrapper(std::shared_ptr<Lynx::PropertySets::BaseProperty> theArray)
   : propertyArray(std::dynamic_pointer_cast<Lynx::PropertySets::ArrayProperty>(theArray))
   , stringArray(std::dynamic_pointer_cast<Lynx::PropertySets::StringArrayProperty>(theArray))
   , boolArray(std::dynamic_pointer_cast<Lynx::PropertySets::BoolArrayProperty>(theArray))
   , uint8Array(std::dynamic_pointer_cast<Lynx::PropertySets::Uint8ArrayProperty>(theArray))
   , int16Array(std::dynamic_pointer_cast<Lynx::PropertySets::Int16ArrayProperty>(theArray))
   , int32Array(std::dynamic_pointer_cast<Lynx::PropertySets::Int32ArrayProperty>(theArray))
   , int64Array(std::dynamic_pointer_cast<Lynx::PropertySets::Int64ArrayProperty>(theArray))
   , floatArray(std::dynamic_pointer_cast<Lynx::PropertySets::Float32ArrayProperty>(theArray))
   , doubleArray(std::dynamic_pointer_cast<Lynx::PropertySets::Float64ArrayProperty>(theArray))
   , referenceArray(std::dynamic_pointer_cast<Lynx::PropertySets::ReferenceArrayProperty>(theArray))
{}

size_t ArrayPropertyWrapper::getLength() const
{
   if (propertyArray)
      return propertyArray->getLength();
   else if (stringArray)
      return stringArray->getLength();
   else if (boolArray)
      return boolArray->getLength();
   else if (uint8Array)
      return uint8Array->getLength();
   else if (int16Array)
      return int16Array->getLength();
   else if (int32Array)
      return int32Array->getLength();
   else if (int64Array)
      return int64Array->getLength();
   else if (floatArray)
      return floatArray->getLength();
   else if (doubleArray)
      return doubleArray->getLength();
   return referenceArray->getLength();
}

std::string ArrayPropertyWrapper::getStringValue(size_t position) const
{
   return stringArray->getValue(position);
}

bool ArrayPropertyWrapper::getBoolValue(size_t position) const
{
   return boolArray->getValue(position);
}
uint8_t ArrayPropertyWrapper::getUint8Value(size_t position) const
{
   return uint8Array->getValue(position);
}
int16_t ArrayPropertyWrapper::getInt16Value(size_t position) const
{
   return int16Array->getValue(position);
}
int32_t ArrayPropertyWrapper::getInt32Value(size_t position) const
{
   return int32Array->getValue(position);
}

int64_t ArrayPropertyWrapper::getInt64Value(size_t position) const
{
   return int64Array->getValue(position);
}
float ArrayPropertyWrapper::getFloatValue(size_t position) const
{
   return floatArray->getValue(position);
}
double ArrayPropertyWrapper::getDoubleValue(size_t position) const
{
   return doubleArray->getValue(position);
}

void ArrayPropertyWrapper::setStringValue(size_t position, const std::string& value)
{
   stringArray->set(position, value);
}   
void ArrayPropertyWrapper::setBoolValue(size_t position, const bool& value)
{
   boolArray->set(position, value);
}
void ArrayPropertyWrapper::setUint8Value(size_t position, const uint8_t& value)
{
   uint8Array->set(position, value);
}
void ArrayPropertyWrapper::setInt16Value(size_t position, const int16_t& value)
{
   int16Array->set(position, value);
}
void ArrayPropertyWrapper::setInt32Value(size_t position, const int32_t& value)
{
   int32Array->set(position, value);
}
void ArrayPropertyWrapper::setInt64Value(size_t position, const int64_t& value)
{
   int64Array->set(position, value);
}
void ArrayPropertyWrapper::setFloatValue(size_t position, const float& value)
{
   floatArray->set(position, value);
}
void ArrayPropertyWrapper::setDoubleValue(size_t position, const double& value)
{
   doubleArray->set(position, value);
}

std::shared_ptr<IPropertySet> ArrayPropertyWrapper::getProperty(size_t position)
{
   return std::make_shared<PropertySetWrapper>(propertyArray->getValue(position));
}
std::shared_ptr<IReferenceProperty> ArrayPropertyWrapper::getReference(size_t position)
{
   return std::make_shared<ReferencePropertySet>(std::dynamic_pointer_cast<Lynx::PropertySets::ReferenceProperty>(referenceArray->get(position)));
}

/////////////////////////////////////////////////////////////////////////////
// IArrayProperty::getIds + record/playback

std::vector<std::string> ArrayPropertyWrapper::getIds() const
{
   return referenceArray->getIds();
}

void ArrayPropertyWrapper::playback_getStringValue(const ArgsReader& ar)
{
   // TODO: popSizeT
   ar.functionReturn(
      getStringValue(ar.popInt()));
}
void ArrayPropertyWrapper::playback_getBoolValue(const ArgsReader& ar)
{
   // TODO: popSizeT
   ar.functionReturn(
      getBoolValue(ar.popInt()));
}
void ArrayPropertyWrapper::playback_getUint8Value(const ArgsReader& ar)
{
   // TODO: popSizeT
   ar.functionReturn(
      getUint8Value(ar.popInt()));
}
void ArrayPropertyWrapper::playback_getInt16Value(const ArgsReader& ar)
{
   // TODO: popSizeT
   ar.functionReturn(
      getInt16Value(ar.popInt()));
}
void ArrayPropertyWrapper::playback_getInt32Value(const ArgsReader& ar)
{
   // TODO: popSizeT
   ar.functionReturn(
      getInt32Value(ar.popInt()));
}
void ArrayPropertyWrapper::playback_getInt64Value(const ArgsReader& ar)
{
   // TODO: popSizeT
   ar.functionReturn(
      getInt64Value(ar.popInt()));
}
void ArrayPropertyWrapper::playback_getFloatValue(const ArgsReader& ar)
{
   // TODO: popSizeT
   ar.functionReturn(
      getFloatValue(ar.popInt()));
}
void ArrayPropertyWrapper::playback_getDoubleValue(const ArgsReader& ar)
{
   // TODO: popSizeT
   ar.functionReturn(
      getDoubleValue(ar.popInt()));
}
void ArrayPropertyWrapper::playback_setStringValue(const ArgsReader& ar)
{
   // TODO: methodReturn
   // TODO: popSizeT
   setStringValue(ar.popInt(), ar.popString());
}
void ArrayPropertyWrapper::playback_setBoolValue(const ArgsReader& ar)
{
   // TODO: methodReturn
   // TODO: popSizeT
   setBoolValue(ar.popInt(), !!ar.popInt());
}
void ArrayPropertyWrapper::playback_setUint8Value(const ArgsReader& ar)
{
   // TODO: methodReturn
   // TODO: popSizeT
   setUint8Value(ar.popInt(), ar.popInt());
}
void ArrayPropertyWrapper::playback_setInt16Value(const ArgsReader& ar)
{
   // TODO: methodReturn
   // TODO: popSizeT
   setInt16Value(ar.popInt(), ar.popInt());
}
void ArrayPropertyWrapper::playback_setInt32Value(const ArgsReader& ar)
{
   // TODO: methodReturn
   // TODO: popSizeT
   setInt32Value(ar.popInt(), ar.popInt());
}
void ArrayPropertyWrapper::playback_setInt64Value(const ArgsReader& ar)
{
   // TODO: methodReturn
   // TODO: popSizeT
   setInt64Value(ar.popInt(), ar.popInt());
}
void ArrayPropertyWrapper::playback_setFloatValue(const ArgsReader& ar)
{
   // TODO: methodReturn
   // TODO: popSizeT
   setFloatValue(ar.popInt(), (float)ar.popDouble());
}
void ArrayPropertyWrapper::playback_setDoubleValue(const ArgsReader& ar)
{
   // TODO: methodReturn
   setDoubleValue(ar.popInt(), ar.popDouble());
}

void ArrayPropertyWrapper::setProperty(size_t position, std::shared_ptr<IPropertySet> theProp)
{
   propertyArray->set(position, std::dynamic_pointer_cast<PropertySetWrapper>(theProp)->accessPropertyToAssign());
}
void ArrayPropertyWrapper::setReference(size_t position, std::shared_ptr<IReferenceProperty> theRef)
{
   referenceArray->set(position, std::dynamic_pointer_cast<ReferencePropertySet>(theRef)->accessReferenceToAssign());
}

void PrimitivePropertySet::playback_setStringValue(const ArgsReader& ar)
{
   // TODO: methodReturn
   setStringValue(ar.popString());
}
void PrimitivePropertySet::playback_setBoolValue(const ArgsReader& ar)
{
   // TODO: methodReturn
   setBoolValue(!!ar.popInt());
}
void PrimitivePropertySet::playback_setUint8Value(const ArgsReader& ar)
{
   // TODO: methodReturn
   setUint8Value(ar.popInt());
}
void PrimitivePropertySet::playback_setInt16Value(const ArgsReader& ar)
{
   // TODO: methodReturn
   setInt16Value(ar.popInt());
}
void PrimitivePropertySet::playback_setInt32Value(const ArgsReader& ar)
{
   // TODO: methodReturn
   setInt32Value(ar.popInt());
}
void PrimitivePropertySet::playback_setInt64Value(const ArgsReader& ar)
{
   // TODO: methodReturn
   setInt64Value(ar.popInt());
}
void PrimitivePropertySet::playback_setFloatValue(const ArgsReader& ar)
{
   // TODO: methodReturn
   setFloatValue((float)ar.popDouble());
}
void PrimitivePropertySet::playback_setDoubleValue(const ArgsReader& ar)
{
   // TODO: methodReturn
   setDoubleValue(ar.popDouble());
}
