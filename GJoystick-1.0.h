#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <glib-object.h>

G_BEGIN_DECLS

#define JOYSTICK_TYPE (joystick_get_type())
G_DECLARE_FINAL_TYPE(Joystick, joystick, JOYSTICK, GOBJECT, GObject)

/**
 * Returns all the available joysticks.
 */
GSList* joystick_enumerate();

/**
 * unrefs every joystick object in the list, except for js
 */
void joystick_keep(GSList* list, Joystick* js);

/**
 * Gets the device name as a string
 */
gchar* joystick_get_name(Joystick* self);

/**
 * Begins listening to events from the joystick
 */
void joystick_start(Joystick *self);
/**
 * Stops listening to events from the joystick
 */
void joystick_stop(Joystick *self);

G_END_DECLS

#endif // JOYSTICK_H