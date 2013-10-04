cmake_minimum_required(VERSION 2.8)

project(tensorheaven_test)

find_path(EIGEN_INCLUDE_DIR "eigen" PATHS $ENV{EIGEN_INCLUDE_DIR} NO_DEFAULT_PATH)
find_path(BOOST_INCLUDE_DIR "boost" PATHS $ENV{BOOST_INCLUDE_DIR} NO_DEFAULT_PATH)

OPTION(ASM_EXAM "Keep ASM intermedearies for inspection of compiler optimizations." OFF)

# the root directory of the project (the one containing the tenh/ dir) is the only necessary include dir
include_directories(${tensorheaven_test_SOURCE_DIR} ${EIGEN_INCLUDE_DIR} ${BOOST_INCLUDE_DIR})

set(BASE_MSVC_FLAGS /arch:SSE2)
# misnomer: The above MSVC flags are not used in Debug mode
set(PRODUCTION_MSVC_FLAGS /GL /Ox /Oi /Ot /Ob2 )
set(BASE_GCC_FLAGS -std=c++0x -fpermissive -msse2 -Wall -Wno-reorder -Wno-strict-aliasing -Wno-missing-braces -Wno-uninitialized -Wno-sequence-point )
if(BUILD_64_BIT)
  set(BASE_GCC_FLAGS ${BASE_GCC_FLAGS} -fPIC )
else()
  set(BASE_GCC_FLAGS ${BASE_GCC_FLAGS} -m32 )
endif()
set(BASE_CLANG_FLAGS -std=c++11 -Wall -Wno-deprecated-declarations -Wno-address-of-temporary -Wno-dynamic-class-memaccess -Wno-overloaded-virtual -Wno-missing-braces -Wno-unneeded-internal-declaration -Wno-reorder )
set(PRODUCTION_GCC_FLAGS "")
if(BUILD_WINDOWS)
  set(COMPILER_DEBUG_FLAGS /Zi /D_DEBUG /MDd /Od /fp:except /D_CRT_SECURE_NO_WARNINGS /D_SCL_SECURE_NO_WARNINGS=1 /RTC1 /MP)
  set(COMPILER_RELEASE_FLAGS /Zi /D NDEBUG /D _CRT_SECURE_NO_WARNINGS /MD /O2 /fp:fast /WX /MP)
  # USE_PRODUCTION_FLAGS will replace /Od or /O2 with /Ox /Ob2 /Oi /Ot, adds ~20% compile time
  set(COMPILER_NOPRODUCTION_FLAGS /Zi )
  set(BASE_LINKER_FLAGS /NODEFAULTLIB:LIBCMT /ignore:4099 /DEBUG )
  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /OPT:REF,ICF")
  set(NOPRODUCTION_LINKER_FLAGS "")
  set(PRODUCTION_LINKER_FLAGS /LTCG )
  add_definitions(-DNOMINMAX)
else() # clang or gcc
  set(COMPILER_DEBUG_FLAGS -g )
  set(COMPILER_RELEASE_FLAGS -O3 -g -DNDEBUG )
  set(COMPILER_NOPRODUCTION_FLAGS "")
  set(BASE_LINKER_FLAGS "")
  set(NOPRODUCTION_LINKER_FLAGS "")
  set(PRODUCTION_LINKER_FLAGS "")
  if(BUILD_MAC)
    set(COMPILER_DEBUG_FLAGS ${COMPILER_DEBUG_FLAGS} -mmacosx-version-min=10.4 -fvisibility=hidden)
    set(COMPILER_RELEASE_FLAGS ${COMPILER_RELEASE_FLAGS} -mmacosx-version-min=10.4 -fvisibility=hidden)
    set(STRIPPED_EXE_LINKER_FLAGS "-Xlinker -unexported_symbol -Xlinker \"*\" -Xlinker -dead_strip -Xlinker -dead_strip_dylibs")
  else()
    set(STRIPPED_EXE_LINKER_FLAGS " ")
    set(COMPILER_DEBUG_FLAGS ${COMPILER_DEBUG_FLAGS} -fvisibility=hidden)
    set(COMPILER_RELEASE_FLAGS ${COMPILER_RELEASE_FLAGS} -fvisibility=hidden)
  endif()
endif()

if(BUILD_WINDOWS)
  set(COMPILER_FLAGS ${COMPILER_FLAGS} ${BASE_MSVC_FLAGS})
elseif(BUILD_MAC)
  set(COMPILER_FLAGS ${COMPILER_FLAGS} ${BASE_CLANG_FLAGS})
else() # Linux
  set(COMPILER_FLAGS ${COMPILER_FLAGS} ${BASE_GCC_FLAGS})
endif()

# # TODO: build options for other compilers
# set(CMAKE_CXX_FLAGS "-Wall -Werror") # -Werror can give problems under clang due to -Wtautological-compare see pragma in array.hpp and componentindex.hpp to disable it as needed.
# set(CMAKE_CXX_FLAGS_DEBUG "-g -O3") # this uses -O3 so that the tests run reasonably quickly, even though the inlining may interfere with step-debugging.
# set(CMAKE_CXX_FLAGS_DEBUG_NO_OPTIMIZE "-g -O0") # this uses -O0, which is useful for step-debugging sometimes.
# set(CMAKE_CXX_FLAGS_RELEASE "-g -O3 -DNDEBUG")
# set(CMAKE_CXX_FLAGS_ASMEXAM "-g0 -O3 -save-temps -DNDEBUG")

# temp tests
add_executable(algebraic_expression_prototype algebraic_expression_prototype.cpp)
add_executable(asm_exam asm_exam.cpp asm_exam_separate_functions.cpp)
add_executable(conceptual_inheritance_prototype conceptual_inheritance_prototype.cpp)
add_executable(contraction_prototype contraction_prototype.cpp)
add_executable(eigen_interop_prototype eigen_interop_prototype.cpp)
add_executable(list list.cpp)
add_executable(strongly_typed_vector_prototype strongly_typed_vector_prototype.cpp)
add_executable(tuple tuple.cpp)
add_executable(typelist typelist.cpp)
add_executable(typetuple typetuple.cpp)

set(conceptual_prototype_SRCS
    conceptual_prototype.cpp
    conceptual_prototype_00.cpp
    conceptual_prototype_01.cpp
    conceptual_prototype_02.cpp
    conceptual_prototype_03.cpp
    conceptual_prototype_04.cpp
    conceptual_prototype_05.cpp
    conceptual_prototype_06.cpp
    conceptual_prototype_07.cpp
    conceptual_prototype_08.cpp
    conceptual_prototype_09.cpp
    conceptual_prototype_10.cpp
    conceptual_prototype_11.cpp)
add_executable(conceptual_prototype ${conceptual_prototype_SRCS})

# test-framework-using tests
set(test_SRCS
    lvd/lvd_callstack.cpp
    lvd/lvd_commandlineparser.cpp
    lvd/lvd_spawner.cpp
    lvd/lvd_testsystem.cpp
    standard/randomize.cpp
    standard/test_abstractindex.cpp
    standard/test_array.cpp
    standard/test_dimindex.cpp
    standard/test_expressiontemplate_reindex.cpp
    standard/test_list.cpp
    standard/test_tuple.cpp
    standard/test_typelist.cpp
    standard/test_vector.cpp)
add_executable(test standard/test.cpp ${test_SRCS})
include_directories(${tensorheaven_test_SOURCE_DIR}/.. ${tensorheaven_test_SOURCE_DIR}/lvd ${tensorheaven_test_SOURCE_DIR}/standard)

#     standard/test_euclideanembedding.cpp
#     standard/test_euclideanembeddinginverse.cpp
#     standard/test_expressiontemplates.cpp
#     standard/test_interop_eigen_euclideanlyembedded.cpp
#     standard/test_interop_eigen_euclideanlyembedded0.cpp
#     standard/test_interop_eigen_euclideanlyembedded1.cpp
#     standard/test_interop_eigen_euclideanlyembedded2.cpp
#     standard/test_interop_eigen_euclideanlyembedded3.cpp
#     standard/test_interop_eigen_euclideanlyembedded4.cpp
#     standard/test_interop_eigen_euclideanlyembedded5.cpp
#     standard/test_interop_eigen_euclideanlyembedded6.cpp
#     standard/test_interop_eigen_euclideanlyembedded7.cpp
#     standard/test_interop_eigen_inversion.cpp
#     standard/test_interop_eigen_inversion0.cpp
#     standard/test_interop_eigen_inversion1.cpp
#     standard/test_interop_eigen_inversion2.cpp
#     standard/test_interop_eigen_inversion3.cpp
#     standard/test_interop_eigen_inversion4.cpp
#     standard/test_interop_eigen_inversion5.cpp
#     standard/test_interop_eigen_inversion6.cpp
#     standard/test_interop_eigen_inversion7.cpp
#     standard/test_interop_eigen_ldlt.cpp
#     standard/test_tensor2.cpp
#     standard/test_tensor2diagonal.cpp
#     standard/test_vector.cpp)


set(applications_SRCS
    lvd/lvd_callstack.cpp
    lvd/lvd_commandlineparser.cpp
    lvd/lvd_spawner.cpp
    lvd/lvd_testsystem.cpp
    standard/randomize.cpp
    applications_test/test_homogeneouspolynomials.cpp
    applications_test/test_multivariatepolynomials.cpp)
add_executable(applications_test applications_test/test.cpp ${applications_SRCS})
include_directories(${tensorheaven_test_SOURCE_DIR}/.. ${tensorheaven_test_SOURCE_DIR}/lvd ${tensorheaven_test_SOURCE_DIR}/standard ${tensorheaven_test_SOURCE_DIR}/applications_test)
