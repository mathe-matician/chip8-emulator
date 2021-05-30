#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "chip8.h"
#include "SDLRect.h"

const unsigned char fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

const unsigned int g_keys[KEYSIZE] = {
    SDL_SCANCODE_0, SDL_SCANCODE_1, SDL_SCANCODE_2,
    SDL_SCANCODE_3, SDL_SCANCODE_4, SDL_SCANCODE_5,
    SDL_SCANCODE_6, SDL_SCANCODE_7, SDL_SCANCODE_8,
    SDL_SCANCODE_9, SDL_SCANCODE_A, SDL_SCANCODE_B,
    SDL_SCANCODE_C, SDL_SCANCODE_D, SDL_SCANCODE_E,
    SDL_SCANCODE_F
};

const unsigned int* g_keys_p = g_keys;

//Store fontset in memory locations 0x000 - 0x079
void init_font(CHIP8_t* a_chip8) {
    uint16_t memFont = 0x0000;
    for (int i = 0; i < 80; i++) {
        a_chip8->memory[memFont++] = fontset[i];
    }
}
uint16_t hash_code(uint16_t a_opcode) {
   return a_opcode % OPCODE_SIZE;
}

//automatic reference counter for freeing memory?
void insert(uint16_t a_opcode) {
    struct opCode* l_opcodeStruct = (struct opCode*)malloc(sizeof(struct opCode));
    l_opcodeStruct->opcode = a_opcode;
    uint16_t l_opcodeIndex = hash_code(a_opcode);
    while(opcode_hash[l_opcodeIndex] != NULL) {
        if (l_opcodeIndex++ > OPCODE_SIZE) {
            l_opcodeIndex = 0;
        }
        l_opcodeIndex % OPCODE_SIZE;
    }
    printf("Inserting: %#06x as %d\n", a_opcode, l_opcodeIndex);
    opcode_hash[l_opcodeIndex] = l_opcodeStruct;
}

void init_hash() {
    for (int i = 0; i < OPCODE_SIZE; i++) {
        insert(opcodesInit[i]);
    }
}

void Execute(uint16_t a_opcode) {

}

uint16_t Decode(uint16_t a_opcode) {
    printf("a_opcode = %#06x\n", a_opcode);
    uint16_t l_code = a_opcode & 0xF000;
    printf("l_code = %#06x\n", l_code);
    //figure out what opcode it is and error if not in hash table.
    uint16_t test = hash_code(l_code);
    printf("test = %#06x\n", test);
    return test
    /*struct opCode* l_temp = opcode_hash[test];
    printf("l_temp->opcode = %#06x\n", l_temp->opcode);
    uint16_t l_opcode = l_temp->opcode;
    printf("l_opcode = %#06x\n", l_opcode);
    if (!l_opcode) {
        ERROR_MSG;
        fprintf(stderr, "Error: Not a valid opcode.\n");
        return -1;
    }
    //TODO: Parse the opcode for particulars.
    return l_opcode;*/
}

void emulateCycleImp(CHIP8_t* a_chip8) {
    //fetch
    uint16_t l_msb = a_chip8->memory[pc] << 8;
    uint16_t l_lsb = a_chip8->memory[pc+1];
    //decode
    uint16_t l_opcode = l_msb | l_lsb;
    uint16_t l_decodedOpcode = Decode(l_opcode);
    printf("int v: %d\n", l_decodedOpcode);
    //execute
    a_chip8->opcode_execute[l_decodedOpcode](a_chip8);
}

void initSystemImp(CHIP8_t* a_chip8) {
    init_hash();
    memset(a_chip8->memory, 0, MEMORY_SIZE);
    sp = &a_chip8->memory[STACK_ADDR];
    pc = 0x200;
    a_chip8->delay_timer = 0;
    a_chip8->sound_timer = 0;
    init_font(a_chip8);

    a_chip8->memory[0x200] = 0xD1; //fetch test
    a_chip8->memory[0x201] = 0x25; //fetch test

    a_chip8->V[1] = 0x30; //drawing test
    a_chip8->V[2] = 0x30; //drawing test
    a_chip8->I = 0x0000; //drawing test
}

void loadProgramImp(CHIP8_t* a_chip8, char* a_program) {
    printf("Loading %s\n", a_program);
    uint16_t l_programStart = 0x0200;
    while (a_program++ && l_programStart != MEMORY_SIZE+1) {
        a_chip8->memory[l_programStart++] = *a_program;
    }
}

void setKeysImp(CHIP8_t* a_chip8) {
    const uint8_t *state;
    state = SDL_GetKeyboardState(NULL);
    
}