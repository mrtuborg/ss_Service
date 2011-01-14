
core_include_dir	:= include
core_source_dir		:= src
core_source_subdirs	:= . arg_parser buffer deqUdp functions srvAppLayer srvAppLayer/functionNode
paths_to_libraries	:= ../libs

service_include_dir	:= services/3
service_source_dir	:= services/3
service_source_subdirs	:= .

include ../../$(service_source_dir)/Makefile.inc
include ../../$(paths_to_libraries)/rcsLib/Makefile.inc

libs_source_dirs	:= $(rcsLib_source_dirs)


compile_flags		:= -Wall -MD -pipe
link_flags			:= -s -pipe
libraries			:= -ldl

relative_include_dirs	:= 	$(addprefix ../../, $(core_include_dir))\
                                $(addprefix ../../, $(service_include_dir))\
                                $(addprefix ../../$(service_include_dir)/, $(service_source_subdirs))\
                                $(addprefix ../../, $(paths_to_libraries))
                                
relative_source_dirs	:= 	$(addprefix ../../$(core_source_dir)/, $(core_source_subdirs))\
				$(addprefix ../../$(service_source_dir)/, $(service_source_subdirs))\
				$(addprefix ../../$(paths_to_libraries)/rcsLib/,$(libs_source_dirs))

objects_dirs		:= 	$(addprefix $(core_source_dir)/, $(core_source_subdirs))\
				$(addprefix $(service_source_dir)/, $(service_source_subdirs))\
				$(addprefix $(paths_to_libraries)/rcsLib/,$(libs_source_dirs))
				
objects					:= $(patsubst ../../%, %, $(wildcard $(addsuffix /*.c*, $(relative_source_dirs))))
objects					:= $(objects:.cpp=.o)
objects					:= $(objects:.c=.o)

all : $(program_name)

$(program_name) : obj_dirs $(objects)
	g++ -o $@ $(objects) $(link_flags) $(libraries)

obj_dirs :
	echo "libs_source_dirs: " $(libs_source_dirs)
	echo "object_dirs: " $(objects_dirs)
	echo "relative_include_dirs: " $(relative_include_dirs)
	echo "relative_source_dirs: " $(relative_source_dirs)
	echo "service_source_subdirs: " $(service_source_subdirs)
	mkdir -p $(objects_dirs)

VPATH := ../../

%.o : %.cpp
	g++ -o $@ -c $< $(compile_flags) $(build_flags) $(addprefix -I, $(relative_include_dirs))

%.o : %.c
	g++ -o $@ -c $< $(compile_flags) $(build_flags) $(addprefix -I, $(relative_include_dirs))

.PHONY : clean

clean :
	rm -rf bin obj

include $(wildcard $(addsuffix /*.d, $(objects_dirs)))
