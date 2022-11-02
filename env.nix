{ stdenv, mkShell, clang-tools, qemu }:

mkShell {
  CROSS_COMPILE = stdenv.cc.targetPrefix;
  depsBuildBuild = [ clang-tools qemu ];
  hardeningDisable = [ "all" ];
}
