#pragma once

#ifdef CLO_EVENT_API_PLUGIN
    #if defined(__APPLE__) || defined(__linux__)
        #define MV_CLO_SCENE_API
    #else
    #define MV_CLO_SCENE_API __declspec(dllexport)
    #endif

#else
    #if defined(__APPLE__) || defined(__linux__)
        #define MV_CLO_SCENE_API 
    #else
        #define MV_CLO_SCENE_API __declspec(dllimport)
    #endif 
#endif

#include <string>
#include <unordered_map>
#include <vector>

namespace CLOAPI
{
class MV_CLO_SCENE_API CloEventPlugin
{
public:
	CloEventPlugin() {}
	virtual ~CloEventPlugin() {}
    // _mimeData : QMimeData from QT has been converted for use in the c++ std library.
    // key : "text/plain", "text/html", "text/uri-list", "application/x-qt-windows-mime;value=\"FileName\"", "application/octet-stream"
    virtual bool MouseDropEventOn2DView(float _scenePosX, float _scenePosY, const std::unordered_map<std::string, std::vector<std::string>>& _mimeData);
    virtual bool MouseDropEventOn3DView(float _scenePosX, float _scenePosY, const std::unordered_map<std::string, std::vector<std::string>>& _mimeData);
};
}
extern "C"
{
	extern MV_CLO_SCENE_API CLOAPI::CloEventPlugin* Create();
}


