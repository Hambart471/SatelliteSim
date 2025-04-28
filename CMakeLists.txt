cmake_minimum_required(VERSION 3.16)
project(SatelliteSim LANGUAGES CXX)

# Указываем, что это Qt6-проект
find_package(Qt6 REQUIRED COMPONENTS Widgets Network)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Собираем исполняемый файл из наших исходников
add_executable(${PROJECT_NAME}
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

# Генерируем код из .ui и подключаем Qt-макросы
set_target_properties(${PROJECT_NAME} PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC OFF
)

# Линкуем с нужными модулями Qt
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt6::Widgets
        Qt6::Network
)
