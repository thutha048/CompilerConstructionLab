/* Scanner
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reader.h"
#include "charcode.h"
#include "token.h"
#include "error.h"


extern int lineNo;
extern int colNo;
extern int currentChar;

extern CharCode charCodes[];

/***************************************************************/

void skipBlank() {
  // TODO
 do {
    readChar();
    if (currentChar == EOF) break;
  }while (charCodes[currentChar] == CHAR_SPACE);
}

void skipComment() {
  // TODO
  readChar();
  do {
    if (currentChar == EOF)
      error(ERR_ENDOFCOMMENT, lineNo, colNo);
    else if (charCodes[currentChar] == CHAR_TIMES) {
      readChar();
      if (charCodes[currentChar] == CHAR_RPAR) break;
      else if(currentChar == EOF){
        error(ERR_ENDOFCOMMENT, lineNo, colNo);
      } else readChar();
    } else readChar();
  }while (1);
  readChar();
}

/*void readString() {
  readChar();
  do{  
    if (currentChar == EOF)
      error(ERR_ENDOFSTRING, lineNo, colNo);
    else if (charCodes[currentChar] == CHAR_DOUBLEQUOTE) {
     // readChar();
       if(currentChar == EOF){
        error(ERR_ENDOFSTRING, lineNo, colNo);
        break;
       }else readChar();
    }
  }while(1);
  readChar();
}*/

Token* readIdentKeyword(void) {
  // TODO
  Token *result = makeToken(TK_IDENT, lineNo, colNo);
  char *ident = (char *)calloc(MAX_IDENT_LEN + 1, sizeof(char));
  if (ident == NULL)
  {
    printf("Calloc Fail");
    exit(-1);
  }

  int i = 0;
  do
  {
    if (currentChar == EOF)
      break;
    ident[i] = (char)currentChar;
    // printf("%c\n", ident[i]);
    if (++i > MAX_IDENT_LEN)
    {
      error(ERR_IDENTTOOLONG, lineNo, colNo);
    }
    readChar();

  } while (charCodes[currentChar] == CHAR_LETTER || 
  charCodes[currentChar] == CHAR_DIGIT || charCodes[currentChar] == CHAR_UNDERSCORE);

  ident[i] = '\0';

  TokenType identType = checkKeyword(ident);
  if (identType == TK_NONE)
    identType = TK_IDENT;
  result->tokenType = identType;

  strcpy(result->string, ident);
  if (ident != NULL)
    free(ident);
  return result;
}


Token* readNumber(void) {
  // TODO
  Token* result = makeToken(TK_NUMBER, lineNo, colNo);

  char* strNum = (char *) calloc(MAX_IDENT_LEN + 1, sizeof(char));
  if (strNum == NULL) {
    printf("Calloc Fail");
    exit(-1);
  }
  int i = 0;
  do {
    if (currentChar == EOF) break;
    strNum[i] = (char) currentChar;
    if (++i > MAX_IDENT_LEN) {
      error(ERR_IDENTTOOLONG, lineNo, colNo);
    }
    readChar();
  } while(charCodes[currentChar] == CHAR_DIGIT);

  /* if(charCodes[currentChar] == CHAR_PERIOD) {
     result->tokenType = TK_FLOAT;
     strNum[i++] = (char) currentChar;
     readChar();
     do {
       if (currentChar == EOF) break;
       strNum[i] = (char) currentChar;
       if (++i > MAX_IDENT_LEN) {
         error(ERR_IDENTTOOLONG, lineNo, colNo);
       }
       readChar();
     } while(charCodes[currentChar] == CHAR_DIGIT);
   }*/

  strNum[i] = '\0';

  strcpy(result->string, strNum);
  if (strNum != NULL) free(strNum);
  return result;
}


Token *readString(void){
  int c=0;
  Token *result = makeToken(TK_STRING, lineNo, colNo);
  while( (charCodes[currentChar] == CHAR_LETTER || 
          charCodes[currentChar] == CHAR_DIGIT || charCodes[currentChar] == CHAR_UNDERSCORE || charCodes[currentChar]==CHAR_SPACE) &&currentChar!= EOF ){
            result->string[c]= (char)currentChar;
            c++;
            if(c>MAX_STRING_LEN){
              error(ERR_ENDOFSTRING,lineNo,colNo);
              return result;
            }
            readChar();
          }
  if(charCodes[currentChar]!= CHAR_DOUBLEQUOTE){
   error(ERR_ENDOFSTRING,lineNo,colNo);
    return result;
  }
  if(c==0){
    result->tokenType= TK_NONE;
    return result;
  }
  result->string[c]='\0';
  readChar();
  return result;
}


Token* readConstChar(void) {
  // TODO
  char c[2] = "\0";
  Token* result = makeToken(TK_CHAR, lineNo, colNo);
  readChar();
  if (currentChar == EOF) {
    error(ERR_INVALIDCHARCONSTANT, lineNo, colNo);
  }else{
    c[0] = (char) currentChar;
    c[1] = '\0';
    readChar();
    if (charCodes[currentChar] == CHAR_SINGLEQUOTE) {
      strcpy(result->string, c);

    } else{
      error(ERR_INVALIDCHARCONSTANT, lineNo, colNo);
      // printf("Flagg hereee");
    }
    readChar();
  }
  return result;
}

Token* getToken(void) {
  Token *token;
  int ln, cn;
  ln= lineNo;
  cn = colNo;
  // printf("curentChar: %d\n",currentChar);
  if (currentChar == EOF) 
    return makeToken(TK_EOF, lineNo, colNo);

  switch (charCodes[currentChar]) {
  case CHAR_SPACE: skipBlank(); return getToken();
  case CHAR_LETTER: return readIdentKeyword();
  case CHAR_DIGIT: return readNumber();
  case CHAR_PLUS:
    readChar();
    if(charCodes[currentChar] == CHAR_EQ) {
      readChar();
      return makeToken(SB_ASSIGN_PLUS, ln, cn);
    }
    return makeToken(SB_PLUS, ln, cn);
  case CHAR_MINUS:
    readChar();
    if(charCodes[currentChar] == CHAR_EQ) {
      readChar();
      return makeToken(SB_ASSIGN_SUBTRACT, ln, cn);
    }
    return makeToken(SB_MINUS, ln, cn);
  case CHAR_TIMES:
    readChar();
    if(charCodes[currentChar] == CHAR_EQ) {
      readChar();
      return makeToken(SB_ASSIGN_TIMES, ln, cn);
    }
    return makeToken(SB_TIMES, ln, cn);
  case CHAR_SLASH:
    readChar();
    if(charCodes[currentChar] == CHAR_EQ) {
      readChar();
      return makeToken(SB_ASSIGN_DIVIDE, ln, cn);
    }
    return makeToken(SB_SLASH, ln, cn);
  case CHAR_MODULE:
    readChar();
    return makeToken(SB_MODULE, ln, cn);
  case CHAR_LT:
    readChar();
    if (charCodes[currentChar] == CHAR_EQ) {
      readChar();
      return makeToken(SB_LE, ln, cn);
    }
    return makeToken(SB_LT, ln, cn);
  case CHAR_GT:
    readChar();
    if (charCodes[currentChar] == CHAR_EQ) {
      readChar();
      return makeToken(SB_GE, ln, cn);
    }
    return makeToken(SB_GT, ln, cn);
  case CHAR_EXCLAIMATION:
    readChar();
    if (charCodes[currentChar] == CHAR_EQ) {
      readChar();
      return makeToken(SB_NEQ, ln, cn);
    }
    error(ERR_INVALIDSYMBOL, ln, cn);
  case CHAR_EQ:
    readChar();
    return makeToken(SB_EQ, ln, cn);
  case CHAR_COMMA:
    readChar();
    return makeToken(SB_COMMA, ln, cn);
  case CHAR_PERIOD:
    readChar();
    if (charCodes[currentChar] == CHAR_RPAR) {
      readChar();
      return makeToken(SB_RSEL, ln, cn);
    }
    return makeToken(SB_PERIOD, ln, cn);
  case CHAR_COLON:
    readChar();
    if (charCodes[currentChar] == CHAR_EQ) {
      readChar();
      return makeToken(SB_ASSIGN, ln, cn);
    }
    return makeToken(SB_COLON, ln, cn);
  case CHAR_SEMICOLON:
    readChar();
    return makeToken(SB_SEMICOLON, ln, cn);
  case CHAR_SINGLEQUOTE:
    return readConstChar();
  case CHAR_LPAR:
    readChar();
    if (charCodes[currentChar] == CHAR_TIMES) {
      skipComment();
      return getToken();
    } else if (charCodes[currentChar] == CHAR_PERIOD) {
      readChar();
      return makeToken(SB_LSEL, ln, cn);
    } else {
      return makeToken(SB_LPAR, ln, cn);
    }
  case CHAR_RPAR:
    readChar();
    return makeToken(SB_RPAR, ln, cn);
  case CHAR_DOUBLEQUOTE:
    readChar();
    return readString();
  

  default:
    token = makeToken(TK_NONE, lineNo, colNo);
    error(ERR_INVALIDSYMBOL, lineNo, colNo);
    readChar(); 
    return token;
  }
}


/******************************************************************/

void printToken(Token *token) {

  printf("%d-%d:", token->lineNo, token->colNo);

  switch (token->tokenType) {
  case TK_NONE: printf("TK_NONE\n"); break;
  case TK_IDENT: printf("TK_IDENT(%s)\n", token->string); break;
  case TK_NUMBER: printf("TK_NUMBER(%s)\n", token->string); break;
  case TK_CHAR: printf("TK_CHAR(\'%s\')\n", token->string); break;
  case TK_EOF: printf("TK_EOF\n"); break;
  case TK_STRING: printf("TK_STRING(%s)\n", token->string); break;

  // case TK_FLOAT: printf("TK_FLOAT\n"); break;
  case KW_PROGRAM: printf("KW_PROGRAM\n"); break;
  case KW_CONST: printf("KW_CONST\n"); break;
  case KW_TYPE: printf("KW_TYPE\n"); break;
  case KW_VAR: printf("KW_VAR\n"); break;
  case KW_INTEGER: printf("KW_INTEGER\n"); break;
  case KW_CHAR: printf("KW_CHAR\n"); break;
  case KW_ARRAY: printf("KW_ARRAY\n"); break;
  case KW_OF: printf("KW_OF\n"); break;
  case KW_FUNCTION: printf("KW_FUNCTION\n"); break;
  case KW_PROCEDURE: printf("KW_PROCEDURE\n"); break;
  case KW_BEGIN: printf("KW_BEGIN\n"); break;
  case KW_END: printf("KW_END\n"); break;
  case KW_CALL: printf("KW_CALL\n"); break;
  case KW_IF: printf("KW_IF\n"); break;
  case KW_THEN: printf("KW_THEN\n"); break;
  case KW_ELSE: printf("KW_ELSE\n"); break;
  case KW_WHILE: printf("KW_WHILE\n"); break;
  case KW_DO: printf("KW_DO\n"); break;
  case KW_FOR: printf("KW_FOR\n"); break;
  case KW_TO: printf("KW_TO\n"); break;

  case SB_SEMICOLON: printf("SB_SEMICOLON\n"); break;
  case SB_COLON: printf("SB_COLON\n"); break;
  case SB_PERIOD: printf("SB_PERIOD\n"); break;
  case SB_COMMA: printf("SB_COMMA\n"); break;
  case SB_ASSIGN: printf("SB_ASSIGN\n"); break;
  case SB_EQ: printf("SB_EQ\n"); break;
  case SB_NEQ: printf("SB_NEQ\n"); break;
  case SB_LT: printf("SB_LT\n"); break;
  case SB_LE: printf("SB_LE\n"); break;
  case SB_GT: printf("SB_GT\n"); break;
  case SB_GE: printf("SB_GE\n"); break;
  case SB_PLUS: printf("SB_PLUS\n"); break;
  case SB_MINUS: printf("SB_MINUS\n"); break;
  case SB_TIMES: printf("SB_TIMES\n"); break;
  case SB_SLASH: printf("SB_SLASH\n"); break;
  case SB_MODULE: printf("SB_MODULE\n"); break;
  case SB_LPAR: printf("SB_LPAR\n"); break;
  case SB_RPAR: printf("SB_RPAR\n"); break;
  case SB_LSEL: printf("SB_LSEL\n"); break;
  case SB_RSEL: printf("SB_RSEL\n"); break;
  case SB_ASSIGN_PLUS: printf("SB_ASSIGN_PLUS\n"); break;
  case SB_ASSIGN_SUBTRACT: printf("SB_ASSIGN_SUBTRACT\n"); break;
  case SB_ASSIGN_TIMES: printf("SB_ASSIGN_TIMES\n"); break;
  case SB_ASSIGN_DIVIDE: printf("SB_ASSIGN_DIVIDE\n"); break;
  case SB_DOUBLEQUOTE: printf("SB_DOUBLEQUOTE\n"); break;

  }
}

int scan(char *fileName) {
  Token *token;

  if (openInputStream(fileName) == IO_ERROR)
    return IO_ERROR;

  token = getToken();
  while (token->tokenType != TK_EOF) {
    printToken(token);
    free(token);
    token = getToken();
  }

  free(token);
  closeInputStream();
  return IO_SUCCESS;
}

/******************************************************************/

int main(int argc, char *argv[]) {
  if (argc <= 1) {
    printf("scanner: no input file.\n");
    return -1;
  }

  if (scan(argv[1]) == IO_ERROR) {
    printf("Can\'t read input file!\n");
    return -1;
  }
    
  return 0;
}



