#include "header.h"

double ft_fabs(double x)
{
	if (x < 0)
		x = -x;
	return (x);
}
void run_command(char *command)
{
	if(!command || command[0] == '\0')
		fprintf(stderr,"command not found\n");

	pid_t pid = fork();

	if(pid == -1)
	{
		fprintf(stderr, "cannot fork\n");
		return ;
	}
	if(pid == 0)
	{
		execl("/bin/sh", "sh", "-c", command, (char*) NULL);
		fprintf(stderr,"cannot run command\n");
		exit(1);
	}
}
int config_handler(void *user_data, const char *section, const char *name, const char *value)
{
	struct s_config *config = (struct s_config *)user_data;

	if (strcmp(section, "gestures_3") == 0)
	{
		if (strcmp(name, "swipe_up_3") == 0)
		{
			config->swipe_up_3 = strdup(value);
		}
		else if (strcmp(name, "swipe_down_3") == 0)
		{
			config->swipe_down_3 = strdup(value);
		}
		else if (strcmp(name, "swipe_left_3") == 0)
		{
			config->swipe_left_3 = strdup(value);
		}
		else if (strcmp(name, "swipe_right_3") == 0)
		{
			config->swipe_right_3 = strdup(value);
		}
	}
	else if (strcmp(section, "gestures_4") == 0)
	{
		if (strcmp(name, "swipe_up_4") == 0)
		{
			config->swipe_up_4 = strdup(value);
		}
		else if (strcmp(name, "swipe_down_4") == 0)
		{
			config->swipe_down_4 = strdup(value);
		}
		else if (strcmp(name, "swipe_left_4") == 0)
		{
			config->swipe_left_4 = strdup(value);
		}
		else if (strcmp(name, "swipe_right_4") == 0)
		{
			config->swipe_right_4 = strdup(value);
		}
	}

	return 1;
}
