#pragma once

#include <map>
#include <string>
#include <vector>
#define EXPORT_PYTHON 
/**
	@file CloApiData.h
	@brief Pre defined Enumerator and option classes / structures
*/
namespace Marvelous
{
	/// @enum TEXTURE_SAVE_METHOD
	/// @brief Do Not Use this in CLO API/SDK side. This enumerator represents the "Texture" comobobox index for "Export Fbx" feature
	enum TEXTURE_SAVE_METHOD
	{
		RELATIVE_PATH,
		ABSOLUTE_PATH,
		SAVE_WITH_TEXTURE,
		EMBED_MEDIA
	};

	/// @enum WELD_TYPE
	/// @brief Weld type to export meshes. WELDED - merge meshes on seamlines, UNWELDED - as-is
	enum WELD_TYPE
	{
		FULLY_UNWELDED = 0, /// UNWELDED for all seamlines
		DEFAULT_WELDED = 1, /// deprecated, but DO NOT REMOVE.
		FULLY_WELDED = 2,	/// WELDED for all seamlines
		SELECTED_WELDED = 3 /// WELDED for selected seamlines
	};

	enum ALEMBIC_TYPE
	{
		ALEMBIC_OGAWA = 0,
		ALEMBIC_HDF5 = 1,
		ALEMBIC_MAX = 2
	};


	/// @class ImportExportOption
	/// @brief Options for Export OBJ and Export GLTF as parameters without dialogs in CLO API
	class ImportExportOption
	{
	public:

		/////////////////////////
		// Export OBJ / GLTF
		EXPORT_PYTHON bool bExportGarment; /// true : Export Garment (Cloth Meshes)
		EXPORT_PYTHON bool bExportAvatar;  /// true : Export Avatar (Avatar Meshes)

		EXPORT_PYTHON bool bSingleObject;  /// true : OBJ single object, false : OBJ multiple object
		EXPORT_PYTHON WELD_TYPE weldType;  /// FULLY_UNWELDED : unweld, FULLY_WELDED : weld, SELECTED_WELDED : Weld selected patterns in workspace, DEFAULT_WELDED : Do not use!!!

		EXPORT_PYTHON bool bThin;          /// if true, export the garment as thin mesh. Otherwise, export it as thick mesh.

		EXPORT_PYTHON bool bUnifiedUVCoordinates;   	  /// flag for Unified UV Coordinates
		EXPORT_PYTHON bool bCreateUnifiedTexture;   	  /// flag for Unified Texture option - if bUnifiedUVCoordinates is true, this value should be set in true as well.
		EXPORT_PYTHON float unifiedTextureSize;     	  /// true: (In case bCreateUnifiedTexture is true) The size of unified texture (pixels). Texture will be created in square.
		
		EXPORT_PYTHON int unifiedTextureFillSeamSize; 	  /// true: (In case bCreateUnifiedTexture is true) Fill texture seam size in pixels.
		EXPORT_PYTHON bool bUseInifinteSeams; 		/// true: (In case bCreateUnifiedTexture is true) Use infinite fill texture seam size.

		EXPORT_PYTHON float unifiedTextureBakeMargin;   /// true: (In case bCreateUnifiedTexture is true) The margin of the baked texture in the image canvas
		EXPORT_PYTHON bool unifiedTextureBakeRelateive; /// true: (In case bCreateUnifiedTexture is true)

		EXPORT_PYTHON bool bUnifiedDiffuseMap;		  /// false: (In case bCreateUnifiedTexture is true) export diffuse map baked in the unified uv
		EXPORT_PYTHON bool bUnifiedNormalMap; 		  /// false: (In case bCreateUnifiedTexture is true) export normal map baked in the unified uv
		EXPORT_PYTHON bool bUnifiedMetalnessMap;		  /// false: (In case bCreateUnifiedTexture is true) export metalness map baked in the unified uv
		EXPORT_PYTHON bool bUnifiedRoughnessMap;		  /// false: (In case bCreateUnifiedTexture is true) export roughness map baked in the unified uv
		EXPORT_PYTHON bool bUnifiedOpacityMap;		  /// false: (In case bCreateUnifiedTexture is true) export opacity map baked in the unified uv
		EXPORT_PYTHON bool bUnifiedDisplacementMap;	  /// false: (In case bCreateUnifiedTexture is true) export displacement map baked in the unified uv
		
		EXPORT_PYTHON bool bIncludeHiddenObject; 		  /// if true, export all the pattern meshes include 'hidden pattern meshes on 3D' to OBJ
		EXPORT_PYTHON bool bIncludeInnerShape;		  /// If true, export geometries of internal lines to ease the picking the internal lines in external modeling tools such as Maya and 3ds Max.

		EXPORT_PYTHON float scale; 					  /// how to scale the object created in CLO when exporting. The default unit of CLO is millimeters for obj/fbx(mm) and gltf(m)
					 					  /// use 0.001 for gltf in default 

		EXPORT_PYTHON int axisX; /// the index of the axis which the X axis will be converted to. If you don't want axis conversion, set this value as "0"
		EXPORT_PYTHON int axisY; /// the index of the axis which the Y axis will be converted to. If you don't want axis conversion, set this value as "1"
		EXPORT_PYTHON int axisZ; /// the index of the axis which the Z axis will be converted to. If you don't want axis conversion, set this value as "2"
		EXPORT_PYTHON bool bInvertX; /// invert the axis which the X axis will be converted to
		EXPORT_PYTHON bool bInvertY; /// invert the axis which the Y axis will be converted to
		EXPORT_PYTHON bool bInvertZ; /// invert the axis which the Z axis will be converted to

		EXPORT_PYTHON bool bSaveColorWays; /// if true, export for all the colorways.

		EXPORT_PYTHON bool bSaveInZip; 		/// If true, export as a zipped file with textures. It will output a single zipped file(.zip), not separated files.
		EXPORT_PYTHON bool bDiffuseColorCombined; /// if true, the texture will be exported after being combined with diffuse color	
		EXPORT_PYTHON bool bExcludeAmbient; /// if true, exclude ambient map and color in MTL files.
		EXPORT_PYTHON bool bOpacityMap; 	  /// if true, generate an opacity map by extracting alpha channel from diffuse map
		EXPORT_PYTHON bool bMetaData;   	  /// export meta data
						  	  // end of Export OBJ
						  	  /////////////////////////

		/////////////////////////
		//	for glTF export only
		EXPORT_PYTHON bool bEmbedded;				/// for embedded-gltf export option
		EXPORT_PYTHON bool bExportLight;			/// export light information in gltf
		EXPORT_PYTHON bool bExportFabric;			/// true : Export Fabric (Fabric information)
		EXPORT_PYTHON bool bExportAnimation;		/// true : Export Garment Animation in gltf
		EXPORT_PYTHON bool bSaveColorWaysSingleFile;	/// true : export gltf with KHR_materials_variants extension (affect when bSaveColorWays is true)

					 	  	  /////////////////////////
					 	  	  //	for FBX export only (These options are not available on CLO API)
		bool bAscii; 		  /// fbx ascii or binary
		int  fbxSdkVersion;   /// 0 : 2016, 1: 2014/2015, 2: 2013, 3 : 2012
		TEXTURE_SAVE_METHOD textureSaveMethod; /// 0: Relative Path, 1: Absolute Path, 2: Save with textures(in Zip file), 3: Embed Media
		bool bIncludeSceneProp;

		/////////////////////////
		// for vertex cache only (These options are not available on CLO API)
		float timeScale;  	 /// The index of the axis which the X axis will be converted to. If you don't want axis conversion, set this value as "0"
		unsigned int fps; 	 /// FPS to import/export vertex cache 

	  	/////////////////////////
	  	// for Alembic only (These options are not available on CLO API)
		float anim_step;
		bool bExportShutterSpeed;
		float shutter_low;
		float shutter_high;
		bool m_bExportStrainColorMap;
		bool m_bExportStressColorMap;
		ALEMBIC_TYPE m_alembicType;

		/////////////////////////
		// for ZBrush, Alembic only (These options are not available on CLO API)
		bool bIncludeAvatarShape;

		bool bIncludeSkeleton;

		EXPORT_PYTHON std::string m_AuthenticationKeyForAPI; /// For Alvanon to use extended functionalies in CLO API/SDK

		EXPORT_PYTHON int ImportObjectType; /// import file type. 0 : Avatar, 1: Trim, 2 : Garment

		EXPORT_PYTHON bool bAutoTranslate; /// If true, allign bottom to ground
		EXPORT_PYTHON bool bCreateCamera; /// If true, create object camera
		EXPORT_PYTHON bool bCreateAnimation; /// If true, create object animation
		EXPORT_PYTHON bool bCreateCacheAnimation; /// If true, create object cache animation
		EXPORT_PYTHON bool bMoveGarment; /// If true, move garment to start position
		EXPORT_PYTHON bool bAddArrangementPoints; /// If true, auto add arrangement points
		EXPORT_PYTHON bool bAutoCreateFittingSuit; /// If true, auto create fitting suit
		EXPORT_PYTHON bool bAdd; /// If true, set load type as add


		EXPORT_PYTHON float translationValueX;		/// set x-offset value per the position-value in 3D Window for Avatar/Garment (left - right) unit (mm)
		EXPORT_PYTHON float translationValueY;		/// set y-offset value per the position-value in 3D Window for Avatar/Garment (up - down) unit (mm)		
		EXPORT_PYTHON float translationValueZ;		/// set z-offset value per the position-value in 3D Window for Avatar/Garment (monitor - user) unit (mm)
	
		EXPORT_PYTHON bool bTrace2DPatternsUVMap;	/// If true, set load type for trace 2d patterns from uv map

		EXPORT_PYTHON bool bCreateMetallicRoughnessMap;	/// true : (In case bCreateUnifiedTexture is false) export metallic-roughness map baked using diffuse alpha value (only in gltf)

		ImportExportOption()
		{
			bExportGarment = true;
			bExportAvatar = true;

			bSingleObject = false;
			weldType = FULLY_UNWELDED;

			bThin = true;

			bUnifiedUVCoordinates = false;
			bCreateUnifiedTexture = false;
			unifiedTextureSize = 2048.0f;
			unifiedTextureFillSeamSize = 10;
			bUseInifinteSeams = false;
			bUnifiedDiffuseMap = false;
			bUnifiedNormalMap = false;
			bUnifiedMetalnessMap = false;
			bUnifiedRoughnessMap = false;
			bUnifiedOpacityMap = false;
			bUnifiedDisplacementMap = false;

			unifiedTextureBakeRelateive = true;
			unifiedTextureBakeMargin = 0.01f;

			bIncludeHiddenObject = false;
			bIncludeInnerShape = false;

			scale = 1.0f;

			axisX = 0;
			axisY = 1;
			axisZ = 2;
			bInvertX = false;
			bInvertY = false;
			bInvertZ = false;

			bSaveColorWays = false;

			bSaveInZip = false;
			bDiffuseColorCombined = false;
			bExcludeAmbient = false;
			bOpacityMap = false;
			bMetaData = false;

			bEmbedded = false; 
			bExportLight = false;
			bExportFabric = false;
			bExportAnimation = false;

			////
			bAscii = false;
			fbxSdkVersion = 0;
			textureSaveMethod = RELATIVE_PATH;

			anim_step = 1.0f;
			bExportShutterSpeed = false;
			shutter_low = 0.0f;
			shutter_high = 0.0f;
			m_bExportStrainColorMap = false;
			m_bExportStressColorMap = false;
			m_alembicType = ALEMBIC_TYPE::ALEMBIC_OGAWA;

			bIncludeAvatarShape = true;
			bIncludeSkeleton = true;
			bIncludeSceneProp = true;
			//
			timeScale = 1.0f;
			fps = 30;

			ImportObjectType = 0;
			bAutoTranslate = false;
			bCreateCamera = false;
			bCreateAnimation = false;
			bCreateCacheAnimation = false;
			bMoveGarment = false;
			bAddArrangementPoints = false;
			bAutoCreateFittingSuit = false;
			bAdd = false;
			translationValueX = 0.0f;
			translationValueY = 0.0f;
			translationValueZ = 0.0f;
			bTrace2DPatternsUVMap = false;

			bCreateMetallicRoughnessMap = false;
			
		}
		virtual ~ImportExportOption()
		{}

		void init(const ImportExportOption& _rhs)
		{
			bExportGarment = _rhs.bExportGarment;
			bExportAvatar = _rhs.bExportAvatar;
			bSingleObject = _rhs.bSingleObject;
			weldType = _rhs.weldType;
			bThin = _rhs.bThin;
			bUnifiedUVCoordinates = _rhs.bUnifiedUVCoordinates;
			bCreateUnifiedTexture = _rhs.bCreateUnifiedTexture;
			unifiedTextureSize = _rhs.unifiedTextureSize;
			unifiedTextureFillSeamSize = _rhs.unifiedTextureFillSeamSize;
			bUseInifinteSeams = _rhs.bUseInifinteSeams;
			bUnifiedDiffuseMap = _rhs.bUnifiedDiffuseMap;
			bUnifiedNormalMap = _rhs.bUnifiedNormalMap;
			bUnifiedMetalnessMap = _rhs.bUnifiedMetalnessMap;
			bUnifiedRoughnessMap = _rhs.bUnifiedRoughnessMap;
			bUnifiedOpacityMap = _rhs.bUnifiedOpacityMap;
			bUnifiedDisplacementMap = _rhs.bUnifiedDisplacementMap;
			unifiedTextureBakeRelateive = _rhs.unifiedTextureBakeRelateive;
			unifiedTextureBakeMargin = _rhs.unifiedTextureBakeMargin;
			bIncludeHiddenObject = _rhs.bIncludeHiddenObject;
			bIncludeInnerShape = _rhs.bIncludeInnerShape;
			scale = _rhs.scale;
			axisX = _rhs.axisX;
			axisY = _rhs.axisY;
			axisZ = _rhs.axisZ;
			bInvertX = _rhs.bInvertX;
			bInvertY = _rhs.bInvertY;
			bInvertZ = _rhs.bInvertZ;
			bSaveColorWays = _rhs.bSaveColorWays;
			bSaveInZip = _rhs.bSaveInZip;
			bDiffuseColorCombined = _rhs.bDiffuseColorCombined;
			bExcludeAmbient = _rhs.bExcludeAmbient;
			bOpacityMap = _rhs.bOpacityMap;
			bMetaData = _rhs.bMetaData;
			bEmbedded = _rhs.bEmbedded;
			bExportLight = _rhs.bExportLight;
			bExportFabric = _rhs.bExportFabric;
			bExportAnimation = _rhs.bExportAnimation;
			bAscii = _rhs.bAscii;
			fbxSdkVersion = _rhs.fbxSdkVersion;
			textureSaveMethod = _rhs.textureSaveMethod;
			anim_step = _rhs.anim_step;
			bExportShutterSpeed = _rhs.bExportShutterSpeed;
			shutter_low = _rhs.shutter_low;
			shutter_high = _rhs.shutter_high;
			m_bExportStrainColorMap = _rhs.m_bExportStrainColorMap;
			m_bExportStressColorMap = _rhs.m_bExportStressColorMap;
			m_alembicType = _rhs.m_alembicType;
			bIncludeAvatarShape = _rhs.bIncludeAvatarShape;
			bIncludeSkeleton = _rhs.bIncludeSkeleton;
			bIncludeSceneProp = _rhs.bIncludeSceneProp;
			timeScale = _rhs.timeScale;
			fps = _rhs.fps;
			ImportObjectType = _rhs.ImportObjectType;

#pragma region lower_version_compatibility
			
			if (_rhs.m_AuthenticationKeyForAPI.length() == 36 || _rhs.m_AuthenticationKeyForAPI.length() == 0)
			{
				m_AuthenticationKeyForAPI = _rhs.m_AuthenticationKeyForAPI;
			}
			else
			{
				m_AuthenticationKeyForAPI = "";
			}

#pragma endregion Code exists due to lower version compatibility.		

			bSaveColorWaysSingleFile = _rhs.bSaveColorWaysSingleFile;

			bAutoTranslate = _rhs.bAutoTranslate;
			bCreateCamera = _rhs.bCreateCamera;
			bCreateAnimation = _rhs.bCreateAnimation;
			bCreateCacheAnimation = _rhs.bCreateCacheAnimation;
			bMoveGarment = _rhs.bMoveGarment;
			bAddArrangementPoints = _rhs.bAddArrangementPoints;
			bAutoCreateFittingSuit = _rhs.bAutoCreateFittingSuit;
			bAdd = _rhs.bAdd;
			translationValueX = _rhs.translationValueX;
			translationValueY = _rhs.translationValueY;
			translationValueZ = _rhs.translationValueZ;
			bTrace2DPatternsUVMap = _rhs.bTrace2DPatternsUVMap;
			bCreateMetallicRoughnessMap = _rhs.bCreateMetallicRoughnessMap;
		}

		ImportExportOption(const ImportExportOption& _rhs)
		{
			init(_rhs);
		}

		ImportExportOption& operator= (const ImportExportOption& _rhs)
		{
			init(_rhs);

			return *this;
		}
	};

	/// @enum CLO_API_UNIT
	/// @brief Unit types 
	enum CLO_API_UNIT
	{
		CLO_API_MILIMETER,  /// milimeter
		CLO_API_CENTIMETER, /// centimeter
		CLO_API_INCH,		/// inch
		CLO_API_METER,		/// meter
		CLO_API_FEET		/// feet
	};

	/// @class ImportZPRJOption
	/// @brief Options for ImportZPRJ as parameters without dialogs in CLO API
	struct ImportZPRJOption 
	{
		EXPORT_PYTHON bool bAppend;					/// if true, append the project file to the current loaded project
										/// if false, overwrite/replace the project file in the current loaded project

		EXPORT_PYTHON bool bLoadGarment;				/// if true, open/add Garment inside the zprj file into the current loaded project
		EXPORT_PYTHON bool bLoadAvatar;				/// if true, open/add Avatar inside the zprj file into the current loaded project

		EXPORT_PYTHON float translationValueX;		/// set x-offet value per current patterns in 2D Window for Patterns (left - right)
										/// set x-offset value per the position-value inside the zprj file in 3D Window for Avatar/Garment (left - right)
		EXPORT_PYTHON float translationValueY;		/// set y-offet value per current patterns in 2D Window for Patterns (up - down)
										/// set y-offset value per the position-value inside the zprj file in 3D Window for Avatar/Garment
		EXPORT_PYTHON float translationValueZ;		/// set z-offset value per the position-value inside the zprj file in 3D Window for Avatar/Garment (monitor - user)

		EXPORT_PYTHON CLO_API_UNIT translationUnit;	/// translationUnit - 0: Milimeter, 1: Centimeter, 2: Inch, 3: Meter, 4: Feet

		EXPORT_PYTHON bool bLoadSceneAndProps;				/// if true, open/add SceneAndProps inside the zprj file into the current loaded project
		EXPORT_PYTHON bool bLoadRenderProperties;				/// if true, open/add RenderProperties(Vray) inside the zprj file into the current loaded project

		EXPORT_PYTHON bool bLoadCustomView;				/// if true, load Custom View (default: false)

		// initialize
		ImportZPRJOption() : bAppend(false),
			bLoadGarment(true),
			bLoadAvatar(true),
			translationValueX(0.0f),
			translationValueY(0.0f),
			translationValueZ(0.0f),
			translationUnit(CLO_API_MILIMETER),
			bLoadSceneAndProps(true),
			bLoadRenderProperties(true),
			bLoadCustomView(false)
		{
		}
	};


	/// @class ImportDxfOption
	/// @brief Options for ImportDxf as parameters without dialogs in CLO API
	struct ImportDxfOption
	{
		EXPORT_PYTHON bool m_bAppend;
		EXPORT_PYTHON bool m_bAutoScale;
		EXPORT_PYTHON float m_Scale;
		EXPORT_PYTHON float m_Angle;
		EXPORT_PYTHON bool m_bImportPatternAnnotation;
		EXPORT_PYTHON bool m_bAutoTraceAsInternalLines;
		EXPORT_PYTHON bool m_SwapCuttingLineAndSewingLine;
		EXPORT_PYTHON bool m_bIncludeNotchs;
		EXPORT_PYTHON bool m_bAutoDistribute;
		EXPORT_PYTHON bool m_bOptimizeAllCurvePoints;
		EXPORT_PYTHON bool m_bImportAllDrawCurvePoint;
		EXPORT_PYTHON bool m_bRemoveNonGradingPoints;
		EXPORT_PYTHON bool m_bConvertToSeamAllowance;

		// initialize
		ImportDxfOption() : 
			m_bAppend(false),
			m_bAutoScale(true),
			m_Scale(1.0f),
			m_Angle(0.0f),
			m_bImportPatternAnnotation(true),
			m_bAutoTraceAsInternalLines(false),
			m_SwapCuttingLineAndSewingLine(false),
			m_bIncludeNotchs(false),
			m_bAutoDistribute(false),
			m_bOptimizeAllCurvePoints(false),
			m_bImportAllDrawCurvePoint(false),
			m_bRemoveNonGradingPoints(false),
			m_bConvertToSeamAllowance(false) {}
	};

	/// @class ExportDxfOption
	/// @brief Options for ExportDxf as parameters without dialogs in CLO API
	struct ExportDxfOption
	{
		EXPORT_PYTHON bool m_bSwapOutLine;			    /// if true, swap the boundary line with sew line
		EXPORT_PYTHON bool m_bDuplicateNotch;			    /// if true, remove duplicate notches
		EXPORT_PYTHON bool m_bCheckedConvertCtoS;		    /// if true, convert curve points to straight points to export
		EXPORT_PYTHON bool m_bExportWithoutGrading;	    /// if true, export without grading information
		EXPORT_PYTHON bool m_bOptimizeCurvePoints;	    /// if true, optimize curve points with removing the points close to some points
		EXPORT_PYTHON bool m_bExportWithoutBaselines;     /// if true, export only out lines without base lines
		EXPORT_PYTHON bool m_bExportSelectedPatterns;     /// if true, export selected patterns
		EXPORT_PYTHON unsigned int m_ExportDXFFormatType; /// 0: DXF-AAMA, 1: DXF-ASTM, 2: Gerber(DXF-AAMA)
		EXPORT_PYTHON unsigned int m_ExportBBType;		/// 1: Use Bounding box covering all the patterns, 2: use Bounding Box for each pattern
		EXPORT_PYTHON float m_fScale;						/// scale variables 
		EXPORT_PYTHON float m_RotateAngle;				/// Rotation angle for patterns
		EXPORT_PYTHON bool m_bMetric;						/// Measurement unit shown on DXF, true: Metric, false: English

		ExportDxfOption() : m_bSwapOutLine(false),
			m_bCheckedConvertCtoS(false),
			m_bExportWithoutGrading(false),
			m_bOptimizeCurvePoints(false),
			m_bExportWithoutBaselines(false),
			m_bExportSelectedPatterns(false),
			m_ExportDXFFormatType(1),
			m_ExportBBType(1),
			m_fScale(1.0f),
			m_RotateAngle(0.0f),
			m_bMetric(true) {}
	};

	/// @class ExportUSDOption
	/// @brief Options for ExportUSD as parameters without dialogs in CLO API
	struct ExportUSDOption
	{
		EXPORT_PYTHON int m_upAxis;							/// 1: Y Axis as up, 2 : Z Axis as up
		EXPORT_PYTHON double m_metersPerUnit;				/// Set unit value based on 1 m
		EXPORT_PYTHON bool m_bAssignShape;					/// If true export geometry data of avatar and cloth to the USD file
		EXPORT_PYTHON std::string m_shapePrimPath;			/// Set prim path of current geometry data default path is '/Mesh'
		EXPORT_PYTHON bool m_bAssignMaterial;				/// If true export material data of avatar and cloth to the USD file
		EXPORT_PYTHON std::string m_materialPrimPath;		/// Set prim path of current geometry data default path is '/Material'
		EXPORT_PYTHON bool m_bExportSimulationData;			/// If true export simulation data to the USD file	

		ExportUSDOption() : m_upAxis(1),
			m_metersPerUnit(0.01),
			m_bAssignShape(true),
			m_shapePrimPath("/Mesh"),
			m_bAssignMaterial(true),
			m_materialPrimPath("/Material"),
			m_bExportSimulationData(false) {}
	};

	/// @enum ExportTechPackType
	/// @brief Export Techpack types
	enum ExportTechPackType
	{
		CLO_TECH_PACK,		/// techpack used in CLO
		CLO_SET_TECH_PACK,	/// techpack used in CLOSET
		CLO_SET_SREST, 		/// techpack used in SREST (used in CLOSET)
		CLO_API_TECH_PACK,	/// techpack for CLO API (excludes the internal values used in CLO Virtual Fashion)
		CLO_DUMMY			/// n/a
	};

	/// @struct ExportTechpackOption
	/// @brief Options for Export Techpack as parameters without dialogs in CLO API
	struct ExportTechpackOption
	{
		EXPORT_PYTHON bool m_bSaveZpac;										/// if true, save garment as a (project name).zpac file and describe the path on the techpack json (CLO API only)
		EXPORT_PYTHON bool m_bSaveZprj;										/// if true, save project as a (project name).zprj file and describe the path on the techpack json (CLO API only)

		EXPORT_PYTHON bool m_bCaptureSceneThumbnailWithoutAvatar;				/// if true, capture the thumbnail images as image files and describe the paths on the techpack json
		EXPORT_PYTHON bool m_bCaptureSceneThumbnailWithMark;					/// if true, capture the thumbnail images with markers as image files and describe the paths on the techpack json (CLO API and CLOSET)
		EXPORT_PYTHON bool m_bCaptureSceneThumbnailWithAvatar;				/// if true, capture the thumbnail images with avatars for all the colorways and describe the paths on the techpack json

		EXPORT_PYTHON bool m_bCaptureItemThumbnail;							/// if true, capture thumbnail for pattern piecese (for CLO API and Techpack Image)
		EXPORT_PYTHON bool m_bCaptureHighResolutionFrontThumbnail;				/// if true, capture bigger thumbnail for 2D First Mode (CLOSET)
		EXPORT_PYTHON bool m_bCapturePatternLayoutThumbnail;					/// if true, capture thumbnail for pattern layout (for CLO API and Techpack Image)
		EXPORT_PYTHON bool m_bCapturePatternLayoutThumbnailWithFabricMark;	/// if true, capture thumbnail for pattern layout with fabric marks (for CLO API and Techpack Image)
		EXPORT_PYTHON bool m_bCapturePatternLayoutThumbnailWithTrimMark;		/// if true, capture thumbnail for pattern layout with trim marks (for CLO API and Techpack Image)
		EXPORT_PYTHON bool m_bCapturePatternLayoutThumbnailForCloset2DPatternWindow;

		EXPORT_PYTHON bool m_bCaptureTurntableThumbnail;				/// if true, capture the turntable thumbnail images as image files and describe the paths on the techpack json

		EXPORT_PYTHON bool m_bExportTextures;									/// if true, export all the textures as image files and describe the paths on the techpack json (CLO API only)
		EXPORT_PYTHON bool m_bUseAverageColor;								/// if true, calculate average color for the image used in materials and show the values in techpack json
		
		EXPORT_PYTHON bool m_bShowModalProgressBar;						    /// if true, a Modal progress bar will appear to show progress in CLO Software

		EXPORT_PYTHON bool m_bUseCustomImageAsThumbnail;				/// if true, thumbnail image will be replaced with a custom image from the MVFile Header

		EXPORT_PYTHON std::map<unsigned int, bool> m_ShowBaseLineFor3DCapturePerColorway;  /// map(colorway index, show/hide flag) for Base Line while capturing 3D
		EXPORT_PYTHON std::string m_AuthenticationKeyForAPI;						/// For companies to use extended functionalies in CLO API/SDK

		EXPORT_PYTHON ExportTechPackType m_ExportType;					    /// 0: CLO_TECH_PACK, 1: CLO_SET_TECH_PACK, 2: CLO_SET_SREST, 3: CLO_API_TECH_PACK, 4: CLO_DUMMY

		ExportTechpackOption()
		{
			m_bSaveZpac = true;
			m_bSaveZprj = true;
			m_bCaptureSceneThumbnailWithoutAvatar = true;
			m_bCaptureSceneThumbnailWithMark = true;
			m_bCaptureSceneThumbnailWithAvatar = true;
			m_bExportTextures = true;
			m_bCaptureItemThumbnail = true;
			m_bCaptureHighResolutionFrontThumbnail = true;
			m_bCapturePatternLayoutThumbnail = true;
			m_bCapturePatternLayoutThumbnailWithFabricMark = true;
			m_bCapturePatternLayoutThumbnailWithTrimMark = true;
			m_bCaptureTurntableThumbnail = true;
			m_bUseAverageColor = true;
			m_bShowModalProgressBar = true;
			m_bUseCustomImageAsThumbnail = true;
			m_bCapturePatternLayoutThumbnailForCloset2DPatternWindow = true;

			m_ShowBaseLineFor3DCapturePerColorway.clear();

			m_ExportType = CLO_API_TECH_PACK;
		}

		ExportTechpackOption(ExportTechPackType type)
		{
			if (type == CLO_TECH_PACK || type == CLO_API_TECH_PACK)
			{
				m_bSaveZpac = true;
				m_bSaveZprj = true;
				m_bCaptureSceneThumbnailWithoutAvatar = true;
				m_bCaptureSceneThumbnailWithMark = true;
				m_bCaptureSceneThumbnailWithAvatar = true;
				m_bExportTextures = true;
				m_bCaptureItemThumbnail = true;
				m_bCaptureHighResolutionFrontThumbnail = false;
				m_bCapturePatternLayoutThumbnail = true;
				m_bCapturePatternLayoutThumbnailWithFabricMark = true;
				m_bCapturePatternLayoutThumbnailWithTrimMark = true;
				m_bCaptureTurntableThumbnail = true;
				m_bUseAverageColor = true;
				m_bUseCustomImageAsThumbnail = false;
				m_bCapturePatternLayoutThumbnailForCloset2DPatternWindow = false;
			}
			else if (type == CLO_SET_TECH_PACK)
			{
				m_bSaveZpac = false;
				m_bSaveZprj = false;
				m_bCaptureSceneThumbnailWithoutAvatar = true;
				m_bCaptureSceneThumbnailWithMark = true;
				m_bCaptureSceneThumbnailWithAvatar = true;
				m_bExportTextures = false; // fabric의 texture는 내보낼 필요 없다.
				m_bCaptureItemThumbnail = true;
				m_bCaptureHighResolutionFrontThumbnail = true;
				m_bCapturePatternLayoutThumbnail = true;
				m_bCapturePatternLayoutThumbnailWithFabricMark = true;
				m_bCapturePatternLayoutThumbnailWithTrimMark = true;
				m_bCaptureTurntableThumbnail = true;
				m_bUseAverageColor = false;
				m_bUseCustomImageAsThumbnail = false;
				m_bCapturePatternLayoutThumbnailForCloset2DPatternWindow = false;
			}
			else if(type == CLO_SET_SREST)
			{
				m_bSaveZpac = false;
				m_bSaveZprj = false;
				m_bCaptureSceneThumbnailWithoutAvatar = true;
				m_bCaptureSceneThumbnailWithMark = false;
				m_bCaptureSceneThumbnailWithAvatar = true;
				m_bExportTextures = true;
				m_bCaptureItemThumbnail = false;
				m_bCaptureHighResolutionFrontThumbnail = false;
				m_bCapturePatternLayoutThumbnail = false;
				m_bCapturePatternLayoutThumbnailWithFabricMark = false;
				m_bCapturePatternLayoutThumbnailWithTrimMark = false;
				m_bCaptureTurntableThumbnail = false;
				m_bUseAverageColor = false;
				m_bUseCustomImageAsThumbnail = false;
				m_bCapturePatternLayoutThumbnailForCloset2DPatternWindow = true; // Hank : SRest 컨버팅 할때 Backend에서 사용할 Capture Image 만들어내기.
			}
			else
			{
				m_bSaveZpac = false;
				m_bSaveZprj = false;
				m_bCaptureSceneThumbnailWithoutAvatar = false;
				m_bCaptureSceneThumbnailWithMark = false;
				m_bCaptureSceneThumbnailWithAvatar = false;
				m_bExportTextures = false;
				m_bCaptureItemThumbnail = false;
				m_bCaptureHighResolutionFrontThumbnail = false;
				m_bCapturePatternLayoutThumbnail = false;
				m_bCapturePatternLayoutThumbnailWithFabricMark = false;
				m_bCapturePatternLayoutThumbnailWithTrimMark = false;
				m_bCaptureTurntableThumbnail = false;
				m_bUseAverageColor = false;
				m_bUseCustomImageAsThumbnail = false;
				m_bCapturePatternLayoutThumbnailForCloset2DPatternWindow = false;
			}

			m_ShowBaseLineFor3DCapturePerColorway.clear();

			m_ExportType = type;
		}
	};

	/// @struct CloApiRgb
	/// @brief RGB Values structure for Color related api calls in CLO API
	struct CloApiRgb
	{
		EXPORT_PYTHON int R; //Red: 0 ~ 255
		EXPORT_PYTHON int G; //Green: 0 ~ 255
		EXPORT_PYTHON int B; //Blue: 0 ~ 255

		CloApiRgb() :R(0), G(0), B(0) {}
	};

	/// @struct CloApiRgba
	/// @brief RGBA Values structure for Color related api calls in CLO API
	struct CloApiRgba
	{
		EXPORT_PYTHON int R; //Red: 0 ~ 255
		EXPORT_PYTHON int G; //Green: 0 ~ 255
		EXPORT_PYTHON int B; //Blue: 0 ~ 255
		EXPORT_PYTHON int A; //Alpha: 0 ~ 255

		CloApiRgba() :R(0), G(0), B(0), A(255) {}
	};

	/// @enum RenderColorwayType
	/// @brief Colorway Type for ExportRenderingImage in CLO API
	enum RenderColorwayType
	{
		SINGLE,			/// Export Rendering Image for only the current colorway
		ALL,			/// Export Rendering Image for all the colorways
		USER_DEFINED	/// Export Rendering Image per the current setting in the colorway type
	};

	/// @struct AttachFileInfo
	/// @brief AttachFileInfo structure for attaching files when using REST API
	struct AttachFileInfo{
		EXPORT_PYTHON std::string fileName; /// The fileName
		EXPORT_PYTHON std::string filePath; /// The file path
	};

	/// @struct MimeData
	/// @brief MimeData structure for form-data type
	struct MimeData{
		EXPORT_PYTHON std::string mimeType; /// MIME type 
		EXPORT_PYTHON std::string mimeNmae; /// MIME field name
		EXPORT_PYTHON std::string mimeValue; /// MIME value
	};

	/// @enum HttpMethodType
	/// @brief Http method Type for Http request
	enum HttpMethodType 
	{
		HTTP_GET,  /// HTTP GET method
		HTTP_POST, /// HTTP POST method
		HTTP_PUT   /// HTTP PUT method
	};

	/// @struct CloApiRestRequest
	/// @brief CloApiRestRequest structure that requests http when using REST API
	struct CloApiRestRequest
	{
		EXPORT_PYTHON std::vector<std::pair<std::string, std::string>> header;   /// Http header
		EXPORT_PYTHON HttpMethodType method;									 /// Http method Type
		EXPORT_PYTHON std::string payload;										 /// Http payload supports json/raw.
		EXPORT_PYTHON std::string url;											 /// Request url
		EXPORT_PYTHON std::vector<AttachFileInfo> attachements;					 /// List of Http attachments, but with PUT method there can be only one file.
		EXPORT_PYTHON std::vector<MimeData> formData;					 		 /// List of multi form-data
	};

	/// @struct CloApiRestResponse
	/// @brief CloApiRestResponse structure that responses http when using REST API
	struct CloApiRestResponse
	{
		EXPORT_PYTHON std::string header;			/// Http header								
		EXPORT_PYTHON std::string buffer;			/// Http response body
		EXPORT_PYTHON unsigned int responseCode;	/// Http response code
		CloApiRestResponse() :responseCode(0) {}
	};

	struct CloApiGraphicDimensions
	{
		EXPORT_PYTHON float width;
		EXPORT_PYTHON float height;
		CloApiGraphicDimensions() : width(0.0f), height(0.0f) {}
	};

	/// @enum CloWidgetType
	/// @brief CloWidgetType is used to get specific style codes when using UpdateCloStyleForPlugIn(). 
	enum CloWidgetType
	{
		Widget_GeneralType = 0,
		Widget_1DepthFrameType,
		Widget_2DepthFrameType,
		Widget_3DepthFrameType,
		Widget_4DepthFrameType,
		Widget_5DepthFrameType,
		Widget_CheckBoxType,
		Widget_ComboBoxType,
		Widget_DialogType,
		Widget_DoubleSpinBoxType,
		Widget_HeaderViewType,
		Widget_LabelType,
		Widget_LineEditType,
		Widget_ListType,
		Widget_MenuType,
		Widget_MenuBarType,
		Widget_PushButtonType,
		Widget_RadioButtonType,
		Widget_ScrollBarType,
		Widget_ScrollAreaType,
		Widget_SliderType,
		Widget_SpinBoxType,
		Widget_TabType,
		Widget_TableViewType,
		Widget_TableWidgetType,
		Widget_TextBrowserType,
		Widget_TextEditType,
		Widget_TitleBarType,
		Widget_ToolButtonType,
		Widget_TooltipType,
		Widget_TransparentType,
		Widget_TreeType,
	};

	/// @enum CloTheme
	/// @brief CloTheme is the Theme information currently used in CLO
	enum CloTheme
	{
		Theme_Dark,
		Theme_Light,
	};

	/// @class ImportAlembicOption
	/// @brief Options for ImportAlembic as parameters without dialogs in CLO API
	struct ImportAlembicOption 
	{
		EXPORT_PYTHON bool aLoadType;					/// if false means open, true means add

		EXPORT_PYTHON CLO_API_UNIT aUnit;	/// translationUnit - 0: Milimeter, 1: Centimeter, 2: Inch, 3: Meter, 4: Feet
		EXPORT_PYTHON float aScale;				/// 

		EXPORT_PYTHON bool boolAddArrangementPoints;		/// if true add the arrangement points
		EXPORT_PYTHON bool boolAutoCreateFittingSuit;		/// if true create fitting suit
		EXPORT_PYTHON int morphingFrameCount;		/// set the morphing frame count ex: 1 to 99
		EXPORT_PYTHON bool boolMoveGarmentToStartPosition;				/// if true, this will select moveGarmentToStartPosition
		EXPORT_PYTHON bool boolTrace2DPatternsFromUVMap;				/// if true, this will select trace2DPatternsFromUVMap

		EXPORT_PYTHON bool boolAlignBottonToGround;				/// if true, set the allignment to bottom
		EXPORT_PYTHON float translationValueX;		/// set x-offset value per the position-value in 3D Window for Avatar/Garment (left - right) unit (mm)
		EXPORT_PYTHON float translationValueY;		/// set y-offset value per the position-value in 3D Window for Avatar/Garment (up - down) unit (mm)		
		EXPORT_PYTHON float translationValueZ;		/// set z-offset value per the position-value in 3D Window for Avatar/Garment (monitor - user) unit (mm)
	
		// initialize
		ImportAlembicOption() : aLoadType(false),
			aUnit(CLO_API_MILIMETER),
			aScale(100.0f),
			boolAddArrangementPoints(true),
			boolAutoCreateFittingSuit(true),
			morphingFrameCount(1),
			boolMoveGarmentToStartPosition(false),
			boolTrace2DPatternsFromUVMap(false),
			boolAlignBottonToGround(false),
			translationValueX(0.0f),
			translationValueY(0.0f),
			translationValueZ(0.0f)
		{
		}
	};
}
