#include "PLMSettingsSample.h"
#include "PLMSettingDataSample.h"

#include "CLOAPIInterface.h"

#include <QDebug>
#include <QFileDialog>

namespace CLOAPISample
{
	PLMSettingsSample::PLMSettingsSample(QWidget* parent)
	: QWidget(parent)
	{
		setupUi(this);

		UTILITY_API->UpdateCloStyleForPlugIn(this);

		generateUIfromData();
		connectSignalSlots();
	}
	
	PLMSettingsSample::~PLMSettingsSample()
	{
		// DESTROY_PLM_SETTING_DATA_SAMPLE // do not use this here. please release plm setting data when unloading the library windows sample plug-in dll.

		saveDataFromUI();
		disconnectSignalSlots();
	}

	void PLMSettingsSample::Reset()
	{
		// re-create the PLM settings data instance
		DESTROY_PLM_SETTING_DATA_SAMPLE;
		
		// create ui from the data
		generateUIfromData();
	}

	void PLMSettingsSample::generateUIfromData()
	{
		CLOAPISample::PLMSettingDataSample* plmSettingsData = PLM_SETTING_DATA_SAMPLE;
		
		if (plmSettingsData)
		{
			lineEdit_PLMServerURL->setText(plmSettingsData->GetPLMServerURL());
			lineEdit_EndPoint->setText(plmSettingsData->GetPLMRestAPIEndPoint());
			lineEdit_ConfigurationFile->setText(plmSettingsData->GetConfigurationFilePath());

			if (plmSettingsData->GetPLMProtocol() == PLMSettingProtocol::PLM_SETTING_HTTPS)
				comboBox_Protocol->setCurrentIndex(0);
			else if (plmSettingsData->GetPLMProtocol() == PLMSettingProtocol::PLM_SETTING_HTTP)
				comboBox_Protocol->setCurrentIndex(1);
			else if (plmSettingsData->GetPLMProtocol() == PLMSettingProtocol::PLM_SETTING_SSH)
				comboBox_Protocol->setCurrentIndex(2);
			else
				comboBox_Protocol->setCurrentIndex(0);
				
		}
	}

	void PLMSettingsSample::saveDataFromUI()
	{
		CLOAPISample::PLMSettingDataSample* plmSettingsData = PLM_SETTING_DATA_SAMPLE;

		if (plmSettingsData)
		{
			plmSettingsData->SetPLMServerURL(lineEdit_PLMServerURL->text());
			plmSettingsData->SetPLMRestAPIEndPoint(lineEdit_EndPoint->text());
			plmSettingsData->SetConfigurationFilePath(lineEdit_ConfigurationFile->text());

			int protocolIndex = comboBox_Protocol->currentIndex();
			plmSettingsData->SetPLMProtocol(DEFAULT_PLM_PROTOCOL);

			if (protocolIndex == 0)
				plmSettingsData->SetPLMProtocol(PLMSettingProtocol::PLM_SETTING_HTTPS);
			else if (protocolIndex == 1)
				plmSettingsData->SetPLMProtocol(PLMSettingProtocol::PLM_SETTING_HTTP);
			else if (protocolIndex == 2)
				plmSettingsData->SetPLMProtocol(PLMSettingProtocol::PLM_SETTING_SSH);
		}
	}
	
	void PLMSettingsSample::connectSignalSlots()
	{
		connect(pbOpen, SIGNAL(clicked()), this, SLOT(OnOpenClicked()));
		connect(pbRefresh, SIGNAL(clicked()), this, SLOT(OnRefreshClicked()));
	}
	
	void PLMSettingsSample::disconnectSignalSlots()
	{
		disconnect(pbOpen, SIGNAL(clicked()), this, SLOT(OnOpenClicked()));
		disconnect(pbRefresh, SIGNAL(clicked()), this, SLOT(OnRefreshClicked()));
	}

	void PLMSettingsSample::OnOpenClicked()
	{
		QString selectFilter = "";
		selectFilter += "All files (*.*);;";
		selectFilter += "Configuration files (*.json);;";

		QString filePath = QFileDialog::getOpenFileName(this, "Open Configuration File", lineEdit_ConfigurationFile->text(), selectFilter, &selectFilter);

		QFileInfo fi(filePath);
		if (fi.exists())
			lineEdit_ConfigurationFile->setText(filePath);
	}

	void PLMSettingsSample::OnRefreshClicked()
	{
		QString defaultFilePath = DEFAULT_CONFIGURATION_FILE_PATH;

		// set data
		CLOAPISample::PLMSettingDataSample* plmSettingsData = PLM_SETTING_DATA_SAMPLE;
		plmSettingsData->SetConfigurationFilePath(defaultFilePath);

		// set ui
		lineEdit_ConfigurationFile->setText(plmSettingsData->GetConfigurationFilePath());
	}
}