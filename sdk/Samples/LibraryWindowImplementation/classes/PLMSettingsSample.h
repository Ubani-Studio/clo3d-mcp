#pragma once

#include <QDialog>

#include <string>
#include <iostream>

#include "ui_PLMSettingsSample.h"

using namespace std;

namespace CLOAPISample
{

	class PLMSettingsSample : public QWidget, public Ui::PLMSettings
	{
		Q_OBJECT
	
	public:
		PLMSettingsSample(QWidget* parent = 0);
		~PLMSettingsSample();		

		void Reset();

	private:
		void generateUIfromData();
		void saveDataFromUI();

		void connectSignalSlots();
		void disconnectSignalSlots();

	protected slots:
		void OnOpenClicked();
		void OnRefreshClicked();
	};
}