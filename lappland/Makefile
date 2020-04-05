TARGET 		:= $(notdir $(CURDIR))
SOURCES 	:= src ../horizonlib/lib
INCLUDES 	:= include ../horizonlib/include

TESTS		:= test
BUILD 		:= build
OBJDIR		:= obj
CXXFLAGS	+= -std=c++17 -g -MD $(shell wx-config --cxxflags)
LIBS		:= -lmbedcrypto $(shell wx-config --libs)

CPPFILES 	:= $(foreach dir,$(SOURCES),$(shell find $(dir) -type f -iname "*.cpp"))
CPPFILES 	+= $(foreach dir,$(TESTS),$(wildcard $(dir)/*.cpp))
HPPFILES	:= $(foreach dir,$(INCLUDES),$(wildcard $(dir)/*.hpp))
INCLUDE		:= $(foreach dir,$(INCLUDES),-I$(dir))

# Shitty workaround
_CPPFILES	:= $(foreach file,$(CPPFILES),$(realpath $(file) --relative-to=$(OBJDIR)))
OFILES		:= $(addprefix $(OBJDIR)/,$(_CPPFILES:.cpp=.o))
WARNINGS	:= -Wall -Wextra -Wno-unused-private-field
CXXFLAGS	+= $(INCLUDE) $(WARNINGS)

ifeq ($(CXX),g++)
# -lstdc++fs is needed because of Ubuntu
LIBS		+= -lstdc++fs
else ifeq ($(CXX),$(filter $(CXX),clang c++))
else
$(warning Unknown compiler '$(CXX)', this might be a problem.)
endif

.PHONY: all clean

all: $(BUILD)/$(TARGET)

clean:
	rm -rf $(BUILD) $(OBJDIR)

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(BUILD)/$(TARGET): $(OFILES) $(HPPFILES)
	@mkdir -p $(BUILD)
	$(CXX) $(OFILES) -o $@ $(LIBS)

-include $(OFILES:.o=.d)
