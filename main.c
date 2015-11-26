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

#include <stdio.h>
#include <unistd.h>
#include <getopt.h>

#include "nym.h"

#ifdef HAVE_CONFIG_H
# include "config.h"
# define VERSION_STRING PACKAGE_STRING
# define PROG PACKAGE
#else
# define VERSION_STRING "nym custom build"
# define PROG "nym"
#endif

#define USAGE_INFO "USAGE: " PROG " [options]\n\n"	\
  PROG "reads from stdin and writes an initialism of it to stdout.\n\n\
OPTIONS:\n\
\t-h, --help\tPrints this help message and exits\n\
\t-v, --version\tPrints version info and exits\n"

#define VERSION_INFO VERSION_STRING "\n\
Copyright (C) 2015 David McMackins II\n\
License AGPLv3: GNU AGPL version 3 only <http://gnu.org/licenses/agpl.html>.\n\
This is libre software: you are free to change and redistribute it.\n\
There is NO WARRANTY, to the extent permitted by law.\n\n\
Written by David McMackins II."

int
main (int argc, char *argv[])
{
  struct option longopts[] =
    {
      {"help",    no_argument, 0, 'h'},
      {"version", no_argument, 0, 'v'},
      {0, 0, 0, 0}
    };

  if (argc > 1)
    {
      int c;
      int longindex;
      while ((c = getopt_long (argc, argv, "hv", longopts, &longindex)) != -1)
	{
	  switch (c)
	    {
	    case 'h':
	      puts (USAGE_INFO);
	      return 0;

	    case 'v':
	      puts (VERSION_INFO);
	      return 0;

	    case '?':
	      return 1;
	    }
	}
    }

  struct nym_buffer *buf = nym_mkbuf ();
  if (!buf)
    {
      fprintf (stderr, "Could not allocate input buffer: %s\n",
	       nym_estring ());
      return nym_errno;
    }

  int c;
  while ((c = getchar ()) != EOF)
    {
      nym_errno = nym_pushbuf (buf, c);
      if (nym_errno)
	goto end;
    }

  char *nym = nym_convert (buf->data);
  if (!nym)
    {
      fprintf (stderr, "Could not nymify: %s\n", nym_estring ());
      goto end;
    }

  puts (nym);
  free (nym);

 end:
  nym_freebuf (buf, true);
  return nym_errno;
}
