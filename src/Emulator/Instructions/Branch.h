#define SHIFT 2

int final_offset(int offset) {
	offset <<= SHIFT;
	offset = signExtend(offset);
	return offset;
}

void execute_branch(State *state, Branch *branch) {
	(*state).registers.PC += final_offset((*branch).offset);
}
