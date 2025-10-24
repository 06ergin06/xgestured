#include <fcntl.h>
#include <libinput.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>

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
	const char						*touchpad_path = "/dev/input/event10";
	enum libinput_event_type		type;
	struct libinput_event_gesture	*gesture_event;
	int								finger_count;
	double							dy;
	double							dx;

	li = libinput_path_create_context(&interface, NULL);
	if (!li)
	{
		fprintf(stderr, "error : libinput cannot create context \n");
		return (1);
	}
	printf("sucess : libinput create context \n");
	if (!libinput_path_add_device(li, touchpad_path))
	{
		fprintf(stderr, "cannot add device : %s \n", touchpad_path);
		libinput_unref(li);
		return (1);
	}
	printf("sucess : added a %s device \n", touchpad_path);
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
				break ;
			case LIBINPUT_EVENT_GESTURE_SWIPE_UPDATE:
				break ;
			case LIBINPUT_EVENT_GESTURE_SWIPE_END:
				printf("end a gesture \n");
				gesture_event = libinput_event_get_gesture_event(event);
				finger_count = libinput_event_gesture_get_finger_count(gesture_event);
				printf("finger count : %d \n", finger_count);
				dy = libinput_event_gesture_get_dy(gesture_event);
				dx = libinput_event_gesture_get_dx(gesture_event);
				printf("dx : %f\n dy : %f\n", dx, dy);
				if (fabs(dy) > fabs(dx))
				{
					if (dy < 0)
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
					if (dx < 0)
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
	return (0);
}
