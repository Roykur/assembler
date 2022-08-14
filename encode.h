
#define equals(str1, str2) (strcmp(str1, str2) == 0)

int is_reserved_word(char *str);

int encode_op(char *line[], int i, int op_code);

int find_operand_type(char *operand);

int check_extra_info(char **line, int i, int op1_type, int op2_type, int op_code);

int search_dot(char* str);

char* copy_str(char *str, int i);

int get_num(char str[]);

int count_args(char *line[], int i);

