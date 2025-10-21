include(cmake/CPM.cmake)

CPMAddPackage(
    NAME magic_enum
    GITHUB_REPOSITORY Neargye/magic_enum
    GIT_TAG v0.9.7
)

CPMAddPackage(
    NAME loguru
    GITHUB_REPOSITORY emilk/loguru
    GIT_TAG master
)

CPMAddPackage(
    NAME cxxopts
    GITHUB_REPOSITORY jarro2783/cxxopts
    GIT_TAG v3.3.1
)

