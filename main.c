#include "header.h"

int open_restricted(const char *path, int flags, void *user_data)
{
	int fd;

	fd = open(path, flags);
	if (fd < 0)
	{
		perror("cannot open");
	}
	return (fd);
}

void close_restricted(int fd, void *user_data)
{
	close(fd);
}

struct libinput_interface interface = {
	.open_restricted = open_restricted,
	.close_restricted = close_restricted,
};

int main(void)
{
	struct libinput *li;
	struct libinput_event *event;
	enum libinput_event_type type;
	struct libinput_event_gesture *gesture_event;
	int finger_count;
	double total_dx;
	double total_dy;
	struct udev *udev;
	struct s_config config = {0};
	char config_path[256];
	struct pollfd fds;
	int libinput_fd;

	// snprintf(config_path, sizeof(config_path), "%s/.config/jestapp/config.ini", getenv("HOME"));
	snprintf(config_path, sizeof(config_path), "config.ini");
	udev = udev_new();
	if (!udev)
	{
		fprintf(stderr, "error : cannot create udev\n");
		return (1);
	}
	li = libinput_udev_create_context(&interface, NULL, udev);

	if (ini_parse(config_path, config_handler, &config) < 0)
	{
		fprintf(stderr, "Cannot read config file \n");
	}
	else
	{
		printf("sucess : readed config file \n");
	}
	if (!li)
	{
		fprintf(stderr, "error : libinput cannot create context \n");
		return (1);
	}
	else
	{
		printf("sucess : libinput create context \n");
	}
	if (libinput_udev_assign_seat(li, "seat0") != 0)
	{
		fprintf(stderr, "cannot assign seat 'seat0'\n");
		libinput_unref(li);
		udev_unref(udev);
		return (1);
	}
	libinput_fd = libinput_get_fd(li);
	if (libinput_fd < 0)
	{
		fprintf(stderr, "error : libinputgetfd");
		libinput_unref(li);
		udev_unref(udev);
		return 1;
	}
	printf("sucess : added a touchpad \n");
	while (1)
	{
		fds.fd = libinput_fd;
		fds.events = POLLIN;
		fds.revents = 0;

		int ret = poll(&fds, 1, -1);
		if (ret < 0)
		{
			fprintf(stderr, "error : poll \n");
			continue;
		}
		if (fds.revents & POLLIN)
		{
			libinput_dispatch(li);
			while ((event = libinput_get_event(li)))
			{
				type = libinput_event_get_type(event);
				switch (type)
				{
				case LIBINPUT_EVENT_GESTURE_SWIPE_BEGIN:
					printf("start a gesture \n");
					total_dx = 0.0;
					total_dy = 0.0;
					break;
				case LIBINPUT_EVENT_GESTURE_SWIPE_UPDATE:
					gesture_event = libinput_event_get_gesture_event(event);
					total_dx += libinput_event_gesture_get_dx(gesture_event);
					total_dy += libinput_event_gesture_get_dy(gesture_event);
					break;
				case LIBINPUT_EVENT_GESTURE_SWIPE_END:
					gesture_event = libinput_event_get_gesture_event(event);
					finger_count = libinput_event_gesture_get_finger_count(gesture_event);
					printf("finger count : %d \n", finger_count);
					printf("total_dx: %.2f\ntotal_dy: %.2f\n", total_dx, total_dy);
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
					printf("end a gesture \n");
					break;
				case LIBINPUT_EVENT_GESTURE_PINCH_BEGIN:
					printf("start a pinch \n");
					break;
				case LIBINPUT_EVENT_GESTURE_PINCH_END:
					printf("end a pinch \n");
				default:
					break;
				}
				libinput_event_destroy(event);
			}
		}
	}
	free(config.swipe_up_3);
	free(config.swipe_down_3);
	free(config.swipe_right_3);
	free(config.swipe_left_3);
	free(config.swipe_up_4);
	free(config.swipe_down_4);
	free(config.swipe_right_4);
	free(config.swipe_left_4);
	libinput_unref(li);
	udev_unref(udev);
	return (0);
}
