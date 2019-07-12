#ifndef PRINT_H
#define PRINT_H

#include "file.h"

#define HEADER_NB_LINE		3
#define FOOTER_NB_LINE		3
#define CONSOLE_MAX_LINE	45
#define MAX_LINE			CONSOLE_MAX_LINE - (HEADER_NB_LINE + FOOTER_NB_LINE)

// how many caracter can be printed until end of line (for filename)
#define MAX_LINE_PRINT	74

void	print_directory(t_files_select *begin, size_t cursor);
void	print_header(void);
void	print_footer(void);

#endif
