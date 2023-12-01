#if defined(SIMDJSON_CONDITIONAL_INCLUDE) && !defined(SIMDJSON_SRC_GENERIC_DEPENDENCIES_H)
#error generic/dependencies.h must be included before generic/amalgamated.h!
#endif

#include "base.h"
#include "dom_parser_implementation.h"
#include "json_character_block.h"
