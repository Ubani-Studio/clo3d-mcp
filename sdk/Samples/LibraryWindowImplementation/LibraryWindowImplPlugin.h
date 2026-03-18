#ifndef _LIBRARY_API_IMPLMENTATION_H
#define _LIBRARY_API_IMPLMENTATION_H

// Header only class - should drop an implemented "LibraryAPIImpl.dll" made by plug-in developer into the CLO executable folder.

#ifdef LIBRARYAPIIMPLEMENTATION_LIB

#if defined(__APPLE__)
#define CLO_PLUGIN_SPECIFIER
#else
#define CLO_PLUGIN_SPECIFIER __declspec(dllexport)
#endif

#else

#if defined(__APPLE__)
#define CLO_PLUGIN_SPECIFIER
#else
#define CLO_PLUGIN_SPECIFIER __declspec(dllimport)
#endif

#endif

#include "LibraryWindowInterface.h"

#include <stdio.h>
#include <assert.h>
#include <fstream>
#include <stdlib.h>
#include <QWidget>

#if (!defined(NDEBUG) || defined(_DEBUG))
// debug
#if defined(__APPLE__)
#define uprintf(format,...) {fprintf(stderr,"[%s, %04d]",__FILE__,__LINE__);fprintf(stderr,format,##__VA_ARGS__);} // error
#else // windows
#define uprintf(...) {printf(__VA_ARGS__);}
#endif
#endif

namespace CLOAPI
{
	class CLO_PLUGIN_SPECIFIER LibraryWindowImplPlugin : public LibraryWindowInterface {
	public:
		LibraryWindowImplPlugin();
		~LibraryWindowImplPlugin();

		/*
		If the return value of this function is true, the UI is activated in the CLO Library.
		@return When implementing this dll, the return value should always be true.
		*/
		virtual bool EnableCustomUI();

		/*
		If the return value of this function is true, API tab will be the shown in the first tab as the default tab in Library Window, CLO
		@return When implementing this dll, the return value should always be true.
		*/
		virtual bool IsDefaultTab();

		/*
		@return the tab name for the API items onto the Library Window, CLO
		*/
		virtual std::string GetTabName();

		/*
		This function will be called either when CLO Library Window opens or when a user clicks a folder icon.
		It returns a item (file or folder) list under a folder. CLO will display the item list in its Library window.

		@param itemId The ID of the current folder. If itemId is "", it must return the item list of the root folder.
		@param pageNo Page number(default:1). pageNo will be given by user's scroll.
		@param pageSize The number of items to display in a page.
		@param sort
		@param bAscending
		@param searchText
		@return The item list in json format. Developers should implement this function so that it returns the item list corresponding to "pageNo" and "pageSize".

			itemID : Unique id
			type : FOLDER or FILE
			name : Item name
			dateTime : dateTime in ISO 8601 format (https://en.wikipedia.org/wiki/ISO_8601#Coordinated_Universal_Time_(UTC)
			fileSize : file size in bytes
			metaData : user field for 1-depth json format (key - value list)

			[
			{
			"itemID": "/Users/inoblaze/Documents/CLO Install",
			"type": "FOLDER",
			"name": "CLO Install",
			"dateTime": "2018-03-09T11:10:03",
			"metaData" : [
			  {
				"(key1)" : value1,
				"(key2)" : value2,
				...
			  }
			 ]
			}
			,
			{
			"itemID": "/Users/inoblaze/Documents/FLANNEL SHIRT-A.Zprj",
			"type": "FILE",
			"name": "FLANNEL SHIRT-A.Zprj",
			"dateTime": "2017-08-25T16:21:41",
			"fileSize": 5703918,
			"metaData" : [
			  {
				"(key1)" : value1,
				"(key2)" : value2,
				...
			  }
			 ]
			}
			,
			{
			"itemID": "/Users/inoblaze/Documents/1920x1080.png",
			"type": "FILE",
			"name": "1920x1080.png",
			"dateTime": "2018-01-04T13:34:30",
			"fileSize": 1099570,
			"metaData" : [
			  {
				"(key1)" : value1,
				"(key2)" : value2,
				...
			  }
			 ]
			}
			,
			{
			"itemID": "/Users/inoblaze/Documents/9865.txt",
			"type": "FILE",
			"name": "9865.txt",
			"dateTime": "2018-08-31T15:28:32",
			"filesize": 9038,
			"metaData" : [
			  {
				"(key1)" : value1,
				"(key2)" : value2,
				...
			  }
			 ]
			}
			,
			{
			"itemID": "/Users/inoblaze/Documents/dressform.Zprj",
			"type": "FILE",
			"name": "dressform.Zprj",
			"dateTime": "2017-07-14T18:16:35",
			"fileSize": 3464539,
			"metaData" : [
			  {
				"(key1)" : value1,
				"(key2)" : value2,
				...
			  }
			 ]
			}
			]
		*/
		virtual std::string GetItemList(const std::string& itemId, unsigned int pageNo, unsigned int pageSize, CLO_FINDER_SORT sort, bool bAscending, const std::string& searchText);

		/*
				this function is deprecated in 5.1
		*/
		virtual std::string GetSearchItemList(unsigned int pageNo, unsigned int pageSize, CLO_FINDER_SORT sort, bool bAscending, const std::string& searchText);

		/*
		This function will be called either when CLO Library Window opens or when a user clicks a folder icon.
		It returns a item (file or folder) list under a folder. CLO will display the item list in its Library window.

		@param itemId The ID of the current folder. If itemId is "", it must return the item list of the root folder.
		@param pageNo Page number(default:1). pageNo will be given by user's scroll.
		@param pageSize The number of items to display in a page.
		@param sort
		@param bAscending
		@param resultItemList - The item list in json format. Developers should implement the value, so that it returns the item list corresponding to "pageNo" and "pageSize"
		@return true - success to search, false - cancel to search.
		*/
		virtual bool GetSearchItemList(const std::string& itemId, unsigned int pageNo, unsigned int pageSize, CLO_FINDER_SORT sort, bool bAscending, std::string& resultItemList);

		/*
		@param parentFolderID The ID of the parent folder.
		@return If any exceptions happen, this function should return "false". If the parent folder is "root", it should return "" ad parentFolderID.
		*/
		virtual bool GetParentFolderID(const std::string& itemId, std::string& parentFolderID);

		/*
		This function is called when starting paging from CLO SW.

		@param itemId The ID of a file item
		@param sizeInByte The size of returned thumbnail image data in bytes
		@return Thumbnail image data in byte array. The data should be given in PNG format.
		*/
		virtual CLO_BYTE* GetPNGThumbnail(const std::string& itemId, unsigned int& sizeInByte);

		/*
		will be supported in v5.1 or above
		*/
		virtual std::string GetMetaData(const std::string& itemId);

		/*
		To get the metadata key list to show the key on the CLO UI
		@param itemId The ID of a file item
		@return std::string list for the meta data keys
		*/
		virtual std::vector<std::string> GetMetaDataKeyListForUI(const std::string& itemId);

		/*
		will be supported in v5.1 or above
		*/
		virtual CLO_BYTE* GetPreviewImage(const std::string& itemId, unsigned int& sizeInByte);

		/*
		This function is called when a user double-clicks/drag-drops a thumbnail icon of a file item.
		~~~~

		@param itemId The ID of a file item
		@param sizeInByte The size of returned file data in bytes
		@return file data in byte array
		*/
		virtual CLO_BYTE* GetFile(const std::string& itemId, unsigned int& sizeInByte);

		/*
		will be supported in v5.1 or above. If you don't want to show Sign-in dialog when users try to use Library Window for PLM, implement it to return "false".
		*/
		virtual bool IsSigninEnabled();

		/*
		will be supported in v5.1 or above. This function will be called when users try to use Library Window for PLM only if IsSigninEnabled() returns "true". Developers should implement this function so that it returns the result of the sign-in to PLM using id and pw.
		@param id The ID of PLM, given in User Settings UI by the user
		@param pw The password of PLM, given in User Settings UI by the user
		@param token This function should set the token value if the sign-in succeeds. This token can be used the following actions in CLO like when you send the current scene to PLM.
		@return If sign-in succeeds, then it returns "true"
		*/
		virtual bool Signin();

		virtual bool Signin(const std::string& id, const std::string& pw, std::string& token);

		virtual std::string GetFavoriteList();

		/// PLM Settings related functions 
		// enable / disable PLM settings
		virtual bool IsPLMSettingsEnabled();

		// create PLM Settings widget and return the void* for the widget instance
		virtual void* GetPLMSettingsWidget();

		// delete the widget instance and set the widget pointer variable as nullptr
		virtual void ReleasePLMSettingsWidget();

		// set the widget pointer variable as nullptr
		virtual void SetNullptrPLMSettingsWidget();

		// reset plm settings
		virtual void ResetPLMSettingsWidget();

		/// plug-in related functions
		virtual bool IsPluginEnabled();
		virtual void DoFunction();
		virtual void DoFunctionStartUp();
		virtual void DoFunctionAfterLoadingCLOFile(const char* fileExtenstion);// ascii
		virtual const char* GetActionName();
		virtual const char* GetObjectNameTreeToAddAction();
		virtual int GetPositionIndexToAddAction();

		/*
		@return the icon file path for the tab ui button when is clicked in the Library Window, CLO
		*/
		virtual std::string GetIconOnFilePathForTab();

		/*
		@return the icon file path for the tab ui button when is not clicked in the Library Window, CLO
		*/
		virtual std::string GetIconOffFilePathForTab();
	};
}

extern "C"
{
	extern CLO_PLUGIN_SPECIFIER CLOAPI::LibraryWindowInterface* Create();
}

#endif //_LIBRARY_API_IMPLMENTATION_H
