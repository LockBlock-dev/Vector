COMPILER=g++
OPTIONS=-g -std=c++20 -pedantic -Wall -Wextra -Werror -Wshadow -Wconversion -Wunreachable-code
COMPILE=$(COMPILER) $(OPTIONS)
SRC=src
BUILD=build

$(BUILD)/main.out: $(BUILD) $(BUILD)/main.o $(BUILD)/Memory.o $(BUILD)/Processor.o $(BUILD)/Registers.o $(BUILD)/utils.o $(BUILD)/Rtype.o $(BUILD)/Itype.o $(BUILD)/Stype.o $(BUILD)/Btype.o $(BUILD)/Utype.o $(BUILD)/Jtype.o
	$(COMPILE) $(BUILD)/*.o -o $@

$(BUILD)/main.o: $(SRC)/main.cpp
	$(COMPILE) -c $< -o $@

$(BUILD)/Memory.o: $(SRC)/Memory.cpp
	$(COMPILE) -c $< -o $@

$(BUILD)/Processor.o: $(SRC)/Processor.cpp
	$(COMPILE) -c $< -o $@

$(BUILD)/Registers.o: $(SRC)/Registers.cpp
	$(COMPILE) -c $< -o $@

$(BUILD)/utils.o: $(SRC)/utils.cpp
	$(COMPILE) -c $< -o $@

$(BUILD)/Rtype.o: $(SRC)/instructions/Rtype.cpp
	$(COMPILE) -c $< -o $@

$(BUILD)/Itype.o: $(SRC)/instructions/Itype.cpp
	$(COMPILE) -c $< -o $@

$(BUILD)/Stype.o: $(SRC)/instructions/Stype.cpp
	$(COMPILE) -c $< -o $@

$(BUILD)/Btype.o: $(SRC)/instructions/Btype.cpp
	$(COMPILE) -c $< -o $@

$(BUILD)/Utype.o: $(SRC)/instructions/Utype.cpp
	$(COMPILE) -c $< -o $@

$(BUILD)/Jtype.o: $(SRC)/instructions/Jtype.cpp
	$(COMPILE) -c $< -o $@

build:
	mkdir -p $(BUILD)

clean:
	rm -f $(BUILD)/*
