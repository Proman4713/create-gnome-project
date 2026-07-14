/* {{FILENAME}}-application.c
 *
 * Place your license here
 */

#include "config.h"

#include "{{FILENAME}}-window.h"

struct _{{PascalName}}Window {
	GtkApplicationWindow parent_instance;

	/* Template widgets */
	GtkLabel* label;
};

G_DEFINE_FINAL_TYPE({{PascalName}}Window, {{FILENAME}}_window, GTK_TYPE_APPLICATION_WINDOW)

static void
{{FILENAME}}_window_class_init({{PascalName}}WindowClass* klass) {
	GtkWidgetClass* widget_class = GTK_WIDGET_CLASS(klass);

	gtk_widget_class_set_template_from_resource(widget_class, "{{PROJECT_ID_PATH}}/{{FILENAME}}-window.ui");
	gtk_widget_class_bind_template_child(widget_class, {{PascalName}}Window, label);
}

static void
{{FILENAME}}_window_init({{PascalName}}Window* self) {
	gtk_widget_init_template(GTK_WIDGET(self));
}