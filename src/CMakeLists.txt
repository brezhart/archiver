set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -v")
add_executable(
        archiver
        archiver.cpp
        Utils.h
        Utils.cpp
        BinaryTrie.h
        BinaryTrie.cpp
        archivate.h
        archivate.cpp
        BinaryReader.h
        BinaryReader.cpp
        BinaryWriter.h
        BinaryWriter.cpp
        dearchivate.h
        dearchivate.cpp
)
