#ifndef PRIVATE_DIKSAMC_H_INCLUDED
#define PRIVATE_DIKSAMC_H_INCLUDED

#include "DKC.h"
#include "DVM_code.h"

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

void ResetLex(void);

#define dkc_compile_error(line_number, id, arg)  DikSam::GetClassObject(g_iCurrentThreadIndex)->GetError()->CompileError(line_number, id, arg)

#define smaller(a, b) ((a) < (b) ? (a) : (b))
#define larger(a, b) ((a) > (b) ? (a) : (b))

#define LINE_BUF_SIZE       (1024)

typedef struct MEM_Storage_tag *MEM_Storage;

typedef enum
{
    INT_MESSAGE_ARGUMENT = 1,
    DOUBLE_MESSAGE_ARGUMENT,
    STRING_MESSAGE_ARGUMENT,
    CHARACTER_MESSAGE_ARGUMENT,
    POINTER_MESSAGE_ARGUMENT,
    MESSAGE_ARGUMENT_END
} MessageArgumentType;

typedef enum
{
    PARSE_ERR = 1,
    CHARACTER_INVALID_ERR,
    FUNCTION_MULTIPLE_DEFINE_ERR,
    BAD_MULTIBYTE_CHARACTER_ERR,
    UNEXPECTED_WIDE_STRING_IN_COMPILE_ERR,
    ARRAY_ELEMENT_CAN_NOT_BE_FINAL_ERR,
    COMPLEX_ASSIGNMENT_OPERATOR_TO_FINAL_ERR,
    PARAMETER_MULTIPLE_DEFINE_ERR,
    VARIABLE_MULTIPLE_DEFINE_ERR,
    IDENTIFIER_NOT_FOUND_ERR,
    FUNCTION_IDENTIFIER_ERR,
    DERIVE_TYPE_CAST_ERR,
    CAST_MISMATCH_ERR,
    MATH_TYPE_MISMATCH_ERR,
    COMPARE_TYPE_MISMATCH_ERR,
    LOGICAL_TYPE_MISMATCH_ERR,
    MINUS_TYPE_MISMATCH_ERR,
    LOGICAL_NOT_TYPE_MISMATCH_ERR,
    INC_DEC_TYPE_MISMATCH_ERR,
    FUNCTION_NOT_IDENTIFIER_ERR,
    FUNCTION_NOT_FOUND_ERR,
    ARGUMENT_COUNT_MISMATCH_ERR,
    NOT_LVALUE_ERR,
    LABEL_NOT_FOUND_ERR,
    ARRAY_LITERAL_EMPTY_ERR,
    INDEX_LEFT_OPERAND_NOT_ARRAY_ERR,
    INDEX_NOT_INT_ERR,
    ARRAY_SIZE_NOT_INT_ERR,
    DIVISION_BY_ZERO_IN_COMPILE_ERR,
    DIKSAM_ERROR_COUNT_PLUS_1
} DikSamError;

typedef struct Expression_tag   Expression;

typedef enum
{
    BOOLEAN_EXPRESSION = 1,
    INT_EXPRESSION,
    DOUBLE_EXPRESSION,
    STRING_EXPRESSION,
    IDENTIFIER_EXPRESSION,
    COMMA_EXPRESSION,
    ASSIGN_EXPRESSION,
    ADD_EXPRESSION,
    SUB_EXPRESSION,
    MUL_EXPRESSION,
    DIV_EXPRESSION,
    MOD_EXPRESSION,
    EQ_EXPRESSION,
    NE_EXPRESSION,
    GT_EXPRESSION,
    GE_EXPRESSION,
    LT_EXPRESSION,
    LE_EXPRESSION,
    LOGICAL_AND_EXPRESSION,
    LOGICAL_OR_EXPRESSION,
    MINUS_EXPRESSION,
    LOGICAL_NOT_EXPRESSION,
    FUNCTION_CALL_EXPRESSION,
    MEMBER_EXPRESSION,
    NULL_EXPRESSION,
    ARRAY_LITERAL_EXPRESSION,
    INDEX_EXPRESSION,
    INCREMENT_EXPRESSION,
    DECREMENT_EXPRESSION,
    CAST_EXPRESSION,
    ARRAY_CREATION_EXPRESSION,
    EXPRESSION_KIND_COUNT_PLUS_1
} ExpressionKind;

typedef struct ArgumentList_tag
{
    Expression              *expression;
    struct ArgumentList_tag *next;
} ArgumentList;

typedef struct TypeSpecifier_tag    TypeSpecifier;

typedef struct ParameterList_tag
{
    char                        *name;
    TypeSpecifier               *type;
    int                         line_number;
    struct ParameterList_tag    *next;
} ParameterList;

typedef enum
{
    FUNCTION_DERIVE,
    ARRAY_DERIVE
} DeriveTag;

typedef struct 
{
    ParameterList   *parameter_list;
} FunctionDerive;

typedef struct
{
    int dummy;
} ArrayDerive;

typedef struct TypeDerive_tag
{
    DeriveTag   tag;
    union
    {
        FunctionDerive  function_d;
        ArrayDerive     array_d;
    } u;
    struct TypeDerive_tag   *next;
} TypeDerive;

struct TypeSpecifier_tag
{
    DVM_BasicType   basic_type;
    TypeDerive      *derive;
};

typedef struct FunctionDefinition_tag   FunctionDefinition;

typedef struct 
{
    char            *name;
    TypeSpecifier   *type;
    Expression      *initializer;
    int             variable_index;
    DVM_Boolean     is_local;
} Declaration;

typedef struct DeclarationList_tag
{
    Declaration                 *declaration;
    struct DeclarationList_tag  *next;
} DeclarationList;

typedef struct 
{
    char        *name;
    DVM_Boolean is_function;
    union
    {
        FunctionDefinition  *function;
        Declaration         *declaration;
    } u;
} IdentifierExpression;

typedef struct Statement_tag    Statement;

typedef struct 
{
    Expression  *left;
    Statement   *right;
} CommaExpression;

typedef enum
{
    NORMAL_ASSIGN = 1,
    ADD_ASSIGN,
    SUB_ASSIGN,
    MUL_ASSIGN,
    DIV_ASSIGN,
    MOD_ASSIGN
} AssignmentOperator;

typedef struct 
{
    AssignmentOperator  op;
    Expression          *left;
    Expression          *operand;
} AssignExpression;

typedef struct 
{
    Expression  *left;
    Expression  *right;
} BinaryExpression;

typedef struct 
{
    Expression      *function;
    ArgumentList    *argument;
} FunctionCallExpression;

typedef struct ExpressionList_tag
{
    Expression                  *expression;
    struct ExpressionList_tag   *next;
} ExpressionList;

typedef struct
{
    Expression  *array;
    Expression  *index;
} IndexExpression;

typedef struct
{
    Expression  *expression;
    char        *member_name;
} MemberExpression;

typedef struct 
{
    Expression  *operand;
} IncrementOrDecrement;

typedef enum
{
    INT_TO_DOUBLE_CAST,
    DOUBLE_TO_INT_CAST,
    BOOLEAN_TO_STRING_CAST,
    INT_TO_STRING_CAST,
    DOUBLE_TO_STRING_CAST
} CastType;

typedef struct 
{
    CastType    type;
    Expression  *operand;
} CastExpression;

typedef struct ArrayDimension_tag
{
    Expression                  *expression;
    struct ArrayDimension_tag   *next;
} ArrayDimension;

typedef struct
{
    TypeSpecifier   *type;
    ArrayDimension  *dimension;
} ArrayCreation;

struct Expression_tag
{
    TypeSpecifier   *type;
    ExpressionKind  kind;
    int             line_number;
    union
    {
        DVM_Boolean             boolean_value;
        int                     int_value;
        double                  double_value;
        DVM_Char                *string_value;
        IdentifierExpression    identifier;
        CommaExpression         comma;
        AssignExpression        assign_expression;
        BinaryExpression        binary_expression;
        Expression              *minus_expression;
        Expression              *logical_not;
        FunctionCallExpression  function_call_expression;
        MemberExpression        member_expression;
        ExpressionList          *array_literal;
        IndexExpression         index_expression;
        IncrementOrDecrement    inc_dec;
        CastExpression          cast;
        ArrayCreation           array_creation;
    } u;
};

typedef struct StatementList_tag
{
    Statement                   *statement;
    struct StatementList_tag    *next;
} StatementList;

typedef enum
{
    UNDEFINED_BLOCK = 1,
    FUNCTION_BLOCK,
    WHILE_STATEMENT_BLOCK,
    FOR_STATEMENT_BLOCK
} BlockType;

typedef struct 
{
    Statement   *statement;
    int         continue_label;
    int         break_label;
} StatementBlockInfo;

typedef struct 
{
    FunctionDefinition  *function;
    int                 end_label;
} FunctionBlockInfo;

typedef struct Block_tag
{
    BlockType           type;
    struct Block_tag    *outer_block;
    StatementList       *statement_list;
    DeclarationList     *declaration_list;
    union
    {
        StatementBlockInfo  statement;
        FunctionBlockInfo   function;
    } parent;
} Block;

typedef struct Elsif_tag
{
    Expression          *condition;
    Block               *block;
    struct Elsif_tag    *next;
} Elsif;

typedef struct 
{
    Expression  *condition;
    Block       *then_block;
    Elsif       *elsif_list;
    Block       *else_block;
} IfStatement;

typedef struct 
{
    char        *label;
    Expression  *condition;
    Block       *block;
} WhileStatement;

typedef struct
{
    char        *label;
    Expression  *init;
    Expression  *condition;
    Expression  *post;
    Block       *block;
} ForStatement;

typedef struct 
{
    char    *label;
} BreakStatement;

typedef struct 
{
    char    *label;
} ContinueStatement;

typedef struct 
{
    Expression  *return_value;
} ReturnStatement;

typedef struct 
{
    Block   *try_block;
    Block   *catch_block;
    char    *exception;
    Block   *finally_block;
} TryStatement;

typedef struct 
{
    Expression  *exception;
} ThrowStatement;

typedef enum
{
    EXPRESSION_STATEMENT = 1,
    IF_STATEMENT,
    WHILE_STATEMENT,
    FOR_STATEMENT,
    RETURN_STATEMENT,
    BREAK_STATEMENT,
    CONTINUE_STATEMENT,
    TRY_STATEMENT,
    THROW_STATEMENT,
    DECLARATION_STATEMENT,
    STATEMENT_TYPE_COUNT_PLUS_1
} StatementType;

struct Statement_tag
{
    StatementType   type;
    int             line_number;
    union
    {
        Expression          *expression_s;
        IfStatement         if_s;
        WhileStatement      while_s;
        ForStatement        for_s;
        BreakStatement      break_s;
        ContinueStatement   continue_s;
        ReturnStatement     return_s;
        TryStatement        try_s;
        ThrowStatement      throw_s;
        Declaration         *declaration_s;
    } u;
};

struct FunctionDefinition_tag
{
    TypeSpecifier   *type;
    char            *name;
    ParameterList   *parameter;
    Block           *block;
    int             local_variable_count;
    Declaration     **local_variable;
    int             index;
    int             end_line_number;
    struct FunctionDefinition_tag   *next;
};

typedef enum
{
    EUC_ENCODING = 1,
    GB2312_ENCODING,
    UTF_8_ENCODING
} Encoding;

struct DKC_Compiler_tag
{
    MEM_Storage         compile_storage;
    FunctionDefinition  *function_list;
    int                 function_count;
    DeclarationList     *declaration_list;
    StatementList       *statement_list;
    int                 current_line_number;
    Block               *current_block;
    DKC_InputMode       input_mode;
    Encoding            source_encoding;
};

typedef struct 
{
    DVM_Char            *string;
} VString;

// diksam.l
void dkc_set_source_string(char **source);

// Util.cpp
DKC_Compiler* dkc_get_current_compiler(void);

// Error.cpp
int yyerror(char const *str);

// StringLiteral.cpp
void dkc_open_string_literal(void);
void dkc_add_string_literal(int letter);
DVM_Char* dkc_close_string_literal(void);
char* dkc_create_identifier(char *str);

// CreateC.cpp
void dkc_function_define(TypeSpecifier *type, char *identifier, ParameterList *parameter_list, Block *block);
ParameterList* dkc_create_parameter(TypeSpecifier *type, char *identifier);
ParameterList* dkc_chain_parameter(ParameterList *list, TypeSpecifier *type, char *identifier);
ArgumentList* dkc_create_argument_list(Expression *expression);
ArgumentList* dkc_chain_argument_list(ArgumentList *list, Expression *expr);
ExpressionList* dkc_create_expression_list(Expression *expression);
ExpressionList* dkc_chain_expression_list(ExpressionList *list, Expression *expression);
StatementList* dkc_create_statement_list(Statement *statement);
StatementList* dkc_chain_statement_list(StatementList *list, Statement *statement);
TypeSpecifier* dkc_create_type_specifier(DVM_BasicType basic_type);
TypeSpecifier* dkc_create_array_type_specifier(TypeSpecifier *base);
Expression* dkc_alloc_expression(ExpressionKind kind);
Expression* dkc_create_comma_expression(Expression *left, Expression *right);
Expression* dkc_create_assign_expression(Expression *left, AssignmentOperator op, Expression *operand);
Expression* dkc_create_binary_expression(ExpressionKind op, Expression *left, Expression *right);
Expression* dkc_create_munis_expression(Expression *operand);
Expression* dkc_create_logical_not_expression(Expression *operand);
Expression* dkc_create_incdec_expression(Expression *operand, ExpressionKind inc_or_dec);
Expression* dkc_create_identifier_expression(char *identifier);
Expression* dkc_create_function_call_expression(Expression *function, ArgumentList *argument);
Expression* dkc_create_boolean_expression(DVM_Boolean value);
Expression* dkc_create_null_expression(void);
Expression* dkc_create_array_literal_expression(ExpressionList *list);
Expression* dkc_create_array_creation(DVM_BasicType basic_type, ArrayDimension *dim_expr_list, ArrayDimension *dim_list);
ArrayDimension* dkc_create_array_dimension(Expression *expr);
ArrayDimension* dkc_chain_array_dimension(ArrayDimension *list, ArrayDimension *dim);
Statement* dkc_create_if_statement(Expression *condition, Block *then_block, Elsif *elsif_list, Block *else_block);
Elsif* dkc_chain_elsif_list(Elsif *list, Elsif *add);
Elsif* dkc_create_elsif(Expression *expr, Block *block);
Statement* dkc_create_while_statement(char *label, Expression *condition, Block *block);
Statement* dkc_create_for_statement(char *label, Expression *init, Expression *cond, Expression *post, Block *block);
Block* dkc_open_block(void);
Block* dkc_close_block(Block *block, StatementList *statement_list);
Statement* dkc_create_expression_statement(Expression *expression);
Statement* dkc_create_return_statement(Expression *expression);
Statement* dkc_create_break_statement(char *label);
Statement* dkc_create_continue_statement(char *label);
Statement* dkc_create_try_statement(Block *try_block, char *exception, Block *catch_block, Block *finally_block);
Statement* dkc_create_throw_statement(Expression *expression);
Statement* dkc_create_declaration_statement(TypeSpecifier *type, char *identifier, Expression *initializer);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif  /* PRIVATE_DIKSAMC_H_INCLUDED */
