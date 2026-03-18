#pragma once

#if defined CLO_API_INTERFACE_STATIC_BUILD || defined(__APPLE__)
#define MV_CLO_INTERFACE_API
#else
#ifdef CLOAPIINTERFACE_EXPORTS
#define MV_CLO_INTERFACE_API __declspec(dllexport)
#else
#define MV_CLO_INTERFACE_API __declspec(dllimport)
#endif
#endif
