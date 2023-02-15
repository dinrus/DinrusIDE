ОБЗОР: LDC - компилятор LLVM D

ИСПОЛЬЗОВАНИЕ: ldc2 [опции] файлы --run Выполняет итоговую программу, передавая ей остальные аргументы

ОПЦИИ:

Основные опции:

  -D                                     - Генерировать документацию
  --Dd=<директория>                       - Записать файл документации в директорию <директория>
  --Df=<имя_файла>                        - Записать файл документации в <имя_файла>
  -H                                     - Сгенерировать файл 'header'
  --HC=<значение>                           - Сгенерировать файл C++ header
    =silent                              -   Only list extern(C[++]) declarations (default)
    =verbose                             -   Also add comments for ignored declarations (e.g. extern(D))
  --HCd=<директория>                      - Записать файл C++ 'header' в <директория>
  --HCf=<имя_файла>                       - Записать файл C++ 'header' в <имя_файла>
  --Hd=<директория>                       - Записать файл 'header' в <директория>
  --Hf=<имя_файла>                        - Записать файл 'header' в <имя_файла>
  --Hkeep-all-bodies                     - Keep all function bodies in .di files
  -I=<директория>                         - Look for imports also in <директория>
  -J=<директория>                         - Look for string imports also in <директория>
  Setting the optimization level:
      -O                                   - Equivalent to -O3
      --O0                                  - No optimizations (default)
      --O1                                  - Simple optimizations
      --O2                                  - Good optimizations
      --O3                                  - Aggressive optimizations
      --O4                                  - Equivalent to -O3
      --O5                                  - Equivalent to -O3
      --Os                                  - Like -O2 with extra optimizations for size
      --Oz                                  - Like -Os but reduces code size further
  -X                                     - Generate JSON file
  --Xf=<имя_файла>                        - Write JSON в <имя_файла>
  --allinst                              - Generate code for all template instantiations
  --betterC                              - Omit generating some runtime information and helper functions
  --boundscheck=<значение>                  - Array bounds check
    =off                                 -   Disabled
    =safeonly                            -   Enabled for @safe functions only
    =on                                  -   Enabled for all functions
  -c                                     - Compile only, do not link
  --cache=<cache dir>                    - Enable compilation cache, using <cache dir> to store cache files (experimental)
  --cache-prune                          - Enable cache pruning.
  --cache-prune-expiration=<dur>         - Sets the pruning expiration time of cache files to <dur> seconds (default: 1 week). Implies -cache-prune.
  --cache-prune-interval=<dur>           - Sets the cache pruning interval to <dur> seconds (default: 20 min). Set to 0 to force pruning. Implies -cache-prune.
  --cache-prune-maxbytes=<size>          - Sets the maximum cache size to <size> bytes. Implies -cache-prune.
  --cache-prune-maxpercentage=<perc>     - Sets the cache size limit to <perc> percent of the available space (default: 75%). Implies -cache-prune.
  --cache-retrieval=<значение>              - Set the cache retrieval mechanism (default: copy).
    =copy                                -   Make a copy of the cache file
    =hardlink                            -   Create a hard link to the cache file (recommended)
    =link                                -   Equal to 'hardlink' on Windows, but 'symlink' on Unix and OS X
    =symlink                             -   Create a symbolic link to the cache file
  --check-printf-calls                   - Validate printf call format strings against arguments
  --checkaction=<значение>                  - Action to take when an assert/boundscheck/final-switch fails
    =D                                   -   Usual D behavior of throwing an AssertError
    =C                                   -   Call the C runtime library assert failure function
    =halt                                -   Halt the program execution (very lightweight)
    =context                             -   Use D assert with context information (when available)
  --conf=<имя_файла>                      - Use configuration file <имя_файла>
  --cov                                  - Compile-in code coverage analysis and .lst file generation
                                           Use -cov=<n> for n% minimum required coverage
                                           Use -cov=ctfe to include code executed during CTFE
  --cov-increment=<значение>                - Set the type of coverage line count increment instruction
    =default                             -   Use the default (atomic)
    =atomic                              -   Atomic increment
    =non-atomic                          -   Non-atomic increment (not thread safe)
    =boolean                             -   Don't read, just set counter to 1
  Allow deprecated language features and symbols:
      -d                                   - Silently allow deprecated features and symbols
      --dw                                  - Issue a message when deprecated features or symbols are used (default)
      --de                                  - Issue an error when deprecated features or symbols are used (halt compilation)
  --d-debug[=<level/idents>]               - Compile in debug code >= <level> or identified by <idents>
  --d-version=<level/idents>             - Compile in version code >= <level> or identified by <idents>
  --deps[=<имя_файла>]                      - Write module dependencies to <имя_файла> (only imports). '-deps' alone prints module dependencies (imports/file/version/debug/lib)
  --enable-asserts=<значение>               - (*) Enable assertions
  --disable-d-passes                     - Disable all D-specific passes
  --disable-gc2stack                     - Disable promotion of GC allocations to stack memory
  --enable-invariants=<значение>            - (*) Enable invariants
  --disable-loop-unrolling               - Disable loop unrolling in all relevant passes
  --disable-loop-vectorization           - Disable the loop vectorization pass
  --disable-red-zone                     - Do not emit code that uses the red zone.
  --disable-simplify-drtcalls            - Disable simplification of druntime calls
  --disable-simplify-libcalls            - Disable simplification of well-known C runtime calls
  --dllimport=<значение>                    - Windows only: which extern(D) global variables to dllimport implicitly if not defined in a root module
    =none                                -   None (default with -link-defaultlib-shared=false)
    =defaultLibsOnly                     -   Only druntime/Phobos symbols (default with -link-defaultlib-shared and -fvisibility=hidden).
    =all                                 -   All (default with -link-defaultlib-shared and -fvisibility=public)
  --dwarf-version=<int>                  - Dwarf version
  --enable-color=<значение>                 - (*) Force colored console output
  --enable-contracts=<значение>             - (*) Enable function pre- and post-conditions
  --enable-inlining=<значение>              - (*) Enable function inlining (default in -O2 and higher)
  --enable-postconditions=<значение>        - (*) Enable function postconditions
  --enable-preconditions=<значение>         - (*) Enable function preconditions
  --enable-switch-errors=<значение>         - (*) Enable runtime errors for unhandled switch cases
  --extern-std=<значение>                   - C++ standard for name mangling compatibility
    =c++98                               -   Sets `__traits(getTargetInfo, "cppStd")` to `199711`
    =c++11                               -   Sets `__traits(getTargetInfo, "cppStd")` to `201103` (default)
    =c++14                               -   Sets `__traits(getTargetInfo, "cppStd")` to `201402`
    =c++17                               -   Sets `__traits(getTargetInfo, "cppStd")` to `201703`
    =c++20                               -   Sets `__traits(getTargetInfo, "cppStd")` to `202002`
  --fdmd-trace-functions                 - DMD-style runtime performance profiling of generated code
  --ffast-math                           - Set @fastmath for all functions.
  --finstrument-functions                - Instrument function entry and exit with GCC-compatible profiling calls
  --float-abi=<значение>                    - ABI/operations to use for floating-point types:
    =default                             -   Target default floating-point ABI
    =soft                                -   Software floating-point ABI and operations
    =softfp                              -   Soft-float ABI, but hardware floating-point instructions
    =hard                                -   Hardware floating-point ABI and instructions
  --flto=<значение>                         - Set LTO mode, requires linker support
    =full                                -   Merges all input into a single module
    =thin                                -   Parallel importing and codegen (faster than 'full')
  --flto-binary=<file>                   - Set the linker LTO plugin library file (e.g. LLVMgold.so (Unixes) or libLTO.dylib (Darwin))
  --fno-plt                              - Do not use the PLT to make function calls
  --fp-contract=<значение>                  - Enable aggressive formation of fused FP ops
    =fast                                -   Fuse FP ops whenever profitable
    =on                                  -   Only fuse 'blessed' FP ops.
    =off                                 -   Only fuse FP ops when the result won't be affected.
  --fprofile-generate[=<имя_файла>]         - Generate instrumented code to collect a runtime profile into default.profraw (overriden by '=<имя_файла>' or LLVM_PROFILE_FILE env var)
  --fprofile-instr-generate[=<имя_файла>]   - Generate instrumented code to collect a runtime profile into default.profraw (overriden by '=<имя_файла>' or LLVM_PROFILE_FILE env var)
  --fprofile-instr-use=<имя_файла>        - Use instrumentation data for profile-guided optimization
  --fprofile-use=<имя_файла>              - Use instrumentation data for profile-guided optimization
  --frame-pointer=<значение>                - Specify frame pointer elimination optimization
    =all                                 -   Disable frame pointer elimination
    =non-leaf                            -   Disable frame pointer elimination for non-leaf frame
    =none                                -   Enable frame pointer elimination
  --fsanitize=<checks>                   - Turn on runtime checks for various forms of undefined or suspicious behavior.
  --fsanitize-blacklist=<file>           - Add <file> to the blacklist files for the sanitizers.
  --fsanitize-coverage=<type>            - Specify the type of coverage instrumentation for -fsanitize
  --fsanitize-memory-track-origins=<int> - Enable origins tracking in MemorySanitizer (0=disabled, default)
  --fsave-optimization-record[=<имя_файла>] - Generate a YAML optimization record file of optimizations performed by LLVM
  --fthread-model=<значение>                - Thread model
    =global-dynamic                      -   Global dynamic TLS model (default)
    =local-dynamic                       -   Local dynamic TLS model
    =initial-exec                        -   Initial exec TLS model
    =local-exec                          -   Local exec TLS model
  --ftime-trace                          - Turn on time profiler. Generates JSON file based on the output имя_файла (also see --ftime-trace-file).
  --ftime-trace-file=<имя_файла>          - Specify time trace file destination
  --ftime-trace-granularity=<uint>       - Minimum time granularity (in microseconds) traced by time profiler
  --fvisibility=<значение>                  - Default visibility of symbols
    =default                             -   Hidden for Windows targets without -shared, otherwise public
    =hidden                              -   Only export symbols marked with 'export'
    =public                              -   Export all symbols
  --fxray-instruction-threshold=<значение>  - Sets the minimum function size to instrument with XRay
  --fxray-instrument                     - Generate XRay instrumentation sleds on function entry and exit
  Generating debug information:
      -g                                   - Add symbolic debug info
      --gc                                  - Add symbolic debug info, optimize for non D debuggers
      --gline-tables-only                   - Add line tables only
  --gdwarf                               - Emit DWARF debuginfo (instead of CodeView) for MSVC targets
  --hash-threshold=<uint>                - Hash symbol names longer than this threshold (experimental)
  -i[=<pattern>]                           - Include imported modules in the compilation
  --ignore                               - Ignore unsupported pragmas
  --lib                                  - Create static library
      --linkonce-templates                  - Use discardable linkonce_odr linkage for template symbols and lazily & recursively define all referenced instantiated symbols in each object file
      --linkonce-templates-aggressive       - Experimental, more aggressive variant
  --lowmem                               - Enable the garbage collector for the LDC front-end. This reduces the compiler memory requirements but increases compile times.
  --m32                                  - 32 bit target
  --m64                                  - 64 bit target
  --main                                 - Add default main() if not present already (e.g. for unittesting)
  --makedeps[=<имя_файла>]                  - Write module dependencies in Makefile compatible format to <имя_файла>/stdout (only imports)
  --march=<string>                       - Architecture to generate code for (see --version)
  --mattr=<a1,+a2,-a3,...>               - Target specific attributes (-mattr=help for details)
  --mcpu=<cpu-name>                      - Target a specific cpu type (-mcpu=help for details)
  --mdcompute-file-prefix=<prefix>       - Prefix to prepend to the generated kernel files.
  --mdcompute-targets=<targets>          - Generates code for the specified DCompute target list. Use 'ocl-xy0' for OpenCL x.y, and 'cuda-xy0' for CUDA CC x.y
  --mixin=<имя_файла>                     - Expand and save mixins to <имя_файла>
  --mtriple=<string>                     - Override target triple
  --mv=<<package.module>=<filespec>>     - Use <filespec> as source file for <package.module>
  --noasm                                - Disallow use of inline assembler
  --nogc                                 - Do not allow code that generates implicit garbage collector calls
  --o-                                   - Do not write object file
  --od=<директория>                       - Write object files to <директория>
  --of=<имя_файла>                        - Use <имя_файла> as output file name
  --op                                   - Preserve source path for output files
  --oq                                   - Write object files with fully qualified names
  --output-bc                            - Write LLVM bitcode
  --output-ll                            - Write LLVM IR
  --output-mlir                          - Write MLIR
  --output-o                             - Write native object
  --output-s                             - Write native assembly
  --preview=<name>                       - Enable an upcoming language change identified by <name>, use ? for list
  --release                              - Compile release version, defaulting to disabled asserts/contracts/invariants, and bounds checks in @safe functions only
  --relocation-model=<значение>             - Choose relocation model
    =static                              -   Non-relocatable code
    =pic                                 -   Fully relocatable, position independent code
    =dynamic-no-pic                      -   Relocatable external references, non-relocatable code
    =ropi                                -   Code and read-only data relocatable, accessed PC-relative
    =rwpi                                -   Read-write data relocatable, accessed relative to static base
    =ropi-rwpi                           -   Combination of ropi and rwpi
  --revert=<name>                        - Revert language change identified by <name>, use ? for list
  --run <string>...                      - Runs the resulting program, passing the remaining arguments to it
  --shared                               - Create shared library (DLL)
  --singleobj                            - Create only a single output object file
  --template-depth=<uint>                - Set maximum number of nested template instantiations
  --transition=<name>                    - Help with language change identified by <name>, use ? for list
  --unittest                             - Compile in unit tests
  -v                                     - Verbose
  --v-cg                                 - Verbose codegen
  --vcolumns                             - Print character (column) numbers in diagnostics
  --verror-style=<значение>                 - Set the style for file/line number annotations on compiler messages
    =digitalmars                         -   'file(line[,column]): message' (default)
    =gnu                                 -   'file:line[:column]: message', conforming to the GNU standard used by gcc and clang
  --verrors=<uint>                       - Limit the number of error messages (0 means unlimited)
  --verrors-context                      - Show error messages with the context of the erroring source line
  --verrors-spec                         - Show errors from speculative compiles such as __traits(compiles,...)
  --vgc                                  - List all gc allocations including hidden ones
  --vtemplates                           - List statistics on template instantiations
                                           Use -vtemplates=list-instances to additionally show all instantiation contexts for each template
  --vv                                   - Print front-end/glue code debug log
  Warnings:
      -w                                   - Enable warnings as errors (compilation will halt)
      --wi                                  - Enable warnings as messages (compilation will continue)

Generic Options:

  --help                                 - Display available options (--help-hidden for more)
  --help-list                            - Display list of available options (--help-list-hidden for more)
  --version                              - Display the version of this program

Linking options:

  -L=<linkerflag>                        - Pass <linkerflag> to the linker
  --Xcc=<ccflag>                         - Pass <ccflag> to GCC/Clang for linking
  --defaultlib=<lib1,lib2,...>           - Default libraries to link with (overrides previous)
  --disable-linker-strip-dead            - Do not try to remove unused symbols during linking
  --gcc=<gcc|clang|...>                  - C compiler to use for linking (and external assembling). Defaults to the CC environment variable if set, otherwise to `cc`.
  --link-defaultlib-debug                - Link with debug versions of default libraries
  --link-defaultlib-shared               - Link with shared versions of default libraries. Defaults to true when generating a shared library (-shared).
  --link-internally                      - Use internal LLD for linking
  --linker=<lld-link|lld|gold|bfd|...>   - Set the linker to use. When explicitly set to '' (nothing), prevents LDC from passing `-fuse-ld` to `cc`.
  --mscrtlib=<libcmt[d]|msvcrt[d]>       - MS C runtime library to link with
  --platformlib=<lib1,lib2,...>          - Platform libraries to link with (overrides previous)
  --static                               - Create a statically linked binary, including all system dependencies

-d-debug can also be specified without options, in which case it enables all debug checks (i.e. asserts, boundschecks, contracts and invariants) as well as acting as -d-debug=1.

Boolean options can take an optional значение, e.g., -link-defaultlib-shared=<true,false>.
Boolean options marked with (*) also have a -disable-FOO variant with inverted meaning.
