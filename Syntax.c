#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Lexer.c"



char* parse(Token_Nodes** current);
char* parseProgram(Token_Nodes** current);
char* parseStatement(Token_Nodes** current);
char* parseArithmetic(Token_Nodes** current);
char* parseTerm(Token_Nodes** current);
char* parseFactor(Token_Nodes** current);
char* parseAssignment(Token_Nodes** current);
char* parseRelational(Token_Nodes** current);
char* parseConditional(Token_Nodes** current);

Token_Nodes* advance(Token_Nodes** current);

Token_Nodes* current;

int parseCount = 1;
//int branchDeduct = 0;

void branchCreate();

//Function to create lines that serve as branch for the Abstract Syntax Tree
void branchCreate(){
    for (int i = 0; i < parseCount; i++){
        printf("-");
    }

    parseCount++;
    return;
}
char* parse(Token_Nodes** current){
    char* stmt;

    printf(">PROGRAM\n");
    while ((*current) != NULL){

        if(!(strcmp((*current)->tokenType, "NEWLINE"))){
            *current = advance(current);
            printf("%s", (*current)->tokenType);
            continue;
        }
        //printf("ITERATION [%d]\n", i++);
        if((stmt = parseProgram(current)) != NULL){
            printf("STATEMENT OUTPUT: %s\n\n", stmt);
            
        }
        //printf("---Last pointed to: %s %s\n\n", (*current)->tokenType, (*current)->tokenValue);
        //*current = advance(current);
        
    }

    return ""; //parseProgram(&current);
}

char* parseProgram(Token_Nodes** current){

    //printf("---Last pointed to: %s %s\n\n", (*current)->tokenType, (*current)->tokenValue);
    printf(">STATEMENT");
    return parseStatement(current);
}

char* parseStatement(Token_Nodes** current){

    char* stmt;
    Token_Nodes* lookAhead = advance(current);

    if (!(strcmp(lookAhead->tokenType, "ADDITION_OPERATOR")) || !(strcmp(lookAhead->tokenType, "SUBTRACTION_OPERATOR")) || !(strcmp(lookAhead->tokenType, "MULTIPLICATION_OPERATOR"))
    || !(strcmp(lookAhead->tokenType, "DIVISION_OPERATOR")) || !(strcmp(lookAhead->tokenType, "EXPONENTIAL_OPERATOR")) || !(strcmp(lookAhead->tokenType, "MODULO_OPERATOR"))
    || !(strcmp(lookAhead->tokenType, "FLOOR_DIVISION_OPERATOR"))) {
        
        printf("\n\n---ARITHMETIC STATEMENT---\n");
        stmt = (parseArithmetic(current));
        return stmt;

    }

    else if (!(strcmp(lookAhead->tokenType, "ASS_OP")) || !(strcmp(lookAhead->tokenType, "ADD_ASS_OP")) || !(strcmp(lookAhead->tokenType, "SUB_ASS_OP"))
    || !(strcmp(lookAhead->tokenType, "MULTIPLY_ASS_OP")) || !(strcmp(lookAhead->tokenType, "DIVISION_ASS_OP")) || !(strcmp(lookAhead->tokenType, "EXPONENTIATION_ASS_OP"))
    || !(strcmp(lookAhead->tokenType, "MODULO_ASS_OP")) || !(strcmp(lookAhead->tokenType, "FLOOR_DIVISION_ASS_OP"))) {

        printf("\n\n---ASSIGNMENT---\n");
        stmt = (parseAssignment(current));
        return stmt;
    }

    else if (!(strcmp(lookAhead->tokenType, "IS_EQUAL_OP")) || !(strcmp(lookAhead->tokenType, "NOT_EQUAL_OP"))
    || !(strcmp(lookAhead->tokenType, "GREATER_THAN_OP")) || !(strcmp(lookAhead->tokenType, "LESS_THAN_OP"))
    || !(strcmp(lookAhead->tokenType, "GREATER_EQUAL_OP")) || !(strcmp(lookAhead->tokenType, "LESS_EQUAL_OP"))){

        printf("---RELATIONAL---\n");
        stmt = (parseRelational(current));
        return stmt;

    }

    else if (!(strcmp((*current)->tokenType, "KEYWORD"))){
        if (!(strcmp((*current)->tokenValue, "when"))){

            printf("\n\n---CONDITIONAL---\n");
            stmt = (parseConditional(current));
            return stmt;
        }
    }

    return stmt;
}

char* parseArithmetic(Token_Nodes** current){
    //Arithmetic Statment Grammar Rule:
    //<arithmetic> ::= <term> {("+" | "-") <term>}*

    char* stmt = malloc(100);
    char* arithStmt;


    if ((arithStmt = parseTerm(current)) != NULL){
        sprintf(stmt, arithStmt);
        
        while (((*current)->tokenType) != NULL && strcmp((*current)->tokenType, "NEWLINE")){
            //if ((strcmp(((*current)->tokenType), "NEWLINE"))){

                if (!(strcmp ((*current)->tokenType, "ADDITION_OPERATOR")) || !(strcmp ((*current)->tokenType, "SUBTRACTION_OPERATOR"))){
                    printf(">PARSED %s: %s\n", (*current)->tokenType,(*current)->tokenValue);
                    sprintf(stmt + strlen(stmt), "%s ", (*current)->tokenValue);
                    
                    *current = advance(current);
                    //printf("current %s %s\n\n", (*current)->tokenType, (*current)->tokenValue);
                    //Check if current token is <term>

                    //printf("--->TERM\n------");
                    //If not, return Syntax Error
                    if ((arithStmt = parseTerm(current))){
                        sprintf(stmt + strlen(stmt), arithStmt);
                        
                    }

                    else if (arithStmt == NULL){
                        return NULL;
                    }
                    
                    else{
                        printf("SYNTAX ERROR: UNEXPECTED TOKEN TYPE: %s\n\n", (*current)->tokenType);
                        *current = advance(current);
                        return "";
                    }
                    
                }
                
            //}
            
            if ((*current)->tokenType != NULL){
                if (!(strcmp((*current)->tokenType, "CLOSED_PARENTHESIS"))){
                    
                    return stmt;
                }
            }
            
            //printf("%s", (*current)->tokenType);
        }

        
        
        
        if ((*current) != NULL){
            *current = advance(current);
        }

        //printf(">ARITHMETIC STATEMENT PARSED: ");
        return stmt;
        
    }

    return NULL;
}

char* parseTerm(Token_Nodes** current){
    char* stmt = malloc(100);
    char* termStmt = malloc(100);


    //Check if current token is <factor>
    //printf("--->FACTOR\n--------");
    if ((termStmt = parseFactor(current)) != NULL){
        sprintf(stmt, "%s ", termStmt); 
        
        while ((*current) != NULL){
            
            if (!(strcmp ((*current)->tokenType, "MULTIPLICATION_OPERATOR")) || !(strcmp ((*current)->tokenType, "DIVISION_OPERATOR"))
            || !(strcmp ((*current)->tokenType, "EXPNOENTIATION_OPERATOR")) || !(strcmp ((*current)->tokenType, "MODULO_OPERATOR"))
            || !(strcmp ((*current)->tokenType, "FLOOR_DIVISION_OPERATOR"))){

                printf(">PARSED %s: %s\n", (*current)->tokenType,(*current)->tokenValue);
                sprintf(stmt + strlen(stmt), "%s ", (*current)->tokenValue);

                *current = advance(current);
                if ((termStmt = parseFactor(current))){
                        sprintf(stmt + strlen(stmt), termStmt);
                        
                }

                else if (termStmt == NULL){
                    return NULL;
                }

                else{
                        printf("SYNTAX ERROR: UNEXPECTED TOKEN TYPE: %s\n\n", (*current)->tokenType);
                        *current = advance(current);
                        return NULL;
                }
            }
            
            
            else{
                
                return stmt;
            }

            
        }

        //printf("%s", (*current)->tokenType);
        return stmt;
    }

    return NULL;
}

char* parseFactor(Token_Nodes** current){

    char* stmt = malloc(100);
    char* factorStmt = malloc(100);
    if (!(strcmp((*current)->tokenType, "IDENTIFIER"))){

        printf(">PARSED FACTOR: %s\n", (*current)->tokenValue);
        sprintf(stmt, (*current)->tokenValue);

        *current = advance(current);

        return stmt;
    }

    else if (!(strcmp((*current)->tokenType, "NUMBER_LITERAL"))){

        printf(">PARSED FACTOR: %s\n", (*current)->tokenValue);
        sprintf(stmt, (*current)->tokenValue);

        *current = advance(current);
        
        return stmt;
    }

    else if (!(strcmp((*current)->tokenType, "OPEN_PARENTHESIS"))){
        //printf("Currently Pointing at: %s %s\n", lookahead->tokenType, lookahead->tokenValue);

        printf(">PARSED: %s\n", (*current)->tokenValue);
        sprintf(stmt, (*current)->tokenValue);

        

        *current = advance(current);
        
        if ((factorStmt = parseArithmetic(current)) == NULL){
            printf("SYNTAX ERROR: EXPECTED ARITHMETIC STATEMENT");
            *current = advance(current);
            return NULL;
        }
        //printf("%s", factorStmt);

        sprintf(stmt + strlen(stmt), "%s", factorStmt);

        

        //*current = advance(current);

        if ((*current) == NULL || (strcmp((*current)->tokenType, "CLOSED_PARENTHESIS"))){
            printf("SYNTAX ERROR: EXPECTED: TOKEN TYPE: \"CLOSED_PARENTHESIS\"");
            *current = advance(current);
            return NULL;
        }

        printf(">PARSED %s\n", (*current)->tokenValue);
        
        sprintf(stmt + strlen(stmt), "%s ", (*current)->tokenValue);

        *current = advance(current);
        
        
        
        return stmt;
        
    }

    /*else if (!((stmt = parseArithmetic(current)) == NULL)) {
        return "";
    }*/

    return NULL;
}

char* parseAssignment(Token_Nodes** current){
    //Production Rule for Assignment Statement
    // <assignment> ::= (<identifier> "=" (<arithmetic>|<string> | <boolean>)) | (<identifier> ("+=" | "-=" | "*=" | "/=" | "^*=" | "%=" | "//=") (<arithmetic>))

    //Allocate memory for stmt
    char* stmt = malloc(100);
    char* arithStmt = malloc(100);

    //Check first token for <IDENTIFIER>
    if (!(strcmp((*current)->tokenType, "IDENTIFIER"))){

        //Print parsed token
        printf(">PARSED %s: %s\n", (*current)->tokenType, (*current)->tokenValue);

        //Print into stmt string
        sprintf(stmt, "%s", (*current)->tokenValue);

        //Advance Token
        *current = advance(current);

        //Check if <ASS_OP>
        if (!(strcmp((*current)->tokenType, "ASS_OP"))){

            //Print parsed token
            printf(">PARSED %s: %s\n", (*current)->tokenType, (*current)->tokenValue);

            //Print into stmt string
            sprintf(stmt + strlen(stmt),  " %s ", (*current)->tokenValue);

            //Advance Token
            *current = advance(current);

            //Or check if <ARITHMETIC>
            //Insert Arithmetic Rule
            Token_Nodes* lookAhead = advance(current);
            
            if (lookAhead != NULL){

                if (!(strcmp(lookAhead->tokenType, "ADDITION_OPERATOR")) || !(strcmp(lookAhead->tokenType, "SUBTRACTION_OPERATOR")) || !(strcmp(lookAhead->tokenType, "MULTIPLICATION_OPERATOR"))
                || !(strcmp(lookAhead->tokenType, "DIVISION_OPERATOR")) || !(strcmp(lookAhead->tokenType, "EXPONENTIAL_OPERATOR")) || !(strcmp(lookAhead->tokenType, "MODULO_OPERATOR"))
                || !(strcmp(lookAhead->tokenType, "FLOOR_DIVISION_OPERATOR"))) {

                    printf("\n---ARITHMETIC---\n");
                    arithStmt = parseArithmetic(current);
                        printf("%s\n", arithStmt);
                        sprintf(stmt + strlen(stmt), "%s ", arithStmt);
                        return stmt;

                }
            }

            //Check if <IDENTIFIER>
            if (!(strcmp((*current)->tokenType, "IDENTIFIER"))){
                printf(">PARSED %s: %s\n", (*current)->tokenType, (*current)->tokenValue);
                sprintf(stmt + strlen(stmt),  " %s", (*current)->tokenValue);

                *current = advance(current);
                return stmt;
            }

            //Or check if <BOOLEAN>
            else if (!(strcmp((*current)->tokenType, "BOOLEAN"))){
                printf(">PARSED %s: %s\n", (*current)->tokenType, (*current)->tokenValue);
                sprintf(stmt + strlen(stmt),  " %s", (*current)->tokenValue);

                *current = advance(current);
                return stmt;
            }

            else if (!(strcmp((*current)->tokenType, "NUMBER_LITERAL"))){
                printf(">PARSED %s: %s\n", (*current)->tokenType, (*current)->tokenValue);
                sprintf(stmt + strlen(stmt),  " %s", (*current)->tokenValue);

                *current = advance(current);
                return stmt;
            }

            else if (!(strcmp((*current)->tokenType, "STRING_LITERAL"))){
                printf(">PARSED %s: %s\n", (*current)->tokenType, (*current)->tokenValue);
                sprintf(stmt + strlen(stmt),  " %s", (*current)->tokenValue);

                *current = advance(current);
                return stmt;
            }

            //If checks fail, return Error
            else{
                *current = advance(current);
                return "SYNTAX ERROR";
                
            }
        }

        //Or check if Compound Assignment
        else if (!(strcmp((*current)->tokenType, "ADD_ASS_OP")) || !(strcmp((*current)->tokenType, "SUB_ASS_OP")) || !(strcmp((*current)->tokenType, "MULTIPLY_ASS_OP"))
        || !(strcmp((*current)->tokenType, "DIVISION_ASS_OP")) || !(strcmp((*current)->tokenType, "EXPONENTIATION_ASS_OP"))|| !(strcmp((*current)->tokenType, "MODULO_ASS_OP"))
        || !(strcmp((*current)->tokenType, "FLOOR_DIVISION_ASS_OP"))) {

            //Print parsed token
            printf("--->PARSED %s: %s\n----", (*current)->tokenType, (*current)->tokenValue);

            //Print into stmt string
            sprintf(stmt + strlen(stmt),  " %s ", (*current)->tokenValue);

            //Advance Token
            *current = advance(current);

            Token_Nodes* lookAhead = advance(current);
            
            if (lookAhead != NULL){
                if (!(strcmp(lookAhead->tokenType, "ADDITION_OPERATOR")) || !(strcmp(lookAhead->tokenType, "SUBTRACTION_OPERATOR")) || !(strcmp(lookAhead->tokenType, "MULTIPLICATION_OPERATOR"))
                || !(strcmp(lookAhead->tokenType, "DIVISION_OPERATOR")) || !(strcmp(lookAhead->tokenType, "EXPONENTIAL_OPERATOR")) || !(strcmp(lookAhead->tokenType, "MODULO_OPERATOR"))
                || !(strcmp(lookAhead->tokenType, "FLOOR_DIVISION_OPERATOR"))) {

                    printf(">ARITHMETIC\n");
                    arithStmt = parseArithmetic(current);
                        printf("%s\n", arithStmt);
                        sprintf(stmt + strlen(stmt), "%s ", arithStmt);
                        return stmt;

                }
            }



            //Check if <IDENTIFIER>
            if (!(strcmp((*current)->tokenType, "IDENTIFIER"))){
                printf(">PARSED %s: %s\n", (*current)->tokenType, (*current)->tokenValue);
                sprintf(stmt + strlen(stmt),  " %s", (*current)->tokenValue);

                *current = advance(current);
                return stmt;
            }

            //Or check if <BOOLEAN>
            else if (!(strcmp((*current)->tokenType, "BOOLEAN"))){
                printf(">PARSED %s: %s\n", (*current)->tokenType, (*current)->tokenValue);
                sprintf(stmt + strlen(stmt),  " %s", (*current)->tokenValue);

                *current = advance(current);
                return stmt;
            }

            else if (!(strcmp((*current)->tokenType, "NUMBER_LITERAL"))){
                printf(">PARSED %s: %s\n", (*current)->tokenType, (*current)->tokenValue);
                sprintf(stmt + strlen(stmt),  " %s", (*current)->tokenValue);

                *current = advance(current);
                return stmt;
            }

            else if (!(strcmp((*current)->tokenType, "STRING_LITERAL"))){
                printf(">PARSED %s: %s\n", (*current)->tokenType, (*current)->tokenValue);
                sprintf(stmt + strlen(stmt),  " %s", (*current)->tokenValue);

                *current = advance(current);
                return stmt;
            }


            else{
                return "SYNTAX ERROR";
            }

        }

    }

    return NULL;
}

char* parseRelational(Token_Nodes** current){
    char* stmt = malloc(100);
    if (!(strcmp((*current)->tokenType, "IDENTIFIER")) || !(strcmp((*current)->tokenType, "NUMBER_LITERAL"))
    || !(strcmp((*current)->tokenType, "STRING"))){

        printf(">PARSED %s: %s\n", (*current)->tokenType, (*current)->tokenValue);
        sprintf(stmt, "%s ", (*current)->tokenValue);

        *current = advance(current);

        if (!(strcmp((*current)->tokenType, "IS_EQUAL_OP")) || !(strcmp((*current)->tokenType, "NOT_EQUAL_OP"))
        || !(strcmp((*current)->tokenType, "GREATER_THAN_OP")) || !(strcmp((*current)->tokenType, "LESS_THAN_OP"))
        || !(strcmp((*current)->tokenType, "GREATER_EQUAL_OP")) || !(strcmp((*current)->tokenType, "LESS_EQUAL_OP"))){
            
            
            printf(">PARSED %s: %s\n", (*current)->tokenType, (*current)->tokenValue);
            sprintf(stmt + strlen(stmt), "%s ", (*current)->tokenValue);

            *current = advance(current);

            if (!(strcmp((*current)->tokenType, "IDENTIFIER")) || !(strcmp((*current)->tokenType, "NUMBER_LITERAL"))
            || !(strcmp((*current)->tokenType, "STRING"))){

                printf(">PARSED %s: %s\n", (*current)->tokenType, (*current)->tokenValue);
                sprintf(stmt + strlen(stmt), "%s ", (*current)->tokenValue);
            }

            else{
                printf("SYNTAX ERROR: UNEXPECTED TOKEN TYPE: %s (%s)", (*current)->tokenType, (*current)->tokenValue);
                return "";
            }
            
            *current = advance(current);
            return stmt;
        }

    }

    return NULL ;
}

char* parseLogical(Token_Nodes** current){


}

char* parseConditional (Token_Nodes** current){
    char* stmt = malloc(100);
    char* conditionalStmt = malloc(100);

    if (!(strcmp((*current)->tokenValue, "when"))){
        printf("\n>CONDITIONAL KEYWORD: %s\n", (*current)->tokenValue);
        sprintf(stmt, (*current)->tokenValue);

        *current = advance(current);
        

        Token_Nodes* lookAhead = advance(current);

        if (!(strcmp(lookAhead->tokenType, "IS_EQUAL_OP")) || !(strcmp(lookAhead->tokenType, "NOT_EQUAL_OP"))
        || !(strcmp(lookAhead->tokenType, "GREATER_THAN_OP")) || !(strcmp(lookAhead->tokenType, "LESS_THAN_OP"))
        || !(strcmp(lookAhead->tokenType, "GREATER_EQUAL_OP")) || !(strcmp(lookAhead->tokenType, "LESS_EQUAL_OP"))){

            if ((conditionalStmt = parseRelational(current)) != NULL){
                //printf("PARSED LOGICAL: %s", conditionalStmt);
                sprintf(stmt + strlen(stmt), " %s", conditionalStmt);
            }

            // *current = advance(current);
            // printf("%s", (*current)->tokenType);

        }


        else if (!(strcmp((*current)->tokenType, "IDENTIFIER"))){
             printf(">PARSED: %s\n", (*current)->tokenValue);
                sprintf(stmt + strlen(stmt), " %s", (*current)->tokenType);
                *current = advance(current);
        }

        else if (!(strcmp((*current)->tokenType, "BOOLEAN"))){
             printf(">PARSED: %s\n", (*current)->tokenValue);
                sprintf(stmt + strlen(stmt), " %s", (*current)->tokenType);
                *current = advance(current);
        }

        else {
            printf("SYNTAX ERROR:");
            *current = advance(current);
            return NULL;
        }


        if (!(strcmp((*current)->tokenType, "COLON"))){
            printf(">PARSED: %s\n", (*current)->tokenType);
            sprintf(stmt + strlen(stmt), "%s\n  ", (*current)->tokenValue);
        }

        else{
            printf("SYNTAX ERROR:");
            *current = advance(current);
            return NULL;
        }

        *current = advance(current);

        printf("-->STATEMENT:");
        if ((conditionalStmt = parseStatement(current)) != NULL){
            sprintf(stmt + strlen(stmt), "%s ", conditionalStmt);
            
            
        }

        if ((*current) == NULL){
            printf("\n\nPARSED CONDITIONAL STATEMENT:\n");
            return stmt;
        }

        if (!(strcmp((*current)->tokenValue, "otherwise"))){
            printf("\n>CONDITIONAL KEYWORD: %s\n", (*current)->tokenValue);
            sprintf(stmt + strlen(stmt), "\n%s", (*current)->tokenValue);

            *current = advance(current);
            

            Token_Nodes* lookAhead = advance(current);

            if (!(strcmp(lookAhead->tokenType, "IS_EQUAL_OP")) || !(strcmp(lookAhead->tokenType, "NOT_EQUAL_OP"))
            || !(strcmp(lookAhead->tokenType, "GREATER_THAN_OP")) || !(strcmp(lookAhead->tokenType, "LESS_THAN_OP"))
            || !(strcmp(lookAhead->tokenType, "GREATER_EQUAL_OP")) || !(strcmp(lookAhead->tokenType, "LESS_EQUAL_OP"))){

                if ((conditionalStmt = parseRelational(current)) != NULL){
                    //printf("PARSED LOGICAL: %s", conditionalStmt);
                    sprintf(stmt + strlen(stmt), " %s", conditionalStmt);
                }

                // *current = advance(current);
                // printf("%s", (*current)->tokenType);



            }

            else if (!(strcmp((*current)->tokenType, "IDENTIFIER"))){
                printf(">PARSED: %s\n", (*current)->tokenValue);
                    sprintf(stmt + strlen(stmt), " %s", (*current)->tokenValue);
                    *current = advance(current);
            }

            else if (!(strcmp((*current)->tokenType, "BOOLEAN"))){
                printf(">PARSED: %s\n", (*current)->tokenValue);
                    sprintf(stmt + strlen(stmt), " %s", (*current)->tokenType);
                    *current = advance(current);
            }

            else {
                printf("SYNTAX ERROR:");
                *current = advance(current);
                return NULL;
            }


            if (!(strcmp((*current)->tokenType, "COLON"))){
                printf(">PARSED: %s\n", (*current)->tokenType);
                sprintf(stmt + strlen(stmt), "%s\n  ", (*current)->tokenValue);
            }

            else{
                printf("SYNTAX ERROR:");
                *current = advance(current);
                return NULL;
            }

            *current = advance(current);

            printf("-->STATEMENT:");
            if ((conditionalStmt = parseStatement(current)) != NULL){
                sprintf(stmt + strlen(stmt), "%s", conditionalStmt);

                
            }
        }
        // *current = advance(current);
        // *current = advance(current);

        if ((*current) == NULL){
            printf("\n\nPARSED CONDITIONAL STATEMENT:\n");
            return stmt;
        }

        else{
            *current = advance(current);
        }
        if (!(strcmp((*current)->tokenValue, "unless"))){
            printf("\n>CONDITIONAL KEYWORD: %s\n", (*current)->tokenValue);
            sprintf(stmt + strlen(stmt), "\n%s", (*current)->tokenValue);

            *current = advance(current);

            if (!(strcmp((*current)->tokenType, "COLON"))){
                printf(">PARSED: %s\n", (*current)->tokenType);
                sprintf(stmt + strlen(stmt), "%s\n  ", (*current)->tokenValue);
            }

            else{
                printf("SYNTAX ERROR:");
                *current = advance(current);
                return NULL;
            }

            *current = advance(current);

            printf("-->STATEMENT:");
            if ((conditionalStmt = parseStatement(current)) != NULL){
                sprintf(stmt + strlen(stmt), "%s\n\n", conditionalStmt);

                
            }
        }

        else {
            printf("\n\nPARSED CONDITIONAL STATEMENT:\n");
            return stmt;
        }

        printf("\n\nPARSED CONDITIONAL STATEMENT:\n");
        return stmt;
        
    }

    return NULL;

}

Token_Nodes* advance(Token_Nodes** current){
    
    
    return (*current)->next;
}

int main (){
    Lexer();

    parse(&current);
    printf("\n\nEND");

    return 0;
}