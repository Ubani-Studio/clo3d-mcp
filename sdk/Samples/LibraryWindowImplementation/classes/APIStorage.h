#pragma once

#include <vector>

#include "APIDefine.h"

using namespace std;

namespace CLOAPISample
{
	class APIStorage
	{
	private:
		static APIStorage* _instance;
		APIStorage();
		~APIStorage();
	
	public:
		static APIStorage*	getInstance();
		static void			destroy();

		vector<LibraryAPIItem*> m_LibraryAPIItemList;

	private:
		void initialize();
		void clear();

		void setFileSizeForAllItems();
		unsigned int getFileSize(const QString& filePath);

	public:
		void GenerateAPIItemListForSample();

		vector<LibraryAPIItem> GetSearchList(const QString& searchText, const QVariantMap& searchKeyValues);
		QString GetParentFolderID(const QString& itemId);
		
		bool GetItemWithID(const QString& itemId, LibraryAPIItem& result);
		
		bool GetFilePathWithID(const QString& itemId, QString& resultFilePath);
		bool GetIconThumbnailPathWithID(const QString& itemId, QString& resultFilePath);
		bool GetPreviewThumbnailPathWithID(const QString& itemId, QString& resultFilePath);

	private:
		void generateAPIItemListForPatterns();
		void generateAPIItemListForSeasonPatterns();
		void generateAPIItemListForTrims();
		void generateAPIItemListForFabrics();
		void generateAPIItemListForProjectFiles();
		void generateAPIItemListForAvatars();
		void generateAPIItemListForGarments();
		void generateAPIItemListForTopstitches();
		void generateAPIItemListForButtonHeads();
		void generateAPIItemListForButtonHoles();
		void generateAPIItemListForZipperPulers(); // generate many items to show Scrolling Page sample
		void generateAPIItemListForZipperSliders();
		void generateAPIItemListForZipperTopStoppers();
		void generateAPIItemListForZipperBottomStoppers();

		LibraryAPIItem* newLibraryAPIItemWithRandomizedDate(); // constructing an instance for LibraryAPIItem with radomly set in 'Date' and return the pointer
	};

}

#define API_STORAGE (CLOAPISample::APIStorage::getInstance())
#define DESTROY_API_STORAGE (CLOAPISample::APIStorage::destroy())