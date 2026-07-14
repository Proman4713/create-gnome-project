/* {{FILENAME}}-application.c
 *
 * Place your license here
 */

#pragma once

#include <gtk-4.0/gtk/gtk.h>

G_BEGIN_DECLS

#define {{NAME_CAPS}}_TYPE_APPLICATION ({{FILENAME}}_application_get_type())

G_DECLARE_FINAL_TYPE ({{PascalName}}Application, {{FILENAME}}_application, {{NAME_CAPS}}, APPLICATION, GtkApplication)

{{PascalName}}Application* {{FILENAME}}_application_new (const char* application_id,
														GApplicationFlags  flags);

G_END_DECLS
