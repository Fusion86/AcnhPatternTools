TARGET 		:= $(notdir $(CURDIR))
BUILD 		:= build
SOURCES 	:= source ../horizonlib/lib
INCLUDES 	:= include ../horizonlib/include
CXXFLAGS	+= -std=c++17 -g -MD
LIBS		:= -lmbedcrypto

CPPFILES 	:= $(foreach dir,$(SOURCES),$(shell find $(dir) -type f -iname *.cpp))
HPPFILES	:= $(foreach dir,$(INCLUDES),$(wildcard $(dir)/*.hpp))
INCLUDE		:=	$(foreach dir,$(INCLUDES),-I$(dir))

WARNINGS	:= -Wall -Wextra -Wno-unused-private-field
CXXFLAGS	+= $(INCLUDE) $(WARNINGS)

ifeq ($(CXX),g++)
# -lstdc++fs is needed because of Ubuntu
LIBS		+= -lstdc++fs
else ifeq ($(CXX),$(filter $(CXX),clang c++))
else
$(warning Unknown compiler '${CXX}', this might be a problem.)
endif

.PHONY: all clean

all: $(BUILD)/$(TARGET)

$(BUILD)/$(TARGET): $(CPPFILES) $(HPPFILES)
	mkdir -p $(BUILD)
	$(CXX) $(CPPFILES) $(INCLUDE) -o $(BUILD)/$(TARGET) $(CXXFLAGS) $(LIBS)

run: all
	./$(BUILD)/$(TARGET)

clean:
	rm -rf build
