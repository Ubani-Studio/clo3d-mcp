#pragma once


#include <vector>

#include "DefineDllForWin.h"

#include "CloApiData.h"

#if defined(__APPLE__)
#include <string>
#endif

namespace CLOAPI
{
	class MV_CLO_INTERFACE_API ExportAPIInterface {
	public:

		/// @fn ExportZPac()
		/// @brief Export ZPac file in the CLO temporary folder
		/// @return Output file path. If an error occurs, return empty string. If filePath parameter is not given, output files will be created in CLO temporary folder.
		virtual std::string ExportZPac() { return ""; }

		/// @fn ExportZPac(const string& _filePath)
		/// @brief Export ZPac file
		/// @param string _filePath : target file path in ASCII
		/// @return Output file path. If an error occurs, return empty string. If filePath parameter is not given, output files will be created in CLO temporary folder.
		virtual std::string ExportZPac(const std::string& _filePath) { return ""; }

		/// @fn ExportZPacW(const wstring& _filePath)
		/// @brief Export ZPac file
		/// @param wstring _filePath : target file path in UNICODE
		/// @return Output file path. If an error occurs, return empty string. If filePath parameter is not given, output files will be created in CLO temporary folder.
		virtual std::wstring ExportZPacW(const std::wstring& _filePath) { return L""; }
		
		/// @fn ExportZPrj()
		/// @brief Export ZPrj file 
		/// @return Output file path; output files will be created in CLO temporary folder.
		virtual std::string ExportZPrj() { return ""; }

		/// @fn ExportZPrj(const string& _filePath)
		/// @brief Export ZPrj file 
		/// @param _filePath: output file path
		/// @return Output file path. If an error occurs, return empty string.
		virtual std::string ExportZPrj(const std::string& _filePath) { return ""; }

		/// @fn ExportZPrj(const string& _filePath, bool _bCreateThumbnail)
		/// @brief Export ZPrj file 
		/// @param _filePath: output file path
		/// @param _bCreateThumbnail: if _bCreateThumbnail is true, 'png' file will be created along 'zprj' file.
		/// @return Output file path. If an error occurs, return empty string.
		virtual std::string ExportZPrj(const std::string& _filePath, bool _bCreateThumbnail) { return ""; }

		/// @fn ExportZPrjW(const wstring& _filePath, bool _bCreateThumbnail)
		/// @brief Export ZPrj file 
		/// @param _filePath: output file path
		/// @param _bCreateThumbnail: if _bCreateThumbnail is true, 'png' file will be created along 'zprj' file.
		/// @return Output file path. If an error occurs, return empty wstring.
		virtual std::wstring ExportZPrjW(const std::wstring& _filePath, bool _bCreateThumbnail) { return L""; }
		
		///	@fn ExportOBJ()
		/// @brief Export OBJ file
		/// @param options If "options" is given, it exports OBJ according to options, not allowing user for selecting options in Export Dialog.
		/// @return Output file paths. It outputs the file paths of an OBJ file and multiple MTL files for colorways. If the value "bSaveInZip" in ImportExportOption is true, it outputs a file path of a zipped file. If an error occurs, return empty string. If filePath parameter is not given, output files will be created in CLO temporary folder.
		virtual std::vector<std::string> ExportOBJ()
		{
			std::vector<std::string> interface_vector;
			return interface_vector;
		}

		///	@fn ExportOBJ()
		/// @brief Export OBJ file
		/// @param _filePath: output file path
		/// @return Output file paths. It outputs the file paths of an OBJ file and multiple MTL files for colorways. If the value "bSaveInZip" in ImportExportOption is true, it outputs a file path of a zipped file. If an error occurs, return empty string. If filePath parameter is not given, output files will be created in CLO temporary folder.
		virtual std::vector<std::string> ExportOBJ(const std::string& _filePath)
		{
			std::vector<std::string> interface_vector;
			return interface_vector;
		}

		///	@fn ExportOBJ(const Marvelous::ImportExportOption& _options)
		/// @brief Export OBJ file
		/// @param options: If "options" is given, it exports OBJ according to options, not allowing user for selecting options in Export Dialog.
		/// @return Output file paths. It outputs the file paths of an OBJ file and multiple MTL files for colorways. If the value "bSaveInZip" in ImportExportOption is true, it outputs a file path of a zipped file. If an error occurs, return empty string. If filePath parameter is not given, output files will be created in CLO temporary folder.
		virtual std::vector<std::string> ExportOBJ(const Marvelous::ImportExportOption& _options)
		{
			std::vector<std::string> interface_vector;
			return interface_vector;
		}

		///	@fn ExportOBJ(const string& _filePath, const Marvelous::ImportExportOption& _options)
		/// @brief Export OBJ file
		/// @param _filePath: output file path
		/// @param options: If "options" is given, it exports OBJ according to options, not allowing user for selecting options in Export Dialog.
		/// @return Output file paths. It outputs the file paths of an OBJ file and multiple MTL files for colorways. If the value "bSaveInZip" in ImportExportOption is true, it outputs a file path of a zipped file. If an error occurs, return empty string. If filePath parameter is not given, output files will be created in CLO temporary folder.
		virtual std::vector<std::string> ExportOBJ(const std::string& _filePath, const Marvelous::ImportExportOption& _options)
		{
			std::vector<std::string> interface_vector;
			return interface_vector;
		}

		///	@fn ExportOBJW(const wstring& _filePath, const Marvelous::ImportExportOption& _options)
		/// @brief Export OBJ file
		/// @param _filePath: output file path
		/// @param options: If "options" is given, it exports OBJ according to options, not allowing user for selecting options in Export Dialog.
		/// @return Output file paths. It outputs the file paths of an OBJ file and multiple MTL files for colorways. If the value "bSaveInZip" in ImportExportOption is true, it outputs a file path of a zipped file. If an error occurs, return empty wstring. If filePath parameter is not given, output files will be created in CLO temporary folder.
		virtual std::vector<std::wstring> ExportOBJW(const std::wstring& _filePath, const Marvelous::ImportExportOption& _options)
		{
			std::vector<std::wstring> interface_vector;
			return interface_vector;
		}

		/// @fn ExportGLTF(const string& _filePath, const Marvelous::ImportExportOption& _options)
		///	@brief Export GLTF
		///	@param options: If "options" is given, it exports GLTF according to options, not allowing user for selecting options in Export Dialog.
		///	@return Output file paths. 
		virtual std::vector<std::string> ExportGLTF(const std::string& _filePath, const Marvelous::ImportExportOption& _options, bool _bGLBinary)
		{
			std::vector<std::string> interface_vector;
			return interface_vector;
		}

		/// @fn ExportGLTFW(const wstring& _filePath, const Marvelous::ImportExportOption& _options)
		///	@brief Export GLTFW
		///	@param options: If "options" is given, it exports GLTF according to options, not allowing user for selecting options in Export Dialog.
		///	@return Output file paths. 
		virtual std::vector<std::wstring> ExportGLTFW(const std::wstring& _filePath, const Marvelous::ImportExportOption& _options, bool _bGLBinary)
		{
			std::vector<std::wstring> interface_vector;
			return interface_vector;
		}

		/// @fn ExportAlembic(const string& _filePath, const Marvelous::ImportExportOption& _options)
		/// @brief Export Alembic
		/// @param options: If "options" is given, it exports Alembic according to options, not allowing user for selecting options in Export Dialog.
		/// @return Output file paths.
		virtual std::vector<std::string> ExportAlembic(const std::string& _filePath, const Marvelous::ImportExportOption& _options)
		{
			std::vector<std::string> interface_vector;
			return interface_vector;
		}

		/// @fn ExportAlembicW(const wstring& _filePath, const Marvelous::ImportExportOption& _options)
		/// @brief Export Alembic W
		/// @param options: If "options" is given, it exports Alembic according to options, not allowing user for selecting options in Export Dialog.
		/// @return Output file paths.
		virtual std::vector<std::wstring> ExportAlembicW(const std::wstring& _filePath, const Marvelous::ImportExportOption& _options)
		{
			std::vector<std::wstring> interface_vector;
			return interface_vector;
		}
				
		/// @fn ExportDXF()
		/// @brief Export DXF file. This api call will display the dialog to set options
		/// @return Output file path. If an error occurs, return empty string. Output files will be created in CLO temporary folder.
		virtual std::string ExportDXF() { return ""; }

		/// @fn ExportDXF(const string& _filePath)
		/// @brief Export DXF file. This api call will display the dialog to set options
		/// @param _filePath: output file path to export dxf
		/// @return Output file path. If an error occurs, return empty string. 
		virtual std::string ExportDXF(const std::string& _filePath) { return ""; }

		/// @fn ExportDXF(const string& _filePath, const Marvelous::ExportDxfOption& _exportOption)
		/// @brief Export DXF file without the option dialog. Param ExportDxfOption is applicable for the options.
		/// @param _filePath: output file path to export dxf
		/// @param _exportOption: options to export dxf file
		/// @return Output file path. If an error occurs, return empty string. 
		virtual std::string ExportDXF(const std::string& _filePath, const Marvelous::ExportDxfOption& _exportOption) { return ""; } // without dialog

		/// @fn ExportDXF(const string& _filePath, const Marvelous::ExportDxfOption& _exportOption)
		/// @brief Export DXF file without the option dialog. Param ExportDxfOption is applicable for the options.
		/// @param _filePath: output file path
		/// @param _exportOption: options to export dxf file
		/// @return Output file path. If an error occurs, return empty wstring. 
		virtual std::wstring ExportDXFW(const std::wstring& _filePath, const Marvelous::ExportDxfOption& _exportOption) { return L""; } // without dialog

		/// @fn ExportTechPack(const string& _filePath, const Marvelous::ExportTechpackOption& _exportOption)
		/// @brief Export Tech Pack data in json file and associated image files.
		/// @param _filePath: output filepath; the filePath Should be given in "*.json" format
		/// @param _exportOption: options to export techpack(.json) file
		virtual void ExportTechPack(const std::string& _filePath, const Marvelous::ExportTechpackOption& _exportOption) {}

		/// @fn ExportTechPackW(const wstring& _filepath, const Marvelous::ExportTechpackOption& _exportOption)
		/// @brief Export Tech Pack data in json file and associated image files.
		/// @param _filePath: output filepath; the filePath Should be given in "*.json" format
		/// @param _exportOption: options to export techpack(.json) file
		virtual void ExportTechPackW(const std::wstring& _filepath, const Marvelous::ExportTechpackOption& _exportOption) {}

		/// @fn ExportTechPackToStream(string& _outputImageFolderPath)
		/// @brief Export Tech Pack data in string and associated image files.
		/// @param _outputImageFolderPath: get output folder path where the image files are located by ExportTechpack
		/// @return Output stream for JSON data with the output folder path at the first line. If an error occurs, return empty string.
		virtual std::string ExportTechPackToStream(std::string& _outputImageFolderPath) { _outputImageFolderPath = "";  return ""; }

		/// @fn ExportTechPackToStreamW(wstring& _outputImageFolderPath)
		/// @brief Export Tech Pack data in string and associated image files.
		/// @param _outputImageFolderPath: get output folder path where the image files are located by ExportTechpack
		/// @return Output stream for JSON data with the output folder path at the first line. If an error occurs, return empty wstring.
		virtual std::wstring ExportTechPackToStreamW(std::wstring& _outputImageFolderPath) { _outputImageFolderPath = L"";  return L""; }

		/// @fn ExportThumbnail3D()
		/// @brief Export thumbnail of the current scene
		/// @return Output file path: output files will be created in CLO temporary folder.
		virtual std::string ExportThumbnail3D() { return ""; }
		
		/// @fn ExportThumbnail3D(const string& _filePath)
		/// @brief Export thumbnail of the current scene in the 3D Windows
		/// @param _filePath: output file path to save the screenshot
		/// @return Output file path. If an error occurs, return empty string. 
		virtual std::string ExportThumbnail3D(const std::string& _filePath) { return ""; }

		/// @fn ExportThumbnail3D(const string& _filePath)
		/// @brief Export thumbnail of the current scene in the 3D Windows
		/// @param _filePath: output file path to save the screenshot
		/// @return Output file path. If an error occurs, return empty wstring. 
		virtual std::wstring ExportThumbnail3DW(const std::wstring& _filePath) { return L""; }

		/// @fn ExportMaskSnapshot3D(const std::string& _filePath, const Marvelous::ExportTechpackOption& _exportOption)
		/// @brief Export mask snapshot of the current scene in the 3D windows. only for H&M.
		/// @param _filePath: output file path to save the screenshot.
		/// @param _exportOption: export option include autentication key.
		virtual std::vector< std::vector < std::string> > ExportMaskSnapshot3D(const std::string& _filePath, const Marvelous::ExportTechpackOption& _exportOption)
		{
			std::vector < std::vector < std::string > > interface_vector;
			return interface_vector;
		}
		
		/// @fn ExportSnapshot3D(const string& _filePath)
		/// @brief Export snapshot images. This function displays the same dialog as CLO so that users can configure the snapshots. If user turns on the option "Save Separate Images", then series of images will be saved with the name followed by the postfix "_01", "_02", ...
		/// @param _filePath: output file path to save the screenshot
		/// @return Return the list of the path of output files per colorway. The first item of each array is the file path of the unified image. If filePath parameter is not given, output files will be created in CLO temporary folder.	
		virtual std::vector< std::vector < std::string > > ExportSnapshot3D(const std::string& _filePath)
		{
			std::vector< std::vector < std::string > > interface_vector;
			return interface_vector;
		}

		/// @fn ExportSnapshot3D()
		/// @brief Export snapshot images. This function displays the same dialog as CLO so that users can configure the snapshots. If user turns on the option "Save Separate Images", then series of images will be saved with the name followed by the postfix "_01", "_02", ...		
		/// @return Return the list of the path of output files per colorway. The first item of each array is the file path of the unified image. If filePath parameter is not given, output files will be created in CLO temporary folder.	
		virtual std::vector< std::vector < std::string > > ExportSnapshot3D()
		{
			std::vector< std::vector < std::string > > interface_vector;
			return interface_vector;
		}

		/// @fn ExportSnapshot3DW(const wstring& _filePath)
		/// @brief Export snapshot images. This function displays the same dialog as CLO so that users can configure the snapshots. If user turns on the option "Save Separate Images", then series of images will be saved with the name followed by the postfix "_01", "_02", ...
		/// @param _filePath: output file path to save the screenshot
		/// @return Return the list of the path of output files per colorway. The first item of each array is the file path of the unified image. If filePath parameter is not given, output files will be created in CLO temporary folder.	
		virtual std::vector< std::vector < std::wstring > > ExportSnapshot3DW(const std::wstring& _filePath)
		{
			std::vector< std::vector < std::wstring > > interface_vector;
			return interface_vector;
		}

		/// @fn ExportCustomViewSnapshot(const string& _targetFolderPath, unsigned int _width, unsigned int _height, string _outputPrefix = "")
		/// @brief Export snapshot images of Custom View.
		/// @param _targetFolderPath: output folder path
		/// @param _width: image width for the snapshots
		/// @param _height: image height for the snapshots
		/// @param _outputPrefix: If you set the 'outputPrefix' as empty value - "", then the saved file names will start with the index in the custom view; or if you set the 'outputPrefix' as some letters, the saved file names will have it as the first letters.
		/// @return Return the list of the path of output files
		virtual std::vector < std::string > ExportCustomViewSnapshot(const std::string& _targetFolderPath, unsigned int _width, unsigned int _height, std::string _outputPrefix = "")
		{
			std::vector<std::string> interface_vector;
			return interface_vector;
		}

		/// @fn ExportCustomViewSnapshotW(const wstring& _targetFolderPath, unsigned int _width, unsigned int _height, wstring _outputPrefix = L"")
		/// @brief Export snapshot images of Custom View.
		/// @param _targetFolderPath: output folder path
		/// @param _width: image width for the snapshots
		/// @param _height: image height for the snapshots
		/// @param _outputPrefix: If you set the 'outputPrefix' as empty value - "", then the saved file names will start with the index in the custom view; or if you set the 'outputPrefix' as some letters, the saved file names will have it as the first letters.
		/// @return Return the list of the path of output files
		virtual std::vector < std::wstring > ExportCustomViewSnapshotW(const std::wstring& _targetFolderPath, unsigned int _width, unsigned int _height, std::wstring _outputPrefix = L"")
		{
			std::vector<std::wstring> interface_vector;
			return interface_vector;
		}

		/// @fn ExportRenderingImage(const string& _filePath)
		/// @brief Export Rendering Image.
		/// @param _filePath: output file path
		/// @return Return the list of the path of output files per colorway. 
		virtual std::vector< std::vector < std::string > > ExportRenderingImage(const std::string& _filePath)
		{
			std::vector< std::vector < std::string > > interface_vector;
			return interface_vector;
		}

		/// @fn ExportRenderingImage(const string& _filePath, bool _bRenderAllColorways)
		/// @brief Export Rendering Image.
		/// @param _filePath: output file path
		/// @param _bRenderAllColorways: If true, output the images for all colorways. Otherwise, it returns the images for the current colorway specified by CLO user.
		/// @return Return the list of the path of output files per colorway. 
		virtual std::vector< std::vector < std::string > > ExportRenderingImage(const std::string& _filePath, bool _bRenderAllColorways)
		{
			std::vector< std::vector < std::string > > interface_vector;
			return interface_vector;
		}

		/// @fn ExportRenderingImage(bool _bRenderAllColorways)
		/// @brief Export Rendering Image.
		/// @param _bRenderAllColorways: If true, output the images for all colorways. Otherwise, it returns the images for the current colorway specified by CLO user.
		/// @return Return the list of the path of output files per colorway. 
		virtual std::vector< std::vector < std::string > > ExportRenderingImage(bool _bRenderAllColorways)
		{
			std::vector< std::vector < std::string > > interface_vector;
			return interface_vector;
		}

		/// @fn ExportRenderingImageW(const wstring& _filePath, bool _bRenderAllColorways)
		/// @brief Export Rendering Image.
		/// @param _filePath: output file path
		/// @param _bRenderAllColorways: If true, output the images for all colorways. Otherwise, it returns the images for the current colorway specified by CLO user.
		/// @return Return the list of the path of output files per colorway. 
		virtual std::vector< std::vector < std::wstring > > ExportRenderingImageW(const std::wstring& _filePath, bool _bRenderAllColorways)
		{
			std::vector< std::vector < std::wstring > > interface_vector;
			return interface_vector;
		}

		/// @fn ExportSingleColorwayRenderingImage(const string& _filePath, unsigned int _colorwayIndex)
		/// @brief Export Rendering Image for the colorway
		/// @param _filePath: output file path
		/// @param _colorwayIndex: colorway index to render the image
		/// @return Return the list of the path of output files
		virtual std::vector < std::string > ExportSingleColorwayRenderingImage(const std::string& _filePath, unsigned int _colorwayIndex)
		{
			std::vector < std::string > interface_vector;
			return interface_vector;
		}

		/// @fn ExportSingleColorwayRenderingImage(unsigned int _colorwayIndex)
		/// @brief Export Rendering Image for the colorway
		/// @param _colorwayIndex: colorway index to render the image
		/// @return Return the list of the path of output files; output files will be created in CLO temporary folder.	
		virtual std::vector < std::string > ExportSingleColorwayRenderingImage(unsigned int _colorwayIndex)
		{
			std::vector < std::string > interface_vector;
			return interface_vector;
		}

		/// @fn ExportSingleColorwayRenderingImageW(const wstring& _filePath, unsigned int _colorwayIndex)
		/// @brief Export Rendering Image for the colorway
		/// @param _filePath: output file path
		/// @param _colorwayIndex: colorway index to render the image
		/// @return Return the list of the path of output files	
		virtual std::vector < std::wstring > ExportSingleColorwayRenderingImageW(const std::wstring& _filePath, unsigned int _colorwayIndex)
		{
			std::vector < std::wstring > interface_vector;
			return interface_vector;
		}

		/// @fn GetTotalRenderImagePaths()
		/// @brief Get File name list for all the exported rendering images
		/// @return all the rendered image file paths
		virtual std::vector < std::string >  GetTotalRenderImagePaths()
		{
			std::vector < std::string > interface_vector;
			return interface_vector;
		}

		/// @fn GetTotalRenderImagePathsW()
		/// @brief Get File name list for all the exported rendering images
		/// @return all the rendered image file paths
		virtual std::vector < std::wstring >  GetTotalRenderImagePathsW()
		{
			std::vector < std::wstring > interface_vector;
			return interface_vector;
		}

		/// @fn GetCurrentRenderImagePaths()
		/// @brief Current Render Image paths
		/// @return all current renderded image files paths per colorways
		virtual std::vector < std::string >  GetCurrentRenderImagePaths()
		{
			std::vector < std::string > interface_vector;
			return interface_vector;
		}

		/// @fn GetCurrentRenderImagePathsW()
		/// @brief Current Render Image paths
		/// @return all current renderded image files paths per colorways
		virtual std::vector < std::wstring >  GetCurrentRenderImagePathsW()
		{
			std::vector < std::wstring > interface_vector;
			return interface_vector;
		}

		/// @fn GetFileNameOnRenderingProperty()
		/// @return the output file names set on the Rendering Property in CLO
		virtual std::string GetFileNameOnRenderingProperty()
		{
			std::string str = "";
			return str;
		}

		/// @fn GetFileNameOnRenderingPropertyW()
		/// @return the output file names set on the Rendering Property in CLO
		virtual std::wstring GetFileNameOnRenderingPropertyW()
		{
			std::wstring str = L"";
			return str;
		}

		/// @fn ExportGarmentInformation()
		/// @brief Export garment information in json file. The information is the same as given in Garment Information Dialog in CLO (you can see this dialog by clicking File > Information > Garment menu in CLO)
		////@return Output file path. If an error occurs, return empty string; output files will be created in CLO temporary folder.
		virtual std::string ExportGarmentInformation() { return ""; }

		/// @fn ExportGarmentInformation(const string& _filePath)
		/// @brief Export garment information in json file. The information is the same as given in Garment Information Dialog in CLO (you can see this dialog by clicking File > Information > Garment menu in CLO)
		/// @param _filePath output file path to export the garment information
		////@return Output file path. If an error occurs, return empty string. 
		virtual std::string ExportGarmentInformation(const std::string& _filePath) { return ""; }

		/// @fn ExportGarmentInformationW(const wstring& _filePath)
		/// @brief Export garment information in json file. The information is the same as given in Garment Information Dialog in CLO (you can see this dialog by clicking File > Information > Garment menu in CLO)
		/// @param _filePath output file path to export the garment information
		////@return Output file path. If an error occurs, return empty string. 
		virtual std::wstring ExportGarmentInformationW(const std::wstring& _filePath) { return L""; }

		/// @fn ExportGarmentInformationToStream()
		/// @brief Export garment information in string
		/// @return Garment Information stream in sstring
		virtual std::string ExportGarmentInformationToStream() { return ""; }

		/// @fn ExportGarmentInformationToStreamW()
		/// @brief Export garment information in string as well as json file
		/// @return Garment Information stream in wstring
		virtual std::wstring ExportGarmentInformationToStreamW() { return L""; }
		
		/// @fn ExportGarmentInformationConfigData()
		/// @brief Export garment information config data in json file. The information is the same as given in "Conf_Garment_Information.json" file via Preference
		/// @return Output file path; the output files will be created in CLO temporary folder. If an error occurs, return empty string.
		virtual std::string ExportGarmentInformationConfigData() { return ""; }

		/// @fn ExportGarmentInformationConfigData(const string& _filePath)
		/// @brief Export garment information config data in json file. The information is the same as given in "Conf_Garment_Information.json" file via Preference
		/// @param _filePath output file path to export the garment information configuration data
		/// @return Output file path. If an error occurs, return empty string.
		virtual std::string ExportGarmentInformationConfigData(const std::string& _filePath) { return ""; }

		/// @fn ExportGarmentInformationConfigDataW(const wstring& _filePath)
		/// @brief Export garment information config data in json file. The information is the same as given in "Conf_Garment_Information.json" file via Preference
		/// @param _filePath output file path to export the garment information configuration data
		/// @return Output file path. If an error occurs, return empty string.
		virtual std::wstring ExportGarmentInformationConfigDataW(const std::wstring& _filePath) { return L""; }

		/// @fn ExportGarmentInformationConfigDataToStream()
		///	Export garment information configuration data in string
		/// @return Output stream for JSON data. If an error occurs, return empty string.
		virtual std::string ExportGarmentInformationConfigDataToStream() { return ""; }

		/// @fn ExportGarmentInformationConfigDataToStreamW()
		///	Export garment information configuration data in string
		/// @return Output stream for JSON data. If an error occurs, return empty string.
		virtual std::wstring ExportGarmentInformationConfigDataToStreamW() { return L""; }

		/// @fn ExportTurntableVideo()
		/// @brief Export turntable video. This function requires XVid Mpeg-4 codec installed on user's computer.
		/// @return Output file path; output files will be created in CLO temporary folder. If an error occurs, return empty string.
		virtual std::string ExportTurntableVideo() { return ""; }

		/// @fn ExportTurntableVideo(const string& _filePath)
		/// @brief Export turntable video. This function requires XVid Mpeg-4 codec installed on user's computer.
		/// @param _filePath 
		/// @return Output file path; output files will be created in CLO temporary folder. If an error occurs, return empty string.
		virtual std::string ExportTurntableVideo(const std::string& _filePath) { return ""; }

		/// @fn ExportTurntableVideoW(const wstring& _filePath)
		/// @brief Export turntable video. This function requires XVid Mpeg-4 codec installed on user's computer.
		/// @return Output file path; output files will be created in CLO temporary folder. If an error occurs, return empty string. 
		virtual std::wstring ExportTurntableVideoW(const std::wstring& _filePath) { return L""; }

		/// @fn ExportAnimationVideo()
		/// @brief Export animation video. This function requires XVid Mpeg-4 codec installed on user's computer.
		/// @return Output file path. If an error occurs, return empty string. If filePath parameter is not given, output files will be created in CLO temporary folder.
		virtual std::string ExportAnimationVideo() { return ""; }

		/// @fn ExportAnimationVideo()
		/// @brief Export animation video. This function requires XVid Mpeg-4 codec installed on user's computer.
		/// @return Output file path. If an error occurs, return empty string. If filePath parameter is not given, output files will be created in CLO temporary folder.
		virtual std::string ExportAnimationVideo(const std::string& _filePath) { return ""; }

		/// @fn ExportAnimationVideo()
		/// @brief Export animation video. This function requires XVid Mpeg-4 codec installed on user's computer.
		/// @return Output file path. If an error occurs, return empty string. If filePath parameter is not given, output files will be created in CLO temporary folder.
		virtual std::wstring ExportAnimationVideoW(const std::wstring& _filePath) { return L""; }

		//////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////

		/// @fn GetColorwayCount()
		/// @brief Get the number of colorways in the current garment loaded in CLO
		/// @return total count of colorways		
		virtual unsigned int GetColorwayCount() { return 0; }

		/// @fn GetCurrentColorwayIndex()
		/// @brief Get the index of the current colorway in CLO
		/// @return the current colorway index
		virtual unsigned int GetCurrentColorwayIndex() { return 0; }

		/// @fn GetColorwayNameList()
		/// @brief Get all the colorway names for the current garment 
		/// @return the list of name of all colorways
		virtual std::vector<std::string> GetColorwayNameList()
		{
			std::vector < std::string > interface_vector;
			return interface_vector;
		}

		/// @fn GetColorwayNameListW()
		/// @brief Get all the colorway names for the current garment 
		/// @return the list of name of all colorways
		virtual std::vector<std::wstring> GetColorwayNameListW()
		{
			std::vector < std::wstring > interface_vector;
			return interface_vector;
		}

		/// @fn GetAvatarCount()
		/// @brief Get the number of avatars loaded in CLO
		/// @return total count of avatars		
		virtual unsigned int GetAvatarCount() { return 0; }

		/// @fn GetAvatarNameList()
		/// @brief Get all the names of avatars loaded in CLO
		/// @return the list of name of all avatars
		virtual std::vector<std::string> GetAvatarNameList()
		{
			std::vector < std::string > interface_vector;
			return interface_vector;
		}

		/// @fn GetAvatarNameListW()
		/// @brief Get all the names of avatars loaded in CLO
		/// @return the list of name of all avatars
		virtual std::vector<std::wstring> GetAvatarNameListW()
		{
			std::vector < std::wstring > interface_vector;
			return interface_vector;
		}

		/// @fn GetAvatarGenderList()
		/// @brief Get all the genders of avatars loaded in CLO repectively
		/// @return the list of gender of all avatars. 0 : male, 1 : female, -1: unknown
		virtual std::vector<int> GetAvatarGenderList()
		{
			std::vector < int > interface_vector;
			return interface_vector;
		}
		
		/// @fn ExportTurntableImages(int _numberOfImages)
		/// @brief Export turntable images for current colorway.
		/// @param _numberOfImages: the number of images in 360 turn table. The turn table will turn by (360 / _numberOfImages) per an image each by each.
		/// @return Output file path list; output files will be created in CLO temporary folder. If an error occurs, return empty string.
		virtual std::vector<std::string> ExportTurntableImages(int _numberOfImages)
		{
			std::vector < std::string > interface_vector;
			return interface_vector;
		}

		/// @fn ExportTurntableImages(const string& _filePath, int _numberOfImages, int _width = 2500, int _height = 2500) 
		/// @brief Export turntable images for current colorway.
		/// @param _filePath: the output file path to export the turntable snapshots
		/// @param _numberOfImages: the number of images in 360 turn table. The turn table will turn by (360 / _numberOfImages) per an image each by each.
		/// @param _width: image width for the snapshots
		/// @param _height: image height for the snapshots
		/// @return Output file path list. If an error occurs, return empty string. 
		virtual std::vector<std::string> ExportTurntableImages(const std::string& _filePath, int _numberOfImages, int _width = 2500, int _height = 2500) 
		{ 
			std::vector < std::string > interface_vector;
			return interface_vector;
		}

		/// @fn ExportTurntableImagesW(const wstring& _filePath, int _numberOfImages, int _width = 2500, int _height = 2500)
		/// @brief Export turntable images for current colorway.
		/// @param _filePath: the output file path to export the turntable snapshots
		/// @param _numberOfImages: the number of images in 360 turn table. The turn table will turn by (360 / _numberOfImages) per an image each by each.
		/// @param _width: image width for the snapshots
		/// @param _height: image height for the snapshots
		/// @return Output file path list. If an error occurs, return empty string. 
		virtual std::vector<std::wstring> ExportTurntableImagesW(const std::wstring& _filePath, int _numberOfImages, int _width = 2500, int _height = 2500)
		{ 
			std::vector < std::wstring > interface_vector;
			return interface_vector;
		}

		/// @fn ExportGLB(const string& _filePath, const Marvelous::ImportExportOption& _options)
		///	@brief Export GLB
		///	@param options: If "options" is given, it exports GLB according to options, not allowing user for selecting options in Export Dialog.
		///	@return Output file paths. 
		virtual std::vector<std::string> ExportGLB(const std::string& _filePath, const Marvelous::ImportExportOption& _options)
		{
			std::vector<std::string> interface_vector;
			return interface_vector;
		}

		/// @fn ExportGLBW(const wstring& _filePath, const Marvelous::ImportExportOption& _options)
		///	@brief Export GLBW
		///	@param options: If "options" is given, it exports GLB according to options, not allowing user for selecting options in Export Dialog.
		///	@return Output file paths. 
		virtual std::vector<std::wstring> ExportGLBW(const std::wstring& _filePath, const Marvelous::ImportExportOption& _options)
		{
			std::vector<std::wstring> interface_vector;
			return interface_vector;
		}

		/// @fn ExportPOM(()
		/// @brief Export POM
		/// @return Output file path;output file will be created in CLO temporary folder. If an error occurs, return empty string.
		virtual std::string ExportPOM() { return ""; }	

		/// @fn ExportPOM(const string& _filePath)
		/// @brief Export POM
		/// @param _filePath: output file path
		/// @return Output file path. If an error occurs, return empty string. 
		virtual std::string ExportPOM(const std::string& _filePath) { return ""; }

		/// @fn ExportPOM(const wstring& _filePath)
		/// @brief Export POM
		/// @param _filePath: output file path	
		/// @return Output file path. If an error occurs, return empty string. 
		virtual std::wstring ExportPOMW(const std::wstring& _filePath) { return L""; }

		/// @fn ExportPOM(()
		/// @brief Export POM
		/// @param _bInclude3DLength, true: include 3D info, false: exclude 3D info 
		/// @return Output file path;output file will be created in CLO temporary folder. If an error occurs, return empty string.
		virtual std::string ExportPOM(bool _bInclude3DLength) { return ""; }

		/// @fn ExportPOM(const string& _filePath)
		/// @brief Export POM
		/// @param _filePath: output file path, _bInclude3DLength, true: include 3D info, false: exclude 3D info 
		/// @return Output file path. If an error occurs, return empty string. 
		virtual std::string ExportPOM(bool _bInclude3DLength, const std::string& _filePath) { return ""; }

		/// @fn ExportPOM(const wstring& _filePath)
		/// @brief Export POM
		/// @param _filePath: output file path, _bInclude3DLength, true: include 3D info, false: exclude 3D info 	
		/// @return Output file path. If an error occurs, return empty string. 
		virtual std::wstring ExportPOMW(bool _bInclude3DLength, const std::wstring& _filePath) { return L""; }

		/// @fn ExportBOM(const string& _filePath)
		/// @brief Export BOM
		/// @param _filePath: output file path
		/// @return If an error occurs, return false. 
		virtual bool ExportBOM(const std::string& _filePath) { return false; }
        
        /// @fn ExportBOM(const wstring& _filePath)
        /// @brief Export BOM
        /// @param _filePath: output file path
        /// @return If an error occurs, return false.
        virtual bool ExportBOMW(const std::wstring& _filePath) { return false; }

		/// @fn ExportGLTFWithDialog(const string& _filePath)
		///	@brief Export GLTF with Dialog
		/// @param _filePath: output file path
		///	@return Output file paths. 
		virtual std::vector<std::string> ExportGLTFWithDialog(const std::string& _filePath, bool _bGLBinary)
		{
			std::vector<std::string> interface_vector;
			return interface_vector;
		}

		/// @fn ExportGLBWithDialog(const string& _filePath)
		///	@brief Export GLB with Dialog
		/// @param _filePath: output file path
		///	@return Output file paths. 
		virtual std::vector<std::string> ExportGLBWithDialog(const std::string& _filePath)
		{
			std::vector<std::string> interface_vector;
			return interface_vector;
		}

		/// @fn ExportThumbnail3DByColorwayIndex(unsigned int _colorwayIndex)
		/// @brief Export thumbnail of the current scene
		/// @param _colorwayIndex: colorway index to render the image
		/// @return Output file path: output files will be created in CLO temporary folder.
		virtual std::string ExportThumbnail3DByColorwayIndex(unsigned int _colorwayIndex) { return ""; }

		/// @fn ExportThumbnail3DByColorwayIndex(const string& _filePath, unsigned int _colorwayIndex)
		/// @brief Export thumbnail of the current scene in the 3D Windows
		/// @param _filePath: output file path to save the screenshot
		/// @param _colorwayIndex: colorway index to render the image
		/// @return Output file path. If an error occurs, return empty string. 
		virtual std::string ExportThumbnail3DByColorwayIndex(const std::string& _filePath, unsigned int _colorwayIndex) { return ""; }

		/// @fn ExportThumbnail3DByColorwayIndexW(const wstring& _filePath, unsigned int _colorwayIndex)
		/// @brief Export thumbnail of the current scene in the 3D Windows
		/// @param _filePath: output file path to save the screenshot
		/// @param _colorwayIndex: colorway index to render the image
		/// @return Output file path. If an error occurs, return empty wstring. 
		virtual std::wstring ExportThumbnail3DWByColorwayIndexW(const std::wstring& _filePath, unsigned int _colorwayIndex) { return L""; }

		/// @fn ExportTurntableImagesByColorwayIndex(int _numberOfImages, unsigned int _colorwayIndex)
		/// @brief Export turntable images for colorway.
		/// @param _numberOfImages: the number of images in 360 turn table. The turn table will turn by (360 / _numberOfImages) per an image each by each.
		/// @param _colorwayIndex: colorway index to render the image
		/// @return Output file path list; output files will be created in CLO temporary folder. If an error occurs, return empty string.
		virtual std::vector<std::string> ExportTurntableImagesByColorwayIndex(int _numberOfImages, unsigned int _colorwayIndex)
		{
			std::vector<std::string> interface_vector;
			return interface_vector;
		}

		/// @fn ExportTurntableImagesByColorwayIndex(const string& _filePath, int _numberOfImages, unsigned int _colorwayIndex, int _width = 2500, int _height = 2500) 
		/// @brief Export turntable images for colorway.
		/// @param _filePath: the output file path to export the turntable snapshots
		/// @param _numberOfImages: the number of images in 360 turn table. The turn table will turn by (360 / _numberOfImages) per an image each by each.
		/// @param _colorwayIndex: colorway index to render the image
		/// @param _width: image width for the snapshots
		/// @param _height: image height for the snapshots
		/// @return Output file path list. If an error occurs, return empty string. 
		virtual std::vector<std::string> ExportTurntableImagesByColorwayIndex(const std::string& _filePath, int _numberOfImages, unsigned int _colorwayIndex, int _width = 2500, int _height = 2500)
		{
			std::vector<std::string> interface_vector;
			return interface_vector;
		}

		/// @fn ExportTurntableImagesByColorwayIndexW(const wstring& _filePath, int _numberOfImages, unsigned int _colorwayIndex, int _width = 2500, int _height = 2500)
		/// @brief Export turntable images for colorway.
		/// @param _filePath: the output file path to export the turntable snapshots
		/// @param _numberOfImages: the number of images in 360 turn table. The turn table will turn by (360 / _numberOfImages) per an image each by each.
		/// @param _colorwayIndex: colorway index to render the image
		/// @param _width: image width for the snapshots
		/// @param _height: image height for the snapshots
		/// @return Output file path list. If an error occurs, return empty string. 
		virtual std::vector<std::wstring> ExportTurntableImagesByColorwayIndexW(const std::wstring& _filePath, int _numberOfImages, unsigned int _colorwayIndex, int _width = 2500, int _height = 2500)
		{
			std::vector<std::wstring> interface_vector;
			return interface_vector;
		}

		/// @fn ExportPose()
		/// @brief Export ZPac file in the CLO temporary folder
		/// @return Output file path. If an error occurs, return empty string. If filePath parameter is not given, output files will be created in CLO temporary folder.
		virtual std::string ExportPose() { return ""; }

		/// @fn ExportPose(const string& _filePath)
		/// @brief Export ZPac file
		/// @param string _filePath : target file path in ASCII
		/// @return Output file path. If an error occurs, return empty string. If filePath parameter is not given, output files will be created in CLO temporary folder.
		virtual std::string ExportPose(const std::string& _filePath) { return ""; }

		/// @fn ExportPoseW(const wstring& _filePath)
		/// @brief Export ZPac file
		/// @param wstring _filePath : target file path in UNICODE
		/// @return Output file path. If an error occurs, return empty string. If filePath parameter is not given, output files will be created in CLO temporary folder.
		virtual std::wstring ExportPoseW(const std::wstring& _filePath) { return L""; }

		///	@fn ExportFBX(const Marvelous::ImportExportOption& _options)
		/// @brief Export FBX file
		/// @param options: It exports FBX according to options, not allowing user for selecting options in Export Dialog.
		/// @return Output file paths. If filePath parameter is not given, output files will be created in CLO temporary folder.
		virtual std::vector<std::string> ExportFBX(const Marvelous::ImportExportOption& _options)
		{
			std::vector<std::string> interface_vector;
			return interface_vector;
		}

		///	@fn ExportFBX(const Marvelous::ImportExportOption& _options)
		/// @brief Export FBX file
		/// @param options: It exports FBX according to options, not allowing user for selecting options in Export Dialog.
		/// @param string _filePath : target file path in ASCII
		/// @return Output file paths.
		virtual std::vector<std::string> ExportFBX(const std::string& _filePath, const Marvelous::ImportExportOption& _options)
		{
			std::vector<std::string> interface_vector;
			return interface_vector;
		}

		///	@fn ExportFBX(const Marvelous::ImportExportOption& _options)
		/// @brief Export FBX file
		/// @param options: It exports FBX according to options, not allowing user for selecting options in Export Dialog.
		/// @param string _filePath : target file path in UNICODE
		/// @return Output file paths.
		virtual std::vector<std::wstring> ExportFBXW(const std::wstring& _filePath, const Marvelous::ImportExportOption& _options)
		{
			std::vector<std::wstring> interface_vector;
			return interface_vector;
		}

		/// @fn ExportAVT(const string& _filePath)
		/// @brief Export AVT
		/// @param _filePath: output file path
		/// @return Output file path. If an error occurs, return empty string. 
		virtual std::string ExportAVT(const std::string& _filePath) { return ""; }

		/// @fn ExportAVTW(const wstring& _filePath)
		/// @brief Export AVT
		/// @param _filePath: output file path	
		/// @return Output file path. If an error occurs, return empty string. 
		virtual std::wstring ExportAVTW(const std::wstring& _filePath) { return L""; }

		/// @fn ExportTrim(const std::string& _filePath, unsigned int _trimStyleIndex)
		/// @brief Export trim file
		/// @param _filePath: the output file path to export trim
		/// @param _trimStyleIndex: trimstyle index to export trim style
		/// @return Output file path list. If an error occurs, return empty string. 
		virtual std::string ExportTrim(const std::string& _filePath, unsigned int _trimStyleIndex) { return ""; }

		/// @fn ExportStdViewImage(int _viewIndex, const std::string& _outputFolderPath, int _colorwayIndex, int _width, int _height)
		/// @brief ExportStdViewImage : Exports The Image based on the given Standard image view index for selected colorway.
		/// @param : 
		/// @return Output file path along with the selected colorway name. If an error occurs, return error message string. 
		virtual std::string ExportStdViewImage(int _viewIndex, const std::string& _outputFolderPath, int _colorwayIndex, int _width, int _height) { return ""; }

		/// @fn ExportStdViewImageForAllColorways(int _viewIndex, const std::string& _outputFolderPath, int _width, int _height)
		/// @brief ExportStdViewImageForAllColorways : Exports The Image based on the given Standard image view index for all Colorways.
		/// @param : 
		/// @return Output file paths along with the colorway names. If an error occurs, return error message string. 
		virtual std::string ExportStdViewImageForAllColorways(int _viewIndex, const std::string& _outputFolderPath, int _width, int _height) { return ""; }

		/// @fn ExportCustomViewImage((const std::string& _zcmrFileUrl, const std::string& _outputFolderPath, int _colorwayIndex, int _width, int _height)
		/// @brief ExportCustomViewImage : Exports The Custom image based on the given custom angles in ZCMR file for selected Colorway.
		/// @param : 
		/// @return Output file path along with the selected colorway name. If an error occurs, return error message string. 
		virtual std::string ExportCustomViewImage(const std::string& _zcmrFileUrl, const std::string& _outputFolderPath, int _colorwayIndex, int _width, int _height) { return ""; }

		/// @fn ExportCustomViewImageForAllColorways((const std::string& _zcmrFileUrl, const std::string& _outputFolderPath, int _width, int _height)
		/// @brief ExportCustomViewImageForAllColorways : Exports The Custom image based on the given custom angles in ZCMR file for all Colorways.
		/// @param : 
		/// @return Output file path along with the colorway names. If an error occurs, return error message string. 
		virtual std::string ExportCustomViewImageForAllColorways(const std::string& _zcmrFileUrl, const std::string& _outputFolderPath, int _width, int _height) { return ""; }

		/// @fn ExportMultiViewImages(const std::string& _mvsFilePath, const std::string& _outputFilePath, int _colorwayIndex, int _width, int _height)
		/// @brief ExportMultiViewImages : Exports The Multi View Images based on the given MVS File for selected Colorway.
		/// @param : 
		/// @return Output file path along with the selected colorway name. If an error occurs, return error message string. 
		virtual std::string ExportMultiViewImages(const std::string& _mvsFilePath, const std::string& _outputFilePath, int _colorwayIndex, int _width, int _height) { return ""; }

		/// @fn ExportMultiViewImagesForAllColorways(const std::string& _mvsFilePath, const std::string& _outputFilePath, int _width, int _height)
		/// @brief ExportMultiViewImagesForAllColorways : Exports The Multi View Images based on the given MVS File for all Colorways.
		/// @param : 
		/// @return Output file path along with the colorway names. If an error occurs, return error message string. 
		virtual std::string ExportMultiViewImagesForAllColorways(const std::string& _mvsFilePath, const std::string& _outputFilePath, int _width, int _height) { return ""; }

		/// @fn ConvertZblcToZmod(const std::vector<std::wstring>& _zblcFilePathList, const std::wstring& _saveDirPath, const std::wstring& _categoryName, const std::wstring& _styleName)
		/// @brief Convert .zblc file to .zmod file
		/// @param _zblcFilePathList: The path of the ZBLC files you want to convert
		/// @param _saveDirPath: Where to save the converted file
		/// @param _categoryName : Category name
		/// @param _styleName : Style name
		/// @param _lineName : Line Name (This is not mandatory data. By default, an empty string is set.)
		/// @return Returns the path list of failed files. Returns an empty list if all files are successful.
		virtual std::vector<std::wstring> ConvertZblcToZmod(const std::vector<std::wstring>& _zblcFilePathList, const std::wstring& _saveDirPath, const std::wstring& _categoryName, const std::wstring& _styleName, const std::wstring& _lineName = L"") { return {}; }

		/// @fn ExportMVSData(const std::string& _mvsFilePath)
		/// @brief ExportMVSData : Exports The MVS File data without creating any Images.
		/// @param : 
		/// @return MVS file data and with custom angles. If an error occurs, return error message string. 
		virtual std::string ExportMVSData(const std::string& _mvsFilePath) { return ""; }

		///	@fn ExportUSD(const Marvelous::ImportExportOption& _options)
		/// @brief Export USD file
		/// @param options: It exports USD according to options, not allowing user for selecting options in Export Dialog.
		/// @return Output file paths. If filePath parameter is not given, output files will be created in CLO temporary folder.
		virtual std::vector<std::string> ExportUSD(const Marvelous::ImportExportOption& _options, const Marvelous::ExportUSDOption& _usdOptions)
		{
			std::vector<std::string> interface_vector;
			return interface_vector;
		}

		///	@fn ExportUSD(const Marvelous::ImportExportOption& _options)
		/// @brief Export USD file
		/// @param options: It exports USD according to options, not allowing user for selecting options in Export Dialog.
		/// @param string _filePath : target file path in ASCII
		/// @return Output file paths.
		virtual std::vector<std::string> ExportUSD(const std::string& _filePath, const Marvelous::ImportExportOption& _options, const Marvelous::ExportUSDOption& _usdOptions)
		{
			std::vector<std::string> interface_vector;
			return interface_vector;
		}

		///	@fn ExportUSD(const Marvelous::ImportExportOption& _options)
		/// @brief Export USD file
		/// @param options: It exports USD according to options, not allowing user for selecting options in Export Dialog.
		/// @param string _filePath : target file path in UNICODE
		/// @return Output file paths.
		virtual std::vector<std::wstring> ExportUSDW(const std::wstring& _filePath, const Marvelous::ImportExportOption& _options, const Marvelous::ExportUSDOption& _usdOptions)
		{
			std::vector<std::wstring> interface_vector;
			return interface_vector;
		}
	};
}
