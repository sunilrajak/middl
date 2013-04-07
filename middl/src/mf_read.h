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

#ifndef MF_READ_H
#define MF_READ_H

/* == Handlers  <#hndl> */

typedef int (*mf_hndl)();
typedef mf_hndl mf_handlers[6];

extern mf_handlers mf_dmp_handlers;

int mf_null_handler();

void mf_sethandlers (mf_handlers new_handlers);
void mf_gethandlers (mf_handlers cur_handlers);

#endif
