#include "header.h"

int	open_restricted(const char *path, int flags, void *user_data)
{
	int	fd;

	fd = open(path, flags);
	if (fd < 0)
	{
		perror("cannot open");
	}
	return (fd);
}

void	close_restricted(int fd, void *user_data)
{
	close(fd);
}

struct libinput_interface	interface = {
	.open_restricted = open_restricted,
	.close_restricted = close_restricted,
};

int	main(void)
{
	struct libinput					*li;
	struct libinput_event			*event;
	enum libinput_event_type		type;
	struct libinput_event_gesture	*gesture_event;
	int								finger_count;
	double							total_dx;
	double							total_dy;
	struct udev						*udev;

	udev = udev_new();
	if (!udev)
	{
		fprintf(stderr, "error : cannot create udev\n");
		return (1);
	}
	li = libinput_udev_create_context(&interface, NULL, udev);
	if (!li)
	{
		fprintf(stderr, "error : libinput cannot create context \n");
		return (1);
	}
	printf("sucess : libinput create context \n");
	if (libinput_udev_assign_seat(li, "seat0") != 0)
	{
		fprintf(stderr, "cannot assign seat 'seat0'\n");
		libinput_unref(li);
		udev_unref(udev);
		return (1);
	}
	printf("sucess : added a touchpad \n");
	while (1)
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
				break ;
			case LIBINPUT_EVENT_GESTURE_SWIPE_UPDATE:
				gesture_event = libinput_event_get_gesture_event(event);
				total_dx += libinput_event_gesture_get_dx(gesture_event);
				total_dy += libinput_event_gesture_get_dy(gesture_event);
				break ;
			case LIBINPUT_EVENT_GESTURE_SWIPE_END:
				printf("end a gesture \n");
				gesture_event = libinput_event_get_gesture_event(event);
				finger_count = libinput_event_gesture_get_finger_count(gesture_event);
				printf("finger count : %d \n", finger_count);
				printf("total_dx: %.2f\ntotal_dy: %.2f\n", total_dx, total_dy);
				if (ft_fabs(total_dy) > ft_fabs(total_dx))
				{
					if (total_dy < 0)
					{
						printf("fingers up \n");
					}
					else
					{
						printf("fingers down \n");
					}
				}
				else
				{
					if (total_dx < 0)
					{
						printf("fingers left \n");
					}
					else
					{
						printf("fingers right \n");
					}
				}
				break ;
			case LIBINPUT_EVENT_GESTURE_PINCH_BEGIN:
				printf("start a pinch \n");
				break ;
			case LIBINPUT_EVENT_GESTURE_PINCH_END:
				printf("end a pinch \n");
			default:
				break ;
			}
			libinput_event_destroy(event);
		}
	}
	libinput_unref(li);
	udev_unref(udev);
	return (0);
}
