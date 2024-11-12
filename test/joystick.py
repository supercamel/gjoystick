import gi
gi.require_version("GJoystick", "1.0")
from gi.repository import GJoystick, GLib

def on_button_pressed(listener, button, pressed):
    print(f"Button {button} {'pressed' if pressed else 'released'}")

def on_axis_moved(listener, axis, value):
    print(f"Axis {axis} moved to {value}")

def main():
    # Create a joystick manager instance
    manager = GJoystick.Manager.new()

    # List available joysticks
    joysticks = manager.list_joysticks()
    if not joysticks:
        print("No joysticks found.")
        return

    # Print details of each joystick and create a listener for the first one
    for idx, joystick_info in enumerate(joysticks):
        print(f"Joystick {idx}:")
        print(f"  Device Name: {joystick_info.device_name}")
        print(f"  ID: {joystick_info.id}")
        print(f"  Axes: {joystick_info.num_axes}")
        print(f"  Buttons: {joystick_info.num_buttons}")

    # Use the first joystick in the list
    first_joystick_info = joysticks[1]
    listener = GJoystick.Listener.new(first_joystick_info)

    # Connect signals to the listener
    listener.connect("button-pressed", on_button_pressed)
    listener.connect("axis-moved", on_axis_moved)

    # Start listening for events
    listener.start()

    # Run the main loop to handle events
    loop = GLib.MainLoop()
    try:
        loop.run()
    except KeyboardInterrupt:
        print("Exiting...")

    # Stop the listener and free resources
    listener.stop()

if __name__ == "__main__":
    main()
