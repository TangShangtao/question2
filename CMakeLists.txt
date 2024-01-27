cmake_minimum_required(VERSION 3.15)
project(TradeGateway)
# 编译选项
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "-g -pg ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "-o3 ${CMAKE_CXX_FLAGS}")
# 三方库
find_package(spdlog REQUIRED)
# 路径设置
get_filename_component(RootDir "${CMAKE_CURRENT_LIST_DIR}/Statistic" DIRECTORY)
set(EXECUTABLE_OUTPUT_PATH ${RootDir}/bin)
set(LIBRARY_OUTPUT_PATH ${RootDir}/lib)
# 头文件、库文件搜索路径
# include_directories(${RootDir}/Public)
# include_directories(${RootDir}/ToolKit)
# link_directories(${RootDir}/lib)

aux_source_directory(${RootDir}/Statistic src)
aux_source_directory(${RootDir}/Replayer src)
aux_source_directory(${RootDir}/SimExchange src)
aux_source_directory(${RootDir}/Strategy src)
aux_source_directory(${RootDir}/Test src)

add_executable(backtest ${src})