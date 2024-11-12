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

struct _GJoystickListener {
    GObject parent_instance;
    GJoystickInfo *info;
#ifdef _WIN32
    UINT joystick_id;
#else
    int fd;
#endif
};

G_DEFINE_TYPE(GJoystickListener, gjoystick_listener, G_TYPE_OBJECT)

static guint joystick_signals[GJOYSTICK_LAST_SIGNAL] = {0};

static void joystick_emit_button_signal(GJoystickListener *self, gint button, gboolean pressed) {
    g_signal_emit(self, joystick_signals[GJOYSTICK_BUTTON_PRESSED], 0, button, pressed);
}

static void joystick_emit_axis_signal(GJoystickListener *self, gint axis, gint value) {
    g_signal_emit(self, joystick_signals[GJOYSTICK_AXIS_MOVED], 0, axis, value);
}

#ifdef _WIN32
// Windows polling function
static gboolean joystick_poll_windows(GJoystickListener *self) {
    JOYINFOEX joyInfo;
    joyInfo.dwSize = sizeof(JOYINFOEX);
    joyInfo.dwFlags = JOY_RETURNALL;

    if (joyGetPosEx(self->joystick_id, &joyInfo) == JOYERR_NOERROR) {
        joystick_emit_axis_signal(self, 0, joyInfo.dwXpos);
        joystick_emit_axis_signal(self, 1, joyInfo.dwYpos);
        joystick_emit_button_signal(self, 1, (joyInfo.dwButtons & JOY_BUTTON1) != 0);
        // Add more buttons/axes as necessary
    }
    return G_SOURCE_CONTINUE;
}
#else
// Linux polling function
static gboolean joystick_poll_linux(GJoystickListener *self) {
    struct js_event event;
    ssize_t bytes = read(self->fd, &event, sizeof(event));

    if (bytes == sizeof(event)) {
        if (event.type == JS_EVENT_BUTTON) {
            joystick_emit_button_signal(self, event.number, event.value);
        } else if (event.type == JS_EVENT_AXIS) {
            joystick_emit_axis_signal(self, event.number, event.value);
        }
    }
    return G_SOURCE_CONTINUE;
}
#endif

// Start listening for joystick events
void gjoystick_listener_start(GJoystickListener *self) {
#ifdef _WIN32
    self->joystick_id = self->info->id;
    g_timeout_add(16, (GSourceFunc) joystick_poll_windows, self);
#else
    gchar *device_path = g_strdup_printf("/dev/input/js%d", self->info->id);
    self->fd = open(device_path, O_RDONLY | O_NONBLOCK);
    if (self->fd < 0) {
        g_warning("Failed to open joystick device: %s", device_path);
        g_free(device_path);
        return;
    }
    g_free(device_path);

    g_timeout_add(16, (GSourceFunc)joystick_poll_linux, self);
#endif
}

// Stop listening for joystick events
void gjoystick_listener_stop(GJoystickListener *self) {
#ifdef _WIN32
    // No specific stop action needed for Windows
#else
    if (self->fd >= 0) {
        close(self->fd);
    }
#endif
}

GJoystickListener *gjoystick_listener_new(GJoystickInfo *info) {
    GJoystickListener *listener = g_object_new(GJOYSTICK_TYPE_LISTENER, NULL);
    listener->info = info;
    return listener;
}

static void gjoystick_listener_init(GJoystickListener *self) {}

static void gjoystick_listener_class_init(GJoystickListenerClass *klass) {
    joystick_signals[GJOYSTICK_BUTTON_PRESSED] = g_signal_new(
        "button-pressed", G_TYPE_FROM_CLASS(klass), G_SIGNAL_RUN_LAST,
        0, NULL, NULL, NULL, G_TYPE_NONE, 2, G_TYPE_INT, G_TYPE_BOOLEAN);

    joystick_signals[GJOYSTICK_AXIS_MOVED] = g_signal_new(
        "axis-moved", G_TYPE_FROM_CLASS(klass), G_SIGNAL_RUN_LAST,
        0, NULL, NULL, NULL, G_TYPE_NONE, 2, G_TYPE_INT, G_TYPE_INT);
}
