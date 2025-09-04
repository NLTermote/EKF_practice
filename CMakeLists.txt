cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


project(EKF_estimator_basic)

add_executable(main_test main/main.cpp src/EKF_predict.cpp src/CSV_parser.cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# External libraries

include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

find_package(Eigen3 3.3 REQUIRED NO_MODULE)

target_link_libraries(main_test 
    Eigen3::Eigen
    nlohmann_json::nlohmann_json
)