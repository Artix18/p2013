# -*- Makefile -*-

lib_TARGETS = champion

# Tu peux rajouter des fichiers sources, headers, ou changer
# des flags de compilation.
champion-srcs = prologin.cc state.cpp genome.cpp actions.cpp
champion-dists =
champion-cxxflags = -ggdb3 -Wall -Wunused-function -std=c++11

# Evite de toucher a ce qui suit
champion-dists += prologin.hh
STECHEC_LANG=cxx
include ../includes/rules.mk
