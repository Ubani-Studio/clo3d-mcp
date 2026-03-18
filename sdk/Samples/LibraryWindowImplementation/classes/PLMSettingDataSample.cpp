
#include "APICLODataBase.h"
#include "APIStorage.h"
#include "PLMSettingDataSample.h"

namespace CLOAPISample
{
	PLMSettingDataSample* PLMSettingDataSample::_instance = NULL;

	PLMSettingDataSample* PLMSettingDataSample::getInstance()
	{
		if (_instance == NULL) {
			_instance = new PLMSettingDataSample();
		}

		return _instance;
	}

	void PLMSettingDataSample::destroy() {
		if (_instance) {
			delete _instance;
			_instance = NULL;
		}
	}

	PLMSettingDataSample::PLMSettingDataSample()
	{
		initialize();
	}

	PLMSettingDataSample::~PLMSettingDataSample()
	{
		
	}

	void PLMSettingDataSample::initialize()
	{
		m_PLMServerURL = DEFAULT_PLM_SERVER_URL;
		m_PLMProtocol = DEFAULT_PLM_PROTOCOL;
		m_PLMRestAPIEndPoint = DEFAULT_PLM_RESTAPI_ENDPOINT;
		m_ConfigurationFilePath = DEFAULT_CONFIGURATION_FILE_PATH;
	}

	void PLMSettingDataSample::clear()
	{

	}

	QString PLMSettingDataSample::GetPLMServerURL()
	{
		return m_PLMServerURL;
	}

	PLMSettingProtocol PLMSettingDataSample::GetPLMProtocol()
	{
		return m_PLMProtocol;
	}

	QString PLMSettingDataSample::GetPLMRestAPIEndPoint()
	{
		return m_PLMRestAPIEndPoint;
	}

	QString PLMSettingDataSample::GetConfigurationFilePath()
	{
		return m_ConfigurationFilePath;
	}

	void PLMSettingDataSample::SetPLMServerURL(const QString& url)
	{
		m_PLMServerURL = url;
	}

	void PLMSettingDataSample::SetPLMProtocol(const PLMSettingProtocol& protocol)
	{
		m_PLMProtocol = protocol;
	}

	void PLMSettingDataSample::SetPLMRestAPIEndPoint(const QString& endpoint)
	{
		m_PLMRestAPIEndPoint = endpoint;
	}

	void PLMSettingDataSample::SetConfigurationFilePath(const QString& path)
	{
		m_ConfigurationFilePath = path;
	}
}