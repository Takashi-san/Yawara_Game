#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# Assume-se uma distribuição Linux como sistema operacional padrão
#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#O compilador
COMPILER = g++
# Comando para remover pastas
RMDIR = rm -rdf
#Comando para remover arquivos
RM = rm -f

#"Flags" para a geração automática de dependências
DEP_FLAGS = -M -MT $@  -MT $(BIN_PATH)/$(*F).o -MP -MF $@
#Bibliotecas a serem linkadas
LIBS = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lm
#Caminho dos includes
INC_PATHS = $(addprefix -I,$(INC_PATH)) $(addprefix -I,$(SDL_INC_PATHS))

#Diretivas de compilação
FLAGS = -std=c++11 -Wall -g -pedantic -Wextra -Wno-unused-parameter -Werror=init-self
#Diretivas extras para debug
DFLAGS = -ggdb -O0 -DDEBUG
#Diretivas extras para release
RFLAGS = -O3 -mtune=native

INC_PATH = $(sort $(dir $(wildcard include/*/*/)))
INC_PATH += include/
SRC_PATH = $(sort $(dir $(wildcard src/*/*/)))
SRC_PATH += src/
BIN_PATH = build/bin
DEP_PATH = build/dep

# Uma lista de arquivos por extensão:
CPP_FILES = $(wildcard $(addsuffix *.cpp, $(SRC_PATH)))
INC_FILES = $(wildcard $(addsuffix *.hpp, $(INC_PATH)))
FILE_NAMES = $(sort $(notdir $(CPP_FILES:.cpp=)) $(notdir $(INC_FILES:.hpp=)))
DEP_FILES	=	$(addprefix	$(DEP_PATH)/,$(addsufix	.d,$(FILE_NAMES)))
OBJ_FILES	=	$(addprefix	$(BIN_PATH)/,$(notdir	$(CPP_FILES:.cpp=.o)))

# Nome do executável
EXEC = yawara
#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# Caso o sistema seja Windows
#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ifeq (($OS),Windows_NT)

#Comando para remover um diretório recursivamente
RMDIR = rd /s /q

#comando para deletar um único arquivo
RM = del /q

#Possiíveis paths da SDL. Caso seja possível ter mais de um locak, adicione espaço entre eles
#Por exempolo.: SDL_PATHs = C:/SDL2 D:/Tools/SDL2 C:/dev-tools/SDL2s
SDL_PATH = C:/SDL2/x86_64-w64-mingw32 C:/Tools/msys64/mingw64

SDL_INC_PATH += $(addsuffix /include,$(SDL_PATHS))
LINK_PATH = $(addprefix -L,$(addsufix /lib,$(SDL_PATHS)))
FLAGS += -mwindows
DFLAGS += -mconsole
LIBS := -lmingw32 -lSDL2main $(LIBS)

#nome do executável
EXEC := $(EXEC).exe

else

UNAME_S := $(shell uname -s)

#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# Caso o sistema seja Mac
#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

ifeq ($(UNAME_S), Darwin)
LIBS = -lm -framework SDL2 -framework SDL2_image -framework SDL2_mixer -framework SDL2_ttf

endif
endif
################################################################################################################################################################################

.PRECIOUS: $(DEP_FILES)
.PHONY: release debug clean folders help

#Regra geral
all:	$(EXEC)

#Gera o executável
$(EXEC):	$(OBJ_FILES)
	$(COMPILER)  -o $@ $^ $(LINK_PATH) $(LIBS) $(FLAGS)

#Gera os arquivos objetos
$(BIN_PATH)/%.o:	$(DEP_PATH)/%.d	|	folders
	$(COMPILER) $(INC_PATHS) $(subst //,/,$(filter %/$*.cpp, $(CPP_FILES))) -c $(FLAGS) -o $@

.SECONDEXPANSION:
#Gera os arquivos de dependência
$(DEP_PATH)/%.d:	$$(filter $$(addsuffix $$*.cpp, $$(SRC_PATH)), $$(CPP_FILES))	|	folders
	$(COMPILER) $(INC_PATHS) $(subst //,/,$(filter %/$*.cpp, $(CPP_FILES))) $(DEP_FLAGS) $(FLAGS)

clean:
		-$(RMDIR) $(DEP_PATH)
		-$(RMDIR) $(BIN_PATH)
		-$(RM) $(EXEC)

release: FLAGS += $(RFLAFS)
release: $(EXEC)

debug: FLAGS += $(DFLAGS)
debug: $(EXEC)

folders:
ifeq ($(OS), Windows_NT)
	@if NOT exists $(DEP_PATH) (	mkdir $(DEP_PATH)	)
	@if NOT exists $(BIN_PATH) (	mkdir $(BIN_PATH)	)
	@if NOT exists $(INC_PATH) (	mkdir $(INC_PATH)	)
	@if NOT exists $(SRC_PATH) (	mkdir $(SRC_PATH)	)
else
	@mkdir -p $(DEP_PATH) $(BIN_PATH) $(INC_PATH) $(SRC_PATH)
endif

#Regra para debug
print-% : ; @echo $* = $($*)

help:
ifeq ($(OS), Windows_NT)
	@echo.
endif
	@echo Available targets:
	@echo - release:	Builds the release version
	@echo - debug:		Builds the debug version
	@echo - clean: 		Cleans the generated files
	@echo - folders:	Generates project directories
	@echo - help:			Shows this help
ifeq ($(OS), Windows_NT)
	@echo.
endif

.SECONDEXPANSION:
-include $$(DEP_FILES)
