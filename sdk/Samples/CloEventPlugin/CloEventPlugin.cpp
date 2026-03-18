#include "stdafx.h"

#include "CloEventPlugin.h"

#include "CLOAPIInterface.h"

namespace CLOAPI
{
static std::vector<std::string> GetMimeData(const std::string& _mimeType, const std::unordered_map<std::string, std::vector<std::string>>& _mimeData)
{
	auto it = _mimeData.find(_mimeType);
	if (it != _mimeData.end()) {
		return it->second;
	}
	return {};
}

bool CloEventPlugin::MouseDropEventOn2DView(float _scenePosX, float _scenePosY, const std::unordered_map<std::string, std::vector<std::string>>& _mimeData)
{
	printf("MouseDropEventOn2DView : %f(x), %f(y)\n", _scenePosX, _scenePosY);

	std::vector<std::string> urlList = GetMimeData("text/uri-list", _mimeData);
	std::vector<std::string> textList = GetMimeData("text/plain", _mimeData);
	if (!urlList.empty())
	{
        // do something
		for (int i = 0; i < urlList.size(); i++)
			printf("url[%d] : %s\n", i, urlList.at(i).c_str());
	}
	if (!textList.empty())
	{
		// do something
		for (int i = 0; i < textList.size(); i++)
			printf("text[%d] : %s\n", i, textList.at(i).c_str());

		// This is an example in which the fabric file path is received as "text/plain", added to the CLO, and assigned to the pattern located in the drop.
		auto patternAPI = CLOAPI::APICommand::getInstance().GetPatternAPI();
		auto fabricAPI = CLOAPI::APICommand::getInstance().GetFabricAPI();
		auto utilityAPI = CLOAPI::APICommand::getInstance().GetUtilityAPI();
		auto fabricFilePath = textList.at(0);

		utilityAPI->CreateProgressBar();
		utilityAPI->SetProgress("Load Fabric", 30);
		auto addedFabricIndex = fabricAPI->AddFabric(fabricFilePath);
		utilityAPI->SetProgress("Search Pattern", 60);
		auto selectedPatternIndex = patternAPI->GetPatternIndexFrom2DView(_scenePosX, _scenePosY);

		if (addedFabricIndex >= 0 && selectedPatternIndex >= 0)
		{
			utilityAPI->SetProgress("Assign Fabric to Pattern", 100);
			auto result = fabricAPI->AssignFabricToPattern(addedFabricIndex, selectedPatternIndex, 1);
			if (!result)
				utilityAPI->DisplayMessageBox("failed assign fabric to pattern. file path : " + fabricFilePath + "\n");
		}
		else
		{
			utilityAPI->DeleteProgressBar(true);
			utilityAPI->DisplayMessageBox("failed load fabric. file path : " + fabricFilePath + "\n");
		}
	}

    return true;
}

bool CloEventPlugin::MouseDropEventOn3DView(float _scenePosX, float _scenePosY, const std::unordered_map<std::string, std::vector<std::string>>& _mimeData)
{
	printf("MouseDropEventOn3DView : %f(x), %f(y)\n", _scenePosX, _scenePosY);

	std::vector<std::string> urlList = GetMimeData("text/uri-list", _mimeData);
	std::vector<std::string> textList = GetMimeData("text/plain", _mimeData);
	if (!urlList.empty())
	{
		// do something
		for (int i = 0; i < urlList.size(); i++)
			printf("url[%d] : %s\n", i, urlList.at(i).c_str());
	}
	if (!textList.empty())
	{
		// do something
		for (int i = 0; i < textList.size(); i++)
			printf("text[%d] : %s\n", i, textList.at(i).c_str());

		// This is an example in which the fabric file path is received as "text/plain", added to the CLO, and assigned to the pattern located in the drop.
		auto patternAPI = CLOAPI::APICommand::getInstance().GetPatternAPI();
		auto fabricAPI = CLOAPI::APICommand::getInstance().GetFabricAPI();
		auto utilityAPI = CLOAPI::APICommand::getInstance().GetUtilityAPI();
		auto fabricFilePath = textList.at(0);

		utilityAPI->CreateProgressBar();
		utilityAPI->SetProgress("Load Fabric", 30);
		auto addedFabricIndex = fabricAPI->AddFabric(fabricFilePath);
		utilityAPI->SetProgress("Search Pattern", 60);
		auto selectedPatternIndex = patternAPI->GetPatternIndexFrom3DView(_scenePosX, _scenePosY);

		if (addedFabricIndex >= 0 && selectedPatternIndex >= 0)
		{
			utilityAPI->SetProgress("Assign Fabric to Pattern", 100);
			auto result = fabricAPI->AssignFabricToPattern(addedFabricIndex, selectedPatternIndex, 1);
			if (!result)
				utilityAPI->DisplayMessageBox("failed assign fabric to pattern. file path : " + fabricFilePath + "\n");
		}
		else
		{
			utilityAPI->DeleteProgressBar(true);
			utilityAPI->DisplayMessageBox("failed load fabric. file path : " + fabricFilePath + "\n");
		}
	}

	return true;
}
}

// the class factories
extern MV_CLO_SCENE_API CLOAPI::CloEventPlugin* Create() {
	return new CLOAPI::CloEventPlugin();
}