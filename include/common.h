#ifndef COMMON_H
#define COMMON_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <curl/curl.h>
#include <sys/socket.h>
#include <sys/stat.h>

#include <switch.h>

#include "file.h"
#include "linked_list.h"
#include "print.h"
#include "array.h"

#define SELECT_DIR	1

#define PRINT_DEBUG printf("\x1b[16;20H");

#endif
