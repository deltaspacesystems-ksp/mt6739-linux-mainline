//// MTKEnableMfgClock.isra.3 @ffffff8008672964

void MTKEnableMfgClock_isra_3(void)

{
  long lVar1;
  int iVar2;

  ged_log_buf_print2(uRamffffff8009662260,0x100,0xffffff8008e84543);
  mt_gpufreq_voltage_enable_set(1);
  ged_dvfs_gpu_clock_switch_notify(1);
  lVar1 = lRamffffff8009662250;
  if (lRamffffff8009662250 != 0) {
    iVar2 = clk_prepare(lRamffffff8009662250);
    if ((iVar2 == 0) && (iVar2 = clk_enable(lVar1), iVar2 != 0)) {
      clk_unprepare(lVar1);
    }
    if ((lRamffffff8009662250 != 0) && (lRamffffff8009662268 != 0)) {
      clk_set_parent();
    }
  }
  lVar1 = lRamffffff8009662248;
  if (((lRamffffff8009662248 != 0) && (iVar2 = clk_prepare(lRamffffff8009662248), iVar2 == 0)) &&
     (iVar2 = clk_enable(lVar1), iVar2 != 0)) {
    clk_unprepare(lVar1);
  }
  if (iRamffffff8009662238 != 0) {
    PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e8454b);
  }
  mtk_notify_gpu_power_change(1);
  return;
}


//// MTKDisableMfgClock.isra.1 @ffffff80086728a4

void MTKDisableMfgClock_isra_1(void)

{
  long lVar1;

  mtk_notify_gpu_power_change(0);
  lVar1 = lRamffffff8009662248;
  if (lRamffffff8009662248 != 0) {
    clk_disable(lRamffffff8009662248);
    clk_unprepare(lVar1);
  }
  if ((lRamffffff8009662250 != 0) && (lRamffffff8009662258 != 0)) {
    clk_set_parent();
  }
  lVar1 = lRamffffff8009662250;
  if (lRamffffff8009662250 != 0) {
    clk_disable(lRamffffff8009662250);
    clk_unprepare(lVar1);
  }
  ged_dvfs_gpu_clock_switch_notify(0);
  ged_log_buf_print2(uRamffffff8009662260,0x100,0xffffff8008e84527);
  mt_gpufreq_voltage_enable_set(0);
  if (iRamffffff8009662238 != 0) {
    PVRSRVDebugPrintf(2,0xffffff8008dc29e3,0,0xffffff8008e84530);
  }
  return;
}


//// MTKDevPrePowerState @ffffff8008672b78

undefined8 MTKDevPrePowerState(undefined8 param_1,int param_2,int param_3)

{
  long lVar1;

  if (param_2 == 0 && param_3 == 1) {
    if (uRamffffff8009662274 == 0) {
      lVar1 = MTKGetRGXDevNode();
      uRamffffff8009662274 = 0;
      if (lVar1 != 0) {
        uRamffffff8009662274 = (uint)(*(int *)(lVar1 + 0x18) == 2);
      }
    }
    MTKDisableMfgClock_isra_1();
    return 0;
  }
  return 0;
}


//// MTKDevPostPowerState @ffffff8008672be0

undefined8 MTKDevPostPowerState(undefined8 param_1,int param_2,int param_3)

{
  long lVar1;

  if (param_3 == 0 && param_2 == 1) {
    MTKEnableMfgClock_isra_3();
    MTKInitHWAPM();
    if (uRamffffff8009662274 == 0) {
      lVar1 = MTKGetRGXDevNode();
      uRamffffff8009662274 = 0;
      if (lVar1 != 0) {
        uRamffffff8009662274 = (uint)(*(int *)(lVar1 + 0x18) == 2);
      }
    }
    if (iRamffffff800966223c == 1) {
      mt_gpufreq_target(uRamffffff8009662240);
      iRamffffff800966223c = 0;
    }
    return 0;
  }
  return 0;
}


//// mt_gpufreq_voltage_enable_set @ffffff8008445678

int mt_gpufreq_voltage_enable_set(int param_1)

{
  int iVar1;
  undefined8 uVar2;

  mutex_lock(0xffffff800943ab90);
  iVar1 = -1;
  if (cRamffffff80095c2de1 == '\0') goto LAB_ffffff8008445720;
  if (param_1 == 1) {
    if (uRamffffff800943ab8c == 0x1a8ce) {
      uVar2 = 2;
LAB_ffffff8008445764:
      iVar1 = vcorefs_request_dvfs_opp(8,uVar2);
      if (iVar1 != 0) goto LAB_ffffff8008445774;
    }
    else {
      if (0x1a8ce < uRamffffff800943ab8c) {
        if (uRamffffff800943ab8c == 0x1b503) {
          uVar2 = 1;
        }
        else {
          uVar2 = 0;
          if (uRamffffff800943ab8c != 0x1c138) goto LAB_ffffff80084456e8;
        }
        goto LAB_ffffff8008445764;
      }
      uVar2 = 3;
      if (uRamffffff800943ab8c == 0x19a28) goto LAB_ffffff8008445764;
LAB_ffffff80084456e8:
      iVar1 = 0;
      printk(0xffffff8008df9579,0xffffff8008bc1928);
    }
LAB_ffffff8008445704:
    if ((uRamffffff80095c2df8 & 1) == 0) {
      _mt_gpufreq_kick_pbm(1);
      iRamffffff80095c2de4 = param_1;
    }
    else {
      cancel_delayed_work(0xffffff80095c2df8);
      iRamffffff80095c2de4 = param_1;
    }
  }
  else {
    iVar1 = vcorefs_request_dvfs_opp(8,0xffffffff);
    if (iVar1 != 0) {
LAB_ffffff8008445774:
      printk(0xffffff8008df95b4,0xffffff8008bc1928,uRamffffff800943ab8c);
      if (param_1 == 1) goto LAB_ffffff8008445704;
    }
    queue_delayed_work_on(10,uRamffffff80093cff88,0xffffff80095c2df8,0x1e);
    iRamffffff80095c2de4 = param_1;
  }
LAB_ffffff8008445720:
  mutex_unlock(0xffffff800943ab90);
  return iVar1;
}


//// mt_gpufreq_voltage_lpm_set @ffffff800844538c

int mt_gpufreq_voltage_lpm_set(int param_1)

{
  int iVar1;
  undefined8 uVar2;

  mutex_lock(0xffffff800943ab90);
  if (cRamffffff80095c2de1 == '\0') {
    iVar1 = -1;
    printk(0xffffff8008df9544,0xffffff8008bc18f0);
    goto LAB_ffffff80084453d8;
  }
  if (param_1 == 0) {
    if (uRamffffff800943ab8c == 0x1a8ce) {
      uVar2 = 2;
LAB_ffffff80084454b4:
      iVar1 = vcorefs_request_dvfs_opp(8,uVar2);
      if (iVar1 != 0) goto LAB_ffffff8008445428;
    }
    else {
      if (0x1a8ce < uRamffffff800943ab8c) {
        if (uRamffffff800943ab8c == 0x1b503) {
          uVar2 = 1;
        }
        else {
          uVar2 = 0;
          if (uRamffffff800943ab8c != 0x1c138) goto LAB_ffffff8008445478;
        }
        goto LAB_ffffff80084454b4;
      }
      uVar2 = 3;
      if (uRamffffff800943ab8c == 0x19a28) goto LAB_ffffff80084454b4;
LAB_ffffff8008445478:
      iVar1 = 0;
      printk(0xffffff8008df9579,0xffffff8008bc18f0);
    }
LAB_ffffff80084454c4:
    uRamffffff80095c2de4 = 1;
    goto LAB_ffffff80084453d8;
  }
  if (cRamffffff80095c2de2 == '\0') {
    iVar1 = vcorefs_request_dvfs_opp(8,0xffffffff);
    if (iVar1 == 0) goto LAB_ffffff8008445404;
LAB_ffffff8008445428:
    printk(0xffffff8008df95b4,0xffffff8008bc18f0,uRamffffff800943ab8c);
    if (param_1 == 0) goto LAB_ffffff80084454c4;
  }
  else {
LAB_ffffff8008445404:
    iVar1 = 0;
  }
  uRamffffff80095c2de4 = 0;
LAB_ffffff80084453d8:
  mutex_unlock(0xffffff800943ab90);
  return iVar1;
}


//// _mt_gpufreq_get_cur_volt @ffffff80084454e4

int _mt_gpufreq_get_cur_volt(void)

{
  int iVar1;

  iVar1 = regulator_get_voltage(*(undefined8 *)(lRamffffff80095c2de8 + 0x10));
  return iVar1 / 10;
}


//// mt_gpufreq_clock_switch @ffffff8008447238

void mt_gpufreq_clock_switch(uint param_1)

{
  uint uVar1;

  _mt_gpufreq_get_cur_volt();
  _mt_gpufreq_get_cur_freq();
  if (param_1 - 290000 < 0x445c1) {
    uVar1 = ((param_1 / 100 << 0x10) / 0x1a + 5) / 10;
  }
  else {
    printk(0xffffff8008df9d80,0xffffff8008bc1ab0,param_1);
    uVar1 = 0;
  }
  mt_dfs_general_pll(3,uVar1);
  if (pcRamffffff80095c2dd0 != (code *)0x0) {
    (*pcRamffffff80095c2dd0)(param_1);
  }
  return;
}
