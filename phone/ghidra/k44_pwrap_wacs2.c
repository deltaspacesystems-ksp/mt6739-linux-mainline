//// pwrap_wacs2_hal @ffffff8008514b5c

undefined4 pwrap_wacs2_hal(uint param_1,ulong param_2,uint param_3,uint *param_4)

{
  uint *puVar1;
  undefined4 *puVar2;
  uint uVar3;
  undefined8 uVar4;
  undefined4 uVar5;

  uVar5 = 2;
  if ((param_1 & 0xfffffffe) == 0) {
    uVar5 = 3;
    if (((param_2 & 0xffff0000) == 0) && (uVar5 = 4, (param_3 & 0xffff0000) == 0)) {
      if (((param_1 & (int)param_2 == 0xf8c) != 0) && ((param_3 >> 3 & 1) == 0)) {
        printk(0xffffff8008e27517,0x1c3);
        aee_kernel_warning_api(0xffffff8008e27486,0x1c4,0,0xffffff8008e2756b,0xffffff8008e27547);
      }
      uVar4 = _raw_spin_lock_irqsave(0xffffff80095e0f18);
      puVar1 = (uint *)(lRamffffff80095e0f08 + 0xc24);
      puVar2 = (undefined4 *)(lRamffffff80095e0f08 + 0xc28);
      do {
        while( true ) {
          if ((*puVar1 >> 0x15 & 1) == 0) {
            uVar5 = 7;
            printk(0xffffff8008e2757f,0x16f);
            printk(0xffffff8008e275ac,0x1d0,7);
            goto LAB_ffffff8008514c44;
          }
          uVar3 = *puVar1 >> 0x10 & 7;
          if (uVar3 != 6) break;
          *puVar2 = 1;
          DataSynchronizationBarrier(3,3,0);
        }
      } while (uVar3 != 0);
      *(uint *)(lRamffffff80095e0f08 + 0xc20) =
           param_1 << 0x1f | (int)((param_2 & 0xffffffff) >> 1) << 0x10 | param_3;
      DataSynchronizationBarrier(3,3,0);
      uVar5 = 0;
      if (param_1 == 0) {
        if (param_4 == (uint *)0x0) {
          uVar5 = 1;
          printk(0xffffff8008e275da,0x1da);
        }
        else {
          do {
            uVar3 = *(uint *)(lRamffffff80095e0f08 + 0xc24);
            if ((uVar3 >> 0x15 & 1) == 0) {
              uVar5 = 8;
              printk(0xffffff8008e275fe,0x1e2,7);
              goto LAB_ffffff8008514c44;
            }
          } while ((uVar3 >> 0x10 & 7) != 6);
          *param_4 = uVar3 & 0xffff;
          *(undefined4 *)(lRamffffff80095e0f08 + 0xc28) = 1;
          DataSynchronizationBarrier(3,3,0);
          uVar5 = 0;
        }
      }
LAB_ffffff8008514c44:
      _raw_spin_unlock_irqrestore(0xffffff80095e0f18,uVar4);
    }
  }
  return uVar5;
}
