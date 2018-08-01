#include <conio.h>
#include <ctype.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
  char *name;
  struct node *lchild;
  struct node *rchild;
};
int istype(char buffer[]);
int isKeyword(char buffer[]);
int isoperator(int c);
int isspecialc(int c);
int isrelop(int c, int prechar);
int iscomment(int c, int prechar);
void display(struct node *ptr);
void store(char *str, char *token, struct node **root);
void find(char *str, struct node **par, struct node **loc, struct node **root);

int main(int argc, char const *argv[]) {
  struct node *id = NULL;
  struct node *keyword = NULL;
  struct node *numberic = NULL;

  char lexemes[150][30], tokens[150][30], dump[2], bufferc[30], buffern[30];
  char type[][20] = {"identifier", "keyword", "numberic constant"};
  int c, jc = 0, jn = 0, index = 1, lastchar;
  FILE *fp;

  if ((fp = fopen("code.txt", "r")) == NULL)
    printf("cat: can't open s\n");

  while ((c = getc(fp)) != EOF) {
    if ((isdigit(c) || c == '.') && jc == 0) {
      buffern[jn++] = c;
      continue;
    } else if ((!isdigit(c)) && (jn != 0)) {
      lastchar = buffern[jn - 1];
      buffern[jn] = '\0';
      jn = 0;

      store(buffern, type[2], &numberic);
    }

    if ((isalnum(c) || c == '_')) {
      bufferc[jc++] = c;
      continue;
    } else if ((c == ' ' || c == '\n' || c == '\t' || isoperator(c) ||
                isspecialc(c)) &&
               (jc != 0)) {
      lastchar = bufferc[jc - 1];
      bufferc[jc] = '\0';
      jc = 0;

      if (isKeyword(bufferc))
        store(bufferc, type[1], &keyword);
      else
        store(bufferc, type[0], &id);
    }

    if (isoperator(c)) {
      dump[0] = c;
      dump[1] = '\0';

      switch (iscomment(c, lastchar)) {
      case 1:
        index--;
        bufferc[0] = '/';
        bufferc[1] = '*';
        jc = 2;
        while ((c = getc(fp)) != '/' && (bufferc[jc - 1] != '*' || jc == 2))
          bufferc[jc++] = c;

        bufferc[jc++] = '/';
        bufferc[jc] = '\0';
        jc = 0;
        printf("%s is multi line comment\n", bufferc);
        break;
      case 2:
        index--;
        bufferc[0] = '/';
        bufferc[1] = '/';
        jc = 2;
        while ((c = getc(fp)) != '\n')
          bufferc[jc++] = c;

        bufferc[jc] = '\0';
        jc = 0;
        printf("%s is single line comment\n", bufferc);
        break;
      default:
        if (isrelop(c, lastchar) == 1) {
          strcpy(lexemes[index], dump);
          strcpy(tokens[index++], "relational operator");
          lastchar = c;
          printf("%s is comparesion operator \n", dump);

        } else if (isrelop(c, lastchar) == 2) {
          dump[0] = lastchar;
          dump[1] = c;
          dump[2] = '\0';
          index--;
          strcpy(lexemes[index], dump);
          strcpy(tokens[index++], "relational operator");
          lastchar = c;
          printf("%s is comparesion operator \n", dump);
          continue;
        } else {
          strcpy(lexemes[index], dump);
          strcpy(tokens[index++], "operator");
          lastchar = c;
          printf("%s is operator\n", dump);
          continue;
        }
      }
    }
    if (isspecialc(c)) {
      dump[0] = c;
      dump[1] = '\0';
      lastchar = c;
      strcpy(lexemes[index], dump);
      strcpy(tokens[index++], "special char");
      printf("%s is special char\n", dump);
      continue;
    }
  }
  printf("\nidentifier  ");
  display(id);
  printf("\nkeyword  ");
  display(keyword);
  printf("\nnumberic constant ");
  display(numberic);

  // system("pause");
  return 0;
}

int istype(char buffer[]) {
  if (strcmp(buffer, "int") || strcmp(buffer, "float"))
    return 1;
  return 0;
}

int isKeyword(char buffer[]) {

  char keywords[][10] = {
      "int",      "float",    "case",  "char",     "const",  "do",     "double",
      "else",     "float",    "for",   "while",    "if",     "int",    "long",
      "register", "return",   "short", "sizeof",   "static", "struct", "switch",
      "union",    "unsigned", "void",  "volatile", "include"};
  int len = sizeof(keywords) / sizeof(keywords[0]);
  int i;

  for (i = 0; i < len; i++) {
    if (!strcmp(keywords[i], buffer)) {
      return 1;
    }
  }

  return 0;
}
int isrelop(int c, int prechar) {
  if (((c == '=') &&
       (prechar == '!' || prechar == '>' || prechar == '<' || prechar == '=')))
    return 2;
  if (c == '<' || c == '>')
    return 1;
  return 0;
}
int iscomment(int c, int prechar) {
  if (c == '*' && prechar == '/') {
    return 1;
  } else if (c == '/' && prechar == '/') {
    return 2;
  }

  return 0;
}

int isoperator(int c) {

  char operators[] = "+-*/%=<>";
  int i;
  for (i = 0; i < strlen(operators); i++)
    if (c == operators[i])
      return 1;
  return 0;
}

int isspecialc(int c) {

  char specialc[] = "#;,(){}!";
  int i;
  for (i = 0; i < strlen(specialc); i++)
    if (c == specialc[i])
      return 1;

  return 0;
}

////////////////////////////////////// tree functions
////////////////////////////////////////////

void find(char *str, struct node **par, struct node **loc, struct node **root) {
  struct node *ptr, *ptrsave = NULL;
  if (*root == NULL) {
    *loc = NULL;
    *par = NULL;
    return;
  }
  ptr = *root;
  while (ptr != NULL) {
    if (!(strcmp(str, ptr->name))) {
      *loc = ptr;
      *par = ptrsave;
      return;
    }
    ptrsave = ptr;
    if (strcmp(str, ptr->name) < 0)
      ptr = ptr->lchild;
    else
      ptr = ptr->rchild;
  }
  *loc = NULL;
  *par = ptrsave;
}

void store(char *str, char *token, struct node **root) {
  struct node *parent, *location, *temp;
  find(str, &parent, &location, root);
  if (location != NULL) {
    printf("%s > %s already Exists\n", str, token);
    return;
  } else {
    printf("%s is %s\n", str, token);
  }
  temp = (struct node *)malloc(sizeof(struct node));
  temp->name = strdup(str);
  temp->lchild = NULL;
  temp->rchild = NULL;
  if (*root == NULL)
    *root = temp;
  else if (strcmp(str, parent->name) < 0)
    parent->lchild = temp;
  else
    parent->rchild = temp;
}

void display(struct node *ptr) {
  static int flag = 1;
  if (ptr == NULL && flag) {
    printf("Tree is empty");
    return;
  } else if (ptr == NULL)
    return;
  else {
    flag = 0;
    display(ptr->lchild);
    printf("%s -> ", ptr->name);
    display(ptr->rchild);
  }
}
