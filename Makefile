# Compilador e flags de compilação
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g -I.

# Nome do executável final
EXEC = compilador_racket

# ===== Estrutura MVC =====

# Model: Estruturas de dados e autômatos
MODEL_SRCS = Model/AutomatonBase.cpp \
             Model/AFNDEpsilon.cpp \
             Model/AFND.cpp \
             Model/AFD.cpp \
             Model/MinimizedAFD.cpp \
             Model/ThompsonFactory.cpp \
             Model/Generator.cpp \
             Model/Regex.cpp \
             Model/Scanner.cpp

MODEL_HEADERS = Model/State.hpp \
                Model/AutomatonBase.hpp \
                Model/AutomatonFactory.hpp \
                Model/AFNDEpsilon.hpp \
                Model/AFND.hpp \
                Model/AFD.hpp \
                Model/MinimizedAFD.hpp \
                Model/ThompsonFactory.hpp \
                Model/Regex.hpp \
                Model/Scanner.hpp \
                Model/Token.h

# Controller: Orquestração de geração e parsing
CONTROLLER_SRCS = Controller/ScannerGenerator.cpp \
                  Controller/Parser.cpp \
                  Controller/SyntaxTree.cpp

CONTROLLER_HEADERS = Controller/ScannerGenerator.hpp \
                     Controller/Parser.hpp \
                     Controller/SyntaxTree.hpp

# View: Visualização de resultados (por enquanto vazio)
VIEW_SRCS = 

# Main
MAIN_SRCS = main.cpp

# Combinação de todos os sources
SRCS = $(MAIN_SRCS) $(MODEL_SRCS) $(CONTROLLER_SRCS) $(VIEW_SRCS)

# Transforma a lista de .cpp em uma lista de .o (arquivos objeto)
OBJS = $(SRCS:.cpp=.o)

# ===== Regras =====

# Regra principal
all: $(EXEC)

# Gera o executável linkando os objetos
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Regra genérica para compilar cada .cpp em um .o correspondente
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpa os arquivos compilados
clean:
	rm -f $(OBJS) $(EXEC)

# Lista de targets "phony" (não são arquivos reais)
.PHONY: all clean