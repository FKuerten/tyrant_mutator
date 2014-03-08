# Configuration section
MODULE_NAME=tyrantMutator
DEPENDENCIES=basics core mutator
LIBS:=-lboost_program_options -lpugixml -lboost_regex
CXXFLAGS+= --std=c++11


# Automatic section
INCLUDEDIRS:=${MODULEDIR}/target/generated
iftrue ${DEPENDENCIES}
	INCLUDEDIRS+=${MODULEDIR}/../${DEPENDENCIES}/src/main/c++
endif
CPPFLAGS+=-I${INCLUDEDIRS}
