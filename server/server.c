#include "server.h"

/* Shared variables */
Queue *q;
pthread_t *threads;
pthread_cond_t is_q_empty;
pthread_rwlock_t rw_lock;

int
main( int argc, char **argv )
{
    char method[10000], path[10000], protocol[10000], idx[20000], location[20000];
    char *file;
    size_t len;
    int ich;
    struct stat sb;
    FILE *fp;
    struct dirent **dl;
    int i, n;
    
    if ( argc != 2 )
        send_error( 500, "Internal Error", (char*) 0, "Config error - no dir specified." );
    if ( chdir( argv[1] ) < 0 )
        send_error( 500, "Internal Error", (char*) 0, "Config error - couldn't chdir()." );

    char buff[MAX_BUFF_LEN];
    int port_num, n_thread, opt_val = 1;
    int serv_fd, cli_fd, n_bytes_read = 0;
    struct sockaddr_in cli_addr, serv_addr;

    if ((serv_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
      printf("Error : Can't open stream socket!\n");
      exit(EXIT_FAILURE);
    }

    printf("Port number : ");
    scanf("%d", &port_num);

    printf("Thread number : \n");
    scanf("%d", &n_thread);

    q = gen_queue(); 
    init_thread_pool(n_thread);

    printf("%d\n", n_thread);

    for (int i = 0; i < n_thread; ++i)
      pthread_create(&threads[i], NULL, work, NULL);

    memset((char*)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port_num);

    printf("%d\n", n_thread);

    if (setsockopt(serv_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
        (char*)&opt_val, sizeof(opt_val)) == -1)
    {
      printf("Error : setsockopt failed\n");
      close(serv_fd);
      exit(EXIT_FAILURE);
    }

    if (bind(serv_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
      printf("Error : unable to bind socket in server\n");
      close(serv_fd);
      exit(EXIT_FAILURE);
    }

    printf("%d\n", n_thread);

    listen(serv_fd, 5);

    printf("%d\n", n_thread);

    while (true)
    {
      cli_fd = accept(serv_fd, (struct sockaddr *)&cli_addr, &cli_addr);
     
      if (cli_fd < 0)
      {
         printf("Error : connection to client failed in server\n");
         close(serv_fd);
         continue;
      }

      n_bytes_read = read(cli_fd, buff, sizeof(buff));

      pthread_rwlock_wrlock(&rw_lock);
      push(q, gen_elem(buff));
      pthread_rwlock_unlock(&rw_lock);
      
      printf("what\n");
      print_q(q);
      printf("the\n");

/*
      if (n_bytes_read == 0)
        send_error( 400, "Bad Request", (char*) 0, "No request found." );

      if ( sscanf( buff, "%[^ ] %[^ ] %[^ ]", method, path, protocol ) != 3 )
        send_error( 400, "Bad Request", (char*) 0, "Can't parse request." );

      if (n_bytes_read == -1)
        send_error( 400, "Bad Request", (char*) 0, "No request found." );

      if ( strcasecmp( method, "get" ) != 0 )
        send_error( 501, "Not Implemented", (char*) 0, "That method is not implemented." );
      if ( path[0] != '/' )
        send_error( 400, "Bad Request", (char*) 0, "Bad filename." );
      file = &(path[1]);
      strdecode( file, file );
      if ( file[0] == '\0' )
        file = "./";
      len = strlen( file );
      
      if ( file[0] == '/' || strcmp( file, ".." ) == 0 || 
           strncmp( file, "../", 3 ) == 0 || 
           strstr( file, "/../" ) != (char*) 0 || 
           strcmp( &(file[len-3]), "/.." ) == 0 )
        send_error( 400, "Bad Request", (char*) 0, "Illegal filename." );
      if ( stat( file, &sb ) < 0 )
        send_error( 404, "Not Found", (char*) 0, "File not found." );
      if ( S_ISDIR( sb.st_mode ) )
      {
        if ( file[len-1] != '/' )
        {
          (void) snprintf( location, sizeof(location), "Location: %s/", path );
          send_error( 302, "Found", location, "Directories must end with a slash." );
        }
        (void) snprintf( idx, sizeof(idx), "%sindex.html", file );
        if ( stat( idx, &sb ) >= 0 )
        {
          file = idx;
          goto do_file;
        }
        send_headers( 200, "Ok", (char*) 0, "text/html", -1, sb.st_mtime );
        (void) printf( "\
<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\">\n\
<html>\n\
  <head>\n\
    <meta http-equiv=\"Content-type\" content=\"text/html;charset=UTF-8\">\n\
    <title>Index of %s</title>\n\
  </head>\n\
  <body bgcolor=\"#99cc99\">\n\
    <h4>Index of %s</h4>\n\
    <pre>\n", file, file );
        n = scandir( file, &dl, NULL, alphasort );
        if ( n < 0 )
            perror( "scandir" );
        else
            for ( i = 0; i < n; ++i )
                file_details( file, dl[i]->d_name );
        (void) printf( "\
    </pre>\n\
    <hr>\n\
    <address><a href=\"%s\">%s</a></address>\n\
  </body>\n\
</html>\n", SERVER_URL, SERVER_NAME );
      }
      else
      {
        do_file:
          fp = fopen( file, "r" );
          if ( fp == (FILE*) 0 )
            send_error( 403, "Forbidden", (char*) 0, "File is protected." );
          send_headers( 200, "Ok", (char*) 0, get_mime_type( file ), sb.st_size, sb.st_mtime );
          while ( ( ich = getc( fp ) ) != EOF )
            putchar( ich );
      }    
      close(cli_fd);
*/
    }
    
    destroy_q(q);

    for (int i = 0; i < n_thread; ++i)
      pthread_join(threads[i], NULL);

    free_thread_pool();

    close(serv_fd);
    (void) fflush( stdout );
    exit( 0 );
}

void* 
work(void *param)
{
  while (true)
  {
    pthread_rwlock_wrlock(&rw_lock);
    
    while (is_empty(q))
      pthread_cond_wait(&is_q_empty, &rw_lock);
       
    QElem *elem = front(q);
    pop(q);

    pthread_rwlock_unlock(&rw_lock);

    printf("wawa %s\n", elem->path);

    free(elem);    
  }

  pthread_exit(0);
}

static void
file_details( char *dir, char *name )
{
    static char encoded_name[1000];
    static char path[2000];
    struct stat sb;
    char timestr[16];
    
    strencode( encoded_name, sizeof(encoded_name), name );
    (void) snprintf( path, sizeof(path), "%s/%s", dir, name );
    if ( lstat( path, &sb ) < 0 )
        (void) printf( "<a href=\"%s\">%-32.32s</a>    ???\n", encoded_name, name );
    else
    {
        (void) strftime( timestr, sizeof(timestr), "%d%b%Y %H:%M", localtime( &sb.st_mtime ) );
        (void) printf( "<a href=\"%s\">%-32.32s</a>    %15s %14lld\n", encoded_name, name, timestr, (long long) sb.st_size );
    }
}

static void
send_error( int status, char *title, char *extra_header, char *text )
{
    send_headers( status, title, extra_header, "text/html", -1, -1 );
    (void) printf( "\
<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\">\n\
<html>\n\
  <head>\n\
    <meta http-equiv=\"Content-type\" content=\"text/html;charset=UTF-8\">\n\
    <title>%d %s</title>\n\
  </head>\n\
  <body bgcolor=\"#cc9999\">\n\
    <h4>%d %s</h4>\n", status, title, status, title );
    (void) printf( "%s\n", text );
    (void) printf( "\
    <hr>\n\
    <address><a href=\"%s\">%s</a></address>\n\
  </body>\n\
</html>\n", SERVER_URL, SERVER_NAME );
    (void) fflush( stdout );
// exit( 1 );
}


static void
send_headers( int status, char *title, char *extra_header, char *mime_type, off_t length, time_t mod )
{
    time_t now;
    char timebuf[100];
    
    (void) printf( "%s %d %s\015\012", PROTOCOL, status, title );
    (void) printf( "Server: %s\015\012", SERVER_NAME );
    now = time( (time_t*) 0 );
    (void) strftime( timebuf, sizeof(timebuf), RFC1123FMT, gmtime( &now ) );
    (void) printf( "Date: %s\015\012", timebuf );
    if ( extra_header != (char*) 0 )
        (void) printf( "%s\015\012", extra_header );
    if ( mime_type != (char*) 0 )
        (void) printf( "Content-Type: %s\015\012", mime_type );
    if ( length >= 0 )
        (void) printf( "Content-Length: %lld\015\012", (long long) length );
    if ( mod != (time_t) -1 )
    {
        (void) strftime( timebuf, sizeof(timebuf), RFC1123FMT, gmtime( &mod ) );
        (void) printf( "Last-Modified: %s\015\012", timebuf );
    }
    (void) printf( "Connection: close\015\012" );
    (void) printf( "\015\012" );
}


static char*
get_mime_type( char *name )
{
    char* dot;
    
    dot = strrchr( name, '.' );
    if ( dot == (char*) 0 )
        return "text/plain; charset=UTF-8";
    if ( strcmp( dot, ".html" ) == 0 || strcmp( dot, ".htm" ) == 0 )
        return "text/html; charset=UTF-8";
    if ( strcmp( dot, ".xhtml" ) == 0 || strcmp( dot, ".xht" ) == 0 )
        return "application/xhtml+xml; charset=UTF-8";
    if ( strcmp( dot, ".jpg" ) == 0 || strcmp( dot, ".jpeg" ) == 0 )
        return "image/jpeg";
    if ( strcmp( dot, ".gif" ) == 0 )
        return "image/gif";
    if ( strcmp( dot, ".png" ) == 0 )
        return "image/png";
    if ( strcmp( dot, ".css" ) == 0 )
        return "text/css";
    if ( strcmp( dot, ".xml" ) == 0 || strcmp( dot, ".xsl" ) == 0 )
        return "text/xml; charset=UTF-8";
    if ( strcmp( dot, ".au" ) == 0 )
        return "audio/basic";
    if ( strcmp( dot, ".wav" ) == 0 )
        return "audio/wav";
    if ( strcmp( dot, ".avi" ) == 0 )
        return "video/x-msvideo";
    if ( strcmp( dot, ".mov" ) == 0 || strcmp( dot, ".qt" ) == 0 )
        return "video/quicktime";
    if ( strcmp( dot, ".mpeg" ) == 0 || strcmp( dot, ".mpe" ) == 0 )
        return "video/mpeg";
    if ( strcmp( dot, ".vrml" ) == 0 || strcmp( dot, ".wrl" ) == 0 )
        return "model/vrml";
    if ( strcmp( dot, ".midi" ) == 0 || strcmp( dot, ".mid" ) == 0 )
        return "audio/midi";
    if ( strcmp( dot, ".mp3" ) == 0 )
        return "audio/mpeg";
    if ( strcmp( dot, ".ogg" ) == 0 )
        return "application/ogg";
    if ( strcmp( dot, ".pac" ) == 0 )
        return "application/x-ns-proxy-autoconfig";
    return "text/plain; charset=UTF-8";
}


static void
strdecode( char *to, char *from )
{
    for ( ; *from != '\0'; ++to, ++from )
    {
        if ( from[0] == '%' && isxdigit( from[1] ) && isxdigit( from[2] ) )
        {
            *to = hexit( from[1] ) * 16 + hexit( from[2] );
            from += 2;
        }
        else
            *to = *from;
    }
    *to = '\0';
}


static int
hexit( char c )
{
    if ( c >= '0' && c <= '9' )
        return c - '0';
    if ( c >= 'a' && c <= 'f' )
        return c - 'a' + 10;
    if ( c >= 'A' && c <= 'F' )
        return c - 'A' + 10;
    return 0;		/* shouldn't happen, we're guarded by isxdigit() */
}


static void
strencode( char *to, size_t tosize, const char *from )
{
    int tolen;
    
    for ( tolen = 0; *from != '\0' && tolen + 4 < tosize; ++from )
    {
        if ( isalnum(*from) || strchr( "/_.-~", *from ) != (char*) 0 )
        {
            *to = *from;
            ++to;
            ++tolen;
        }
        else
        {
            (void) sprintf( to, "%%%02x", (int) *from & 0xff );
            to += 3;
            tolen += 3;
        }
    }
    *to = '\0';
}
