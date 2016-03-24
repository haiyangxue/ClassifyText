cmake_minimum_required(VERSION 3.3)
project(TextClassification)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    cppjieba/limonp/ArgvContext.hpp
    cppjieba/limonp/BlockingQueue.hpp
    cppjieba/limonp/BoundedBlockingQueue.hpp
    cppjieba/limonp/BoundedQueue.hpp
    cppjieba/limonp/Closure.hpp
    cppjieba/limonp/Colors.hpp
    cppjieba/limonp/Condition.hpp
    cppjieba/limonp/Config.hpp
    cppjieba/limonp/FileLock.hpp
    cppjieba/limonp/ForcePublic.hpp
    cppjieba/limonp/LocalVector.hpp
    cppjieba/limonp/Logging.hpp
    cppjieba/limonp/Md5.hpp
    cppjieba/limonp/MutexLock.hpp
    cppjieba/limonp/NonCopyable.hpp
    cppjieba/limonp/StdExtension.hpp
    cppjieba/limonp/StringUtil.hpp
    cppjieba/limonp/Thread.hpp
    cppjieba/limonp/ThreadPool.hpp
    cppjieba/common.h
    cppjieba/DictTrie.hpp
    cppjieba/FullSegment.hpp
    cppjieba/HMMModel.hpp
    cppjieba/HMMSegment.hpp
    cppjieba/Jieba.hpp
    cppjieba/KeywordExtractor.hpp
    cppjieba/LevelSegment.hpp
    cppjieba/MixSegment.hpp
    cppjieba/MPSegment.hpp
    cppjieba/PosTagger.hpp
    cppjieba/PreFilter.hpp
    cppjieba/Preprocess.h
    cppjieba/QuerySegment.hpp
    cppjieba/SegmentBase.hpp
    cppjieba/TransCode.hpp
    cppjieba/Trie.hpp
    PreProcess.cpp
    PreProcess.h
    TextClassification.cpp utils/utils.h)

add_executable(TextClassification ${SOURCE_FILES})