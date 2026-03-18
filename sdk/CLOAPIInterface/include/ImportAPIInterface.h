#pragma once

#include <vector>

#include "DefineDllForWin.h"

#include "CloApiData.h"

namespace Marvelous
{
	class View3DBase;
	class SceneGraph;
	class PatternEditor;
	class Library;
}

namespace CLOAPI
{
	class MV_CLO_INTERFACE_API ImportAPIInterface {
	public:
		/// @fn ImportFile(const string& filePath)
		/// @brief Load File (zprj, zpac, avt, obj, fbx, zcmr). This function will show up the dialog per the file type.
		/// @param filePath: the input file path to load
		/// @return if it succeeds, return true.
		virtual bool ImportFile(const std::string& filePath) { return true; }

		/// @fn ImportFileW(const wstring& filePath)
		/// @brief Load File (zprj, zpac, avt, obj, fbx, zcmr). This function will show up the dialog per the file type.
		/// @param filePath: the input file path to load
		/// @return if it succeeds, return true.
		virtual bool ImportFileW(const std::wstring& filePath) { return true; }

		/// @fn ImportZprj(const string& filePath, const Marvelous::ImportZPRJOption& loadOption)
		/// @brief Load zprj File without the dialog but the loadOption
		/// @param filePath: the input file path to load
		/// @param loadOption: the options to load the file
		/// @return if it succeeds, return true.
		virtual bool ImportZprj(const std::string& filePath, const Marvelous::ImportZPRJOption& loadOption) { return true; }

		/// @fn ImportZprjW(const wstring& filePath, const Marvelous::ImportZPRJOption& loadOption)
		/// @brief Load zprj File without the dialog but the loadOption
		/// @param filePath: the input file path to load
		/// @param loadOption: the options to load the file
		/// @return if it succeeds, return true.
		virtual bool ImportZprjW(const std::wstring& filePath, const Marvelous::ImportZPRJOption& loadOption) { return true; }

		/// @fn ImportGarmentInformation(const string& filePath)
		/// @brief Load Garment Information (json)
		/// @param filePath: the input file to load the garment information (.json)
		/// @return if it succeeds, return true.
		virtual bool ImportGarmentInformation(const std::string& filePath) { return true; }

		/// @fn ImportGarmentInformationW(const wstring& filePath)
		/// @brief Load Garment Information (json)
		/// @param filePath: the input file to load the garment information (.json)
		/// @return if it succeeds, return true.
		virtual bool ImportGarmentInformationW(const std::wstring& filePath) { return true; }

		/// @fn ImportGarmentInformationConfigData(const string& filePath)
		/// @brief Load Garment Information Configuration (json)		
		/// @param filePath: the input file to load the garment information configuration data(.json)
		/// @return if it succeeds, return true.
		virtual bool ImportGarmentInformationConfigData(const std::string& filePath) { return true; }

		/// @fn ImportGarmentInformationConfigDataW(const wstring& filePath)
		/// @brief Load Garment Information Configuration (json)		
		/// @param filePath: the input file to load the garment information configuration data(.json)
		/// @return if it succeeds, return true.
		virtual bool ImportGarmentInformationConfigDataW(const std::wstring& filePath) { return true; }

		///	@fn ImportOBJ(const string& _filePath, const Marvelous::ImportExportOption& _options)
		/// @brief Import OBJ file
		/// @param _filePath: input file path
		/// @param options: If "options" is given, it imports OBJ according to options, not allowing user for selecting options in Import Dialog.
		/// @return if it succeeds, return true.
		virtual bool ImportOBJ(const std::string& _filePath, const Marvelous::ImportExportOption& _options)
		{
			return false;
		}

		///	@fn ImportOBJW(const wstring& _filePath, const Marvelous::ImportExportOption& _options)
		/// @brief Import OBJ file
		/// @param _filePath: input file path
		/// @param options: If "options" is given, it imports OBJ according to options, not allowing user for selecting options in Import Dialog.
		/// @return if it succeeds, return true.
		virtual bool ImportOBJW(const std::wstring& _filePath, const Marvelous::ImportExportOption& _options)
		{
			return false;
		}

		///	@fn ImportVMP(const string& _filePath)
		/// @brief Import VMP file
		/// @param _filePath: input file path
		/// @return if it succeeds, return true.
		virtual bool ImportVMP(const std::string& _filePath)
		{
			return false;
		}

		///	@fn ImportVMP(const wstring& _filePath)
		/// @brief Import VMP file
		/// @param _filePath: input file path
		/// @return if it succeeds, return true.
		virtual bool ImportVMPW(const std::wstring& _filePath)
		{
			return false;
		}

		///	@fn ImportCPT(const string& _filePath)
		/// @brief Import CPT file
		/// @param _filePath: input file path
		/// @return if it succeeds, return true.
		virtual bool ImportCPT(const std::string& _filePath)
		{
			return false;
		}

		///	@fn ImportCPT(const wstring& _filePath)
		/// @brief Import CPT file
		/// @param _filePath: input file path
		/// @return if it succeeds, return true.
		virtual bool ImportCPTW(const std::wstring& _filePath)
		{
			return false;
		}

		///	@fn ImportVLP(const string& _filePath)
		/// @brief Import VLP file
		/// @param _filePath: input file path
		/// @return if it succeeds, return true.
		virtual bool ImportVLP(const std::string& _filePath)
		{
			return false;
		}

		///	@fn ImportVLP(const wstring& _filePath)
		/// @brief Import VLP file
		/// @param _filePath: input file path
		/// @return if it succeeds, return true.
		virtual bool ImportVLPW(const std::wstring& _filePath)
		{
			return false;
		}

		///	@fn ImportVRP(const string& _filePath)
		/// @brief Import VRP file
		/// @param _filePath: input file path
		/// @return if it succeeds, return true.
		virtual bool ImportVRP(const std::string& _filePath)
		{
			return false;
		}

		///	@fn ImportVRP(const wstring& _filePath)
		/// @brief Import VRP file
		/// @param _filePath: input file path
		/// @return if it succeeds, return true.
		virtual bool ImportVRPW(const std::wstring& _filePath)
		{
			return false;
		}

		///	@fn ImportDXF(const string& _filePath, const Marvelous::ImportDxfOption& loadOption)
		/// @brief Import DXF file
		/// @param _filePath: input file path
		/// @param loadOption: the options to load the file
		/// @return if it succeeds, return true.
		virtual bool ImportDXF(const std::string& _filePath, const Marvelous::ImportDxfOption& loadOption) { return true; }


		///	@fn ImportDXF(const wstring& _filePath, const Marvelous::ImportDxfOption& loadOption)
		/// @brief Import DXF file
		/// @param _filePath: input file path
		/// @param loadOption: the options to load the file
		/// @return if it succeeds, return true.
		virtual bool ImportDXFW(const std::wstring& _filePath, const Marvelous::ImportDxfOption& loadOption) { return true; }


		///	@fn ImportImageAsGraphicStyle(const string& _filePath)
		/// @brief Import DXF file
		/// @param _filePath: input file path
		/// @return if it succeeds, return true.
		virtual bool ImportGraphicStyleFromImage(const std::string& _filePath) { return true; }

		///	@fn ImportImageAsGraphicStyle(const wstring& _filePath)
		/// @brief Import image file as graphic style
		/// @param _filePath: input file path
		/// @return if it succeeds, return true.
		virtual bool ImportGraphicStyleFromImage(const std::wstring& _filePath) { return true; }

		///	@fn ImportAvatar(const wstring& _filePath, const Marvelous::ImportExportOption& _importOption)
		/// @brief Import avac avatar file
		/// @param _filePath: input avatar file path
		/// @param _apfFilePath: input apf File path
		/// @return if it succeeds, return true.
		virtual bool ImportAVAC(const std::wstring& _filePath, const std::wstring& _apfFilePath) { return true; }
		virtual bool ImportAVAC(const std::string& _filePath, const std::string& _apfFilePath) { return true; }

		/// @fn ImportFile(const string& filePath, const Marvelous::ImportExportOption& _options)
		/// @brief Load File(obj, fbx, gltf) by obj type . This function will show up the dialog per the file type.
		/// @param filePath: the input file path to load, ImportExportOption - loadObjectType 0 : avater, 1 : trim
		/// @return if it succeeds, return true.
		virtual bool ImportFile(const std::string& filePath, const Marvelous::ImportExportOption& _options) { return true; }

		/// @fn ImportFileW(const wstring& filePath, const Marvelous::ImportExportOption& _options)
		/// @brief Load File(obj, fbx, gltf) by obj type. This function will show up the dialog per the file type.
		/// @param filePath: the input file path to load, ImportExportOption - loadObjectType 0 : avater, 1 : trim
		/// @return if it succeeds, return true.
		virtual bool ImportFileW(const std::wstring& filePath, const Marvelous::ImportExportOption& _options) { return true; }

		///	@fn ImportFBX(const string& _filePath, const Marvelous::ImportExportOption& _options)
		/// @brief Import FBX file
		/// @param _filePath: input file path
		/// @param options: If "options" is given, it imports FBX according to options, not allowing user for selecting options in Import Dialog.
		/// @return if it succeeds, return true.
		virtual bool ImportFBX(const std::string& _filePath, const Marvelous::ImportExportOption& _options)
		{
			return false;
		}

		///	@fn ImportFBXW(const wstring& _filePath, const Marvelous::ImportExportOption& _options)
		/// @brief Import FBX file
		/// @param _filePath: input file path
		/// @param options: If "options" is given, it imports FBX according to options, not allowing user for selecting options in Import Dialog.
		/// @return if it succeeds, return true.
		virtual bool ImportFBXW(const std::wstring& _filePath, const Marvelous::ImportExportOption& _options)
		{
			return false;
		}

		///	@fn ImportGLTF(const string& _filePath, const Marvelous::ImportExportOption& _options)
		/// @brief Import GLTF file
		/// @param _filePath: input file path
		/// @param options: If "options" is given, it imports GLTF according to options, not allowing user for selecting options in Import Dialog.
		/// @return if it succeeds, return true.
		virtual bool ImportGLTF(const std::string& _filePath, const Marvelous::ImportExportOption& _options)
		{
			return false;
		}

		///	@fn ImportGLTFW(const wstring& _filePath, const Marvelous::ImportExportOption& _options)
		/// @brief Import GLTF file
		/// @param _filePath: input file path
		/// @param options: If "options" is given, it imports GLTF according to options, not allowing user for selecting options in Import Dialog.
		/// @return if it succeeds, return true.
		virtual bool ImportGLTFW(const std::wstring& _filePath, const Marvelous::ImportExportOption& _options)
		{
			return false;
		}

		///	@fn ImportSMP(const string& _filePath)
		/// @brief Import SMP file
		/// @param _filePath: input file path
		/// @return if it succeeds, return true.
		virtual bool ImportSMP(const std::string& _filePath)
		{
			return false;
		}

		///	@fn ImportSMPW(const wstring& _filePath)
		/// @brief Import SMP file
		/// @param _filePath: input file path
		/// @return if it succeeds, return true.
		virtual bool ImportSMPW(const std::wstring& _filePath)
		{
			return false;
		}

		///	@fn ImportAsGraphic(const std::string& _filePath)
		/// @brief Import an image file as Graphic.
		/// @param _filePath: input file path
		virtual void ImportAsGraphic(const std::string& _filePath)
		{
		}

		///	@fn ImportAsGraphicW(const std::wstring& _filePath)
		/// @brief Import an image file as Graphic.
		/// @param _filePath: input file path
		virtual void ImportAsGraphicW(const std::wstring& _filePath)
		{
		}

		///	@fn ImportTrim(const std::string& _filePath)
		/// @brief Import a trim
		/// @param _filePath			: input file path
		/// @param _bTranslateToOrigin  : option to translate the position of the imported trim 
		///								  true: move trim to the pos(0,0)
		///								  false: move trim to original pos
		virtual void ImportTrim(const std::string& _filePath, const bool _bTranslateToOrigin)
		{
		}

		///	@fn ImportAvatarMeasurement(std::string _csvPath, std::string _avtPath, ImportExportOption _opt)
		/// @brief Import Avatar Measurement CSV file
		/// @param _csvPath: input CSV file path, _avtPath : input Avatar file path, _opt : input load options
		virtual bool ImportAvatarMeasurement(std::string _csvPath, std::string _avtPath, Marvelous::ImportExportOption _opt)
		{
			return false;
		}

		///	@fn ImportAvatar(std::string _avtPath, ImportExportOption _opt)
		/// @brief Import Avatar 
		/// @param _avtPath : input Avatar file path, _opt : input load options
		virtual bool ImportAvatar(std::string _avtPath, Marvelous::ImportExportOption _opt)
		{
			return false;
		}

		///	@fn ImportMeasurement(std::string _csvPath)
		/// @brief Import Avatar Measurement CSV file
		/// @param _csvPath: input CSV file path
		virtual bool ImportMeasurement(std::string _csvPath)
		{
			return false;
		}
    
		///	@fn ImportPose(const std::string& _filePath)
		/// @brief Import a pose/hand pose file.
		/// @param _filePath: input file path
		virtual bool ImportPose(const std::string& _filePath) 
		{
			return false;
		}

		///	@fn ImportPose(const std::wstring& _filePath)
		/// @brief Import a pose/hand pose file.
		/// @param _filePath: input file path
		virtual bool ImportPoseW(const std::wstring& _filePath)
		{
			return false;
		}

		///	@fn ImportAlembic(const std::string& _filePath, const Marvelous::ImportAlembicOption& option)
		/// @brief Import an alembic (.abc) file.
		/// @param _filePath: input file path
		/// @param options: 
		virtual bool ImportAlembic(const std::string& filePath, const Marvelous::ImportAlembicOption& option)
		{
			return false;
		}


	};
}