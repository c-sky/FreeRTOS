/*
 *  Copyright (C) 2016 YunOS Project. All rights reserved.
 */

int g_critical_flags;

void os_critical_enter(void)
{
  //  g_critical_flags = yunos_cpu_intrpt_save();
}

void os_critical_exit(void)
{
  //  yunos_cpu_intrpt_restore(g_critical_flags);
}

