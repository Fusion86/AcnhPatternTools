.PHONY: all clean

TARGET 		:= horizoncli
BUILD 		:= build
SOURCES 	:= source ../horizonlib/lib
INCLUDES 	:= include ../horizonlib/include
CXXFLAGS	+= -std=c++17 -lmbedcrypto -lstdc++fs -g

CPPFILES 	:= $(foreach dir,$(SOURCES),$(shell find $(dir) -type f -iname *.cpp))
HPPFILES	:= $(foreach dir,$(INCLUDES),$(wildcard $(dir)/*.hpp))
INCLUDE		:=	$(foreach dir,$(INCLUDES),-I$(dir))

all: $(BUILD)/$(TARGET)

$(BUILD)/$(TARGET): $(CPPFILES) $(HPPFILES)
	mkdir -p $(BUILD)
	$(CXX) $(CPPFILES) $(INCLUDE) -o $(BUILD)/$(TARGET) $(CXXFLAGS)

run: all
	./$(BUILD)/$(TARGET)

clean:
	rm -rf build
