
# usage: make id=3

app_binary_dir		:=bin/$(shell uname -s)
app_libray_dir		:=libs
root_build_dir		:=obj
debug_build_dir		:=$(root_build_dir)/Debug
release_build_dir	:=$(root_build_dir)/Release

program_name		:= s$(id)_Service
build_flags		:= -O0 -g3 -D_DEBUG

core_include_dir	:= include
core_source_dir		:= src
core_source_subdirs	:= . arg_parser buffer deqUdp functions srvAppLayer srvAppLayer/functionNode

paths_to_libraries	:= ../libs

service_include_dir	:= services/$(id)
service_source_dir	:= services/$(id)
service_source_subdirs	:= .    # add some dirs in next inclusion:

include $(service_source_dir)/Makefile.inc
include $(paths_to_libraries)/rcsLib/Makefile.inc


libs_source_dirs	:= $(lib_source_dirs)


compile_flags		:= -Wall -MD -pipe
link_flags		:= -pipe
libraries		:= -ldl

relative_include_dirs	:= 	$(core_include_dir)\
				$(addprefix $(service_source_dir)/,$(service_source_subdirs))\
                                $(paths_to_libraries)
                                
relative_source_dirs	:= 	$(addprefix $(core_source_dir)/, $(core_source_subdirs))\
				$(addprefix $(service_source_dir)/, $(service_source_subdirs))\
				$(addprefix $(paths_to_libraries)/rcsLib/,$(libs_source_dirs))

objects_dirs		:= 	$(addprefix $(core_source_dir)/, $(core_source_subdirs))\
				$(addprefix $(service_source_dir)/, $(service_source_subdirs))\
				$(addprefix $(paths_to_libraries)/rcsLib/,$(libs_source_dirs))
				
objects				:= $(patsubst ../../%, %, $(wildcard $(addsuffix /*.c*, $(relative_source_dirs))))
objects				:= $(objects:.cpp=.o)
objects				:= $(objects:.c=.o)

all: build_dir:=$(release_build_dir)
all: prebuild $(program_name)

debug: build_dir:=$(debug_build_dir)
debug: prebuild $(program_name)

prebuild:
	@echo " "
	@echo "1. Creating directories structure (bin, lib, objs)"
	@mkdir -p $(app_binary_dir)
	@mkdir -p $(root_build_dir)
	@mkdir -p $(addprefix $(build_dir)/,$(objects_dirs))

$(program_name): $(objects)
	@echo "2. "$(program_name)" linking"
	@g++ -o $(app_binary_dir)/$@ $(addprefix $(build_dir)/,$(objects)) $(link_flags) $(libraries)
	@echo "done"

%.o : %.cpp
	@echo "- Compiling " $<
	@g++ -o $(build_dir)/$@ -c $< $(compile_flags) $(build_flags) $(addprefix -I, $(relative_include_dirs))

%.o : %.c
	@echo "- Compiling " $<
	@g++ -o $(build_dir)/$@ -c $< $(compile_flags) $(build_flags) $(addprefix -I, $(relative_include_dirs))


.PHONY : clean

clean :
	rm -rf bin obj

include $(wildcard $(addsuffix /*.d, $(objects_dirs)))
