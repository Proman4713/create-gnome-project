/* {{FILENAME}}-application.c
 *
 * Place your license here
 */

#pragma once

#include <gtk-4.0/gtk/gtk.h>

G_BEGIN_DECLS

#define {{PREFIX}}_TYPE_WINDOW ({{FILENAME}}_window_get_type())

G_DECLARE_FINAL_TYPE ({{PascalName}}Window, {{FILENAME}}_window, {{NAME_CAPS}}, WINDOW, GtkApplicationWindow)

G_END_DECLS