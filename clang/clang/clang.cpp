#include <DinrusLLVM/clang/ClangFE.h>

int main(int argc, char **argv)
{
	return clang_main(argc, argv, {argv[0], nullptr, false});
}