# CppPlayer
Header-only library - API adapter for Playback-Based Testing

Steps to integrate CppPlayer:
1. Derive your operational base class from IOperational.
2. Insert following static method into that base class declaration.
```
   static inline CallMap& libraryCallMap()
   {
      static CallMap yourCallmap;
      return yourCallmap;
   }
```
3. To wrap each of your business objects, derive a class from your operational base class.
4. Derive your main business object from IMain.
5. Instrument your business object wrappers.

The framework requires that wrapper objects instantiate using std::shared_ptr

Instrumentation:
1. From each instrumented API, invoke recordFunction at the end.
2. (Or recordMethod if function returns void).
3. Declare and implement a playback method.

Samples

IHFDM sample (does not compile)
