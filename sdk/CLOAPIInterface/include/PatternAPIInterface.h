#pragma once

#include <vector>

#include "DefineDllForWin.h"

namespace CLOAPI
{
	class MV_CLO_INTERFACE_API PatternAPIInterface {
	public:

		/// @fn GetPatternSize()
		/// @brief Get the number of patterns
		///	@return return the number of patterns on the pattern editor and avatar window
		virtual int GetPatternSize() { return 0; } // deprecated. please use 'GetPatternCount()' function

		/// @fn GetPatternIndex(const string& patternName)
		/// @brief Get the pattern index which is using the pattern name
		/// @param patternName: the input pattern name to get the pattern index
		/// @return pattern index
		virtual int GetPatternIndex(const std::string& patternName) { return 0; }

		/// @fn GetPatternIndexW(const wstring& patternName)
		/// @brief Get the pattern index which is using the pattern name
		/// @param patternName: the pattern name to get the pattern index
		/// @return pattern index
		virtual int GetPatternIndexW(const std::wstring& patternName) { return 0; }

		/// @fn GetPatternIndexFrom2DView(float _scenePosX, float _scenePosY)
		/// @brief Get the pattern index which is using the 2D scene position
		/// @param scenePosX: the scene position X
		/// @param scenePosY: the scene position Y
		/// @return pattern index
		virtual int GetPatternIndexFrom2DView(float _scenePosX, float _scenePosY) { return 0; }

		/// @fn GetPatternIndexFrom3DView(float _scenePosX, float _scenePosY)
		/// @brief Get the pattern index which is using the 3D scene position
		/// @param scenePosX: the scene position X
		/// @param scenePosY: the scene position Y
		/// @return pattern index
		virtual int GetPatternIndexFrom3DView(float _scenePosX, float _scenePosY) { return 0; }
		
		/// @fn GetPatternInformation(int patternIndex)
		/// @brief Get the pattern information which is using for pattern index
		/// @param patternIndex: the pattern index to get the pattern information
		/// @return json string for pattern information
		virtual std::string GetPatternInformation(int patternIndex) { return ""; }

		/// @fn GetPatternInformationW(int patternIndex)
		/// @brief Get the pattern information which is using for pattern index
		/// @param patternIndex: the pattern index to get the pattern information
		/// @return json string for pattern information
		virtual std::wstring GetPatternInformationW(int patternIndex) { return L""; }

		/// @fn GetPatternCount()
		/// @brief Get the number of patterns
		///	@return return the number of patterns on the pattern editor and avatar window
		virtual int GetPatternCount() { return 0; }

		/// @fn GetPatternInputInformation(int patternIndex)
		/// @brief Get the pattern input information which is using for pattern index
		/// @param patternIndex: the pattern index to get the pattern input information
		/// @return json string for pattern input information
		virtual std::string GetPatternInputInformation(int _patternIndex) { return ""; }

		/// @fn GetPatternInputInformationW(int patternIndex)
		/// @brief Get the pattern input information which is using for pattern index
		/// @param patternIndex: the pattern index to get the pattern input information
		/// @return json string for pattern input information
		virtual std::wstring GetPatternInputInformationW(int _patternIndex) { return L""; }

		/// @fn GetPatternInputInformation()
		/// @brief Get the pattern input information which is using for pattern index
		/// @param patternIndex: the pattern index to get the pattern input information
		/// @return json string for pattern input information
		virtual std::string GetPatternInputInformation() { return ""; }

		/// @fn GetPatternInputInformationW()
		/// @brief Get the pattern input information which is using for pattern index
		/// @param patternIndex: the pattern index to get the pattern input information
		/// @return json string for pattern input information
		virtual std::wstring GetPatternInputInformationW() { return L""; }

		/// @fn GetPatternPieceArea()
		/// @brief Get the pattern piece area which is using for pattern index
		///	@return return pattern piece
		virtual float GetPatternPieceArea(int _patternIndex) { return 0.0f; }

		/// @fn GetLineLength()
		/// @brief Get the pattern line length which is using for pattern index and line index
		///	@return return line length
		virtual float GetLineLength(int _patternIndex, int _lineIndex) { return 0.0f; }

		/// @fn GetLineLengthForInnerShape()
		/// @brief Get the pattern line length which is using for pattern index and children shape index and line index
		///	@return return line length
		virtual float GetLineLength(int _patternIndex, int _childrenIndex, int _lineIndex) { return 0.0f; }

		/// @fn GetParticleDistanceOfPattern(int _patternIndex)
		/// @brief Get particle distance which is using pattern index
		/// @param patternIndex: the pattern index to get the pattern particle distance
		/// @return Output particle distance; If an error occurs, return MIN_PARTICLE_DISTANCE, 0.8f.
		virtual float GetParticleDistanceOfPattern(int _patternIndex)
		{
			// MIN_PARTICLE_DISTANCE : 0.8f.
			return 0.8f;
		}

		/// @fn GetParticleDistanceOfPattern(const string& _patternName)
		/// @brief Get particle distance which is using the pattern name
		/// @param patternName: the pattern name to get the pattern particle distance
		/// @return Output particle distance; If an error occurs, return MIN_PARTICLE_DISTANCE, 0.8f.
		virtual float GetParticleDistanceOfPattern(const std::string& _patternName)
		{
			// MIN_PARTICLE_DISTANCE : 0.8f.
			return 0.8f;
		}

		/// @fn GetParticleDistanceOfPatternW(const wstring& _patternName)
		/// @brief Get particle distance which is using the pattern name
		/// @param patternName: the pattern name to get the pattern particle distance
		/// @return Output particle distance; If an error occurs, return MIN_PARTICLE_DISTANCE, 0.8f.
		virtual float GetParticleDistanceOfPatternW(const std::wstring& _patternName)
		{
			// MIN_PARTICLE_DISTANCE : 0.8f.
			return 0.8f;
		}

		/// @fn GetMeshCountByType(int _patternIndex)
		/// @brief Get mesh face, vertex count by mesh type which is using pattern index
		/// @param patternIndex: the pattern index to get the pattern mesh face, vertex count
		/// @return Output map string mesh face,vertex count, mesh type; If an error occurs, return infoMap
		virtual std::map<std::string, std::string> GetMeshCountByType(int _patternIndex)
		{
			std::map<std::string, std::string> infoMap;
			return infoMap;
		}

		/// @fn GetMeshCountByType(const string& _patternName)
		/// @brief Get mesh face, vertex count by mesh type which is using the pattern name
		/// @param patternName: the pattern name to get the pattern mesh face, vertex count
		/// @return Output map string mesh face,vertex count, mesh type; If an error occurs, return infoMap
		virtual std::map<std::string, std::string> GetMeshCountByType(const std::string& _patternName)
		{
			std::map<std::string, std::string> infoMap;
			return infoMap;
		}

		/// @fn GetMeshCountByTypeW(const wstring& _patternName)
		/// @brief Get mesh face, vertex count by mesh type which is using the pattern name
		/// @param patternName: the pattern name to get the pattern mesh face, vertex count
		/// @return Output map string mesh face,vertex count, mesh type; If an error occurs, return infoMap

		virtual std::map<std::string, std::string> GetMeshCountByTypeW(const std::wstring& _patternName)
		{
			std::map<std::string, std::string> infoMap;
			return infoMap;
		}
		
		/// @fn GetShrinkagePercentage(int _patternIndex)
		/// @brief Get ShrinkagePercentage each width, height which is using pattern index
		/// @param patternIndex: the pattern index to get the pattern shrinkagePercentage each width, height
		/// @return Output map string shrinkagePercentage width, height; If an error occurs, return infoMap

		virtual std::map<std::string, std::string> GetShrinkagePercentage(int _patternIndex)
		{
			std::map<std::string, std::string> infoMap;
			return infoMap;
		}

		/// @fn GetShrinkagePercentage(const string& _patternName)
		/// @brief Get ShrinkagePercentage each width, height which is using the pattern name
		/// @param patternName: the pattern name to get the pattern shrinkagePercentage each width, height
		/// @return Output map string shrinkagePercentage width, height; If an error occurs, return infoMap

		virtual std::map<std::string, std::string> GetShrinkagePercentage(const std::string& _patternName)
		{
			std::map<std::string, std::string> infoMap;
			return infoMap;
		}

		/// @fn GetShrinkagePercentageW(const wstring& _patternName)
		/// @brief Get ShrinkagePercentage each width, height which is using the pattern name
		/// @param patternName: the pattern name to get the pattern shrinkagePercentage each width, height
		/// @return Output map string shrinkagePercentage width, height; If an error occurs, return infoMap

		virtual std::map<std::string, std::string> GetShrinkagePercentageW(const std::wstring& _patternName)
		{
			std::map<std::string, std::string> infoMap;
			return infoMap;
		}
		
		/// @fn GetBoundingBoxOfPattern()
		/// @brief Get patterns BoundingBox Size each width, height
		/// @param none
		/// @return Output vector map string patterns BoundingBox Size width, height, pattern index, pattern count; If an error occurs, return infoMap

		virtual std::vector<std::map<std::string, std::string>> GetBoundingBoxOfPattern()
		{
			std::vector<std::map<std::string, std::string>> vMap;
			return vMap;
		}

		/// @fn GetBoundingBoxOfPattern(int _patternIndex)
		/// @brief Get BoundingBox Size each width, height which is using pattern index
		/// @param patternIndex: the pattern index to get the pattern BoundingBox Size each width, height
		/// @return Output map string BoundingBox Size width, height; If an error occurs, return infoMap

		virtual std::map<std::string, std::string> GetBoundingBoxOfPattern(int _patternIndex)
		{
			std::map<std::string, std::string> infoMap;
			return infoMap;
		}

		/// @fn GetBoundingBoxOfPattern(const string& _patternName)
		/// @brief Get BoundingBox Size each width, height which is using the pattern name
		/// @param patternName: the pattern name to get the pattern BoundingBox Size each width, height
		/// @return Output map string BoundingBox Size width, height, pattern index; If an error occurs, return infoMap

		virtual std::map<std::string, std::string> GetBoundingBoxOfPattern(const std::string& _patternName)
		{
			std::map<std::string, std::string> infoMap;
			return infoMap;
		}

		/// @fn GetBoundingBoxOfPatternW(const wstring& _patternName)
		/// @brief Get BoundingBox Size each width, height which is using the pattern name
		/// @param patternName: the pattern name to get the pattern BoundingBox Size each width, height
		/// @return Output map string BoundingBox Size width, height, pattern index; If an error occurs, return infoMap

		virtual std::map<std::string, std::string> GetBoundingBoxOfPatternW(const std::wstring& _patternName)
		{
			std::map<std::string, std::string> infoMap;
			return infoMap;
		}

		/// @fn SetParticleDistanceOfPattern(int _patternIndex, float _length)
		/// @brief Set particle distance using pattern index, length
		/// @param	patternIndex: the pattern index to set the pattern particle distance,
		///			length: the length value to determine particle distance
	
		virtual void SetParticleDistanceOfPattern(int _patternIndex, float _length) {}

		/// @fn SetParticleDistanceOfPatterns()
		/// @brief Set patterns particle distance using length
		/// @param length: the length value to determine particle distance
	
		virtual void SetParticleDistanceOfPatterns(float _length) {}


		/// @fn SetMeshType(int _patternIndex, const string& _meshType)
		/// @brief Set mesh type to get mesh count information
		/// @param	patternIndex: the pattern index to set the pattern mesh type,
		///			meshType: the mesh type to set the pattern mesh type "Triangle", "Quad" Other types not allowed
	
		virtual void SetMeshType(int _patternIndex, const std::string& _meshType) {}

		/// @fn SetMeshType(int _patternIndex, const wstring& _meshType)
		/// @brief Set mesh type to get mesh count information
		/// @param	patternIndex: the pattern index to set the pattern mesh type,
		///			meshType: the mesh type to set the pattern mesh type "Triangle", "Quad" Other types not allowed
	
		virtual void SetMeshTypeW(int _patternIndex, const std::wstring& _meshType) {}

		/// @fn SetWidthShrinkagePercentage (int _patternIndex, float _width)
		/// @brief Set Width Shrinkage Percentage using pattern index, width
		/// @param	patternIndex: the pattern index to set the pattern width shrinkage percentage,
		///			width: the width value to determine width shrinkage percentage
		
		virtual void SetWidthShrinkagePercentage(int _patternIndex, float _width) {}

		/// @fn SetHeightShrinkagePercentage (int _patternIndex, float _height)
		/// @brief Set Height Shrinkage Percentage using pattern index, height
		/// @param	patternIndex: the pattern index to set the pattern height shrinkage percentage,
		///			height: the height value to determine width shrinkage percentage
	
		virtual void SetHeightShrinkagePercentage(int _patternIndex, float _height) {}

		/// @fn GetArrangementList()
		/// @brief Get all ArrangementList
		/// @param	none
		/// @return Output vector map string Arrangement information of avatar arrangementlist (Arrangement Name,Type, Offset x,y,z, Orientation); If an error occurs, return vector infoMap
	
		virtual std::vector<std::map<std::string, std::string>> GetArrangementList()
		{
			std::vector<std::map<std::string, std::string>> vMap;
			return vMap;
		}

		/// @fn GetArrangementOfPattern()
		/// @brief Get Arrangement of patterns
		/// @param	none
		/// @return Output vector map string Arrangement information of patterns (Arrangement Name,Type, Offset x,y,z, Orientation); If an error occurs, return vector infoMap
	
		virtual std::vector<std::map<std::string, std::string>> GetArrangementOfPattern()
		{
			std::vector<std::map<std::string, std::string>> vMap;
			return vMap;
		}

		/// @fn GetArrangementOfPattern(int _patternIndex)
		/// @brief Get Arrangement of pattern which is using pattern index
		/// @param patternIndex: the pattern index to get the pattern arrangement information
		/// @return Output map string Arrangement information of pattern (Arrangement Name,Type, Offset x,y,z, Orientation); If an error occurs, return infoMap
	
		virtual std::map<std::string, std::string> GetArrangementOfPattern(int _patternIndex)
		{
			std::map<std::string, std::string> infoMap;
			return infoMap;
		}

		/// @fn GetArrangementOfPattern(const string& _patternName)
		/// @brief Get Arrangement of pattern which is using the pattern name
		/// @param patternName: the pattern name to get the pattern arrangement information
		/// @return Output map string Arrangement information of pattern (Arrangement Name,Type, Offset x,y,z, Orientation); If an error occurs, return infoMap
	
		virtual std::map<std::string, std::string> GetArrangementOfPattern(const std::string& _patternName)
		{
			std::map<std::string, std::string> infoMap;
			return infoMap;
		}

		/// @fn GetArrangementOfPatternW(const wstring& _patternName)
		/// @brief Get Arrangement of pattern which is using the pattern name
		/// @param patternName: the pattern name to get the pattern arrangement information
		/// @return Output map string Arrangement information of pattern (Arrangement Name,Type, Offset x,y,z, Orientation); If an error occurs, return infoMap
	
		virtual std::map<std::string, std::string> GetArrangementOfPatternW(const std::wstring& _patternName)
		{
			std::map<std::string, std::string> infoMap;
			return infoMap;
		}

		/// @fn SetArrangementShapeStyle(int _patternIndex, const string& _shapeStyle)
		/// @brief Set Arrangement ShapeStyle to set Arrangement ShapeStyle
		/// @param	patternIndex: the pattern index to set the pattern Arrangement shapeStyle,
		///			shapeStyle: the shapeStyle to set the pattern shapeStyle "Flat", "Curved" Other types not allowed

		virtual void SetArrangementShapeStyle(int _patternIndex, const std::string& _shapeStyle) {}

		/// @fn SetArrangementShapeStyleW(int _patternIndex, const wstring& _shapeStyle)
		/// @brief Set Arrangement ShapeStyle to set Arrangement ShapeStyle
		/// @param	patternIndex: the pattern index to set the pattern arrangement ShapeStyle,
		///			shapeStyle: the shapeStyle to set the pattern shapeStyle "Flat", "Curved" Other types not allowed

		virtual void SetArrangementShapeStyleW(int _patternIndex, const std::wstring& _shapeStyle) {}

		/// @fn SetArrangementPosition(int _patternIndex, int _positionX, int _positionY, int _offset)
		/// @brief Set Arrangement Position using positionX, positionY, offset
		/// @param	patternIndex: the pattern index to set the pattern arrangement position,
		///			positionX, positionY, offset : the value to determine pattern arrangement positionX, positionY, offset

		virtual void SetArrangementPosition(int _patternIndex, int _positionX, int _positionY, int _offset) {}

		/// @fn SetArrangementOrientation(int _patternIndex, int _orientation)
		/// @brief Set Arrangement Orientation using pattern arrangement Orientation
		/// @param	patternIndex: the pattern index to set the pattern arrangement orientation,
		///			Orientation: the value to determine pattern arrangement orientation

		virtual void SetArrangementOrientation(int _patternIndex, int _orientation) {}

		/// @fn SetArrangement(int _patternIndex, int _arrangementIndex)
		/// @brief Set Arrangement using arrangementlist Index
		/// @param	patternIndex: the pattern index to set the arrangement
		///			ArrangementIndex: the value to determine arrangement information

		virtual void SetArrangement(int _patternIndex, int _arrangementIndex) {}

		/// @fn CopyPatternPiecePos(int _patternPieceIndex, float _x, float _y)
		/// @brief Copy Pattern using the pattern index and x,y position
		/// @param	patternIndex: the pattern index for copying pattern
		///			positionX, positionY : the value to determine pattern copy position

		virtual int CopyPatternPiecePos(int _patternPieceIndex, float _x, float _y) { return -1; }

		/// @fn CopyPatternPiecePos(int _patternPieceIndex, float _offsetX, float _offsetY)
		/// @brief Copy Pattern using the pattern index and x,y offset distance
		/// @param	patternIndex: the pattern index for copying pattern
		///			_offsetX, _offsetY : the value to determine pattern copy offset distance,
		///	@return return copy pattern index

		virtual int CopyPatternPieceMove(int _patternPieceIndex, float _offsetX, float _offsetY) { return -1; }

		/// @fn DeletePatternPiece(int _patternPieceIndex)
		/// @brief Delete Pattern using the pattern index
		/// @param	patternIndex: the pattern index for delete pattern
		///	@return return copy pattern index

		virtual void DeletePatternPiece(int _patternIndex) {}

		/// @fn DeletePatternPiece(int _patternPieceIndex)
		/// @brief Delete Pattern using the pattern index
		/// @param	patternIndex: the pattern index for delete pattern
		/// @param	lineIndex: the line index for delete line

		virtual void DeleteLine(int _patternIndex, int _lineIndex) {}

		/// @fn DeletePatternPiece(int _patternPieceIndex)
		/// @brief Delete Pattern using the pattern index
		/// @param	patternIndex: the pattern index for delete pattern
		/// @param	poinIndex: the point index for delete point

		virtual void DeletePoint(int _patternIndex, int _poinIndex) {}

		/// @fn FlipPatternPiece(int _patternIndex, bool _bHorizontally, bool _bEach)
		/// @brief Flip Pattern using the pattern index
		/// @param	patternIndex: the pattern index for flip pattern
		///			_bHorizontally : the value is in the flip direction
		///			_bEach : the value is based on the pattern local matrix

		virtual void FlipPatternPiece(int _patternIndex, bool _bHorizontally, bool _bEach) {}

		/// @fn LayerClonePatternPiecePos(int _patternIndex, float _x, float _y, bool _bUnder)
		/// @brief Layer Clone Pattern using the pattern index and x,y position
		/// @param	patternIndex: the pattern index for layer clone copying pattern
		///			positionX, positionY : the value to determine pattern copy position
		///			_bUnder : the value to 3D phase pattern position

		virtual void LayerClonePatternPiecePos(int _patternIndex, float _x, float _y, bool _bUnder) {}

		/// @fn LayerClonePatternPiecePos(int _patternIndex, float _offsetX, float _offsetY, bool _bUnder)
		/// @brief Layer Clone Pattern using the pattern index and x,y offset distance
		/// @param	patternIndex: the pattern index for layer clone copying pattern
		///			_offsetX, _offsetY : the value to determine pattern copy offset distance,
		///			_bUnder : the value to 3D phase pattern position

		virtual void LayerClonePatternPieceMove(int _patternIndex, float _offsetX, float _offsetY, bool _bUnder) {}

		/// @fn OffsetAsInternalLine(int _patternIndex, int _lineIndex, int _number, float _distance, bool _bReverse, bool _bDirection, bool _bExtend)
		/// @brief OffsetAsInternalLine using the pattern index and offset option
		/// @param	patternIndex: the pattern index for OffsetAsInternalLine pattern
		///			_lineIndex: the line index for Offset line
		///			_number: the value for number of offset internalLines,
		///			_distance: the value to offset distance,
		///			_bReverseDirection: the value to offset reverse direction,	
		///			_bExtend: the value to offset internalLines extend,

		virtual void OffsetAsInternalLine(int _patternIndex, int _lineIndex, int _number, float _distance, bool _bReverseDirection, bool _bExtend) {}
		
		/// @fn UnfoldPatternPiece(int _patternIndex, int _lineIndex, bool _bHalfSymmetry)
		/// @brief Unfold Pattern using the pattern index and line index and halfSymmetry option
		/// @param	patternIndex: the pattern index for delete pattern
		///			lineIndex: the line index for unfold line
		///			halfSymmetry: the value to make halfSymmetry pattern

		virtual void UnfoldPatternPiece(int _patternIndex, int _lineIndex, bool _bHalfSymmetry) {}
		
		/// @fn GetPatternPieceName(int _patternIndex)
		/// @brief Get Pattern name using the pattern index
		/// @param	_patternIndex: the pattern index for get name

		virtual std::string GetPatternPieceName(int _patternIndex) { return ""; }

		/// @fn SetPatternPieceName(int _patternIndex)
		/// @brief Set Pattern name using the pattern index
		/// @param	_patternIndex: the pattern index to set
		///			_patternName: the value to set as pattern name

		virtual void SetPatternPieceName(int _patternIndex, std::string _patternName) {}
		
		/// @fn GetPatternPieceFabricIndex(int _patternIndex)
		/// @brief Get Pattern fabric index using the pattern index
		/// @param	_patternIndex: the pattern index to get

		virtual int GetPatternPieceFabricIndex(int _patternPieceIndex) { return 0; }
		
		/// @fn SetPatternPieceFabricIndex(int _patternPieceIndex, int _fabricIndex)
		/// @brief Set Pattern fabric index using the pattern index
		/// @param	_patternIndex: the pattern index to set
		///			_fabricIndex: the value to set as gabric index

		virtual void SetPatternPieceFabricIndex(int _patternPieceIndex, int _fabricIndex) {}

		/// @fn GetPatternPieceGrainDirection(int _patternPieceIndex)
		/// @brief Get Pattern Grain direction info using the pattern index
		/// @param	_patternIndex: the pattern index to get

		virtual float GetPatternPieceGrainDirection(int _patternPieceIndex) { return 0.0f; }
		
		/// @fn SetPatternPieceGrainDirection(int _patternPieceIndex, float _degree)
		/// @brief Set Pattern grain direction value using the pattern index
		/// @param	_patternIndex: the pattern index to set
		///			_degree: the value to set as grain direction

		virtual void SetPatternPieceGrainDirection(int _patternPieceIndex, float _degree) {}

		/// @fn IsPatternPieceGrainLinkAllColorways(int patternPieceIndex)
		/// @brief Check if Pattern link all color ways value using the pattern index
		/// @param	_patternIndex: the pattern index to set
	
		virtual bool IsPatternPieceGrainLinkAllColorways(int _patternPieceIndex) { return false; }

		/// @fn  SetPatternPieceGrainLinkAllColorways(int _patternPieceIndex, bool _isOn)
		/// @brief Set Pattern link all color ways value using the pattern index
		/// @param	_patternIndex: the pattern index to set
		///			_isOn: the value to set as link all color ways

		virtual void SetPatternPieceGrainLinkAllColorways(int _patternPieceIndex, bool _isOn) {}

		/// @fn GetPatternPieceCategory(int _patternPieceIndex)
		/// @brief Get Pattern category number using the pattern index
		/// @param	_patternPieceIndex: the pattern index to get
		
		virtual int GetPatternPieceCategory(int _patternPieceIndex) { return 0; }
		
		/// @fn SetPatternPieceCategory(int _patternPieceIndex, int _category)
		/// @brief Set Pattern category number using the pattern index
		/// @param	_patternPieceIndex: the pattern index to get
		///			_category: the value to set category value

		virtual void SetPatternPieceCategory(int _patternPieceIndex, int _category) {}
		
		/// @fn GetPatternPieceClassification(int _patternPieceIndex)
		/// @brief Get Pattern classification using the pattern index
		/// @param	_patternPieceIndex: the pattern index to get

		virtual std::string GetPatternPieceClassification(int _patternPieceIndex) { return ""; }
		
		/// @fn SetPatternPieceClassification(int _patternPieceIndex, string _classificatio)
		/// @brief Set Pattern classification using the pattern index
		/// @param	_patternPieceIndex: the pattern index to get
		///			_classification: the value to set classification

		virtual void SetPatternPieceClassification(int _patternPieceIndex, std::string _classification) {}
		
		/// @fn IsPatternPieceSolidify(int _patternPieceIndex)
		/// @brief Check if Pattern Solidify using the pattern index
		/// @param	_patternPieceIndex: the pattern index to get

		virtual bool IsPatternPieceSolidify(int _patternPieceIndex) { return false; }
		
		/// @fn SetPatternPieceSolidify(int _patternPieceIndex, bool _isOn);
		/// @brief Set Pattern Solidify using the pattern index
		/// @param	_patternPieceIndex: the pattern index to get
		///         _isOn: the value to set Solidify On/Off

		virtual void SetPatternPieceSolidify(int _patternPieceIndex, bool _isOn) {}
		
		/// @fn GetPatternPieceSolidifyStrengthen(int _patternPieceIndex)
		/// @brief Get Pattern Solidify using the pattern index
		/// @param	_patternPieceIndex: the pattern index to get
		
		virtual float GetPatternPieceSolidifyStrengthen(int _patternPieceIndex) { return 0; }
		
		/// @fn  SetPatternPieceSolidifyStrengthen(int _patternPieceIndex, float _strengthen);
		/// @brief Get Pattern Solidify using the pattern index
		/// @param	_patternPieceIndex: the pattern index to get
		///			_strengthen : strengthen value to set

		virtual void SetPatternPieceSolidifyStrengthen(int _patternPieceIndex, float _strengthen) {}
		
		/// @fn ConvertToBaseLine(int _patternIndex, int _childrenIndex)
		/// @brief Convert To BaseLine using the pattern index and children index
		/// @param	patternIndex: the pattern index for convert pattern
		///			children Index: the children index for convert children

		virtual void ConvertToBaseLine(int _patternIndex, int _childrenIndex) {}
		
		/// @fn ConvertToInternalLine(int _patternPieceIndex)
		/// @brief Convert To InternalLine using the pattern index and children index
		/// @param	patternIndex: the pattern index for convert pattern
		///			children Index: the children index for convert children

		virtual void ConvertToInternalLine(int _patternIndex, int _childrenIndex) {}

		/// @fn DistribueInternalLinesbetweenSegments(int _patternIndex, vector<int> _indexs, int _number, bool _bStraightLine, bool _bPerpendicularDirection, bool _bGraduateSegmentLengths)
		/// @brief Distribue Internal Lines betweenSegments using the pattern index and line indexs and option
		/// @param	patternIndex: the pattern index for taget pattern
		///			lineIndexs: the line indexs for taget lines
		///			number: the number of internal shapes to be created
		///			straightLine: create an internal figure with straight lines
		///			perpendicularDirection: target line reversal
		///			GraduateSegmentLengths: extend to the outline of the pattern

		virtual void DistribueInternalLinesbetweenSegments(int _patternIndex, std::vector<int> _lineIndexs, int _number, bool _bStraightLine, bool _bPerpendicularDirection, bool _bGraduateSegmentLengths) {}

		/// @fn		GetPatternPiecePos(int _patternIndex)
		/// @brief	Get Pattern piece's world position
		/// @param	_patternIndex: The pattern index to get the world position
		/// @return	The world position of pattern is given in order (x, y). If an error occurs, return empty vector
		virtual std::vector<float> GetPatternPiecePos(int _patternIndex) { return std::vector<float>(); }

		/// @fn		SetPatternPiecePos(int _patternIndex)
		/// @brief	Set Pattern piece's world position to (_x, _y)
		/// @param	_patternIndex: The pattern index to move
		///			_x: The X position of pattern want to move
		///			_y: The Y position of pattern want to move
		virtual void SetPatternPiecePos(int _patternIndex, float _x, float _y) {}

		/// @fn		SetPatternPieceMove(int _patternIndex)
		/// @brief	Set Pattern piece's world position to (CurrentPosition.x + _offsetX, CurrentPosition.y + _offsetY)
		/// @param	_patternIndex: The pattern index to move
		///			_x: The X position offset of pattern want to move
		///			_y: The Y position offset of pattern want to move
		virtual void SetPatternPieceMove(int _patternIndex, float _offsetX, float _offsetY) {}

		/// @fn		SetPatternPieceElastic(int _patternIndex, int _lineIndex, bool _bElastic)
		/// @brief	Set Pattern piece's elastic. When _lineIndex == -1, apply to all line, 0 <= _lineIndex, apply to line
		/// @param	_patternIndex: The pattern index to apply elastic
		///			_lineIndex: The line index to apply elastic. When _lineIndex == -1, apply to all line, 0 <= _lineIndex, apply to line
		///			_bElastic: Whether to apply elastic
		virtual void SetPatternPieceElastic(int _patternIndex, int _lineIndex, bool _bElastic) {}

		/// @fn		SetPatternPieceElasticStrength(int _patternIndex, int _lineIndex, float _strength)
		/// @brief	Set Pattern piece's elastic strength. When _lineIndex == -1, apply to all line, 0 <= _lineIndex, apply to line
		/// @param	_patternIndex: The pattern index to apply elastic strength
		///			_lineIndex: The line index to apply elastic strength. When _lineIndex == -1, apply to all line, 0 <= _lineIndex, apply to line
		///			_strength: The strength of elastic
		virtual void SetPatternPieceElasticStrength(int _patternIndex, int _lineIndex, float _strength) {}

		/// @fn		SetPatternPieceElasticStrengthRatio(int _patternIndex, int _lineIndex, int _ratio)
		/// @brief	Set Pattern piece's elastic strength ratio. When _lineIndex == -1, apply to all line, 0 <= _lineIndex, apply to line
		/// @param	_patternIndex: The pattern index to apply elastic strength ratio
		///			_lineIndex: The line index to apply elastic strength ratio. When _lineIndex == -1, apply to all line, 0 <= _lineIndex, apply to line
		///			_ratio: The strength ratio of elastic
		virtual void SetPatternPieceElasticStrengthRatio(int _patternIndex, int _lineIndex, int _ratio) {}

		/// @fn		SetPatternPieceElasticSegmentLength(int _patternIndex, int _lineIndex, float _segmentLength)
		/// @brief	Set Pattern piece's segment length of elastic. When _lineIndex == -1, apply to all line, 0 <= _lineIndex, apply to line
		/// @param	_patternIndex: The pattern index to apply segment length
		///			_lineIndex: The line index to apply segment length. When _lineIndex == -1, apply to all line, 0 <= _lineIndex, apply to line
		///			_segmentLength: The segment length of elastic 
		virtual void SetPatternPieceElasticSegmentLength(int _patternIndex, int _lineIndex, float _segmentLength) {}
		
		/// @fn		SetPatternPieceElasticTotalLength(int _patternIndex, int _lineIndex, float _totalLength)
		/// @brief	Set Pattern piece's total length of elastic. When _lineIndex == -1, apply to all line, 0 <= _lineIndex, apply to line
		/// @param	_patternIndex: The pattern index to apply total length 
		///			_lineIndex: The line index to apply total length. When _lineIndex == -1, apply to all line, 0 <= _lineIndex, apply to line
		///			_totalLength: The total length of elasitc
		virtual void SetPatternPieceElasticTotalLength(int _patternIndex, int _lineIndex, float _totalLength) {}

		/// @fn		SetPatternPieceShirring(int _patternIndex, int _lineIndex, bool _bShirring)
		/// @brief	Set Pattern piece's shirring. When _lineIndex == -1, apply to all line, 0 <= _lineIndex, apply to line
		/// @param	_patternIndex: The pattern index to apply shirring 
		///			_lineIndex: The line index to apply shirring. When _lineIndex == -1, apply to all line, 0 <= _lineIndex, apply to line
		///			_bShirring: Whether to apply shirring
		virtual void SetPatternPieceShirring(int _patternIndex, int _lineIndex, bool _bShirring) {}

		/// @fn		SetPatternPieceShirringInterval(int _patternIndex, int _lineIndex, float _interval)
		/// @brief	Set Pattern piece's shirring interval. When _lineIndex == -1, apply to all line, 0 <= _lineIndex, apply to line
		/// @param	_patternIndex: The pattern index to apply shirring interval
		///			_lineIndex: The line index to apply shirring interval. When _lineIndex == -1, apply to all line, 0 <= _lineIndex, apply to line
		///			_interval: The interval of shirring
		virtual void SetPatternPieceShirringInterval(int _patternIndex, int _lineIndex, float _interval) {}

		/// @fn		SetPatternPieceShirringHeight(int _patternIndex, int _lineIndex, float _height)
		/// @brief	Set Pattern piece's shirring height. When _lineIndex == -1, apply to all line, 0 <= _lineIndex, apply to line
		/// @param	_patternIndex: The pattern index to apply shirring height
		///			_lineIndex: The line index to apply shirring height. When _lineIndex == -1, apply to all line, 0 <= _lineIndex, apply to line
		///			_height: The height of shirring
		virtual void SetPatternPieceShirringHeight(int _patternIndex, int _lineIndex, float _height) {}

		/// @fn		SetPatternPieceShirringExtend(int _patternIndex, int _lineIndex, bool _bExtend)
		/// @brief	Set Pattern piece's shirring extend. When _lineIndex == -1, apply to all line, 0 <= _lineIndex, apply to line
		/// @param	_patternIndex: The pattern index to apply shirring extend
		///			_lineIndex: The line index to apply shirring extend. When _lineIndex == -1, apply to all line, 0 <= _lineIndex, apply to line
		///			_bExtend: Whether to apply shirring extend
		virtual void SetPatternPieceShirringExtend(int _patternIndex, int _lineIndex, bool _bExtend) {}

		/// @fn		SetPatternPieceSeamtaping(int _patternIndex, int _lineIndex, bool _bSeamtaping)
		/// @brief	Set Pattern piece's seamtaping. When _lineIndex == -1, apply to all line, 0 <= _lineIndex, apply to line
		/// @param	_patternIndex: The pattern index to apply seamtaping
		///			_lineIndex: The line index to apply seamtaping. When _lineIndex == -1, apply to all line, 0 <= _lineIndex, apply to line
		///			_bSeamtaping: Whether to apply seamtaping
		virtual void SetPatternPieceSeamtaping(int _patternIndex, int _lineIndex, bool _bSeamtaping) {}

		/// @fn		SetPatternPieceSametapingWidth(int _patternIndex, int _lineIndex, int _width)
		/// @brief	Set Pattern piece's seamtaping width. When _lineIndex == -1, apply to all line, 0 <= _lineIndex, apply to line
		/// @param	_patternIndex: The pattern index to apply seamtaping width
		///			_lineIndex: The line index to apply seamtaping width. When _lineIndex == -1, apply to all line, 0 <= _lineIndex, apply to line
		///			_width: The width of seamtaping width
		virtual void SetPatternPieceSametapingWidth(int _patternIndex, int _lineIndex, float _width) {}
		
		/// @fn		CreatePatternWithPoints
		/// @brief	Create a pattern using a set of vertices.
		/// @param	_points: set of vertices.
		///			tuple --> postion X, postion Y, VertexType  
		///			VertexType : 0, 2, 3 
		///			0 : Straight Point
		///			2 : Spline Curve Point
		///			3 : Bezier Curve Point
		virtual int CreatePatternWithPoints(std::vector<std::tuple<float, float, int>> _points) { return 0; }

		/// @fn		CreateInternalShapeWithPoints
		/// @brief	Create a pattern's internal shape using a set of vertices.
		/// @param	_patternIndex: Parent Pattern Index
		///			tuple --> postion X, postion Y, VertexType  
		///			VertexType : 0, 2, 3 
		///			0 : Straight Point
		///			2 : Spline Curve Point
		///			3 : Bezier Curve Point
		///			_isClosed: If true, connect the last vertex to the first vertex.
		///	@return	PatternIndex
		virtual int CreateInternalShapeWithPoints(int _patternIndex, std::vector<std::tuple<float, float, int>> _points, bool _isClosed) { return 0;  }

		/// @fn		CreateBaseShapeWithPoints
		/// @brief	Create a pattern's base shape using a set of vertices.
		/// @param	_patternIndex: Parent Pattern Index
		///			_points: set of vertices.
		///			tuple --> postion X, postion Y, VertexType  
		///			VertexType : 0, 2, 3 
		///			0 : Straight Point
		///			2 : Spline Curve Point
		///			3 : Bezier Curve Point
		///			_isClosed: If true, connect the last vertex to the first vertex.
		///	@return	InternalShapeIndex
		virtual int CreateBaseShapeWithPoints(int _patternIndex, std::vector<std::tuple<float, float, int>> _points, bool _isClosed) { return 0; }

		/// @fn SetPatternLayer(int _patternIndex, int _layer)
		/// @brief Set Pattern Layer using pattern index, layer
		/// @param	patternIndex: the pattern index to set the pattern layer,
		///			layer: the layer value to determine pattern layer
		///	@return	BaseShapeIndex
		virtual void SetPatternLayer(int _patternIndex, int _layer) {}

		/// @fn GetPatternLayer(int _patternIndex)
		/// @brief Get Pattern Layer using pattern index
		/// @param	patternIndex: the pattern index to get the pattern layer,
		virtual int GetPatternLayer(int _patternIndex) { return 0; }

		/// @fn SetPatternFreeze(int _patternIndex, int _bFreeze)
		/// @brief Set Pattern Freeze using pattern index, freeze state
		/// @param	patternIndex: the pattern index to set the pattern freeze state,
		///			freeze: the freeze state to determine pattern freeze
		virtual void SetPatternFreeze(int _patternIndex, bool _bFreeze) {}

		/// @fn SetPatternStrengthen(int _patternIndex, bool _bActive)
		/// @brief Set Pattern Strengthen using pattern index, Active state
		/// @param	patternIndex: the pattern index to set the pattern Strengthen,
		///			Strengthen: the Active state to determine pattern Strengthen
		virtual void SetPatternStrengthen(int _patternIndex, bool _bStrengthen) {}

		/// @fn SetPatternLock(int _patternIndex, bool _bLock)
		/// @brief Set Pattern Lock using pattern index, Lock state
		/// @param	patternIndex: the pattern index to set the pattern Lock,
		///			Lock: the Lock state to determine pattern Lock
		virtual void SetPatternLock(int _patternIndex, bool _bLock) {}

		/// @fn SetPatternHide3D(int _patternIndex, bool _bHide)
		/// @brief Set Pattern Hide 3d using pattern index, Hide state
		/// @param	patternIndex: the pattern index to set the pattern Hide 3d,
		///			Hide: the Hide state to determine pattern Lock
		virtual void SetPatternHide3D(int _patternIndex, bool _bHide) {}

		/// @fn AddSegmentTopstitch(int _patternIndex, int _lineIndex)
		/// @brief Add Segment Topstitch to  Pattern using the pattern index, lineindex , topStitchStyleIndex
		/// @param	patternIndex: the pattern index for Add Segment Topstitch to pattern
		/// @param	lineIndex: the line index for topstitch added line
		/// @param	topStitchStyleIndex: the topstitch style index for topstitch

		virtual bool AddSegmentTopstitch(int _patternIndex, int _lineIndex, int _topStitchStyleIndex) { return false; }

		/// @fn AddSeamlineTopstitch(unsigned int _seamlinePairGroupIndex, float _startRatio, float _endRatio, int _topStichStyleIndex)
		/// @brief Add Seamline Topstitch to  Seamline using the seamline pair group index, startRatio, endRatio, topStitchStyleIndex
		/// @param	_seamlinePairGroupIndex: the seamline pair groupindex for Add Seamilne Topstitch to pattern
		/// @param	_startRatio: This is where the topstitch starts. You need to enter what percentage of the total seamline pair group length you want to start at.
		/// @param	_endRatio: This is where the topstitch ends. You need to enter what percentage of the total seamline pair group length you want to end at.
		/// @param  _topStichStyleIndex : the topstitch style index for topstitch
		virtual bool AddSeamlineTopstitch(unsigned int _seamlinePairGroupIndex, float _startRatio, float _endRatio, int _topStichStyleIndex) { return false;}

		/// @fn GetTopstitchStyleList()
		/// @brief Get all GetTopstitchStyleList
		/// @param	none
		/// @return Output vector map string TopstitchStyleList information (Topstitch style Name, index); If an error occurs, return vector infoMap

		virtual std::vector<std::map<std::string, std::string>> GetTopstitchStyleList()
		{
			std::vector<std::map<std::string, std::string>> vMap;
			return vMap;
		}

		/// @fn GetPatternAssignedTopstitchCount()
		/// @brief Get Pattern Assigned Topstitch Count
		/// @param	none
		/// @return Output int Count Pattern Assigned Topstitch Count; If an error occurs, return -1

		virtual int GetPatternAssignedTopstitchCount(int _patternIndex) { return -1; }

		/// @fn GetPatternAssignedTopstitchStyle(int _patternIndex)
		/// @brief Get Pattern Assigned Topstitch style
		/// @param	none
		/// @return Output vector map string Pattern Assigned Topstitch Style (Topstitch style Name, Count); If an error occurs, return vector vMap

		virtual std::vector<std::map<std::string, std::string>> GetPatternAssignedTopstitchStyle(int _patternIndex)
		{
			std::vector<std::map<std::string, std::string>> vMap;
			return vMap;
		}

		/// @fn GetPatternAssignedTopstitch(int _patternIndex)
		/// @brief Get Pattern Assigned Topstitch
		/// @param	patternIndex: the pattern index for Get Topstitch
		/// @return Output vector map string Pattern Assigned Topstitch (Topstitch Name, index); If an error occurs, return vector vMap

		virtual std::vector<std::map<std::string, std::string>> GetPatternAssignedTopstitch(int _patternIndex)
		{
			std::vector<std::map<std::string, std::string>> vMap;
			return vMap;
		}

		/// @fn GetPatternAssignedTopstitchStyleIndex(int _patternIndex, int _segmentStitchIndex)
		/// @brief Get Pattern Assigned Topstitch Style
		/// @param	patternIndex: the pattern index for Get Topstitch Style
		/// @return Output int Pattern Assigned Topstitch Syle index; If an error occurs, return 0

		virtual int GetPatternAssignedTopstitchStyleIndex(int _patternIndex, int _segmentStitchIndex) { return 0; }

		/// @fn SetPatternAssignedTopstitchStyle(int _patternIndex, int _segmentStitchIndex, int _stitchStyleIndex)
		/// @brief Set Pattern Assigned Topstitch Style
		/// @param	patternIndex: the pattern index for Set Topstitch Style

		virtual void SetPatternAssignedTopstitchStyle(int _patternIndex, int _segmentStitchIndex, int _stitchStyleIndex) {}

		/// @fn IsPatternAssignedTopstitchExtendStart(int _patternIndex, int _segmentStitchIndex)
		/// @brief  Get Pattern Assigned Topstitch Property Value ExtendStart
		/// @param	patternIndex: the pattern index for Get Topstitch Style
		/// @param	segmentStitchIndex: the segmentStitch index for Get Topstitch property ExtendStart

		virtual bool IsPatternAssignedTopstitchExtendStart(int _patternIndex, int _segmentStitchIndex) { return false; }

		/// @fn SetPatternAssignedTopstitchExtendStart(int _patternIndex, int _segmentStitchIndex, bool _bStart)
		/// @brief Set Pattern Assigned Topstitch Property Value ExtendStart
		/// @param	patternIndex: the pattern index for Set Topstitch Style
		/// @param	segmentStitchIndex: the segmentStitch index for Set Topstitch property ExtendStart

		virtual void SetPatternAssignedTopstitchExtendStart(int _patternIndex, int _segmentStitchIndex, bool _bStart) {}

		/// @fn IsPatternAssignedTopstitchExtendEnd(int _patternIndex, int _segmentStitchIndex)
		/// @brief  Get Pattern Assigned Topstitch Property Value ExtendEnd
		/// @param	patternIndex: the pattern index for Get Topstitch Style
		/// @param	segmentStitchIndex: the segmentStitch index for Get Topstitch property ExtendEnd

		virtual bool IsPatternAssignedTopstitchExtendEnd(int _patternIndex, int _segmentStitchIndex) { return false; }

		/// @fn SetPatternAssignedTopstitchExtendEnd(int _patternIndex, int _segmentStitchIndex, bool _bEnd)
		/// @brief Set Pattern Assigned Topstitch Property Value ExtendEnd
		/// @param	patternIndex: the pattern index for Set Topstitch Style
		/// @param	segmentStitchIndex: the segmentStitch index for Set Topstitch property ExtendEnd

		virtual	void SetPatternAssignedTopstitchExtendEnd(int _patternIndex, int _segmentStitchIndex, bool _bEnd) {}

		/// @fn IsPatternAssignedTopstitchCurved(int _patternIndex, int _segmentStitchIndex)
		/// @brief  Get Pattern Assigned Topstitch Property Value Curved
		/// @param	patternIndex: the pattern index for Get Topstitch Style
		/// @param	segmentStitchIndex: the segmentStitch index for Get Topstitch property Curved

		virtual	bool IsPatternAssignedTopstitchCurved(int _patternIndex, int _segmentStitchIndex) { return false; }

		/// @fn SetPatternAssignedTopstitchCurved(int _patternIndex, int _segmentStitchIndex, bool _bCurved)
		/// @brief Set Pattern Assigned Topstitch Property Value Curved
		/// @param	patternIndex: the pattern index for Set Topstitch Style
		/// @param	segmentStitchIndex: the segmentStitch index for Set Topstitch property Curved

		virtual	void SetPatternAssignedTopstitchCurved(int _patternIndex, int _segmentStitchIndex, bool _bCurved) {}

		/// @fn GetPatternAssignedTopstitchCurvedLength(int _patternIndex, int _segmentStitchIndex)
		/// @brief  Get Pattern Assigned Topstitch Property Value CurvedLength
		/// @param	patternIndex: the pattern index for Get Topstitch Style
		/// @param	segmentStitchIndex: the segmentStitch index for Get Topstitch property CurvedLength

		virtual	int GetPatternAssignedTopstitchCurvedLength(int _patternIndex, int _segmentStitchIndex) { return 0; }

		/// @fn SetPatternAssignedTopstitchCurvedLength(int _patternIndex, int _segmentStitchIndex, int _length)
		/// @brief Set Pattern Assigned Topstitch Property Value CurvedLength
		/// @param	patternIndex: the pattern index for Set Topstitch Style
		/// @param	segmentStitchIndex: the segmentStitch index for Set Topstitch property CurvedLength

		virtual	void SetPatternAssignedTopstitchCurvedLength(int _patternIndex, int _segmentStitchIndex, int _length) {}

		/// @fn IsPatternAssignedTopstitchCurvedRightAngled(int _patternIndex, int _segmentStitchIndex)
		/// @brief  Get Pattern Assigned Topstitch Property Value CurvedRightAngled
		/// @param	patternIndex: the pattern index for Get Topstitch Style
		/// @param	segmentStitchIndex: the segmentStitch index for Get Topstitch property CurvedRightAngled

		virtual	bool IsPatternAssignedTopstitchCurvedRightAngled(int _patternIndex, int _segmentStitchIndex) { return false; }

		/// @fn SetPatternAssignedTopstitchCurvedRightAngled(int _patternIndex, int _segmentStitchIndex, bool _bRightAngled)
		/// @brief Set Pattern Assigned Topstitch Property Value CurvedRightAngled
		/// @param	patternIndex: the pattern index for Set Topstitch Style
		/// @param	segmentStitchIndex: the segmentStitch index for Set Topstitch property CurvedRightAngled

		virtual	void SetPatternAssignedTopstitchCurvedRightAngled(int _patternIndex, int _segmentStitchIndex, bool _bRightAngled) {}

		/// @fn GetPatternAssignedTopstitchZOffset(int _patternIndex, int _segmentStitchIndex)
		/// @brief  Get Pattern Assigned Topstitch Property Value ZOffset
		/// @param	patternIndex: the pattern index for Get Topstitch Style
		/// @param	segmentStitchIndex: the segmentStitch index for Get Topstitch property ZOffset

		virtual float GetPatternAssignedTopstitchZOffset(int _patternIndex, int _segmentStitchIndex) { return 0.0f; }

		/// @fn SetPatternAssignedTopstitchZOffset(int _patternIndex, int _segmentStitchIndex, float _zOffset)
		/// @brief Set Pattern Assigned Topstitch Property Value ZOffset
		/// @param	patternIndex: the pattern index for Set Topstitch Style
		/// @param	segmentStitchIndex: the segmentStitch index for Set Topstitch property ZOffset

		virtual void SetPatternAssignedTopstitchZOffset(int _patternIndex, int _segmentStitchIndex, float _zOffset) {}

		/// @fn ImportTopStitchStyle(const string& _filePath)
		/// @brief Import TopStitch Style from file path
		/// @param	filePath: the .sst file path for Import TopStitch Style

		virtual bool ImportTopStitchStyle(const std::string& _filePath) { return false; }

		/// @fn GetNestingPatternPieceGrainDirection(int patternPieceIndex)
		/// @brief Returns the index of the graindirection option widget for the pattern.
		/// @param	patternPieceIndex: PatternIndex
		virtual int GetNestingPatternPieceGrainDirection(int patternPieceIndex) { return 0; };

		/// @fn SetNestingPatternPieceGrainDirection(int patternPieceIndex, int menuIndex)
		/// @brief Set the index of the graindirection option widget for the pattern.
		/// @param	patternPieceIndex: PatternIndex
		/// @param	menuIndex: graindirection option widget index, 0 : 1-way, 1 : 2-way , 2 : 4-way
		virtual void SetNestingPatternPieceGrainDirection(int patternPieceIndex, int menuIndex) {};

		/// @fn GetNestingFixedPatternPiecePos(int patternPieceIndex)
		/// @brief When the pattern is fixed in print layout mode, the coordinates of the fixed position are returned. If not, it returns {0, 0}.
		/// @param	patternPieceIndex: PatternIndex
		virtual std::pair<int, int> GetNestingFixedPatternPiecePos(int patternPieceIndex) { return {}; };

		/// @fn SetNestingFixedPatternPiecePos(int patternPieceIndex, int x, int y)
		/// @brief Fix the pattern in print layout mode.
		/// @param	patternPieceIndex: PatternIndex
		/// @param	x: fixed position x
		/// @param	y: fixed position y
		virtual void SetNestingFixedPatternPiecePos(int patternPieceIndex, int x, int y) {};

		/// @fn AddPatternAnnotation(int patternIndex, float posX, float posY, std::string annotation)
		/// @brief Add Annotation in pattern
		/// @param	patternPieceIndex: PatternIndex
		/// @param	posX: annotation position x
		/// @param	posY: annotation position y
		virtual void AddPatternAnnotation(int patternIndex, float posX, float posY, std::string annotation) {};

		/// @fn EditPatternAnnotation(int patternIndex, float posX, float posY, int annotationIndex, std::string annotation)
		/// @brief Edit Annotation
		/// @param	patternPieceIndex: PatternIndex
		/// @param	posX: annotation position x
		/// @param	posY: annotation position y
		/// @param	annotationIndex: annotation Index
		virtual void EditPatternAnnotation(int patternIndex, float posX, float posY, int annotationIndex, std::string annotation) {};

		/// @fn vector<string GetPatternAnnotation(int patternIndex)
		/// @brief Returns all annotations the pattern has.
		/// @param	patternPieceIndex: PatternIndex
		virtual std::vector<std::tuple<std::string, float, float>> GetPatternAnnotation(int patternIndex) { return { { std::make_tuple("", 0.f, 0.f) } }; };

		/// @fn std::vector<int> GetLinkedPatternIndex(int patternIndex)
		/// @brief Get the pattern index information connected to the pattern. 
		/// @param patternIndex: Tartget Pattern Index
		virtual std::vector<int> GetLinkedPatternIndex(int patternIndex) { return {}; };

		/// @fn GetTopstitchStyleModelType(int _topStitchStyleIndex)
		/// @brief Get topstitch style's model type (Type value: OBJ = 0, Texture = 1, Fail = -1)
		/// @param	_topStitchStyleIndex: the topstitch style index for topstitch
		virtual int GetTopstitchStyleModelType(int _topStitchStyleIndex) { return -1; }

		/// @fn SetTopstitchStyleModelType(int _topStitchStyleIndex, int _modelType)
		/// @brief Set topstitch style's model type. If it succeeds, return true
		/// @param	_topStitchStyleIndex: the topstitch style index for topstitch
		/// @param	_modelType:  the topstitch style model type (Type value: OBJ = 0, Texture = 1)
		virtual bool SetTopstitchStyleModelType(int _topStitchStyleIndex, int _modelType) { return false; }

		/// @fn ExportObjectBrowserMaterialUsedList( )
		/// @brief Get the material name used in the garment 
		/// @return json string for pattern input information
		virtual std::string ExportObjectBrowserMaterialsList() { return ""; }
		
		/// @fn GetPinList()
		/// @brief Get all pins from patterns as list
		/// @return list of pins with their uuid
		virtual std::vector<std::string> GetPinList() { return {}; };

		/// @fn RemovePinByIndex(int _index)
		/// @brief Remove the index-wise selected pin 
		/// @param index of pin
		virtual bool RemovePinByIndex(int _index) { return false; };

		/// @fn GetVaildGradingSizeGroupNameList()
		/// @brief Gets the names of the grading size groups that are valid for the patterns currently loaded in the scene.
		/// @return names of the grading size groups
		virtual std::vector<std::wstring> GetValidGradingSizeGroupInformation(){return {};};

		/// @fn GetVaildGradingSizeGroupCount()
		/// @brief Get the size of the grading size grouplist that is valid for the patterns currently loaded in the scene.
		/// @return size of the grading size groups
		virtual unsigned int GetValidGradingSizeGroupCount(){return  0;};

		/// @fn GetGradingSizeGroupNameList()
		/// @brief Get the names of all existing grading size groups.
		/// @return names of the grading size groups
		virtual std::vector<std::wstring> GetGradingSizeGroupInformation(){return  {};};

		/// @fn GetGradingSizeGroupCount()
		/// @brief Get the size of all existing grading size groups.
		/// @return size of the grading size groups
		virtual unsigned int GetGradingSizeGroupCount(){return  0;};

		/// @fn GetGradingSizeTotalCount()
		/// @brief Get the number of sizes/gradings
		/// @return total count of sizes/gradings
		virtual unsigned int GetGradingSizeTotalCount() { return 0; }

		/// @fn GetCurrentGradingSizeIndex()
		/// @brief Get the index of the current size/grading
		/// @return the current index of size/grading
		virtual unsigned int GetCurrentGradingSizeIndex() { return 0; }

		/// @fn GetGradingSizeListFromRuleTable()
		/// @brief Get the list of name about the grading size from grading size group;
		/// @return the list of name of all sizes/gradings
		virtual std::vector<std::wstring> GetGradingSizeListFromRuleTable(const std::wstring& _gradingSizeGroupName){return  {};}

		virtual std::vector<std::wstring> GetGradingSizeListFromRuleTable(unsigned int _gradingSizeGroupIndex){return  {};}

		/// @fn GetGradingSizeNameList()
		/// @brief Get the name of the current size/grading
		/// @return the list of name of all sizes/gradings
		virtual std::vector<std::string> GetGradingSizeNameList()
		{
			std::vector < std::string > interface_vector;
			return interface_vector;
		}

		/// @fn GetSizeNameListW()
		/// @brief Get the index of the current size/grading
		/// @return the list of name of all sizes/gradings
		virtual std::vector<std::wstring> GetGradingSizeNameListW()
		{
			std::vector < std::wstring > interface_vector;
			return interface_vector;
		}

		/// @fn GetGradingSizeInforamtionFromSizeGroup(const std::wstring& _sizeGroupName)
		/// @brief Change Grading Size information
		/// @return Successful grading size change
		/// @param _gradingSizeGroupName: sizegroup name
		/// @param _gradingSizeName: grading size name 
		virtual bool ChangeGradingSizeInformation(const std::wstring& _gradingSizeGroupName, const std::wstring& _gradingSizeName){return  false;}

		/// @fn GetGradingSizeInforamtionFromSizeGroup(const std::wstring& _sizeGroupName)
		/// @brief Change Grading Size information
		/// @return Successful grading size change
		/// @param _gradingSizeGroupIndex: sizegroup index
		/// @param _gradingSizeIndex: grading size index
		virtual bool ChangeGradingSizeInformation(unsigned int _gradingSizeGroupIndex, unsigned int _gradingSizeIndex){return  false;}

		/// @fn GetGradingPairedAvatar(unsigned int _gradingIndex)
		/// @brief Get grading paired avatar's name
		/// @return Paired avatar's name
		/// @param grading index
		virtual std::string GetGradingPairedAvatar(unsigned int _gradingIndex) { return ""; }

		/// @fn GetSeamlinePairGroupListInPattern(unsigned int _patternIndex)
		/// @brief Gets a list of the seamlinePairGroup associated with the pattern.
		/// @return Index List of seamlinePairGroup
		/// @param _patternIndex: pattern index
		virtual std::vector<unsigned int> GetSeamlinePairGroupListInPattern(unsigned int _patternIndex) { return {}; }

		/// @fn GetSeamlinePairGroupCount()
		/// @brief Gets the number of seamlinePairGroup that exist in the scene.
		/// @return Number of seamlinePairGroup
		virtual int GetSeamlinePairGroupCount() { return 0; }

		/// @fn GetSeamlinePairGroupName(int _seamlinePiarGroupIndex)
		/// @brief Get name from seamline pair group index
		/// @return name of seamline Pair Group
		/// @param _seamlinePiarGroupIndex : seamlinePairGroup index
		virtual std::string GetSeamlinePairGroupName(int _seamlinePiarGroupIndex) { return ""; }

		/// @fn GetSeamlinePairGroupNameW(int _seamlinePiarGroupIndex)
		/// @brief Get name from seamline pair group index
		/// @return name of seamline Pair Group
		/// @param _seamlinePiarGroupIndex : seamline Pair Group index
		virtual std::wstring GetSeamlinePairGroupNameW(int _seamlinePiarGroupIndex) { return L""; }

		/// @fn GetSeamlinePairGroupIndexFromName(std::string _seamlinePairGroupName)
		/// @brief Get index from seamline pair group name
		/// @return name of sewing group(wide string)
		/// @param _seamlinePairGroupName : seamlinePairGroup name
		virtual int GetSeamlinePairGroupIndexFromName(std::string _seamlinePairGroupName) { return 0; }
		
		/// @fn GetSeamlinePairGroupIndexFromName(std::string _seamlinePairGroupName)
		/// @brief Get index from seamline pair group name
		/// @return name of sewing group(wide string)
		/// @param _seamlinePairGroupNameW : seamlinePairGroup name
		virtual int GetSeamlinePairGroupIndexFromNameW(std::wstring _seamlinePairGroupNameW) { return 0; }

		/// @fn ExportPatternJSON(std::string _path)
		/// @brief Export pattern information by JSON format
		/// @return boolean result of exporting json file
		virtual bool ExportPatternJSON(std::string _path) { return false; }

		/// @fn ImportPatternJSON(std::string _path)
		/// @brief Import pattern information of JSON file
		/// @return boolean result of importing json file
		virtual bool ImportPatternJSON(std::string _path) { return false; }

		/// @fn GetAllStitchProperty()
		/// @brief Gets property information for all the stitches that exist in the scene. It will be returned as a JSON string.
		/// @return stitch property list, It will be returned as a JSON string.
		virtual std::string GetAllStitchProperty() { return ""; }

		/// @fn AddSeamlinePairGroup(int _patternAIndex, int _lineAIndex, int _patternBIndex, int _lineBIndex, bool _bReverse)
		/// @brief Add Pattern and Sewing between Pattern
		/// @return name of sewing group(wide string)
		/// @param _patternAIndex : A pattern index
		/// @param _lineAIndex : The index of the line that belongs to the A pattern.
		/// @param _patternBIndex : B pattern index
		/// @param _lineBIndex : The index of the line that belongs to the B pattern.
		/// @param _directionA : The direction of the stitching, true : forward, false : backward
		/// @param _directionB : The direction of the stitching, true : forward, false : backward
		virtual bool AddSeamlinePairGroup(int _patternAIndex, int _lineAIndex, int _patternBIndex, int _lineBIndex, bool _directionA, bool _directionB) { return false; }

		/// @fn AddSeamlinePairGroup(int _patternAIndex, int _lineAIndex, int _patternBIndex, int _lineBIndex, bool _directionA, bool _directionB)
		/// @brief Add Patterns and Sewing between InnerShape
		/// @return name of sewing group(wide string)
		/// @param _patternAIndex : A pattern index
		/// @param _lineAIndex : The index of the line that belongs to the A pattern.
		/// @param _patternBIndex : B pattern index
		/// @param _childIndex : The index of the innershape that belongs to the B pattern.
		/// @param _lineBIndex : The index of the line that belongs to the innerShape.
		/// @param _directionA : The direction of the stitching, true : forward, false : backward
		/// @param _directionB : The direction of the stitching, true : forward, false : backward
		virtual bool AddSeamlinePairGroup(int _patternAIndex, int _lineAIndex, int _patternBIndex, int _childIndex, int _lineBIndex, bool _directionA, bool _directionB) { return false; }

		/// @fn AddSeamlinePairGroup(int _patternAIndex, int _lineAIndex, int _patternBIndex, int _lineBIndex, bool _directionA, bool _directionB)
		/// @brief Add InnerShape and Sewing between InnerShape
		/// @return name of sewing group(wide string)
		/// @param _patternAIndex : A pattern index
		/// @param _childAIndex : The index of the innershape that belongs to the A pattern.
		/// @param _lineAIndex : The index of the line that belongs to the innerShape.
		/// @param _patternBIndex : B pattern index
		/// @param _childBIndex : The index of the innershape that belongs to the B pattern.
		/// @param _lineBIndex : The index of the line that belongs to the innerShape.
		/// @param _directionA : The direction of the stitching, true : forward, false : backward
		/// @param _directionB : The direction of the stitching, true : forward, false : backward
		virtual bool AddSeamlinePairGroup(int _patternAIndex, int _childAIndex, int _lineAIndex, int _patternBIndex, int _childBIndex, int _lineBIndex, bool _directionA, bool _directionB) { return false; }
	};
}
