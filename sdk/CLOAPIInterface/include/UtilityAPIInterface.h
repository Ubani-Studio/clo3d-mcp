#pragma once

#include <vector>
#include <map>

#include "DefineDllForWin.h"
#include "CloApiData.h"

class QWidget;

namespace CLOAPI
{
	class MV_CLO_INTERFACE_API UtilityAPIInterface {
	public:
		/// @fn GetCLOTemporaryFolderPath()
		/// @brief Create and get the temporary folder path
		/// @return Get the temporary folder path to store temporary files used when exporting to server
		virtual std::string GetCLOTemporaryFolderPath() { return ""; }

		/// @fn GetCLOTemporaryFolderPathW()
		/// @brief Create and get the temporary folder path
		/// @return Get the temporary folder path to store temporary files used when exporting to server
		virtual std::wstring GetCLOTemporaryFolderPathW() { return L""; }

		/// @fn DisplayMessageBox(string str)
		/// @brief Show a message with using the DialogBox implemented in CLO software
		/// @param str: message
		/// @return Display a message box to show input string on CLO. This can be used for debugging.
		virtual void DisplayMessageBox(std::string str) {}

		/// @fn DisplayMessageBoxW(wstring str)
		/// @brief Show a message with using the DialogBox implemented in CLO software
		/// @param str: message
		/// @return Display a message box to show input string on CLO. This can be used for debugging.
		virtual void DisplayMessageBoxW(std::wstring str) {}

		/// @fn GetProjectName()
		/// @brief Get the current project name in CLO
		/// @return The current project name. When you opening "test.zprj" file at last, then the project name is given as "test"
		virtual std::string GetProjectName() { return ""; }

		/// @fn GetProjectNameW()
		/// @brief Get the current project name in CLO
		/// @return The current project name. When you opening "test.zprj" file at last, then the project name is given as "test"
		virtual std::wstring GetProjectNameW() { return L""; }

		/// @fn GetProjectFilePath()
		/// @brief Get the current profile file path
		/// @return The current project file path. If you load a project file saved in "c://download/default.zprj", this function returns the full path - "c://download/default.zprj". If you never load project files, then it returns "NULL".
		virtual std::string GetProjectFilePath() { return ""; }

		/// @fn GetProjectFilePathW()
		/// @brief Get the current profile file path
		/// @return The current project file path. If you load a project file saved in "c://download/default.zprj", this function returns the full path - "c://download/default.zprj". If you never load project files, then it returns "NULL".
		virtual std::wstring GetProjectFilePathW() { return L""; }

		/// @fn GetMajorVersion()
		/// @brief Get the major version of the CLO software
		/// @return Major version of SW. ex) 5 of 5.0.72
		virtual unsigned int GetMajorVersion() { return 0; }

		/// @fn GetMinorVersion()
		/// @brief Get the minor version of the CLO software
		/// @return Minor version of SW. ex) 0 of 5.0.72
		virtual unsigned int GetMinorVersion() { return 0; }

		/// @fn GetPatchVersion()
		/// @brief Get the patch version of the CLO software
		/// @return Patch version of SW. ex) 72 of 5.0.72
		virtual unsigned int GetPatchVersion() { return 0; }

		/// @fn toUtf8(const std::wstring &str)
		/// @brief Convert and get the string encoded in UTF-8 from wstring
		/// @param str: target wstring to convert
		/// @return string in UTF8 from wstring encoded by UCS-2 in Windows or UTF-8 in Mac OS
		virtual std::string toUtf8(const std::wstring &str) { return ""; }

		/// @fn GetColorwayCount()
		/// @brief Get the number of colorways in the current project
		/// @return The number of Colorways in the current project.
		virtual unsigned int GetColorwayCount() { return 0; }

		/// @fn GetCurrentColorwayIndex()
		/// @brief Get the current colorway index
		/// @return The current colorway index.
		virtual unsigned int GetCurrentColorwayIndex() { return 0; }

		/// @fn SetCurrentColorwayIndex(unsigned int index)
		/// @brief Change the current colorway
		/// @param index: the colorway index
		virtual void SetCurrentColorwayIndex(unsigned int index) { }

		/// @fn SetColorwayName(unsigned int index, const string& str)
		/// @brief Change colorway name
		/// @param index: the target colorway index to change the name
		/// @param str: new name for the colorway
		virtual void SetColorwayName(unsigned int index, const std::string& str) {}

		/// @fn SetColorwayNameW(unsigned int index, const wstring& wstr)
		/// @brief Change colorway name
		/// @param index: the target colorway index to change the name
		/// @param str: new name for the colorway
		virtual void SetColorwayNameW(unsigned int index, const std::wstring& wstr) {}

		/// @fn GetColorwayName(unsigned int index)
		/// @brief Get the colorway name for the colorway index
		/// @param index: colorway index to get the name
		/// @return the colorway name for the colorway index
		virtual std::string GetColorwayName(unsigned int index) { return ""; }

		/// @fn GetColorwayNameW(unsigned int index)
		/// @brief Get the colorway name for the colorway index
		/// @param index: colorway index to get the name
		/// @return the colorway name for the colorway index
		virtual std::wstring GetColorwayNameW(unsigned int index) { return L""; }

		/// @fn CopyColorway(unsigned int index, int _colorwayCopyOption)
		/// @brief Copy the colorway in the index and create a new one
		/// @param index: the source colorway index to copy
		/// @param _colorwayCopyOption: The copy option
		///								0: Unlink all properties
		///								1: Unlink material properties only
		///								2: Link all properties
		/// 
		/// @return index for the created colorway
		virtual unsigned int CopyColorway(unsigned int index, int _colorwayCopyOption) { return 0; }

		/// @fn GetCustomViewInformation()
		/// @brief Get the custom view information		
		/// {
		/// "customViewList": [
		/// 	{
		/// 		"cameraMatrix": [ // row vector 0, 1, 2 of "local to world camera matrix"
		/// 		  1.0,
		/// 		  0.000166,
		/// 		  -0.001024,
		/// 		  0.352125,
		/// 		  0.0,
		/// 		  0.987069,
		/// 		  0.160306,
		/// 		  2592.8,
		/// 		  0.001039,
		/// 		  -0.160306,
		/// 		  0.987068,
		/// 		  7973.05
		/// 		],
		/// 		"fov": 40.0
		/// 	}
		/// ]
		/// }		
		/// @return Json stream including the data of Custom View such as Camera Matrix, FOV, and so on.
		virtual std::string GetCustomViewInformation() { return ""; }

		/// @fn GetCustomViewInformationW()
		/// @brief Get the custom view information		
		/// {
		/// "customViewList": [
		/// 	{
		/// 		"cameraMatrix": [ // row vector 0, 1, 2 of "local to world camera matrix"
		/// 		  1.0,
		/// 		  0.000166,
		/// 		  -0.001024,
		/// 		  0.352125,
		/// 		  0.0,
		/// 		  0.987069,
		/// 		  0.160306,
		/// 		  2592.8,
		/// 		  0.001039,
		/// 		  -0.160306,
		/// 		  0.987068,
		/// 		  7973.05
		/// 		],
		/// 		"fov": 40.0
		/// 	}
		/// ]
		/// }		
		/// @return Json stream including the data of Custom View such as Camera Matrix, FOV, and so on.
		virtual std::wstring GetCustomViewInformationW() { return L""; }

		/// @fn GetClothPositions(vector<float>& positions)
		/// @brief Get the position array of cloth shape. Each three array has x, y, z position of each vertex. 
		/// @param positions: the output position list for the cloth shape
		virtual void GetClothPositions(std::vector<float>& positions) {}

		/// @fn ResetClothArrangement()
		/// @brief Restore the shape of cloth to when the cloth was loaded
		virtual void ResetClothArrangement() {}

		/// @fn GetThumbnailInCLOFile(std::string filePath, unsigned int thumbnailIndex, unsigned int& fileSize)
		/// @brief Get Preview thumbnail file buffer(png type) from the CLO file format(.zprj, .zpac, .zfab, .sst, and so on)
		/// @param filePath: the source file path to retrieve the thumbnail buffer
		/// @param thumbnailIndex: the index of the thumbnails in the CLO file format(the number of thumbnails differs by the format)
		/// @param fileSize: get the size of the thumbnail buffer
		/// @return thumbnail(png) buffer
		virtual unsigned char* GetThumbnailInCLOFile(std::string filePath, unsigned int thumbnailIndex, unsigned int& fileSize) { return nullptr; }

		/// @fn SaveCLOFileThumbnail(std::string filePath, std::string outputPath, unsigned int thumbnailIndex)
		/// @brief Save Preview thumbnail image file(png type) from the CLO file format(.zprj, .zpac, .zfab, .sst, and so on)
		/// @param filePath: the source file path to retrieve the thumbnail buffer
		/// @param outputPath: output png imager path
		/// @param thumbnailIndex: the index of the thumbnails in the CLO file format(the number of thumbnails differs by the format)
		/// @return save result
		virtual bool SaveCLOFileThumbnail(std::string filePath, std::string outputPath, unsigned int thumbnailIndex) { return false; }

		/// @fn SaveCLOFileThumbnail(std::string filePath, std::wstring outputPath, unsigned int thumbnailIndex)
		/// @brief Save Preview thumbnail image file(png type) from the CLO file format(.zprj, .zpac, .zfab, .sst, and so on)
		/// @param filePath: the source file path to retrieve the thumbnail buffer
		/// @param outputPath: output png imager path
		/// @param thumbnailIndex: the index of the thumbnails in the CLO file format(the number of thumbnails differs by the format)
		/// @return save result
		virtual bool SaveCLOFileThumbnail(std::string filePath, std::wstring outputPath, unsigned int thumbnailIndex) { return false; }

		/// @fn GetAssetIconInCLOFile(std::string filePath, unsigned int& fileSize)
		/// @brief Get Asset Icon thumbnail file buffer(png type) from the CLO file format(.zprj, .zpac, .zfab, .sst, and so on)
		/// @param filePath: the source file path to retrieve the thumbnail buffer		
		/// @param fileSize: get the size of the thumbnail buffer
		/// @return thumbnail(png) buffer
		virtual unsigned char* GetAssetIconInCLOFile(std::string filePath, unsigned int& fileSize) { return nullptr; }

		/// @fn GetMetaDataForCurrentGarment()
		/// @brief Get Meta data for the current Garment
		/// @return the meta data for the current garment
		virtual std::string GetMetaDataForCurrentGarment() { return ""; }

		/// @fn GetMetaDataForCurrentGarmentW()
		/// @brief Get Meta data for the current Garment
		/// @return the meta data for the current garment
		virtual std::wstring GetMetaDataForCurrentGarmentW() { return L""; }

		/// @fn SetMetaDataForCurrentGarment(const string& metaDataStr)
		/// @brief Overwrite Meta data for the current Garment
		/// @param metaDataStr: the new meata data to set to the current garment
		virtual void SetMetaDataForCurrentGarment(const std::string& metaDataStr) { return; }

		/// @fn ChangeMetaDataValueForCurrentGarment(const string& metaDataKey, const string& metaDataValue)
		/// @brief Change Meta Data Value for the current Garment
		/// @metaDataKey: target Key
		/// @metaDataValue: the new value for the key
		virtual void ChangeMetaDataValueForCurrentGarment(const std::string& metaDataKey, const std::string& metaDataValue) {}

		/// @fn CreateProgressBar()
		/// @brief Create Progress Bar to show progress. This function should be called before using SetProgress function		
		virtual void CreateProgressBar() {}

		/// @fn SetProgress(const string& title, int progress)
		/// @brief Set progress on the Progress Bar
		virtual void SetProgress(const std::string& title, int progress) {}

		/// @fn SetProgress(const string& title, int progress)
		/// @brief Set progress on the Progress Bar
		virtual void SetProgressW(const std::wstring& title, int progress) {}

		/// @fn DeleteProgressBar(bool bForce)
		/// @brief Delete the current Progress Bar. This function should be called after using the SetProgress funtion
		/// @param bForce: true - retry to delete progress bar; false - try only once to delete progress bar
		virtual void DeleteProgressBar(bool bForce) {}

		/// @fn stringToMD5(const string& str)
		/// @brief Create MD5 hashing string from string
		/// @param str: the source string to hash 
		/// @return MD5 hashed string
		virtual std::string stringToMD5(const std::string& str) { return ""; }

		/// @fn AddColorSwatch(const map<string, string>& plmIDtoNameList, const map<string, Marvelous::CloApiRgb>& plmIDtoColorList, const map<string, string>& plmIDtoApiMetaDataList, const string& swatchName)
		/// @brief Add Color Swatch
		/// @param plmIDtoNameList: the list for map of PLM ID to Color Name
		/// @param plmIDtoColorList: list for map of PLM ID to color name
		/// @param plmIDtoApiMetaDataList: list for map of PLM ID to Api Meta Data
		/// @param swatchName: swatchName which will be shown in the color palette and the swatch file name to save
		/// @return added swatch file name(.aco) inside the asset folder for the current version of CLO
		virtual std::string AddColorSwatch(const std::map<std::string, std::string>& plmIDtoNameList, const std::map<std::string, Marvelous::CloApiRgb>& plmIDtoColorList, const std::map<std::string, std::string>& plmIDtoApiMetaDataList, const std::string& swatchName) { return ""; }

		/// @fn AddColorSwatchW(const map<wstring, wstring>& plmIDtoNameList, const map<wstring, Marvelous::CloApiRgb>& plmIDtoColorList, const map<wstring, wstring>& plmIDtoApiMetaDataList, const wstring& swatchName)
		/// @brief Add Color Swatch
		/// @param plmIDtoNameList: the list for map of PLM ID to Color Name
		/// @param plmIDtoColorList: list for map of PLM ID to color name
		/// @param plmIDtoApiMetaDataList: list for map of PLM ID to Api Meta Data
		/// @param swatchName: swatchName which will be shown in the color palette and the swatch file name to save
		/// @return added swatch file name(.aco) inside the asset folder for the current version of CLO
		virtual std::wstring AddColorSwatchW(const std::map<std::wstring, std::wstring>& plmIDtoNameList, const std::map<std::wstring, Marvelous::CloApiRgb>& plmIDtoColorList, const std::map<std::wstring, std::wstring>& plmIDtoApiMetaDataList, const std::wstring& swatchName) { return L""; }

		/// @fn LoadLibraryColorSwatchList(const std::vector<std::string>& library)
		/// @brief Load the library and add it to the color palette tab.
		/// @param library: the list with multiple color swatch file paths
		/// @param tabName: tabName which will be shown in the color palette library tab
		virtual void LoadLibraryColorSwatchList(const std::vector<std::string>& library, const std::string tabName) {}

		/// @fn AddLibraryColorSwatchList(const std::vector<std::string>& library, const std::string& tabName)
		/// @brief Add the library to the color swatch list of the color palette tab(for combobox palette).
		/// @param library: The list of multiple color swatch file paths
		/// @param tabName: Name of the color palette library tab
		/// @return true on success, false on failure
		virtual bool AddLibraryColorSwatchList(const std::vector<std::string>& library, const std::string& tabName) { return false; }

		/// @fn SetColorSwatchListItemName(const std::string& tabName, const int index, const std::string& text)
		/// @brief Sets the name of a color switch item in the library(for combobox palette).
		/// @param tabName: Name of the color palette library tab
		/// @param index: Index of the item to be renamed from the color swatch list
		/// @param text: Text to be set
		/// @return true on success, false on failure
		virtual bool SetColorSwatchListItemName(const std::string& tabName, const int index, const std::string& text) {return false; }

		/// @fn DeleteColorSwatchListItem(const std::string& tabName, int index)
		/// @brief Delete the index item from the Color Switch list within the library tab(for combobox palette).
		/// @param tabName: Name of the color palette library tab
		/// @param index: Index of the item to be deleted from the color swatch list.
		/// @return true on success, false on failure
		virtual bool DeleteColorSwatchListItem(const std::string& tabName, int index) { return false; }
		
		/// @fn SetColorSwatchListItemName(const std::string& tabName, const int index, const std::string& text)
		/// @brief Sets the name of the library tab.
		/// @param tabName: Name of the color palette library tab
		/// @param newTabName: Text to be set
		/// @return true on success, false on failure
		virtual bool SetColorSwatchLibraryTabName(const std::string& tabName, const std::string& newTabName) { return false; }

		/// @fn DeleteColorSwatchLibraryTabByName(const std::string tabName, bool bCaseInsensitive = true)
		/// @brief Delete library tab from color palette in name 
		/// @param tabName: tabName which will be shown in the color palette library tab
		/// @param bCaseInsensitive: If true, case is not sensitive.
		/// @return true if deletion is successful
		virtual bool DeleteColorSwatchLibraryTabByName(const std::string& tabName, bool bCaseInsensitive = true) { return false; }

		/// @fn NewProject()
		/// @brief Clear the current garment and begin a new garment
		virtual void NewProject() {}

		/// API calls; especially for Library Windows Sample Demo.
		/// - IsReadableImageFormatFromExtension / IsReadableImageFormatFromExtensionW
		/// : Check if the file stream loaded from the filepath used in Library Windows Implementations can be displayed on Library Windows for item thumbnail and/or preview dialog.
		///   (The files should not contain image data over 8bits per channel for 1 ~ 4 channels image.)
		/// - IsCLOFileFormatWithThumbnailExtension / IsCLOFileFormatWithThumbnailExtensionW
		/// : Check if the CLO file format has the thumbnail memory.
		/// 			
		/// - IsCLOFileFormatWithTripleThumbnailExtension / IsCLOFileFormatWithTripleThumbnailExtensionW
		/// : Check if the CLO file format has the three images memory for preview thumbnail for the Library Preview Dialog.

		/// @fn IsReadableImageFormat(const string& filePath)
		/// @brief Check if the file stream loaded from the filepath used in Library Windows Implementations can be displayed on Library Windows for item thumbnail and/or preview dialog.
		/// @return true if the file can be displayed on Library Windows for item thumbnail and/or preview dialog
		virtual bool IsReadableImageFormat(const std::string& filePath) { return false; }

		/// @fn IsReadableImageFormatW(const wstring& filePath)
		/// @brief Check if the file stream loaded from the filepath used in Library Windows Implementations can be displayed on Library Windows for item thumbnail and/or preview dialog.
		/// @return true if the file can be displayed on Library Windows for item thumbnail and/or preview dialog
		virtual bool IsReadableImageFormatW(const std::wstring& filePath) { return false; }

		/// @fn IsCLOFileFormatWithThumbnail(const string& filePath)
		/// @brief Check if the CLO file format has the thumbnail memory.
		/// @return true if the file is the CLO format file and contains the thumbnail memory which can be displayed on Library Windows for item thumbnail and/or preview dialog
		virtual bool IsCLOFileFormatWithThumbnail(const std::string& filePath) { return false; }

		/// @fn IsCLOFileFormatWithThumbnailW(const wstring& filePath)
		/// @brief Check if the CLO file format has the thumbnail memory.
		/// @return true if the file is the CLO format file and contains the thumbnail memory which can be displayed on Library Windows for item thumbnail and/or preview dialog
		virtual bool IsCLOFileFormatWithThumbnailW(const std::wstring& filePath) { return false; }

		/// @fn IsCLOFileFormatWithTripleThumbnail(const string& filePath)
		/// @brief Check if the CLO file format has the three images memory for preview thumbnail for the Library Preview Dialog.
		/// @return true if the CLO file format has the three images memory for preview thumbnail for the Library Preview Dialog.
		virtual bool IsCLOFileFormatWithTripleThumbnail(const std::string& filePath) { return false; }

		/// @fn IsCLOFileFormatWithTripleThumbnailW(const wstring& filePath)
		/// @brief Check if the CLO file format has the three images memory for preview thumbnail for the Library Preview Dialog.
		/// @return true if the CLO file format has the three images memory for preview thumbnail for the Library Preview Dialog.
		virtual bool IsCLOFileFormatWithTripleThumbnailW(const std::wstring& filePath) { return false; }

		/// @fn GetCLOExecutableFolderPath(bool bLinuxTypePathDelimeter = true)
		/// @brief Get the executable folder path where the current version of CLO is located
		/// @param bLinuxTypePathDelimeter: if true, set path delimiter to '/', false: set path delimiter to '\'
		/// @return Get the executable folder path for the current version of CLO software
		virtual std::string GetCLOExecutableFolderPath(bool bLinuxTypePathDelimeter = true) { return ""; }

		/// @fn GetCLOExecutableFolderPathW(bool bLinuxTypePathDelimeter = true)
		/// @brief Get the executable folder path where the current version of CLO is located
		/// @param bLinuxTypePathDelimeter: if true, set path delimiter to '/', false: set path delimiter to '\'
		/// @return Get the executable folder path for the current version of CLO software
		virtual std::wstring GetCLOExecutableFolderPathW(bool bLinuxTypePathDelimeter = true) { return L""; }

		/// @fn GetCLOAssetFolderPath(bool bLinuxTypePathDelimeter = true)
		/// @brief Get the folder path where the CLO asset files are distributed
		/// @param bLinuxTypePathDelimeter: if true, set path delimiter to '/', false: set path delimiter to '\'
		/// @return Get the asset folder path where the CLO asset files are located in
		virtual std::string GetCLOAssetFolderPath(bool bLinuxTypePathDelimeter = true) { return ""; }

		/// @fn GetCLOAssetFolderPathW(bool bLinuxTypePathDelimeter = true)
		/// @brief Get the folder path where the CLO asset files are distributed
		/// @param bLinuxTypePathDelimeter: if true, set path delimiter to '/', false: set path delimiter to '\'
		/// @return Get the asset folder path where the CLO asset files are located in
		virtual std::wstring GetCLOAssetFolderPathW(bool bLinuxTypePathDelimeter = true) { return L""; }

		/// @fn GetAPIMetaData(const string& filePath)
		/// @brief Get API meta data for the file
		/// @param filePath: CLO file path
		/// @return json string for meta data [key - value] list		
		virtual std::string GetAPIMetaData(const std::string& filePath) { return ""; }

		/// @fn GetAPIMetaDataW(const wstring& filePath)
		/// @brief Get API meta data for the file
		/// @param filePath: CLO file path
		/// @return json string for meta data [key - value] list
		virtual std::wstring GetAPIMetaDataW(const std::wstring& filePath) { return L""; }

		/// @fn SetAPIMetaData(const string& filePath, string jsonStr)
		/// @brief Set API meta data to the file
		/// @param filePath: target file path
		/// @param jsonStr: api meta data in string format
		/// @return if it succeeds, return true
		virtual bool SetAPIMetaData(const std::string& filePath, std::string jsonStr) { return ""; }

		/// @fn SetAPIMetaDataW(const wstring& filePath, wstring jsonStr)
		/// @brief Set API meta data to the file
		/// @param filePath: target file path
		/// @param jsonStr: api meta data in string format
		/// @return if it succeeds, return true
		virtual bool SetAPIMetaDataW(const std::wstring& filePath, std::wstring jsonStr) { return L""; }

		/// @fn Set3DWindowTitle(const string& title)
		/// @brief Set 3D Window Title
		/// @param title: desired text in 3D window
		/// @return if it succeeds, return true
		virtual bool Set3DWindowTitle(const std::string& title) { return false; }

		/// @fn Set3DWindowTitleW(const wstring& title) 
		/// @brief Set 3D Window Title
		/// @param title: desired text in 3D window
		/// @return if it succeeds, return true
		virtual bool Set3DWindowTitleW(const std::wstring& title) { return false; }

		/// @fn GetColorListForColorWay(unsigned int _colorWayIndex) 
		/// @brief Get Color List for ColorWay
		/// @param _colorwayIndex: the colorway index to get the result
		/// @return the color list in CloAgiRgba for the target ColorWay
		virtual std::vector<Marvelous::CloApiRgba> GetColorListForColorWay(unsigned int _colorWayIndex) { return std::vector<Marvelous::CloApiRgba>(); }

		/// @fn SetShowHideAvatar(bool _bShow) 
		/// @brief Set all avatars' show/hide status
		/// @param _bShow: true for show, false for hide
		virtual void SetShowHideAvatar(bool _bShow) {}

		/// @fn SetShowHideAvatar(bool _bShow, int _avatarIndex) 
		/// @brief Set show/hide status of avatar that matches the index
		/// @param _bShow: true for show, false for hide
		virtual void SetShowHideAvatar(bool _bShow, int _avatarIdex) {}

		/// @fn IsShowAvatar(int _avatarIndex) 
		/// @brief Get show status of avatar that matches the index
		/// @param _avatarIndex: the avatar index to get the result.
		/// @return if avatar is shown, return true. if avatar is hidden, return false.
		virtual bool IsShowAvatar(int _avatarIndex) { return false; }

		/// @fn SetSchematicRender(bool _bSet) 
		/// @brief Set Schematic Render status
		/// @param _bSet: true for enable, false for disable
		virtual void SetSchematicRender(bool _bSet) {}

		/// @fn UpdateColorways(bool bUpdateSnapshot = true)	
		/// @brief Update Colorways in the CLO software
		/// @param bUpdateSnapshot: put the value to true to update the colorway's snapshot
		virtual void UpdateColorways(bool bUpdateSnapshot = true) {}

		/// @fn DeleteColorwayItem(unsigned int _index)
		/// @brief Delete the colorway in the index
		/// @param index: the source colorway index to delete
		virtual void DeleteColorwayItem(unsigned int _index) {}

		///	@fn ReplaceGraphicTextureFromImage(int graphicstyleIndex, const string& _imagePath)
		/// @brief replace graphic style in index 
		/// @param _imagePath: input file path
		/// @param _graphicstyleindex: index of the style to be replaced
		/// @param _anchor: position that will be fixed when replacing. (0: center, 1: up, 2: right-up, 3: right, 4: right-down, 5: down, 6: left-down, 7: left, 8: left-up)
		/// @return if it succeeds, return true.
		virtual bool ReplaceGraphicStyleFromImage(int _graphicstyleIndex, const std::string& _imagePath, int _anchor) { return false; }

		///	@fn ReplaceGraphicTextureFromImage((int graphicstyleIndex, const wstring& _imagePath)
		/// @brief replace graphic style in index
		/// @param _imagePath: input file path
		/// @param _graphicstyleindex: index of the style to be replaced
		/// @param _anchor: position that will be fixed when replacing. (0: center, 1: up, 2: right-up, 3: right, 4: right-down, 5: down, 6: left-down, 7: left, 8: left-up)
		/// @return if it succeeds, return true.
		virtual bool ReplaceGraphicStyleFromImage(int _graphicstyleIndex, const std::wstring& _imagePath, int _anchor) { return false; }

		///	@fn Simulate(unsigned int _steps)
		/// @brief Simulate the garment in multi steps. All dynamics properties (time step, CG iteration count, ...) follow the current simulation properties
		/// @param _steps: how many steps(=frames) to simulate
		/// @return if it succeeds, return true.
		virtual bool Simulate(unsigned int _steps) { return false; }

		///	@fn SetSimulationQuality(int _quality)
		/// @brief Set simulation quality preset
		/// @param _quality: input simulation preset (0: Normal(Default), 1: Animation(Stable), 2: Fitting(Accurate Fabric), 3: FAST(GPU))
		/// @param _simulationMode: input simulation mode (0: CPU, 1: FAST(GPU))
		virtual void SetSimulationQuality(int _quality, int _simulationMode) {}

		///	@fn SetSimulationTimeStep(int _timeStep)
		/// @brief Set simulation time step
		/// @param _timeStep: time step in second (Default : 0.03333, Min value: 0.001)
		virtual void SetSimulationTimeStep(float _timeStep) {}

		///	@fn SetSimulationNumberOfSimulation(int _numberOfSimulation)
		/// @brief Set simulation substeps in one time step
		/// @param _numberOfSimulation: the number of substeps in one time step (Default : 1, Min/Max Range : 1~50)
		virtual void SetSimulationNumberOfSimulation(int _numberOfSimulation) {}

		///	@fn SetSimulationCGFinishCondition(int _cgFinishCondition)
		/// @brief Set simulation finish condition
		/// @param _cgFinishCondition : Conjugate Gradient finish condition (0: iteration, 1: residual)
		virtual void SetSimulationCGFinishCondition(int _cgFinishCondition) {}

		///	@fn SetSimulationCGIterationCount(int _cgIterationCount)
		/// @brief Set Conjugate Gradient iteration count per one step.
		/// @param _cgIterationCount : iteration count (Default : 50, Min/Max Range 1~1000)
		virtual void SetSimulationCGIterationCount(int _cgIterationCount) {}

		///	@fn SetSimulationCGResidual(int _cgResidual)
		/// @brief Set Conjugate Gradient relative residual value for finishing
		/// @param _cgResidual : relative residual value for finishing (0.0001 in Normal, 0.00001 in Animation and Fitting)
		virtual void SetSimulationCGResidual(float _cgResidual) {}

		///	@fn SetSimulationSelfCollisionIterationCount(int _selfCollisionIterationCount)
		/// @brief Set self collision iteration count
		/// @param _selfCollisionIterationCount : self collision iteration count (Default: 1)
		virtual void SetSimulationSelfCollisionIterationCount(int _selfCollisionIterationCount) {}

		///	@fn SetSimulationAirDamping(int _airDamping)
		/// @brief Set air damping value
		/// @param _airDamping : air damping(Default: 1.0)
		virtual void SetSimulationAirDamping(float _airDamping) {}

		///	@fn SetSimulationGravity(int _gravity)
		/// @brief Set gravity value in y-axis
		/// @param _gravity : gravity  (Default: -9800.0 mm/s^2)
		virtual void SetSimulationGravity(float _gravity) {}

		///	@fn SetSimulationNumberOfCPUInUse(int _numberOfSimulationInUse)
		/// @brief Set the number of CPUs for simulation
		/// @param _numberOfSimulationInUse : the number of CPUs (Default: total number of logical threads - 1, Min/Max Range : 1~total number of logical threads)
		virtual void SetSimulationNumberOfCPUInUse(int _numberOfSimulationInUse) {}

		///	@fn SetSimulationNonlinearSimulation(int _bNonlinearSimulation)
		/// @brief Set nonlinear simulation
		/// @param _bNonlinearSimulation : true for enable, false for disable
		virtual void SetSimulationNonlinearSimulation(bool _bNonlinearSimulation) {}

		///	@fn SetSimulationGroundCollision(int _bGroundCollision)
		/// @brief Set ground collision
		/// @param _bGroundCollision : true for enable, false for disable
		virtual void SetSimulationGroundCollision(bool _bGroundCollision) {}

		///	@fn SetSimulationGroundHeight(int _groundHeight)
		/// @brief Set ground height value
		/// @param _groundHeight : ground height in y-axis (Default: 0.0)
		virtual void SetSimulationGroundHeight(float _groundHeight) {}

		///	@fn SetSimulationSelfCollisionAvoidanceStiffness(int _avoidanceStiffness)
		/// @brief Set stiffness of the inserted spring to avoid self collision
		/// @param _avoidanceStiffness : (Default: 0.001)
		virtual void SetSimulationSelfCollisionAvoidanceStiffness(float  _avoidanceStiffness) {}

		///	@fn SetSimulationLayerBasedCollisionDetection(int _bUseLayer)
		/// @brief Set layer based collision detection 
		/// @param _bUseLayer : true for enable, false for disable
		virtual void SetSimulationLayerBasedCollisionDetection(bool _bUseLayer) {}


		///	@fn StartNesting()
		/// @brief Start Nesting Simulation
		virtual void StartNesting() {}

		///	@fn GetNestingBufferSpacing()
		/// @brief Get Nesting Buffer Space 
		virtual float GetNestingBufferSpacing() { return 0.f; }

		///	@fn SetNestingBufferSpacing(float _mm)
		/// @brief Set Nesting Buffer Space 
		/// @param _mm : Buffer Space 
		virtual void SetNestingBufferSpacing(float _mm) {}

		///	@fn SetNestingTargetColorway(std::vector<int> _colorwayList)
		/// @brief Colorway index to apply nesting result
		/// @param _colorwayList : Colorway index to apply nesting result
		virtual void SetNestingTargetColorway(std::vector<int> _colorwayList) {}

		///	@fn GetNestingTargetColorway()
		/// @brief Gets the colorway index to apply nesting to.
		virtual std::vector<int> GetNestingTargetColorway() { return {}; }


		///	@fn SetAvatarBodyTexture(const wstring& _bodyShapeName, int _matShapeIndex, const wstring& _textureImagePath)
		/// @brief Import AvtarTexture Body Part(face, body, arm, leg)
		/// @param _bodyShapeName: input avatar shape name(find object browser)
		/// @param _matShapeIndex: mat shape index (0: face, 1:body, 2:arm, 3:leg)
		/// @param _textureImagePath: change texture full path
		/// @return if it succeeds, return true.
		virtual bool SetAvatarMeshTexture(const std::wstring& _matShapeName, int _matMeshIndex, const std::wstring& _textureImagePath) { return false; }

		///	@fn SetAnimationRecording(bool _bStart)
		/// @brief set animation recording
		/// @param _bStart: true for recording start, false for recording stop
		virtual void SetAnimationRecording(bool _bStart) {}

		///	@fn SetCurrentAnimationFrame(double _frame)
		/// @brief set current animation frame
		/// @param _frame: Value for setting current frame
		virtual void SetCurrentAnimationFrame(double _frame) {}

		///	@fn GetCurrentAnimationFrame()
		/// @brief get current animation frame
		virtual double GetCurrentAnimationFrame() { return -1.0; }

		///	@fn SetStartAnimationFrame(double _frame)
		/// @brief set start animation frame
		/// @param _frame: Value for setting start frame
		virtual void SetStartAnimationFrame(double _frame) {}

		///	@fn GetStartAnimationFrame()
		/// @brief get start animation frame
		virtual double GetStartAnimationFrame() { return -1.0; }

		///	@fn SetEndAnimationFrame(double _frame)
		/// @brief set end animation frame
		/// @param _frame: Value for setting end frame
		virtual void SetEndAnimationFrame(double _frame) {}

		///	@fn GetEndAnimationFrame()
		/// @brief get end animation frame
		virtual double GetEndAnimationFrame() { return -1.0; }

		///	@fn GetStressMapStatus()
		/// @brief return current rendered status of the stress map (false = Off | true = On)
		virtual bool GetStressMapStatus() { return false; }

		///	@fn SetStressMapStatus(bool _bOn)
		/// @brief set the rendered status of stress map
		/// @param _bOn: false = Off | true = On
		virtual void SetStressMapStatus(bool _bOn) {}

		///	@fn GetStrainMapStatus()
		/// @brief return current rendered status of the strain map (false = Off | true = On)
		virtual bool GetStrainMapStatus() { return false; }

		///	@fn SetStrainMapStatus(bool _bOn)
		/// @brief set the rendered status of strain map
		/// @param _bOn: false = Off | true = On
		virtual void SetStrainMapStatus(bool _bOn) {}

		/// @fn GetSchematicRender(bool _bSet)
		/// @brief Get Show Schematic Render Status
		virtual bool GetSchematicRender() { return false; }

		///	@fn RefreshPlugIns()
		/// @brief Refresh all PlugIns
		virtual void RefreshPlugIns() {}

		///	@fn SetCamViewPoint(int _viewPointIndex)
		/// @brief set camera view point
		/// @param _viewPointIndex: Value for setting camera view point index(0: bottom, 1:3/4 right, 2:front, 3:3/4 left, 4: right, 5:top, 6:left, 7:focus zoom, 8:back, 9:zoom extents all)
		virtual void SetCamViewPoint(int _viewPointIndex) {}

		///	@fn StopNesting()
		/// @brief Stop Nesting process
		virtual void StopNesting() {}

		/// @fn CreateModularCategory(const std::wstring& _title, const std::wstring& _folderPath)
		/// @brief create modular category
		/// @param _title : category title, _folderPath: folder
		virtual void CreateModularCategory(const std::wstring& _title, const std::wstring& _folderPath) {}

		/// @fn DeleteModularCategory(const std::wstring& _title, const std::wstring& _folderPath)
		/// @brief delete modular category
		/// @param _title : category title
		virtual void DeleteModularCategory(const std::wstring& _title) {}

		///	@fn GetNestingTime()
		/// @brief Get Nesting duration Time (milliseconds)
		virtual int GetNestingTime() { return 0; }

		///	@fn SetSchematicSilhouetteLineWidth(int _value)
		/// @brief set schematic silhouette line width
		/// @param _value: Value for setting schematic silhouette line width
		virtual void SetSchematicSilhouetteLineWidth(int _value) {}

		///	@fn SetSchematicSeamLineWidth(int _value)
		/// @brief set schematic seamline width
		/// @param _value: Value for setting schematic seamline width
		virtual void SetSchematicSeamLineWidth(int _value) {}

		///	@fn SetSchematicInternalLineWidth(int _value)
		/// @brief set schematic internal line width
		/// @param _value: Value for setting chematic internal line width
		virtual void SetSchematicInternalLineWidth(int _value) {}

		///	@fn SetSchematicTopstitchLineScalePercent(int _value)
		/// @brief set schematic topstitch line scale percent
		/// @param _value: Value for setting schematic topstitch line scale percent
		virtual void SetSchematicTopstitchLineScalePercent(int _value) {}

		///	@fn SetSchematicBrightness(int _value)
		/// @brief set schematic brightness
		/// @param _value: Value for setting schematic brightness
		virtual void SetSchematicBrightness(int _value) {}

		///	@fn SetShowSchematicSilhouetteLine(bool _bOn)
		/// @brief set show schematic silhouetteline
		/// @param _bOn: Value for setting showschematic silhouetteline
		virtual void SetShowSchematicSilhouetteLine(bool _bOn) {}

		///	@fn SetShowSchematicSeamLine(int _bOn)
		/// @brief set show schematic seamline
		/// @param _bOn: Value for setting show schematic seamline
		virtual void SetShowSchematicSeamLine(bool _bOn) {}

		///	@fn SetShowSchematicInternalLine(bool _bOn)
		/// @brief set show schematic internal line
		/// @param _bOn: Value for setting show schematic internalline
		virtual void SetShowSchematicInternalLine(bool _bOn) {}

		///	@fn SetShowSchematicTopstitchLine(bool _bOn)
		/// @brief set show schematic topstitch line
		/// @param _bOn: Value for setting show schematic topstitchline
		virtual void SetShowSchematicTopstitchLine(bool _bOn) {}

		///	@fn SetSchematicClothRenderType(bool _bTexture)
		/// @brief set schematic cloth render type
		/// @param _bTexture: Value for setting schematic cloth render type
		virtual void SetSchematicClothRenderType(bool _bTexture) {}

		///	@fn SetStyleLineColor(int _r, int _g, int _b)
		/// @brief set style line color
		/// @param int _r, int _g, int _b: Value for setting style line color
		/// Red: 0 ~ 255
		/// Green: 0 ~ 255
		/// Blue: 0 ~ 255
		virtual void SetStyleLineColor(int _r, int _g, int _b) {}

		///	@fn SetSchematicClothColor(int _r, int _g, int _b)
		/// @brief set schematic cloth color
		/// @param int _r, int _g, int _b: Value for setting schematic cloth color
		/// Red: 0 ~ 255
		/// Green: 0 ~ 255
		/// Blue: 0 ~ 255
		virtual void SetSchematicClothColor(int _r, int _g, int _b) {}

		/// @fn SetQualityRender(bool _bSet)
		/// @brief Set Quality Render status
		/// @param _bSet: true for enable, false for disable 
		virtual void SetQualityRender(bool _bSet) {}

		/// @fn GetQualityRenderStatus(bool _bSet)
		///	@brief Get Current Quality Render Status
		virtual bool GetQualityRenderStatus() { return false; }

		/// @fn BakeUVTexture(const string& _filePath, const Marvelous::ImportExportOption& _options)
		///	@brief Bake textures on UV editor
		/// @param _filePath: File path to be saved.
		///	@param _options: It bakes textures according to options, not allowing user for selecting options in Bake Textures Dialog.
		/// @param _bAll: true for all UV tiles, false for only 0-1 tile
		///	@return Output file paths. 
		virtual std::vector<std::string> BakeUVTexture(const std::string& _filePath, const Marvelous::ImportExportOption& _options, bool _bAll)
		{
			std::vector<std::string> interface_vector;
			return interface_vector;
		}

		/// @fn BakeUVTextureW(const wstring& _filePath, const Marvelous::ImportExportOption& _options)
		///	@brief Bake textures on UV editor
		/// @param _filePath: File path to be saved.
		///	@param _options: It bakes textures according to options, not allowing user for selecting options in Bake Textures Dialog.
		/// @param _bAll: true for all UV tiles, false for only 0-1 tile
		///	@return Output file paths. 
		virtual std::vector<std::wstring> BakeUVTextureW(const std::wstring& _filePath, const Marvelous::ImportExportOption& _options, bool _bAll)
		{
			std::vector<std::wstring> interface_vector;
			return interface_vector;
		}

		/// @fn UVPacking(bool _bAll, float _textureFillSeam, int _imgSize)
		/// @brief Packing patterns in UV editor
		/// @param _bAll: true for all patterns, false for selected patterns
		/// @param _bFixScaleRatio: true - preserve current patterns size; false - rescale patterns
		/// @param _bAdvancedSearch: true - use genetic algorithm to find improved solution
		/// @param _padding: related values for UV packing padding (0.001 = 1 padding value in the UV tile)
		/// @param _fidelity: (1-5) how simplified (internally) the patterns are for nesting (1=most simplified, 5=least simplified).
		/// @param _roationAngleOptions: rotation angle(degree) options for packing, default value is 0 degree ex) {0, 180} can rotate up side down
		/// @return If packing succeeds, return true.
		virtual bool UVPacking(bool _bAll, bool _bFixScaleRatio, bool _bAdvancedSearch, float _padding, int _fidelity = 3, const std::vector<int>& _roationAngleOptions = { 0 }) { return false; }

		/// @fn ResetUVTo2DArrangement()
		/// @brief Reset UV islands to the 2D arrangement.
		virtual void ResetUVTo2DArrangement() {};

		/// @fn SetTrimWeight(unsigned int _trimStyleIndex, float _mass)
		/// @brief Change weight of the trim style
		/// @param _trimStyleIndex: trimstyle index to change weight of trim style
		/// @param _mass: value to set weight of trim
		virtual void SetTrimWeight(unsigned int _trimStyleIndex, float _mass) {}

		/// @fn Refresh3DWindow()
		/// @brief Refresh 3D Garment Window
		virtual void Refresh3DWindow() {};
		
		/// @fn SetColorwayColorItem(int _colorwayIndex, int _colorItemIndex, std::string _plmId, std::string _colorName, Marvelous::CloApiRgb _rgb)
		/// @brief Changes the color of the color item in colorway
		/// @param _colorwayIndex: colorway index to change color of color item
		/// @param _colorItemIndex: color item index to change color of colorway
		/// @param _plmId: plm id of color
		/// @param _colorName: name of color
		/// @param _rgb: rgb value of color
		virtual void SetColorwayColorItem(int _colorwayIndex, int _colorItemIndex, std::string _plmId, std::string _colorName, Marvelous::CloApiRgb _rgb) {}

		/// @fn GetColorwayColorItemRGB(int _colorwayIndex, int _colorItemIndex)
		/// @brief Get rgb color value of the color item in colorway
		/// @param _colorwayIndex: colorway index to get rgb color value of color item
		/// @param _colorItemIndex: color item index to get rgb color value color of colorway
		/// @return a rgb color value if the indices are valid, (0, 0, 0) otherwise.
		virtual Marvelous::CloApiRgb GetColorwayColorItemRGB(int _colorwayIndex, int _colorItemIndex) { return Marvelous::CloApiRgb(); }

		/// @fn GetColorwayColorItemPlmId(int _colorwayIndex, int _colorItemIndex)
		/// @brief Get plm id of the color item in colorway
		/// @param _colorwayIndex: colorway index to get plm id of color item
		/// @param _colorItemIndex: color item index to get plm id of colorway
		/// @return a plm id if the indices are valid, empty string otherwise.
		virtual std::string GetColorwayColorItemPlmId(int _colorwayIndex, int _colorItemIndex) { return std::string(); }

		/// @fn GetColorwayColorItemName(int _colorwayIndex, int _colorItemIndex)
		/// @brief Get color name of the color item in colorway
		/// @param _colorwayIndex: colorway index to get color name of color item
		/// @param _colorItemIndex: color item index to get color name of colorway
		/// @return color name if the indices are valid, empty string otherwise.
		virtual std::string GetColorwayColorItemName(int _colorwayIndex, int _colorItemIndex) { return std::string(); }

		/// @fn RemovePlugInFromList(unsigned int _pluginListIndex)
		/// @brief Remove Plug In From PluginList
		/// @param _pluginListIndex: PluginList index to delete the plugin
		virtual void RemovePlugInFromList(unsigned int _pluginListIndex) {}

		/// @fn AddPlugInFromFile(const std::string& _filePath)
		/// @brief Add Plug In From Filepath
		/// @param _filePath: File path to be added plugin.
		virtual void AddPlugInFromFile(const std::string& _filePath) {}
		
		/// @fn UsePlugInFromList(unsigned int _pluginListIndex)
		/// @brief Use Plug In From PluginList
		/// @param _pluginListIndex: PluginList index to use the plugin
		virtual void UsePlugInFromList(unsigned int _pluginListIndex) {}

		/// @fn TerminatePlugInFromList(unsigned int _pluginListIndex)
		/// @brief Terminate Plug In From PluginList
		/// @param _pluginListIndex: PluginList index to use the plugin
		virtual void TerminatePlugInFromList(unsigned int _pluginListIndex) {}

		///	@fn Set3DGarmentRenderingStyle()
		/// @brief Sets garment surface rending style in the 3D window.
		/// @param _surfaceTextureTypeIndex : 0: textured surface, 1: thick textured surface, 2 : monochrome surface, 
		/// 3 : translucent surface, 4 : mesh, 5 : thick textured surface (back), 6 : random color surface. Any other value defaults to 1.
		virtual void Set3DGarmentRenderingStyle(unsigned int _surfaceTextureTypeIndex) {}

		///	@fn Get3DGarmentRenderingStyle()
		/// @brief Gets garment surface rending style currently used in the 3D window.
		/// return: _surfaceTextureTypeIndex : 0: textured surface, 1: thick textured surface, 2 : monochrome surface, 
		/// 3 : translucent surface, 4 : mesh, 5 : thick textured surface (back), 6 : random color surface. Invalid response : -1. 
		virtual int Get3DGarmentRenderingStyle() { return -1; }

		/// @fn GetTotalGraphicItemQuantity()
		/// @brief Returns the total quantity of all graphic items
		/// @return If valid, returns the total quantity; otherwise, -1.
		virtual int GetTotalGraphicItemQuantity() { return -1; }
		
		/// @fn GetGraphicStyleCount()
		/// @brief Returns the number of graphic styles in the current colorway
		/// @return If valid, returns the number of graphic styles; otherwise, -1.
		virtual int GetGraphicStyleCount() { return -1; }

		/// @fn GetGraphicStyleName(int _index)
		/// @brief Returns the name of the graphic style
		/// @param _index: Graphic style index to use the plugin
		/// @return If valid, returns the name; otherwise, an empty string.
		virtual std::string GetGraphicStyleName(int _index) { return std::string(); }

		/// @fn GetGraphicStyleDimensions(int _index)
		/// @brief Returns the dimensions of the graphic style
		/// @param _index: Graphic style index to use the plugin
		/// @return If valid, returns a Dimension object filled with the graphic's dimension information.
		virtual Marvelous::CloApiGraphicDimensions GetGraphicStyleDimensions(int _index) { return Marvelous::CloApiGraphicDimensions(); }

		/// @fn SetGraphicStyleName(int _index, std::string _name)
		/// @brief Sets the name of the graphic style
		/// @param _index: Graphic style index to use the plugin
		/// @param _name: The name to be set for the graphic style
		virtual void SetGraphicStyleName(int _index, std::string _name) {}

		/// @fn SetGraphicStyleDimensions(int _index, float _width, float _height)
		/// @brief Sets the dimensions of the graphic style
		/// @param _index: Graphic style index to use the plugin
		/// @param _width: The width to be set for the graphic style
		/// @param _height: The height to be set for the graphic style
		virtual void SetGraphicStyleDimensions(int _index, float _width, float _height) {}

		/// @fn SetGraphicStyleWidth(int _index, float _width, bool _bOnly)
		/// @brief Sets the width of the graphic style
		/// @param _index: Graphic style index to use the plugin
		/// @param _width: The width to be set for the graphic style
		/// @param _bOnly: If true, the ratio is ignored, if false, the height is set according to the ratio.
		virtual void SetGraphicStyleWidth(int _index, float _width, bool _bOnly) {}

		/// @fn SetGraphicStyleHeight(int _index, float _height, bool _bOnly)
		/// @brief Sets the height of the graphic style
		/// @param _index: Graphic style index to use the plugin
		/// @param _height: The height to be set for the graphic style
		/// @param _bOnly: If true, the ratio is ignored, if false, the height is set according to the ratio.
		virtual void SetGraphicStyleHeight(int _index, float _height, bool _bOnly) {}
		
		///	@fn UpdateCloStyleForPlugIn(QWidget* _pWidget)
		/// @brief Set CLO style-sheet code to QWidget.
		/// @param _pWidget: Root widget pointer of Plug-In Window.
		/// ex> QWidget, QDialog, QFrame, etc...
		virtual void UpdateCloStyleForPlugIn(QWidget* _pWidget) {}

		/// @fn UpdateCloStyleForPlugIn(QWidget* _pWidget, Marvelous::CloWidgetType _type)
		/// @brief Set the style-sheet code corresponding to the CloWidgetType.
		/// @param _pWidget: Widget pointer to set style-sheet code
		/// @param _type: CloWidgetType of widget pointer
		/// ex> TitleBar, ComboBox, Slider, ScrollArea, Dialog, Frame, etc...
		virtual void UpdateCloStyleForPlugIn(QWidget* _pWidget, Marvelous::CloWidgetType _type) {}

		/// @fn GetStyleSheetCodeForWidget(Marvelous::CloWidgetType _type)
		/// @brief Returns the style-sheet code for CloWidgetType
		/// @param _type: CloWidgetType of widget pointer
		/// @return Style-sheet code for CloWidgetType
		virtual std::string GetStyleSheetCodeForWidget(Marvelous::CloWidgetType _type) { return std::string(); }

		/// @fn GetStyleSheetCodeForWidgetW(Marvelous::CloWidgetType _type)
		/// @brief Returns the style-sheet code for CloWidgetType
		/// @param _type: CloWidgetType of widget pointer
		/// @return Style-sheet code for CloWidgetType
		virtual std::wstring GetStyleSheetCodeForWidgetW(Marvelous::CloWidgetType _type) { return std::wstring(); }

		/// @fn CurrentlyThemeInCLO()
		/// @brief Returns the currently Theme information used in CLO
		/// @return currently Theme type
		virtual Marvelous::CloTheme CurrentlyThemeInCLO() { return Marvelous::CloTheme::Theme_Dark; }
		
		///	@fn SetCropStatus()
		/// @brief Set crop status.
		/// @param _bToggle: change crop background as _bToggle
		virtual void SetCropBackground(bool _bToggle) {}

		///	@fn SetZoomCloth()
		/// @brief Zoom cloth
		virtual void SetZoomView() {}

		///	@fn SetViewPoint()
		/// @brief Set View Point
		virtual void SetViewPoint(int _index) {}

		///	@fn GetViewPoint()
		/// @brief Get current view point
		virtual int GetViewPoint() { return -1; }
		
		///	@fn ReDrape3DArrangement()
		/// @brief Redrape all patterns 3D Arrangement
		virtual void ReDrape3DArrangement(const Marvelous::ImportExportOption& _options) {}

		///	@fn SetAPF()
		/// @brief Set APF file path
		/// @param _path: APF file path
		virtual void SetAPF(std::string _path) {}

		///	@fn ABPNetworkAuth()
		/// @brief Regist Alvanon ID and Password
		/// @param id: Alvanon ID 
		/// @param pw: Alvanon Password 
		/// @param _APFpath : APF File path
		virtual void ABPNetworkAuth(std::string id, std::string pw, std::string _APFpath = "") {}


		virtual void SetGraphicBaseColorMapTexture(const std::string _imageFilePath, int _graphicStyleIndex) = 0;
		virtual void SetGraphicBaseColorMapTexture(const std::string _imageFilePath, int _graphicStyleIndex, int _colorwayIndex) = 0;

		/// @fn SetGraphicNormalMapTexture(std::string _imageFilePath)
		/// @brief Sets the texture map for the graphic style (default index = 0)
		/// @param _imageFilePath: The file path to be set for the graphic style
		virtual void SetGraphicNormalMapTexture(const std::string _imageFilePath) {}
        
        /// @fn SetGraphicDisplacementMapTexture(std::string _imageFilePath)
        /// @brief Sets the texture map for the graphic style (default index = 0)
        /// @param _imageFilePath: The file path to be set for the graphic style
		virtual void SetGraphicDisplacementMapTexture(const std::string _imageFilePath) {}
        
        /// @fn SetGraphicOpacityMapTexture(std::string _imageFilePath)
        /// @brief Sets the texture map for the graphic style (default index = 0)
        /// @param _imageFilePath: The file path to be set for the graphic style
		virtual void SetGraphicOpacityMapTexture(const std::string _imageFilePath) {}
        
        /// @fn SetGraphicRoughnessMapTexture(std::string _imageFilePath)
        /// @brief Sets the texture map for the graphic style (default index = 0)
        /// @param _imageFilePath: The file path to be set for the graphic style
		virtual void SetGraphicRoughnessMapTexture(const std::string _imageFilePath) {}
        
        /// @fn SetGraphicMetalnessMapTexture(std::string _imageFilePath)
        /// @brief Sets the texture map for the graphic style (default index = 0)
        /// @param _imageFilePath: The file path to be set for the graphic style
		virtual void SetGraphicMetalnessMapTexture(const std::string _imageFilePath) {}

		/// @fn SetGraphicNormalMapTexture(unsigned int _GraphicIndex, std::string _imageFilePath)
		/// @brief Sets the texture map for the graphic style
		/// @param _index: Graphic style index to use the plugin
		/// @param _imageFilePath: The file path to be set for the graphic style
		virtual void SetGraphicNormalMapTexture(unsigned int _GraphicIndex, const std::string _imageFilePath) {}
        
        /// @fn SetGraphicDisplacementMapTexture(unsigned int _GraphicIndex, std::string _imageFilePath)
        /// @brief Sets the texture map for the graphic style
        /// @param _index: Graphic style index to use the plugin
        /// @param _imageFilePath: The file path to be set for the graphic style
		virtual void SetGraphicDisplacementMapTexture(unsigned int _GraphicIndex, const std::string _imageFilePath) {}
        
        /// @fn SetGraphicNormalMapTexture(unsigned int _GraphicIndex, std::string _imageFilePath)
        /// @brief Sets the texture map for the graphic style
        /// @param _index: Graphic style index to use the plugin
        /// @param _imageFilePath: The file path to be set for the graphic style
		virtual void SetGraphicOpacityMapTexture(unsigned int _GraphicIndex, const std::string _imageFilePath) {}
        
        /// @fn SetGraphicNormalMapTexture(unsigned int _GraphicIndex, std::string _imageFilePath)
        /// @brief Sets the texture map for the graphic style
        /// @param _index: Graphic style index to use the plugin
        /// @param _imageFilePath: The file path to be set for the graphic style
		virtual void SetGraphicRoughnessMapTexture(unsigned int _GraphicIndex, const std::string _imageFilePath) {}
        
        /// @fn SetGraphicNormalMapTexture(unsigned int _GraphicIndex, std::string _imageFilePath)
        /// @brief Sets the texture map for the graphic style
        /// @param _index: Graphic style index to use the plugin
        /// @param _imageFilePath: The file path to be set for the graphic style
		virtual void SetGraphicMetalnessMapTexture(unsigned int _GraphicIndex, const std::string _imageFilePath) {}
        
        /// @fn SetGraphicNormalMapTexture()
        /// @brief Gets the texture map for the graphic style
        /// @return _imageFilePath: The file path of the graphic style
		virtual std::string GetGraphicNormalMapTexture() { return std::string(); }
        
        /// @fn GetGraphicDisplacementMapTexture()
        /// @brief Gets the texture map for the graphic style
        /// @return _imageFilePath: The file path of the graphic style
		virtual std::string GetGraphicDisplacementMapTexture() { return std::string(); }
        
        /// @fn GetGraphicOpacityMapTexture()
        /// @brief Gets the texture map for the graphic style
        /// @return _imageFilePath: The file path of the graphic style
		virtual std::string GetGraphicOpacityMapTexture() { return std::string(); }
        
        /// @fn GetGraphicRoughnessMapTexture()
        /// @brief Gets the texture map for the graphic style
        /// @return _imageFilePath: The file path of the graphic style
		virtual std::string GetGraphicRoughnessMapTexture() { return std::string(); }
        
        /// @fn GetGraphicMetalnessMapTexture()
        /// @brief Gets the texture map for the graphic style
        /// @return _imageFilePath: The file path of the graphic style
		virtual std::string GetGraphicMetalnessMapTexture() { return std::string(); }

        /// @fn GetGraphicNormalMapTexture(unsigned int GraphicIndex)
        /// @brief Gets the texture map for the graphic style
        /// @param GraphicIndex: Graphic style index to use the plugin
        /// @return imageFilePath: The file path to be set for the graphic style
		virtual std::string GetGraphicNormalMapTexture(unsigned int _GraphicIndex) { return std::string(); }
        
        /// @fn GetGraphicDisplacementMapTexture(unsigned int GraphicIndex)
        /// @brief Gets the texture map for the graphic style
        /// @param _GraphicIndex: Graphic style index to use the plugin
        /// @return imageFilePath: The file path to be set for the graphic style
		virtual std::string GetGraphicDisplacementMapTexture(unsigned int _GraphicIndex) { return std::string(); }
        
        /// @fn GetGraphicOpacityMapTexture(unsigned int GraphicIndex)
        /// @brief Gets the texture map for the graphic style
        /// @param _GraphicIndex: Graphic style index to use the plugin
        /// @return imageFilePath: The file path to be set for the graphic style
		virtual std::string GetGraphicOpacityMapTexture(unsigned int _GraphicIndex) { return std::string(); }
        
        /// @fn GetGraphicRoughnessMapTexture(unsigned int GraphicIndex)
        /// @brief Gets the texture map for the graphic style
        /// @param _GraphicIndex: Graphic style index to use the plugin
        /// @return imageFilePath: The file path to be set for the graphic style
		virtual std::string GetGraphicRoughnessMapTexture(unsigned int _GraphicIndex) { return std::string(); }
        
        /// @fn GetGraphicMetalnessMapTexture(unsigned int GraphicIndex)
        /// @brief Gets the texture map for the graphic style
        /// @param _GraphicIndex: Graphic style index to use the plugin
        /// @return imageFilePath: The file path to be set for the graphic style
		virtual std::string GetGraphicMetalnessMapTexture(unsigned int _GraphicIndex) { return std::string(); }

        /// @fn GetGraphicStyleColor()
        /// @brief Get a base color of PBR material in a Graphic.  (default index = 0)
        /// @return RGBA float tuple
		virtual std::tuple<float, float, float, float> GetGraphicStyleColor() = 0;

        /*
        Set a base color of PBR material in a Graphic using given RGBA values.
        @return if it succeeds, return true.
        */
        /// @fn SetGraphicStyleColor(float r, float g, float b, float a)
        /// @brief Set a base color of PBR material in a Graphic using given RGBA values. (default index = 0)
        /// @parm r : To be set as red value
        /// @parm g : To be set as green value
        /// @parm b : To be set as blue value
        /// @parm a : To be set as alpha value
        /// @return if it succeeds, return true.
		virtual bool SetGraphicStyleColor(float r, float g, float b, float a) = 0;
		/*
		Set a base color of PBR material in a Graphic using given RGBA values.
		@return if it succeeds, return true.
		*/
		
		/// @fn AutoHang(const std::string _garmentPath, const std::string _hangerPath, const unsigned int _hangerType, const Marvelous::ImportExportOption& _hangerImportOption)
		/// @brief Auto-hang a garment to a hanger.
		/// @parm _garmentPath : path to a garment file. .zprj or .zpac is accepted. If .zprj is given, only the garment will be loaded. 
		/// @parm _hangerPath : path to a hanger file. .obj or .avt is accepted.
		/// @parm _hangerType : type of the _hangerFIlePath hanger. 0 = top hanger, 1 = bottom hanger
		/// @parm _hangerImportOption : if .obj is given for _hangerFIlePath, specify the import option. Especially, specify the scale. For example, if the file is in cm, scale should be 10.
		/// @return if it succeeds, return true.
		virtual bool AutoHang(const std::string _garmentPath, const std::string _hangerPath, const unsigned int _hangerType, const Marvelous::ImportExportOption& _hangerImportOption) { return false; };

        /// @fn UpdatePropertyWindow()
        /// @brief Update the PropertyWindow 
        virtual void UpdatePropertyWindow() {}
        
        /// @fn FitAllUV(int x_coordinate, int y_coordinate, bool _bIsRelative, float _marginValue)
        /// @brief Fitting patterns in UV editor to given coordinates
        /// @param x_coordinate: x coordinate value
        /// @param y_coordinate: y coordinate value
        /// @param _bIsRelative: true for relative, false for Absolute
        /// @param _marginValue: margin value
        virtual void FitAllUV(int x_coordinate, int y_coordinate, bool _bIsRelative, float _marginValue) {}

		/// @fn		GetGraphicStyleColor(int _styleIndex)
		/// @brief	Get base color of PBR material from Graphic Style
		/// @param	_styleIndex: Graphic Style index on Object Browser to get Color
		/// @return The color value in CloAgiRgba.
		virtual Marvelous::CloApiRgba GetGraphicStyleColor(int _styleIndex) = 0;
		
		/// @fn		GetGraphicStyleColor(int _styleIndex, int _colorwayIndex)
		/// @brief	Get base color of PBR material from Graphic Style
		/// @param	_styleIndex: Graphic Style index on [_colorwayIndex]th Colorway to get Color
		///			_colorwayIndex: Colorway Index to get Style
		/// @return The color value in CloAgiRgba.
		virtual Marvelous::CloApiRgba GetGraphicStyleColor(int _styleIndex, int _colorwayIndex) = 0;

		/// @fn		SetGraphicStyleColor(int _styleIndex, int _r, int _g, int _b, int _a)
		/// @brief	Set base color of PBR material in a Graphic Style using given RGBA values
		/// @param	_styleIndex: Graphic Style index on Object Browser to set Color
		///	@param	_r: Red channel 
		///	@param	_g: Green channel
		///	@param	_b: Blue channel
		///	@param	_a: Alpha channel
		virtual void SetGraphicStyleColor(int _styleIndex, int _r, int _g, int _b, int _a) = 0;

		virtual unsigned int GetItemsCountsinColorway(int _colorwayIndex) = 0;
		/// @fn		SetGraphicStyleColor(int _styleIndex, int _colorwayIndex, int _r, int _g, int _b, int _a)
		/// @brief	Set base color of PBR material in a Graphic Style using given RGBA values
		/// @param	_styleIndex: Graphic Style index on [_colorwayIndex]th Colorway to set Color
		/// @param	_colorwayIndex: Colorway Index to get Style
		///	@param	_r: Red channel 
		///	@param	_g: Green channel
		///	@param	_b: Blue channel
		///	@param	_a: Alpha channel
		virtual void SetGraphicStyleColor(int _styleIndex, int _colorwayIndex, int _r, int _g, int _b, int _a) = 0;
		
		/// @fn		GetButtonHeadStyleColor(int _styleIndex, int _meshIndex)
		/// @brief	Get base color of PBR material from ButtonHead Style
		/// @param	_styleIndex: ButtonHead Style index on Object Browser to get Color
		/// @param	_meshIndex: Mesh index to get Color
		///				0: ButtonHead
		///				1: Thread
		/// @return The color value in CloAgiRgba.
		virtual Marvelous::CloApiRgba GetButtonHeadStyleColor(int _styleIndex, int _meshIndex) = 0;
		
		/// @fn		GetButtonHeadStyleColor(int _styleIndex, int _colorwayIndex, int _meshIndex)
		/// @brief	Get base color of PBR material from ButtonHead Style
		/// @param	_styleIndex: ButtonHead Style index [_colorwayIndex]th Colorway to get Color
		/// @param	_colorwayIndex: Colorway Index to get Style
		/// @param	_meshIndex: Mesh index to get Color
		///				0: ButtonHead
		///				1: Thread
		/// @return The color value in CloAgiRgba.
		virtual Marvelous::CloApiRgba GetButtonHeadStyleColor(int _styleIndex, int _colorwayIndex, int _meshIndex) = 0;

		/// @fn		SetButtonHeadStyleColor(int _styleIndex, int _meshIndex, int _r, int _g, int _b, int _a)
		/// @brief	Set base color of PBR material in a ButtonHead Style using given RGBA values
		/// @param	_styleIndex: ButtonHead Style index on Object Browser to set Color
		/// @param	_meshIndex: Mesh index to get Color
		///				-1: All
		///				0: ButtonHead
		///				1: Thread
		///	@param	_r: Red channel 
		///	@param	_g: Green channel
		///	@param	_b: Blue channel
		///	@param	_a: Alpha channel
		virtual void SetButtonHeadStyleColor(int _styleIndex, int _meshIndex, int _r, int _g, int _b, int _a) = 0;

		/// @fn		SetButtonHeadStyleColor(int _styleIndex, int _colorwayIndex, int _meshIndex, int _r, int _g, int _b, int _a)
		/// @brief	Set base color of PBR material in a ButtonHead Style using given RGBA values
		/// @param	_styleIndex: ButtonHead Style index [_colorwayIndex]th Colorway to set Color
		///			_colorwayIndex: Colorway Index to get Style
		/// @param	_meshIndex: Mesh index to get Color
		///				-1: All
		///				0: ButtonHead
		///				1: Thread
		///	@param	_r: Red channel 
		///	@param	_g: Green channel
		///	@param	_b: Blue channel
		///	@param	_a: Alpha channel
		virtual void SetButtonHeadStyleColor(int _styleIndex, int _colorwayIndex, int _meshIndex, int _r, int _g, int _b, int _a) = 0;
		
		/// @fn		GetButtonHoleStyleColor(int _styleIndex)
		/// @brief	Get base color of PBR material from ButtonHole Style
		/// @param	_styleIndex: ButtonHole Style index on Object Browser to get Color	
		/// @return The color value in CloAgiRgba.
		virtual Marvelous::CloApiRgba GetButtonHoleStyleColor(int _styleIndex) = 0;
		
		/// @fn		GetButtonHoleStyleColor(int _styleIndex, int _colorwayIndex)
		/// @brief	Get base color of PBR material from ButtonHole Style
		/// @param	_styleIndex: ButtonHole Style index [_colorwayIndex]th Colorway to get Color
		/// @param	_colorwayIndex: Colorway Index to get Style
		/// @return The color value in CloAgiRgba.
		virtual Marvelous::CloApiRgba GetButtonHoleStyleColor(int _styleIndex, int _colorwayIndex) = 0;

		/// @fn		SetButtonHoleStyleColor(int _styleIndex, int _r, int _g, int _b, int _a)
		/// @brief	Set base color of PBR material in a ButtonHole Style using given RGBA values
		/// @param	_styleIndex: ButtonHole Style index on Object Browser to set Color
		///	@param	_r: Red channel 
		///	@param	_g: Green channel
		///	@param	_b: Blue channel
		///	@param	_a: Alpha channel
		virtual void SetButtonHoleStyleColor(int _styleIndex, int _r, int _g, int _b, int _a) = 0;

		/// @fn		SetButtonHoleStyleColor(int _styleIndex, int _colorwayIndex, int _r, int _g, int _b, int _a)
		/// @brief	Set base color of PBR material in a ButtonHole Style using given RGBA values
		/// @param	_styleIndex: ButtonHole Style index [_colorwayIndex]th Colorway to set Color
		/// @param	_colorwayIndex: Colorway Index to get Style
		///	@param	_r: Red channel 
		///	@param	_g: Green channel
		///	@param	_b: Blue channel
		///	@param	_a: Alpha channel
		virtual void SetButtonHoleStyleColor(int _styleIndex, int _colorwayIndex, int _r, int _g, int _b, int _a) = 0;
		
		/// @fn		GetTrimStyleColor(int _styleIndex, int _meshIndex)
		/// @brief	Get base color of PBR material from Trim Style
		/// @param	_styleIndex: Trim Style index on Object Browser to get Color
		/// @param	_meshIndex: Mesh index to get Color
		///				0~: [_meshIndex]th mesh
		/// @return The color value in CloAgiRgba.
		virtual Marvelous::CloApiRgba GetTrimStyleColor(int _styleIndex, int _meshIndex) = 0;
		
		/// @fn		GetTrimStyleColor(int _styleIndex, int _colorwayIndex, int _meshIndex)
		/// @brief	Get base color of PBR material from Trim Style
		/// @param	_styleIndex: Trim Style index [_colorwayIndex]th Colorway to get Color
		/// @param	_colorwayIndex: Colorway Index to get Style
		/// @param	_meshIndex: Mesh index to get Color
		///				0~: [_meshIndex]th mesh
		/// @return The color value in CloAgiRgba.
		virtual Marvelous::CloApiRgba GetTrimStyleColor(int _styleIndex, int _colorwayIndex, int _meshIndex) = 0;

		/// @fn		SetTrimStyleColor(int _styleIndex, int _meshIndex, int _r, int _g, int _b, int _a)
		/// @brief	Set base color of PBR material in a Trim Style using given RGBA values
		/// @param	_styleIndex: Trim Style index on Object Browser to set Color
		/// @param	_meshIndex: Mesh index to get Color
		///				-1: All
		///				0~: [_meshIndex]th mesh
		///	@param	_r: Red channel 
		///	@param	_g: Green channel
		///	@param	_b: Blue channel
		///	@param	_a: Alpha channel
		virtual void SetTrimStyleColor(int _styleIndex, int _meshIndex, int _r, int _g, int _b, int _a) = 0;

		/// @fn		SetTrimStyleColor(int _styleIndex, int _colorwayIndex, int _meshIndex, int _r, int _g, int _b, int _a)
		/// @brief	Set base color of PBR material in a Trim Style using given RGBA values
		/// @param	_styleIndex: Trim Style index [_colorwayIndex]th Colorway to set Color
		/// @param	_colorwayIndex: Colorway Index to get Style
		/// @param	_meshIndex: Mesh index to get Color
		///				-1: All
		///				0~: [_meshIndex]th mesh
		///	@param	_r: Red channel 
		///	@param	_g: Green channel
		///	@param	_b: Blue channel
		///	@param	_a: Alpha channel
		virtual void SetTrimStyleColor(int _styleIndex, int _colorwayIndex, int _meshIndex, int _r, int _g, int _b, int _a) = 0;

		/// @fn		GetTopStitchOpacity(int _styleIndex)
		/// @brief	Get opacity of PBR material from TopStitch
		/// @param	_styleIndex: TopStitch index on Object Browser
		/// @param	_lineIndex: Stitch index to get opacity
		///				0~: [_lineIndex]th Stitch
		/// @return Opacity value
		virtual int GetTopStitchOpacity(int _styleIndex, int _lineIndex) = 0;

		/// @fn		GetTopStitchOpacity(int _styleIndex, int _colorwayIndex)
		/// @brief	Get opacity of PBR material from TopStitch
		/// @param	_styleIndex: TopStitch index [_colorwayIndex]th Colorway
		/// @param	_colorwayIndex: Colorway Index to get Style
		/// @param	_lineIndex: Stitch index to get opacity
		///				0~: [_lineIndex]th Stitch
		/// @return Opacity value
		virtual int GetTopStitchOpacity(int _styleIndex, int _colorwayIndex, int _lineIndex) = 0;

		/// @fn		SetTopStitchOpacity(int _styleIndex, int _opacityValue)
		/// @brief	Set opacity of PBR material in a TopStitch
		/// @param	_styleIndex: TopStitch index on Object Browser
		/// @param	_lineIndex: Stitch index to set opacity
		///				0~: [_lineIndex]th Stitch
		///	@param	_opacityValue: opacity
		virtual void SetTopStitchOpacity(int _styleIndex, int _lineIndex, int _opacityValue) = 0;

		/// @fn		SetTopStitchOpacity(int _styleIndex, int _colorwayIndex, int _opacityValue)
		/// @brief	Set opacity of PBR material in a TopStitch
		/// @param	_styleIndex: TopStitch index [_colorwayIndex]th Colorway
		/// @param	_colorwayIndex: Colorway Index to get Style
		/// @param	_lineIndex: Stitch index to set opacity
		///				0~: [_lineIndex]th Stitch
		///	@param	_opacityValue: opacity
		virtual void SetTopStitchOpacity(int _styleIndex, int _colorwayIndex, int _lineIndex, int _opacityValue) = 0;

		/// @fn		GetTopStitchColor(int _styleIndex, int _lineIndex)
		/// @brief	Get base color of PBR material from TopStitch Style
		/// @param	_styleIndex: TopStitch Style index on Object Browser to get Color
		/// @param	_lineIndex: Stitch index to get Color
		///				0~: [_lineIndex]th Stitch
		/// @return The color value in CloAgiRgba.
		virtual Marvelous::CloApiRgba GetTopStitchColor(int _styleIndex, int _lineIndex) = 0;

		/// @fn		GetTopStitchColor(int _styleIndex, int _colorwayIndex, int _lineIndex)
		/// @brief	Get base color of PBR material from TopStitch Style
		/// @param	_styleIndex: TopStitch Style index [_colorwayIndex]th Colorway to get Color
		/// @param	_colorwayIndex: Colorway Index to get Style
		/// @param	_lineIndex: Stitch index to get Color
		///				0~: [_lineIndex]th Stitch
		/// @return The color value in CloAgiRgba.
		virtual Marvelous::CloApiRgba GetTopStitchColor(int _styleIndex, int _colorwayIndex, int _lineIndex) = 0;

		/// @fn		SetTopStitchColor(int _styleIndex, int _lineIndex, int _r, int _g, int _b, int _a)
		/// @brief	Set base color of PBR material in a TopStitch Style using given RGBA values
		/// @param	_styleIndex: TopStitch Style index on Object Browser to set Color
		/// @param	_lineIndex: Stitch index to set Color
		///				-1: All
		///				0~: [_lineIndex]th Stitch
		///	@param	_r: Red channel 
		///	@param	_g: Green channel
		///	@param	_b: Blue channel
		///	@param	_a: Alpha channel
		virtual void SetTopStitchColor(int _styleIndex, int _lineIndex, int _r, int _g, int _b, int _a) = 0;

		/// @fn		SetTopStitchColor(int _styleIndex, int _colorwayIndex, int _lineIndex, int _r, int _g, int _b, int _a)
		/// @brief	Set base color of PBR material in a TopStitch Style using given RGBA values
		/// @param	_styleIndex: TopStitch Style index [_colorwayIndex]th Colorway to set Color
		/// @param	_colorwayIndex: Colorway Index to get Style
		/// @param	_lineIndex: Stitch index to set Color
		///				-1: All
		///				0~: [_lineIndex]th Stitch
		///	@param	_r: Red channel 
		///	@param	_g: Green channel
		///	@param	_b: Blue channel
		///	@param	_a: Alpha channel
		virtual void SetTopStitchColor(int _styleIndex, int _colorwayIndex, int _lineIndex, int _r, int _g, int _b, int _a) = 0;

		/// @fn		SetGraphicStyleToGraphic(int _styleIndex, int _colorwayIndex, int _graphicIndex, int _assignOption)
		/// @brief	Set GraphicStyle of Graphic
		/// @param	_styleIndex: Graphic Style index on the Object Browser
		/// @param	_graphicIndex: Graphic index to change style
		/// @param _assignOption:
		///			_assignOption = 1 (Current Colorway): An item is only applied to the current colorway, while the other colorways retain the existing items.
		///			_assignOption = 2 (All Colorways unlinked Materials): Each colorway is applied its own materials, such as textures and colors.
		///			_assignOption = 3 (All Colorways linked): All colorways share the same item with linked properties, allowing any changes to be applied across all colorways simultaneously
		virtual void SetGraphicStyleToGraphic(int _styleIndex, int _graphicIndex, int _assignOption) = 0;

		/// @fn		SetGraphicStyleToGraphic(int _styleIndex, int _colorwayIndex, int _graphicIndex, int _assignOption)
		/// @brief	Set GraphicStyle of Graphic
		/// @param	_styleIndex: Graphic Style index on the [_colorwayIndex]th Colorway
		/// @param	_colorwayIndex: Colorway index to get Graphic Style
		/// @param	_graphicIndex: Graphic index to change Graphic Style
		/// @param _assignOption:
		///			_assignOption = 1 (Current Colorway): An item is only applied to the current colorway, while the other colorways retain the existing items.
		///			_assignOption = 2 (All Colorways unlinked Materials): Each colorway is applied its own materials, such as textures and colors.
		///			_assignOption = 3 (All Colorways linked): All colorways share the same item with linked properties, allowing any changes to be applied across all colorways simultaneously
		virtual void SetGraphicStyleToGraphic(int _styleIndex, int _colorwayIndex, int _graphicIndex, int _assignOption) = 0;

		/// @fn		GetSimulationQuality()
		/// @return The Simulation type(0: Normal(Default), 1: Animation(Stable), 2: Fitting(Accurate Fabric)) and simulation mode (0: CPU, 1: FAST(GPU))
		virtual std::pair<int, int> GetSimulationQuality() = 0;

		/// @fn		CheckZPRJForUnsavedChanges()
		/// @return Returns true if the file has been modified.
		virtual bool CheckZPRJForUnsavedChanges() = 0;
	};
}
