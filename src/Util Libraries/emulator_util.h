
void print_registers(State *state);
uint32_t word_to_print_build(uint8_t *byte);
void store_word_in_memory(uint32_t data, uint8_t *byte);
uint32_t word_build(uint8_t *byte);
int signExtend(int number);
bool gpio_access(uint32_t mem_address);
bool memory_in_bounds(uint32_t mem_address);
size_t bytes_count(FILE *file);
void check_rotations(uint32_t data, uint16_t *destination);