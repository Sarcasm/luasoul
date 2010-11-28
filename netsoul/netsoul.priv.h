/*
 * netsoul.priv.h for libnetsoul
 * 
 * Copyright © 2010 Guillaume Papin <guillaume.papin@epitech.eu>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _LIBNETSOUL_PRIV_H_
#define _LIBNETSOUL_PRIV_H_

#define         SOCK_MSG_SIZE 4096

char           *netsoul_recv(int sockfd);
int             netsoul_send(int sockfd, const char *fmt, ...);

/* utils */
char            *netsoul_url_decode(char *s);
char            *netsoul_url_encode(const char *s);
char            *netsoul_md5sum(const char *fmt, ...);

#endif /* _LIBNETSOUL_PRIV_H_ */








