project('gjoystick-1.0', 'c', version: '0.2.0', license: 'MIT')

gnome = import('gnome')
pkg = import('pkgconfig')

dependencies = [
  dependency('glib-2.0'),
  dependency('gobject-2.0')
  ]

system = build_machine.system()

gjoystick_sources = ['joystick_manager.c', 'joystick_listener.c']

if system == 'windows'
  gjoysticklib = shared_library('gjoystick-1.0',
    gjoystick_sources,
    vala_vapi: 'gjoystick-1.0.vapi',
    vala_gir: 'gjoystick-1.0.gir',
    include_directories: './',
    dependencies: dependencies,
    install: true
  )
else
  dependencies += dependency('libevdev')
  gjoysticklib = shared_library('gjoystick-1.0',
    gjoystick_sources,
    vala_vapi: 'gjoystick-1.0.vapi',
    vala_gir: 'gjoystick-1.0.gir',
    include_directories: './',
    dependencies: dependencies,
    install: true
  )
endif

pkg.generate(gjoysticklib,
  version: '1.0',
  name: 'gjoystick',
  filebase: 'gjoystick-1.0',
  description: 'A joystick library',
  subdirs: 'gjoystick-1.0'
  )


headers = ['GJoystick-1.0.h']

install_headers(headers, subdir: 'gjoystick-1.0')

introspection_sources = ['joystick_manager.c', 'joystick_listener.c', 'GJoystick-1.0.h']

g_ir_compiler = find_program('g-ir-compiler')
gjoystick_gir = gnome.generate_gir(gjoysticklib,
  sources: introspection_sources,
  nsversion: '1.0',
  namespace: 'GJoystick',
  symbol_prefix: 'gjoystick_',
  identifier_prefix: 'GJoystick',
  export_packages: 'gjoystick',
  includes: ['GObject-2.0'],
  install: true,
  install_dir_gir: get_option('libdir') / 'girepository-1.0',
  install_dir_typelib: get_option('libdir') / 'girepository-1.0'
  )[0]

gnome.generate_vapi('gjoystick-1.0',
  install: true,
  sources: gjoystick_gir
  )
