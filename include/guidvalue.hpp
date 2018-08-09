#pragma once

typedef struct _GUID {
   unsigned long  Data1;
   unsigned short Data2;
   unsigned short Data3;
   unsigned char  Data4[8];
} GUID;
typedef struct _GUID GUID;

#ifndef DEFINE_GUID
#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    EXTERN_C const GUID FAR name
#else // !defined(INITGUID)

__inline int InlineIsEqualGUID(const GUID& rguid1, const GUID& rguid2)
{
   return (
      ((unsigned long *)&rguid1)[0] == ((unsigned long *)&rguid2)[0] &&
      ((unsigned long *)&rguid1)[1] == ((unsigned long *)&rguid2)[1] &&
      ((unsigned long *)&rguid1)[2] == ((unsigned long *)&rguid2)[2] &&
      ((unsigned long *)&rguid1)[3] == ((unsigned long *)&rguid2)[3]);
}

enum GUIDconstructionOption { GUIDinvalid, GUIDnew };

class GUIDvalue
{
public:
   // construct
   // No-arg call makes a null value:
   // Call with argument GUIDnew to generate one:
   GUIDvalue(GUIDconstructionOption option = GUIDinvalid);
   GUIDvalue(const GUIDvalue &g);
   GUIDvalue& operator= (const GUIDvalue &g);

   // a constant null
   static const GUIDvalue invalidGUIDvalue;

   // creation from plain GUID
   explicit GUIDvalue( const GUID & );
   const GUID& asGUID() const { return m_guid; };
   
   // convert
   explicit GUIDvalue(const std::string&);
   std::string getGuid() const;

   bool operator == (const GUIDvalue &rhs) const
   { return !!(InlineIsEqualGUID(m_guid,rhs.m_guid)); }
   bool operator != (const GUIDvalue &rhs) const
   { return !(*this == rhs); }
   bool operator < (const GUIDvalue &rhs) const
   { return (memcmp(&m_guid,&rhs.m_guid,sizeof(GUID)) < 0); }
   bool isNull() const
   { return !!(InlineIsEqualGUID(m_guid,GUID_NULL)); }
   size_t hash() const
   {
      static_assert(sizeof(_GUID) == 128 / CHAR_BIT, "GUID");

      std::hash<uint64_t> _HashFunctor;
      GUID guid = asGUID();
      uint64_t* p = reinterpret_cast<uint64_t*>(&guid);
      // XOR both halves
      return
         _HashFunctor(p[0])
         ^ _HashFunctor(p[1]);
   }
private:
   GUID m_guid;
};

struct GuidHasher
{
   FORCEINLINE size_t operator()(const GUIDvalue& rGUIDvalue) const
   {
      return rGUIDvalue.hash();
   }
};

#endif
