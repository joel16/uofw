#include <common_imp.h>
#include <threadman_kernel.h>

SCE_MODULE_INFO("sceChkreg", SCE_MODULE_KERNEL | SCE_MODULE_KIRK_SEMAPHORE_LIB | SCE_MODULE_ATTR_EXCLUSIVE_LOAD
                              | SCE_MODULE_ATTR_EXCLUSIVE_START, 1, 9);
SCE_SDK_VERSION(SDK_VERSION);

typedef struct {
    SceUID sema;
} g_chkreg_struct;

g_chkreg_struct g_chkreg = {0};

// Declarations
s32 sceIdStorageReadLeaf(u16 key, void *buf);
s32 sceIdStorageLookup(u16 key, u32 offset, void *buf, u32 len);
s32 sceUtilsBufferCopyWithRange(void* outBuf, s32 outsize, void* inBuf, s32 insize, s32 cmd);

s32 sub_00000000(void)
{
    return 0;
}

s32 sub_00000128(s32 unk1, s32 unk2)
{
    return 0;
}

// Subroutine sub_00000190 - Address 0x00000190 
s32 sub_00000190(void)
{
    if (sceIdStorageLookup(0x100, 0x38, 0xa80, 0xb8) < 0)
        return SCE_ERROR_NOT_FOUND;
        
    if (sceIdStorageLookup(0x120, 0x38, 0xa80, 0xb8) < 0)
        return SCE_ERROR_NOT_FOUND;
        
    return SCE_ERROR_OK;
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
s32 sceChkregGetPsCode(u8 *code)
{
    return 0;
}

// Subroutine sceChkreg_driver_54495B19 - Address 0x00000390
s32 sceChkregCheckRegion(u32 unk1, u32 unk2)
{
    s32 ret = SCE_ERROR_SEMAPHORE;
    
    if (sceKernelWaitSema(g_chkreg.sema, 1, NULL) == 0)
    {
        if (/*(DAT_00000044 != 0) ||*/ (ret = sub_00000000(), ret == 0))
            ret = sub_00000128(0x80000000, (unk1 | unk2));
        
        if (sceKernelSignalSema(g_chkreg.sema, 1) != 0)
            ret = SCE_ERROR_SEMAPHORE;
    }

    return ret;
}

s32 sceChkreg_driver_6894A027(void) {
    return 0;
}

s32 sceChkreg_driver_7939C851(void) {
    return 0;
}

s32 sceChkreg_driver_9C6E1D34(void) {
    return 0;
}
