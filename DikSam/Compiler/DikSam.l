%{
#undef  YY_INPUT
#define YY_INPUT(buf, result, max_size) (result = DikSam_yyinput(buf, max_size))

#include <stdio.h>
#include <io.h>
#include <string.h>

#include "DikSamc.h"
#include "y.tab.h"

static int file_input(char *buf, int max_size)
{
    int ch;
    int len;
    
    if (feof(yyin))
        return 0;
    
    for (len = 0; len < max_size; len++)
    {
        ch = getc(yyin);
        if (EOF == ch)
            break;
        buf[len] = ch;
    }
    
    return len;
}

static char **st_source_string;
static int st_current_source_line;
static int st_current_char_index;

static DVM_Boolean st_character_literal_state;

static const int error_message_buf_len = 100;
static char error_message_buf[error_message_buf_len];

void dkc_set_source_string(char **source)
{
    st_source_string = source;
    st_current_source_line = 0;
    st_current_char_index = 0;
}

static int string_input(char *buf, int max_size)
{
    int len;
    
    if (NULL == st_source_string[st_current_source_line])
        return 0;
        
    while ('\0' == st_source_string[st_current_source_line][st_current_char_index])
    {
        st_current_source_line++;
        st_current_char_index = 0;
        
        if (NULL == st_source_string[st_current_source_line])
        {
            return 0;
        }
    }
    
    if (NULL == st_source_string[st_current_source_line])
        return 0;
        
    len = smaller(int(strlen(st_source_string[st_current_source_line]))
                  - st_current_char_index,
                  max_size);
    strncpy_s(buf, max_size, &st_source_string[st_current_source_line][st_current_char_index],
            len);
            
    st_current_char_index += len;
    
    return len;
}

static int DikSam_yyinput(char *buf, int max_size)
{
    int result;
    
    switch (dkc_get_current_compiler()->input_mode)
    {
    case FILE_INPUT_MODE :
        result = file_input(buf, max_size);
        break;
        
    case STRING_INPUT_MODE :
        result = string_input(buf, max_size);
        break;
    
    /*default :
        DBG_panic(("bad default. input_mode..", dkc_get_current_compiler()->input_mode));*/
    }
    
    return result;
}

static void increment_line_number(void)
{
    dkc_get_current_compiler()->current_line_number++;
}

int yywrap(void)
{
    return 1;
}
%}

%start C_COMMENT CC_COMMENT STRING_LITERAL_STATE GB2312_2ND_CHAR

%%
<INITIAL>"if"           return IF;
<INITIAL>"else"         return ELSE;
<INITIAL>"while"        return WHILE;
<INITIAL>"do"           return DO_T;
<INITIAL>"for"          return FOR;
<INITIAL>"return"       return RETURN_T;
<INITIAL>"break"        return BREAK;
<INITIAL>"continue"     return CONTINUE;
<INITIAL>"null"         return NULL_T;
<INITIAL>"true"         return TRUE_T;
<INITIAL>"false"        return FALSE_T;
<INITIAL>"try"          return TRY;
<INITIAL>"catch"        return CATCH;
<INITIAL>"finally"      return FINALLY;
<INITIAL>"throw"        return THROW;
<INITIAL>"boolean"      return BOOLEAN_T;
<INITIAL>"void"         return VOID_T;
<INITIAL>"int"          return INT_T;
<INITIAL>"double"       return DOUBLE_T;
<INITIAL>"string"       return STRING_T;
<INITIAL>"new"          return NEW;
<INITIAL>"require"      return REQUIRE;
<INITIAL>"rename"       return RENAME;
<INITIAL>"class"        return CLASS_T;
<INITIAL>"interface"    return INTERFACE_T;
<INITIAL>"public"       return PUBLIC_T;
<INITIAL>"private"      return PRIVATE_T;
<INITIAL>"virtual"      return VIRTUAL_T;
<INITIAL>"override"     return OVERRIDE_T;
<INITIAL>"abstract"     return ABSTRACT_T;
<INITIAL>"this"         return THIS_T;
<INITIAL>"super"        return SUPER_T;
<INITIAL>"constructor"  return CONSTRUCTOR;
<INITIAL>"instanceof"   return INSTANCEOF;
<INITIAL>"("            return LEFTP;
<INITIAL>")"            return RIGHTP;
<INITIAL>"{"            return LC;
<INITIAL>"}"            return RC;
<INITIAL>"["            return LB;
<INITIAL>"]"            return RB;
<INITIAL>";"            return SEMICOLON;
<INITIAL>":"            return COLON;
<INITIAL>","            return COMMA;
<INITIAL>"&&"           return LOGICAL_AND;
<INITIAL>"||"           return LOGICAL_OR;
<INITIAL>"="            return ASSIGN_T;
<INITIAL>"=="           return EQ;
<INITIAL>"!="           return NE;
<INITIAL>">"            return GT;
<INITIAL>">="           return GE;
<INITIAL>"<"            return LT;
<INITIAL>"<="           return LE;
<INITIAL>"+"            return ADD;
<INITIAL>"-"            return SUB;
<INITIAL>"*"            return MUL;
<INITIAL>"/"            return DIV;
<INITIAL>"%"            return MOD;
<INITIAL>"+="           return ADD_ASSIGN_T;
<INITIAL>"-="           return SUB_ASSIGN_T;
<INITIAL>"*="           return MUL_ASSIGN_T;
<INITIAL>"/="           return DIV_ASSIGN_T;
<INITIAL>"%="           return MOD_ASSIGN_T;
<INITIAL>"++"           return INCREMENT;
<INITIAL>"--"           return DECREMENT;
<INITIAL>"!"            return EXCLAMATION;
<INITIAL>"."            return DOT;
<INITIAL>"::"           return DOWN_CAST_BEGIN;
<INITIAL>":>"           return DOWN_CAST_END;

<INITIAL>[A-Za-z_][A-Za-z_0-9]* {
    yylval.identifier = dkc_create_identifier(yytext);
    return IDENTIFIER;
}

<INITIAL>[1-9][0-9]* {
    Expression *expression = dkc_alloc_expression(INT_EXPRESSION);
    sscanf_s(yytext, "%d", &expression->u.int_value);
    yylval.expression = expression;
    return INT_LITERAL;
}

<INITIAL>"0"[xX][0-9a-fA-F]+ {
    Expression *expression = dkc_alloc_expression(INT_EXPRESSION);
    sscanf_s(yytext, "%x", &expression->u.int_value);
    yylval.expression = expression;
    return INT_LITERAL;
}

<INITIAL>"0" {
    Expression *expression = dkc_alloc_expression(INT_EXPRESSION);
    expression->u.int_value=0;
    yylval.expression = expression;
    return INT_LITERAL;
}

<INITIAL>[0-9]+\.[0-9]+ {
    Expression *expression = dkc_alloc_expression(DOUBLE_EXPRESSION);
    sscanf_s(yytext, "%lf", &expression->u.double_value);
    yylval.expression = expression;
    return DOUBLE_LITERAL;
}

<INITIAL>\" {
    dkc_open_string_literal();
    st_character_literal_state = DVM_FALSE;
    BEGIN STRING_LITERAL_STATE;
}

<INITIAL>\' {
    dkc_open_string_literal();
    st_character_literal_state = DVM_TRUE;
    BEGIN STRING_LITERAL_STATE;
}

<INITIAL>[\t]   ;
<INITIAL>[\r\n] { increment_line_number(); }
<INITIAL>"/*"      BEGIN C_COMMENT;
<INITIAL>"//"      BEGIN CC_COMMENT;
<INITIAL>. {
    memset(error_message_buf, 0, error_message_buf_len);
    sprintf_s(error_message_buf, 100, "incorrect character(%c).", &yytext[0]);
    dkc_compile_error(dkc_get_current_compiler()->current_line_number, error_message_buf);
}

<C_COMMENT>\n   increment_line_number();
<C_COMMENT>"*/" BEGIN INITIAL;
<C_COMMENT><<EOF>> {
    memset(error_message_buf, 0, error_message_buf_len);
    sprintf_s(error_message_buf, 100, "end of file in C style comment");
    dkc_compile_error(dkc_get_current_compiler()->current_line_number, error_message_buf);
}
<C_COMMENT>.    ;

<CC_COMMENT>\n {
    increment_line_number();
    BEGIN INITIAL;
}
<CC_COMMENT><<EOF>> BEGIN INITIAL;
<CC_COMMENT>.   ;

<STRING_LITERAL_STATE>\" {
    if (DVM_TRUE == st_character_literal_state) {
        dkc_add_string_literal('\"');
    } else {
        Expression *expression = dkc_alloc_expression(STRING_EXPRESSION);
        expression->u.string_value = dkc_close_string_literal();
        yylval.expression = expression;
        BEGIN INITIAL;
        return STRING_LITERAL;
    }
}

<STRING_LITERAL_STATE>\' {
    if (DVM_TRUE == st_character_literal_state) {
        Expression *expression = dkc_alloc_expression(INT_EXPRESSION);
        expression->u.int_value = dkc_close_character_literal();
        yylval.expression = expression;
        BEGIN INITIAL;
        return INT_LITERAL;
    } else {
        dkc_add_string_literal('\'');
    }
}

<STRING_LITERAL_STATE>\n {
    dkc_add_string_literal('\n');
    increment_line_number();
}

<STRING_LITERAL_STATE>\\\"  dkc_add_string_literal('"');
<STRING_LITERAL_STATE>\\r   dkc_add_string_literal('\r');
<STRING_LITERAL_STATE>\\n   dkc_add_string_literal('\n');
<STRING_LITERAL_STATE>\\t   dkc_add_string_literal('\t');
<STRING_LITERAL_STATE>\\\\  dkc_add_string_literal('\\');

<STRING_LITERAL_STATE>. {
    Encoding enc = dkc_get_current_compiler()->source_encoding;
    dkc_add_string_literal(yytext[0]);
    if (enc == GB2312_ENCODING
        && ((unsigned char*)yytext)[0] >= 0xA1
        && ((unsigned char*)yytext)[0] <= 0xFE) {
        BEGIN GB2312_2ND_CHAR;
    }
}

<STRING_LITERAL_STATE><<EOF>> {
    memset(error_message_buf, 0, error_message_buf_len);
    sprintf_s(error_message_buf, 100, "end of file in string literal");
    dkc_compile_error(dkc_get_current_compiler()->current_line_number, error_message_buf);
}

<GB2312_2ND_CHAR>. {
    dkc_add_string_literal(yytext[0]);
    BEGIN STRING_LITERAL_STATE;
}

%%