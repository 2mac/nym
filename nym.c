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

#include <ctype.h>
#include <string.h>

#include "nym.h"

#define BUFFER_START_SIZE 16

int nym_errno = 0;

const char *
nym_estring ()
{
  switch (nym_errno)
    {
    case NYM_EALLOC:
      return "out of memory";

    case NYM_EEMPTY:
      return "empty input";

    default:
      return NULL;
    }
}

struct nym_buffer *
nym_mkbuf ()
{
  struct nym_buffer *buf = malloc (sizeof (struct nym_buffer));
  if (!buf)
    {
      nym_errno = NYM_EALLOC;
      return NULL;
    }

  buf->data = malloc (BUFFER_START_SIZE * sizeof (char));
  if (!buf->data)
    {
      free (buf);
      nym_errno = NYM_EALLOC;
      return NULL;
    }

  buf->data[0] = '\0';
  buf->buflen = BUFFER_START_SIZE;
  buf->len = 0;

  return buf;
}

void
nym_freebuf (struct nym_buffer *buf, bool free_data)
{
  if (free_data)
    free (buf->data);

  free (buf);
}

int
nym_pushbuf (struct nym_buffer *buf, char c)
{
  if (++buf->len == buf->buflen)
    {
      buf->buflen *= 2;

      char *old = buf->data;
      buf->data = realloc (buf->data, buf->buflen * sizeof (char));
      if (!buf->data)
	{
	  buf->data = old;
	  --buf->len;
	  return NYM_EALLOC;
	}
    }

  buf->data[buf->len - 1] = c;
  buf->data[buf->len] = '\0';

  return 0;
}

static bool
isacro (char c)
{
  return !isspace (c) && !ispunct (c);
}

char *
nym_convert (const char *src)
{
  size_t len = strlen (src);
  if (0 == len)
    {
      nym_errno = NYM_EEMPTY;
      return NULL;
    }

  struct nym_buffer *buf = nym_mkbuf ();
  if (!buf)
    {
      nym_errno = NYM_EALLOC;
      return NULL;
    }

  size_t i;
  for (i = 0; i < len; ++i)
    {
      for (; !isacro (src[i]) && i < len; ++i)
	;

      nym_errno = nym_pushbuf (buf, toupper (src[i]));
      if (nym_errno)
	goto fail;

      for (++i; i < len && isacro (src[i]); ++i)
	;
    }

  char *nym = buf->data;
  nym_freebuf (buf, false);
  return nym;

 fail:
  nym_freebuf (buf, true);
  return NULL;
}
