#include "APISignInDialogSample.h"

#include <QDebug>
#include "CLOAPIInterface.h"

namespace CLOAPISample
{
	APISignIn* APISignIn::_instance = NULL;
	
	APISignIn* APISignIn::getInstance()
	{
		if (_instance == NULL) {
			_instance = new APISignIn();
		}

		return _instance;
	}

	void APISignIn::destroy()
	{
		if (_instance) {
			delete _instance;
			_instance = NULL;
		}
	}

	void APISignIn::SetSignIn(bool b)
	{
		m_bSignIn = b;
	}
	bool APISignIn::GetSignIn()
	{
		return m_bSignIn;
	}

	APISigninDialogSample::APISigninDialogSample(QWidget* parent, Qt::WindowFlags flags)
	: QDialog(parent, flags)
	{
		setupUi(this);

		UTILITY_API->UpdateCloStyleForPlugIn(this);

		lineEdit_Passwd->setEchoMode(QLineEdit::Password);

		QObject::connect(okButton, SIGNAL(clicked()), this, SLOT(OnAccept()));
		QObject::connect(cancelButton, SIGNAL(clicked()), this, SLOT(OnReject()));

		m_ID = "";
		m_Passwd = "";
	}
	
	APISigninDialogSample::~APISigninDialogSample()
	{
	}

	void APISigninDialogSample::SetDefaultID(QString str_id)
	{
		lineEdit_ID->setText(m_ID);
	}

	void APISigninDialogSample::SetDefaultPasswd(QString str_passwd)
	{
		lineEdit_Passwd->setText(m_Passwd);
	}

	const QString& APISigninDialogSample::GetID()
	{
		return m_ID;
	}

	const QString& APISigninDialogSample::GetPasswd()
	{
		return m_Passwd;
	}

	void APISigninDialogSample::OnAccept()
	{
		m_ID = lineEdit_ID->text();
		m_Passwd = lineEdit_Passwd->text();

		accept();
	}

	void APISigninDialogSample::OnReject()
	{
		reject();
	}
}