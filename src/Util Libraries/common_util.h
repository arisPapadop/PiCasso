#define byte_from_word(b, n) \
    (((0x000000FF <<(8 * (3 - n))) & b) >> (8 * (3 - n)))

int get_bit(int stream, int position);
uint32_t get_bits(uint32_t stream, uint8_t from, uint8_t to);
Shift_Struct shift_op2(State *state, uint32_t operand2);
uint32_t rotate_right(uint32_t to_rotate, uint32_t rotate_amount);
uint32_t rotate_left(uint32_t to_rotate, uint32_t rotate_amount);
uint32_t arithmetic_right(uint32_t to_shift, uint32_t shift_amount);
Shift_Struct shift_immed(uint32_t op2);
Rotation calculate_rotation(uint32_t data);
bool fits_in_eight(uint32_t data);
