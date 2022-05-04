#include "CHIP8.h"

CHIP8::CHIP8(bool dbg) : gen(rd()), rnd(std::uniform_int_distribution<>(0, 0xFF))
{
    debug = dbg;
    gen.seed(time(NULL));

    display = std::make_unique<Display>();
    keypad = std::make_unique<Keypad>();
    PC = ROM_START;
    SP = -1;
    std::fill(RAM, RAM + RAM_SIZE, 0);
}

void CHIP8::load_ROM(char const *filename)
{
    std::fstream rom;
    rom.open(filename, std::ios::binary | std::ios::in);
    if (!rom.is_open())
    {
        std::cout << "cannot open file\n";
        exit(1);
    }
    // get length of rom
    rom.seekg(0, std::ios::end);
    ulong length = rom.tellg();
    rom.seekg(0, std::ios::beg);
    // read file into buffer
    auto buf = std::make_unique<char[]>(length);
    rom.read(buf.get(), length);
    // set ram from buffer
    for (int i = 0; i < length; ++i)
    {
        RAM[ROM_START + i] = buf[i];
    }
    rom.close();
    if (rom.is_open())
    {
        std::cout << "failed to close file\n";
        exit(1);
    }

    // load fontset into memory
    for (int i = 0; i < FONTSET_SIZE; ++i)
    {
        RAM[FONTSET_START + i] = FONTSET[i];
    }
}

void CHIP8::print_RAM()
{
    for (const uint8_t &b : RAM)
    {
        std::cout << b;
    }
}

uint16_t CHIP8::fetch()
{
    uint16_t instruction = RAM[PC] << 8 | RAM[PC+1];
    PC += 2;
    update_timers();
    return instruction;
}

void CHIP8::update_timers()
{
    if (DTIME > 0)
    {
        uint64_t t = SDL_GetTicks();
        DTIME = DTIME - ((t - dtStart) / TPH);
        if (DTIME > 0xFF)
        {
            DTIME = 0;
        }
    }
    if (STIME > 0)
    {
        uint64_t t = SDL_GetTicks();
        STIME = STIME - ((t - stStart) / TPH);
        if (STIME > 0xFF)
        {
            STIME = 0;
        }
    }
}

void CHIP8::step() {
    if(keypad->handleEvents() == 0xFF) {
        clean_up();
        exit(1);
    }
    decode_and_execute(fetch());
    display->draw();
}

void CHIP8::clean_up() {
    display->destroy_window();
    SDL_Quit();
}
void CHIP8::decode_and_execute(uint16_t instruction)
{
    std::cout << "PC: " << std::hex << PC << " Instruction: " << std::hex << instruction << "\n";
    uint8_t first_nibble = (instruction >> 12) & 0xF; // XXXXoooooooooooo
    uint8_t second_nibble = (instruction >> 8) & 0xF; // ooooXXXXoooooooo
    uint8_t third_nibble = (instruction >> 4) & 0xF;  // ooooooooXXXXoooo
    uint8_t fourth_nibble = instruction & 0xF;
    uint8_t last_byte = (instruction & 0xFF);           // ooooooooXXXXXXXX
    uint16_t last_three_nibbles = instruction & 0x0FFF; // ooooXXXXXXXXXXXX
    switch (first_nibble)
    {
    case 0x0:
    {
        switch (last_three_nibbles)
        {
        case 0x0EE:
            RET();
            break;
        case 0x0E0:
            CLS();
            break;
        default:
        if(debug) {
        }
        }
        break;
    }
    case 0x1:
    {
        JP(last_three_nibbles);
        break;
    }
    case 0x2:
    {
        CALL(last_three_nibbles);
        break;
    }
    case 0x3:
    {
        SE(second_nibble, last_byte);
        break;
    }
    case 0x4:
    {
        SNE(second_nibble, last_byte);
        break;
    }
    case 0x5:
    {
        SER(second_nibble, third_nibble);
        break;
    }
    case 0x6:
    {
        LD(second_nibble, last_byte);
        break;
    }
    case 0x7:
    {
        ADD(second_nibble, last_byte);
        break;
    }
    case 0x8:
    {
        switch (fourth_nibble)
        {
        case 0x0:
        {
            LDR(second_nibble, third_nibble);
            break;
        }
        case 0x1:
        {
            OR(second_nibble, third_nibble);
            break;
        }
        case 0x2:
        {
            AND(second_nibble, third_nibble);
            break;
        }
        case 0x3:
        {
            XOR(second_nibble, third_nibble);
            break;
        }
        case 0x4:
        {
            ADDC(second_nibble, third_nibble);
            break;
        }
        case 0x5:
        {
            SUB(second_nibble, third_nibble);
            break;
        }
        case 0x6:
        {
            SHR(second_nibble, third_nibble);
            break;
        }
        case 0x7:
        {
            SUBN(second_nibble, third_nibble);
            break;
        }
        case 0xE:
        {
            SHL(second_nibble, third_nibble);
            break;
        }
        default:
        {
            std::cout << "reached 0x8___ default\n";
            exit(1);
        }
        }
        break;
    }
    case 0x9:
    {
        SNER(second_nibble, third_nibble);
        break;
    }
    case 0xA:
    {
        LDI(last_three_nibbles);
        break;
    }
    case 0xB:
    {
        JPP(last_three_nibbles);
        break;
    }
    case 0xC:
    {
        RND(second_nibble, last_byte);
        break;
    }
    case 0xD:
    {
        DRW(second_nibble, third_nibble, fourth_nibble);
        break;
    }
    case 0xE:
    {
        switch (last_byte)
        {
        case 0x9E:
        {
            SKP(second_nibble);
            break;
        }
        case 0xA1:
        {
            SKNP(second_nibble);
            break;
        }
        default:
        {
            std::cout << "0xE___ default\n";
            exit(1);
        }
        }
        break;
    }
    case 0xF:
    {
        switch (last_byte)
        {
        case 0x07:
        {
            LDT(second_nibble);
            break;
        }
        case 0x0A:
        {
            LDK(second_nibble);
            break;
        }
        case 0x15:
        {
            LDDT(second_nibble);
            break;
        }
        case 0x18:
        {
            LDST(second_nibble);
            break;
        }
        case 0x1E:
        {
            ADDI(second_nibble);
            break;
        }
        case 0x29:
        {
            LDF(second_nibble);
            break;
        }
        case 0x33:
        {
            LDB(second_nibble);
            break;
        }
        case 0x55:
        {
            RTM(second_nibble);
            break;
        }
        case 0x65:
        {
            MTR(second_nibble);
            break;
        }
        default:
        {
            std::cout << "0xF___ default\n";
            exit(1);
        }
        }
        break;
    }
    default:
    {
        std::cout << "empty instruction reached bottom\n";
        exit(1);
    }
    }
}

void CHIP8::CLS()
{
    for (int row = 0; row < ROWS; ++row)
    {
        for (int col = 0; col < COLS; ++col)
        {
            display->setPixel(row, col, false);
        }
    }
}

void CHIP8::JP(uint16_t address)
{
    PC = address;
}



void CHIP8::DRW(uint8_t x_reg, uint8_t y_reg, uint8_t n)
{
    uint8_t x = V[x_reg] & (COLS - 1);
    uint8_t y = V[y_reg] & (ROWS - 1);
    V[0xF] = 0;
    for (uint8_t i = 0; i < n; ++i)
    {
        uint8_t x = V[x_reg] & (COLS - 1);
        uint8_t data = RAM[IC + i];
        for (uint8_t j = 0; j < 8; ++j)
        {
            bool d = display->getPixel(y, x);
            if ((data >> 7 - j) & 0x1 == 1)
            {
                if (d)
                {
                    display->setPixel(y, x, false);
                    V[0xF] = 0x1;
                }
                else
                {
                    display->setPixel(y, x, true);
                }
            }
            x += 1;
            if (x >= COLS)
            {
                break;
            }
        }
        y += 1;
        if (y >= ROWS)
        {
            break;
        }
    }
    display->draw();
}

void CHIP8::RET()
{
    if (SP <= -1) {
        std::cout << "nothing to return to\n";
        exit(1);
    }
    std::cout << "SP:" << SP << " STACK[SP]: " << STACK[SP] << "\n";
    PC = STACK[SP];
    --SP;
    if (SP <= -2)
    {
        std::cout << "nothing to return to\n";
        exit(1);
    }
    std::cout << "new SP after ret: " << SP << " new PC: " << PC << "\n"; 
}

void CHIP8::CALL(uint16_t addr)
{
    std::cout << "SP before:" << SP << "\n";
    ++SP;
    std::cout << "SP after:" << SP << "\n";
    if (SP >= 16)
    {
        std::cout << "stack overflow\n";
        exit(1);
    }
    STACK[SP] = PC;
    std::cout << "STACK[SP] CALL: " << STACK[SP] << "\n";
    PC = addr;
}

void CHIP8::SE(uint8_t reg, uint8_t byte)
{
    PC = V[reg] == byte ? (PC + 2) : PC;
}

void CHIP8::SNE(uint8_t x_reg, uint8_t byte)
{
    PC = V[x_reg] != byte ? (PC + 2) : PC;
}

void CHIP8::SER(uint8_t x_reg, uint8_t y_reg)
{
    PC = (V[x_reg] == V[y_reg]) ? (PC + 2) : PC;
}

void CHIP8::SNER(uint8_t x_reg, uint8_t y_reg)
{
    PC = (V[x_reg] != V[y_reg]) ? (PC + 2) : PC;
}

void CHIP8::LD(uint8_t reg, uint8_t byte)
{
    V[reg] = byte;
}

void CHIP8::ADD(uint8_t reg, uint8_t byte)
{
    V[reg] += byte;
}

void CHIP8::LDR(uint8_t x_reg, uint8_t y_reg)
{
    V[x_reg] = V[y_reg];
}

void CHIP8::OR(uint8_t x_reg, uint8_t y_reg)
{
    V[x_reg] |= V[y_reg];
}

void CHIP8::AND(uint8_t x_reg, uint8_t y_reg)
{
    V[x_reg] &= V[y_reg];
}

void CHIP8::XOR(uint8_t x_reg, uint8_t y_reg)
{
    V[x_reg] ^= V[y_reg];
}

void CHIP8::ADDC(uint8_t x_reg, uint8_t y_reg)
{
    uint8_t sum = V[x_reg] + V[y_reg];
    V[0xF] = sum < V[x_reg] ? 0x01 : 0x00;
    V[x_reg] = sum;
}

void CHIP8::SUB(uint8_t x_reg, uint8_t y_reg)
{
    uint8_t diff = V[x_reg] - V[y_reg];
    V[0xF] = diff <= V[x_reg] ? 0x01 : 0x00;
    V[x_reg] = diff;
}

void CHIP8::SUBN(uint8_t x_reg, uint8_t y_reg)
{
    uint8_t diff = V[y_reg] - V[x_reg];
    V[0xF] = diff <= V[y_reg] ? 0x01 : 0x00;
    V[x_reg] = diff;
}

void CHIP8::SHR(uint8_t x_reg, uint8_t y_reg)
{
    if (copy_on_shift == true)
    {
        V[x_reg] = V[y_reg];
    }
    V[0xF] = 0x01 ? (V[x_reg] & 0x01) : 0x00;
    V[x_reg] = V[x_reg] >> 1;
}

void CHIP8::SHL(uint8_t x_reg, uint8_t y_reg)
{
    if (copy_on_shift == true)
    {
        V[x_reg] = V[y_reg];
    }
    V[0xF] = 0x01 ? (V[x_reg] & 0b10000000) : 0x00;
    V[x_reg] = V[x_reg] << 1;
}

void CHIP8::LDI(uint16_t addr)
{
    IC = addr;
}

void CHIP8::JPP(uint16_t addr)
{
    uint8_t jmp_val;
    if (jump_offset_quirk)
    {
        uint8_t reg = (addr >> 8) & 0xF;
        jmp_val = V[reg];
    }
    else
    {
        jmp_val = V[0x0];
    }
    PC = addr + jmp_val;
}

void CHIP8::RND(uint8_t reg, uint8_t byte)
{
    V[reg] = rnd(gen) & byte;
}

void CHIP8::SKP(uint8_t reg)
{
    if (keypad->getKey(V[reg]) == true)
    {
        ++PC;
    }
}

void CHIP8::SKNP(uint8_t reg)
{
    if (keypad->getKey(V[reg]) == false)
    {
        ++PC;
    }
}

void CHIP8::LDT(uint8_t reg)
{
    V[reg] = DTIME;
}

void CHIP8::LDDT(uint8_t reg)
{
    DTIME = V[reg];
}

void CHIP8::LDST(uint8_t reg)
{
    STIME = V[reg];
}

void CHIP8::ADDI(uint8_t reg)
{
    IC += V[reg];
    if (add_i_carry && IC > 0x0FFF)
    {
        V[0xF] = 0x01;
    }
}

void CHIP8::LDK(uint8_t reg)
{
    uint8_t key;
    while (true)
    {
        key = keypad->handleEvents();
        if(key == 0xFF) clean_up();
        if(key != 0xEE) break;
        update_timers();
    }
    std::cout << "break from loop\n";
    V[reg] = key;

}

void CHIP8::LDF(uint8_t reg)
{
    IC = RAM[FONTSET_START + 5 * V[reg]];
}

void CHIP8::LDB(uint8_t reg)
{
    uint8_t num = V[reg];
    RAM[IC + 2] = static_cast<uint8_t>(num % 10);
    num /= 10;
    RAM[IC + 1] = static_cast<uint8_t>(num % 10);
    num /= 10;
    RAM[IC] = num % 10;
}

void CHIP8::RTM(uint8_t reg)
{
    for (int i = 0x0; i <= reg; ++i)
    {
        uint16_t start = IC;
        RAM[start + i] = V[i];
        if (change_i_on_copy)
        {
            ++IC;
        }
    }
}

void CHIP8::MTR(uint8_t reg)
{
    for (int i = 0x0; i <= reg; ++i)
    {
        uint16_t start = IC;
        V[i] = RAM[start + i];
        if (change_i_on_copy)
        {
            ++IC;
        }
    }
}
