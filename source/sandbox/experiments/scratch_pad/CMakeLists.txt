cmake_minimum_required(VERSION 3.25.0)

declare_target(ScratchPad)

include_src(decorator_pattern.h)
include_src(scratch_pad.cc)
include_src(bit_manipulation_experiments.h)
include_src(cpp_constructs_experiments.h)
make_executable_target()
group_sources("")
group_target("")
