{ stdenv, mkShell, clang-tools }:

mkShell {
  CROSS_COMPILE = stdenv.cc.targetPrefix;
  depsBuildBuild = [ clang-tools ];
  hardeningDisable = [ "all" ];
}
