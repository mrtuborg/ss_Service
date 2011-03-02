
# usage: make id=3
id:=.
host:=1

use_libs		:=rcsLib peripheral extra math udpAction conv
app_binary_dir		:=bin/$(shell uname -s)_$(shell uname -r)
app_libray_dir		:=libs
root_build_dir		:=obj
debug_build_dir		:=$(root_build_dir)/Debug
release_build_dir	:=$(root_build_dir)/Release

program_name		:= ss_Service
service_name		:= $(program_name)_$(id)

core_include_dir	:= include
core_source_dir		:= core
core_source_subdirs	:= . arg_parser buffer deqUdp functions SrvAppLayer SrvAppLayer/functionNode


paths_to_libraries	:= ../libs

service_include_dir	:= services/$(id)
service_source_dir	:= services/$(id)
service_source_subdirs	:= .    # add some dirs in next inclusion:

include $(addprefix $(service_source_dir)/,Makefile.inc)

include $(addprefix $(paths_to_libraries)/,rcsLib/Makefile.inc)
lib_dirs:=$(addprefix $(paths_to_libraries)/rcsLib/, $(lib_subdirs))

include $(addprefix $(paths_to_libraries)/,extra/Makefile.inc)
lib_dirs+=$(addprefix $(paths_to_libraries)/extra/, $(lib_subdirs))

include $(addprefix $(paths_to_libraries)/,math/Makefile.inc)
lib_dirs+=$(addprefix $(paths_to_libraries)/math/, $(lib_subdirs))

include $(addprefix $(paths_to_libraries)/,schedule/Makefile.inc)
lib_dirs+=$(addprefix $(paths_to_libraries)/schedule/, $(lib_subdirs))


include $(addprefix $(paths_to_libraries)/,peripheral/Makefile.inc)               
lib_dirs+=$(addprefix $(paths_to_libraries)/peripheral/, $(lib_subdirs))

include $(addprefix $(paths_to_libraries)/,storage/Makefile.inc)
lib_dirs+=$(addprefix $(paths_to_libraries)/storage/, $(lib_subdirs))

include $(addprefix $(paths_to_libraries)/,system/Makefile.inc)
lib_dirs+=$(addprefix $(paths_to_libraries)/system/, $(lib_subdirs))

include $(addprefix $(paths_to_libraries)/,conv/Makefile.inc)
lib_dirs+=$(addprefix $(paths_to_libraries)/conv/, $(lib_subdirs))


compile_flags		:= -Wall -Wno-char-subscripts -MD -pipe
link_flags		:= -pipe
libraries		:= -ldl -pthread


relative_include_dirs	:= 	$(core_include_dir)\
				$(addprefix $(service_source_dir)/,$(service_source_subdirs))\
                                $(paths_to_libraries)
                                
objects_dirs		:= 	$(addprefix $(core_source_dir)/, $(core_source_subdirs))\
				$(addprefix $(service_source_dir)/, $(service_source_subdirs))\
				$(lib_dirs)
				
objects				:= $(patsubst ../../%, %, $(wildcard $(addsuffix /*.c*, $(objects_dirs))))
objects				:= $(objects:.cpp=.o)
objects				:= $(objects:.c=.o)

all: build_dir:=$(release_build_dir)
all: build_flags:=-O2 -fomit-frame-pointer
all: prebuild $(service_name)

debug: build_dir:=$(debug_build_dir)
debug: build_flags:=-O0 -g3 -D_DEBUG
debug: prebuild $(service_name)

prebuild:
	@echo "obj_dirs: " $(libs_source_subdirs)
	@echo "lib_dirs" $(lib_dirs)

	@echo "1. Creating directories structure (bin, lib, objs)"
	@mkdir -p $(app_binary_dir)
	@mkdir -p $(root_build_dir)
	@mkdir -p $(addprefix $(build_dir)/,$(objects_dirs))

$(service_name): $(objects)
	@echo "2. "$(service_name)" linking"
	@$(CXX) -o $(app_binary_dir)/$@ $(addprefix $(build_dir)/,$(objects)) $(link_flags) $(libraries)
	@echo "done"

%.o : %.cpp
	@echo "- Compiling " $<
	@$(CXX) -o $(build_dir)/$@ -c $< $(compile_flags) $(build_flags) $(addprefix -I, $(relative_include_dirs))  -std=gnu++98

%.o : %.c
	@echo "- Compiling " $<
	@$(CC) -o $(build_dir)/$@ -c $< $(compile_flags) $(build_flags) $(addprefix -I, $(relative_include_dirs))


.PHONY : clean
.PHONY : doc
.PHONY : remote

clean : id:=.
clean :
	rm -rf bin obj docs

doc : id:=.
doc : 
	doxygen Doxyfile

include utils/remotes.inc

remote: host_ip	:= $(word $(host), $(REMOTE_IP))
remote: host_user:= $(word $(host), $(REMOTE_USER))
remote: host_pass:= $(word $(host), $(REMOTE_PASSWORD))
remote: clean
	@echo $(program_name) $(host_user):$(host_pass)@$(host_ip)
	utils/./remote_make $(program_name) $(host_ip) $(host_user) $(host_pass) $(id)

#cur_user:= $(foreach n, $(n_list), $(word $(n), $(REMOTE_USER)))


include $(wildcard $(addsuffix /*.d, $(objects_dirs)))
