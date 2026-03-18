#pragma once

#include <string>
#include <iostream>

#include "ui_APISignInDialogSample.h"

using namespace std;

namespace CLOAPISample
{
	class APISignIn
	{
	private:
		static APISignIn* _instance;

		bool m_bSignIn;

		APISignIn()
		{
			m_bSignIn = false;
		}
		~APISignIn()
		{
		}
	public:
		static APISignIn*	getInstance();
		static void			destroy();

		void SetSignIn(bool b);
		bool GetSignIn();
	};

	class APISigninDialogSample : public  QDialog, public Ui::DialogSignIn
	{
		Q_OBJECT

	private:
		QString m_ID;
		QString m_Passwd;

	public:
		APISigninDialogSample(QWidget* parent = 0, Qt::WindowFlags flags = Qt::Dialog | Qt::FramelessWindowHint);
		~APISigninDialogSample();

		void SetDefaultID(QString str_id);
		void SetDefaultPasswd(QString str_passwd);

		const QString& GetID();
		const QString& GetPasswd();

	protected slots:
		void OnAccept();
		void OnReject();
		
	};
}