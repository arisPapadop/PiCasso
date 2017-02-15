#include "common_util.h"

int get_bit(int stream, int position) {
    return get_bits(stream, position, position);
}

uint32_t get_bits(uint32_t stream, uint8_t from, uint8_t to) {
    return (stream >> from) & ((1 << (to - from + 1)) - 1);
}

Shift_Struct shift_op2(State *state, uint32_t operand2) {
    Shift_Struct shift_struct;

    uint32_t reg = get_bits(operand2, 0, 3);
    uint32_t to_shift = (*state).registers.general_purpose[reg];

    uint32_t shift_amount;
    uint8_t  shift_type = get_bits(operand2, 5, 6);

    if (get_bit(operand2, 4) == 0) {
        // Shift by a constant amount
        shift_amount = get_bits(operand2, 7, 11);
    } else {
        // Shift specified by a register
        shift_amount =
                (*state).registers.general_purpose[get_bits(operand2, 8, 11)];
    }

    // Sets C flag
    if (shift_amount != 0) {
        shift_struct.C = get_bit(to_shift, shift_amount - 1);
    }

    switch (shift_type) {
        case LSL:
            // Logical left
            if (shift_amount != 0) {
                shift_struct.C = get_bit(to_shift,
                        BYTE_SIZE * sizeof(uint32_t) - shift_amount + 1);
            }
            shift_struct.shift = to_shift << shift_amount;
            break;
        case LSR:
            // Logical right
            shift_struct.shift = to_shift >> shift_amount;
            break;
        case ASR:
            // Arith right
            shift_struct.shift = arithmetic_right(to_shift, shift_amount);
            break;
        case ROR:
            // Rotate right
            shift_struct.shift = rotate_right(to_shift, shift_amount);
            break;
        default:
            // Nothing happens
            perror("Shift type not valid");
            shift_struct.shift = to_shift;
            break;
    }

    // Return a structure of both C and the shift: need to split it later
    return shift_struct;
}

uint32_t arithmetic_right(uint32_t to_shift, uint32_t shift_amount) {
  // Arithmetic right shift on 'x' is equivalent to >> when 'x' is signed
  int signed_to_shift = (int32_t) to_shift;
  return signed_to_shift >> shift_amount;
}

Shift_Struct shift_immed(uint32_t op2) {
    Shift_Struct shift_struct;

    uint32_t imm = get_bits(op2, 0, 7);
    uint32_t rotation = 2 * get_bits(op2, 8, 11);

    shift_struct.C = get_bit(imm, rotation - 1);
    shift_struct.shift = rotate_right(imm, rotation);
    return shift_struct;
}

uint32_t rotate_right(uint32_t to_rotate, uint32_t rotate_amount) {
    return (to_rotate >> rotate_amount) |
           (to_rotate << (sizeof(to_rotate) * BYTE_SIZE - rotate_amount));
}

uint32_t rotate_left(uint32_t to_rotate, uint32_t rotate_amount) {
    return (to_rotate << rotate_amount) |
           (to_rotate >> (sizeof(to_rotate) * BYTE_SIZE - rotate_amount));
}

Rotation calculate_rotation(uint32_t data) {
    Rotation rot;
    for (int i = 0; i < (sizeof(data) * BYTE_SIZE) / 2; i++) {
        uint32_t mask_end   = 3;
        uint32_t mask_front = 0xFFFFFF00;
        if ((mask_end & data) != 0 && ((mask_front & data) == 0)) {
            rot.result   = data;
            rot.rotation = i;
            return rot;
        }
        data = rotate_left(data, 2);
    }
    return rot;
}

bool fits_in_eight(uint32_t data) {
    return ((uint8_t)data == data);
}

void check_rotations(uint32_t data, uint16_t *destination) {
    if (fits_in_eight(data)) {
        *destination = (uint16_t)data;
    } else {
        Rotation rot  = calculate_rotation(data);
        *destination  = rot.rotation << 8 | rot.result;
    }
}
