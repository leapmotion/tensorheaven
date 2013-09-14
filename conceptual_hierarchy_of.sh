#!/bin/bash

HELP_STRING="
This script is designed to give a commandline interface for generating conceptual\n\
hierarchy graphs quickly and easily using Graphviz's 'dot' application.  If the\n\
single commandline argument \"-cleanup\" is provided, it will delete all the\n\
temporary files left over from previous invocations.  Ordinary commandline options:\n\
\n\
  ./conceptual_hierarchy_of.sh ConceptType path/to/tensorheaven \"decl1\" \"decl2\" ...\n\
\n\
Here, ConceptType is the type of the concept whose hierarchy will be graphed, and\n\
decl1, decl2, etc. are the optional additional supplemental declarations to assist\n\
in defining ConceptType.\n\
\n\
Example:\n\
\n\
  ./conceptual_hierarchy_of.sh BasedVectorSpace . \"#include \\\"tenh/conceptual/vectorspace.hpp\\\"\" \"using namespace Tenh;\" \"struct X { static std::string type_as_string () { return \\\"X\\\"; } };\" \"typedef VectorSpace_c<RealField,3,X> VectorSpace;\" \"typedef BasedVectorSpace_c<VectorSpace,Basis_c<X> > BasedVectorSpace;\"\n\
\n\
This will produce a file called conceptual_hierarchy_of.png, which a graph generated\n\
by the 'dot' application, which visualizes the conceptual ancestors of ConceptType,\n\
where each arrow represents a forgetful functor (the arrow could be read as \"is a\").\n\
"

if [ $# -eq 0 ]
then
    echo -e ${HELP_STRING}
    exit 0
fi

if [ "${1}" == "-cleanup" ]
then
    rm -f conceptual_hierarchy_of.cpp conceptual_hierarchy_of conceptual_hierarchy_of.dot conceptual_hierarchy_of.png
    exit 0
fi

CONCEPT_TYPE=$1
TENSORHEAVEN_DIR=$2

shift 2 # this chomps off the first two params

DECLARATIONS=""
for arg in "$@"
do
    DECLARATIONS="${DECLARATIONS}
${arg}"
done

SOURCE_CODE="
// generated source code

#include \"tenh/conceptual/conceptualinheritancegraph.hpp\"
#include <iostream>
${DECLARATIONS}

int main (int argc, char **argv)
{
    Graph g;
    add_concept_hierarchy_to_graph(${CONCEPT_TYPE}(), g);
    g.print_as_dot_graph(std::cout);
    return 0;
}
"

echo "${SOURCE_CODE}"

# write the generated source code out to a file
echo "${SOURCE_CODE}" > conceptual_hierarchy_of.cpp
# compile the source code, generate the dot file, and generate the png graph
g++ -I ${TENSORHEAVEN_DIR} conceptual_hierarchy_of.cpp -o conceptual_hierarchy_of && \
./conceptual_hierarchy_of > conceptual_hierarchy_of.dot && \
dot conceptual_hierarchy_of.dot -Tpng -oconceptual_hierarchy_of.png
