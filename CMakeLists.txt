cmake_minimum_required(VERSION 3.16)

project(untitled VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Charts Core Gui Qml Quick)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appuntitled
    main.cpp
)

qt_add_qml_module(appuntitled
    URI untitled
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES signalTest.h signalTest.cpp
        QML_FILES Figure.qml
        SOURCES biquad.h biquad.cpp
        SOURCES butterworth.h butterworth.cpp
        SOURCES fftreal/Array.h fftreal/Array.hpp fftreal/def.h fftreal/DynArray.h fftreal/DynArray.hpp fftreal/FFTReal.h fftreal/FFTReal.hpp fftreal/FFTRealFixLen.h fftreal/FFTRealFixLen.hpp fftreal/FFTRealFixLenParam.h fftreal/FFTRealPassDirect.h fftreal/FFTRealPassDirect.hpp fftreal/FFTRealPassInverse.h fftreal/FFTRealPassInverse.hpp fftreal/FFTRealSelect.h fftreal/FFTRealSelect.hpp fftreal/FFTRealUseTrigo.h fftreal/FFTRealUseTrigo.hpp fftreal/OscSinCos.h fftreal/OscSinCos.hpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appuntitled PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appuntitled
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appuntitled PUBLIC
    Qt::Charts
    Qt::Core
    Qt::Gui
    Qt::Qml
    Qt::Quick
)

include(GNUInstallDirs)
install(TARGETS appuntitled
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
