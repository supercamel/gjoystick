#include "GJoystick-1.0.h"
#include <glib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#ifdef _WIN32
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#else
#include <linux/joystick.h>
#endif

struct _GJoystickManager {
    GObject parent_instance;
};

G_DEFINE_TYPE(GJoystickManager, gjoystick_manager, G_TYPE_OBJECT)

static void gjoystick_manager_init(GJoystickManager *self) {}

static void gjoystick_manager_class_init(GJoystickManagerClass *klass) {}

#ifdef _WIN32
// Helper function to get joystick information on Windows
static void get_joystick_info(UINT joystick_id, GJoystickInfo *info) {
    JOYCAPS caps;
    if (joyGetDevCaps(joystick_id, &caps, sizeof(JOYCAPS)) == JOYERR_NOERROR) {
        info->device_name = g_strdup(caps.szPname);
        info->id = joystick_id;
        info->num_axes = caps.wNumAxes;
        info->num_buttons = caps.wNumButtons;
    }
}
#endif

#ifndef _WIN32
// Helper function to get joystick information on Linux
static int get_joystick_info_linux(int fd, GJoystickInfo *info, int id) {
    char name[128];
    if (ioctl(fd, JSIOCGNAME(sizeof(name)), name) < 0) {
        strcpy(name, "Unknown");
    }
    info->device_name = g_strdup(name);
    info->id = id;

    ioctl(fd, JSIOCGAXES, &info->num_axes);
    ioctl(fd, JSIOCGBUTTONS, &info->num_buttons);
    return 0;
}
#endif

// Get list of all connected joysticks
GList *gjoystick_manager_list_joysticks(GJoystickManager *self) {
    GList *joysticks = NULL;
    GJoystickInfo *info;

#ifdef _WIN32
    UINT numDevs = joyGetNumDevs();
    for (UINT i = 0; i < numDevs; ++i) {
        info = g_new0(GJoystickInfo, 1);
        get_joystick_info(i, info);
        joysticks = g_list_append(joysticks, info);
    }
#else
    for (int i = 0; i < 10; i++) {  // Arbitrary limit of 10 for example
        gchar *device_path = g_strdup_printf("/dev/input/js%d", i);
        int fd = open(device_path, O_RDONLY | O_NONBLOCK);
        if (fd >= 0) {
            info = g_new0(GJoystickInfo, 1);
            get_joystick_info_linux(fd, info, i);
            joysticks = g_list_append(joysticks, info);
            close(fd);
        }
        g_free(device_path);
    }
#endif

    return joysticks;
}

// Free joystick list
void gjoystick_manager_free_joystick_list(GList *joysticks) {
    g_list_free_full(joysticks, (GDestroyNotify)g_free);
}

GJoystickManager *gjoystick_manager_new(void) {
    return g_object_new(GJOYSTICK_TYPE_MANAGER, NULL);
}
