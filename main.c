#include "header.h"

static const struct libinput_interface interface = {
		.open_restricted = open_restricted,
		.close_restricted = close_restricted,
};

int main(void)
{
	struct libinput					*li;
	struct libinput_event			*event;
	enum libinput_event_type		type;
	struct libinput_event_gesture	*gesture_event;
	int								finger_count;
	double							total_dx;
	double							total_dy;
	struct pollfd					fds;
	int								libinput_fd;
	struct udev						*udev;
	struct sigaction				sa;
	int								ret;

	total_dx = 0.0;
	total_dy = 0.0;
	sa.sa_handler = handle_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	struct s_config config = {0};

	if (sigaction(SIGHUP, &sa, NULL) == -1)
		perror("sigaction SIGHUP");
	// Zombie process
	if (signal(SIGCHLD, SIG_IGN) == SIG_ERR)
		perror("signal SIGCHLD");

	udev = udev_create();
	li = libinput_udev_create_context(&interface, NULL, udev);
	load_config(&config);
	if (!li)
	{
		perror("error : libinput cannot create context \n");
		return (1);
	}
	else
		printf("sucess : libinput create context \n");
	if (libinput_udev_assign_seat(li, "seat0") != 0)
	{
		perror("cannot assign seat 'seat0'\n");
		libinput_unref(li);
		udev_unref(udev);
		return (1);
	}
	libinput_fd = libinput_get_fd(li);
	if (libinput_fd < 0)
	{
		perror("error : libinputgetfd");
		libinput_unref(li);
		udev_unref(udev);
		return 1;
	}
	printf("sucess : added a touchpad \n");
	while (1)
	{
		if (reload_requested)
		{
			reload_requested = 0;
			printf("sucess : config reload \n");
			load_config(&config);
		}
		fds.fd = libinput_fd;
		fds.events = POLLIN;
		fds.revents = 0;

		ret = poll(&fds, 1, -1);
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
					gesture_command_run(total_dx, total_dy, finger_count, config);
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
