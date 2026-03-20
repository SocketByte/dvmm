#include <stddef.h>
#include <gtk/gtk.h>
#include <libayatana-appindicator/app-indicator.h>
#include <string.h>
#include "process.h"

static char nvidia_icon_path[PATH_MAX];
static char vfio_icon_path[PATH_MAX];

static AppIndicator *indicator = NULL;

static void on_lg_exit(GPid pid, gint status, gpointer user_data) {
    g_print("Looking Glass exited (status=%d)\n", status);

    char* args[] = {
        "virsh",
        "-c", "qemu:///system",
        "shutdown", "win11",
        NULL
    };
    run_process(args);

    g_spawn_close_pid(pid);
}

static void on_vm_run(GtkMenuItem *item, gpointer user_data) {
    char* args[] = {
        "virsh",
        "-c", "qemu:///system",
        "start", "win11",
        NULL
    };
    run_process(args);

    char* lg_args[] = {
        "looking-glass-client",
        NULL
    };

    const pid_t pid = run_process_async(lg_args);

    if (pid > 0) {
        g_child_watch_add((GPid)pid, on_lg_exit, NULL);
    }
}

gboolean check_gpu_status(gpointer user_data) {
    char *output = run_command_capture("gpu-check");

    if (!output)
        return FALSE;

    if (strstr(output, "nvidia")) {
        app_indicator_set_icon(indicator, nvidia_icon_path);
    } else if (strstr(output, "vfio-pci")) {
        app_indicator_set_icon(indicator, vfio_icon_path);
    } else {
        app_indicator_set_icon(indicator, vfio_icon_path);
    }

    free(output);
    return TRUE;
}

int main(int argc, char **argv) {
    gtk_init(&argc, &argv);

    const char *home = getenv("HOME");
    if (!home) {
        g_error("HOME environment variable not set");
    }

    snprintf(nvidia_icon_path, sizeof(nvidia_icon_path),
             "%s/.config/dvmm/nvidia.png", home);

    snprintf(vfio_icon_path, sizeof(vfio_icon_path),
             "%s/.config/dvmm/vfio.png", home);

    g_log_set_handler("libayatana-appindicator",
                      G_LOG_LEVEL_WARNING,
                      (GLogFunc)g_log_default_handler,
                      NULL);

    indicator = app_indicator_new(
        "dvmm",
        nvidia_icon_path,
        APP_INDICATOR_CATEGORY_APPLICATION_STATUS
    );

    app_indicator_set_status(indicator, APP_INDICATOR_STATUS_ACTIVE);

    GtkWidget *menu = gtk_menu_new();
    GtkWidget *run_item = gtk_menu_item_new_with_label("Run Virtual Machine");
    g_signal_connect(run_item, "activate", G_CALLBACK(on_vm_run), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), run_item);
    gtk_widget_show_all(menu);
    app_indicator_set_menu(indicator, GTK_MENU(menu));

    g_timeout_add(3000, check_gpu_status, NULL);

    gtk_main();
    return 0;
}