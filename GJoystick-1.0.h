#ifndef GJOYSTICK_H
#define GJOYSTICK_H

#include <glib-object.h>

G_BEGIN_DECLS

#define GJOYSTICK_TYPE_MANAGER (gjoystick_manager_get_type())
#define GJOYSTICK_TYPE_LISTENER (gjoystick_listener_get_type())

typedef struct _GJoystickManager GJoystickManager;
typedef struct _GJoystickListener GJoystickListener;

typedef struct {
    gchar *device_name;
    gint id;            // Platform-specific ID for joystick identification
    gint num_axes;
    gint num_buttons;
} GJoystickInfo;

G_DECLARE_FINAL_TYPE(GJoystickManager, gjoystick_manager, GJOYSTICK, MANAGER, GObject)
G_DECLARE_FINAL_TYPE(GJoystickListener, gjoystick_listener, GJOYSTICK, LISTENER, GObject)

// Joystick Manager API
/**
 * gjoystick_manager_new:
 *
 * Creates a new instance of #GJoystickManager.
 *
 * Returns: (transfer full): a new #GJoystickManager instance.
 */
GJoystickManager *gjoystick_manager_new(void);

/**
 * gjoystick_manager_list_joysticks:
 * @self: (in): a #GJoystickManager instance.
 *
 * Retrieves a list of available joysticks.
 *
 * Returns: (element-type GJoystickInfo) (transfer full): a list of #GJoystickInfo structs, representing each joystick.
 *          The caller is responsible for freeing this list with gjoystick_manager_free_joystick_list.
 */
GList *gjoystick_manager_list_joysticks(GJoystickManager *self);

/**
 * gjoystick_manager_free_joystick_list:
 * @joysticks: (element-type GJoystickInfo) (transfer full): a list of #GJoystickInfo structs to free.
 *
 * Frees a joystick list created by gjoystick_manager_list_joysticks.
 */
void gjoystick_manager_free_joystick_list(GList *joysticks);

// Joystick Listener API
/**
 * gjoystick_listener_new:
 * @info: (in): a #GJoystickInfo struct describing the joystick.
 *
 * Creates a new joystick listener for the specified joystick.
 *
 * Returns: (transfer full): a new #GJoystickListener instance.
 */
GJoystickListener *gjoystick_listener_new(GJoystickInfo *info);

/**
 * gjoystick_listener_start:
 * @listener: (in): a #GJoystickListener instance.
 *
 * Starts the listener to emit signals on joystick events.
 */
void gjoystick_listener_start(GJoystickListener *listener);

/**
 * gjoystick_listener_stop:
 * @listener: (in): a #GJoystickListener instance.
 *
 * Stops the listener from emitting signals.
 */
void gjoystick_listener_stop(GJoystickListener *listener);


// Signals
typedef enum {
    GJOYSTICK_BUTTON_PRESSED,
    GJOYSTICK_AXIS_MOVED,
    GJOYSTICK_LAST_SIGNAL
} GJoystickSIGNALS;

G_END_DECLS

#endif // JOYSTICK_H
