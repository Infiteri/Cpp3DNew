DIR := $(subst /,\,${CURDIR})
BUILD_DIR := Bin
OBJ_DIR := Bin-Obj

ASSEMBLY := Engine
EXTENSION := .dll
COMPILER_FLAGS := -g -std=c++2a 
VP := -IEngine/Vendor
INCLUDE_FLAGS := -IEngine\Source $(VP)/GLFW/include $(VP)/glad/include $(VP)/ImGui $(VP)/stb $(VP)/YAML/include $(VP)/ImGuizmo $(VP)/bullet/bullet3-master/src
LINKER_FLAGS := -g -shared -LBin -lglfw3 -lglad -lImGui -lcomdlg32 -lShlwapi -lyaml -lImGuizmo -lBullet
DEFINES := -D_DEBUG -DCE_BUILD_DLL -D_CRT_SECURE_NO_WARNINGS

# Make does not offer a recursive wildcard function, so here's one:
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

SRC_FILES := $(call rwildcard,$(ASSEMBLY)/Source/,*.cpp) # Get all .cpp files
DIRECTORIES := \$(ASSEMBLY)\Source $(subst $(DIR),,$(shell dir $(ASSEMBLY)\Source /S /AD /B | findstr /i Source)) # Get all directories under src.
OBJ_FILES := $(SRC_FILES:%=$(OBJ_DIR)/%.o) # Get all compiled .cpp.o objects for engine

all: scaffold compile link

.PHONY: scaffold
scaffold: # create build directory
	-@setlocal enableextensions enabledelayedexpansion && mkdir $(addprefix $(OBJ_DIR), $(DIRECTORIES)) 2>NUL || cd .
	-@setlocal enableextensions enabledelayedexpansion && mkdir $(BUILD_DIR) 2>NUL || cd .

.PHONY: link
link: scaffold $(OBJ_FILES) # link
	@g++ $(OBJ_FILES) -o $(BUILD_DIR)\$(ASSEMBLY)$(EXTENSION) $(LINKER_FLAGS) 

.PHONY: compile
compile: #compile .cpp files
	@echo Building And Linking $(ASSEMBLY)

.PHONY: clean
clean: # clean build directory
	if exist $(BUILD_DIR)\$(ASSEMBLY)$(EXTENSION) del $(BUILD_DIR)\$(ASSEMBLY)$(EXTENSION)
	rmdir /s /q $(OBJ_DIR)\$(ASSEMBLY)

$(OBJ_DIR)/%.cpp.o: %.cpp # compile .cpp to .cpp.o object
	@echo   $<...
	@g++ $< $(COMPILER_FLAGS) -c -o $@ $(DEFINES) $(INCLUDE_FLAGS) 