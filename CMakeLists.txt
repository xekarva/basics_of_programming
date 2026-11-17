cmake_minimum_required(VERSION 3.16)
project(GraphicCalculator LANGUAGES CXX)

# ---------------------------------------------------------------------------
# Стандарт C++17
# ---------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------------------------------------------------------------------
# Автоматическое скачивание зависимостей через FetchContent
# ---------------------------------------------------------------------------
include(FetchContent)

# GLFW
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)
# Статическая библиотека GLFW (по умолчанию и так статическая)
set(BUILD_SHARED_LIBS   OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.3.8
    GIT_SHALLOW    TRUE
)

# Dear ImGui (официальный репозиторий, стабильный тег v1.89.9)
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.89.9
    GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(glfw imgui)

# ---------------------------------------------------------------------------
# Целевая сборка для ImGui + GLFW + OpenGL3
# ---------------------------------------------------------------------------
add_library(imgui_glfw_gl3 STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui_glfw_gl3 PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

find_package(OpenGL REQUIRED)

target_link_libraries(imgui_glfw_gl3 PUBLIC
    glfw
    OpenGL::GL
)

# ---------------------------------------------------------------------------
# Исполняемый файл калькулятора
# ---------------------------------------------------------------------------
add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE imgui_glfw_gl3)

# ---------------------------------------------------------------------------
# Флаги статической линковки под Windows (MinGW из состава Code::Blocks),
# чтобы готовый .exe запускался автономно, без поиска внешних DLL
# (libgcc, libstdc++, winpthread)
# ---------------------------------------------------------------------------
if (WIN32)
    target_link_options(${PROJECT_NAME} PRIVATE
        -static-libgcc
        -static-libstdc++
        -static
    )
    # Убираем консольное окно у релизной сборки GUI-приложения.
    # Для MinGW (Code::Blocks) линкуем с флагом -mwindows: exe запускается
    # как GUI-приложение, без лишнего окна консоли.
    # Если нужна отладочная печать в консоль — закомментируйте строку ниже.
    if (MSVC)
        set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
    else()
        target_link_options(${PROJECT_NAME} PRIVATE -mwindows)
    endif()
endif()
