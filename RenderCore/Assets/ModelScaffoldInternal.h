// Copyright 2015 XLGAMES Inc.
//
// Distributed under the MIT License (See
// accompanying file "LICENSE" or the website
// http://www.opensource.org/licenses/mit-license.php)

#pragma once

#include "../../Math/Vector.h"
#include "../../Math/Matrix.h"
#include "../../Assets/BlockSerializer.h"
#include "../../Utility/Streams/Serialization.h"
#include "../../Core/Types.h"

namespace RenderCore { namespace Assets 
{
    typedef uint64 MaterialGuid;
    typedef unsigned TopologyPlaceholder;
    typedef unsigned NativeFormatPlaceholder;

    #pragma pack(push)
    #pragma pack(1)

///////////////////////////////////////////////////////////////////////////////////////////////////
    //      g e o m e t r y         //

    class ModelCommandStream
    {
    public:
            //  "Geo calls" & "draw calls". Geo calls have 
            //  a vertex buffer and index buffer, and contain
            //  draw calls within them.
        class GeoCall
        {
        public:
            unsigned        _geoId;
            unsigned        _transformMarker;
            MaterialGuid*   _materialGuids;
            size_t          _materialCount;
            unsigned        _levelOfDetail;
        };

        class InputInterface
        {
        public:
            uint64*     _jointNames;
            size_t      _jointCount;
        };

            /////   -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-   /////
        const GeoCall&  GetGeoCall(size_t index) const;
        size_t          GetGeoCallCount() const;

        const GeoCall&  GetSkinCall(size_t index) const;
        size_t          GetSkinCallCount() const;

        auto            GetInputInterface() const -> const InputInterface& { return _inputInterface; }

        ~ModelCommandStream();
    private:
        GeoCall*        _geometryInstances;
        size_t          _geometryInstanceCount;
        GeoCall*        _skinControllerInstances;
        size_t          _skinControllerInstanceCount;
        InputInterface  _inputInterface;

        ModelCommandStream(const ModelCommandStream&) = delete;
        ModelCommandStream& operator=(const ModelCommandStream&) = delete;
    };

    inline auto         ModelCommandStream::GetGeoCall(size_t index) const -> const GeoCall&    { return _geometryInstances[index]; }
    inline size_t       ModelCommandStream::GetGeoCallCount() const                             { return _geometryInstanceCount; }
    inline auto         ModelCommandStream::GetSkinCall(size_t index) const -> const GeoCall&   { return _skinControllerInstances[index]; }
    inline size_t       ModelCommandStream::GetSkinCallCount() const                            { return _skinControllerInstanceCount; }

///////////////////////////////////////////////////////////////////////////////////////////////////

    class DrawCallDesc
    {
    public:
        unsigned    _firstIndex, _indexCount;
        unsigned    _firstVertex;
        unsigned    _subMaterialIndex;
        TopologyPlaceholder    _topology;

        DrawCallDesc(
            unsigned firstIndex, unsigned indexCount, unsigned firstVertex, unsigned subMaterialIndex, 
            TopologyPlaceholder topology) 
        : _firstIndex(firstIndex), _indexCount(indexCount), _firstVertex(firstVertex)
        , _subMaterialIndex(subMaterialIndex), _topology(topology) {}
    };

    class VertexElement
    {
    public:
        char            _semanticName[16];  // limited max size for semantic name (only alternative is to use a hash value)
        unsigned        _semanticIndex;
        NativeFormatPlaceholder    _nativeFormat;
        unsigned        _alignedByteOffset;

        VertexElement();
        VertexElement(const VertexElement&) never_throws;
        VertexElement& operator=(const VertexElement&) never_throws;
    };

    class GeoInputAssembly
    {
    public:
        SerializableVector<VertexElement>   _elements;
        unsigned                            _vertexStride;

        uint64 BuildHash() const;

        GeoInputAssembly();
        GeoInputAssembly(GeoInputAssembly&& moveFrom) never_throws;
        GeoInputAssembly& operator=(GeoInputAssembly&& moveFrom) never_throws;
        ~GeoInputAssembly();

		#if defined(COMPILER_DEFAULT_IMPLICIT_OPERATORS)
			GeoInputAssembly(const GeoInputAssembly&) = default;
			GeoInputAssembly& operator=(const GeoInputAssembly&) = default;
		#endif
    };

    class VertexData
    {
    public:
        GeoInputAssembly    _ia;
        unsigned            _offset, _size;
    };

    class IndexData
    {
    public:
        NativeFormatPlaceholder _format;
        unsigned                _offset, _size;
    };

///////////////////////////////////////////////////////////////////////////////////////////////////

    class RawGeometry
    {
    public:
        VertexData  _vb;
        IndexData   _ib;
        SerializableVector<DrawCallDesc>   _drawCalls;

        RawGeometry();
        RawGeometry(RawGeometry&&) never_throws;
        RawGeometry& operator=(RawGeometry&&) never_throws;
        ~RawGeometry();
    };

    class BoundSkinnedGeometry : public RawGeometry
    {
    public:

            //  The "RawGeometry" base class contains the 
            //  unanimated vertex elements (and draw calls for
            //  rendering the object as a whole)
        VertexData      _animatedVertexElements;
        VertexData      _skeletonBinding;

        Float4x4*       _inverseBindMatrices;
        size_t          _inverseBindMatrixCount;
        Float4x4*       _inverseBindByBindShapeMatrices;
        size_t          _inverseBindByBindShapeMatrixCount;
        uint16*         _jointMatrices;         // (uint16 or uint8 for this array)
        size_t          _jointMatrixCount;
        Float4x4        _bindShapeMatrix;

        DrawCallDesc*   _preskinningDrawCalls;
        size_t          _preskinningDrawCallCount;

        std::pair<Float3, Float3>   _localBoundingBox;

        ~BoundSkinnedGeometry();
    private:
        BoundSkinnedGeometry();
    };

    class SupplementGeo
    {
    public:
        unsigned    _geoId;
        VertexData  _vb;
    };

///////////////////////////////////////////////////////////////////////////////////////////////////

    #pragma pack(pop)

}}

template<>
inline void Serialize(
	Serialization::NascentBlockSerializer& outputSerializer,
	const RenderCore::Assets::GeoInputAssembly& ia)
{
	outputSerializer.SerializeRaw(ia._elements);
	Serialize(outputSerializer, ia._vertexStride);
}

template<>
inline void Serialize(
	Serialization::NascentBlockSerializer& outputSerializer,
	const RenderCore::Assets::IndexData& indexData)
{
	Serialize(outputSerializer, indexData._format);
	Serialize(outputSerializer, indexData._offset);
	Serialize(outputSerializer, indexData._size);
}

template<>
inline void Serialize(
	Serialization::NascentBlockSerializer& outputSerializer,
	const RenderCore::Assets::VertexData& vertexData)
{
	Serialize(outputSerializer, vertexData._ia);
	Serialize(outputSerializer, vertexData._offset);
	Serialize(outputSerializer, vertexData._size);
}

template<>
inline void Serialize(
	Serialization::NascentBlockSerializer& outputSerializer,
	const RenderCore::Assets::DrawCallDesc& drawCall)
{
	outputSerializer.SerializeValue(drawCall._firstIndex);
	outputSerializer.SerializeValue(drawCall._indexCount);
	outputSerializer.SerializeValue(drawCall._firstVertex);
	outputSerializer.SerializeValue(drawCall._subMaterialIndex);
	outputSerializer.SerializeValue(drawCall._topology);
}

