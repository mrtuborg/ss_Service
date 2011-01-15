
path_to_makefile	:= ../..
program_name		:= s3_Service
build_flags		:= -O0 -g3 -D_DEBUG

core_include_dir	:= include
core_source_dir		:= src
core_source_subdirs	:= . arg_parser buffer deqUdp functions srvAppLayer srvAppLayer/functionNode

paths_to_libraries	:= ../libs

service_include_dir	:= services/3
service_source_dir	:= services/3
service_source_subdirs	:= .    # add some dirs in next inclusion:

include $(service_source_dir)/Makefile.inc
include $(paths_to_libraries)/rcsLib/Makefile.inc


libs_source_dirs	:= $(rcsLib_source_dirs)


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



all : $(program_name)

$(program_name) : obj_dirs $(objects)
	
	@g++ -o $@ $(objects) $(link_flags) $(libraries)

obj_dirs :
	@mkdir -p bin
	@mkdir -p obj
	@mkdir -p obj/Debug
	CURDIR=obj/Debug
	@echo
	@echo "libs_source_dirs: " $(libs_source_dirs)
	@echo
	@echo "object_dirs: " $(objects_dirs)
	@echo
	@echo "relative_include_dirs: " $(relative_include_dirs)
	@echo
	@echo "relative_source_dirs: " $(relative_source_dirs)
	@echo
	@echo "service_source_subdirs: " $(service_source_subdirs)
	@echo 
	
	@mkdir -p $(objects_dirs)

VPATH := ../../

%.o : %.cpp
	@g++ -o $@ -c $< $(compile_flags) $(build_flags) $(addprefix -I, $(relative_include_dirs))

%.o : %.c
	@g++ -o $@ -c $< $(compile_flags) $(build_flags) $(addprefix -I, $(relative_include_dirs))

.PHONY : clean

clean :
	rm -rf bin obj

include $(wildcard $(addsuffix /*.d, $(objects_dirs)))
