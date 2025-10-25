# Edit following two lines to set component requirements (see docs)
set(COMPONENT_REQUIRES )
set(COMPONENT_PRIV_REQUIRES )

# Source files for this component
set(COMPONENT_SRCS "main.c")
set(COMPONENT_ADD_INCLUDEDIRS "")

idf_component_register(SRCS "main.c" "audioconfig.c" INCLUDE_DIRS ".")

