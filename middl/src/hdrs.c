/*
**  (C) 2008 by Remo Dentato (rdentato@users.sourceforge.net)
**
** Permission to use, copy, modify and distribute this code and
** its documentation for any purpose is hereby granted without
** fee, provided that the above copyright notice, or equivalent
** attribution acknowledgement, appears in all copies and
** supporting documentation.
**
** Copyright holder makes no representations about the suitability
** of this software for any purpose. It is provided "as is" without
** express or implied warranty.
*/

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>

char buf[2048];

/* Checks for "#ifdef MSQ_H" allowing multiple spaces */
int chk_start(char *s)
{
  int k=0;
  
  while (isspace(*s)) s++;
  if (*s++ != '#') return 0;
  
  while (isspace(*s)) s++;
  
  for (k=0; *s == "ifdef"[k]; s++, k++) ;
  if (k<5) return 0;
  
  for (k=0; isspace(*s); s++,k++);
  if (k==0) return 0;
  for(k=0; *s == "MIDDL_H"[k]; s++, k++) ;
  if (k<5) return 0;
  return 1;
}

/* Checks for "#endif /* MSQ_H *] allowing multiple spaces
                                ^--this is a /
*/

int chk_end(char *s)
{
  int k=0;
  
  while (isspace(*s)) s++;
  if (*s++ != '#') return 0;
  
  while (isspace(*s)) s++;
  
  for (k=0; *s == "endif"[k]; s++, k++) ;
  if (k<5) return 0;
  
  while (isspace(*s)) s++;

  if (*s++ != '/') return 0;
  if (*s++ != '*') return 0;
  
  while (isspace(*s)) s++;

  for(k=0; *s == "MIDDL_H"[k]; s++, k++) ;
  if (k<5) return 0;
  
  while (isspace(*s)) s++;

  if (*s++ != '*') return 0;
  if (*s++ != '/') return 0;
  
  return 1;
}

int main(int argc, char *argv[])
{
  int print_it = 0;
  int k;
  FILE *f;
  
  printf("/*\n");
  printf("**  (C) by Remo Dentato (rdentato@users.sourceforge.net)\n");
  printf("**\n");
  printf("** Permission to use, copy, modify and distribute this code and\n");
  printf("** its documentation for any purpose is hereby granted without\n");
  printf("** fee, provided that the above copyright notice, or equivalent\n");
  printf("** attribution acknowledgement, appears in all copies and\n");
  printf("** supporting documentation.\n");
  printf("**\n");
  printf("** Copyright holder makes no representations about the suitability\n");
  printf("** of this software for any purpose. It is provided \"as is\" without\n");
  printf("** express or implied warranty.\n");
  printf("*/\n");

  printf("\n/** THIS IS A GENERATED FILE - DO NOT MODIFY **/\n\n");
  
  printf("#ifndef MIDDL_H\n");
  printf("#define MIDDL_H\n");
  
  for (k = 1; k < argc; k++) {
    f = fopen(argv[k],"r");
    if (f == NULL) {
      fprintf(stderr,"ERROR - File not found \"%s\"\n",argv[k]);
      exit(1);
    }

    print_it = 0;
    while (fgets(buf,2048,f)) {
      if (print_it) {
        if (chk_end(buf)) break;
        fputs(buf,stdout);
      }
      print_it = print_it || chk_start(buf);
    }
    fclose(f);
  }
  printf("#endif\n");
  exit(0);
}

