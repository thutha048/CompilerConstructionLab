/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

void freeObject(Object* obj);
void freeScope(Scope* scope);
void freeObjectList(ObjectNode *objList);
void freeReferenceList(ObjectNode *objList);

SymTab* symtab;
Type* intType;
Type* charType;

/******************* Type utilities ******************************/

Type* makeIntType(void) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_INT;
  return type;
}

Type* makeCharType(void) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_CHAR;
  return type;
}

Type* makeArrayType(int arraySize, Type* elementType) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_ARRAY;
  type->arraySize = arraySize;
  type->elementType = elementType;
  return type;
}

Type* duplicateType(Type* type) {
  // TODO
  Type* dupType = (Type*) malloc(sizeof(Type));
  dupType->typeClass = type->typeClass;
  dupType->arraySize = type->arraySize;
  dupType->elementType = type->elementType;
  return dupType;
}

int compareType(Type* type1, Type* type2) {
  // TODO
  if (type1->typeClass != type2->typeClass)
    return 0;

  if (type1->typeClass != TP_ARRAY)
    return 1;

  if (type1->arraySize == type2->arraySize && compareType(type1->elementType, type2->elementType))
    return 1;
  return 0;
}

void freeType(Type* type) {
  // TODO
  if (type == NULL)
    return;
  if (type->elementType == NULL)
    free(type);
  freeType(type->elementType);
  type = NULL;
}

/******************* Constant utility ******************************/

ConstantValue* makeIntConstant(int i) {
  // TODO
  ConstantValue* constantValue = (ConstantValue*)malloc(sizeof(ConstantValue));
  constantValue->type = TP_INT;
  constantValue->intValue = i;
  return constantValue;
}

ConstantValue* makeCharConstant(char ch) {
  // TODO
  ConstantValue* constantValue = (ConstantValue*)malloc(sizeof(ConstantValue));
  constantValue->type = TP_CHAR;
  constantValue->charValue = ch;
  return constantValue;

}

ConstantValue* duplicateConstantValue(ConstantValue* v) {
  // TODO
  ConstantValue* dupConstantValue = (ConstantValue*)malloc(sizeof(ConstantValue));
  dupConstantValue->type = v->type;
  if (v->type == TP_INT)
    dupConstantValue->intValue = v->intValue;
  if (v->type == TP_CHAR)
    dupConstantValue->charValue = v->charValue; 
  return dupConstantValue;
}

/******************* Object utilities ******************************/

Scope* createScope(Object* owner, Scope* outer) {
  Scope* scope = (Scope*) malloc(sizeof(Scope));
  scope->objList = NULL;
  scope->owner = owner;
  scope->outer = outer;
  return scope;
}

Object* createProgramObject(char *programName) {
  Object* program = (Object*) malloc(sizeof(Object));
  strcpy(program->name, programName);
  program->kind = OBJ_PROGRAM;
  program->progAttrs = (ProgramAttributes*) malloc(sizeof(ProgramAttributes));
  program->progAttrs->scope = createScope(program,NULL);
  symtab->program = program;

  return program;
}

Object* createConstantObject(char *name) {
  // TODO
  Object* c = (Object *) malloc(sizeof(Object));
  strcpy(c->name, name);
  c->kind = OBJ_CONSTANT;
  c->constAttrs = (ConstantAttributes *) malloc(sizeof(ConstantAttributes));
  c->constAttrs->value = NULL;

  return c;
}

Object* createTypeObject(char *name) {
  // TODO
  Object* t = (Object *) malloc(sizeof(Object));
  strcpy(t->name, name);
  t->kind = OBJ_TYPE;
  t->typeAttrs = (TypeAttributes *) malloc(sizeof(TypeAttributes));
  t->typeAttrs->actualType = NULL;

  return t;

}

Object* createVariableObject(char *name) {
  // TODO
  Object* var = (Object *) malloc(sizeof(Object));
  strcpy(var->name, name);
  var->kind = OBJ_VARIABLE;
  var->varAttrs = (VariableAttributes *) malloc(sizeof(VariableAttributes));
  var->varAttrs->type = NULL;
  var->varAttrs->scope = createScope(var, symtab->currentScope);

  return var;
}

Object* createFunctionObject(char *name) {
  // TODO
  Object* func = (Object *) malloc(sizeof(Object));
  strcpy(func->name, name);
  func->kind = OBJ_FUNCTION;
  func->funcAttrs = (FunctionAttributes *) malloc(sizeof(FunctionAttributes));
  func->funcAttrs->returnType = NULL;
  func->funcAttrs->scope = createScope(func, symtab->currentScope);
  func->funcAttrs->paramList = NULL;

  return func;

}

Object* createProcedureObject(char *name) {
  // TODO
  Object* proc = (Object *) malloc(sizeof(Object));
  strcpy(proc->name, name);
  proc->kind = OBJ_PROCEDURE;
  proc->procAttrs = (ProcedureAttributes*) malloc(sizeof(ProcedureAttributes));
  proc->procAttrs->paramList = NULL;
  proc->procAttrs->scope = createScope(proc, symtab->currentScope);

  return proc;

}

Object* createParameterObject(char *name, enum ParamKind kind, Object* owner) {
  // TODO
  Object* param = (Object *) malloc(sizeof(Object));
  strcpy(param->name, name);
  param->kind = OBJ_PARAMETER;
  param->paramAttrs = (ParameterAttributes*) malloc(sizeof(ParameterAttributes));
  param->paramAttrs->kind = kind;
  param->paramAttrs->type = NULL;
  param->paramAttrs->function = owner;

  return param;
}

void freeObject(Object* obj) {
  // TODO

  if (obj == NULL) 
    return;

  if (obj->constAttrs == NULL) {
    free(obj);
    obj = NULL;
  }

  switch (obj->kind) {
    case OBJ_CONSTANT:
      free(obj->constAttrs->value);
      obj->constAttrs->value = NULL;
      break;
    case OBJ_VARIABLE:
      free(obj->varAttrs->type);
      obj->varAttrs->type = NULL;
      break;
    case OBJ_TYPE:
      free(obj->typeAttrs->actualType);
      obj->typeAttrs->actualType = NULL;
      break;
    case OBJ_PROGRAM:
      freeScope(obj->progAttrs->scope);
      obj->progAttrs->scope = NULL;
      break;
    case OBJ_FUNCTION:
      freeScope(obj->funcAttrs->scope); // Free scope also free the param list
      obj->funcAttrs->scope = NULL;
      break;
    case OBJ_PROCEDURE:
      freeScope(obj->procAttrs->scope); // Free scope also free the param list
      obj->procAttrs->scope = NULL;
      break;
    case OBJ_PARAMETER:
      free(obj->paramAttrs->type);
      obj->paramAttrs->type = NULL;
      break;
    default:
      break;
  }
}

void freeScope(Scope* scope) {
  // TODO
  if (scope == NULL) 
    return;

  freeObjectList(scope->objList);
  free(scope);
  scope = NULL;
}

void freeObjectList(ObjectNode *objList) {
  // TODO
  if (objList == NULL) 
    return;

  freeObject(objList->object);
  freeObjectList(objList->next);
  objList = NULL;
}

void freeReferenceList(ObjectNode *objList) {
  // TODO
  if (objList == NULL) 
    return;

  freeObject(objList->object);
  freeReferenceList(objList->next);
  objList = NULL;
}

void addObject(ObjectNode **objList, Object* obj) {
  ObjectNode* node = (ObjectNode*) malloc(sizeof(ObjectNode));
  node->object = obj;
  node->next = NULL;
  if ((*objList) == NULL) 
    *objList = node;
  else {
    ObjectNode *n = *objList;
    while (n->next != NULL) 
      n = n->next;
    n->next = node;
  }
}

Object* findObject(ObjectNode *objList, char *name) {
  // TODO
  ObjectNode* node = objList;
  while (node->next != NULL) {
    if (strcmp(node->object->name, name) == 0)
      return node->object;
    node = node->next;
  }
  return NULL;
}

/******************* others ******************************/

void initSymTab(void) {
  Object* obj;
  Object* param;

  symtab = (SymTab*) malloc(sizeof(SymTab));
  symtab->globalObjectList = NULL;
  
  obj = createFunctionObject("READC");
  obj->funcAttrs->returnType = makeCharType();
  addObject(&(symtab->globalObjectList), obj);

  obj = createFunctionObject("READI");
  obj->funcAttrs->returnType = makeIntType();
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITEI");
  param = createParameterObject("i", PARAM_VALUE, obj);
  param->paramAttrs->type = makeIntType();
  addObject(&(obj->procAttrs->paramList),param);
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITEC");
  param = createParameterObject("ch", PARAM_VALUE, obj);
  param->paramAttrs->type = makeCharType();
  addObject(&(obj->procAttrs->paramList),param);
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITELN");
  addObject(&(symtab->globalObjectList), obj);

  intType = makeIntType();
  charType = makeCharType();
}

void cleanSymTab(void) {
  freeObject(symtab->program);
  freeObjectList(symtab->globalObjectList);
  free(symtab);
  freeType(intType);
  freeType(charType);
}

void enterBlock(Scope* scope) {
  symtab->currentScope = scope;
}

void exitBlock(void) {
  symtab->currentScope = symtab->currentScope->outer;
}

void declareObject(Object* obj) {
  if (obj->kind == OBJ_PARAMETER) {
    Object* owner = symtab->currentScope->owner;
    switch (owner->kind) {
    case OBJ_FUNCTION:
      addObject(&(owner->funcAttrs->paramList), obj);
      break;
    case OBJ_PROCEDURE:
      addObject(&(owner->procAttrs->paramList), obj);
      break;
    default:
      break;
    }
  }
 
  addObject(&(symtab->currentScope->objList), obj);
}


