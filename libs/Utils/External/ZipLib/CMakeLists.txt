add_subdirectory(ZipLib/extlibs)
# source files
set(sources_compression
    ZipLib/compression/bzip2/bzip2_decoder_properties.h
    ZipLib/compression/bzip2/bzip2_decoder.h
    ZipLib/compression/bzip2/bzip2_encoder_properties.h
    ZipLib/compression/bzip2/bzip2_encoder.h
    ZipLib/compression/deflate/deflate_decoder_properties.h
    ZipLib/compression/deflate/deflate_decoder.h
    ZipLib/compression/deflate/deflate_encoder_properties.h
    ZipLib/compression/deflate/deflate_encoder.h
    ZipLib/compression/lzma/detail/lzma_alloc.h
    ZipLib/compression/lzma/detail/lzma_handle.h
    ZipLib/compression/lzma/detail/lzma_header.h
    ZipLib/compression/lzma/detail/lzma_in_stream.h
    ZipLib/compression/lzma/detail/lzma_out_stream.h
    ZipLib/compression/lzma/lzma_decoder_properties.h
    ZipLib/compression/lzma/lzma_decoder.h
    ZipLib/compression/lzma/lzma_encoder_properties.h
    ZipLib/compression/lzma/lzma_encoder.h
    ZipLib/compression/store/store_decoder_properties.h
    ZipLib/compression/store/store_decoder.h
    ZipLib/compression/store/store_encoder_properties.h
    ZipLib/compression/store/store_encoder.h
    ZipLib/compression/compression_interface.h    
)
set(sources_detail
    ZipLib/detail/EndOfCentralDirectoryBlock.cpp
    ZipLib/detail/EndOfCentralDirectoryBlock.h
    ZipLib/detail/ZipCentralDirectoryFileHeader.cpp
    ZipLib/detail/ZipCentralDirectoryFileHeader.h
    ZipLib/detail/ZipGenericExtraField.cpp
    ZipLib/detail/ZipGenericExtraField.h
    ZipLib/detail/ZipLocalFileHeader.cpp
    ZipLib/detail/ZipLocalFileHeader.h
)
set(sources_methods
    ZipLib/methods/Bzip2Method.h
    ZipLib/methods/DeflateMethod.h
    ZipLib/methods/ICompressionMethod.h
    ZipLib/methods/LzmaMethod.h
    ZipLib/methods/StoreMethod.h
    ZipLib/methods/ZipMethodResolver.h
)
set(sources_streams
    ZipLib/streams/streambuffs/compression_decoder_streambuf.h
    ZipLib/streams/streambuffs/compression_encoder_streambuf.h
    ZipLib/streams/streambuffs/crc32_streambuf.h
    ZipLib/streams/streambuffs/mem_streambuf.h
    ZipLib/streams/streambuffs/null_streambuf.h
    ZipLib/streams/streambuffs/sub_streambuf.h
    ZipLib/streams/streambuffs/tee_streambuff.h
    ZipLib/streams/streambuffs/zip_crypto_streambuf.h
    ZipLib/streams/compression_decoder_stream.h
    ZipLib/streams/compression_encoder_stream.h
    ZipLib/streams/crc32stream.h
    ZipLib/streams/memstream.h
    ZipLib/streams/nullstream.h
    ZipLib/streams/serialization.h
    ZipLib/streams/substream.h
    ZipLib/streams/teestream.h
    ZipLib/streams/zip_cryptostream.h
)
set(sources_utils
    ZipLib/utils/enum_utils.h
    ZipLib/utils/stream_utils.h
    ZipLib/utils/time_utils.h
)
set(sources
    ZipLib/ZipArchive.cpp
    ZipLib/ZipArchive.h
    ZipLib/ZipArchiveEntry.cpp
    ZipLib/ZipArchiveEntry.h
    ZipLib/ZipFile.cpp
    ZipLib/ZipFile.h
)
source_group("Sources/Compression" FILES ${sources_compression})
source_group("Sources/Detail" FILES ${sources_detail})
source_group("Sources/Methods" FILES ${sources_methods})
source_group("Sources/Streams" FILES ${sources_streams})
source_group("Sources/Utils" FILES ${sources_utils})
source_group("Sources" FILES ${sources})
# library
add_library(ZipLib STATIC ${sources_compression} ${sources_detail} ${sources_methods} ${sources_streams} ${sources_utils} ${sources}
)
# properties
target_include_directories(ZipLib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(ZipLib
    PRIVATE
        bzip2
        lzma
        zlib
)
