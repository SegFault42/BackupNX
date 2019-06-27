#include "common.h"

size_t	count_2d_array(char **array)
{
	size_t	idx = 0;

	while (array[idx]) {
		idx++;
	}

	return (idx);
}

/*void	free_2d_array(char **array)*/
/*{*/
		/*PRINT_DEBUG*/
		/*printf("\n%p", (void *)&array);*/
	/*[>for (size_t idx = 0; array[idx]; idx++) {<]*/
		/*[>free(array[idx]);<]*/
		/*[>[>array[idx] = NULL;<]<]*/
	/*[>}<]*/

	/*[>free(array);<]*/
	/*[>array = NULL;<]*/
/*}*/

void	print_2d_array(char **array)
{
	if (array == NULL) {
		printf("NULL");
		return ;
	}

	printf("[ ");

	for (int idx = 0; array[idx]; idx++) {
		if (array[idx +1] == NULL) {
			printf("[ %s ] ", array[idx]);
		} else {
			printf("[ %s ], ", array[idx]);
		}
	}

	printf(" ]");
}
