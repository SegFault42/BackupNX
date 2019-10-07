#ifndef INPUT_H
#define INPUT_H

typedef struct	s_cred
{
	char		*cloud_provider;
	char		*api_key;
	char		**files;
}				t_cred;

void	move_down(t_files *s_files);
void	move_up(t_files *s_files);
bool	input(u64 kDown, t_files *s_files);

#endif
