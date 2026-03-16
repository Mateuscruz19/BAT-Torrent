cmake_minimum_required(VERSION 3.16)
project(BATorrent VERSION 1.0)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(LibtorrentRasterbar REQUIRED)
set(CMAKE_AUTOMOC ON)
qt_add_resources(RESOURCES src/resources.qrc)
add_executable(BATorrent
    src/main.cpp
    src/core/sessionmanager.cpp
    src/core/translator.cpp
    src/gui/mainwindow.cpp
    src/gui/torrentmodel.cpp
    src/gui/torrentfilter.cpp
    src/gui/progressdelegate.cpp
    src/gui/detailspanel.cpp
    src/gui/settingsdialog.cpp
    src/gui/welcomedialog.cpp
    src/gui/speedgraph.cpp
    src/gui/thememanager.cpp
    ${RESOURCES}
)
target_link_libraries(BATorrent PRIVATE Qt6::Widgets LibtorrentRasterbar::torrent-rasterbar)
