#ifndef PRIVATE_DIKSAMC_H_INCLUDED
#define PRIVATE_DIKSAMC_H_INCLUDED

#include "DVM_code.h"

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

void ResetLex(void);

#define dkc_compile_error(line_number, id, arg)  DikSam::GetClassObject(g_iCurrentThreadIndex)->GetError()->CompileError(line_number, id, arg)

#define DEFAULT_CONSTRUCTOR_NAME    ("initialize")

#define smaller(a, b) ((a) < (b) ? (a) : (b))
#define larger(a, b) ((a) > (b) ? (a) : (b))

#define LINE_BUF_SIZE               (1024)

#define ABSTRACT_METHOD_INDEX       (-1)

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
    DIVISION_BY_ZERO_ERR,
    LOCAL_VARIABLE_IN_TOP_ERR,
    INDEX_OUT_OF_BOUNDS_ERR,
    NULL_POINTER_ERR,
    PACKAGE_NAME_TOO_LONG_ERR,
    REQUIRE_FILE_NOT_FOUND_ERR,
    REQUIRE_FILE_ERR,
    REQUIRE_DUPLICATE_ERR,
    REQUIRE_ITSELF_ERR,
    RENAME_HAS_NO_PACKAGED_NAME_ERR,
    ABSTRACT_MULTIPILE_SPECIFIED_ERR,
    ACCESS_MODIFIER_MULTIPLE_SPECIFIED_ERR,
    OVERRIDE_MODIFIER_MULTIPLE_SPECIFIED_ERR,
    VIRTUAL_MODIFIER_MULTIPLE_SPECIFIED_ERR,
    MEMBER_EXPRESSION_TYPE_ERR,
    MEMBER_NOT_FOUND_ERR,
    PRIVATE_MEMBER_ACCESS_ERR,
    ABSTRACT_METHOD_HAS_BODY_ERR,
    CONCRETE_METHOD_HAS_NO_BODY_ERR,
    MULTIPLE_INHERITANCE_ERR,
    INHERIT_CONCRETE_CLASS_ERR,
    NEW_ABSTRACT_CLASS_ERR,
    RETURN_IN_VOID_FUNCTION_ERR,
    CLASS_NOT_FOUND_ERR,
    CONSTRUCTOR_IS_NOT_METHOD_ERR,
    NOT_CONSTRUCTOR_ERR,
    FIELD_CAN_NOT_CALL_ERR,
    METHOD_CAN_NOT_CALL_ERR,
    ASSIGN_TO_METHOD_ERR,
    NON_VIRTUAL_METHOD_OVERRIDED_ERR,
    NEED_OVERRIDE_ERR,
    ABSTRACT_METHOD_IN_CONCRETE_CLASS_ERR,
    HASNT_SUPER_CLASS_ERR,
    SUPER_NOT_IN_MEMBER_EXPRESSION_ERR,
    FIELD_OF_SUPER_REFERENCED_ERR,
    FIELD_OVERRIDED_ERR,
    FIELD_NAME_DUPLICATE_ERR,
    ARRAY_METHOD_NOT_FOUND_ERR,
    STRING_METHOD_NOT_FOUND_ERR,
    INSTANCEOF_OPERAND_NOT_REFERENCE_ERR,
    INSTANCEOF_TYPE_NOT_REFERENCE_ERR,
    INSTANCEOF_FOR_NOT_CLASS_ERR,
    INSTANCEOF_MUST_RETURN_TRUE_ERR,
    INSTANCEOF_MUST_RETURN_FALSE_ERR,
    INSTANCEOF_INTERFACE_ERR,
    DOWN_CAST_OPERAND_IS_NOT_CLASS_ERR,
    DOWN_CAST_TARGET_IS_NOT_CLASS_ERR,
    DOWN_CAST_DO_NOTHING_ERR,
    DOWN_CAST_TO_SUPER_CLASS_ERR,
    DOWN_CAST_TO_BAD_CLASS_ERR,
    DOWN_CAST_INTERFACE_ERR,
    IF_CONDITION_NOT_BOOLEAN_ERR,
    WHILE_CONDITION_NOT_BOOLEAN_ERR,
    FOR_CONDITION_NOT_BOOLEAN_ERR,
    DO_WHILE_CONDITION_NOT_BOOLEAN_ERR,
    BAD_PARAMETER_COUNT_ERR,
    BAD_PARAMETER_TYPE_ERR,
    OVERRIDE_METHOD_ACCESSIBILITY_ERR,
    BAD_RETURN_TYPE_ERR,
    CONSTRUCTOR_CALLED_ERR,
    TYPE_NAME_NOT_FOUND_ERR,
    INTERFACE_INHERIT_ERR,
    PACKAGE_MEMBER_ACCESS_ERR,
    PACKAGE_CLASS_ACCESS_ERR,
    THIS_OUT_OF_CLASS_ERR,
    SUPER_OUT_OF_CLASS_ERR,
    EOF_IN_C_COMMENT_ERR,
    EOF_IN_STRING_LITERAL_ERR,
    TOO_LONG_CHARACTER_LITERAL_ERR,
    LOAD_FILE_NOT_FOUND_ERR,
    LOAD_FILE_ERR,
    CLASS_MULTIPLE_DEFINE_ERR,
    CLASS_CAST_ERR,
    DYNAMIC_LOAD_WITHOUT_PACKAGE_ERR,
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
    THIS_EXPRESSION,
    SUPER_EXPRESSION,
    ARRAY_LITERAL_EXPRESSION,
    INDEX_EXPRESSION,
    INCREMENT_EXPRESSION,
    DECREMENT_EXPRESSION,
    INSTANCEOF_EXPRESSION,
    DOWN_CAST_EXPRESSION,
    CAST_EXPRESSION,
    UP_CAST_EXPRESSION,
    NEW_EXPRESSION,
    ARRAY_CREATION_EXPRESSION,
    EXPRESSION_KIND_COUNT_PLUS_1
} ExpressionKind;

// 包的定义
typedef struct PackageName_tag
{
    char                    *name;  // 包名称
    struct PackageName_tag  *next;  // 下一个包
} PackageName;

// 依赖包列表
typedef struct RequireList_tag
{
    PackageName             *package_name;  // 包名称
    int                      line_number;   // 行号
    struct RequireList_tag  *next;          // 下一个依赖包
} RequireList;

// 包重命名列表
typedef struct RenameList_tag
{
    PackageName             *package_name;      // 真实的包名称
    char                    *original_name;     // 原始的包名称
    char                    *renamed_name;      // 重命名后的包名称
    int                      line_number;       // 行号
    struct RenameList_tag   *next;              // 下一个重命名对象
} RenameList;

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
    int                          line_number;
    struct ParameterList_tag    *next;
} ParameterList;

typedef enum
{
    FUNCTION_DERIVE,
    ARRAY_DERIVE
} DeriveTag;

typedef struct ClassDefinition_tag ClassDefinition;

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
    DVM_BasicType    basic_type;
    struct
    {
        char            *identifier;
        ClassDefinition *class_definition;
        int              class_index;
    } class_ref;
    int              line_number;
    TypeDerive      *derive;
};

typedef struct FunctionDefinition_tag   FunctionDefinition;

// 声明
typedef struct 
{
    char            *name;              // 名字
    TypeSpecifier   *type;              // 类型
    DVM_Boolean      is_final;          // 是否为final变量或者函数
    Expression      *initializer;       // 初始化的表达式
    int              variable_index;    // 变量的索引
    DVM_Boolean      is_local;          // 是否为局部的变量/函数
} Declaration;

typedef struct DeclarationList_tag
{
    Declaration                 *declaration;
    struct DeclarationList_tag  *next;
} DeclarationList;

typedef struct
{
    FunctionDefinition  *function_definition;
    int                  function_index;
} FunctionIdentifier;

typedef enum
{
    VARIABLE_IDENTIFIER,
    FUNCTION_IDENTIFIER
} IdentifierKind;

typedef struct 
{
    char            *name;
    IdentifierKind   kind;
    union
    {
        FunctionIdentifier   function;
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
    AssignmentOperator   op;
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

typedef struct MemberDeclaration_tag MemberDeclaration;

typedef struct
{
    Expression          *expression;
    char                *member_name;
    MemberDeclaration   *declaration;
    int                  method_index;  /* use for only array and string */
} MemberExpression;

typedef struct 
{
    Expression  *operand;
} IncrementOrDecrement;

typedef struct
{
    Expression          *operand;
    TypeSpecifier       *type;
} InstanceofExpression;

typedef struct
{
    Expression          *operand;
    TypeSpecifier       *type;
} DownCastExpression;

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
    CastType     type;
    Expression  *operand;
} CastExpression;

typedef struct
{
    ClassDefinition     *interface_definition;
    Expression          *operand;
    int                  interface_index;
} UpCastExpression;

typedef struct
{
    char                *class_name;
    ClassDefinition     *class_definition;
    int                  class_index;
    char                *method_name;
    MemberDeclaration   *method_declaration;
    ArgumentList        *argument;
} NewExpression;

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
    ExpressionKind   kind;
    int              line_number;
    union
    {
        DVM_Boolean              boolean_value;
        int                      int_value;
        double                   double_value;
        DVM_Char                *string_value;
        IdentifierExpression     identifier;
        CommaExpression          comma;
        AssignExpression         assign_expression;
        BinaryExpression         binary_expression;
        Expression              *minus_expression;
        Expression              *logical_not;
        FunctionCallExpression   function_call_expression;
        MemberExpression         member_expression;
        ExpressionList          *array_literal;
        IndexExpression          index_expression;
        IncrementOrDecrement     inc_dec;
        InstanceofExpression     instanceof;
        DownCastExpression       down_cast;
        CastExpression           cast;
        UpCastExpression         up_cast;
        NewExpression            new_e;
        ArrayCreation            array_creation;
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
    FOR_STATEMENT_BLOCK,
    DO_WHILE_STATEMENT_BLOCK,
    TRY_CLAUSE_BLOCK,
    CATCH_CLAUSE_BLOCK,
    FINALLY_CLAUSE_BLOCK
} BlockType;

typedef struct 
{
    Statement   *statement;
    int          continue_label;
    int          break_label;
} StatementBlockInfo;

typedef struct 
{
    FunctionDefinition  *function;
    int                  end_label;
} FunctionBlockInfo;

// 语句块
typedef struct Block_tag
{
    BlockType            type;              // 语句块的类型
    struct Block_tag    *outer_block;       // 外部块
    StatementList       *statement_list;    // 语句列表
    DeclarationList     *declaration_list;  // 声明列表
    union                                   // 父语句块
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
    char        *label;
    Block       *block;
    Expression  *condition;
} DoWhileStatement;

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

// catch从句
typedef struct CatchClause_tag
{
    TypeSpecifier           *type;                  // 类型
    char                    *variable_name;         // 变量名
    Declaration             *variable_declaration;  // 变量的定义
    Block                   *block;                 // catch语句块
    int                      line_number;           // 行号
    struct CatchClause_tag  *next;                  // 下一个catch从句
} CatchClause;

// try语句
typedef struct 
{
    Block           *try_block;         // try的语句块
    CatchClause     *catch_clause;      // catch从句
    Block           *finally_block;     // finally语句块
} TryStatement;

typedef struct 
{
    Expression  *exception;
    Declaration *variable_declaration;
} ThrowStatement;

typedef enum
{
    EXPRESSION_STATEMENT = 1,
    IF_STATEMENT,
    WHILE_STATEMENT,
    FOR_STATEMENT,
    DO_WHILE_STATEMENT,
    RETURN_STATEMENT,
    BREAK_STATEMENT,
    CONTINUE_STATEMENT,
    TRY_STATEMENT,
    THROW_STATEMENT,
    DECLARATION_STATEMENT,
    STATEMENT_TYPE_COUNT_PLUS_1
} StatementType;

// 语句
struct Statement_tag
{
    StatementType   type;                   // 语句类型
    int             line_number;            // 行号
    union
    {
        Expression          *expression_s;  // 一般的表达式
        IfStatement          if_s;          // if语句
        WhileStatement       while_s;       // switch语句
        ForStatement         for_s;         // for语句
        DoWhileStatement     do_while_s;    // do_while语句
        BreakStatement       break_s;       // break语句
        ContinueStatement    continue_s;    // continue语句
        ReturnStatement      return_s;      // return语句
        TryStatement         try_s;         // try语句
        ThrowStatement       throw_s;       // throw语句
        Declaration         *declaration_s; // 普通的声明
    } u;
};

// 函数的定义
struct FunctionDefinition_tag
{
    TypeSpecifier   *type;                  // 函数的返回值类型
    PackageName     *package_name;          // 函数所属的包名称
    char            *name;                  // 函数的名字
    ParameterList   *parameter;             // 形式参数列表
    Block           *block;                 // 函数体
    int              local_variable_count;  // 局部变量的数量
    Declaration    **local_variable;        // 局部变量
    ClassDefinition *class_definition;      // 函数所属的类定义
                                            // 像Java一样可以声明衣蛾throw来抛出异常 throws
    int              index;
    int              end_line_number;       // 函数结束的行号
    struct FunctionDefinition_tag   *next;  // 下一个函数定义
};

typedef enum
{
    EUC_ENCODING = 1,
    GB2312_ENCODING,
    UTF_8_ENCODING
} Encoding;

typedef enum
{
    ABSTRACT_MODIFIER,
    PUBLIC_MODIFIER,
    PRIVATE_MODIFIER,
    OVERRIDE_MODIFIER,
    VIRTUAL_MODIFIER,
    NOT_SPECIFIED_MODIFIER
} ClassOrMemberModifierKind;

typedef struct
{
    ClassOrMemberModifierKind   is_abstract;
    ClassOrMemberModifierKind   access_modifier;
    ClassOrMemberModifierKind   is_virtual;
    ClassOrMemberModifierKind   is_override;
} ClassOrMemberModifierList;

typedef struct ExtendsList_tag
{
    char                    *identifier;
    ClassDefinition         *class_definition;
    struct ExtendsList_tag  *next;
} ExtendsList;

typedef enum
{
    METHOD_MEMBER,
    FIELD_MEMBER
} MemberKind;

typedef struct
{
    DVM_Boolean          is_constructor;
    DVM_Boolean          is_abstract;
    DVM_Boolean          is_virtual;
    DVM_Boolean          is_override;
    FunctionDefinition *function_definition;
    int                  method_index;
} MethodMember;

typedef struct
{
    char            *name;
    TypeSpecifier   *type;
    int              field_index;
} FieldMember;

struct MemberDeclaration_tag
{
    MemberKind          kind;
    DVM_AccessModifier  access_modifier;
    union
    {
        MethodMember    method;
        FieldMember     field;
    } u;
    int                 line_number;
    struct MemberDeclaration_tag    *next;
};

// 类的定义
struct ClassDefinition_tag
{
    DVM_Boolean                  is_abstract;           // 是否为抽象类
    DVM_AccessModifier           access_modifier;       // 访问修饰符
    DVM_ClassOrInterface         class_or_interface;    // 类还是接口
    PackageName                 *package_name;          // 包名称
    char                        *name;                  // 类名称
    ExtendsList                 *extends;               // 继承列表
    ClassDefinition             *super_class;           // 父类
    ExtendsList                 *interface_list;        // 继承的接口列表
    MemberDeclaration           *member;                // 成员的声明
    int                          line_number;           // 行号
    struct ClassDefinition_tag  *next;                  // 下一个类的定义
};

typedef struct DKC_Compiler_tag DKC_Compiler;

typedef struct CompilerList_tag
{
    DKC_Compiler            *compiler;
    struct CompilerList_tag *next;
} CompilerList;

// 源文件的后缀名
typedef enum
{
    DKH_SOURCE,     // 头文件
    DKM_SOURCE      // 实现文件
} SourceSuffix;

typedef enum
{
    FILE_INPUT_MODE = 1,
    STRING_INPUT_MODE
} InputMode;

typedef struct
{
    InputMode   input_mode;
    union
    {
        struct
        {
            FILE        *fp;
        } file;

        struct
        {
            char       **lines;
        } string;
    } u;
} SourceInput;

struct DKC_Compiler_tag
{
    MEM_Storage          compile_storage;           // compiler用的存储器
    PackageName         *package_name;              // 包名称
    SourceSuffix         source_suffix;             // 源文件的后缀名
    char                *path;                      // 源代码的路径
    RequireList         *require_list;              // 依赖包列表
    RenameList          *rename_list;               // 重命名列表
    FunctionDefinition  *function_list;             // 函数定义列表
    int                  dvm_function_count;        // 虚拟机的函数的数量
    DVM_Function        *dvm_function;              // 虚拟机的函数
                                                    // 虚拟机枚举的数量 dvm_enum_count
                                                    // 虚拟机的枚举 dvm_enum
                                                    // 虚拟机常量的数量 dvm_constant_count
                                                    // 虚拟机的常量 dvm_constant
    int                  dvm_class_count;           // 虚拟机类的数量
    DVM_Class           *dvm_class;                 // 虚拟机的类
    DeclarationList     *declaration_list;          // 声明列表
    StatementList       *statement_list;            // 语句列表
    ClassDefinition     *class_definition_list;     // 类的定义列表
                                                    // 委托定义列表 delegate_definition_list
                                                    // 枚举的定义列表 enum_definition_list
                                                    // 常量定义 constant_definition_list
    int                  current_line_number;       // 当前行数
    Block               *current_block;             // 当前的语句块
    ClassDefinition     *current_class_definition;  // 当前的类定义
    TryStatement        *current_try_statement;     // 当前的try语句
    CatchClause         *current_catch_clause;      // 当前的catch语句
    int                  current_finally_label;     // 当前finally语句的标签
    InputMode            input_mode;                // 源代码的输入方式
    CompilerList        *required_list;             // 依赖包对应的编译器（每一个依赖包都对应一个编译器）
    int                  array_method_count;        // 内置方法-数组 的数量
    FunctionDefinition  *array_method;              // 内置方法-数组
    int                  string_method_count;       // 内置方法-字符串 的数量
    FunctionDefinition  *string_method;             // 内置方法-字符串
    Encoding             source_encoding;           // 源代码文件的编码方式
};

typedef struct 
{
    DVM_Char            *string;
} VString;

typedef struct
{
    char             *package_name;
    SourceSuffix      suffix;
    char            **source_string;
} BuiltinScript;

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
int dkc_close_character_literal(void);
char* dkc_create_identifier(char *str);

// CreateC.cpp
PackageName* dkc_create_package_name(char *identifier);
PackageName* dkc_chain_package_name(PackageName *list, char *identifier);
RequireList* dkc_create_require_list(PackageName *package_name);
RequireList* dkc_chain_require_list(RequireList *list, RequireList *add);
RenameList* dkc_create_rename_list(PackageName *package_name, char *identifier);
RenameList* dkc_chain_rename_list(RenameList *list, RenameList *add);
void dkc_set_require_and_rename_list(RequireList *require_list, RenameList *rename_list);
FunctionDefinition* dkc_create_function_definition(TypeSpecifier *type, char *identifier, ParameterList *parameter_list, Block *block);
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
TypeSpecifier* dkc_create_class_type_specifier(char *identifier);
TypeSpecifier* dkc_create_array_type_specifier(TypeSpecifier *base);
Expression* dkc_alloc_expression(ExpressionKind kind);
Expression* dkc_create_comma_expression(Expression *left, Expression *right);
Expression* dkc_create_assign_expression(Expression *left, AssignmentOperator op, Expression *operand);
Expression* dkc_create_binary_expression(ExpressionKind op, Expression *left, Expression *right);
Expression* dkc_create_munis_expression(Expression *operand);
Expression* dkc_create_logical_not_expression(Expression *operand);
Expression* dkc_create_incdec_expression(Expression *operand, ExpressionKind inc_or_dec);
Expression* dkc_create_instanceof_expression(Expression *operand, TypeSpecifier *type);
Expression* dkc_create_identifier_expression(char *identifier);
Expression* dkc_create_function_call_expression(Expression *function, ArgumentList *argument);
Expression* dkc_create_down_cast_expression(Expression *operand, TypeSpecifier *type);
Expression* dkc_create_member_expression(Expression *expression, char *member_name);
Expression* dkc_create_boolean_expression(DVM_Boolean value);
Expression* dkc_create_null_expression(void);
Expression* dkc_create_new_expression(char *class_name, char *method_name, ArgumentList *argument);
Expression* dkc_create_array_literal_expression(ExpressionList *list);
Expression* dkc_create_basic_array_creation(DVM_BasicType basic_type, ArrayDimension *dim_expr_list, ArrayDimension *dim_list);
Expression* dkc_create_class_array_creation(TypeSpecifier *type, ArrayDimension *dim_expr_list, ArrayDimension *dim_list);
Expression* dkc_create_this_expression(void);
Expression* dkc_create_super_expression(void);
ArrayDimension* dkc_create_array_dimension(Expression *expr);
ArrayDimension* dkc_chain_array_dimension(ArrayDimension *list, ArrayDimension *dim);
Statement* dkc_alloc_statement(StatementType type);
Statement* dkc_create_if_statement(Expression *condition, Block *then_block, Elsif *elsif_list, Block *else_block);
Elsif* dkc_chain_elsif_list(Elsif *list, Elsif *add);
Elsif* dkc_create_elsif(Expression *expr, Block *block);
Statement* dkc_create_while_statement(char *label, Expression *condition, Block *block);
Statement* dkc_create_for_statement(char *label, Expression *init, Expression *cond, Expression *post, Block *block);
Statement* dkc_create_do_while_statement(char *label, Block *block, Expression *condition);
Block *dkc_alloc_block();
Block* dkc_open_block(void);
Block* dkc_close_block(Block *block, StatementList *statement_list);
Statement* dkc_create_expression_statement(Expression *expression);
Statement* dkc_create_return_statement(Expression *expression);
Statement* dkc_create_break_statement(char *label);
Statement* dkc_create_continue_statement(char *label);
Statement* dkc_create_try_statement(Block *try_block, CatchClause *catch_clause, Block *finally_block);
CatchClause* dkc_create_catch_clause(TypeSpecifier *type, char *variable_name, Block *block);
CatchClause* dkc_start_catch_clause(void);
CatchClause* dkc_end_catch_clause(CatchClause *catch_clause, TypeSpecifier *type, char *variable_name, Block *block);
CatchClause* dkc_chain_catch_list(CatchClause *list, CatchClause *add);
Statement* dkc_create_throw_statement(Expression *expression);
Statement* dkc_create_declaration_statement(TypeSpecifier *type, char *identifier, Expression *initializer);
void dkc_start_class_definition(ClassOrMemberModifierList *modifier, DVM_ClassOrInterface class_or_interface, char *identifier, ExtendsList *extends);
void dkc_class_define(MemberDeclaration *member_list);
ExtendsList* dkc_create_extends_list(char *identifier);
ExtendsList* dkc_chain_extends_list(ExtendsList *list, char *add);
ClassOrMemberModifierList dkc_create_class_or_member_modifier(ClassOrMemberModifierKind modifier);
ClassOrMemberModifierList dkc_chain_class_or_member_modifier(ClassOrMemberModifierList list, ClassOrMemberModifierList add);
MemberDeclaration* dkc_chain_member_declaration(MemberDeclaration *list, MemberDeclaration *add);
MemberDeclaration* dkc_create_method_member(ClassOrMemberModifierList *modifier, FunctionDefinition *function_definition, DVM_Boolean is_constructor);
FunctionDefinition* dkc_method_function_define(TypeSpecifier *type, char *identifier, ParameterList *parameter_list, Block *block);
FunctionDefinition* dkc_constructor_function_define(char *identifier, ParameterList *parameter_list, Block *block);
MemberDeclaration* dkc_create_field_member(ClassOrMemberModifierList *modifier, TypeSpecifier *type, char *name);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif  /* PRIVATE_DIKSAMC_H_INCLUDED */
