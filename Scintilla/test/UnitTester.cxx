/** @file UnitTester.cxx
 ** UnitTester.cpp : Defines the entry point for the console application.
 **/

#define CATCH_CONFIG_WINDOWS_CRTDBG
#define CATCH_CONFIG_RUNNER

#include "catch.hpp"

int main(int argc, char* argv[]) {
	const int result = Catch::Session().run(argc, argv);

	return result;
}
