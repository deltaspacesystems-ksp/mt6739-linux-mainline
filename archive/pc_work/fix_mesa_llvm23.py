from pathlib import Path
p=Path('/home/y5/mesa-native/src/compiler/clc/clc_helpers.cpp');s=p.read_text();assert 'Driver::GetResourcesPath(std::string(clang_path));' in s
anchor='#include <clang/Driver/Driver.h>'
assert anchor in s
s=s.replace(anchor,anchor+'\n#if __has_include(<clang/Options/OptionUtils.h>)\n#include <clang/Options/OptionUtils.h>\n#endif')
s=s.replace('#if LLVM_VERSION_MAJOR >= 20\n      Driver::GetResourcesPath', '#if LLVM_VERSION_MAJOR >= 23\n      clang::GetResourcesPath(std::string(clang_path));\n#elif LLVM_VERSION_MAJOR >= 20\n      Driver::GetResourcesPath')
s=s.replace('auto target = TargetRegistry::lookupTarget(triple, error_msg);','''auto target = TargetRegistry::lookupTarget(
#if LLVM_VERSION_MAJOR >= 23
         llvm::Triple(triple),
#else
         triple,
#endif
         error_msg);''')
p.write_text(s)
