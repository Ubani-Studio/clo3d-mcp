#pragma once

#include <vector>

using namespace std;

namespace CLOAPISample
{
	enum class PLMSettingProtocol
	{
		PLM_SETTING_HTTPS,
		PLM_SETTING_HTTP,
		PLM_SETTING_SSH,
	};

	// default values
	const QString DEFAULT_PLM_SERVER_URL = "www.plm.com";
	const PLMSettingProtocol DEFAULT_PLM_PROTOCOL = PLMSettingProtocol::PLM_SETTING_HTTPS;
	const QString DEFAULT_PLM_RESTAPI_ENDPOINT = "www.plm.com/endpoint_uri";
	const QString DEFAULT_CONFIGURATION_FILE_PATH = "c:/configuration/conf.json";

	class PLMSettingDataSample
	{
	private:
		static PLMSettingDataSample* _instance;
		PLMSettingDataSample();
		~PLMSettingDataSample();

	public:
		static PLMSettingDataSample*	getInstance();
		static void			destroy();

	private:
		void initialize();
		void clear();

	private:
		QString m_PLMServerURL;
		PLMSettingProtocol m_PLMProtocol;
		QString m_PLMRestAPIEndPoint;
		QString m_ConfigurationFilePath;

	public:
		QString GetPLMServerURL();
		PLMSettingProtocol GetPLMProtocol();
		QString GetPLMRestAPIEndPoint();
		QString GetConfigurationFilePath();

		void SetPLMServerURL(const QString& url);
		void SetPLMProtocol(const PLMSettingProtocol& protocol);
		void SetPLMRestAPIEndPoint(const QString& endpoint);
		void SetConfigurationFilePath(const QString& path);
	};
}

#define PLM_SETTING_DATA_SAMPLE (CLOAPISample::PLMSettingDataSample::getInstance())
#define DESTROY_PLM_SETTING_DATA_SAMPLE (CLOAPISample::PLMSettingDataSample::destroy())