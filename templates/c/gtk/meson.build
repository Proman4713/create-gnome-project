{{FILENAME}}_sources = [
	'main.c',
	'{{FILENAME}}-application.c',
	'{{FILENAME}}-window.c'
]

{{FILENAME}}_deps = [
	dependency('gtk4'),
]

{{FILENAME}}_sources += gnome.compile_resources('{{FILENAME}}-resources',
	'{{FILENAME}}.gresource.xml',
	c_name: '{{FILENAME}}'
)

executable('{{FILENAME}}', {{FILENAME}}_sources,
		dependencies: {{FILENAME}}_deps,
		install: true,
)
