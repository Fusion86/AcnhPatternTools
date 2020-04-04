TARGET 		:= horizonlib
SOURCES 	:= lib
INCLUDES 	:= include

TESTS		:= test
BUILD 		:= build
OBJDIR		:= obj
CXXFLAGS	+= -std=c++17 -lmbedcrypto -g

CPPFILES 	:= $(foreach dir,$(SOURCES),$(shell find $(dir) -type f -iname "*.cpp"  ))
CPPFILES 	+= $(foreach dir,$(TESTS),$(wildcard $(dir)/*.cpp))
HPPFILES	:= $(foreach dir,$(INCLUDES),$(wildcard $(dir)/*.hpp))
INCLUDE		:= $(foreach dir,$(INCLUDES),-I$(dir))

OFILES		:= $(addprefix $(OBJDIR)/,$(CPPFILES:.cpp=.o))
WARNINGS	:= -Wall -Wextra -Wno-unused-private-field

ifeq ($(CXX),g++)
# -lstdc++fs is needed because of Ubuntu
CXXFLAGS	+= $(INCLUDE) $(WARNINGS) -MD -lstdc++fs
else ifeq ($(CXX),$(filter $(CXX),clang c++))
CXXFLAGS	+= $(INCLUDE) $(WARNINGS)
else
$(warning Unknown compiler '${CXX}', this might be a problem.)
endif

.PHONY: all test clean

all: $(BUILD)/test

test: $(BUILD)/test
	./$(BUILD)/test

clean:
	rm -rf $(BUILD) $(OBJDIR)

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(BUILD)/test: $(OFILES) $(HPPFILES)
	@mkdir -p $(BUILD)
	$(CXX) $(OFILES) -o $@ $(CXXFLAGS)

-include $(OFILES:.o=.d)
