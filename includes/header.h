#include <fcntl.h>
#include <libinput.h>
#include <libudev.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include <signal.h>
#include "ini.h"
#include <sys/inotify.h>

struct s_config
{
	char	*swipe_up_3;
	char	*swipe_down_3;
	char	*swipe_left_3;
	char	*swipe_right_3;
	char	*swipe_up_4;
	char	*swipe_down_4;
	char	*swipe_left_4;
	char	*swipe_right_4;
};


int			open_restricted(const char *path, int flags, void *user_data);
void		close_restricted(int fd, void *user_data);
struct udev	*udev_create();
int			load_config(struct s_config *config);
void		run_command(char *command);
int			config_handler(void *user_data, const char *section, const char *name, const char *value);
double		ft_fabs(double x);
void		gesture_command_run(double total_dx, double total_dy, int finger_count, struct s_config config);