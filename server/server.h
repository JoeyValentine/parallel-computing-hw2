#ifndef __SERVER__
#define __SERVER__

#include <ctype.h>
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "queue.h"
#include "tpool.h"

#define MAX_BUFF_LEN 65534
#define SERVER_NAME "cse20141510"
#define SERVER_URL "http://www.cspro.sogang.ac.kr"
#define PROTOCOL "HTTP/1.1"
#define RFC1123FMT "%a, %d %b %Y %H:%M:%S GMT"

static void file_details( char *dir, char *name );
static void send_error( int status, char *title, char *extra_header, char *text );
static void send_headers( int status, char *title, char *extra_header, char *mime_type, off_t length, time_t mod );
static char* get_mime_type( char *name );
static void strdecode( char *to, char *from );
static int hexit( char c );
static void strencode( char *to, size_t tosize, const char *from );

#endif

