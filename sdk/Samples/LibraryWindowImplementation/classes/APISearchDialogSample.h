#pragma once

#include <QDialog>

#include <string>
#include <iostream>

#include "classes/APIUtility.h"
#include "classes/APICLODataBase.h"

#include "ui_APISearchDialogSample.h"

using namespace std;

namespace CLOAPISample
{
	enum SEARCH_ITEM_MODE
	{
		SEARCH_NONE,
		SEARCH_PATTERN,
		SEARCH_TRIM,
		SEARCH_FABRIC,
		SEARCH_PROJECT,
		SEARCH_AVATAR,
		SEARCH_GARMENT,
		SEARCH_TOPSTITCH,
		SEARCH_BUTTONHEAD,
		SEARCH_BUTTONHOLE,
		SEARCH_ZIPPER_PULLER,
		SEARCH_ZIPPER_SLIDER,
		SEARCH_ZIPPER_TOP_STOPPER,
		SEARCH_ZIPPER_BOTTOM_STOPPER
	};

	class APISearchDialogSample : public QDialog, public Ui::DialogSearch
	{
		Q_OBJECT

	public:
		APISearchDialogSample(QWidget* parent = 0, Qt::WindowFlags flags = Qt::Dialog | Qt::FramelessWindowHint, SEARCH_ITEM_MODE mode = SEARCH_NONE);
		~APISearchDialogSample();
		
		void SetSearchMode(SEARCH_ITEM_MODE mode);
		void SetSearchModeByItemId(const string& itemId);

		string GetSearchResult();

	protected slots:
		void OnAccept();
		void OnReject();
		
		void OnServiceDivisionComboBox(int _index);

	private:
		SEARCH_ITEM_MODE m_SearchMode;

		QString m_ResultItem;		

		// list for components
		QStringList m_ServiceDivisionList;
		QMap<QString, QStringList> m_BrandListPerServiceDivision;
		QStringList m_ProductTypeList;
		QStringList m_SeasonCreatedList;

		// generate list for components
		void generateHouseOfBrandListUI();
		void generateBrandListUI();
		void generateProductTypeListUI();
		void generateSeasonCreatedListUI();

		void generateUI();

		void setComboBoxLists();

		// result
		void calculateSampleSearchResult();

		vector<LibraryAPIItem> GetSearchItemList();

	};
}