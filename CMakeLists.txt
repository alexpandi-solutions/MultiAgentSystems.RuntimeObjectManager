cmake_minimum_required(VERSION 3.15)

project(RuntimeObjectManager VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_REQUIRED True)

include_directories(
	src/headers
	src/resources
	src
	results
)

add_executable(
	RuntimeObjectManager
	src/main.cpp
	src/headers/Course.hpp
	src/headers/ISerialize.hpp
	src/headers/ObjectManager.hpp
	src/headers/Log.hpp
	src/headers/MoBase.hpp
	src/headers/Room.hpp
	src/headers/Student.hpp
	src/resources/Course.cpp
	src/resources/ObjectManager.cpp
	src/resources/Log.cpp
	src/resources/MoBase.cpp
	src/resources/Room.cpp
	src/resources/Student.cpp
)
