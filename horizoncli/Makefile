.PHONY: all clean

TARGET 		:= horizoncli
BUILD 		:= build
SOURCES 	:= source ../horizonlib/lib
INCLUDES 	:= include ../horizonlib/include
CXXFLAGS	+= -std=c++17 -lmbedcrypto -g -MD

CPPFILES 	:= $(foreach dir,$(SOURCES),$(shell find $(dir) -type f -iname *.cpp))
HPPFILES	:= $(foreach dir,$(INCLUDES),$(wildcard $(dir)/*.hpp))
INCLUDE		:=	$(foreach dir,$(INCLUDES),-I$(dir))

WARNINGS	:= -Wall -Wextra -Wno-unused-private-field

ifeq ($(CXX),g++)
# -lstdc++fs is needed because of Ubuntu
CXXFLAGS	+= $(INCLUDE) $(WARNINGS) -lstdc++fs
else ifeq ($(CXX),$(filter $(CXX),clang c++))
CXXFLAGS	+= $(INCLUDE) $(WARNINGS)
else
$(warning Unknown compiler '${CXX}', this might be a problem.)
endif

all: $(BUILD)/$(TARGET)

$(BUILD)/$(TARGET): $(CPPFILES) $(HPPFILES)
	mkdir -p $(BUILD)
	$(CXX) $(CPPFILES) $(INCLUDE) -o $(BUILD)/$(TARGET) $(CXXFLAGS)

run: all
	./$(BUILD)/$(TARGET)

clean:
	rm -rf build
