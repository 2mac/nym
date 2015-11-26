/*
 *  nym - Unix-style program for creating acronyms and initialisms
 *  Copyright (C) 2015 David McMackins II
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published by
 *  the Free Software Foundation, version 3 only.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NYM_H
#define NYM_H

#include <stdbool.h>
#include <stdlib.h>

enum nym_err
  {
    NYM_EALLOC = -1,
    NYM_EEMPTY = -2
  };

extern int nym_errno;

const char *
nym_estring (void);

struct nym_buffer
{
  char *data;
  size_t buflen;
  size_t len;
};

struct nym_buffer *
nym_mkbuf (void);

void
nym_freebuf (struct nym_buffer *buf, bool free_data);

int
nym_pushbuf (struct nym_buffer *buf, char c);

char *
nym_convert (const char *src);

#endif
