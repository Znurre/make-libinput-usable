#include <libinput.h>
#include <dlfcn.h>

using libinput_device_config_accel_set_profile_t = libinput_config_status (*)(libinput_device *device, libinput_config_accel_profile profile);

extern "C"
{
    libinput_config_status libinput_device_config_accel_set_profile(libinput_device *device, libinput_config_accel_profile profile)
    {
        auto next = reinterpret_cast<libinput_device_config_accel_set_profile_t>(dlsym(RTLD_NEXT,"libinput_device_config_accel_set_profile"));

        if (profile == LIBINPUT_CONFIG_ACCEL_PROFILE_CUSTOM)
        {
            return next(device, profile);
        }

        auto accel_config =  libinput_config_accel_create(LIBINPUT_CONFIG_ACCEL_PROFILE_CUSTOM);

        if (!accel_config)
        {
            return next(device, profile);
        }

        double points[] = {
            0.000, 0.049, 0.097,
            0.167, 0.240, 0.313,
            0.386, 0.491, 0.602,
            0.713, 0.824, 0.935,
            1.046, 1.157, 1.268,
            1.379, 1.490, 1.601,
            1.712, 1.823, 2.052
        };

        if (libinput_config_accel_set_points(accel_config, LIBINPUT_ACCEL_TYPE_MOTION, 0.2031610269, sizeof(points) / sizeof(*points), points) != LIBINPUT_CONFIG_STATUS_SUCCESS)
        {
            return next(device, profile);
        }

        if (libinput_device_config_accel_apply(device, accel_config) != LIBINPUT_CONFIG_STATUS_SUCCESS)
        {
            return next(device, profile);
        }

        return LIBINPUT_CONFIG_STATUS_SUCCESS;
    }
}
