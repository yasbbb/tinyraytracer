cmake_minimum_required(VERSION 3.12)
project(Raycast-Motor)

# Inclure le module FetchContent
include(FetchContent)

# Configuration de SFML
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.0
)

# Télécharger SFML si ce n'est pas déjà présent
FetchContent_GetProperties(SFML)
if(NOT SFML_POPULATED)
    FetchContent_Populate(SFML)
    add_subdirectory(${sfml_SOURCE_DIR} ${sfml_BINARY_DIR})
endif()

# Définir les fichiers source et d'en-tête
set(SOURCE_FILES
    src/2d.cpp
    src/3d.cpp
    src/Raycasting.cpp
)

set(HEADER_FILES
    include/2d.h
    include/3d.h
)

# Indiquer où trouver les fichiers d'en-tête
include_directories(include)

# Ajouter les fichiers source à l'exécutable
add_executable(Raycast-Motor ${SOURCE_FILES} ${HEADER_FILES})

# Liens avec les bibliothèques SFML
target_link_libraries(Raycast-Motor PRIVATE sfml-graphics sfml-window sfml-audio sfml-network sfml-system)