#pragma  once

#ifdef PLUG_EXPORTS
#define PLUG_API  __declspec(dllexport)
#else
#define PLUG_API  __declspec(dllimport)
#endif

