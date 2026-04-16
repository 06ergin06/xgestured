#include "../includes/header.h"

volatile sig_atomic_t keep_running = 1;

int	open_restricted(const char *path, int flags, void *user_data)
{
	(void)user_data;
	int fd;
	fd = open(path, flags);
	if (fd < 0)
	{
		perror("cannot open");
	}
	return (fd);
}

void	close_restricted(int fd, void *user_data)
{
	(void)user_data;
	close(fd);
}

void graceful_shutdown(int signum)
{
	(void)signum;
	keep_running = 0;
}

struct udev	*udev_create()
{
	struct udev *udev;
	udev = udev_new();
	if (!udev)
	{
		perror("error : cannot create udev");
		exit(1);
	}
	return udev;
}

int	load_config(struct s_config *config)
{
	char config_path[256];

	free(config->swipe_up_3);
	config->swipe_up_3 = NULL;
	free(config->swipe_down_3);
	config->swipe_down_3 = NULL;
	free(config->swipe_left_3);
	config->swipe_left_3 = NULL;
	free(config->swipe_right_3);
	config->swipe_right_3 = NULL;
	free(config->swipe_up_4);
	config->swipe_up_4 = NULL;
	free(config->swipe_down_4);
	config->swipe_down_4 = NULL;
	free(config->swipe_left_4);
	config->swipe_left_4 = NULL;
	free(config->swipe_right_4);
	config->swipe_right_4 = NULL;

	snprintf(config_path, sizeof(config_path), CONFIG_PATH);

	if (ini_parse(config_path, config_handler, config) < 0)
	{
		perror("Cannot read config file");
		exit (1);
	}
	else
	{
		printf("sucess : readed config file \n");
		return 0;
	}
}

double	ft_fabs(double x)
{
	if (x < 0)
		x = -x;
	return (x);
}

void	run_command(char *command)
{
	if (!command || command[0] == '\0')
	{
		perror("command not found");
		return ;
	}
	pid_t pid = fork();

	if (pid == -1)
	{
		perror("cannot fork");
		return ;
	}
	if (pid == 0)
	{
		execl("/bin/sh", "sh", "-c", command, (char *)NULL);
		perror("cannot run command");
		exit(1);
	}
}

static char *safe_strdup(const char *s)
{
	char *d;

	if (!s)
		return (NULL);
	d = strdup(s);
	if (!d)
	{
		perror("Fatal error: Out of memory");
		exit(EXIT_FAILURE);
	}
	return (d);
}

int	config_handler(void *user_data, const char *section, const char *name, const char *value)
{
	struct s_config *config = (struct s_config *)user_data;

	if (strcmp(section, "gestures_3") == 0)
	{
		if (strcmp(name, "swipe_up_3") == 0)
			config->swipe_up_3 = safe_strdup(value);
		else if (strcmp(name, "swipe_down_3") == 0)
			config->swipe_down_3 = safe_strdup(value);
		else if (strcmp(name, "swipe_left_3") == 0)
			config->swipe_left_3 = safe_strdup(value);
		else if (strcmp(name, "swipe_right_3") == 0)
			config->swipe_right_3 = safe_strdup(value);
	}
	else if (strcmp(section, "gestures_4") == 0)
	{
		if (strcmp(name, "swipe_up_4") == 0)
			config->swipe_up_4 = safe_strdup(value);
		else if (strcmp(name, "swipe_down_4") == 0)
			config->swipe_down_4 = safe_strdup(value);
		else if (strcmp(name, "swipe_left_4") == 0)
			config->swipe_left_4 = safe_strdup(value);
		else if (strcmp(name, "swipe_right_4") == 0)
			config->swipe_right_4 = safe_strdup(value);
	}
	return (1);
}

void	gesture_command_run(double total_dx, double total_dy, int finger_count, struct s_config config)
{
	if (ft_fabs(total_dy) > ft_fabs(total_dx))
	{
		if (total_dy < 0)
		{
			printf("fingers up \n");
			if (finger_count == 3)
				run_command(config.swipe_up_3);
			else if (finger_count == 4)
				run_command(config.swipe_up_4);
		}
		else
		{
			printf("fingers down \n");
			if (finger_count == 3)
				run_command(config.swipe_down_3);
			else if (finger_count == 4)
				run_command(config.swipe_down_4);
		}
	}
	else
	{
		if (total_dx < 0)
		{
			printf("fingers left \n");
			if (finger_count == 3)
				run_command(config.swipe_left_3);
			else if (finger_count == 4)
				run_command(config.swipe_left_4);
		}
		else
		{
			printf("fingers right \n");
			if (finger_count == 3)
				run_command(config.swipe_right_3);
			else if (finger_count == 4)
				run_command(config.swipe_right_4);
		}
	}
}
