#include <fcntl.h>
#include <libinput.h>
#include <libudev.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "ini.h"

struct s_config
{
    char *swipe_up_3;
    char *swipe_down_3;
    char *swipe_left_3;
    char *swipe_right_3;
    char *swipe_up_4;
    char *swipe_down_4;
    char *swipe_left_4;
    char *swipe_right_4;
};

int config_handler(void *user_data, const char *section, const char *name, const char *value);
double ft_fabs(double x);
