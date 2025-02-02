/* vi: set sw=4 ts=4 wrap ai expandtab: */
/*
 * loongson-infod.c: This file is part of loongson-info.
 *
 * Copyright (C) 2022 Xiaotian Wu <wuxiaotian@loongson.cn>
 *
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, see <http://www.gnu.org/licenses/>.
 * */


#include <stdio.h>

#include <gio/gio.h>
#include <glib/gi18n.h>
#include "info-dbus-generated.h"
#include "loongson-infod.h"
#include "hardinfo.h"

#define INFO_DBUS_NAME "cn.loongson.info"
#define INFO_DBUS_PATH "/cn/loongson/info"

struct _InfoDaemon
{
    GObject            parent;
    //Info              *info;
    BusInfo           *skeleton;
    guint              bus_name_id;
    GMainLoop         *loop;
    gboolean           replace;
};

enum {
    PROP_0,
    PROP_LOOP,
    PROP_REPLACE,
    LAST_PROP
};

gboolean info_get_cpu_name (BusInfo *object,
                           GDBusMethodInvocation *invocation,
                           gpointer user_data);
static GParamSpec *properties[LAST_PROP] = { NULL };

G_DEFINE_TYPE (InfoDaemon, info_daemon, G_TYPE_OBJECT)

gboolean info_get_cpu_name (BusInfo *object,
                            GDBusMethodInvocation *invocation,
                            gpointer user_data)
{
    gchar *cpu_name;
    cpu_info_t *cpu = NULL;
    cpu=get_cpu_info();
    cpu_name = cpu->cpu_name;
    bus_info_complete_cpu_name (object, invocation, cpu_name);
    return TRUE;
}

static gboolean info_get_biso_name (BusInfo *object,
                                    GDBusMethodInvocation *invocation,
                                    gpointer user_data)
{
    gchar *bios_name;

    bios_name = get_bios_version();
    bus_info_complete_bios_name (object, invocation, bios_name);

    return TRUE;
}

static gboolean info_get_calculation_part (BusInfo *object,
                                           GDBusMethodInvocation *invocation,
                                           gpointer user_data)
{
    gchar *calculation_part = NULL;

    bus_info_complete_calculation_part (object, invocation, calculation_part);

    return TRUE;
}
static gboolean info_get_cpu_cache (BusInfo *object,
                                    GDBusMethodInvocation *invocation,
                                    gpointer user_data)
{
    gchar *cpu_cache = NULL;
    cpu_info_t *cpu=NULL;
    cpu=get_cpu_info();
    cpu_cache = cpu->cacheL3;
    bus_info_complete_cpu_cache (object, invocation, cpu_cache);

    return TRUE;
}
static gboolean info_get_cpu_sizes (BusInfo *object,
                                    GDBusMethodInvocation *invocation,
                                    gpointer user_data)
{
    gchar *cpu_sizes = NULL;

    bus_info_complete_cpu_sizes (object, invocation, cpu_sizes);

    return TRUE;
}
static gboolean info_get_cpu_temperature (BusInfo *object,
                                          GDBusMethodInvocation *invocation,
                                          gpointer user_data)
{
    gchar *cpu_temperature = NULL;
    int temp;
    char buf[32];
    ls_sensors_t sen;

    get_sensors(&sen);
    temp = (sen.cputemp0+sen.cputemp1)/2;
    sprintf(buf,"%d",temp);
    cpu_temperature = g_strdup(buf);

    bus_info_complete_cpu_temperature (object, invocation, cpu_temperature);
    g_free(cpu_temperature);

    return TRUE;
}
static gboolean info_get_cpu_threads (BusInfo *object,
                                      GDBusMethodInvocation *invocation,
                                      gpointer user_data)
{
    gchar *cpu_threads = NULL;
    cpu_threads = get_cpu_thread_num();

    bus_info_complete_cpu_threads (object, invocation, cpu_threads);

    return TRUE;
}
static gboolean info_get_extended_instruction (BusInfo *object,
                                               GDBusMethodInvocation *invocation,
                                               gpointer user_data)
{
    gchar *instruction = NULL;

    bus_info_complete_extended_instruction (object, invocation, instruction);

    return TRUE;
}
static gboolean info_get_fan_speed (BusInfo *object,
                                    GDBusMethodInvocation *invocation,
                                    gpointer user_data)
{
    gchar *speed = NULL;

    bus_info_complete_fan_speed (object, invocation, speed);

    return TRUE;
}
static gboolean info_get_hardware_assisted_virtualization (BusInfo *object,
                                                           GDBusMethodInvocation *invocation,
                                                           gpointer user_data)
{
    gchar *virtualization = NULL;

    bus_info_complete_hardware_assisted_virtualization (object, invocation, virtualization);

    return TRUE;
}
static gboolean info_get_junction_temperature (BusInfo *object,
                                               GDBusMethodInvocation *invocation,
                                               gpointer user_data)
{
    gchar *temperature = NULL;
    cpu_info_t *cpu = NULL;
    cpu = get_cpu_info();
    temperature = cpu->cpu_jt_l;
    bus_info_complete_junction_temperature (object, invocation, temperature);

    return TRUE;
}
static gboolean info_get_key_management (BusInfo *object,
                                         GDBusMethodInvocation *invocation,
                                         gpointer user_data)
{
    gchar *key = NULL;

    bus_info_complete_key_management (object, invocation, key);

    return TRUE;
}
static gboolean info_get_maximum_cpu_frequency (BusInfo *object,
                                                GDBusMethodInvocation *invocation,
                                                gpointer user_data)
{
    gchar *frequency = NULL;

    frequency  = get_cpu_max_speed();

    bus_info_complete_maximum_cpu_frequency (object, invocation, frequency);

    return TRUE;
}
static gboolean info_get_maximum_memory_capacity (BusInfo *object,
                                                  GDBusMethodInvocation *invocation,
                                                  gpointer user_data)
{
    gchar *capacity = NULL;

    bus_info_complete_maximum_memory_capacity (object, invocation, capacity);

    return TRUE;
}
static gboolean info_get_maximum_memory_frequency (BusInfo *object,
                                                   GDBusMethodInvocation *invocation,
                                                   gpointer user_data)
{
    gchar *frequency = NULL;

    bus_info_complete_maximum_memory_frequency (object, invocation, frequency);

    return TRUE;
}
static gboolean info_get_memory_channel (BusInfo *object,
                                         GDBusMethodInvocation *invocation,
                                         gpointer user_data)
{
    gchar *channel = NULL;

    bus_info_complete_memory_channel (object, invocation, channel);

    return TRUE;
}
static gboolean info_get_memory_style (BusInfo *object,
                                       GDBusMethodInvocation *invocation,
                                       gpointer user_data)
{
    gchar *memory = NULL;

    bus_info_complete_memory_style (object, invocation, memory);

    return TRUE;
}
static gboolean info_get_memory_verification (BusInfo *object,
                                              GDBusMethodInvocation *invocation,
                                              gpointer user_data)
{
    gchar *verification = NULL;

    bus_info_complete_memory_verification (object, invocation, verification);

    return TRUE;
}
static gboolean info_get_micro_architecture (BusInfo *object,
                                             GDBusMethodInvocation *invocation,
                                             gpointer user_data)
{
    gchar *architecture = NULL;
    cpu_info_t *cpu=NULL;
    cpu=get_cpu_info();
    architecture = cpu->microarch;

    bus_info_complete_micro_architecture (object, invocation, architecture);

    return TRUE;
}
static gboolean info_get_mmu_style (BusInfo *object,
                                    GDBusMethodInvocation *invocation,
                                    gpointer user_data)
{
    gchar *mmu = NULL;

    bus_info_complete_mmu_style (object, invocation, mmu);

    return TRUE;
}
static gboolean info_get_packaging_method (BusInfo *object,
                                           GDBusMethodInvocation *invocation,
                                           gpointer user_data)
{
    gchar *method = NULL;

    bus_info_complete_packaging_method (object, invocation, method);

    return TRUE;
}
static gboolean info_get_physical_kernel (BusInfo *object,
                                          GDBusMethodInvocation *invocation,
                                          gpointer user_data)
{
    gchar *kerenl = NULL;

    kerenl   = get_cpu_core_num();
    bus_info_complete_physical_kernel (object, invocation, kerenl);

    return TRUE;
}
static gboolean info_get_power_waste (BusInfo *object,
                                      GDBusMethodInvocation *invocation,
                                      gpointer user_data)
{
    gchar *power = NULL;

    bus_info_complete_power_waste (object, invocation, power);

    return TRUE;
}
static gboolean info_get_product_name (BusInfo *object,
                                       GDBusMethodInvocation *invocation,
                                       gpointer user_data)
{
    gchar *name = NULL;

    name   = get_product_name();
    bus_info_complete_product_name (object, invocation, name);

    return TRUE;
}
static gboolean info_get_trusted_start (BusInfo *object,
                                        GDBusMethodInvocation *invocation,
                                        gpointer user_data)
{
    gchar *trusted_start = NULL;

    bus_info_complete_trusted_start (object, invocation, trusted_start);

    return TRUE;
}

static void set_dbus_signal_method (InfoDaemon *daemon)
{
    g_signal_connect (daemon->skeleton, "handle-bios-name", G_CALLBACK (info_get_biso_name), daemon);
    g_signal_connect (daemon->skeleton, "handle-calculation-part", G_CALLBACK (info_get_calculation_part), daemon);
    g_signal_connect (daemon->skeleton, "handle-cpu-cache", G_CALLBACK (info_get_cpu_cache), daemon);
    g_signal_connect (daemon->skeleton, "handle-cpu-name", G_CALLBACK (info_get_cpu_name), daemon);
    g_signal_connect (daemon->skeleton, "handle-cpu-sizes", G_CALLBACK (info_get_cpu_sizes), daemon);
    g_signal_connect (daemon->skeleton, "handle-cpu-temperature", G_CALLBACK (info_get_cpu_temperature), daemon);
    g_signal_connect (daemon->skeleton, "handle-cpu-threads", G_CALLBACK (info_get_cpu_threads), daemon);
    g_signal_connect (daemon->skeleton, "handle-extended-instruction", G_CALLBACK (info_get_extended_instruction), daemon);
    g_signal_connect (daemon->skeleton, "handle-fan-speed", G_CALLBACK (info_get_fan_speed), daemon);
    g_signal_connect (daemon->skeleton, "handle-hardware-assisted-virtualization", G_CALLBACK (info_get_hardware_assisted_virtualization), daemon);
    g_signal_connect (daemon->skeleton, "handle-junction-temperature", G_CALLBACK (info_get_junction_temperature), daemon);
    g_signal_connect (daemon->skeleton, "handle-key-management", G_CALLBACK (info_get_key_management), daemon);
    g_signal_connect (daemon->skeleton, "handle-maximum-cpu-frequency", G_CALLBACK (info_get_maximum_cpu_frequency), daemon);
    g_signal_connect (daemon->skeleton, "handle-maximum-memory-capacity", G_CALLBACK (info_get_maximum_memory_capacity), daemon);
    g_signal_connect (daemon->skeleton, "handle-maximum-memory-frequency", G_CALLBACK (info_get_maximum_memory_frequency), daemon);
    g_signal_connect (daemon->skeleton, "handle-memory-channel", G_CALLBACK (info_get_memory_channel), daemon);
    g_signal_connect (daemon->skeleton, "handle-memory-style", G_CALLBACK (info_get_memory_style), daemon);
    g_signal_connect (daemon->skeleton, "handle-memory-verification", G_CALLBACK (info_get_memory_verification), daemon);
    g_signal_connect (daemon->skeleton, "handle-micro-architecture", G_CALLBACK (info_get_micro_architecture), daemon);
    g_signal_connect (daemon->skeleton, "handle-mmu-style", G_CALLBACK (info_get_mmu_style), daemon);
    g_signal_connect (daemon->skeleton, "handle-packaging-method", G_CALLBACK (info_get_packaging_method), daemon);
    g_signal_connect (daemon->skeleton, "handle-physical-kernel", G_CALLBACK (info_get_physical_kernel), daemon);
    g_signal_connect (daemon->skeleton, "handle-power-waste", G_CALLBACK (info_get_power_waste), daemon);
    g_signal_connect (daemon->skeleton, "handle-product-name", G_CALLBACK (info_get_product_name), daemon);
    g_signal_connect (daemon->skeleton, "handle-trusted-start", G_CALLBACK (info_get_trusted_start), daemon);

}

static void bus_acquired_handler_cb (GDBusConnection *connection,
                                     const gchar     *name,
                                     gpointer         user_data)
{
    InfoDaemon *daemon;

    GError *error = NULL;
    gboolean exported;

    daemon = INFO_DAEMON (user_data);

    set_dbus_signal_method (daemon);
    exported = g_dbus_interface_skeleton_export (G_DBUS_INTERFACE_SKELETON (daemon->skeleton),
            connection, INFO_DBUS_PATH, &error);

    if (!exported)
    {
        g_warning ("Failed to export interface: %s", error->message);
        g_error_free (error);

        g_main_loop_quit (daemon->loop);
    }
}

static void name_lost_handler_cb (GDBusConnection *connection,
                                  const gchar     *name,
                                  gpointer         user_data)
{
    InfoDaemon *daemon;

    daemon = INFO_DAEMON (user_data);
    g_debug("bus name lost\n");

    g_main_loop_quit (daemon->loop);
}

static void info_daemon_constructed (GObject *object)
{
    InfoDaemon *daemon;

    GBusNameOwnerFlags flags;

    daemon = INFO_DAEMON (object);

    G_OBJECT_CLASS (info_daemon_parent_class)->constructed (object);

    flags = G_BUS_NAME_OWNER_FLAGS_ALLOW_REPLACEMENT;
    if (daemon->replace)
        flags |= G_BUS_NAME_OWNER_FLAGS_REPLACE;

    daemon->bus_name_id = g_bus_own_name (G_BUS_TYPE_SYSTEM,
                                          INFO_DBUS_NAME, flags,
                                          bus_acquired_handler_cb, NULL,
                                          name_lost_handler_cb, daemon, NULL);
}

static void info_daemon_dispose (GObject *object)
{
    InfoDaemon *daemon;

    daemon = INFO_DAEMON (object);

    if (daemon->skeleton != NULL)
    {
        GDBusInterfaceSkeleton *skeleton;

        skeleton = G_DBUS_INTERFACE_SKELETON (daemon->skeleton);
        g_dbus_interface_skeleton_unexport (skeleton);

        g_clear_object (&daemon->skeleton);
    }

    if (daemon->bus_name_id > 0)
    {
        g_bus_unown_name (daemon->bus_name_id);
        daemon->bus_name_id = 0;
    }
    G_OBJECT_CLASS (info_daemon_parent_class)->dispose (object);
}

static void info_daemon_set_property (GObject *object,
                                      guint prop_id,
                                      const GValue *value,
                                      GParamSpec   *pspec)
{
    InfoDaemon *daemon;

    daemon = INFO_DAEMON (object);

    switch (prop_id)
    {
        case PROP_LOOP:
            daemon->loop = g_value_get_pointer(value);
            break;
        case PROP_REPLACE:
            daemon->replace = g_value_get_boolean (value);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
            break;
    }
}

static void info_daemon_class_init (InfoDaemonClass *class)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (class);

    gobject_class->set_property = info_daemon_set_property;

    gobject_class->constructed = info_daemon_constructed;
    gobject_class->dispose = info_daemon_dispose;

    properties[PROP_LOOP] =
        g_param_spec_pointer("loop", "loop", "loop",
                G_PARAM_CONSTRUCT_ONLY | G_PARAM_WRITABLE |
                G_PARAM_STATIC_STRINGS);
    properties[PROP_REPLACE] =
        g_param_spec_boolean ("replace", "replace", "replace", FALSE,
                G_PARAM_CONSTRUCT_ONLY | G_PARAM_WRITABLE |
                G_PARAM_STATIC_STRINGS);

    g_object_class_install_properties (gobject_class, LAST_PROP, properties);
}

static void info_daemon_init (InfoDaemon *daemon)
{
#if 0
    cpu_info_t *cpu=NULL;
    cpu = get_cpu_info();
    if(cpu==NULL)
    {
        printf("cpu info is no support!\n");
    }
    printf("cpu_name: %s\n",cpu->cpu_name);
    printf("cpu_idh: 0x%llx\n",cpu->id.h);
    printf("cpu_idl: 0x%llx\n",cpu->id.l);
    printf("cpu_ver: %s\n",cpu->cpu_ver);
    printf("technics: %s\n",cpu->technics);
    printf("cpu_tdp: %s\n",cpu->cpu_tdp);
    printf("cpu_jt_l: %s\n",cpu->cpu_jt_l);
    printf("cpu_jt_h: %s\n",cpu->cpu_jt_h);
    printf("cpu_pkg: %s\n",cpu->cpu_pkg);
    printf("cpu_l: %s\n",cpu->cpu_l);
    printf("cpu_w: %s\n",cpu->cpu_w);
    printf("cpu_h: %s\n",cpu->cpu_h);
    printf("cacheL1d: %s\n",cpu->cacheL1d);
    printf("cacheL1i: %s\n",cpu->cacheL1i);
    printf("cacheL2: %s\n",cpu->cacheL2);
    printf("cacheL3: %s\n",cpu->cacheL3);
    printf("microarch: %s\n",cpu->microarch);

    printf("cpu_crrent_speed: %s\n",get_cpu_current_speed());
    printf("cpu_max_speed: %s\n",get_cpu_max_speed());
    printf("cpu_min_speed: %s\n",get_cpu_min_speed());
    printf("cpu_arch: %s\n",get_cpu_arch());
    printf("cpu_core_num: %s\n",get_cpu_core_num());
    printf("cpu_thread_num: %s\n",get_cpu_thread_num());
    printf("product_name: %s\n",get_product_name());
#endif
    daemon->skeleton = bus_info_skeleton_new();
    //daemon->info = info_new();
}

InfoDaemon* info_daemon_new (GMainLoop *loop, gboolean replace)
{
    return g_object_new (INFO_TYPE_DAEMON, "loop", loop, NULL);
}
