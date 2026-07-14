/* {{FILENAME}}-application.c
 *
 * Place your license here
 */

#include <glib-2.0/glib/gi18n.h>

#include "{{FILENAME}}-application.h"
#include "{{FILENAME}}-window.h"

struct _{{PascalName}}Application {
	GtkApplication parent_instance;
};

G_DEFINE_FINAL_TYPE({{PascalName}}Application, {{FILENAME}}_application, GTK_TYPE_APPLICATION)

{{PascalName}}Application*
{{FILENAME}}_application_new(const char* application_id,
							GApplicationFlags flags) {
	g_return_val_if_fail(application_id != NULL, NULL);

	return g_object_new({{NAME_CAPS}}_TYPE_APPLICATION,
						"application-id", application_id,
						"flags", flags,
						NULL);
}

static void
{{FILENAME}}_application_activate(GApplication* app) {
	GtkWindow* window;

	g_assert({{NAME_CAPS}}_IS_APPLICATION(app));

	window = gtk_application_get_active_window(GTK_APPLICATION(app));

	if (window == NULL)
		window = g_object_new ({{NAME_CAPS}}_TYPE_WINDOW,
							   "application", app,
							   NULL);
	
	gtk_window_present(window);
}

static void
{{FILENAME}}_application_class_init({{PascalName}}ApplicationClass* klass) {
	GApplicationClass* app_class = G_APPLICATION_CLASS(klass);

	app_class->activate = {{FILENAME}}_application_activate;
}

static void
{{FILENAME}}_application_about_action(GSimpleAction* action,
									GVariant* param,
									gpointer user_data) {
	static const char* authors[] = {"{{AUTHOR}}", NULL};
	{{PascalName}}Application* self = user_data;
	GtkWindow* window = NULL;

	g_assert({{NAME_CAPS}}_IS_APPLICATION(self));

	window = gtk_application_get_active_window(GTK_APPLICATION(self));

	gtk_show_about_dialog(window,
						  "program-name", "{{PROJECT_NAME}}",
						  "logo-icon-name", "{{PROJECT_ID}}",
						  "authors", authors,
						  "translator-credits", _("translator-credits"),
						  "version", "1.0.0",
						  "copyright", "© {{YEAR}} {{AUTHOR}}",
						  NULL);
}

static void
{{FILENAME}}_application_quit_action (GSimpleAction* action,
									  GVariant* parameter,
									  gpointer user_data)
{
	{{PascalName}}Application* self = user_data;

	g_assert ({{NAME_CAPS}}_IS_APPLICATION (self));

	g_application_quit (G_APPLICATION (self));
}

static const GActionEntry app_actions[] = {
	{"quit", {{FILENAME}}_application_quit_action},
	{"about", {{FILENAME}}_application_about_action},
};

static void
{{FILENAME}}_application_init ({{PascalName}}Application* self)
{
	g_action_map_add_action_entries (G_ACTION_MAP (self),
									 app_actions,
									 G_N_ELEMENTS (app_actions),
									 self);
	gtk_application_set_accels_for_action (GTK_APPLICATION (self),
										   "app.quit",
										   (const char *[]) { "<control>q", NULL });
}