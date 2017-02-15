#define LABEL_SEPARATOR   ':'
#define LABEL_CONSTANT    2

uint32_t number_build(char *str);
void print_binary(uint32_t input);
uint32_t number_build(char *str);
Token tokenize(char* str);
bool is_register(char *str);
int get_length(char *str);
bool is_hex(char *str);
uint32_t build_with_base(char *exp, uint16_t base);
uint8_t register_number_build(char *reg);
uint32_t get_operand(char *op, uint8_t *imm);
uint8_t get_shift_type(char *shift);
bool is_label(char *line);