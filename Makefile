#########################################################
################		Makefile		 ################
#########################################################
###########				 MACROS				  ###########
#########################################################
# $(1) : Compiler
# $(2) : Object file to generate
# $(3) : Source file
# $(4) : Additional dependencies
# $(5) : Compiler flags
define COMPILE
$(2) : $(3) $(4)
	$(1) -c -o $(2) $(3) $(5)
endef

# $(1) : Source file
define C2O
$(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(patsubst $(SRCDIR)%,$(OBJDIR)%,$(1))))
endef

# $(1) : Source file
define C2H
$(patsubst %.c,%.h,$(patsubst %.cpp,%.hpp,$(1)))
endef

# $(1) : The message to print
define PRINT
$(info [$(APP)] $(1))
endef
#########################################################
###########				 CONFIG				  ###########
#########################################################
APP			:= Vincula
CFLAGS		:= #-Wall -pedantic
CCFLAGS 	:= $(CFLAGS) -std=c++17
CC			:= g++
C 			:= gcc
CCWIN		:= x86_64-w64-mingw32-g++
CWIN		:= x86_64-w64-mingw32-gcc
WINDOWSROOT	:= Vincula_WRelease
WINDOWSBIN	:= $(WINDOWSROOT)/bin
BAT 		:= batchs/*

MKDIR		:= mkdir -p
SRCDIR 		:= src
OBJDIR 		:= obj

## LIBS
LIBDIR		:= lib

MANJAROLIBDIR		:= $(LIBDIR)/manjaro
MANJAROSTATICLIBDIR	:= $(MANJAROLIBDIR)/static

WINDOWSLIBDIR		:= $(LIBDIR)/windows
WINDOWSSTATICLIBDIR	:= $(WINDOWSLIBDIR)/static


ifdef WIN
	#LIBSFMOD 	:= -Wl,-rpath=\$$ORIGIN/$(dir $(LIBDIR)/windows/fmod/lib/x86) $(LIBDIR)/windows/fmod/lib/x86/fmod.dll $(LIBDIR)/windows/fmod/lib/x86/fmodstudio.dll #$(LIBDIR)/windows/fmod/lib/fsbank.dll $(LIBDIR)/windows/fmod/lib/libfsbvorbis64.dll
	#LIBSFMOD 	+= -Wl,-rpath=\$$ORIGIN/$(dir $(LIBDIR)/windows/fmod/lib/x64) $(LIBDIR)/windows/fmod/lib/x64/fmod.dll $(LIBDIR)/windows/fmod/lib/x64/fmodstudio.dll #$(LIBDIR)/windows/fmod/lib/fsbank.dll $(LIBDIR)/windows/fmod/lib/libfsbvorbis64.dll
	LIBSFMOD 	:= -Wl,-rpath=\$$ORIGIN/$(dir $(LIBDIR)/windows/fmod/lib/x64) $(LIBDIR)/windows/fmod/lib/x64/fmod.dll $(LIBDIR)/windows/fmod/lib/x64/fmodstudio.dll
	LIBSASSIMP 	:= -Wl,-rpath=\$$ORIGIN/$(dir $(LIBDIR)/windows/assimp/lib/) $(LIBDIR)/windows/assimp/lib/libassimp.dll #$(LIBDIR)/windows/assimp/lib/libgcc_s_seh-1.dll $(LIBDIR)/windows/assimp/lib/libminizip-1.dll $(LIBDIR)/windows/assimp/lib/libwinpthread-1.dll $(LIBDIR)/windows/assimp/lib/zlib1.dll $(LIBDIR)/windows/assimp/lib/libstdc++-6.dll
	LIBFREETYPE	:= -Wl,-rpath=\$$ORIGIN/$(dir $(LIBDIR)/windows/freetype/lib/) $(LIBDIR)/windows/freetype/lib/freetype.dll
else

	ifdef RELEASE
	    FMOD_SUFFIX = #L
	else
	    FMOD_SUFFIX = #L #ACTIVE to FMOD DEBUG
	endif
	FMOD_FLAGS := -pthread -m64 

	LIBSFMOD 	:= $(FMOD_FLAGS) -Wl,-rpath=\$$ORIGIN/$(dir $(LIBDIR)/manjaro/fmod/lib/) $(LIBDIR)/manjaro/fmod/lib/libfmod${FMOD_SUFFIX}.so $(LIBDIR)/manjaro/fmod/lib/libfmodstudio${FMOD_SUFFIX}.so
	LIBSASSIMP 	:= -Wl,-rpath=\$$ORIGIN/$(dir $(LIBDIR)/manjaro/assimp/lib/) $(LIBDIR)/manjaro/assimp/lib/libassimp.so
	LIBFREETYPE	:= -Wl,-rpath=\$$ORIGIN/$(dir $(LIBDIR)/manjaro/freetype/lib/) $(LIBDIR)/manjaro/freetype/lib/libfreetype.so
endif

DYNAMICLIBS := $(LIBSFMOD)
DYNAMICLIBS += $(LIBSASSIMP)
DYNAMICLIBS += $(LIBFREETYPE)

ifdef WIN
	STATICSLIBS	:= -L$(WINDOWSSTATICLIBDIR)/glfw/lib $(WINDOWSSTATICLIBDIR)/glfw/lib/glfw3.dll
	STATICSLIBS	+= -L$(WINDOWSSTATICLIBDIR)/glew/lib $(WINDOWSSTATICLIBDIR)/glew/lib/glew32.dll
else
	STATICSLIBS	:= -L$(MANJAROSTATICLIBDIR)/glew/lib/ $(MANJAROSTATICLIBDIR)/glew/lib/libGLEW.a
	STATICSLIBS	+= -L$(MANJAROSTATICLIBDIR)/glfw/lib/ $(MANJAROSTATICLIBDIR)/glfw/lib/libglfw3.a
	STATICSLIBS	+= -L$(MANJAROSTATICLIBDIR)/glm/lib/  $(MANJAROSTATICLIBDIR)/glm/lib/libglm_static.a
endif

# quitados:
# -lglfw3 -lXi -lXrandr  -lXcursor -lXext -lXxf86vm -L/usr/X11R6/lib

ifdef WIN
	SYSTEMLIBS	:= -lopengl32 
else
	SYSTEMLIBS	:= -lGL -lX11 -ldl -lpthread 
endif

LIBS 		:= $(SYSTEMLIBS)
LIBS 		+= $(STATICSLIBS)
LIBS		+= $(DYNAMICLIBS)

## INCS
STATICINC	:= -I$(SRCDIR) -I$(LIBDIR) 

INCS		:= $(STATICINC)

ifdef WIN
	INCS  		+= -I$(WINDOWSSTATICLIBDIR)/glew/inc -I$(WINDOWSSTATICLIBDIR)/glfw/inc -I$(WINDOWSSTATICLIBDIR)/glm/inc -I$(WINDOWSSTATICLIBDIR)/stb_image/inc
	DYNAMICINC	:= -I$(WINDOWSLIBDIR)/fmod/inc -I$(WINDOWSLIBDIR)/assimp/inc -I$(WINDOWSLIBDIR)/freetype/inc
else
	INCS  		+= -I$(MANJAROSTATICLIBDIR)/glew/inc -I$(MANJAROSTATICLIBDIR)/glfw/inc -I$(MANJAROSTATICLIBDIR)/glm/inc -I$(MANJAROSTATICLIBDIR)/stb_image/inc
	DYNAMICINC	:= -I$(MANJAROLIBDIR)/fmod/inc -I$(MANJAROLIBDIR)/assimp/inc -I$(MANJAROLIBDIR)/freetype/inc
endif

INCS		+= $(DYNAMICINC)

ifdef WIN
	CC := $(CCWIN)
	C := $(CWIN)
endif

ifdef CACHE
	CC := ccache $(CC)
endif

ifdef WIN
	RELEASE := 1
endif

ifdef RELEASE
	CCFLAGS += -O3 -ffast-math -fno-exceptions
else
	CCFLAGS += -g
endif






ALLCPPS		:= $(shell find src/ -type f -iname *.cpp)
ALLCS 		:= $(shell find src/ -type f -iname *.c)
ALLOBJ		:= $(foreach F,$(ALLCPPS) $(ALLCS),$(call C2O,$(F)))
SUBIDRS		:= $(shell find $(SRCDIR) -type d)
OBJSUBDIRS	:= $(patsubst $(SRCDIR)%,$(OBJDIR)%,$(SUBIDRS))


$(APP): $(OBJSUBDIRS) $(ALLOBJ)
	$(call PRINT,Compiling executable file)
	$(CC) -o $(APP) $(ALLOBJ) $(LIBS)

ifdef WIN
	$(call PRINT,Creating Windows release directory)

	@$(MKDIR) $(WINDOWSBIN)/$(SRCDIR)/PapayaEngine/shaders
	@$(MKDIR) $(WINDOWSBIN)/FmodStudioProyect/Build/Desktop/
	@mv $(APP).exe ./$(WINDOWSBIN)

	@cp $(LIBDIR)/windows/assimp/lib/* ./$(WINDOWSBIN) 
	@cp $(LIBDIR)/windows/freetype/lib/* ./$(WINDOWSBIN) 
	@cp $(LIBDIR)/windows/fmod/lib/x64/* ./$(WINDOWSBIN) 
	@cp $(LIBDIR)/windows/static/glew/lib/* ./$(WINDOWSBIN) 
	@cp $(LIBDIR)/windows/static/glfw/lib/* ./$(WINDOWSBIN)

	@cp $(SRCDIR)/PapayaEngine/shaders/* ./$(WINDOWSBIN)/$(SRCDIR)/PapayaEngine/shaders/
	@cp -a FmodStudioProyect/Build/Desktop/* ./$(WINDOWSBIN)/FmodStudioProyect/Build/Desktop/
	@cp -a media ./$(WINDOWSBIN)


	@cp -r media ./$(WINDOWSBIN)
	@cp -r Tipografia ./$(WINDOWSBIN)
	#@cp -r LICENSE ./$(WINDOWSBIN)

	@cp -r $(BAT) ./$(WINDOWSROOT)
		
endif

#Generate rules for all objects
$(foreach F, $(ALLCPPS),$(eval $(call COMPILE,$(CC),$(call C2O,$(F)),$(F),$(call C2H,$(F)),$(CCFLAGS) $(INCS))))
$(foreach F, $(ALLCS),$(eval $(call COMPILE,$(C),$(call C2O,$(F)),$(F),$(call C2H,$(F)),$(CFLAGS) $(INCS))))

$(OBJSUBDIRS): 
	$(MKDIR) $(OBJSUBDIRS)

.PHONY: info clean cleanall all

info:
	$(info $(SUBIDRS))
	$(info $(OBJSUBDIRS))
	$(info $(ALLCPPS))
	$(info $(ALLCS))
	$(info $(ALLOBJ))
	
clean:
	@if [ -d $(OBJDIR) ]; then \
	rm -r "$(OBJDIR)" \
	; fi
	$(call PRINT,Removing .o files)

cleanall: clean
	@if [ -f $(APP) ]; then \
	rm "$(APP)" \
	; fi
	$(call PRINT,Removing executable file)

all: cleanall
	make

