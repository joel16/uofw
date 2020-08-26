#include <common_imp.h>

SCE_MODULE_INFO("sceChkreg", SCE_MODULE_KERNEL | SCE_MODULE_KIRK_SEMAPHORE_LIB | SCE_MODULE_ATTR_EXCLUSIVE_LOAD
                              | SCE_MODULE_ATTR_EXCLUSIVE_START, 1, 9);
SCE_SDK_VERSION(SDK_VERSION);

// Subroutine sub_00000190 - Address 0x00000190 
s32 sub_00000190(void)
{
    s32 ret = SCE_ERROR_OK;
    ret = sceIdStorageLookup(0x100, 0x38, 0xa80, 0xb8);
    if (ret < 0)
        return SCE_ERROR_NOT_FOUND;
    
    ret = sceIdStorageLookup(0x120, 0x38, 0xa80, 0xb8);
    if (ret < 0)
        return SCE_ERROR_NOT_FOUND;
        
    return ret;
}

// Subroutine sub_0000020C - Address 0x0000020C
s32 sub_0000020C(void)
{
    s32 ret = SCE_ERROR_OK;
    ret = sceUtilsBufferCopyWithRange(0, 0, 0xa80, 0xb8, 0x12);
    if (ret != 0)
        ret = SCE_ERROR_INVALID_FORMAT;
        
    return ret;
}


// Subroutine module_start - Address 0x00000248
s32 module_start(SceSize args __attribute__((unused)), void *argp __attribute__((unused)))
{
    return 0;
}

// Subroutine module_stop - Address 0x000002E0
s32 module_stop(SceSize args __attribute__((unused)), void *argp __attribute__((unused)))
{
    return 0;
}

// Subroutine sceChkreg_driver_59F8491D - Address 0x00000438
s32 sceChkregGetPsCode(u8 *code) {

}
