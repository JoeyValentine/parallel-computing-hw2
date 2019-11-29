#ifndef __CLIENT__
#define __CLIENT__

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "timer.h"
#include "my_rand.h"

#define MAX_BUFF_LEN 65534
#define MAX_HOST_NAME_LEN 512
#define MAX_FILE_NUM 65535
#define MAX_FILE_NAME_LEN 512

void gen_file_name_list(const char *config_file_name);
void* uniform_traffic_gen(void *data);
void* random_traffic_gen(void *data);

#endif

