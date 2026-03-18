#pragma once

#include <QString>
#include <QVariant>

const QString META_DATA_KEY_0_DATA_TYPE = QString("Data Type");
const QString META_DATA_KEY_1_SERVICE_DIVISION = QString("CLO Service Division");
const QString META_DATA_KEY_2_BRAND = QString("Brand");
const QString META_DATA_KEY_3_PRODUCT_TYPE = QString("Product Type");
const QString META_DATA_KEY_4_SEASON_CREATED = QString("Season Created");

namespace CLOAPISample
{
	struct LibraryAPISampleItem
	{
		QString itemPath; // for sample only
		QString iconThumbnailPath; // for sample only
		QString previewThumbnailPath; // for sample only
		QString parentItemID; // for sample only

		LibraryAPISampleItem() : itemPath(""), iconThumbnailPath(""), previewThumbnailPath(""), parentItemID("") {}
	};

	struct LibraryAPIItem
	{
		QString itemID;
		QString itemName;
		QString itemType;	
		QString dateTime;
		unsigned int filesize;
		QVariantMap metaData; // user custom field (key - value list)

		LibraryAPISampleItem sampleItemData;
		// designated initializer 
		LibraryAPIItem() :itemID(""), itemName(""), itemType("FILE"), dateTime(""), filesize(0.0f), metaData(QVariantMap()) {}
	};
}

#ifdef __APPLE__
	const QString SAMPLE_STORAGE_DIRECTORY = QString("/Users/inoblaze/sample_assets/");
#else
	const QString SAMPLE_STORAGE_DIRECTORY = QString("C:/sample_assets/");
#endif
