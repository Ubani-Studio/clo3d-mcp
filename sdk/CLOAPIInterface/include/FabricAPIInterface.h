#pragma once

#include <vector>

#include "DefineDllForWin.h"

namespace CLOAPI
{
	class MV_CLO_INTERFACE_API FabricAPIInterface {
	public:
		/// @fn GetFabricCount(bool _bCurrentColorway = true)
		/// @brief Get the number of fabrics
		/// @param _bCurrentColorway: If this value is true, counting only Fabrics that are used in current Colorway (Default: true)
		/// @return the number of fabric in object browser for the current project.
		virtual unsigned int GetFabricCount(bool _bCurrentColorway = true) { return 0; }

		/// @fn GetFabricCount(int _colorwayIndex)
		/// @brief Get the number of fabrics
		/// @param _colorwayIndex:
		/// _colorwayIndex = -2 : It counts all Fabrics for all the Colorways
		/// _colorwayIndex = -1 : It counts the Fabrics of the current Colorway selected
		///	_colorwayIndex >= 0 : It counts the Fabrics of the _colorwayIndex index given by parameter
		/// @return the number of fabric in object browser for the current project.
		virtual unsigned int GetFabricCount(int _colorwayIndex) { return 0; }

		/// @fn GetCurrentFabricIndex()
		/// @brief Get the index of the selected Fabric
		/// @return the index of selected fabric in object browser for the current project..
		virtual unsigned int GetCurrentFabricIndex() { return 0; }

		/// @fn ExportZFab()
		/// @brief Export ZFab which cotains the fabric data selected in the object browser
		/// @return Output file path;output files will be created in CLO temporary folder. If an error occurs, return empty string.
		virtual std::string ExportZFab() { return ""; }

		/// @fn ExportZFab(const string& filePath)
		/// @brief Export Zfab which cotains the fabric data selected in the object browser
		/// @param filePath: output file path
		/// @return Output file path. If an error occurs, return empty string. 
		virtual std::string ExportZFab(const std::string& filePath) { return ""; }

		/// @fn ExportZFab(const string& filePath, const int& index)
		/// @brief Export ZFab which cotains the fabric data in the index of the object browser
		/// @param filePath: output file path
		/// @param index: target fabric index on the object browser to export
		/// @return Output file path. If an error occurs, return empty string. 
		virtual std::string ExportZFab(const std::string& filePath, const int& index) { return ""; }

		/// @fn ExportZFab(const string& _filePath, const int& _colorwayIndex, const int& _fabricIndex)
		/// @brief Export ZFab which cotains the fabric data in the index of the object browser
		/// @param _filePath: output file path
		/// @param _colorwayIndex: target Colorway index
		/// @param _fabricIndex: target Fabric index on the Colorway Editor to export
		/// @return Output file path. If an error occurs, return empty string. 
		virtual std::string ExportZFab(const std::string& _filePath, const int& _colorwayIndex, const int& _fabricIndex) { return ""; }

		/// @fn ExportZFabW(const wstring& filePath, const int& index)
		/// @brief Export ZFab file which cotains the fabric data in the index of the object browser
		/// @param filePath: output file path
		/// @param index: target fabric index on the object browser to export
		/// @return Output file path. If an error occurs, return empty string. 
		virtual std::wstring ExportZFabW(const std::wstring& filePath, const int& index) { return L""; }

		/// @fn ExportZFabW(const wstring& _filePath, const int& _colorwayIndex, const int& _fabricIndex)
		/// @brief Export ZFab file which cotains the fabric data in the index of the object browser
		/// @param _filePath: output file path
		/// @param _colorwayIndex: target Colorway index
		/// @param _fabricIndex: target Fabric index on the Colorway Editor to export
		/// @return Output file path. If an error occurs, return empty string. 
		virtual std::wstring ExportZFabW(const std::wstring& _filePath, const int& _colorwayIndex, const int& _fabricIndex) { return L""; }

		/// @fn ExportFabric(const string& filePath)
		/// @brief Export the selected fabric to the file(.jfab or .zfab)		
		/// @param filePath: output file path		
		/// @return Output file path. If an error occurs, return empty string.
		virtual std::string ExportFabric(const std::string& filePath) { return ""; }

		/// @fn ExportFabric(const string& filePath, const int& index)
		/// @brief Export the selected fabric to the file(.jfab or .zfab)		
		/// @param filePath: output file path		
		/// @param index: target fabric index on the object browser to export
		/// @return Output file path. If an error occurs, return empty string.
		virtual std::string ExportFabric(const std::string& filePath, const int& index) { return ""; }

		/// @fn ExportFabric(const string& _filePath, const int& _colorwayIndex, const int& _fabricIndex)
		/// @brief Export the selected fabric to the file(.jfab or .zfab)		
		/// @param _filePath: output file path
		/// @param _colorwayIndex: target Colorway index
		/// @param _fabricIndex: target Fabric index on the Colorway Editor to export
		/// @return Output file path. If an error occurs, return empty string.
		virtual std::string ExportFabric(const std::string& _filePath, const int& _colorwayIndex, const int& _fabricIndex) { return ""; }

		/// @fn ExportFabricW(const wstring& filePath, const int& index)
		/// @brief Export the selected fabric to the file(.jfab or .zfab)		
		/// @param filePath: output file path		
		/// @param index: target fabric index on the object browser to export
		/// @return Output file path. If an error occurs, return empty string.
		virtual std::wstring ExportFabricW(const std::wstring& filePath, const int& index) { return L""; }

		/// @fn ExportFabricW(const wstring& _filePath, const int& _colorwayIndex, const int& _fabricIndex)
		/// @brief Export the selected fabric to the file(.jfab or .zfab)		
		/// @param _filePath: output file path
		/// @param _colorwayIndex: target Colorway index
		/// @param _fabricIndex: target Fabric index on the Colorway Editor to export
		/// @return Output file path. If an error occurs, return empty string.
		virtual std::wstring ExportFabricW(const std::wstring& _filePath, const int& _colorwayIndex, const int& _fabricIndex) { return L""; }

		/// @fn AddFabric(const string& inputFilePath)
		/// @brief Add Fabric into the Object Browser
		/// @param inputFilePath: 'zfab'(clo file) path or 'jfab'(json) path
		/// @return the index of added fabric in object browser for the current project. .
		virtual unsigned int AddFabric(const std::string& inputFilePath) { return 0; }

		/// @fn ReplaceFabric(const int& _fabricIndex, const std::string& _inputFilePath)
		/// @brief Replaces Fabric with provided index in the Object Browser
		/// @param fabricIndex: fabric index from the object browser
		/// @param inputFilePath: 'zfab'(clo file) path.
		/// @return if it succeeds, return true.
		virtual bool ReplaceFabric(const int& _fabricIndex, const std::string& _inputFilePath) { return ""; }

		/// @fn AddFabricW(const wstring& inputFilePath)
		/// @brief Add Fabric into the Object Browser
		/// @param inputFilePath: 'zfab'(clo file) path or 'jfab'(json) path
		/// @return the index of added fabric in object browser for the current project. .
		virtual unsigned int AddFabricW(const std::wstring& inputFilePath) { return 0; }

		/// @fn GetFirstFabricTextureName()
		/// @brief Get the main image file name for the first fabric in the object browser
		/// @return the name of the image file name without folder path and extension.
		virtual std::string GetFirstFabricTextureName() { return ""; }

		/// @fn GetFirstFabricTextureNameW()
		/// @brief Get the main image file name for the first fabric in the object browser
		/// @return the name of the image file name without folder path and extension.
		virtual std::wstring GetFirstFabricTextureNameW() { return L""; }

		/// @fn GetFirstFabricTextureName(unsigned int _colorwayIndex)
		/// @brief Get the main image file name for the first fabric in the object browser for a colorway index
		/// @param _colorwayIndex: colorway index
		/// @return the name of the image file name without folder path and extension.
		virtual std::string GetFirstFabricTextureName(unsigned int _colorwayIndex) { return ""; }

		/// @fn GetFirstFabricTextureNameW(unsigned int _colorwayIndex)
		/// @brief Get the main image file name for the first fabric in the object browser for a colorway index
		/// @param _colorwayIndex: colorway index
		/// @return the name of the image file name without folder path and extension.
		virtual std::wstring GetFirstFabricTextureNameW(unsigned int _colorwayIndex) { return L""; }

		/// @fn ChangeFabricWithJson(unsigned int fabricIndex, const string& inputJsonFilePath)
		/// @brief Overwrite all the properties of a fabric with json file(.jfab) 
		/// @param fabricIndex: target fabric index in the object browser
		/// @param inputJsonFilePath: the jfab file path to load on to the fabric in the fabricIndex
		/// @return if it succeeds, return true.
		virtual bool ChangeFabricWithJson(unsigned int fabricIndex, const std::string& inputJsonFilePath) { return false; }

		/// @fn ChangeFabricWithJsonW(unsigned int fabricIndex, const wstring& inputJsonFilePath)
		/// @brief Overwrite all the properties of a fabric with json file(.jfab) 
		/// @param fabricIndex: target fabric index in the object browser
		/// @param inputJsonFilePath: the jfab file path to load on to the fabric in the fabricIndex
		/// @return if it succeeds, return true.
		virtual bool ChangeFabricWithJsonW(unsigned int fabricIndex, const std::wstring& inputJsonFilePath) { return false; }

		/// @fn ChangeFabricWithJson(unsigned int _colorwayIndex, unsigned int _fabricIndex, const std::string& _inputJsonFilePath)
		/// @brief Overwrite all the properties of a fabric with json file(.jfab) 
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @param _fabricIndex: target fabric index in the Colorway Editor
		/// @param _inputJsonFilePath: the jfab file path to load on to the fabric in the fabricIndex
		/// @return if it succeeds, return true.
		virtual bool ChangeFabricWithJson(unsigned int _colorwayIndex, unsigned int _fabricIndex, const std::string& _inputJsonFilePath) { return false; }

		/// @fn ChangeFabricWithJsonW(unsigned int _colorwayIndex, unsigned int _fabricIndex, const std::wstring& _inputJsonFilePath)
		/// @brief Overwrite all the properties of a fabric with json file(.jfab) 
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @param _fabricIndex: target fabric index in the Colorway Editor
		/// @param _inputJsonFilePath: the jfab file path to load on to the fabric in the fabricIndex
		/// @return if it succeeds, return true.
		virtual bool ChangeFabricWithJsonW(unsigned int _colorwayIndex, unsigned int _fabricIndex, const std::wstring& _inputJsonFilePath) { return false; }

		/// @fn AssignFabricToPattern(unsigned int _fabricIndex, unsigned int _patternIndex)
		/// @brief Assign a fabric to a pattern. 
		/// @param _fabricIndex: the source fabric index in the object browser to apply to the target pattern
		/// @param _patternIndex: the tagert pattern index to apply the fabric in the fabricIndex
		/// @param _assignOption:
		///			_assignOption = 1 (Current Colorway): An item is only applied to the current colorway, while the other colorways retain the existing items.
		///			_assignOption = 2 (All Colorways unlinked Materials): Each colorway is applied its own materials, such as textures and colors.
		///			_assignOption = 3 (All Colorways linked): All colorways share the same item with linked properties, allowing any changes to be applied across all colorways simultaneously
		/// @return if it succeeds, return true.
		virtual bool AssignFabricToPattern(unsigned int _fabricIndex, unsigned int _patternIndex, int _assignOption) { return false; }

		/// @fn AssignFabricToPattern(unsigned int _colorwayIndex, unsigned int _fabricIndex, unsigned int _patternIndex)
		/// @brief Assign a fabric to a pattern. 
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @param _fabricIndex: the source fabric index in the Colorway Editor to apply to the target pattern
		/// @param _patternIndex: the tagert pattern index to apply the fabric in the fabricIndex
		/// @param _assignOption:
		///			_assignOption = 1 (Current Colorway): An item is only applied to the current colorway, while the other colorways retain the existing items.
		///			_assignOption = 2 (All Colorways unlinked Materials): Each colorway is applied its own materials, such as textures and colors.
		///			_assignOption = 3 (All Colorways linked): All colorways share the same item with linked properties, allowing any changes to be applied across all colorways simultaneously
		/// @return if it succeeds, return true.
		virtual bool AssignFabricToPattern(unsigned int _colorwayIndex, unsigned int _fabricIndex, unsigned int _patternIndex, int _assignOption) { return false; }

		/// @fn ImportXtexFile(unsigned int fabricIndex, const string& xTexFilePath)		
		/// @brief Import xtex to a fabric. Overwrite all the values described in the xtex file into the Fabric
		/// @parm fabricIndex: the target fabric index in the object browser to import xTex file onto
		/// @parm xTexFilePath: the xTex file path to import
		/// @return if it succeeds, return true.
		virtual bool ImportXtexFile(unsigned int fabricIndex, const std::string& xTexFilePath) { return false; }

		/// @fn ImportXtexFileW(unsigned int fabricIndex, const wstring& xTexFilePath)
		/// @brief Import xtex to a fabric. Overwrite all the values described in the xtex file into the Fabric
		/// @parm fabricIndex: the target fabric index in the object browser to import xTex file onto
		/// @parm xTexFilePath: the xTex file path to import
		/// @return if it succeeds, return true.
		virtual bool ImportXtexFileW(unsigned int fabricIndex, const std::wstring& xTexFilePath) { return false; }

		/// @fn ImportXtexFile(unsigned int _colorwayIndex, unsigned int _fabricIndex, const std::string& _xTexFilePath)		
		/// @brief Import xtex to a fabric. Overwrite all the values described in the xtex file into the Fabric
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @parm _fabricIndex: the target fabric index in the Colorway Editor to import xTex file onto
		/// @parm _xTexFilePath: the xTex file path to import
		/// @return if it succeeds, return true.
		virtual bool ImportXtexFile(unsigned int _colorwayIndex, unsigned int _fabricIndex, const std::string& _xTexFilePath) { return false; }

		/// @fn ImportXtexFileW(unsigned int _colorwayIndex, unsigned int _fabricIndex, const std::wstring& _xTexFilePath)
		/// @brief Import xtex to a fabric. Overwrite all the values described in the xtex file into the Fabric
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @parm _fabricIndex: the target fabric index in the Colorway Editor to import xTex file onto
		/// @parm _xTexFilePath: the xTex file path to import
		/// @return if it succeeds, return true.
		virtual bool ImportXtexFileW(unsigned int _colorwayIndex, unsigned int _fabricIndex, const std::wstring& _xTexFilePath) { return false; }

		/// @fn ApplyXtexFile(unsigned int fabricIndex, const string& xTexFilePath)
		/// @brief Apply xtex to a fabric. Only set the values described in the xtex file into the Fabric.
		/// @parm fabricIndex: the target fabric index in the object browser to apply xTex file onto
		/// @parm xTexFilePath: the xTex file path to apply
		/// @return if it succeeds, return true.
		virtual bool ApplyXtexFile(unsigned int fabricIndex, const std::string& xTexFilePath) { return false; }

		/// @fn ApplyXtexFile(unsigned int fabricIndex, const string& xTexFilePath)
		/// @brief Apply xtex to a fabric. Only set the values described in the xtex file into the Fabric.
		/// @parm fabricIndex: the target fabric index in the object browser to apply xTex file onto
		/// @parm xTexFilePath: the xTex file path to apply
		/// @return if it succeeds, return true.
		virtual bool ApplyXtexFileW(unsigned int fabricIndex, const std::wstring& xTexFilePath) { return false; }

		/// @fn ApplyXtexFile(unsigned int _colorwayIndex, unsigned int _fabricIndex, const string& xTexFilePath)
		/// @brief Apply xtex to a fabric. Only set the values described in the xtex file into the Fabric.
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @parm _fabricIndex: the target fabric index in the Colorway Editor to apply xTex file onto
		/// @parm _xTexFilePath: the xTex file path to apply
		/// @return if it succeeds, return true.
		virtual bool ApplyXtexFile(unsigned int _colorwayIndex, unsigned int _fabricIndex, const std::string& _xTexFilePath) { return false; }

		/// @fn ApplyXtexFile(unsigned int _colorwayIndex, unsigned int _fabricIndex, const string& xTexFilePath)
		/// @brief Apply xtex to a fabric. Only set the values described in the xtex file into the Fabric.
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @parm _fabricIndex: the target fabric index in the Colorway Editor to apply xTex file onto
		/// @parm _xTexFilePath: the xTex file path to apply
		/// @return if it succeeds, return true.
		virtual bool ApplyXtexFileW(unsigned int _colorwayIndex, unsigned int _fabricIndex, const std::wstring& _xTexFilePath) { return false; }

		/// @fn DeleteFabric(unsigned int fabricIndex)
		/// @brief Delete a fabric. If the fabric was assigned on Patterns, the patterns will have the default fabric
		/// @return if it succeeds, return true.
		virtual bool DeleteFabric(unsigned int fabricIndex) { return false; }

		/// @fn DeleteFabric(unsigned int _colorwayIndex, unsigned int _fabricIndex)
		/// @brief Delete a fabric. If the fabric was assigned on Patterns, the patterns will have the default fabric
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @parm _fabricIndex: the target fabric index in the Colorway Editor to delete
		/// @return if it succeeds, return true.
		virtual bool DeleteFabric(unsigned int _colorwayIndex, unsigned int _fabricIndex) { return false; }

		/// @fn GetAPIMetaData(unsigned int fabricIndex)
		/// @brief Get API meta data for the fabric
		/// @parm fabricIndex: target fabric index on the Object Browser
		/// @return json string for meta data [key - value] list
		virtual std::string GetAPIMetaData(unsigned int fabricIndex) { return "";		}

		/// @fn GetAPIMetaDataW(unsigned int fabricIndex)
		/// @brief Get API meta data for the fabric
		/// @parm fabricIndex: target fabric index on the Object Browser
		/// @return json string for meta data [key - value] list
		virtual std::wstring GetAPIMetaDataW(unsigned int fabricIndex) { return L""; }

		/// @fn GetAPIMetaData(unsigned int _colorwayIndex, unsigned int _fabricIndex)
		/// @brief Get API meta data for the fabric
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @parm _fabricIndex: target fabric index on the Colorway Editor
		/// @return json string for meta data [key - value] list
		virtual std::string GetAPIMetaData(unsigned int _colorwayIndex, unsigned int _fabricIndex) { return ""; }

		/// @fn GetAPIMetaDataW(unsigned int _colorwayIndex, unsigned int _fabricIndex)
		/// @brief Get API meta data for the fabric
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @parm _fabricIndex: target fabric index on the Colorway Editor
		/// @return json string for meta data [key - value] list
		virtual std::wstring GetAPIMetaDataW(unsigned int _colorwayIndex, unsigned int _fabricIndex) { return L""; }

		/// @fn SetAPIMetaData(unsigned int fabricIndex, string jsonStr)
		/// @brief Set API meta data for the fabric
		/// @param fabricIndex: target fabric index on the Object Browser
		/// @param jsonStr: api meta data in string format 
		/// @return if it succeeds, return true
		virtual bool SetAPIMetaData(unsigned int fabricIndex, std::string jsonStr) { return ""; }

		/// @fn SetAPIMetaData(unsigned int fabricIndex, string jsonStr)
		/// @brief Set API meta data for the fabric
		/// @param fabricIndex: target fabric index on the Object Browser
		/// @param jsonStr: api meta data in string format 
		/// @return if it succeeds, return true
		virtual bool SetAPIMetaDataW(unsigned int fabricIndex, std::wstring jsonStr) { return L""; }

		/// @fn SetAPIMetaData(unsigned int _colorwayIndex, unsigned int _fabricIndex, string jsonStr)
		/// @brief Set API meta data for the fabric
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @param _fabricIndex: target fabric index on the Colorway Editor
		/// @param _jsonStr: api meta data in string format 
		/// @return if it succeeds, return true
		virtual bool SetAPIMetaData(unsigned int _colorwayIndex, unsigned int _fabricIndex, std::string _jsonStr) { return ""; }

		/// @fn SetAPIMetaData(unsigned int _colorwayIndex, unsigned int _fabricIndex, string jsonStr)
		/// @brief Set API meta data for the fabric
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @param _fabricIndex: target fabric index on the Colorway Editor
		/// @param _jsonStr: api meta data in string format 
		/// @return if it succeeds, return true
		virtual bool SetAPIMetaDataW(unsigned int _colorwayIndex, unsigned int _fabricIndex, std::wstring _jsonStr) { return L""; }

		/// @fn ChangeMetaDataValueForFabric(unsigned int fabricIndex, const string& metaDataKey, const string& metaDataValue)
		/// @brief Change or add Meta Data Value for the fabric with the index
		/// @param fabricIndex: target fabric index on the Object Browser 
		/// @param metaDataKey: target Key
		/// @param metaDataValue: the new value for the key
		virtual void ChangeMetaDataValueForFabric(unsigned int fabricIndex, const std::string& metaDataKey, const std::string& metaDataValue) {}

		/// @fn ChangeMetaDataValueForFabric(unsigned int fabricIndex, const wstring& metaDataKey, const wstring& metaDataValue)
		/// @brief Change or add Meta Data Value for the fabric with the index
		/// @param fabricIndex: target fabric index on the Object Browser 
		/// @param metaDataKey: target Key
		/// @param metaDataValue: the new value for the key
		virtual void ChangeMetaDataValueForFabricW(unsigned int fabricIndex, const std::wstring& metaDataKey, const std::wstring& metaDataValue) {}

		/// @fn ChangeMetaDataValueForFabric(unsigned int _colorwayIndex, unsigned int _fabricIndex, const string& metaDataKey, const string& metaDataValue)
		/// @brief Change or add Meta Data Value for the fabric with the index
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @param _fabricIndex: target fabric index on the Colorway Editor 
		/// @param _metaDataKey: target Key
		/// @param _metaDataValue: the new value for the key
		virtual void ChangeMetaDataValueForFabric(unsigned int _colorwayIndex, unsigned int _fabricIndex, const std::string& _metaDataKey, const std::string& _metaDataValue) {}

		/// @fn ChangeMetaDataValueForFabric(unsigned int _colorwayIndex, unsigned int _fabricIndex, const wstring& metaDataKey, const wstring& metaDataValue)
		/// @brief Change or add Meta Data Value for the fabric with the index
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @param _fabricIndex: target fabric index on the Colorway Editor 
		/// @param _metaDataKey: target Key
		/// @param _metaDataValue: the new value for the key
		virtual void ChangeMetaDataValueForFabricW(unsigned int _colorwayIndex, unsigned int _fabricIndex, const std::wstring& _metaDataKey, const std::wstring& _metaDataValue) {}

		/// @fn GetFabricSize()
		/// @brief Get the number of fabrics on the Object Browser
		/// @return return the number of fabrics on the object browser
		virtual int GetFabricSize() { return 0; } // deprecated. please use 'GetFabricCount()' function

		/// @fn GetFabricIndexForPattern(int patternIndex)
		/// @brief Get the fabric index which is assigned on the pattern with the pattern index
		/// @param patternIndex: the input pattern index to get the fabric index
		/// @return fabric index on the object browser for the pattern with the pattern index
		virtual int GetFabricIndexForPattern(int patternIndex) { return 0; }

		/// @fn GetFabricIndexForPattern(int _patternIndex, int _colorwayIndex)
		/// @brief Get the fabric index which is assigned on the pattern with the pattern index
		/// @param	_patternIndex: the input pattern index to get the fabric index
		///			_colorwayIndex: the colorway index to get the fabric index
		/// @return fabric index on the object browser for the pattern with the pattern index and colorway index
		virtual int GetFabricIndexForPattern(int _patternIndex, int _colorwayIndex) { return 0; }

		/// @fn GetFabricIndex(const string& fabricName)
		/// @brief Get the fabric index which is using the fabric name
		/// @param fabricName: the fabric name to get the fabric index
		/// @return fabric index
		virtual int GetFabricIndex(const std::string& fabricName) { return 0; }

		/// @fn GetFabricIndex(const string& fabricName)
		/// @brief Get the fabric index which is using the fabric name
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @param _fabricName: the fabric name to get the fabric index
		/// @return fabric index
		virtual int GetFabricIndex(int _colorwayIndex, const std::string& _fabricName) { return 0; }

		/// @fn GetFabricIndexW(const wstring& fabricName)
		/// @brief Get the fabric index which is using the fabric name
		/// @param fabricName: the fabric name to get the fabric index
		/// @return fabric index
		virtual int GetFabricIndexW(const std::wstring& fabricName) { return 0; }

		/// @fn GetFabricIndexW(const wstring& fabricName)
		/// @brief Get the fabric index which is using the fabric name
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @param _fabricName: the fabric name to get the fabric index
		/// @return fabric index
		virtual int GetFabricIndexW(int _colorwayIndex, const std::wstring& _fabricName) { return 0; }

		/// @fn GetFabricName(int fabricIndex)
		/// @brief Get the fabric name of the fabric in the fabricIndex on the object browser
		/// @parm fabricIndex: the fabric index on the Object Browser to get the name
		/// @return fabric name
		virtual std::string GetFabricName(int fabricIndex) { return ""; }

		/// @fn GetFabricNameW(int fabricIndex)
		/// @brief Get the fabric name of the fabric in the fabricIndex on the object browser
		/// @parm fabricIndex: the fabric index on the Object Browser to get the name
		/// @return fabric name
		virtual std::wstring GetFabricNameW(int fabricIndex) { return L""; }

		/// @fn GetFabricName(int _colorwayIndex, int _fabricIndex)
		/// @brief Get the fabric name of the fabric in the fabricIndex on the object browser
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @parm _fabricIndex: the fabric index on the Colorway Editor to get the name
		/// @return fabric name
		virtual std::string GetFabricName(int _colorwayIndex, int _fabricIndex) { return ""; }

		/// @fn GetFabricNameW(int _colorwayIndex, int _fabricIndex)
		/// @brief Get the fabric name of the fabric in the fabricIndex on the object browser
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @parm _fabricIndex: the fabric index on the Colorway Editor to get the name
		/// @return fabric name
		virtual std::wstring GetFabricNameW(int _colorwayIndex, int _fabricIndex) { return L""; }

		/// @fn GetFabricInfo(int fabricIndex)
		/// @brief Get a fabric information
		/// @param fabricIndex: the fabricIndex for the fabric to get the fabric info
		/// @return json string for fabric information
		virtual std::string GetFabricInfo(int fabricIndex) { return ""; }

		/// @fn GetFabricInfoW(int fabricIndex)
		/// @brief Get a fabric information
		/// @param fabricIndex: the fabricIndex for the fabric to get the fabric info
		/// @return json string for fabric information
		virtual std::wstring GetFabricInfoW(int fabricIndex) { return L""; }

		/// @fn GetColorwayFabricInfo(int _colorwayIndex, int _fabricIndex)
		/// @brief Get a fabric information for a colorway
		/// @param _colorwayIndex: the colorway index for the fabric to get the fabric info
		/// @param _fabricIndex: the fabricIndex for the fabric to get the fabric info
		/// @return json string for fabric information
		virtual std::string GetColorwayFabricInfo(int _colorwayIndex, int _fabricIndex) { return ""; }

		/// @fn GetColorwayFabricInfoW(int _colorwayIndex, int _fabricIndex)
		/// @brief Get a fabric information for a colorway
		/// @param _colorwayIndex: the colorway index for the fabric to get the fabric info
		/// @param _fabricIndex: the fabricIndex for the fabric to get the fabric info
		/// @return json string for fabric information
		virtual std::wstring GetColorwayFabricInfoW(int _colorwayIndex, int _fabricIndex) { return L""; }

		/// @fn GetAPIMetaDataFromFile(const string& filePath)
		/// @brief Get API meta data for the fabric
		/// @param filePath: filepath (.zfab / .jfab)
		/// @return json string for meta data [key - value] list
		virtual std::string GetAPIMetaDataFromFile(const std::string& filePath) { return ""; }

		/// @fn GetAPIMetaDataFromFileW(const wstring& filePath)
		/// @brief Get API meta data for the fabric
		/// @param filePath: filepath (.zfab / .jfab)
		/// @return json string for meta data [key - value] list
		virtual std::wstring GetAPIMetaDataFromFileW(const std::wstring& filePath) { return L""; }

		/// @fn SetFabricInformation(int _fabricIndex, const std::map<string, string> _infoMap)
		/// @brief Set Fabric Information to fabric
		/// @param _fabricIndex: the fabricIndex for the fabric to set the fabric info
		/// @param _infoMap: Fabric Information (etc. Classification, Content, SupplierName, Owner)
		virtual void SetFabricInformation(int _fabricIndex, const std::map<std::string, std::string> _infoMap) {}

		/// @fn SetFabricInformationW(int _fabricIndex, const std::map<wstring, wstring> _infoMap)
		/// @brief Set Fabric Information to fabric
		/// @param _fabricIndex: the fabricIndex for the fabric to set the fabric info
		/// @param _infoMap: Fabric Information (etc. Classification, Content, SupplierName, Owner)
		virtual void SetFabricInformationW(int _fabricIndex, const std::map<std::wstring, std::wstring>& _infoMap) {}

		/// @fn SetFabricInformation(int _colorwayIndex, int _fabricIndex, const std::map<string, string> _infoMap)
		/// @brief Set Fabric Information to fabric
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @param _fabricIndex: the fabricIndex in the Colorway Editor for the fabric to set the fabric info
		/// @param _infoMap: Fabric Information (etc. Classification, Content, SupplierName, Owner)
		virtual void SetFabricInformation(int _colorwayIndex, int _fabricIndex, const std::map<std::string, std::string> _infoMap) {}

		/// @fn SetFabricInformationW(int _colorwayIndex, int _fabricIndex, const std::map<wstring, wstring> _infoMap)
		/// @brief Set Fabric Information to fabric
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @param _fabricIndex: the fabricIndex in the Colorway Editor for the fabric to set the fabric info
		/// @param _infoMap: Fabric Information (etc. Classification, Content, SupplierName, Owner)
		virtual void SetFabricInformationW(int _colorwayIndex, int _fabricIndex, const std::map<std::wstring, std::wstring>& _infoMap) {}

		/// @fn GetFabricInformation(int _fabricIndex)
		/// @brief Get Fabric Information for fabric
		/// @param _fabricIndex: the fabricIndex for the fabric to get the fabric info
		/// @return Fabric Information
		virtual std::map<std::string, std::string> GetFabricInformation(int _fabricIndex)
		{
			std::map<std::string, std::string> infoMap;
			return infoMap;
		}

		/// @fn GetFabricInformationW(int _fabricIndex)
		/// @brief Get Fabric Information for fabric
		/// @param _fabricIndex: the fabricIndex for the fabric to get the fabric info
		/// @return Fabric Information
		virtual std::map<std::wstring, std::wstring> GetFabricInformationW(int _fabricIndex)
		{
			std::map<std::wstring, std::wstring> infoMap;
			return infoMap;
		}

		/// @fn GetFabricInformation(int _colorwayIndex, int _fabricIndex)
		/// @brief Get Fabric Information for fabric
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @param _fabricIndex: the fabricIndex in the Colorway Editor for the fabric to set the fabric info
		/// @return Fabric Information
		virtual std::map<std::string, std::string> GetFabricInformation(int _colorwayIndex, int _fabricIndex)
		{
			std::map<std::string, std::string> infoMap;
			return infoMap;
		}

		/// @fn GetFabricInformationW(int _colorwayIndex, int _fabricIndex)
		/// @brief Get Fabric Information for fabric
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @param _fabricIndex: the fabricIndex in the Colorway Editor for the fabric to set the fabric info
		/// @return Fabric Information
		virtual std::map<std::wstring, std::wstring> GetFabricInformationW(int _colorwayIndex, int _fabricIndex)
		{
			std::map<std::wstring, std::wstring> infoMap;
			return infoMap;
		}

		/// @fn SetCustomImage(int _fabricIndex, const string& _filePath)
		/// @brief Set Custom Image to fabric
		/// @param _fabricIndex: the fabricIndex for the fabric
		/// @param _filePath: filepath  (.png / .jpeg)
		virtual void SetCustomImage(int _fabricIndex, const std::string& _filePath) {}

		/// @fn SetCustomImageW(int _fabricIndex, const wstring& _filePath)
		/// @brief Set Custom Image to fabric
		/// @param _fabricIndex: the fabricIndex for the fabric
		/// @param _filePath: filepath  (.png / .jpeg)
		virtual void SetCustomImageW(int _fabricIndex, const std::wstring& _filePath) {}

		/// @fn SetCustomImage(int _colorwayIndex, int _fabricIndex, const string& _filePath)
		/// @brief Set Custom Image to fabric
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @param _fabricIndex: the fabricIndex in the Colorway Editor for the fabric
		/// @param _filePath: filepath  (.png / .jpeg)
		virtual void SetCustomImage(int _colorwayIndex, int _fabricIndex, const std::string& _filePath) {}

		/// @fn SetCustomImageW(int _colorwayIndex, int _fabricIndex, const wstring& _filePath)
		/// @brief Set Custom Image to fabric
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @param _fabricIndex: the fabricIndex in the Colorway Editor for the fabric
		/// @param _filePath: filepath  (.png / .jpeg)
		virtual void SetCustomImageW(int _colorwayIndex, int _fabricIndex, const std::wstring& _filePath) {}

		/// @fn GetPrimaryFabric()
		/// @brief Return the fabric index used for a major number of the patterns
		virtual int GetPrimaryFabric() { return -1; }

		/// @fn SetFabricName(unsigned int index, const string& str)
		/// @brief Change fabric name
		/// @param index: the target fabric index on the object browser to change the name
		/// @param str: new name for the fabric
		virtual void SetFabricName(unsigned int index, const std::string& str) {}

		/// @fn SetColorwayNameW(unsigned int index, const wstring& wstr)
		/// @brief Change fabric name
		/// @param index: the target fabric index on the object browser to change the name
		/// @param str: new name for the fabric
		virtual void SetFabricNameW(unsigned int index, const std::wstring& wstr) {}

		/// @fn SetFabricName(unsigned int _colorwayIndex, unsigned int _fabricIndex, const string& str)
		/// @brief Change fabric name
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @param _fabricIndex: the target fabric index on the object browser to change the name
		/// @param str: new name for the fabric
		virtual void SetFabricName(unsigned int _colorwayIndex, unsigned int _fabricIndex, const std::string& _str) {}

		/// @fn SetColorwayNameW(unsigned int _colorwayIndex, unsigned int _fabricIndex, const wstring& wstr)
		/// @brief Change fabric name
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @param _fabricIndex: the target fabric index on the object browser to change the name
		/// @param str: new name for the fabric
		virtual void SetFabricNameW(unsigned int _colorwayIndex, unsigned int _fabricIndex, const std::wstring& _wstr) {}

		/// @fn SetFabricName(unsigned int index, const string& str, bool refreshTab)
		/// @brief Change fabric name
		/// @param index: the target fabric index on the object browser to change the name
		/// @param str: new name for the fabric
		/// @param refreshTab: refreshes the tab after setting the new name
		virtual void SetFabricName(unsigned int _index, const std::string& _str, bool _bRefreshTab) {}

		/// @fn GetFabricStyleNameList()
		/// @brief Return all fabric style name
		virtual std::vector<std::string> GetFabricStyleNameList()
		{
			std::vector<std::string> fabricStyleList;
			return fabricStyleList;
		}

		/// @fn ImportSubstanceFile(unsigned int fabricIndex, const string& substanceFilePath)
		/// @brief Import substance to a fabric. 
		/// @parm fabricIndex: the target fabric index in the object browser to import substance file onto
		/// @parm substanceFilePath: the substance file path to import
		/// @return if it succeeds, return true.
		virtual bool ImportSubstanceFile(unsigned int fabricIndex, const std::string& substanceFilePath) { return false; }

		/// @fn ImportSubstanceFileW(unsigned int fabricIndex, const wstring& substanceFilePath)
		/// @brief Import substance to a fabric. 
		/// @parm fabricIndex: the target fabric index in the object browser to import substance file onto
		/// @parm substanceFilePath: the substance file path to import
		/// @return if it succeeds, return true.
		virtual bool ImportSubstanceFileW(unsigned int fabricIndex, const std::wstring& substanceFilePath) { return false; }

		/// @fn ImportSubstanceFile(unsigned int _colorwayIndex, unsigned int _fabricIndex, const string& _substanceFilePath)
		/// @brief Import substance to a fabric. 
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @parm _fabricIndex: the target fabric index in the Colorway Editor to import substance file onto
		/// @parm _substanceFilePath: the substance file path to import
		/// @return if it succeeds, return true.
		virtual bool ImportSubstanceFile(unsigned int _colorwayIndex, unsigned int _fabricIndex, const std::string& _substanceFilePath) { return false; }

		/// @fn ImportSubstanceFileW(unsigned int _colorwayIndex, unsigned int _fabricIndex, const wstring& _substanceFilePath)
		/// @brief Import substance to a fabric. 
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @parm _fabricIndex: the target fabric index in the Colorway Editor to import substance file onto
		/// @parm _substanceFilePath: the substance file path to import
		/// @return if it succeeds, return true.
		virtual bool ImportSubstanceFileW(unsigned int _colorwayIndex, unsigned int _fabricIndex, const std::wstring& _substanceFilePath) { return false; }

		/// @fn GetFabricWidth(int fabricIndex)
		/// @brief Get Fabric width (mm)
		/// @parm fabricIndex: the target fabric index in the object browser
		/// @return fabric width
		virtual float GetFabricWidth(int fabricIndex) { return 0.f;  }

		/// @fn GetFabricWidth(int _colorwayIndex, int _fabricIndex)
		/// @brief Get Fabric width (mm)
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @parm _fabricIndex: the target fabric index in the Colorway Editor
		/// @return fabric width
		virtual float GetFabricWidth(int _colorwayIndex, int _fabricIndex) { return 0.f; }

		/// @fn SetFabricWidth(int fabricIndex, float mm)
		/// @brief Set Fabric width (mm)
		/// @parm fabricIndex: the target fabric index in the object browser
		/// @parm mm: Fabric width
		/// @return void
		virtual void SetFabricWidth(int _fabricIndex, float mm) {}

		/// @fn SetFabricWidth(unsigned int _colorwayIndex, int fabricIndex, float mm)
		/// @brief Set Fabric width (mm)
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @parm _fabricIndex: the target fabric index in the Colorway Editor
		/// @parm _mm: Fabric width
		/// @return void
		virtual void SetFabricWidth(int _colorwayIndex, int _fabricIndex, float _mm) {}

		/// @fn GetFabricLength(int fabricIndex)
		/// @brief Get Fabric Length (mm)
		/// @parm fabricIndex: the target fabric index in the object browser
		/// @return Fabric Length
		virtual float GetFabricLength(int fabricIndex) { return 0.f; }

		/// @fn GetFabricLength(int _colorwayIndex, int _fabricIndex)
		/// @brief Get Fabric Length (mm)
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @parm _fabricIndex: the target fabric index in the Colorway Editor
		/// @return Fabric Length
		virtual float GetFabricLength(int _colorwayIndex, int _fabricIndex) { return 0.f; }

		/// @fn SetTextureMapping(unsigned int fabricIndex, int materialFace, int mappingType)
		/// @brief Set Texture Mapping Type (Repeat or Unified)
		/// @parm fabricIndex: the target fabric index in the object browser
		/// @parm mapping type: the target mapping type to set. 0: "Repeat", 1: "Unified"
		/// @return void
		virtual void SetTextureMapping(unsigned int fabricIndex, int mappingType) {}

		/// @fn SetTextureMapping(unsigned int _colorwayIndex, unsigned int fabricIndex, int materialFace, int mappingType)
		/// @brief Set Texture Mapping Type (Repeat or Unified)
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @parm _fabricIndex: the target fabric index in the ColorwayEditor
		/// @parm _mappingType: the target mapping type to set. 0: "Repeat", 1: "Unified"
		/// @return void
		virtual void SetTextureMapping(unsigned int _colorwayIndex, unsigned int _fabricIndex, int _mappingType) {}

		/// @fn SetSubstancePreset(unsigned int fabricIndex, int materialFace, int preset)
		/// @brief Set Texture Mapping Type (Repeat or Unified)
		/// @parm fabricIndex: the target fabric index in the object browser
		/// @parm materialFace: the target material index in the property editor. 0: "Front", 1: "Back", 2: "Side"
		/// @parm presetIndex: the preset index to set
		/// @return void
		virtual void SetSubstancePreset(unsigned int fabricIndex, int materialFace, int presetIndex) {}

		/// @fn SetSubstancePreset(unsigned int colorwayIndex,unsigned int fabricIndex, int materialFace, int preset)
		/// @brief Set Texture Mapping Type (Repeat or Unified)
		/// @param colorwayIndex: target colorway index in the Colorway Editor
		/// @parm fabricIndex: the target fabric index in the object browser
		/// @parm materialFace: the target material index in the property editor. 0: "Front", 1: "Back", 2: "Side"
		/// @parm presetIndex: the preset index to set
		/// @return void
		virtual void SetSubstancePreset(unsigned int colorwayIndex, unsigned int fabricIndex, int materialFace, int presetIndex) {}

		/// @fn SetSubstanceResolution(unsigned int fabricIndex, int materialFace, int resolution)
		/// @brief Set Texture Mapping Type (Repeat or Unified)
		/// @parm fabricIndex: the target fabric index in the object browser
		/// @parm materialFace: the target material index in the property editor. 0: "Front", 1: "Back", 2: "Side"
		/// @parm resolutionIndex: the resolution index to set
		/// @return void
		virtual void SetSubstanceResolution(unsigned int fabricIndex, int materialFace, int resolutionIndex) {}

		/// @fn SetSubstanceResolution(unsigned int colorwayIndex,unsigned int fabricIndex, int materialFace, int resolution)
		/// @brief Set Texture Mapping Type (Repeat or Unified)
		/// @param colorwayIndex: target colorway index in the Colorway Editor
		/// @parm fabricIndex: the target fabric index in the object browser
		/// @parm materialFace: the target material index in the property editor. 0: "Front", 1: "Back", 2: "Side"
		/// @parm resolutionIndex: the resolution index to set
		/// @return void
		virtual void SetSubstanceResolution(unsigned int colorwayIndex, unsigned int fabricIndex, int materialFace, int resolutionIndex) {}

		/// @fn ImportSubstanceFileAsFaceType(unsigned int _fabricIndex, const string& _substanceFilePath, const string& _faceType)
		/// @brief Import substance to a fabric as facetype. 
		/// @parm _fabricIndex: the target fabric index in the object browser to import substance file onto
		/// @parm _substanceFilePath: the substance file path to import
		/// @parm _facetype :“Front”, “Back”, “Side” 
		/// @return if it succeeds, return true.
		virtual bool ImportSubstanceFileAsFaceType(unsigned int _fabricIndex, const std::string& _substanceFilePath, const std::string& _faceType) { return false; }

		/// @fn ImportSubstanceFileAsFaceType(unsigned int _colorwayIndex, unsigned int _fabricIndex, const string& _substanceFilePath, const string& _faceType)
		/// @brief Import substance to a fabric as facetype. 
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @parm _fabricIndex: the target fabric index in the object browser to import substance file onto
		/// @parm _substanceFilePath: the substance file path to import
		/// @parm _facetype :“Front”, “Back”, “Side” 
		/// @return if it succeeds, return true.
		virtual bool ImportSubstanceFileAsFaceType(unsigned int _colorwayIndex, unsigned int _fabricIndex, const std::string& _substanceFilePath, const std::string& _faceType) { return false; }

		/// @fn ImportSubstanceFileAsFaceTypeW(unsigned int _fabricIndex, const wstring& _substanceFilePath, const string& _faceType)
		/// @brief Import substance to a fabric. 
		/// @parm _fabricIndex: the target fabric index in the object browser to import substance file onto
		/// @parm _substanceFilePath: the substance file path to import
		/// @parm _facetype :“Front”, “Back”, “Side” 
		/// @return if it succeeds, return true.
		virtual bool ImportSubstanceFileAsFaceTypeW(unsigned int _fabricIndex, const std::wstring& _substanceFilePath, const std::string& _faceType) { return false; }

		/// @fn ImportSubstanceFileAsFaceTypeW(unsigned int _colorwayIndex, unsigned int _fabricIndex, const wstring& _substanceFilePath, const string& _faceType)
		/// @brief Import substance to a fabric. 
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @parm _fabricIndex: the target fabric index in the object browser to import substance file onto
		/// @parm _substanceFilePath: the substance file path to import
		/// @parm _facetype :“Front”, “Back”, “Side” 
		/// @return if it succeeds, return true.
		virtual bool ImportSubstanceFileAsFaceTypeW(unsigned int _colorwayIndex, unsigned int _fabricIndex, const std::wstring& _substanceFilePath, const std::string& _faceType) { return false; }

		/// @fn SetUseSameMaterialAsFront(unsigned int fabricIndex, int materialFace, bool _useSameMaterial)
		/// @brief Set use same material as front. 
		/// @parm fabricIndex: the target fabric index in the object browser
		/// @parm materialFace: the target material index in the property editor. 1: "Back", 2: "Side"
		/// @parm _useSameMaterial: use same material as front
		/// @return void
		virtual void SetUseSameMaterialAsFront(unsigned int fabricIndex, int materialFace, bool _useSameMaterial) {}

		/// @fn SetUseSameMaterialAsFront(unsigned int colorwayIndex, unsigned int fabricIndex, int materialFace, bool _useSameMaterial)
		/// @brief Set use same material as front. 
		/// @param colorwayIndex: target colorway index in the Colorway Editor
		/// @parm fabricIndex: the target fabric index in the object browser
		/// @parm materialFace: the target material index in the property editor. 1: "Back", 2: "Side"
		/// @parm _useSameMaterial: use same material as front
		/// @return void
		virtual void SetUseSameMaterialAsFront(unsigned int colorwayIndex, unsigned int fabricIndex, int materialFace, bool _useSameMaterial) {}

		/// @fn SetUseSameColorAsFront(unsigned int fabricIndex, int materialFace, bool _useSameMaterial)
		/// @brief Set use same color as front. 
		/// @parm fabricIndex: the target fabric index in the object browser
		/// @parm materialFace: the target material index in the property editor. 1: "Back", 2: "Side"
		/// @parm _useSameColor: use same color as front
		/// @return void
		virtual void SetUseSameColorAsFront(unsigned int fabricIndex, int materialFace, bool _useSameColor) {}

		/// @fn SetUseSameColorAsFront(unsigned int colorwayIndex, unsigned int fabricIndex, int materialFace, bool _useSameMaterial)
		/// @brief Set use same color as front. 
		/// @param colorwayIndex: target colorway index in the Colorway Editor
		/// @parm fabricIndex: the target fabric index in the object browser
		/// @parm materialFace: the target material index in the property editor. 1: "Back", 2: "Side"
		/// @parm _useSameColor: use same color as front
		/// @return void
		virtual void SetUseSameColorAsFront(unsigned int colorwayIndex, unsigned int fabricIndex, int materialFace, bool _useSameColor) {}

		/*
		Set a base color of PBR material in a fabric using given RGBA values.
		@return if it succeeds, return true.
		*/
		/// @fn SetFabricPBRMaterialBaseColor(unsigned int fabricIndex, unsigned int materialFace, float r, float g, float b, float a)
		/// @brief Set a base color of PBR material in a fabric using given RGBA values.
		/// @parm fabricIndex: the target fabric index in the object browser to import substance file onto
		/// @parm materialFace :“Front”, “Back”, “Side” 
		/// @parm r : To be set as red value
		/// @parm g : To be set as green value
		/// @parm b : To be set as blue value
		/// @parm a : To be set as alpha value
		/// @return if it succeeds, return true.
		virtual bool SetFabricPBRMaterialBaseColor(unsigned int fabricIndex, unsigned int materialFace, float r, float g, float b, float a) = 0;

		/*
		Set a base color of PBR material in a fabric using given RGBA values.
		@return if it succeeds, return true.
		*/
		/// @fn SetFabricPBRMaterialBaseColor(unsigned int _colorwayIndex, unsigned int _fabricIndex, unsigned int _materialFace, float _r, float _g, float _b, float _a)
		/// @brief Set a base color of PBR material in a fabric using given RGBA values.
		/// @parm _colorwayIndex: the target colorway index
		/// @parm _fabricIndex: the target fabric index in the object browser(Fabric index calculated assuming that the "Show All Colorways" option is turned off)
		/// @parm _materialFace :“Front”, “Back”, “Side” 
		/// @parm _r : To be set as red value
		/// @parm _g : To be set as green value
		/// @parm _b : To be set as blue value
		/// @parm _a : To be set as alpha value
		/// @return if it succeeds, return true.
		virtual bool SetFabricPBRMaterialBaseColor(unsigned int _colorwayIndex, unsigned int _fabricIndex, unsigned int _materialFace, float _r, float _g, float _b, float _a) = 0;

		/*
		Set a base color of PBR material in a fabric using given name of color.
		@return if it succeeds, return true.
		*/
		/// @fn SetFabricPBRMaterialBaseColor(unsigned int fabricIndex, unsigned int materialFace, const std::string& _colorName)
		/// @brief Set a base color of PBR material in a fabric using given name of color.
		/// @parm fabricIndex: the target fabric index in the object browser to import substance file onto
		/// @parm materialFace :“Front”, “Back”, “Side” 
		/// @parm _colorName: Name of color
		/// @return if it succeeds, return true.
		virtual bool SetFabricPBRMaterialBaseColor(unsigned int fabricIndex, unsigned int materialFace, const std::string& _colorName) = 0;

		/*
		Set a base color of PBR material in a fabric using given name of color.
		@return if it succeeds, return true.
		*/
		/// @fn SetFabricPBRMaterialBaseColor(unsigned int _colorwayIndex, unsigned int _fabricIndex, unsigned int _materialFace, const std::string& _colorName)
		/// @brief Set a base color of PBR material in a fabric using given name of color.
		/// @parm _colorwayIndex: the target colorway index
		/// @parm _fabricIndex: the target fabric index in the object browser(Fabric index calculated assuming that the "Show All Colorways" option is turned off)
		/// @parm _materialFace :“Front”, “Back”, “Side” 
		/// @parm _colorName: Name of color
		/// @return if it succeeds, return true.
		virtual bool SetFabricPBRMaterialBaseColor(unsigned int _colorwayIndex, unsigned int _fabricIndex, unsigned int _materialFace, const std::string& _colorName) = 0;

		/// @fn GetFabricPBRMaterialBaseColor(unsigned int fabricIndex, unsigned int materialFace)
		/// @brief Get a base color of PBR material in a fabric.
		/// @parm fabricIndex: the target fabric index in the object browser to import substance file onto
		/// @parm materialFace :“Front”, “Back”, “Side” 
		/// @return RGBA float tuple
		virtual std::tuple<float, float, float, float> GetFabricPBRMaterialBaseColor(unsigned int fabricIndex, unsigned int materialFace) = 0;

		/// @fn GetFabricPBRMaterialBaseColor(unsigned int _colorwayIndex, unsigned int _fabricIndex, unsigned int _materialFace)
		/// @brief Get a base color of PBR material in a fabric.
		/// @parm _colorwayIndex: the target colorway index
		/// @parm _fabricIndex: the target fabric index in the object browser to import substance file onto
		/// @parm _materialFace :“Front”, “Back”, “Side” 
		/// @return RGBA float tuple
		virtual std::tuple<float, float, float, float> GetFabricPBRMaterialBaseColor(unsigned int _colorwayIndex, unsigned int _fabricIndex, unsigned int _materialFace) = 0;

		/// @fn SetCurrentFabricIndex(int _index)
		/// @brief Set Current Fabric Index
		/// @parm _index: Fabric Index
		/// @return Fabric setting success or failure return
		virtual bool SetCurrentFabricIndex(int _index) { return true; };

		/// @fn SetCurrentFabricIndex(unsigned int _colorwayIndex, int _index)
		/// @brief Set Current Fabric Index
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @parm _index: Fabric Index
		/// @return Fabric setting success or failure return
		virtual bool SetCurrentFabricIndex(unsigned int _colorwayIndex, int _index) { return true; };

		/// @fn CreateZfabFromTextures(const std::string& _filePath, const std::string& _baseTexturePath, const std::string& _normalTexturePath, const std::string& _disPlacementTexturePath, const std::string& _opacityTexturePath)
		/// @brief Create Zfab From Textures
		/// @param _filePath: created filepath (.zfab)
		/// @parm _baseTexturePath: the baseTexture file path to create zfab
		/// @parm _normalTexturePath: the normalTexturePath file path to create zfab
		/// @parm _disPlacementTexturePath :the disPlacementTexturePath file path to create zfab
		/// @parm _opacityTexturePath :the opacityTexturePath file path to create zfab
		/// @parm _roughnessTexturePath :the roughnessTexturePath file path to create zfab
		/// @parm _metalnessTexturePath :the metalnessTexturePath file path to create zfab
		/// @return zfab file creating success or failure return
		virtual bool CreateZfabFromTextures(const std::string& _filePath, const std::string& _baseTexturePath, const std::string& _normalTexturePath, const std::string& _disPlacementTexturePath, const std::string& _opacityTexturePath, const std::string& _roughnessTexturePath, const std::string& _metalnessTexturePath) { return false; }

		/// @fn CombineZfab(const std::string& _filePath, const std::string& _baseZfabPath, const std::string& _targetZfabPath)
		/// @brief Combining two zfab files into one
		/// @param _filePath: created filepath (.zfab)
		/// @parm _baseZfabPath: the Path to the zfab file on which the physical properties will be based
		/// @parm _targetZfabPath: the path to the zfab file on which the image will be based
		virtual void CombineZfab(const std::string& _filePath, const std::string& _baseZfabPath, const std::string& _targetZfabPath) {}


		/// @fn int GetRoughnessType(int _index, int _materialFace)
		/// @brief Get Material RoughnessType
		/// @parm _index: Fabric Index, _materialFace : Fabric Material Face Type (0 : Front , 1 : BACK , 2 : SIDE)
		/// @return Return fabric's mateiral Roughness Type. ( 0 : Intensity, 1 : Map )
		virtual int GetRoughnessType(int _index, int _materialFace) = 0;

		/// @fn int GetRoughnessType(unsigned int _colorwayIndex, int _index, int _materialFace)
		/// @brief Get Material RoughnessType
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @parm _index: Fabric Index, _materialFace : Fabric Material Face Type (0 : Front , 1 : BACK , 2 : SIDE)
		/// @return Return fabric's mateiral Roughness Type. ( 0 : Intensity, 1 : Map )
		virtual int GetRoughnessType(unsigned int _colorwayIndex, int _index, int _materialFace) = 0;

		/// @fn SetRoughnessType(int _index, int _materialFace, int _roughnessType)
		/// @brief Set Current Fabric's mateiral roughnessType
		/// @parm _index: Fabric Index, _materialFace : target material Face Type (0 : Front , 1 : BACK , 2 : SIDE), _roughnessType : roughnessType to be set ( 0 : Intensity, 1 : Map )
		/// @return Fabric setting success or failure return
		virtual void SetRoughnessType(int _index, int _materialFace, int _roughnessType) = 0;

		/// @fn SetRoughnessType(unsigned int _colorwayIndex, int _index, int _materialFace, int _roughnessType)
		/// @brief Set Current Fabric's mateiral roughnessType
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @parm _index: Fabric Index, _materialFace : target material Face Type (0 : Front , 1 : BACK , 2 : SIDE), _roughnessType : roughnessType to be set ( 0 : Intensity, 1 : Map )
		/// @return Fabric setting success or failure return
		virtual void SetRoughnessType(unsigned int _colorwayIndex, int _index, int _materialFace, int _roughnessType) = 0;

		/// @fn float GetRoughnessValueIntensity(int _index, int _face)
		/// @brief Get fabric material's Roughness Intensity values.
		/// @parm _index: Fabric Index, _face : target material Face Type (0 : Front , 1 : BACK , 2 : SIDE)
		/// @return Fabric material's Roughness Intensity values.
		virtual int GetRoughnessValueIntensity(int _index, int _face) = 0;

		/// @fn float GetRoughnessValueIntensity(unsigned int _colorwayIndex, int _index, int _face)
		/// @brief Get fabric material's Roughness Intensity values.
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @parm _index: Fabric Index, _face : target material Face Type (0 : Front , 1 : BACK , 2 : SIDE)
		/// @return Fabric material's Roughness Intensity values.
		virtual int GetRoughnessValueIntensity(unsigned int _colorwayIndex, int _index, int _face) = 0;

		/// @fn void SetRoughnessValueIntensity(int _fabricIndex, int _face, int _value)
		/// @brief Set fabric material's Roughness Intensity values.
		/// @parm	_fabricIndex: Fabric Index
		///			_face : target material Face Type (0 : Front , 1 : BACK , 2 : SIDE)
		///			_value : roughness value
		/// @return Fabric material's Roughness Intensity values.
		virtual void SetRoughnessValueIntensity(int _fabricIndex, int _face, int _value) = 0;

		/// @fn void SetRoughnessValueIntensity(unsigned int _colorwayIndex, int _fabricIndex, int _face, int _value)
		/// @brief Set fabric material's Roughness Intensity values.
		/// @param	_colorwayIndex: target colorway index in the Colorway Editor
		///			_fabricIndex: the fabricIndex in the Colorway Editor for the fabric to set the fabric info
		///			_face : target material Face Type (0 : Front , 1 : BACK , 2 : SIDE)
		///			_value : roughness value
		/// @return Fabric material's Rough
		/// @return Fabric material's Roughness Intensity values.
		virtual void SetRoughnessValueIntensity(unsigned int _colorwayIndex, int _fabricIndex, int _face, int _value) = 0;


		/// @fn float GetRoughnessValueMapIntensity(int _index, int _face)
		/// @brief Get fabric material's map type roughness intensity values.
		/// @parm _index: Fabric Index, _face : target material Face Type (0 : Front , 1 : BACK , 2 : SIDE)
		/// @return Fabric material's map type roughness Intensity values.
		virtual int GetRoughnessValueMapIntensity(int _index, int _face) = 0;

		/// @fn float GetRoughnessValueMapIntensity(unsigned int _colorwayIndex, int _index, int _face)
		/// @brief Get fabric material's map type roughness intensity values.
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @parm _index: Fabric Index, _face : target material Face Type (0 : Front , 1 : BACK , 2 : SIDE)
		/// @return Fabric material's map type roughness Intensity values.
		virtual int GetRoughnessValueMapIntensity(unsigned int _colorwayIndex, int _index, int _face) = 0;

		/// @fn SetRoughnessValueMapIntensity(int _index, int _face, int _value)
		/// @brief Set fabric material's map type roughness intensity values.
		/// @parm _index: Fabric Index, _face : target material Face Type (0 : Front , 1 : BACK , 2 : SIDE), _value : intensity value to be set.
		virtual void SetRoughnessValueMapIntensity(int _index, int _face, int _value) = 0;

		/// @fn SetRoughnessValueMapIntensity(unsigned int _colorwayIndex, int _index, int _face, int _value)
		/// @brief Set fabric material's map type roughness intensity values.
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @parm _index: Fabric Index, _face : target material Face Type (0 : Front , 1 : BACK , 2 : SIDE), _value : intensity value to be set.
		virtual void SetRoughnessValueMapIntensity(unsigned int _colorwayIndex, int _index, int _face, int _value) = 0;

		/// @fn bool IsRoughnessValueMapInvert(int _index, int _face)
		/// @brief Return whether roughness map is inverted.
		/// @parm _index: Fabric Index, _face : target material Face Type (0 : Front , 1 : BACK , 2 : SIDE)
		/// @Return whether roughness map is inverted.
		virtual bool IsRoughnessValueMapInvert(int _index, int _face) = 0;

		/// @fn bool IsRoughnessValueMapInvert(unsigned int _colorwayIndex, int _index, int _face)
		/// @brief Return whether roughness map is inverted.
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @parm _index: Fabric Index, _face : target material Face Type (0 : Front , 1 : BACK , 2 : SIDE)
		/// @Return whether roughness map is inverted.
		virtual bool IsRoughnessValueMapInvert(unsigned int _colorwayIndex, int _index, int _face) = 0;

		/// @fn bool SetRoughnessValueMapInvert(int _index, int _face, bool _bInvert)
		/// @brief SetS whether roughness map is inverted
		/// @parm _index: Fabric Index, _face : target material Face Type (0 : Front , 1 : BACK , 2 : SIDE), bInvert : Whether roughness map is inverted.
		virtual void SetRoughnessValueMapInvert(int _index, int _face, bool _bInvert)  = 0;

		/// @fn bool SetRoughnessValueMapInvert(unsigned int _colorwayIndex, int _index, int _face, bool _bInvert)
		/// @brief SetS whether roughness map is inverted
		/// @param _colorwayIndex: target colorway index in the Colorway Editor
		/// @parm _index: Fabric Index, _face : target material Face Type (0 : Front , 1 : BACK , 2 : SIDE), bInvert : Whether roughness map is inverted.
		virtual void SetRoughnessValueMapInvert(unsigned int _colorwayIndex, int _index, int _face, bool _bInvert) = 0;

        /// @fn bool SetFabricDisplacementMap(unsigned int _fabricIndex, const std::string &path)
        /// @brief Set fabric's displacement map using given image path.
        /// @parm _fabricIndex: index of fabric
        /// @parm _path: path of the image file
        /// @Return true if succeed, false othrewise.
        virtual bool SetPBRMaterialDisplacementMap(unsigned int _fabricIndex, const std::string &_path) = 0;

		/// @fn bool SetFabricDisplacementMap(unsigned int _colorwayIndex, unsigned int _fabricIndex, const std::string& _path)
		/// @brief Set fabric's displacement map using given image path.
		/// @parm _colorwayIndex: index of colorway
		/// @parm _fabricIndex: index of fabric
		/// @parm _path: path of the image file
		/// @Return true if succeed, false othrewise.
		virtual bool SetPBRMaterialDisplacementMap(unsigned int _colorwayIndex, unsigned int _fabricIndex, const std::string& _path) = 0;

        /// @fn std::tuple<float, float, float, float, bool> GetPBRMaterialDisplacementMapValue(unsigned int _fabricIndex)
        /// @brief Get fabric's displacement map values.
        /// @parm _fabricIndex: index of fabric
        /// @Return [Amount, Shift, Clipping, Particle Distance, Keep(bool)] tuple if succeed, [0, 0, 0, 0, false] otherwise.
        virtual std::tuple<float, float, float, float, bool> GetPBRMaterialDisplacementMapValue(unsigned int _fabricIndex) = 0;

		/// @fn std::tuple<float, float, float, float, bool> GetPBRMaterialDisplacementMapValue(unsigned int _colorwayIndex, unsigned int _fabricIndex)
		/// @brief Get fabric's displacement map values.
		/// @parm _colorwayIndex: _colorwayIndex: index of colorway
		/// @parm _fabricIndex: index of fabric
		/// @Return [Amount, Shift, Clipping, Particle Distance, Keep(bool)] tuple if succeed, [0, 0, 0, 0, false] otherwise.
		virtual std::tuple<float, float, float, float, bool> GetPBRMaterialDisplacementMapValue(unsigned int _colorwayIndex, unsigned int _fabricIndex) = 0;

        /// @fn bool SetPBRMaterialDisplacementMapValue(unsigned int _fabricIndex, float _amount, float _shift, float _clipping, float _particleDist, bool _keep)
        /// @brief Set fabric's displacement map values.
        /// @parm _fabricIndex: index of fabric
        /// @parm _amount: Amount value to be set (mm)
        /// @parm _shift: Shift value to be set (mm)
        /// @parm _clipping: Clipping value to be set (mm)
        /// @parm _particleDist: Particle distance value to be set (mm)
        /// @parm _keep: Keep Continuity value to be set
        /// @Return true if succeed, false othrewise.
        virtual bool SetPBRMaterialDisplacementMapValue(unsigned int _fabricIndex, float _amount, float _shift, float _clipping, float _particleDist, bool _keep) = 0;

		/// @fn bool SetPBRMaterialDisplacementMapValue(unsigned int _colorwayIndex, unsigned int _fabricIndex, float _amount, float _shift, float _clipping, float _particleDist, bool _keep)
		/// @brief Set fabric's displacement map values.
		/// @parm _colorwayIndex: index of colorway
		/// @parm _fabricIndex: index of fabric
		/// @parm _amount: Amount value to be set (mm)
		/// @parm _shift: Shift value to be set (mm)
		/// @parm _clipping: Clipping value to be set (mm)
		/// @parm _particleDist: Particle distance value to be set (mm)
		/// @parm _keep: Keep Continuity value to be set
		/// @Return true if succeed, false othrewise.
		virtual bool SetPBRMaterialDisplacementMapValue(unsigned int _colorwayIndex, unsigned int _fabricIndex, float _amount, float _shift, float _clipping, float _particleDist, bool _keep) = 0;

		/// @fn GetReflectionRoughness(int _fabricIndex, int _materialFace)
		/// @brief Get reflection roughness value
		/// @param _fabricIndex: the target fabric index in the object browser
		/// @param _materialFace: the target material index in the property editor. 0: "Front", 1: "Back", 2: "Side"
		/// @return reflection roughness value
		virtual float GetReflectionRoughness(int _fabricIndex, int _materialFace) { return 0.0f; };

		/// @fn GetReflectionRoughness(unsigned int _colorwayIndex, int fabricIndex, int _materialFace)
		/// @brief Get reflection roughness value
		/// @parm _colorwayIndex: index of colorway
		/// @param _fabricIndex: the target fabric index in the object browser
		/// @param _materialFace: the target material index in the property editor. 0: "Front", 1: "Back", 2: "Side"
		/// @return reflection roughness value
		virtual float GetReflectionRoughness(unsigned int _colorwayIndex, int fabricIndex, int _materialFace) { return 0.0f; };

		/// @fn SetReflectionRoughness(int _fabricIndex, int _materialFace, float _reflectionRoughness)
		/// @brief Set reflection roughness value
		/// @param _fabricIndex: the target fabric index in the object browser
		/// @param _materialFace: the target material index in the property editor. 0: "Front", 1: "Back", 2: "Side"
		/// @param _reflectionRoughness roughness value
		/// @return void
		virtual void SetReflectionRoughness(int _fabricIndex, int _materialFace, float _reflectionRoughness){};

		/// @fn SetReflectionRoughness(unsigned int _colorwayIndex, int fabricIndex, int _materialFace, float _reflectionRoughness)
		/// @brief Set reflection roughness value
		/// @parm _colorwayIndex: index of colorway
		/// @param _fabricIndex: the target fabric index in the object browser
		/// @param _materialFace: the target material index in the property editor. 0: "Front", 1: "Back", 2: "Side"
		/// @param _reflectionRoughness roughness value
		/// @return void
		virtual void SetReflectionRoughness(unsigned int _colorwayIndex, int fabricIndex, int _materialFace, float _reflectionRoughness) {};

		/// @fn GetReflectionIntensity(int _fabricIndex, int _materialFace)
		/// @brief Get reflection intensity
		/// @param _fabricIndex: the target fabric index in the object browser
		/// @param _materialFace: the target material index in the property editor. 0: "Front", 1: "Back", 2: "Side"
		/// @return reflection intensity value
		virtual float GetReflectionIntensity(int _fabricIndex, int _materialFace) { return 0.0f; };

		/// @fn GetReflectionIntensity(unsigned int _colorwayIndex, int fabricIndex, int _materialFace)
		/// @brief Get reflection intensity
		/// @parm _colorwayIndex: index of colorway
		/// @param _fabricIndex: the target fabric index in the object browser
		/// @param _materialFace: the target material index in the property editor. 0: "Front", 1: "Back", 2: "Side"
		/// @return reflection intensity value
		virtual float GetReflectionIntensity(unsigned int _colorwayIndex, int fabricIndex, int _materialFace) { return 0.0f; };

		/// @fn SetReflectionIntensity(int _fabricIndex, int _materialFace, float _reflectionIntensity)
		/// @brief Set reflection intensity
		/// @param _fabricIndex: the target fabric index in the object browser
		/// @param _materialFace: the target material index in the property editor. 0: "Front", 1: "Back", 2: "Side"
		/// @param _reflectionIntensity: reflection intensity value
		/// @return void
		virtual void SetReflectionIntensity(int _fabricIndex, int _materialFace, float _reflectionIntensity){};

		/// @fn SetReflectionIntensity(unsigned int _colorwayIndex, int fabricIndex, int _materialFace, float _reflectionIntensity)
		/// @brief Set reflection intensity
		/// @parm _colorwayIndex: index of colorway
		/// @param _fabricIndex: the target fabric index in the object browser
		/// @param _materialFace: the target material index in the property editor. 0: "Front", 1: "Back", 2: "Side"
		/// @param _reflectionIntensity: reflection intensity value
		/// @return void
		virtual void SetReflectionIntensity(unsigned int _colorwayIndex, int fabricIndex, int _materialFace, float _reflectionIntensity) {};

		/// @fn GetMetalness(int _fabricIndex, int _materialFace)
		/// @brief Get metalness
		/// @param _fabricIndex: the target fabric index in the object browser
		/// @param _materialFace: the target material index in the property editor. 0: "Front", 1: "Back", 2: "Side"
		/// @return metalness value
		virtual float GetMetalness(int _fabricIndex, int _materialFace) { return 0.0f; };

		/// @fn GetMetalness(unsigned int _colorwayIndex, int fabricIndex, int _materialFace)
		/// @brief Get metalness
		/// @parm _colorwayIndex: index of colorway
		/// @param _fabricIndex: the target fabric index in the object browser
		/// @param _materialFace: the target material index in the property editor. 0: "Front", 1: "Back", 2: "Side"
		/// @return metalness value
		virtual float GetMetalness(unsigned int _colorwayIndex, int fabricIndex, int _materialFace) { return 0.0f; };

		/// @fn SetMetalness(int _fabricIndex, int _materialFace, float _metalness)
		/// @brief Set metalness
		/// @param _fabricIndex: the target fabric index in the object browser
		/// @param _materialFace: the target material index in the property editor. 0: "Front", 1: "Back", 2: "Side"
		/// @param _metalness: metalness value
		/// @return void
		virtual void SetMetalness(int _fabricIndex, int _materialFace, float _metalness){};

		/// @fn SetMetalness(unsigned int _colorwayIndex, int fabricIndex, int _materialFace, float _metalness)
		/// @brief Set metalness
		/// @parm _colorwayIndex: index of colorway
		/// @param _fabricIndex: the target fabric index in the object browser
		/// @param _materialFace: the target material index in the property editor. 0: "Front", 1: "Back", 2: "Side"
		/// @param _metalness: metalness value
		/// @return void
		virtual void SetMetalness(unsigned int _colorwayIndex, int fabricIndex, int _materialFace, float _metalness) {};

		/// @fn GetNormalMapIntensity(int _fabricIndex, int _materialFace)
		/// @brief Get normal map intensity in percent
		/// @param _fabricIndex: the target fabric index in the object browser
		/// @param _materialFace: the target material index in the property editor. 0: "Front", 1: "Back", 2: "Side"
		/// @return normal map intensity value
		virtual float GetNormalMapIntensity(int _fabricIndex, int _materialFace) { return 0.0f; };

		/// @fn GetNormalMapIntensity(unsigned int _colorwayIndex, int fabricIndex, int _materialFace)
		/// @brief Get normal map intensity in percent
		/// @parm _colorwayIndex: index of colorway
		/// @param _fabricIndex: the target fabric index in the object browser
		/// @param _materialFace: the target material index in the property editor. 0: "Front", 1: "Back", 2: "Side"
		/// @return normal map intensity value
		virtual float GetNormalMapIntensity(unsigned int _colorwayIndex, int fabricIndex, int _materialFace) { return 0.0f; };

		/// @fn SetNormalMapIntensity(int _fabricIndex, int _materialFace, float _normalMapIntensity)
		/// @brief Set normal map intensity in percent
		/// @param _fabricIndex: the target fabric index in the object browser
		/// @param _materialFace: the target material index in the property editor. 0: "Front", 1: "Back", 2: "Side"
		/// @param _normalMapIntensity: normal map intensity value
		/// @return void
		virtual void SetNormalMapIntensity(int _fabricIndex, int _materialFace, float _normalMapIntensity){};

		/// @fn SetNormalMapIntensity(unsigned int _colorwayIndex, int fabricIndex, int _materialFace, float _normalMapIntensity)
		/// @brief Set normal map intensity in percent
		/// @parm _colorwayIndex: index of colorway
		/// @param _fabricIndex: the target fabric index in the object browser
		/// @param _materialFace: the target material index in the property editor. 0: "Front", 1: "Back", 2: "Side"
		/// @param _normalMapIntensity: normal map intensity value
		/// @return void
		virtual void SetNormalMapIntensity(unsigned int _colorwayIndex, int fabricIndex, int _materialFace, float _normalMapIntensity) {};
		
		/// @fn GetMaterialType(int _fabricIndex, int _materialFace)
		/// @brief Get material type
		/// @param _fabricIndex: the target fabric index in the object browser
		/// @param _materialFace: the target material index in the property editor. 0: "Front", 1: "Back", 2: "Side"
		/// @return material type
		virtual int GetMaterialType(int _fabricIndex, int _materialFace) { return -1.0f; };

		/// @fn GetMaterialType(int _colorwayIndex, int _fabricIndex, int _materialFace)
		/// @brief Get material type
		/// @parm _colorwayIndex: index of colorway
		/// @param _fabricIndex: the target fabric index in the Colorway Editor
		/// @param _materialFace: the target material index in the property editor. 0: "Front", 1: "Back", 2: "Side"
		/// @return material type
		virtual int GetMaterialType(int _colorwayIndex, int _fabricIndex, int _materialFace) { return -1; };

		/// @fn SetMaterialType(int _fabricIndex, int _materialFace, int _materialType)
		/// @brief Set materal type
		/// @param _fabricIndex: the target fabric index in the object browser
		/// @param _materialFace: the target material index in the property editor. 0: "Front", 1: "Back", 2: "Side"
		/// @param _normalMapIntensity: normal map intensity value
		/// @return void
		virtual void SetMaterialType(int _fabricIndex, int _materialFace, int _materialType){};

		/// @fn SetMaterialType(int _colorwayIndex, int _fabricIndex, int _materialFace, int _materialType)
		/// @brief Set materal type
		/// @parm _colorwayIndex: index of colorway
		/// @param _fabricIndex: the target fabric index in the Colorway Editor
		/// @param _materialFace: the target material index in the property editor. 0: "Front", 1: "Back", 2: "Side"
		/// @param _normalMapIntensity: normal map intensity value
		/// @return void
		virtual void SetMaterialType(int _colorwayIndex, int _fabricIndex, int _materialFace, int _materialType) {};

		/// @fn GetOpacityIntensity(int _fabricIndex, int _materialFace)
		/// @brief Get opacity intensity in percent
		/// @param _fabricIndex: the target fabric index in the object browser
		/// @param _materialFace: the target material index in the property editor. 0: "Front", 1: "Back", 2: "Side"
		/// @return opaicty intensity value
		virtual float GetOpacityIntensity(int _fabricIndex, int _materialFace) { return 0.0f; };

		/// @fn GetOpacityIntensity(int _colorwayIndex, int _fabricIndex, int _materialFace)
		/// @brief Get opacity intensity in percent
		/// @parm _colorwayIndex: index of colorway
		/// @param _fabricIndex: the target fabric index in the Colorway Editor
		/// @param _materialFace: the target material index in the property editor. 0: "Front", 1: "Back", 2: "Side"
		/// @return opaicty intensity value
		virtual float GetOpacityIntensity(int _colorwayIndex, int _fabricIndex, int _materialFace) { return 0.0f; };

		/// @fn SetOpacityIntensity(int _fabricIndex, int _materialFace, float _normalMapIntensity)
		/// @brief Set opaicty intensity in percent
		/// @param _fabricIndex: the target fabric index in the object browser
		/// @param _materialFace: the target material index in the property editor. 0: "Front", 1: "Back", 2: "Side"
		/// @param _opacityIntensity: opaicty intensity value
		/// @return void
		virtual void SetOpacityIntensity(int _fabricIndex, int _materialFace, float _opacityIntensity) {};

		/// @fn SetOpacityIntensity(int _colorwayIndex, int _fabricIndex, int _materialFace, float _opacityIntensity)
		/// @brief Set opaicty intensity in percent
		/// @parm _colorwayIndex: index of colorway
		/// @param _fabricIndex: the target fabric index in the Colorway Editor
		/// @param _materialFace: the target material index in the property editor. 0: "Front", 1: "Back", 2: "Side"
		/// @param _opacityIntensity: opaicty intensity value
		/// @return void
		virtual void SetOpacityIntensity(int _colorwayIndex, int _fabricIndex, int _materialFace, float _opacityIntensity) {};

		virtual std::string GetBaseTextureMapImageFilePath() = 0;
		virtual std::string GetNormalMapImageFilePath() = 0;
		virtual std::string GetDisplacementMapImageFilePath() = 0;
		virtual std::string GetOpacityMapImageFilePath() = 0;
		virtual std::string GetRoughnessMapImageFilePath() = 0;
		virtual std::string GetMetalnessMapImageFilePath() = 0;

		virtual std::wstring GetBaseTextureMapImageFilePathW() = 0;
		virtual std::wstring GetNormalMapImageFilePathW() = 0;
		virtual std::wstring GetDisplacementMapImageFilePathW() = 0;
		virtual std::wstring GetOpacityMapImageFilePathW() = 0;
		virtual std::wstring GetRoughnessMapImageFilePathW() = 0;
		virtual std::wstring GetMetalnessMapImageFilePathW() = 0;

		virtual std::string GetBaseTextureMapImageFilePath(int _fabricIndex) = 0;
		virtual std::string GetBaseTextureMapImageFilePath(int _colorwayIndex, int _fabricIndex) = 0;
		virtual std::string GetNormalMapImageFilePath(int _fabricIndex) = 0;
		virtual std::string GetNormalMapImageFilePath(int _colorwayIndex, int _fabricIndex) = 0;
		virtual std::string GetDisplacementMapImageFilePath(int _fabricIndex) = 0;
		virtual std::string GetDisplacementMapImageFilePath(int _colorwayIndex, int _fabricIndex) = 0;
		virtual std::string GetOpacityMapImageFilePath(int _fabricIndex) = 0;
		virtual std::string GetOpacityMapImageFilePath(int _colorwayIndex, int _fabricIndex) = 0;
		virtual std::string GetRoughnessMapImageFilePath(int _fabricIndex) = 0;
		virtual std::string GetRoughnessMapImageFilePath(int _colorwayIndex, int _fabricIndex) = 0;
		virtual std::string GetMetalnessMapImageFilePath(int _fabricIndex) = 0;
		virtual std::string GetMetalnessMapImageFilePath(int _colorwayIndex, int _fabricIndex) = 0;

		virtual std::wstring GetBaseTextureMapImageFilePathW(int _fabricIndex) = 0;
		virtual std::wstring GetBaseTextureMapImageFilePathW(int _colorwayIndex, int _fabricIndex) = 0;
		virtual std::wstring GetNormalMapImageFilePathW(int _fabricIndex) = 0;
		virtual std::wstring GetNormalMapImageFilePathW(int _colorwayIndex, int _fabricIndex) = 0;
		virtual std::wstring GetDisplacementMapImageFilePathW(int _fabricIndex) = 0;
		virtual std::wstring GetDisplacementMapImageFilePathW(int _colorwayIndex, int _fabricIndex) = 0;
		virtual std::wstring GetOpacityMapImageFilePathW(int _fabricIndex) = 0;
		virtual std::wstring GetOpacityMapImageFilePathW(int _colorwayIndex, int _fabricIndex) = 0;
		virtual std::wstring GetRoughnessMapImageFilePathW(int _fabricIndex) = 0;
		virtual std::wstring GetRoughnessMapImageFilePathW(int _colorwayIndex, int _fabricIndex) = 0;
		virtual std::wstring GetMetalnessMapImageFilePathW(int _fabricIndex) = 0;
		virtual std::wstring GetMetalnessMapImageFilePathW(int _colorwayIndex, int _fabricIndex) = 0;

		virtual void SetBaseTextureMapImageGivenFilePath(const std::string _imageFilePath) = 0;
		virtual void SetNormalMapImageGivenFilePath(const std::string _imageFilePath) = 0;
		virtual void SetDisplacementMapImageGivenFilePath(const std::string _imageFilePath) = 0;
		virtual void SetOpacityMapImageGivenFilePath(const std::string _imageFilePath) = 0;
		virtual void SetRoughnessMapImageGivenFilePath(const std::string _imageFilePath) = 0;
		virtual void SetMetalnessMapImageGivenFilePath(const std::string _imageFilePath) = 0;

		virtual void SetBaseTextureMapImageGivenFilePathW(const std::wstring _imageFilePath) = 0;
		virtual void SetNormalMapImageGivenFilePathW(const std::wstring _imageFilePath) = 0;
		virtual void SetDisplacementMapImageGivenFilePathW(const std::wstring _imageFilePath) = 0;
		virtual void SetOpacityMapImageGivenFilePathW(const std::wstring _imageFilePath) = 0;
		virtual void SetRoughnessMapImageGivenFilePathW(const std::wstring _imageFilePath) = 0;
		virtual void SetMetalnessMapImageGivenFilePathW(const std::wstring _imageFilePath) = 0;

		virtual void SetBaseTextureMapImageGivenFilePath(const std::string _imageFilePath, int _fabricIndex) = 0;
		virtual void SetBaseTextureMapImageGivenFilePath(const std::string _imageFilePath, int _colorwayIndex, int _fabricIndex) = 0;
		virtual void SetNormalMapImageGivenFilePath(const std::string _imageFilePath, int _fabricIndex) = 0;
		virtual void SetNormalMapImageGivenFilePath(const std::string _imageFilePath, int _colorwayIndex, int _fabricIndex) = 0;
		virtual void SetDisplacementMapImageGivenFilePath(const std::string _imageFilePath, int _fabricIndex) = 0;
		virtual void SetDisplacementMapImageGivenFilePath(const std::string _imageFilePath, int _colorwayIndex, int _fabricIndex) = 0;
		virtual void SetOpacityMapImageGivenFilePath(const std::string _imageFilePath, int _fabricIndex) = 0;
		virtual void SetOpacityMapImageGivenFilePath(const std::string _imageFilePath, int _colorwayIndex, int _fabricIndex) = 0;
		virtual void SetRoughnessMapImageGivenFilePath(const std::string _imageFilePath, int _fabricIndex) = 0;
		virtual void SetRoughnessMapImageGivenFilePath(const std::string _imageFilePath, int _colorwayIndex, int _fabricIndex) = 0;
		virtual void SetMetalnessMapImageGivenFilePath(const std::string _imageFilePath, int _fabricIndex) = 0;
		virtual void SetMetalnessMapImageGivenFilePath(const std::string _imageFilePath, int _colorwayIndex, int _fabricIndex) = 0;

		virtual void SetBaseTextureMapImageGivenFilePathW(const std::wstring _imageFilePath, int _fabricIndex) = 0;
		virtual void SetBaseTextureMapImageGivenFilePathW(const std::wstring _imageFilePath, int _colorwayIndex, int _fabricIndex) = 0;
		virtual void SetNormalMapImageGivenFilePathW(const std::wstring _imageFilePath, int _fabricIndex) = 0;
		virtual void SetNormalMapImageGivenFilePathW(const std::wstring _imageFilePath, int _colorwayIndex, int _fabricIndex) = 0;
		virtual void SetDisplacementMapImageGivenFilePathW(const std::wstring _imageFilePath, int _fabricIndex) = 0;
		virtual void SetDisplacementMapImageGivenFilePathW(const std::wstring _imageFilePath, int _colorwayIndex, int _fabricIndex) = 0;
		virtual void SetOpacityMapImageGivenFilePathW(const std::wstring _imageFilePath, int _fabricIndex) = 0;
		virtual void SetOpacityMapImageGivenFilePathW(const std::wstring _imageFilePath, int _colorwayIndex, int _fabricIndex) = 0;
		virtual void SetRoughnessMapImageGivenFilePathW(const std::wstring _imageFilePath, int _fabricIndex) = 0;
		virtual void SetRoughnessMapImageGivenFilePathW(const std::wstring _imageFilePath, int _colorwayIndex, int _fabricIndex) = 0;
		virtual void SetMetalnessMapImageGivenFilePathW(const std::wstring _imageFilePath, int _fabricIndex) = 0;
		virtual void SetMetalnessMapImageGivenFilePathW(const std::wstring _imageFilePath, int _colorwayIndex, int _fabricIndex) = 0;

		/// @fn ExportGLTFAsFabric()
		/// @brief Export gltf/glb which contains the fabric data selected in the object browser
		/// @return Output file path;output files will be created in CLO temporary folder. If an error occurs, return empty string.
		virtual std::vector<std::string> ExportGLTFAsFabric()
		{
			std::vector<std::string> interface_vector;
			return interface_vector;
		}

		/// @fn ExportGLTFAsFabric(const string& _filePath)
		/// @brief Export gltf/glb which contains the fabric data selected in the object browser
		/// @param _filePath: output file path
		/// @return Output file path. If an error occurs, return empty string. 
		virtual std::vector<std::string> ExportGLTFAsFabric(const std::string& _filePath)
		{
			std::vector<std::string> interface_vector;
			return interface_vector;
		}

		/// @fn ExportGLTFAsFabric(const string& _filePath, int index)
		/// @brief Export gltf/glb which contains the fabric data in the index of the object browser
		/// @param _filePath: output file path
		/// @param index: target fabric index on the object browser to export
		/// @return Output file path. If an error occurs, return empty string. 
		virtual std::vector<std::string> ExportGLTFAsFabric(const std::string& _filePath, int index)
		{
			std::vector<std::string> interface_vector;
			return interface_vector;
		}

		/// @fn ExportGLTFAsFabricW(const wstring& _filePath, int index)
		/// @brief Export gltf/glb file which contains the fabric data in the index of the object browser
		/// @param _filePath: output file path
		/// @param index: target fabric index on the object browser to export
		/// @return Output file path. If an error occurs, return empty string. 
		virtual std::vector<std::wstring> ExportGLTFAsFabricW(const std::wstring& _filePath, int index)
		{
			std::vector<std::wstring> interface_vector;
			return interface_vector;
		}

		/// @fn ImportGLTFAsFabric(const string& _filePath)
		/// @brief Import gltf/glb as Fabrics into the Object Browser
		/// @param _filePath: input file path
		/// @return the index list of added fabric in object browser for the current project
		virtual std::vector<int> ImportGLTFAsFabric(const std::string& _filePath)
		{
			std::vector<int> interface_vector;
			return interface_vector;
		}

		/// @fn ImportGLTFAsFabric(const string& _filePath, int index)
		/// @brief Import gltf/glb as a Fabric into the Object Browser
		/// @param _filePath: input file path
		/// @param _index: target material index on the gltf/glb file to import as a fabric
		/// @return the index of added fabric in object browser for the current project
		virtual int ImportGLTFAsFabric(const std::string& _filePath, int _index) { return -1; }

		/// @fn ImportGLTFAsFabricW(const wstring& _filePath, int index)
		/// @brief Import gltf/glb as Fabrics into the Object Browser
		/// @param _filePath: input file path
		/// @return the index list of added fabric in object browser for the current project
		virtual std::vector<int> ImportGLTFAsFabricW(const std::wstring& _filePath)
		{
			std::vector<int> interface_vector;
			return interface_vector;
		}

		/// @fn ImportGLTFAsFabricW(const wstring& _filePath, int index)
		/// @brief Import gltf/glb as a Fabric into the Object Browser
		/// @param _filePath: input file path
		/// @param _index: target material index on the gltf/glb file to import as a fabric
		/// @return the index of added fabric in object browser for the current project
		virtual int ImportGLTFAsFabricW(const std::wstring& _filePath, int _index) {	return -1; }

		/// @fn GetSpecificColorwayFabricInfo(int _fabricIndex, int _colorwayIndexForStyleChip)
		/// @brief Get a fabric information for a colorway
		/// @param _fabricIndex: the fabricIndex for the fabric to get the fabric info
		/// @param _colorwayIndexForStyleChip: the colorway index that you want to get StyleChip information
		/// @return json string for fabric information
		virtual std::string GetSpecificColorwayFabricInfo(int _fabricIndex, int _colorwayIndexForStyleChip) { return ""; }

		/// @fn GetColorwayFabricInfoW(int _fabricIndex, int _colorwayIndexForStyleChip)
		/// @brief Get a fabric information for a colorway
		/// @param _fabricIndex: the fabricIndex for the fabric to get the fabric info
		/// @param _colorwayIndexForStyleChip: the colorway index that you want to get StyleChip information
		/// @return json string for fabric information
		virtual std::wstring GetSpecificColorwayFabricInfoW(int _fabricIndex, int _colorwayIndexForStyleChip) { return L""; }

		/// @fn GetSpecificColorwayFabricInfo(int _colorwayIndexForFabric, int _fabricIndex, int _colorwayIndexForStyleChip)
		/// @brief Get a fabric information for a colorway
		/// @param _colorwayIndexForFabric: the colorway index for the fabric to get the fabric info
		/// @param _fabricIndex: the fabricIndex for the fabric to get the fabric info in Colorway Editor
		/// @param _colorwayIndexForStyleChip: the colorway index that you want to get StyleChip information
		/// @return json string for fabric information
		virtual std::string GetSpecificColorwayFabricInfo(int _colorwayIndexForFabric, int _fabricIndex, int _colorwayIndexForStyleChip) { return ""; }

		/// @fn GetColorwayFabricInfoW(int _colorwayIndexForFabric, int _fabricIndex, int _colorwayIndexForStyleChip)
		/// @brief Get a fabric information for a colorway
		/// @param _colorwayIndexForFabric: the colorway index for the fabric to get the fabric info
		/// @param _fabricIndex: the fabricIndex for the fabric to get the fabric info in Colorway Editor
		/// @param _colorwayIndexForStyleChip: the colorway index that you want to get StyleChip information
		/// @return json string for fabric information that only includes 1 StyleChip information in (_colorwayIndexForStyleChip) 
		virtual std::wstring GetSpecificColorwayFabricInfoW(int _colorwayIndexForFabric, int _fabricIndex, int _colorwayIndexForStyleChip) { return L""; }

		/// @fn GetFabricItemNo(int _fabricIndex)
		/// @brief Get a fabric information for a colorway
		/// @param _colorwayIndexForFabric: the colorway index for the fabric to get the fabric info
		/// @param _fabricIndex: the fabricIndex for the fabric to get the fabric info in Object Browser
		/// @return Item No string 
		virtual std::string GetFabricItemNo(int _fabricIndex) { return ""; }

		/// @fn GetFabricItemNo(int _colorwayIndexForFabric, int _fabricIndex)
		/// @brief Get a Fabric item NO
		/// @param _colorwayIndexForFabric: the colorway index for the fabric to get the fabric info
		/// @param _fabricIndex: the fabricIndex for the fabric to get the fabric info in Colorway Editor
		/// @return Item No string 
		virtual std::string GetFabricItemNo(int _colorwayIndexForFabric, int _fabricIndex) { return ""; }

		/// @fn GetFabricItemNoW(int _fabricIndex)
		/// @brief Get a Fabric item NO
		/// @param _colorwayIndexForFabric: the colorway index for the fabric to get the fabric info
		/// @param _fabricIndex: the fabricIndex for the fabric to get the fabric info in Object Browser
		/// @return Item No string 
		virtual std::wstring GetFabricItemNoW(int _fabricIndex) { return L""; }

		/// @fn GetFabricItemNoW(int _colorwayIndexForFabric, int _fabricIndex)
		/// @brief Get a Fabric item NO
		/// @param _colorwayIndexForFabric: the colorway index for the fabric to get the fabric info
		/// @param _fabricIndex: the fabricIndex for the fabric to get the fabric info in Colorway Editor
		/// @return Item No string 
		virtual std::wstring GetFabricItemNoW(int _colorwayIndexForFabric, int _fabricIndex) { return L""; }

		/// @fn SetFabricItemNo(int _fabricIndex, const std::string& _itemNo)
		/// @brief Set a Fabric item NO
		/// @param _colorwayIndexForFabric: the colorway index for the fabric to get the fabric info
		/// @param _fabricIndex: the fabricIndex for the fabric to get the fabric info in Object Browser
		/// @return if it succeeds, return true.
		virtual bool SetFabricItemNo(int _fabricIndex, const std::string& _itemNo) { return false; }

		/// @fn SetFabricItemNo(int _colorwayIndexForFabric, int _fabricIndex, const std::string& _itemNo)
		/// @brief Set a Fabric item NO
		/// @param _colorwayIndexForFabric: the colorway index for the fabric to get the fabric info
		/// @param _fabricIndex: the fabricIndex for the fabric to get the fabric info in Colorway Editor
		/// @return if it succeeds, return true.
		virtual bool SetFabricItemNo(int _colorwayIndexForFabric, int _fabricIndex, const std::string& _itemNo) { return false; }

		/// @fn SetFabricItemNoW(int _fabricIndex, const std::wstring& _itemNo)
		/// @brief Set a Fabric item NO
		/// @param _colorwayIndexForFabric: the colorway index for the fabric to get the fabric info
		/// @param _fabricIndex: the fabricIndex for the fabric to get the fabric info in Object Browser
		/// @return if it succeeds, return true.
		virtual bool SetFabricItemNoW(int _fabricIndex, const std::wstring& _itemNo) { return false; }

		/// @fn SetFabricItemNoW(int _colorwayIndexForFabric, int _fabricIndex, const std::wstring& _itemNo)
		/// @brief Set a Fabric item NO
		/// @param _colorwayIndexForFabric: the colorway index for the fabric to get the fabric info
		/// @param _fabricIndex: the fabricIndex for the fabric to get the fabric info in Colorway Editor
		/// @return if it succeeds, return true.
		virtual bool SetFabricItemNoW(int _colorwayIndexForFabric, int _fabricIndex, const std::wstring& _itemNo) { return false; }

		/// @fn ExtractSBSARInputToJson(int _fabricIndex, int _colorwayIndex, int materialFace, const std::wstring& _savePath)
		/// @brief Extract SBSAR InputData to Json (Only Headless)
		/// @param _fabricIndex : Fabric Index
		/// @param _colorwayIndex : Colorway Index
		/// @parm materialFace: the target material index in the property editor. 0: "Front", 1: "Back", 2: "Side"
		/// @param _savePath : Save path
		/// @return the index of added fabric in object browser for the current project
		virtual std::string ExtractSBSARInputToJson(int _fabricIndex, int _colorwayIndex, int materialFace, const std::wstring& _savePath) { return ""; };

	};
}
