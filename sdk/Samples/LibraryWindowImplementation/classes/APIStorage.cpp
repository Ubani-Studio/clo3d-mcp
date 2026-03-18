#include "APICLODataBase.h"
#include "APIStorage.h"
#include "APIUtility.h"

#include "CLOAPIInterface.h"

namespace CLOAPISample
{
	APIStorage* APIStorage::_instance = NULL;

	APIStorage* APIStorage::getInstance()
	{
		if (_instance == NULL) {
			_instance = new APIStorage();
		}

		return _instance;
	}

	void APIStorage::destroy() {
		if (_instance) {
			delete _instance;
			_instance = NULL;
		}
	}

	APIStorage::APIStorage()
	{
		initialize();
	}

	APIStorage::~APIStorage()
	{
		clear();
	}

	void APIStorage::initialize()
	{
		m_LibraryAPIItemList.clear();
	}

	void APIStorage::clear()
	{
		for (size_t i = 0; i < m_LibraryAPIItemList.size(); ++i)
		{
			 delete m_LibraryAPIItemList[i];
			 m_LibraryAPIItemList[i] = NULL;
		}

		m_LibraryAPIItemList.clear();
	}

	void APIStorage::setFileSizeForAllItems()
	{
		for (size_t i = 0; i < m_LibraryAPIItemList.size(); ++i)
		{
			QString filePath;
			GetFilePathWithID(m_LibraryAPIItemList[i]->itemID, filePath);
			m_LibraryAPIItemList[i]->filesize = getFileSize(filePath);
		}			
	}	
	
	unsigned int APIStorage::getFileSize(const QString& filePath)
	{
		unsigned int size;

		FILE* pFile = NULL;
#if defined( __APPLE__ ) || defined(__linux__)
		pFile = FileOpen(&filePath, "r");
#else
		pFile = FileOpen(&filePath, "rb");
#endif
		if (!pFile) 
			return 0;

		fseek(pFile, 0, SEEK_END);
		size = ftell(pFile);
		fclose(pFile);

		return size;
	}

	void APIStorage::GenerateAPIItemListForSample()
	{
		clear();

		generateAPIItemListForPatterns();
		generateAPIItemListForSeasonPatterns();
		generateAPIItemListForTrims();
		generateAPIItemListForFabrics();
		generateAPIItemListForProjectFiles();
		generateAPIItemListForAvatars();
		generateAPIItemListForGarments();
		generateAPIItemListForTopstitches();
		generateAPIItemListForButtonHeads();
		generateAPIItemListForButtonHoles();
		generateAPIItemListForZipperPulers();
		generateAPIItemListForZipperSliders();
		generateAPIItemListForZipperTopStoppers();
		generateAPIItemListForZipperBottomStoppers();

		setFileSizeForAllItems();
	}

	vector<LibraryAPIItem> APIStorage::GetSearchList(const QString& searchText, const QVariantMap& searchKeyValues)
	{
		vector<LibraryAPIItem> searchResult;

		for (size_t i = 0; i < m_LibraryAPIItemList.size(); ++i)
		{
			if (m_LibraryAPIItemList[i]->itemName.contains(searchText, Qt::CaseInsensitive))
			{
				bool bMatched = true;
				for (QVariantMap::const_iterator iter = searchKeyValues.constBegin(); iter != searchKeyValues.constEnd(); ++iter)
				{
					if (iter.value().toString().compare("All", Qt::CaseInsensitive) != 0 &&
						!m_LibraryAPIItemList[i]->metaData[iter.key()].toString().contains(iter.value().toString()))
					{
						bMatched = false;
						break;
					}
				}

				if (bMatched)
					searchResult.push_back(*m_LibraryAPIItemList[i]);
			}
		}

		return searchResult;
	}

	bool APIStorage::GetItemWithID(const QString& itemId, LibraryAPIItem& result)
	{
		for (size_t i = 0; i < m_LibraryAPIItemList.size(); ++i)
		{
			if (m_LibraryAPIItemList[i]->itemID.compare(itemId, Qt::CaseInsensitive) == 0)
			{
				result = *m_LibraryAPIItemList[i];
				return true;
			}
		}

		return false;
	}

	bool APIStorage::GetFilePathWithID(const QString& itemId, QString& resultFilePath)
	{
		LibraryAPIItem curItem;
		
		if (GetItemWithID(itemId, curItem))
		{
			resultFilePath = SAMPLE_STORAGE_DIRECTORY + curItem.metaData[META_DATA_KEY_0_DATA_TYPE].toString().toLower() + "/" + GetOnlyFileName(curItem.sampleItemData.itemPath);
			return true;
		}

		return false;
	}

	bool APIStorage::GetIconThumbnailPathWithID(const QString& itemId, QString& resultFilePath)
	{
		LibraryAPIItem curItem;

		if (GetItemWithID(itemId, curItem))
		{
			if(curItem.sampleItemData.iconThumbnailPath != "")
				resultFilePath = SAMPLE_STORAGE_DIRECTORY + curItem.metaData[META_DATA_KEY_0_DATA_TYPE].toString().toLower() + "/" + GetOnlyFileName(curItem.sampleItemData.iconThumbnailPath);
			else
				resultFilePath = SAMPLE_STORAGE_DIRECTORY + curItem.metaData[META_DATA_KEY_0_DATA_TYPE].toString().toLower() + "/" + GetOnlyFileName(curItem.sampleItemData.itemPath);

			return true;
		}

		return false;
	}

	bool APIStorage::GetPreviewThumbnailPathWithID(const QString& itemId, QString& resultFilePath)
	{
		LibraryAPIItem curItem;

		if (GetItemWithID(itemId, curItem))
		{
			if (curItem.sampleItemData.previewThumbnailPath != "")
				resultFilePath = SAMPLE_STORAGE_DIRECTORY + curItem.metaData[META_DATA_KEY_0_DATA_TYPE].toString().toLower() + "/" + GetOnlyFileName(curItem.sampleItemData.previewThumbnailPath);
			else
				resultFilePath = SAMPLE_STORAGE_DIRECTORY + curItem.metaData[META_DATA_KEY_0_DATA_TYPE].toString().toLower() + "/" + GetOnlyFileName(curItem.sampleItemData.itemPath);

			return true;
		}

		return false;
	}

	QString APIStorage::GetParentFolderID(const QString& itemId)
	{
		for (const auto& item: m_LibraryAPIItemList)
		{
			if (item == nullptr)
				continue;

			if (item->itemID == itemId)
				return item->sampleItemData.parentItemID;
		}
		return QString();
	}

	void APIStorage::generateAPIItemListForPatterns()
	{
		LibraryAPIItem* patternFolder1 = newLibraryAPIItemWithRandomizedDate();
		patternFolder1->itemID = ITEM_ID_PATTERN_SEASON;
		patternFolder1->itemName = "Season";
		patternFolder1->metaData[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_PATTERN;
		patternFolder1->sampleItemData.itemPath = SAMPLE_STORAGE_DIRECTORY + "pattern/" + patternFolder1->itemID.toLower();
		patternFolder1->sampleItemData.parentItemID = ITEM_ID_PATTERN;
		m_LibraryAPIItemList.push_back(patternFolder1);
		/// pattern items 
// item1
		LibraryAPIItem* patternItem1 = newLibraryAPIItemWithRandomizedDate();
		patternItem1->itemID = "CLO_MENS_1101";
		patternItem1->itemName = "CLO_MENS_1101.dxf";
		//patternItem1->dateTime = "2019-08-20T16:21:41";
		patternItem1->metaData[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_PATTERN;
		patternItem1->metaData[META_DATA_KEY_1_SERVICE_DIVISION] = SD_CLO;
		patternItem1->metaData[META_DATA_KEY_2_BRAND] = BR_CLO_MENS;
		patternItem1->metaData[META_DATA_KEY_3_PRODUCT_TYPE] = PT_FULL_SLEEVE_SHIRTS;
		patternItem1->metaData[META_DATA_KEY_4_SEASON_CREATED] = SEASON_SPRING_SUMMER_20;
		patternItem1->sampleItemData.itemPath = "CLO_MENS_1101.dxf";
		patternItem1->sampleItemData.iconThumbnailPath = "CLO_MENS_1101_thumb.png";
		patternItem1->sampleItemData.previewThumbnailPath = "CLO_MENS_1101_thumb.png";

		m_LibraryAPIItemList.push_back(patternItem1);

		// item3
		LibraryAPIItem* patternItem3 = newLibraryAPIItemWithRandomizedDate();
		patternItem3->itemID = "Pattern #3";
		patternItem3->itemName = "Pattern #3.dxf";
		//patternItem3->dateTime = "2019-08-18T16:00:31";
		patternItem3->metaData[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_PATTERN;
		patternItem3->metaData[META_DATA_KEY_1_SERVICE_DIVISION] = SD_OVERSEAS;
		patternItem3->metaData[META_DATA_KEY_2_BRAND] = BR_OVERSEAS_CASUALS;
		patternItem3->metaData[META_DATA_KEY_3_PRODUCT_TYPE] = PT_CARGO;
		patternItem3->metaData[META_DATA_KEY_4_SEASON_CREATED] = SEASON_AUTUMN_WINTER_20;
		patternItem3->sampleItemData.itemPath = "Pattern #3.dxf";
		patternItem3->sampleItemData.iconThumbnailPath = "Pattern #3_thumb.png";
		patternItem3->sampleItemData.previewThumbnailPath = "Pattern #3_thumb.png";

		m_LibraryAPIItemList.push_back(patternItem3);

		// item4
		LibraryAPIItem* patternItem4 = newLibraryAPIItemWithRandomizedDate();
		patternItem4->itemID = "Pattern #4";
		patternItem4->itemName = "Pattern #4.dxf";
		//patternItem4->dateTime = "2019-08-25T12:00:00";
		patternItem4->metaData[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_PATTERN;
		patternItem4->metaData[META_DATA_KEY_1_SERVICE_DIVISION] = SD_FITTING;
		patternItem4->metaData[META_DATA_KEY_2_BRAND] = BR_C_MIRROR;
		patternItem4->metaData[META_DATA_KEY_3_PRODUCT_TYPE] = PT_DENIMS;
		patternItem4->metaData[META_DATA_KEY_4_SEASON_CREATED] = SEASON_AUTUMN_WINTER_20;
		patternItem4->sampleItemData.itemPath = "Pattern #4.dxf";
		patternItem4->sampleItemData.iconThumbnailPath = "Pattern #4_thumb.png";
		patternItem4->sampleItemData.previewThumbnailPath = "Pattern #4_thumb.png";

		m_LibraryAPIItemList.push_back(patternItem4);
	}

	void APIStorage::generateAPIItemListForSeasonPatterns()
	{
		// item2
		LibraryAPIItem* seasonItem1 = newLibraryAPIItemWithRandomizedDate();
		seasonItem1->itemID = "CLO_WOMENS_1102";
		seasonItem1->itemName = "CLO_WOMENS_1102.dxf";
		//seasonItem1->dateTime = "2019-08-22T12:00:41";
		seasonItem1->metaData[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_PATTERN_SEASON;
		seasonItem1->metaData[META_DATA_KEY_1_SERVICE_DIVISION] = SD_NXT;
		seasonItem1->metaData[META_DATA_KEY_2_BRAND] = BR_CLOSET_WEB;
		seasonItem1->metaData[META_DATA_KEY_3_PRODUCT_TYPE] = PT_JACKETS;
		seasonItem1->metaData[META_DATA_KEY_4_SEASON_CREATED] = SEASON_AUTUMN_WINTER_19;
		seasonItem1->sampleItemData.itemPath = "CLO_WOMENS_1102.dxf";
		seasonItem1->sampleItemData.iconThumbnailPath = "CLO_WOMENS_1102_thumb.png";
		seasonItem1->sampleItemData.previewThumbnailPath = "CLO_WOMENS_1102_thumb.png";

		m_LibraryAPIItemList.push_back(seasonItem1);
	}

	void APIStorage::generateAPIItemListForTrims()
	{
		/// trims
		// item1
		LibraryAPIItem* trimItem1 = newLibraryAPIItemWithRandomizedDate();
		trimItem1->itemID = "TRIM_1";
		trimItem1->itemName = "Trim1.obj";
		//trimItem1->dateTime = "2019-08-20T16:21:41";
		trimItem1->metaData[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_TRIM;
		trimItem1->metaData["size"] = QString("M");
		trimItem1->metaData["use"] = QString("obj");
		trimItem1->sampleItemData.itemPath = "Trim1.obj";
		trimItem1->sampleItemData.iconThumbnailPath = "Trim1_thumb.png";
		trimItem1->sampleItemData.previewThumbnailPath = "Trim1_thumb.png";
		m_LibraryAPIItemList.push_back(trimItem1);

		// item2
		LibraryAPIItem* trimItem2 = newLibraryAPIItemWithRandomizedDate();
		trimItem2->itemID = "TRIM_2";
		trimItem2->itemName = "Trim2.obj";
		//trimItem2->dateTime = "2019-08-17T16:00:26";
		trimItem2->metaData[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_TRIM;
		trimItem2->metaData["size"] = QString("S");
		trimItem2->metaData["use"] = QString("obj");
		trimItem2->sampleItemData.itemPath = "Trim2.obj";
		trimItem2->sampleItemData.iconThumbnailPath = "Trim2_thumb.png";
		trimItem2->sampleItemData.previewThumbnailPath = "Trim2_thumb.png";
		m_LibraryAPIItemList.push_back(trimItem2);

		// item3
		LibraryAPIItem* trimItem3 = newLibraryAPIItemWithRandomizedDate();
		trimItem3->itemID = "TRIM_3";
		trimItem3->itemName = "D-Ring.trm";
		//trimItem3->dateTime = "2019-08-19T08:41:12";
		trimItem3->metaData[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_TRIM;
		trimItem3->metaData["size"] = QString("L");
		trimItem3->metaData["use"] = QString("trm");
		trimItem3->sampleItemData.itemPath = "D-Ring.trm";
		m_LibraryAPIItemList.push_back(trimItem3);

		// item4
		LibraryAPIItem* trimItem4 = newLibraryAPIItemWithRandomizedDate();
		trimItem4->itemID = "TRIM_4";
		trimItem4->itemName = "D-Ring_Small.trm";
		//trimItem4->dateTime = "2019-09-20T18:11:11";
		trimItem4->metaData[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_TRIM;
		trimItem4->metaData["size"] = QString("XS");
		trimItem4->metaData["use"] = QString("trm");
		trimItem4->sampleItemData.itemPath = "D-Ring_Small.trm";
		m_LibraryAPIItemList.push_back(trimItem4);

		// item5
		LibraryAPIItem* trimItem5 = newLibraryAPIItemWithRandomizedDate();
		trimItem5->itemID = "TRIM_5";
		trimItem5->itemName = "O-Ring.trm";
//		trimItem5->dateTime = "2019-05-20T19:21:41";
		trimItem5->metaData[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_TRIM;
		trimItem5->metaData["size"] = QString("M");
		trimItem5->metaData["use"] = QString("trm");
		trimItem5->sampleItemData.itemPath = "O-Ring.trm";
		m_LibraryAPIItemList.push_back(trimItem5);

		// item6
		LibraryAPIItem* trimItem6 = newLibraryAPIItemWithRandomizedDate();
		trimItem6->itemID = "TRIM_6";
		trimItem6->itemName = "Slider.trm";
//		trimItem6->dateTime = "2019-08-23T18:07:19";
		trimItem6->metaData[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_TRIM;
		trimItem6->metaData["size"] = QString("M");
		trimItem6->metaData["use"] = QString("trm");
		trimItem6->sampleItemData.itemPath = "Slider.trm";
		m_LibraryAPIItemList.push_back(trimItem6);
	}

	void APIStorage::generateAPIItemListForFabrics()
	{
		/// fabric
		// fabricItem1
		LibraryAPIItem* fabricItem1 = newLibraryAPIItemWithRandomizedDate();
		fabricItem1->itemID = "FABRIC_1";
		fabricItem1->itemName = "fabric1.jpg";
//		fabricItem1->dateTime = "2019-01-20T11:11:21";
		fabricItem1->metaData[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_FABRIC;
		fabricItem1->metaData[META_DATA_KEY_1_SERVICE_DIVISION] = SD_MD;
		fabricItem1->metaData[META_DATA_KEY_2_BRAND] = BR_MD_MOVIEW;
		fabricItem1->metaData[META_DATA_KEY_3_PRODUCT_TYPE] = PT_FABRIC_SILK_SATIN;
		fabricItem1->metaData[META_DATA_KEY_4_SEASON_CREATED] = SEASON_SPRING_SUMMER_20;
		fabricItem1->sampleItemData.itemPath = SAMPLE_STORAGE_DIRECTORY + "fabric/" + "fabric1.jpg";
		fabricItem1->sampleItemData.iconThumbnailPath = SAMPLE_STORAGE_DIRECTORY + "fabric/" + "fabric1.jpg";
		fabricItem1->sampleItemData.previewThumbnailPath = SAMPLE_STORAGE_DIRECTORY + "fabric/" + "fabric1.jpg";

		m_LibraryAPIItemList.push_back(fabricItem1);

		// fabricItem2
		LibraryAPIItem* fabricItem2 = newLibraryAPIItemWithRandomizedDate();
		fabricItem2->itemID = "FABRIC_2";
		fabricItem2->itemName = "fabric2.jpg";
		//fabricItem2->dateTime = "2019-02-17T14:21:31";
		fabricItem2->metaData[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_FABRIC;
		fabricItem2->metaData[META_DATA_KEY_1_SERVICE_DIVISION] = SD_FITTING;
		fabricItem2->metaData[META_DATA_KEY_2_BRAND] = BR_ONLINE;
		fabricItem2->metaData[META_DATA_KEY_3_PRODUCT_TYPE] = PT_FABRIC_SILK_SATIN;
		fabricItem2->metaData[META_DATA_KEY_4_SEASON_CREATED] = SEASON_REGULAR_SEASON_CORE;
		fabricItem2->sampleItemData.itemPath = SAMPLE_STORAGE_DIRECTORY + "fabric/" + "fabric2.jpg";
		fabricItem2->sampleItemData.iconThumbnailPath = SAMPLE_STORAGE_DIRECTORY + "fabric/" + "fabric2.jpg";
		fabricItem2->sampleItemData.previewThumbnailPath = SAMPLE_STORAGE_DIRECTORY + "fabric/" + "fabric2.jpg";

		m_LibraryAPIItemList.push_back(fabricItem2);

		// fabricItem3
		LibraryAPIItem* fabricItem3 = newLibraryAPIItemWithRandomizedDate();
		fabricItem3->itemID = "FABRIC_3";
		fabricItem3->itemName = "fabric3.png";
		//fabricItem3->dateTime = "2019-08-20T16:21:41";
		fabricItem3->metaData[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_FABRIC;
		fabricItem3->metaData[META_DATA_KEY_1_SERVICE_DIVISION] = SD_MD;
		fabricItem3->metaData[META_DATA_KEY_2_BRAND] = BR_MD_GAME;
		fabricItem3->metaData[META_DATA_KEY_3_PRODUCT_TYPE] = PT_FABRIC_MATTE;
		fabricItem3->metaData[META_DATA_KEY_4_SEASON_CREATED] = SEASON_AUTUMN_WINTER_19;
		fabricItem3->sampleItemData.itemPath = SAMPLE_STORAGE_DIRECTORY + "fabric/" + "fabric3.png";
		fabricItem3->sampleItemData.iconThumbnailPath = SAMPLE_STORAGE_DIRECTORY + "fabric/" + "fabric3.png";
		fabricItem3->sampleItemData.previewThumbnailPath = SAMPLE_STORAGE_DIRECTORY + "fabric/" + "fabric3.png";
		m_LibraryAPIItemList.push_back(fabricItem3);

		// fabricItem4
		LibraryAPIItem* fabricItem4 = newLibraryAPIItemWithRandomizedDate();
		fabricItem4->itemID = "Cotton_14_Wale_Corduroy";
		fabricItem4->itemName = "Cotton_14_Wale_Corduroy.zfab";
		//fabricItem4->dateTime = "2019-08-20T16:21:41";
		fabricItem4->metaData[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_FABRIC;
		fabricItem4->metaData[META_DATA_KEY_1_SERVICE_DIVISION] = SD_CLO;
		fabricItem4->metaData[META_DATA_KEY_2_BRAND] = BR_CLO_LUXURY;
		fabricItem4->metaData[META_DATA_KEY_3_PRODUCT_TYPE] = PT_FABRIC_MATTE;
		fabricItem4->metaData[META_DATA_KEY_4_SEASON_CREATED] = SEASON_SPRING_SUMMER_20;
		fabricItem4->sampleItemData.itemPath = SAMPLE_STORAGE_DIRECTORY + "fabric/" + "Cotton_14_Wale_Corduroy.zfab";
		fabricItem4->sampleItemData.iconThumbnailPath = SAMPLE_STORAGE_DIRECTORY + "fabric/" + "Cotton_14_Wale_Corduroy.zfab";    // thumbnail inside zfab
		fabricItem4->sampleItemData.previewThumbnailPath = SAMPLE_STORAGE_DIRECTORY + "fabric/" + "Cotton_14_Wale_Corduroy.zfab"; // thumbnail inside zfab
		m_LibraryAPIItemList.push_back(fabricItem4);

		// fabricItem5
		LibraryAPIItem* fabricItem5 = newLibraryAPIItemWithRandomizedDate();
		fabricItem5->itemID = "Cotton_Gabardine";
		fabricItem5->itemName = "Cotton_Gabardine.zfab";
		fabricItem5->metaData[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_FABRIC;
		fabricItem5->metaData[META_DATA_KEY_1_SERVICE_DIVISION] = SD_CLO;
		fabricItem5->metaData[META_DATA_KEY_2_BRAND] = BR_CLO_LUXURY;
		fabricItem5->metaData[META_DATA_KEY_3_PRODUCT_TYPE] = PT_FABRIC_MATTE;
		fabricItem5->metaData[META_DATA_KEY_4_SEASON_CREATED] = SEASON_SPRING_SUMMER_20;
		fabricItem5->sampleItemData.itemPath = SAMPLE_STORAGE_DIRECTORY + "fabric/" + "Cotton_Gabardine.zfab";
		fabricItem5->sampleItemData.iconThumbnailPath = SAMPLE_STORAGE_DIRECTORY + "fabric/" + "Cotton_Gabardine.zfab";    // thumbnail inside zfab
		fabricItem5->sampleItemData.previewThumbnailPath = SAMPLE_STORAGE_DIRECTORY + "fabric/" + "Cotton_Gabardine.zfab"; // thumbnail inside zfab
		m_LibraryAPIItemList.push_back(fabricItem5);
	}

	void APIStorage::generateAPIItemListForProjectFiles()
	{
		/// project file
		// projectItem1
		LibraryAPIItem* projectItem1 = newLibraryAPIItemWithRandomizedDate();
		projectItem1->itemID = "sample_zprj";
		projectItem1->itemName = "sample_zprj.zprj";
		//projectItem1->dateTime = "2019-10-20T16:17:11";
		projectItem1->metaData[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_PROJECT;
		projectItem1->metaData[META_DATA_KEY_1_SERVICE_DIVISION] = SD_CLO;
		projectItem1->metaData[META_DATA_KEY_2_BRAND] = BR_CLO_LUXURY;
		projectItem1->metaData[META_DATA_KEY_3_PRODUCT_TYPE] = PT_DRESSES;
		projectItem1->metaData[META_DATA_KEY_4_SEASON_CREATED] = SEASON_SPRING_SUMMER_20;
		projectItem1->sampleItemData.itemPath = "C:/sample_assets/project/sample_zprj.zprj";
		projectItem1->sampleItemData.iconThumbnailPath = "C:/sample_assets/project/sample_zprj.zprj"; // thumbnail inside zprj
		projectItem1->sampleItemData.previewThumbnailPath = "C:/sample_assets/project/sample_zprj.zprj"; // thumbnail inside zprj
		m_LibraryAPIItemList.push_back(projectItem1);
	}

	void APIStorage::generateAPIItemListForAvatars()
	{
		/// avatars
		// avatarItem1
		LibraryAPIItem* avatarItem1 = newLibraryAPIItemWithRandomizedDate();
		avatarItem1->itemID = "FV1_Kelly";
		avatarItem1->itemName = "FV1_Kelly.avt";
		//avatarItem1->dateTime = "2019-11-12T15:20:08";
		avatarItem1->metaData[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_AVATAR;
		avatarItem1->metaData[META_DATA_KEY_1_SERVICE_DIVISION] = SD_CLO;
		avatarItem1->metaData[META_DATA_KEY_2_BRAND] = BR_CLO_LUXURY;
		avatarItem1->metaData[META_DATA_KEY_4_SEASON_CREATED] = SEASON_SPRING_SUMMER_20;
		avatarItem1->sampleItemData.itemPath = "FV1_Kelly.avt";
		avatarItem1->sampleItemData.iconThumbnailPath = "FV1_Kelly.jpg";
		avatarItem1->sampleItemData.previewThumbnailPath = "FV1_Kelly.jpg";
		m_LibraryAPIItemList.push_back(avatarItem1);

		// avatarItem2
		LibraryAPIItem* avatarItem2 = newLibraryAPIItemWithRandomizedDate();
		avatarItem2->itemID = "FV1_Emma";
		avatarItem2->itemName = "FV1_Emma.avt";
		//avatarItem2->dateTime = "2019-11-12T15:20:08";
		avatarItem2->metaData[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_AVATAR;
		avatarItem2->metaData[META_DATA_KEY_1_SERVICE_DIVISION] = SD_CLO;
		avatarItem2->metaData[META_DATA_KEY_2_BRAND] = BR_CLO_LUXURY;
		avatarItem2->metaData[META_DATA_KEY_4_SEASON_CREATED] = SEASON_SPRING_SUMMER_20;
		avatarItem2->sampleItemData.itemPath = "FV1_Emma.avt";
		avatarItem2->sampleItemData.iconThumbnailPath = "FV1_Emma.jpg";
		avatarItem2->sampleItemData.previewThumbnailPath = "FV1_Emma.jpg";
		m_LibraryAPIItemList.push_back(avatarItem2);
	}

	void APIStorage::generateAPIItemListForGarments()
	{
		/// garment
		// garmentItem2
		LibraryAPIItem* garmentItem2 = newLibraryAPIItemWithRandomizedDate();
		garmentItem2->itemID = "sample_garment";
		garmentItem2->itemName = "sample_garment.zpac";
		//garmentItem2->dateTime = "2019-11-08T13:10:48";
		garmentItem2->metaData[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_GARMENT;
		garmentItem2->metaData[META_DATA_KEY_1_SERVICE_DIVISION] = SD_MD;
		garmentItem2->metaData[META_DATA_KEY_2_BRAND] = BR_MD_MOVIEW;
		garmentItem2->metaData[META_DATA_KEY_3_PRODUCT_TYPE] = PT_DRESSES;
		garmentItem2->metaData[META_DATA_KEY_4_SEASON_CREATED] = SEASON_SPRING_SUMMER_20;
		garmentItem2->sampleItemData.itemPath = "sample_garment.zpac";
		garmentItem2->sampleItemData.iconThumbnailPath = "sample_garment.png";
		garmentItem2->sampleItemData.previewThumbnailPath = "sample_garment.zpac"; // thumbnail inside zpac
		m_LibraryAPIItemList.push_back(garmentItem2);
	}

	void APIStorage::generateAPIItemListForTopstitches()
	{
		/// topstitch
		// topstitchItem1
		LibraryAPIItem* topstitchItem1 = newLibraryAPIItemWithRandomizedDate();
		topstitchItem1->itemID = "ISO_101_Single_Thread_Chainstitch";
		topstitchItem1->itemName = "ISO_101_Single_Thread_Chainstitch.sst";
		//topstitchItem1->dateTime = "2019-11-08T13:10:48";
		topstitchItem1->metaData[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_TOPSTITCH;
		topstitchItem1->metaData[META_DATA_KEY_1_SERVICE_DIVISION] = SD_MD;
		topstitchItem1->metaData[META_DATA_KEY_2_BRAND] = BR_MD_MOVIEW;
		topstitchItem1->metaData[META_DATA_KEY_4_SEASON_CREATED] = SEASON_SPRING_SUMMER_20;
		topstitchItem1->sampleItemData.itemPath = "ISO_101_Single_Thread_Chainstitch.sst";
		topstitchItem1->sampleItemData.iconThumbnailPath = "ISO_101_Single_Thread_Chainstitch.sst";
		topstitchItem1->sampleItemData.previewThumbnailPath = "ISO_101_Single_Thread_Chainstitch.sst";
		m_LibraryAPIItemList.push_back(topstitchItem1);

		// topstitchItem2
		LibraryAPIItem* topstitchItem2 = newLibraryAPIItemWithRandomizedDate();
		topstitchItem2->itemID = "ISO_103_Single_Thread_Bindstitch";
		topstitchItem2->itemName = "ISO_103_Single_Thread_Bindstitch.sst";
		//topstitchItem2->dateTime = "2019-11-08T13:10:48";
		topstitchItem2->metaData[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_TOPSTITCH;
		topstitchItem2->metaData[META_DATA_KEY_1_SERVICE_DIVISION] = SD_MD;
		topstitchItem2->metaData[META_DATA_KEY_2_BRAND] = BR_MD_MOVIEW;
		topstitchItem2->metaData[META_DATA_KEY_4_SEASON_CREATED] = SEASON_SPRING_SUMMER_20;
		topstitchItem2->sampleItemData.itemPath = "ISO_103_Single_Thread_Bindstitch.sst";
		topstitchItem2->sampleItemData.iconThumbnailPath = "ISO_103_Single_Thread_Bindstitch.sst";
		topstitchItem2->sampleItemData.previewThumbnailPath = "ISO_103_Single_Thread_Bindstitch.sst";
		m_LibraryAPIItemList.push_back(topstitchItem2);
	}

	void APIStorage::generateAPIItemListForButtonHeads()
	{
		/// button head
		// buttonHeadItem1
		LibraryAPIItem* buttonHeadItem1 = newLibraryAPIItemWithRandomizedDate();
		buttonHeadItem1->itemID = "1_Sew-through_Button_03_Parallel";
		buttonHeadItem1->itemName = "1_Sew-through_Button_03_Parallel.btn";
		//buttonHeadItem1->dateTime = "2019-11-08T13:10:48";
		buttonHeadItem1->metaData[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_BUTTONHEAD;
		buttonHeadItem1->metaData[META_DATA_KEY_1_SERVICE_DIVISION] = SD_CLO;
		buttonHeadItem1->metaData[META_DATA_KEY_2_BRAND] = BR_CLO_BASIC;
		buttonHeadItem1->metaData[META_DATA_KEY_4_SEASON_CREATED] = SEASON_SPRING_SUMMER_20;
		buttonHeadItem1->sampleItemData.itemPath = "1_Sew-through_Button_03_Parallel.btn";
		buttonHeadItem1->sampleItemData.iconThumbnailPath = "1_Sew-through_Button_03_Parallel.btn";
		buttonHeadItem1->sampleItemData.previewThumbnailPath = "1_Sew-through_Button_03_Parallel.btn";
		m_LibraryAPIItemList.push_back(buttonHeadItem1);
	}

	void APIStorage::generateAPIItemListForButtonHoles()
	{
		/// button hole
		// buttonHoleItem1
		LibraryAPIItem* buttonHoleItem1 = newLibraryAPIItemWithRandomizedDate();
		buttonHoleItem1->itemID = "Buttonhole_1_Eyelet_Taper_Bartacking";
		buttonHoleItem1->itemName = "Buttonhole_1_Eyelet_Taper_Bartacking.bth";
		//buttonHoleItem1->dateTime = "2019-11-08T13:10:48";
		buttonHoleItem1->metaData[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_BUTTONHOLE;
		buttonHoleItem1->metaData[META_DATA_KEY_1_SERVICE_DIVISION] = SD_CLO;
		buttonHoleItem1->metaData[META_DATA_KEY_2_BRAND] = BR_CLO_BASIC;
		buttonHoleItem1->metaData[META_DATA_KEY_4_SEASON_CREATED] = SEASON_SPRING_SUMMER_20;
		buttonHoleItem1->sampleItemData.itemPath = "Buttonhole_1_Eyelet_Taper_Bartacking.bth";
		buttonHoleItem1->sampleItemData.iconThumbnailPath = "Buttonhole_1_Eyelet_Taper_Bartacking.bth";
		buttonHoleItem1->sampleItemData.previewThumbnailPath = "Buttonhole_1_Eyelet_Taper_Bartacking.bth";
		m_LibraryAPIItemList.push_back(buttonHoleItem1);
	}

	void APIStorage::generateAPIItemListForZipperPulers()
	{
		// generate many items to show Scrolling Page sample

		/// zipperPuller
		for (int i = 1; i <= 9; ++i)
		{
			LibraryAPIItem* zipperPullerItem = newLibraryAPIItemWithRandomizedDate();
			zipperPullerItem->itemID = "Puller_0" + QString::number(i);
			zipperPullerItem->itemName = "Puller_0" + QString::number(i) + ".zpl";
			//zipperPullerItem->dateTime = "2019-11-01T13:10:48";
			zipperPullerItem->metaData[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_ZIPPER_PULLER;
			zipperPullerItem->metaData[META_DATA_KEY_1_SERVICE_DIVISION] = SD_CLO;
			zipperPullerItem->metaData[META_DATA_KEY_2_BRAND] = BR_CLO_BASIC;
			zipperPullerItem->metaData[META_DATA_KEY_4_SEASON_CREATED] = SEASON_SPRING_SUMMER_20;
			zipperPullerItem->sampleItemData.itemPath = "Puller_0" + QString::number(i) + ".zpl";
			zipperPullerItem->sampleItemData.iconThumbnailPath = "";
			zipperPullerItem->sampleItemData.previewThumbnailPath = "";
			m_LibraryAPIItemList.push_back(zipperPullerItem);
		}

		for (int i = 10; i <= 80; ++i)
		{
			LibraryAPIItem* zipperPullerItem = newLibraryAPIItemWithRandomizedDate();
			zipperPullerItem->itemID = "Puller_" + QString::number(i);
			zipperPullerItem->itemName = "Puller_" + QString::number(i) + ".zpl";
			//zipperPullerItem->dateTime = "2019-11-01T13:10:48";
			zipperPullerItem->metaData[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_ZIPPER_PULLER;
			zipperPullerItem->metaData[META_DATA_KEY_1_SERVICE_DIVISION] = SD_CLO;
			zipperPullerItem->metaData[META_DATA_KEY_2_BRAND] = BR_CLO_BASIC;
			zipperPullerItem->metaData[META_DATA_KEY_4_SEASON_CREATED] = SEASON_SPRING_SUMMER_20;
			zipperPullerItem->sampleItemData.itemPath = "Puller_" + QString::number(i) + ".zpl";
			zipperPullerItem->sampleItemData.iconThumbnailPath = "";
			zipperPullerItem->sampleItemData.previewThumbnailPath = "";
			m_LibraryAPIItemList.push_back(zipperPullerItem);
		}
	}

	void APIStorage::generateAPIItemListForZipperSliders()
	{
		// zipperSliderItem1
		LibraryAPIItem* zipperSliderItem1 = newLibraryAPIItemWithRandomizedDate();
		zipperSliderItem1->itemID = "Slider_01";
		zipperSliderItem1->itemName = "Slider_01.zsd";
		//zipperSliderItem1->dateTime = "2019-11-01T13:10:48";
		zipperSliderItem1->metaData[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_ZIPPER_SLIDER;
		zipperSliderItem1->metaData[META_DATA_KEY_1_SERVICE_DIVISION] = SD_CLO;
		zipperSliderItem1->metaData[META_DATA_KEY_2_BRAND] = BR_CLO_BASIC;
		zipperSliderItem1->metaData[META_DATA_KEY_4_SEASON_CREATED] = SEASON_SPRING_SUMMER_20;
		zipperSliderItem1->sampleItemData.itemPath = "Slider_01.zsd";
		zipperSliderItem1->sampleItemData.iconThumbnailPath = "";
		zipperSliderItem1->sampleItemData.previewThumbnailPath = "";
		m_LibraryAPIItemList.push_back(zipperSliderItem1);

		// zipperSliderItem2
		LibraryAPIItem* zipperSliderItem2 = newLibraryAPIItemWithRandomizedDate();
		zipperSliderItem2->itemID = "Slider_02";
		zipperSliderItem2->itemName = "Slider_02.zsd";
		//zipperSliderItem2->dateTime = "2019-11-02T13:10:48";
		zipperSliderItem2->metaData[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_ZIPPER_SLIDER;
		zipperSliderItem2->metaData[META_DATA_KEY_1_SERVICE_DIVISION] = SD_CLO;
		zipperSliderItem2->metaData[META_DATA_KEY_2_BRAND] = BR_CLO_BASIC;
		zipperSliderItem2->metaData[META_DATA_KEY_4_SEASON_CREATED] = SEASON_SPRING_SUMMER_20;
		zipperSliderItem2->sampleItemData.itemPath = "Slider_02.zsd";
		zipperSliderItem2->sampleItemData.iconThumbnailPath = "";
		zipperSliderItem2->sampleItemData.previewThumbnailPath = "";
		m_LibraryAPIItemList.push_back(zipperSliderItem2);

		// zipperSliderItem3
		LibraryAPIItem* zipperSliderItem3 = newLibraryAPIItemWithRandomizedDate();
		zipperSliderItem3->itemID = "Slider_03";
		zipperSliderItem3->itemName = "Slider_03.zsd";
		//zipperSliderItem3->dateTime = "2019-11-03T13:10:48";
		zipperSliderItem3->metaData[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_ZIPPER_SLIDER;
		zipperSliderItem3->metaData[META_DATA_KEY_1_SERVICE_DIVISION] = SD_CLO;
		zipperSliderItem3->metaData[META_DATA_KEY_2_BRAND] = BR_CLO_BASIC;
		zipperSliderItem3->metaData[META_DATA_KEY_4_SEASON_CREATED] = SEASON_SPRING_SUMMER_20;
		zipperSliderItem3->sampleItemData.itemPath = "Slider_03.zsd";
		zipperSliderItem3->sampleItemData.iconThumbnailPath = "";
		zipperSliderItem3->sampleItemData.previewThumbnailPath = "";
		m_LibraryAPIItemList.push_back(zipperSliderItem3);

		// zipperSliderItem4
		LibraryAPIItem* zipperSliderItem4 = newLibraryAPIItemWithRandomizedDate();
		zipperSliderItem4->itemID = "Slider_04";
		zipperSliderItem4->itemName = "Slider_04.zsd";
		//zipperSliderItem4->dateTime = "2019-11-04T13:10:48";
		zipperSliderItem4->metaData[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_ZIPPER_SLIDER;
		zipperSliderItem4->metaData[META_DATA_KEY_1_SERVICE_DIVISION] = SD_CLO;
		zipperSliderItem4->metaData[META_DATA_KEY_2_BRAND] = BR_CLO_BASIC;
		zipperSliderItem4->metaData[META_DATA_KEY_4_SEASON_CREATED] = SEASON_SPRING_SUMMER_20;
		zipperSliderItem4->sampleItemData.itemPath = "Slider_04.zsd";
		zipperSliderItem4->sampleItemData.iconThumbnailPath = "";
		zipperSliderItem4->sampleItemData.previewThumbnailPath = "";
		m_LibraryAPIItemList.push_back(zipperSliderItem4);

		// zipperSliderItem5
		LibraryAPIItem* zipperSliderItem5 = newLibraryAPIItemWithRandomizedDate();
		zipperSliderItem5->itemID = "Slider_05";
		zipperSliderItem5->itemName = "Slider_05.zsd";
		//zipperSliderItem5->dateTime = "2019-11-05T13:10:48";
		zipperSliderItem5->metaData[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_ZIPPER_SLIDER;
		zipperSliderItem5->metaData[META_DATA_KEY_1_SERVICE_DIVISION] = SD_CLO;
		zipperSliderItem5->metaData[META_DATA_KEY_2_BRAND] = BR_CLO_BASIC;
		zipperSliderItem5->metaData[META_DATA_KEY_4_SEASON_CREATED] = SEASON_SPRING_SUMMER_20;
		zipperSliderItem5->sampleItemData.itemPath = "Slider_05.zsd";
		zipperSliderItem5->sampleItemData.iconThumbnailPath = "";
		zipperSliderItem5->sampleItemData.previewThumbnailPath = "";
		m_LibraryAPIItemList.push_back(zipperSliderItem5);

		// zipperSliderItem6
		LibraryAPIItem* zipperSliderItem6 = newLibraryAPIItemWithRandomizedDate();
		zipperSliderItem6->itemID = "Slider_06";
		zipperSliderItem6->itemName = "Slider_06.zsd";
		//zipperSliderItem6->dateTime = "2019-11-06T13:10:48";
		zipperSliderItem6->metaData[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_ZIPPER_SLIDER;
		zipperSliderItem6->metaData[META_DATA_KEY_1_SERVICE_DIVISION] = SD_CLO;
		zipperSliderItem6->metaData[META_DATA_KEY_2_BRAND] = BR_CLO_BASIC;
		zipperSliderItem6->metaData[META_DATA_KEY_4_SEASON_CREATED] = SEASON_SPRING_SUMMER_20;
		zipperSliderItem6->sampleItemData.itemPath = "Slider_06.zsd";
		zipperSliderItem6->sampleItemData.iconThumbnailPath = "";
		zipperSliderItem6->sampleItemData.previewThumbnailPath = "";
		m_LibraryAPIItemList.push_back(zipperSliderItem6);

		// zipperSliderItem7
		LibraryAPIItem* zipperSliderItem7 = newLibraryAPIItemWithRandomizedDate();
		zipperSliderItem7->itemID = "Slider_07";
		zipperSliderItem7->itemName = "Slider_07.zsd";
		//zipperSliderItem7->dateTime = "2019-11-07T13:10:48";
		zipperSliderItem7->metaData[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_ZIPPER_SLIDER;
		zipperSliderItem7->metaData[META_DATA_KEY_1_SERVICE_DIVISION] = SD_CLO;
		zipperSliderItem7->metaData[META_DATA_KEY_2_BRAND] = BR_CLO_BASIC;
		zipperSliderItem7->metaData[META_DATA_KEY_4_SEASON_CREATED] = SEASON_SPRING_SUMMER_20;
		zipperSliderItem7->sampleItemData.itemPath = "Slider_07.zsd";
		zipperSliderItem7->sampleItemData.iconThumbnailPath = "";
		zipperSliderItem7->sampleItemData.previewThumbnailPath = "";
		m_LibraryAPIItemList.push_back(zipperSliderItem7);

		// zipperSliderItem8
		LibraryAPIItem* zipperSliderItem8 = newLibraryAPIItemWithRandomizedDate();
		zipperSliderItem8->itemID = "Slider_08";
		zipperSliderItem8->itemName = "Slider_08.zsd";
		//zipperSliderItem8->dateTime = "2019-11-08T13:10:48";
		zipperSliderItem8->metaData[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_ZIPPER_SLIDER;
		zipperSliderItem8->metaData[META_DATA_KEY_1_SERVICE_DIVISION] = SD_CLO;
		zipperSliderItem8->metaData[META_DATA_KEY_2_BRAND] = BR_CLO_BASIC;
		zipperSliderItem8->metaData[META_DATA_KEY_4_SEASON_CREATED] = SEASON_SPRING_SUMMER_20;
		zipperSliderItem8->sampleItemData.itemPath = "Slider_08.zsd";
		zipperSliderItem8->sampleItemData.iconThumbnailPath = "";
		zipperSliderItem8->sampleItemData.previewThumbnailPath = "";
		m_LibraryAPIItemList.push_back(zipperSliderItem8);

		// zipperSliderItem9
		LibraryAPIItem* zipperSliderItem9 = newLibraryAPIItemWithRandomizedDate();
		zipperSliderItem9->itemID = "Slider_09";
		zipperSliderItem9->itemName = "Slider_09.zsd";
		//zipperSliderItem9->dateTime = "2019-11-09T13:10:48";
		zipperSliderItem9->metaData[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_ZIPPER_SLIDER;
		zipperSliderItem9->metaData[META_DATA_KEY_1_SERVICE_DIVISION] = SD_CLO;
		zipperSliderItem9->metaData[META_DATA_KEY_2_BRAND] = BR_CLO_BASIC;
		zipperSliderItem9->metaData[META_DATA_KEY_4_SEASON_CREATED] = SEASON_SPRING_SUMMER_20;
		zipperSliderItem9->sampleItemData.itemPath = "Slider_09.zsd";
		zipperSliderItem9->sampleItemData.iconThumbnailPath = "";
		zipperSliderItem9->sampleItemData.previewThumbnailPath = "";
		m_LibraryAPIItemList.push_back(zipperSliderItem9);
	}

	void APIStorage::generateAPIItemListForZipperTopStoppers()
	{
		// zipperTopStopperItem1
		LibraryAPIItem* zipperTopStopperItem1 = newLibraryAPIItemWithRandomizedDate();
		zipperTopStopperItem1->itemID = "StopperTop_01";
		zipperTopStopperItem1->itemName = "StopperTop_01.zsp";
		//zipperTopStopperItem1->dateTime = "2019-11-01T13:10:48";
		zipperTopStopperItem1->metaData[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_ZIPPER_TOP_STOPPER;
		zipperTopStopperItem1->metaData[META_DATA_KEY_1_SERVICE_DIVISION] = SD_CLO;
		zipperTopStopperItem1->metaData[META_DATA_KEY_2_BRAND] = BR_CLO_BASIC;
		zipperTopStopperItem1->metaData[META_DATA_KEY_4_SEASON_CREATED] = SEASON_SPRING_SUMMER_20;
		zipperTopStopperItem1->sampleItemData.itemPath = "StopperTop_01.zsp";
		zipperTopStopperItem1->sampleItemData.iconThumbnailPath = "";
		zipperTopStopperItem1->sampleItemData.previewThumbnailPath = "";
		m_LibraryAPIItemList.push_back(zipperTopStopperItem1);
	}

	void APIStorage::generateAPIItemListForZipperBottomStoppers()
	{
		// zipperBottomStopperItem1
		LibraryAPIItem* zipperBottomStopperItem1 = newLibraryAPIItemWithRandomizedDate();
		zipperBottomStopperItem1->itemID = "Open_StopperBottom_01";
		zipperBottomStopperItem1->itemName = "Open_StopperBottom_01.zsp";
		//zipperBottomStopperItem1->dateTime = "2019-11-01T13:10:48";
		zipperBottomStopperItem1->metaData[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_ZIPPER_BOTTOM_STOPPER;
		zipperBottomStopperItem1->metaData[META_DATA_KEY_1_SERVICE_DIVISION] = SD_CLO;
		zipperBottomStopperItem1->metaData[META_DATA_KEY_2_BRAND] = BR_CLO_BASIC;
		zipperBottomStopperItem1->metaData[META_DATA_KEY_4_SEASON_CREATED] = SEASON_SPRING_SUMMER_20;
		zipperBottomStopperItem1->sampleItemData.itemPath = "Open_StopperBottom_01.zsp";
		zipperBottomStopperItem1->sampleItemData.iconThumbnailPath = "";
		zipperBottomStopperItem1->sampleItemData.previewThumbnailPath = "";
		m_LibraryAPIItemList.push_back(zipperBottomStopperItem1);

		// zipperBottomStopperItem2
		LibraryAPIItem* zipperBottomStopperItem2 = newLibraryAPIItemWithRandomizedDate();
		zipperBottomStopperItem2->itemID = "Closed_StopperBottom_01";
		zipperBottomStopperItem2->itemName = "Closed_StopperBottom_01.zsp";
		//zipperBottomStopperItem2->dateTime = "2019-11-01T13:10:48";
		zipperBottomStopperItem2->metaData[META_DATA_KEY_0_DATA_TYPE] = ITEM_ID_ZIPPER_BOTTOM_STOPPER;
		zipperBottomStopperItem2->metaData[META_DATA_KEY_1_SERVICE_DIVISION] = SD_CLO;
		zipperBottomStopperItem2->metaData[META_DATA_KEY_2_BRAND] = BR_CLO_BASIC;
		zipperBottomStopperItem2->metaData[META_DATA_KEY_4_SEASON_CREATED] = SEASON_SPRING_SUMMER_20;
		zipperBottomStopperItem2->sampleItemData.itemPath = "Closed_StopperBottom_01.zsp";
		zipperBottomStopperItem2->sampleItemData.iconThumbnailPath = "";
		zipperBottomStopperItem2->sampleItemData.previewThumbnailPath = "";
		m_LibraryAPIItemList.push_back(zipperBottomStopperItem2);
	}

	LibraryAPIItem* APIStorage::newLibraryAPIItemWithRandomizedDate()
	{
		LibraryAPIItem* newItem = new LibraryAPIItem();
		newItem->dateTime = GetRandomDateTimer();
		return newItem;
	}
}