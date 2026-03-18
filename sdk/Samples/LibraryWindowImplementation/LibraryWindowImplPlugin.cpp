#include "stdafx.h"
#include "LibraryWindowImplPlugin.h"

#include <QFileInfo>
#include <QChar>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QtAlgorithms>

#include "classes/APIDefine.h"
#include "classes/APIStorage.h"
#include "classes/APIUtility.h"

#include "CLOAPIInterface.h"

// CLO API - Qt 4.8.7 dialog samples by Joshua
#include "classes/APISignInDialogSample.h"
#include "classes/APISearchDialogSample.h"
#include "classes/PLMSettingsSample.h"

// customizing functions
#define USE_LIBRARY_WINDOW_CUSTOM_TAB  0
#define USE_CUSTOM_TAB_AS_DEFAULT	   0
#define USE_PLM_SETTINGS			   0
#define USE_DOFUNCTION				   0
#define USE_STARTUP_DOFUNCTION		   0
#define USE_AFTER_FILE_LOAD_DOFUNCTION 0
#define USE_CUSTOM_TAB_ICON	 0

using namespace std;

namespace CLOAPI
{
	LibraryWindowImplPlugin::LibraryWindowImplPlugin()
	{
		// prepare api storage instance
		API_STORAGE->GenerateAPIItemListForSample();
	}

	LibraryWindowImplPlugin::~LibraryWindowImplPlugin()
	{
		// release api storage instance
		DESTROY_API_STORAGE;
	}

	bool LibraryWindowImplPlugin::EnableCustomUI()
	{
#if USE_LIBRARY_WINDOW_CUSTOM_TAB 
		return true;
#else
		return false;
#endif
	}

	bool LibraryWindowImplPlugin::IsDefaultTab()
	{
#if USE_CUSTOM_TAB_AS_DEFAULT
		return true;
#else
		return false;
#endif
	}

	string LibraryWindowImplPlugin::GetTabName()
	{
		string str = "Sample";
		return str;
	}

	string LibraryWindowImplPlugin::GetItemList(const string& itemId, unsigned int pageNo, unsigned int pageSize, CLO_FINDER_SORT sort, bool bAscending, const string& searchText)
	{
		if (CLOAPISample::APISignIn::getInstance()->GetSignIn() == false)
			return "";

		vector<CLOAPISample::LibraryAPIItem> itemList;
		vector<CLOAPISample::LibraryAPIItem> itemListForThePage;

		// search qvariantmap
		QVariantMap searchKeyValue;
		searchKeyValue[META_DATA_KEY_0_DATA_TYPE] = QString(itemId.c_str());

		if (API_STORAGE)
			itemList = API_STORAGE->GetSearchList("", searchKeyValue);

		// sort item list with ascending / descending 
		// - write code (below is the sample; in general PLM server should return the list as sorted)
		if (sort == NAME)
		{
			if (bAscending)
			{
				// use the list as-is; the sample list is sorted in Acscending in default
			}
			else
			{
				// reverse the list; the sample list is sorted in Acscending in default
				vector<CLOAPISample::LibraryAPIItem> sortedItemList;
				unsigned int listSize = (unsigned int)itemList.size();
				for (unsigned int i = 0; i < listSize; ++i)
					sortedItemList.push_back(itemList.at((listSize - 1) -i));

				itemList.clear();
				itemList = sortedItemList;
			}
		}
		else // DATE_TIME
		{
			unsigned int listSize = (unsigned int)itemList.size();

			if (bAscending)
			{
				for(size_t i=0;i< listSize - 1;++i)
					for (size_t j = i + 1; j < listSize; ++j)
					{
						if (itemList.at(i).dateTime > itemList.at(j).dateTime)
						{
							CLOAPISample::LibraryAPIItem tempItem = itemList[j];
							itemList[j] = itemList[i];
							itemList[i] = tempItem;
						}
					}
			}
			else
			{
				for (size_t i = 0; i < listSize - 1; ++i)
					for (size_t j = i + 1; j < listSize; ++j)
					{
						if (itemList.at(i).dateTime < itemList.at(j).dateTime)
						{
							CLOAPISample::LibraryAPIItem tempItem = itemList[j];
							itemList[j] = itemList[i];
							itemList[i] = tempItem;
						}
					}
			}
		}

		// get item list with pageNo / pageSize considered 
		// - write code (below is the sample)
		if (itemList.size() > 0)
		{
			for (int i = (int)pageNo - 1; i < (int)pageNo; ++i)
				for (int j = 0; j < (int)pageSize; ++j)
				{
					int curIndex = i * (int)pageSize + j;

					if (curIndex >= (int)itemList.size() || curIndex < 0)
						continue;

					itemListForThePage.push_back(itemList.at(curIndex));
				}
		}

		// get/return json string
		QString jsonStr = generateJSONFromLibraryAPIItemList(itemListForThePage);
		string result = jsonStr.toStdString();

		return result;
	}

	string LibraryWindowImplPlugin::GetSearchItemList(unsigned int pageNo, unsigned int pageSize, CLO_FINDER_SORT sort, bool bAscending, const string& searchText)
	{
		string result = "";
		return result;
	}

	bool LibraryWindowImplPlugin::GetSearchItemList(const string& itemId, unsigned int pageNo, unsigned int pageSize, CLO_FINDER_SORT sort, bool bAscending, string& resultItemList)
	{
		if (CLOAPISample::APISignIn::getInstance()->GetSignIn() == false)
			return false;

		string result = "";		

		CLOAPISample::APISearchDialogSample searchDlg;
		searchDlg.SetSearchModeByItemId(itemId);

		if (searchDlg.exec())
		{			
			if (searchDlg.result() == QDialog::Accepted)
			{
				resultItemList = searchDlg.GetSearchResult();
				return true;
			}				
		}

		return false;
	}


	bool LibraryWindowImplPlugin::GetParentFolderID(const string& itemId, string& parentFolderID)
	{
		if (API_STORAGE)
			parentFolderID = API_STORAGE->GetParentFolderID(QString::fromStdString(itemId)).toStdString();

		return !parentFolderID.empty();
	}

	CLO_BYTE* LibraryWindowImplPlugin::GetPNGThumbnail(const string& itemId, unsigned int& sizeInByte)
	{
		if (CLOAPISample::APISignIn::getInstance()->GetSignIn() == false)
			return nullptr;

		CLOAPISample::LibraryAPIItem item;
		
		if (API_STORAGE)
		{
			QString filePath;
			if (API_STORAGE->GetIconThumbnailPathWithID(itemId.c_str(), filePath))
			{				
				if (UTILITY_API->IsCLOFileFormatWithThumbnail(filePath.toStdString()))						// for CLO file format which contains thumbnails
				{
					return UTILITY_API->GetAssetIconInCLOFile(filePath.toStdString(), sizeInByte);	
				}
				else if (UTILITY_API->IsReadableImageFormat(filePath.toStdString()))						// for common type of images which can be displayed in Library Windows, CLO software
				{
					return FileLoadViaString(filePath, sizeInByte);
				}					
			}
		}

		return NULL;
	}

	string LibraryWindowImplPlugin::GetMetaData(const string& itemId)
	{
		string result = "";
		return result;
	}

	vector<string> LibraryWindowImplPlugin::GetMetaDataKeyListForUI(const string & itemId)
	{
		vector<string> strList;
		
		//strList.push_back(META_DATA_KEY_0_DATA_TYPE.toStdString()); // do not show in list view
		//strList.push_back(META_DATA_KEY_1_SERVICE_DIVISION.toStdString()); // do not show in list view
		strList.push_back(META_DATA_KEY_2_BRAND.toStdString()); // show brand in list view
		strList.push_back(META_DATA_KEY_3_PRODUCT_TYPE.toStdString()); // show product type in list view
		//strList.push_back(META_DATA_KEY_4_SEASON_CREATED.toStdString()); // do not show in list view

		return strList;
	}

	CLO_BYTE* LibraryWindowImplPlugin::GetPreviewImage(const string& itemId, unsigned int& sizeInByte)
	{
		if (CLOAPISample::APISignIn::getInstance()->GetSignIn() == false)
			return nullptr;

		CLOAPISample::LibraryAPIItem item;

		if (API_STORAGE)
		{
			QString filePath;
			if (API_STORAGE->GetPreviewThumbnailPathWithID(itemId.c_str(), filePath))
			{				
				if (UTILITY_API->IsCLOFileFormatWithThumbnail(filePath.toStdString()))						// for CLO file format which contains thumbnails
				{
					if (UTILITY_API->IsCLOFileFormatWithTripleThumbnail(filePath.toStdString()))			// for CLO file format which contains 3 preview images (index: 0 = front thumbnail, 1 = back thumbnail)
					{
						return UTILITY_API->GetThumbnailInCLOFile(filePath.toStdString(), 0, sizeInByte);	// index: 0 = front thumbnail, 1 = back thumbnail
					}
					else
					{
						// zfab and sst files are unusual case
						if (UTILITY_API && filePath.contains(".zfab", Qt::CaseInsensitive))
							return UTILITY_API->GetThumbnailInCLOFile(filePath.toStdString(), 0, sizeInByte);
						else if (UTILITY_API && filePath.contains(".sst", Qt::CaseInsensitive))
							return UTILITY_API->GetThumbnailInCLOFile(filePath.toStdString(), 0, sizeInByte); // index: 0 = front thumbnail, 1 = back thumbnail
						else
							return UTILITY_API->GetThumbnailInCLOFile(filePath.toStdString(), 0, sizeInByte); // for CLO file format which contains only one thumnail 
					}
				}
				else if (UTILITY_API->IsReadableImageFormat(filePath.toStdString()))							// for common type of images which can be displayed in Library Windows, CLO software
				{
					return FileLoadViaString(filePath, sizeInByte);
				}
			}				
		}

		return NULL;
	}

	CLO_BYTE* LibraryWindowImplPlugin::GetFile(const string& itemId, unsigned int& sizeInByte)
	{
		if (CLOAPISample::APISignIn::getInstance()->GetSignIn() == false)
			return nullptr;

		CLOAPISample::LibraryAPIItem item;

		if (API_STORAGE)
		{
			QString filePath;
			if (API_STORAGE->GetFilePathWithID(itemId.c_str(), filePath))
				return FileLoadViaString(filePath, sizeInByte);
		}

		return NULL;
	}

	bool LibraryWindowImplPlugin::IsSigninEnabled()
	{
		return true; // show sigin-in dialog when the user clicks 'API' tab in the Library Window
	}

	bool LibraryWindowImplPlugin::Signin()
	{
		if (CLOAPISample::APISignIn::getInstance()->GetSignIn())
			return true;

		CLOAPISample::APISigninDialogSample* dlg = new CLOAPISample::APISigninDialogSample();
		dlg->exec();

		if (dlg->result() == QDialog::Accepted)
		{
			QString id;
			QString passwd;

			id = dlg->GetID();
			passwd = dlg->GetPasswd();

			CLOAPISample::APISignIn::getInstance()->SetSignIn(true);
		}
		else
		{
			CLOAPISample::APISignIn::getInstance()->SetSignIn(false);
		}

		return CLOAPISample::APISignIn::getInstance()->GetSignIn();
	}

	bool LibraryWindowImplPlugin::Signin(const string& id, const string& pw, string& token)
	{
		return false;
	}

	string LibraryWindowImplPlugin::GetFavoriteList()
	{
		vector<CLOAPISample::LibraryAPIItem> itemList;

		// pattern item
		CLOAPISample::LibraryAPIItem pattern;
		pattern.itemID = "PATTERN";
		pattern.itemType = "FOLDER";
		pattern.itemName = "Patterns";
		pattern.sampleItemData.itemPath = SAMPLE_STORAGE_DIRECTORY + pattern.itemID.toLower();
		itemList.push_back(pattern);

		// trim item
		CLOAPISample::LibraryAPIItem trim;
		trim.itemID = "TRIM";
		trim.itemType = "FOLDER";
		trim.itemName = "Trims";
		trim.sampleItemData.itemPath = SAMPLE_STORAGE_DIRECTORY + pattern.itemID.toLower();
		itemList.push_back(trim);

		// fabric
		CLOAPISample::LibraryAPIItem fabric;
		fabric.itemID = "FABRIC";
		fabric.itemType = "FOLDER";
		fabric.itemName = "Fabrics";
		fabric.sampleItemData.itemPath = SAMPLE_STORAGE_DIRECTORY + pattern.itemID.toLower();
		itemList.push_back(fabric);

		// project
		CLOAPISample::LibraryAPIItem project;
		project.itemID = "PROJECT";
		project.itemType = "FOLDER";
		project.itemName = "Projects";
		project.sampleItemData.itemPath = SAMPLE_STORAGE_DIRECTORY + pattern.itemID.toLower();
		itemList.push_back(project);

		// avatar
		CLOAPISample::LibraryAPIItem avatar;
		avatar.itemID = "AVATAR";
		avatar.itemType = "FOLDER";
		avatar.itemName = "Avatars";
		avatar.sampleItemData.itemPath = SAMPLE_STORAGE_DIRECTORY + pattern.itemID.toLower();
		itemList.push_back(avatar);

		// garment
		CLOAPISample::LibraryAPIItem garment;
		garment.itemID = "GARMENT";
		garment.itemType = "FOLDER";
		garment.itemName = "Garments";
		garment.sampleItemData.itemPath = SAMPLE_STORAGE_DIRECTORY + pattern.itemID.toLower();
		itemList.push_back(garment);

		// sst
		CLOAPISample::LibraryAPIItem topstitch;
		topstitch.itemID = "TOPSTITCH";
		topstitch.itemType = "FOLDER";
		topstitch.itemName = "Topstitches";
		topstitch.sampleItemData.itemPath = SAMPLE_STORAGE_DIRECTORY + pattern.itemID.toLower();
		itemList.push_back(topstitch);

		// buttonhead
		CLOAPISample::LibraryAPIItem btnHead;
		btnHead.itemID = "BUTTONHEAD";
		btnHead.itemType = "FOLDER";
		btnHead.itemName = "Buttonheads";
		btnHead.sampleItemData.itemPath = SAMPLE_STORAGE_DIRECTORY + pattern.itemID.toLower();
		itemList.push_back(btnHead);

		// buttonhole
		CLOAPISample::LibraryAPIItem btnHole;
		btnHole.itemID = "BUTTONHOLE";
		btnHole.itemType = "FOLDER";
		btnHole.itemName = "Buttonholes";
		btnHole.sampleItemData.itemPath = SAMPLE_STORAGE_DIRECTORY + pattern.itemID.toLower();
		itemList.push_back(btnHole);

		// zipper puller
		CLOAPISample::LibraryAPIItem zipperPuller;
		zipperPuller.itemID = "ZIPPER_PULLER";
		zipperPuller.itemType = "FOLDER";
		zipperPuller.itemName = "Zipper_Pullers";
		zipperPuller.sampleItemData.itemPath = SAMPLE_STORAGE_DIRECTORY + pattern.itemID.toLower();
		itemList.push_back(zipperPuller);
		
		// zipper slider
		CLOAPISample::LibraryAPIItem zipperSlider;
		zipperSlider.itemID = "ZIPPER_SLIDER";
		zipperSlider.itemType = "FOLDER";
		zipperSlider.itemName = "Zipper_Sliders";
		zipperSlider.sampleItemData.itemPath = SAMPLE_STORAGE_DIRECTORY + pattern.itemID.toLower();
		itemList.push_back(zipperSlider);
		
		// zipper top stopper
		CLOAPISample::LibraryAPIItem zipperTopStopper;
		zipperTopStopper.itemID = "ZIPPER_TOP_STOPPER";
		zipperTopStopper.itemType = "FOLDER";
		zipperTopStopper.itemName = "Zipper_Top_Stoppers";
		zipperTopStopper.sampleItemData.itemPath = SAMPLE_STORAGE_DIRECTORY + pattern.itemID.toLower();
		itemList.push_back(zipperTopStopper);
		
		// zipper bottom stopper
		CLOAPISample::LibraryAPIItem zipperBottomStopper;
		zipperBottomStopper.itemID = "ZIPPER_BOTTOM_STOPPER";
		zipperBottomStopper.itemType = "FOLDER";
		zipperBottomStopper.itemName = "Zipper_Bottom_Stoppers";
		zipperBottomStopper.sampleItemData.itemPath = SAMPLE_STORAGE_DIRECTORY + pattern.itemID.toLower();
		itemList.push_back(zipperBottomStopper);

		QString jsonStr = generateJSONFromLibraryAPIItemList(itemList);		
		string str = jsonStr.toStdString();

		return str;
	}

	CLOAPISample::PLMSettingsSample* currentSettingsWidget = nullptr;

	bool LibraryWindowImplPlugin::IsPLMSettingsEnabled()
	{
#if USE_PLM_SETTINGS
		return true;
#else
		return false;
#endif
	}

	void* LibraryWindowImplPlugin::GetPLMSettingsWidget()
	{					
		if (currentSettingsWidget)
			ReleasePLMSettingsWidget();

		currentSettingsWidget = new CLOAPISample::PLMSettingsSample();
		return  static_cast<void*>(currentSettingsWidget);
	}

	void LibraryWindowImplPlugin::ReleasePLMSettingsWidget()
	{
		if (currentSettingsWidget)
		{
			delete currentSettingsWidget;
			currentSettingsWidget = nullptr;
		}
	}

	void LibraryWindowImplPlugin::SetNullptrPLMSettingsWidget()
	{
		currentSettingsWidget = nullptr;
	}
	
	void LibraryWindowImplPlugin::ResetPLMSettingsWidget()
	{
		if (currentSettingsWidget)
			currentSettingsWidget->Reset();
	}

	/// plug-in related functions
	bool LibraryWindowImplPlugin::IsPluginEnabled()
	{
#if USE_DOFUNCTION
		return true;
#else
		return false;
#endif
	}

	void LibraryWindowImplPlugin::DoFunction()
	{
#if USE_DOFUNCTION
		UTILITY_API->DisplayMessageBox("Library windows sample plug-in");
#endif
	}

	void LibraryWindowImplPlugin::DoFunctionStartUp()
	{
#if USE_STARTUP_DOFUNCTION
		UTILITY_API->DisplayMessageBox("Library windows sample startup plug-in");
#endif
	}

	void LibraryWindowImplPlugin::DoFunctionAfterLoadingCLOFile(const char* fileExtenstion)
	{
#if USE_AFTER_FILE_LOAD_DOFUNCTION
		UTILITY_API->DisplayMessageBox("Library windows sample plug-in after loading clo file");
#endif
	}

	const char* LibraryWindowImplPlugin::GetActionName()
	{
		return "Library Windows Sample Plug-in";
	}

	const char* LibraryWindowImplPlugin::GetObjectNameTreeToAddAction()
	{
		const char* objetNameTree = "menu_Setting / menuPlug_In";

		return objetNameTree;
	}

	int LibraryWindowImplPlugin::GetPositionIndexToAddAction()
	{
		return 1; // 0: Above, 1: below (default = 0)
	}

	string LibraryWindowImplPlugin::GetIconOnFilePathForTab()
	{
		string filePath = "";

#if USE_CUSTOM_TAB_ICON
		if (UTILITY_API != nullptr)
			filePath = UTILITY_API->GetCLOExecutableFolderPath() + "library_tab_api_on.svg";	
#endif

		return filePath;
	}

	string LibraryWindowImplPlugin::GetIconOffFilePathForTab()
	{
		string filePath = "";

#if USE_CUSTOM_TAB_ICON
		if (UTILITY_API != nullptr)
			filePath = UTILITY_API->GetCLOExecutableFolderPath() + "library_tab_api_off.svg";
#endif

		return filePath;
	}
}



// the class factories
extern CLO_PLUGIN_SPECIFIER CLOAPI::LibraryWindowInterface* Create() {
	return new CLOAPI::LibraryWindowImplPlugin();
}