#ifndef YAML_H_62B23520_7C8E_11DE_8A39_0800200C9A66
#define YAML_H_62B23520_7C8E_11DE_8A39_0800200C9A66


#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#include <plugin/yaml/parser.h>
#include <plugin/yaml/emitter.h>
#include <plugin/yaml/emitterstyle.h>
#include <plugin/yaml/stlemitter.h>
#include <plugin/yaml/exceptions.h>

#include <plugin/yaml/node/node.h>
#include <plugin/yaml/node/impl.h>
#include <plugin/yaml/node/convert.h>
#include <plugin/yaml/node/iterator.h>
#include <plugin/yaml/node/detail/impl.h>
#include <plugin/yaml/node/parse.h>
#include<plugin/yaml/node/emit.h>

#endif  // YAML_H_62B23520_7C8E_11DE_8A39_0800200C9A66
