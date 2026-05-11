CXX ?= c++
BUILD_DIR := build
REST_TARGET := $(BUILD_DIR)/deepcoin_rest
WS_TARGET := $(BUILD_DIR)/deepcoin_ws

REST_SRC := $(wildcard rest/src/*.cpp)
REST_OBJ := $(patsubst rest/src/%.cpp,$(BUILD_DIR)/rest/%.o,$(REST_SRC))
WS_SRC := $(wildcard ws/src/*.cpp)
WS_OBJ := $(patsubst ws/src/%.cpp,$(BUILD_DIR)/ws/%.o,$(WS_SRC))

CPPFLAGS += -I/opt/homebrew/include
CXXFLAGS += -std=c++17 -Wall -Wextra -Wpedantic -O2
LDFLAGS += -L/opt/homebrew/lib
REST_LDLIBS += -lcurl -lssl -lcrypto
WS_LDLIBS += -lssl -lcrypto

.PHONY: all clean

all: rest ws

rest: $(REST_TARGET)

ws: $(WS_TARGET)

$(REST_TARGET): $(REST_OBJ)
	$(CXX) $(LDFLAGS) $^ $(REST_LDLIBS) -o $@

$(WS_TARGET): $(WS_OBJ)
	$(CXX) $(LDFLAGS) $^ $(WS_LDLIBS) -o $@

$(BUILD_DIR)/rest/%.o: rest/src/%.cpp
	@mkdir -p $(BUILD_DIR)/rest
	$(CXX) $(CPPFLAGS) -Irest/include $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/ws/%.o: ws/src/%.cpp
	@mkdir -p $(BUILD_DIR)/ws
	$(CXX) $(CPPFLAGS) -Iws/include $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)
