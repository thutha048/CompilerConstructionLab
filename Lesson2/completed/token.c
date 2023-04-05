/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdlib.h>
#include <ctype.h>
#include "token.h"

struct {
  char string[MAX_STRING_LEN + 1];
  //char string2[MAX_STRING_LEN+1];
  TokenType tokenType;
} keywords[KEYWORDS_COUNT] = {
  {"PROGRAM", KW_PROGRAM},
  {"CONST", KW_CONST},
  {"TYPE", KW_TYPE},
  {"VAR", KW_VAR},
  {"INTEGER", KW_INTEGER},
  {"CHAR", KW_CHAR},
  {"ARRAY", KW_ARRAY},
  {"OF", KW_OF},
  {"FUNCTION", KW_FUNCTION},
  {"PROCEDURE", KW_PROCEDURE},
  {"BEGIN", KW_BEGIN},
  {"END", KW_END},
  {"CALL", KW_CALL},
  {"IF", KW_IF},
  {"THEN", KW_THEN},
  {"ELSE", KW_ELSE},
  {"WHILE", KW_WHILE},
  {"DO", KW_DO},
  {"FOR", KW_FOR},
  {"TO", KW_TO}
};

int keywordEq(char *kw, char *string) {
  while ((*kw != '\0') && (*string != '\0')) {
    if (*kw != toupper(*string)) break;
    kw ++; string ++;
  }
  return ((*kw == '\0') && (*string == '\0'));
}

TokenType checkKeyword(char *string) {
  int i;
  for (i = 0; i < KEYWORDS_COUNT; i++)
    if (keywordEq(keywords[i].string, string)) 
      return keywords[i].tokenType;
  return TK_NONE;
}

Token* makeToken(TokenType tokenType, int lineNo, int colNo) {
  Token *token = (Token*)malloc(sizeof(Token));
  token->tokenType = tokenType;
  token->lineNo = lineNo;
  token->colNo = colNo;
  return token;
}

/*Token* readString(void) {
  Token *result = makeToken(TK_STRING, lineNo, colNo);
  char *string2 = (char *)calloc(MAX_STRING_LEN + 1, sizeof(char));
  readChar();
    if (currentChar == EOF)
      error(ERR_ENDOFSTRING, lineNo, colNo);
    else if (charCodes[currentChar] == CHAR_DOUBLEQUOTE) {
     // readChar();
       //if(currentChar == EOF){
       error(ERR_ENDOFSTRING, lineNo, colNo);
        //break;
      //} else readChar();
    } 
 // readChar();
  if (string2 == NULL)
   {
      printf("Calloc Fail");
      exit(-1);
   }

 int i = 0;
  do
  {
    if (currentChar == EOF)
     break;
    string2[i] = (char)currentChar;
            // printf("%c\n", ident[i]);
    if (++i > MAX_STRING_LEN)
    {
       error(ERR_STRINGTOOLONG, lineNo, colNo);
    }
    readChar();

  } while (charCodes[currentChar] == CHAR_LETTER || 
          charCodes[currentChar] == CHAR_DIGIT || charCodes[currentChar] == CHAR_UNDERSCORE || charCodes[currentChar]==CHAR_SPACE);

   string2[i] = '\0';

  TokenType stringType = checkKeyword(string2);
  if (stringType == TK_NONE)
       stringType = TK_STRING;
  result->tokenType = stringType;

  strcpy(result->string, string2);
  if (string2 != NULL)
    free(string2);
  return result;
 // readChar();
}
*/