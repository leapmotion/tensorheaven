cmake_minimum_required(VERSION 2.8)

project(tensorheaven_test)

find_path(EIGEN_INCLUDE_DIR "eigen" PATHS $ENV{EIGEN_INCLUDE_DIR} NO_DEFAULT_PATH)
find_path(BOOST_INCLUDE_DIR "boost" PATHS $ENV{BOOST_INCLUDE_DIR} NO_DEFAULT_PATH)

OPTION(ASM_EXAM "Keep ASM intermediaries for inspection of compiler optimizations." OFF)
OPTION(NO_OPTIMIZE "Compile without optimization (useful for debugging; not implemented for MSVC currently)." OFF)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  set(MSVC 1)
  set(CLANG 0)
  set(GCC 0)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(MSVC 0)
  set(CLANG 1)
  set(GCC 0)
else() #GCC
  set(MSVC 0)
  set(CLANG 0)
  set(GCC 1)
endif()

# the root directory of the project (the one containing the tenh/ dir) is the only necessary include dir
include_directories(${tensorheaven_test_SOURCE_DIR}/include ${EIGEN_INCLUDE_DIR} ${BOOST_INCLUDE_DIR})

set(BASE_MSVC_FLAGS /arch:SSE2)
# misnomer: The above MSVC flags are not used in Debug mode
set(PRODUCTION_MSVC_FLAGS /GL /Ox /Oi /Ot /Ob2 )
set(BASE_GCC_FLAGS -std=c++11 -Wall -Werror -Wno-unused-local-typedefs -Wno-attributes ) #-msse2 -Wno-reorder -Wno-strict-aliasing -Wno-missing-braces -Wno-uninitialized -Wno-sequence-point )
set(BASE_CLANG_FLAGS -std=c++11 -stdlib=libc++ -Wall -Werror -Wno-local-type-template-args -Wno-tautological-compare ) #-stdlib=libc++ -Wall -Wno-deprecated-declarations -Wno-address-of-temporary -Wno-dynamic-class-memaccess -Wno-overloaded-virtual -Wno-missing-braces -Wno-unneeded-internal-declaration -Wno-reorder -Wno-local-type-template-args )
set(PRODUCTION_GCC_FLAGS "")
if(MSVC)
  set(COMPILER_DEBUG_FLAGS /Zi /D_DEBUG /MDd /Od /fp:except /D_CRT_SECURE_NO_WARNINGS /D_SCL_SECURE_NO_WARNINGS=1 /RTC1 /MP)
  set(COMPILER_RELEASE_FLAGS /Zi /D NDEBUG /D _CRT_SECURE_NO_WARNINGS /MD /O2 /fp:fast /WX /MP)
  if(NO_OPTIMIZE)
    # TODO: implement NO_OPTIMIZE for MSVC
  endif()
  # USE_PRODUCTION_FLAGS will replace /Od or /O2 with /Ox /Ob2 /Oi /Ot, adds ~20% compile time
  set(COMPILER_NOPRODUCTION_FLAGS /Zi )
  set(BASE_LINKER_FLAGS /NODEFAULTLIB:LIBCMT /ignore:4099 /DEBUG )
  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /OPT:REF,ICF")
  set(NOPRODUCTION_LINKER_FLAGS "")
  set(PRODUCTION_LINKER_FLAGS /LTCG )
  add_definitions(-DNOMINMAX)
else() # clang or gcc
  set(COMPILER_DEBUG_FLAGS -g3 )
  set(COMPILER_DEBUG_NO_OPTIMIZE_FLAGS -O0 -g3 )
  set(COMPILER_RELEASE_FLAGS -O3 -g0 -DNDEBUG )
  set(COMPILER_NOPRODUCTION_FLAGS "")
  set(BASE_LINKER_FLAGS "")
  set(NOPRODUCTION_LINKER_FLAGS "")
  set(PRODUCTION_LINKER_FLAGS "")
  if(APPLE)
    set(COMPILER_DEBUG_FLAGS ${COMPILER_DEBUG_FLAGS} -mmacosx-version-min=10.4 -fvisibility=hidden)
    set(COMPILER_RELEASE_FLAGS ${COMPILER_RELEASE_FLAGS} -mmacosx-version-min=10.4 -fvisibility=hidden)
    set(STRIPPED_EXE_LINKER_FLAGS "-Xlinker -unexported_symbol -Xlinker \"*\" -Xlinker -dead_strip -Xlinker -dead_strip_dylibs")
  else()
    set(STRIPPED_EXE_LINKER_FLAGS " ")
    set(COMPILER_DEBUG_FLAGS ${COMPILER_DEBUG_FLAGS} -fvisibility=hidden)
    set(COMPILER_RELEASE_FLAGS ${COMPILER_RELEASE_FLAGS} -fvisibility=hidden)
  endif()
  if(ASM_EXAM)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -save-temps") # this uses -O0, which avoids inlined code being optimized out
  endif()
  if(NO_OPTIMIZE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3 -O0") # this uses -O0, which avoids inlined code being optimized out
  endif()
endif()

if(MSVC)
  set(COMPILER_FLAGS ${COMPILER_FLAGS} ${BASE_MSVC_FLAGS})
elseif(CLANG)
  set(COMPILER_FLAGS ${COMPILER_FLAGS} ${BASE_CLANG_FLAGS})
else() # Linux
  set(COMPILER_FLAGS ${COMPILER_FLAGS} ${BASE_GCC_FLAGS})
endif()

foreach(arg ${COMPILER_FLAGS})
  set(TEMP_COMPILER_FLAG_LIST "${TEMP_COMPILER_FLAG_LIST} ${arg}")
endforeach(arg ${COMPILER_FLAGS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TEMP_COMPILER_FLAG_LIST}")

foreach(arg ${COMPILER_DEBUG_FLAGS})
  set(TEMP_COMPILER_DEBUG_FLAG_LIST "${TEMP_COMPILER_DEBUG_FLAG_LIST} ${arg}")
endforeach(arg ${COMPILER_DEBUG_FLAGS})

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${TEMP_COMPILER_DEBUG_FLAG_LIST}")

foreach(arg ${COMPILER_RELEASE_FLAGS})
  set(TEMP_COMPILER_RELEASE_FLAG_LIST "${TEMP_COMPILER_RELEASE_FLAG_LIST} ${arg}")
endforeach(arg ${COMPILER_DEBUG_FLAGS})

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${TEMP_COMPILER_RELEASE_FLAG_LIST}")


# temp tests
# add_executable(algebraic_expression_prototype algebraic_expression_prototype.cpp)
# add_executable(asm_exam asm_exam.cpp asm_exam_separate_functions.cpp)
add_executable(c++11_usage_prototype c++11_usage_prototype.cpp)
add_executable(compile_time_generated_lookup_table compile_time_generated_lookup_table.cpp)
add_executable(conceptual_inheritance_prototype conceptual_inheritance_prototype.cpp)
# add_executable(list list.cpp)
add_executable(prettyprint prettyprint.cpp)
add_executable(sandbox sandbox.cpp)
add_executable(typelist typelist.cpp)
add_executable(taylor_polynomial taylor_polynomial.cpp)

set_source_files_properties(c++11_usage_prototype.cpp PROPERTIES COMPILE_FLAGS -std=c++11)
set_source_files_properties(sandbox.cpp PROPERTIES COMPILE_FLAGS -std=c++11)

set(adhoc_test_SRCS
    adhoc_test.cpp
    adhoc_test_00.cpp
    adhoc_test_01.cpp
    adhoc_test_02.cpp
    adhoc_test_03.cpp
    adhoc_test_04.cpp
    adhoc_test_05.cpp
    adhoc_test_06.cpp
    adhoc_test_07.cpp
    adhoc_test_08.cpp
    adhoc_test_09.cpp
    adhoc_test_10.cpp
    adhoc_test_11.cpp
    adhoc_test_12.cpp
    adhoc_test_13.cpp
    adhoc_test_14.cpp)
add_executable(adhoc_test ${adhoc_test_SRCS})

# test-framework-using tests
set(test_SRCS
    lvd/lvd_callstack.cpp
    lvd/lvd_commandlineparser.cpp
    lvd/lvd_spawner.cpp
    lvd/lvd_testsystem.cpp
    standard/randomize.cpp
    standard/test_abstractindex.cpp
    standard/test_array.cpp
    standard/test_basic_operator.cpp
    standard/test_basic_vector0.cpp
    standard/test_basic_vector1.cpp
    standard/test_basic_vector2.cpp
    standard/test_basic_vector3.cpp
    standard/test_basic_vector4.cpp
    standard/test_basic_vector5.cpp
    standard/test_dimindex.cpp
    standard/test_expressiontemplate_reindex.cpp
    standard/test_linearembedding.cpp
    standard/test_list.cpp
    standard/test_split_and_bundle.cpp
    standard/test_tuple.cpp
    standard/test_typelist.cpp
    standard/test_typle.cpp)
add_executable(test standard/test.cpp ${test_SRCS})
include_directories(${tensorheaven_test_SOURCE_DIR}/../include ${tensorheaven_test_SOURCE_DIR}/lvd ${tensorheaven_test_SOURCE_DIR}/standard)

# tenh-utility-using tests
set(tenh_utility_test_SRCS
    lvd/lvd_callstack.cpp
    lvd/lvd_commandlineparser.cpp
    lvd/lvd_spawner.cpp
    lvd/lvd_testsystem.cpp
    standard/randomize.cpp
    tenh_utility_test/test_homogeneouspolynomials.cpp
    tenh_utility_test/test_multivariatepolynomials.cpp)
add_executable(tenh_utility_test tenh_utility_test/test.cpp ${tenh_utility_test_SRCS})
include_directories(${tensorheaven_test_SOURCE_DIR}/../include
                    ${tensorheaven_test_SOURCE_DIR}/lvd
                    ${tensorheaven_test_SOURCE_DIR}/standard
                    ${tensorheaven_test_SOURCE_DIR}/tenh_utility_test)

add_subdirectory(compile_tests)
