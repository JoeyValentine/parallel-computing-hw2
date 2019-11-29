#include "client.h"

/* shared variables */
double interval, max_interval;
int port_num, n_thread, n_file;
int n_access, opt_val = 1;
char interval_opt[10] = {0,};
char ip_address[MAX_HOST_NAME_LEN] = {0,};
char file_name_list[MAX_FILE_NUM][MAX_FILE_NAME_LEN];

int 
main(int argc, char *argv[])
{
  pthread_t *threads;
  double start, finish, elapsed;

  printf("Ip address : ");
  scanf("%s", ip_address);
  printf("%s\n", ip_address);

  printf("Port number : ");
  scanf("%d%*c", &port_num);

  printf("The number of threads : ");
  scanf("%d", &n_thread);

  threads = malloc(n_thread * sizeof(pthread_t));

  printf("How many times each thread accesses your server : ");
  scanf("%d", &n_access);

  printf("Interval option (random / uniform) : ");
  scanf("%s", interval_opt);

  if (strcmp(interval_opt, "random") == 0)
  {
    printf("Max interval : ");
    scanf("%lf", &max_interval);
  }
  else if (strcmp(interval_opt, "uniform") == 0)
  {
    printf("Interval : ");
    scanf("%lf", &interval);
  }
  else
  {
    printf("Error : invalid interval option");
    exit(EXIT_FAILURE);
  }

  gen_file_name_list(argv[1]);

  GET_TIME(start);
  if (strcmp(interval_opt, "random") == 0)
  {
    for (int i = 0; i < n_thread; ++i)
      pthread_create(&threads[i], NULL, random_traffic_gen, NULL);
  }
  else if (strcmp(interval_opt, "uniform") == 0)
  {
    for (int i = 0; i < n_thread; ++i)
      pthread_create(&threads[i], NULL, uniform_traffic_gen, NULL);
  }
  
  for (int i = 0; i < n_thread; ++i)
    pthread_join(threads[i], NULL);

  GET_TIME(finish);

  elapsed = finish - start;
  printf("\nThe code to be timed took %e seconds\n", elapsed);
  
  free(threads);

  return 0;
}

void 
gen_file_name_list(const char *config_file_name)
{
  int i = 0;
  char read_buff[MAX_FILE_NAME_LEN] = {0,};
  FILE *fp = fopen(config_file_name, "r");

  if (fp == NULL)
  {
    printf("Cannot open %s\n", config_file_name);
    exit(EXIT_FAILURE);
  }
  
  while (fgets(read_buff, sizeof(read_buff), fp))
  {
    strcpy(file_name_list[i], read_buff);
    ++i;
    ++n_file;
  }
  
  fclose(fp);
}

void* 
uniform_traffic_gen(void *data)
{
    int n_bytes_write = 0, cli_fd, file_idx = 0;
    char buff[MAX_BUFF_LEN] = {0,};
    struct sockaddr_in serv_addr;
    
    memset((char*)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip_address);
    serv_addr.sin_port = htons(port_num);

    for (int i = 0; i < n_access; ++i)
    {
      if ((cli_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
      {
        printf("Error : can't open stream socket!\n");
        exit(EXIT_FAILURE);
      }

      if (connect(cli_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
      {
        printf("Error : can't connect to server\n");
        close(cli_fd);
        exit(EXIT_FAILURE);   
      }

      sprintf(buff, "%d GET %s HTTP/1.1\n", cli_fd, file_name_list[file_idx]);
      file_idx = (file_idx + 1) % n_file;

      n_bytes_write = write(cli_fd, buff, sizeof(buff));

      if (n_bytes_write == -1)
      {
         printf("Error : write error\n");
         close(cli_fd);
         exit(EXIT_FAILURE);
      }

      close(cli_fd);
      sleep(interval);
    }

    return NULL;
}

void* 
random_traffic_gen(void *data)
{
    double sleep_time;
    unsigned int seed_val = time(NULL);
    int n_bytes_write = 0, cli_fd, file_idx = 0;
    char buff[MAX_BUFF_LEN] = {0,};
    struct sockaddr_in serv_addr;
    
    memset((char*)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip_address);
    serv_addr.sin_port = htons(port_num);

    for (int i = 0; i < n_access; ++i)
    {
      if ((cli_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
      {
        printf("Error : can't open stream socket!\n");
        exit(EXIT_FAILURE);
      }

      if (connect(cli_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
      {
        printf("Error : can't connect to server\n");
        close(cli_fd);
        exit(EXIT_FAILURE);   
      }

      sprintf(buff, "%d GET %s HTTP/1.1\n", cli_fd, file_name_list[file_idx]);
      file_idx = (file_idx + 1) % n_file;

      n_bytes_write = write(cli_fd, buff, sizeof(buff));

      if (n_bytes_write == -1)
      {
         printf("Error : write error\n");
         close(cli_fd);
         exit(EXIT_FAILURE);
      }

      close(cli_fd);

      sleep_time = my_drand(&seed_val);
      sleep(sleep_time);
    }

    return NULL;
}
