TARGET = example

CXX = clang++

OVERALL_OPTIONS = -pipe
LANGUAGE_OPTIONS = -std=c++14
WARNING_OPTIONS = -Wall -Wextra -Weffc++ -Woverloaded-virtual -Werror
OPTIMIZATION_OPTIONS = -O3 -fno-omit-frame-pointer -march=native -mtune=native
CODE_GENERATION_OPTIONS = -fPIC -pthread
PREPROCESSOR_OPTIONS = -MMD -MP
DEBUGGING_OPTIONS = -gdwarf-3 -fsanitize=address
CXXFLAGS = $(OVERALL_OPTIONS) $(LANGUAGE_OPTIONS) $(WARNING_OPTIONS) $(OPTIMIZATION_OPTIONS) \
           $(CODE_GENERATION_OPTIONS) $(PREPROCESSOR_OPTIONS) $(DEBUGGING_OPTIONS)

LDFLAGS = -fsanitize=address
LIBS = -lm

SOURCES = $(wildcard src/*.cc)
OBJECTS = $(patsubst src/%.cc, obj/main/%.o, $(SOURCES))
DEPENDS = $(patsubst %.o, %.d, $(OBJECTS))

TEST_SOURCES = $(wildcard test/*.cc)
TEST_OBJECTS = $(patsubst test/%.cc, obj/test/%.o, $(TEST_SOURCES))
TEST_DEPENDS = $(patsubst %.o, %.d, $(TEST_OBJECTS))
TESTS = $(patsubst test/%.cc, obj/test/%.exe, $(TEST_SOURCES))

GTEST_DIR = vendor/gtest
GTEST_OBJ_DIR = obj/gtest
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h $(GTEST_DIR)/include/gtest/internal/*.h
GTEST_SOURCES = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)


#==============================================================================
# Build rules for TARGET
#==============================================================================

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

obj/main/%.o: src/%.cc
	@mkdir -p obj/main
	$(CXX) $(CXXFLAGS) -o $@ -c $<

-include $(DEPENDS)


#==============================================================================
# Build rules for TESTS
#==============================================================================

test: $(TESTS)

obj/test/%.exe: obj/test/%.o $(filter-out obj/main/main.o, $(OBJECTS)) $(GTEST_OBJ_DIR)/gtest_main.a
	$(CXX) $(LDFLAGS) -pthread $^ -o $@ $(LIBS)
	$@

obj/test/%.o: test/%.cc
	@mkdir -p obj/test
	$(CXX) -isystem $(GTEST_DIR)/include -Isrc $(CXXFLAGS) -pthread -o $@ -c $<

-include $(TEST_DEPENDS)


#==============================================================================
# Build rules for Google Test
#==============================================================================

$(GTEST_OBJ_DIR)/gtest-all.o: $(GTEST_SOURCES)
	@mkdir -p $(GTEST_OBJ_DIR)
	$(CXX) -isystem $(GTEST_DIR) -isystem $(GTEST_DIR)/include $(CXXFLAGS) -pthread -o $@ -c $(GTEST_DIR)/src/gtest-all.cc

$(GTEST_OBJ_DIR)/gtest_main.o: $(GTEST_SOURCES)
	@mkdir -p $(GTEST_OBJ_DIR)
	$(CXX) -isystem $(GTEST_DIR) -isystem $(GTEST_DIR)/include $(CXXFLAGS) -pthread -o $@ -c $(GTEST_DIR)/src/gtest_main.cc

$(GTEST_OBJ_DIR)/gtest.a: $(GTEST_OBJ_DIR)/gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

$(GTEST_OBJ_DIR)/gtest_main.a: $(GTEST_OBJ_DIR)/gtest-all.o $(GTEST_OBJ_DIR)/gtest_main.o
	$(AR) $(ARFLAGS) $@ $^


#==============================================================================
# Misc
#==============================================================================

format:
	clang-format -i $(SOURCES) $(TEST_SOURCES)

clean:
	rm -rf obj/ $(TARGET)

.PHONY: format clean test

# do not delete intermediate files
.SECONDARY:
