#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/summerbaby/Desktop/summmbaby/myOpenGL/build
  make -f /Users/summerbaby/Desktop/summmbaby/myOpenGL/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/summerbaby/Desktop/summmbaby/myOpenGL/build
  make -f /Users/summerbaby/Desktop/summmbaby/myOpenGL/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/summerbaby/Desktop/summmbaby/myOpenGL/build
  make -f /Users/summerbaby/Desktop/summmbaby/myOpenGL/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/summerbaby/Desktop/summmbaby/myOpenGL/build
  make -f /Users/summerbaby/Desktop/summmbaby/myOpenGL/build/CMakeScripts/ReRunCMake.make
fi

