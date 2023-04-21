/** @file testRESearch.cxx
 ** Unit Tests for Scintilla internal data structures
 **/

#include <cstddef>
#include <cstring>
#include <stdexcept>
#include <string_view>
#include <vector>
#include <optional>
#include <algorithm>
#include <memory>

#include <Scintilla/ScintillaTypes.h>

#include <Scintilla/code/Debugging.h>

#include <Scintilla/code/Position.h>
#include <Scintilla/code/SplitVector.h>
#include <Scintilla/code/Partitioning.h>
#include <Scintilla/code/RunStyles.h>
#include <Scintilla/code/CellBuffer.h>
#include <Scintilla/code/CharClassify.h>
#include <Scintilla/code/RESearch.h>

#include "catch.hpp"

using namespace Scintilla;
using namespace Scintilla::Internal;

class StringCI : public CharacterIndexer {
	std::string s;
public:
	StringCI(std::string_view sv_) : s(sv_) {
	}
	Sci::Position Length() const noexcept {
		return s.length();
	}
	char CharAt(Sci::Position index) const override {
		return s.at(index);
	}
};

// Test RESearch.

TEST_CASE("RESearch") {

	CharClassify cc;
	const char sTextSpace[] = "Scintilla ";
	const char pattern[] = "[a-z]+";

	SECTION("Compile") {
		std::unique_ptr<RESearch> re = std::make_unique<RESearch>(&cc);
		const char *msg = re->Compile(pattern, strlen(pattern), true, false);
		REQUIRE(nullptr == msg);
	}

	SECTION("Execute") {
		std::unique_ptr<RESearch> re = std::make_unique<RESearch>(&cc);
		re->Compile(pattern, strlen(pattern), true, false);
		StringCI sci(sTextSpace);
		const int x = re->Execute(sci, 0, sci.Length());
		REQUIRE(x == 1);
		REQUIRE(re->bopat[0] == 1);
		REQUIRE(re->eopat[0] == sci.Length() - 1);
	}

	SECTION("Grab") {
		std::unique_ptr<RESearch> re = std::make_unique<RESearch>(&cc);
		re->Compile(pattern, strlen(pattern), true, false);
		StringCI sci(sTextSpace);
		re->Execute(sci, 0, sci.Length());
		re->GrabMatches(sci);
		REQUIRE(re->pat[0] == "cintilla");
	}

}
