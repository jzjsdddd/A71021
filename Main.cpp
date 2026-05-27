#include <list>
#include <vector>
#include <cstring>
#include <pthread.h>
#include <thread>
#include <string>
#include <jni.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <dlfcn.h>
#include "Includes/Logger.h"
#include "Includes/obfuscate.h"
#include "Includes/Utils.hpp"
#include "Menu/Menu.hpp"
#include "Menu/Jni.hpp"
#include "Includes/Macros.h"
#include "TuanMeta/Call_Me.h"

#define targetLibName OBFUSCATE("libil2cpp.so")

// تعريف المتغيرات الناقصة (افتراضيًا)
bool il2cppLoaded = false;
bool dumpEnabled = false;
bool dumped = false;

struct MemPatches {
    MemoryPatch ipad_patch1, ipad_patch2, ipad_patch3;
    MemoryPatch stopAI_patch1, stopAI_patch2, stopAI_patch3, stopAI_patch4;
    MemoryPatch multi_patch1, multi_patch2;
    MemoryPatch box_patch1;
    MemoryPatch smootha_patch1, smootha_patch2, smootha_patch3;
    MemoryPatch longRange2_patch1, longRange2_patch2, longRange2_patch3, longRange2_patch4;
    MemoryPatch game_patch2;
    MemoryPatch longRange15_patch1,  longRange15_patch2,  longRange15_patch3,
            longRange15_patch4,  longRange15_patch5,  longRange15_patch6,
            longRange15_patch7,  longRange15_patch8,  longRange15_patch9,
            longRange15_patch10, longRange15_patch11, longRange15_patch12,
            longRange15_patch13, longRange15_patch14, longRange15_patch15;
    MemoryPatch game_patch3;
    MemoryPatch game_patch5;
    MemoryPatch bypass_patch1, bypass_patch2, bypass_patch3, bypass_patch4, bypass_patch5,
            bypass_patch6, bypass_patch7, bypass_patch8, bypass_patch9, bypass_patch10,
            bypass_patch11, bypass_patch12, bypass_patch13, bypass_patch14, bypass_patch15,
            bypass_patch16, bypass_patch17, bypass_patch18, bypass_patch19, bypass_patch20,
            bypass_patch21, bypass_patch22, bypass_patch23, bypass_patch24, bypass_patch25,
            bypass_patch26, bypass_patch27, bypass_patch28, bypass_patch29,
            bypass_patch30, bypass_patch31, bypass_patch32, bypass_patch33, bypass_patch34;
    MemoryPatch speedPlayer_patch1;
    MemoryPatch bypasso_patch1, bypasso_patch2, bypasso_patch3, bypasso_patch4, bypasso_patch5,
            bypasso_patch6, bypasso_patch7, bypasso_patch8, bypasso_patch9, bypasso_patch10,
            bypasso_patch11, bypasso_patch12, bypasso_patch13, bypasso_patch14, bypasso_patch15,
            bypasso_patch16, bypasso_patch17, bypasso_patch18, bypasso_patch19, bypasso_patch20,
            bypasso_patch21, bypasso_patch22, bypasso_patch23, bypasso_patch24, bypasso_patch25,
            bypasso_patch26, bypasso_patch27, bypasso_patch28, bypasso_patch29, bypasso_patch30,
            bypasso_patch31, bypasso_patch32, bypasso_patch33, bypasso_patch34, bypasso_patch35, bypasso_patch36, bypasso_patch37;
            
            MemoryPatch normalAura_patch1, normalAura_patch2, normalAura_patch3, normalAura_patch4, normalAura_patch5;

    MemoryPatch smooth_patch1, smooth_patch2;
    MemoryPatch Galaxya_patch1, Galaxya_patch2;
} feature0;

ProcMap il2cppMap;
ProcMap unityMap;
ElfScanner g_il2cppELF;

void *hack_thread(void *) {
    LOGI(OBFUSCATE("pthread created"));

    // التحقق من تحميل المكتبات
    int attempts = 0;
    const int max_attempts = 100;
    do {
        il2cppMap = KittyMemory::getElfBaseMap("libil2cpp.so");
        unityMap = KittyMemory::getElfBaseMap("libunity.so");
        if (!il2cppMap.isValid() || !unityMap.isValid()) {
            LOGI(OBFUSCATE("Waiting for libraries to load... Attempt %d/%d"), attempts + 1, max_attempts);
            sleep(0);
            attempts++;
        }
    } while ((!il2cppMap.isValid() || !unityMap.isValid()) && attempts < max_attempts);

    if (!il2cppMap.isValid() || !unityMap.isValid()) {
        LOGE(OBFUSCATE("Failed to load libraries after %d attempts"), max_attempts);
        return NULL;
    }

    LOGI(OBFUSCATE("%s has been loaded"), (const char *)targetLibName);

    // فتح مكتبة libil2cpp.so باستخدام dlopen
    void *il2cpp_handle = dlopen(targetLibName, RTLD_NOW);
    if (!il2cpp_handle) {
        LOGE(OBFUSCATE("Failed to open %s: %s"), (const char *)targetLibName, dlerror());
        return NULL;
    }

    // التحقق من صلاحية ELF
    g_il2cppELF = ElfScanner::createWithPath(targetLibName);
    if (!g_il2cppELF.isValid()) {
        LOGE(OBFUSCATE("Failed to create ElfScanner for %s"), (const char *)targetLibName);
        dlclose(il2cpp_handle);
        return NULL;
    }

    il2cppLoaded = true;

    // تهيئة الـ MemoryPatch
    feature0.longRange2_patch1 = MemoryPatch::createWithHex(unityMap, 0xfbfb68, "0000C040");
    feature0.longRange2_patch2 = MemoryPatch::createWithHex(unityMap, 0xfbfb60, "0000C040");
    feature0.longRange2_patch3 = MemoryPatch::createWithHex(unityMap, 0xfbfb64, "0000C040");
    feature0.longRange2_patch4 = MemoryPatch::createWithHex(unityMap, 0xfbfb5c, "0000C040");
    
    feature0.longRange15_patch1  = MemoryPatch::createWithHex(unityMap, 0xfbfb68, "0000C040");
    feature0.longRange15_patch2  = MemoryPatch::createWithHex(unityMap, 0xfbfb64, "0000C040");
    feature0.longRange15_patch3  = MemoryPatch::createWithHex(unityMap, 0xfbfb60, "0000C040");
    feature0.longRange15_patch4  = MemoryPatch::createWithHex(unityMap, 0xfbfb5c, "0000C040");
    feature0.longRange15_patch5  = MemoryPatch::createWithHex(unityMap, 0xfbfb58, "0000C040");
    feature0.longRange15_patch6  = MemoryPatch::createWithHex(unityMap, 0xfbfb54, "0000C040");
    feature0.longRange15_patch7  = MemoryPatch::createWithHex(unityMap, 0xfbfb50, "0000C040");
    feature0.longRange15_patch8  = MemoryPatch::createWithHex(unityMap, 0xfbfb4c, "0000C040");
    feature0.longRange15_patch9  = MemoryPatch::createWithHex(unityMap, 0xfbfb48, "0000C040");
    feature0.longRange15_patch10 = MemoryPatch::createWithHex(unityMap, 0xfbfb44, "0000C040");
    feature0.longRange15_patch11 = MemoryPatch::createWithHex(unityMap, 0xfbfb40, "0000C040");
    feature0.longRange15_patch12 = MemoryPatch::createWithHex(unityMap, 0xfbfb3c, "0000C040");
    feature0.longRange15_patch13 = MemoryPatch::createWithHex(unityMap, 0xfbfb38, "0000C040");
    feature0.longRange15_patch14 = MemoryPatch::createWithHex(unityMap, 0xfbfb34, "0000C040");
    feature0.longRange15_patch15 = MemoryPatch::createWithHex(unityMap, 0xfbfb30, "0000C040");

    feature0.bypass_patch1 = MemoryPatch::createWithHex(il2cppMap, 0x188B0F0, "C0035FD6");
    feature0.bypass_patch2 = MemoryPatch::createWithHex(il2cppMap, 0x1A34FFC, "C0035FD6");
    feature0.bypass_patch3 = MemoryPatch::createWithHex(il2cppMap, 0x188A8F8, "C0035FD6");
    feature0.bypass_patch4 = MemoryPatch::createWithHex(il2cppMap, 0x188A8EC, "C0035FD6");
    feature0.bypass_patch5 = MemoryPatch::createWithHex(il2cppMap, 0x18579F0, "C0035FD6");
    feature0.bypass_patch6 = MemoryPatch::createWithHex(il2cppMap, 0x1582370, "C0035FD6");
    feature0.bypass_patch7 = MemoryPatch::createWithHex(il2cppMap, 0x15820C0, "C0035FD6");
    feature0.bypass_patch8 = MemoryPatch::createWithHex(il2cppMap, 0x15820C8, "C0035FD6");
    feature0.bypass_patch9 = MemoryPatch::createWithHex(il2cppMap, 0x1A9424C, "C0035FD6");
    feature0.bypass_patch10 = MemoryPatch::createWithHex(il2cppMap, 0x198C5CC, "C0035FD6");
    feature0.bypass_patch11 = MemoryPatch::createWithHex(il2cppMap, 0x1584DDC, "C0035FD6");
    
   
    feature0.bypass_patch12 = MemoryPatch::createWithHex(il2cppMap, 0x1584B30, "C0035FD6");
    feature0.bypass_patch13 = MemoryPatch::createWithHex(il2cppMap, 0x1584B28, "C0035FD6");
    feature0.bypass_patch14 = MemoryPatch::createWithHex(il2cppMap, 0x1582AC8, "C0035FD6");
    
    
    feature0.bypass_patch15 = MemoryPatch::createWithHex(il2cppMap, 0x1582A70, "C0035FD6");
    feature0.bypass_patch16 = MemoryPatch::createWithHex(il2cppMap, 0x188A8E4, "C0035FD6");
    feature0.bypass_patch17 = MemoryPatch::createWithHex(il2cppMap, 0x1589770, "C0035FD6");
    feature0.bypass_patch18 = MemoryPatch::createWithHex(il2cppMap, 0x1589204, "C0035FD6");
    feature0.bypass_patch20 = MemoryPatch::createWithHex(il2cppMap, 0x1584F3C, "C0035FD6");
    feature0.bypass_patch21 = MemoryPatch::createWithHex(il2cppMap, 0x188A8EC, "C0035FD6");
    feature0.bypass_patch22 = MemoryPatch::createWithHex(il2cppMap, 0x188A8F8, "C0035FD6");
    feature0.bypass_patch23 = MemoryPatch::createWithHex(il2cppMap, 0x188A900, "C0035FD6");
    feature0.bypass_patch24 = MemoryPatch::createWithHex(il2cppMap, 0x188B0F0, "C0035FD6");
    feature0.bypass_patch25 = MemoryPatch::createWithHex(il2cppMap, 0x188A8EC, "C0035FD6");
    feature0.bypass_patch26 = MemoryPatch::createWithHex(il2cppMap, 0x188A8F8, "C0035FD6");
    feature0.bypass_patch27 = MemoryPatch::createWithHex(il2cppMap, 0x188B0F0, "C0035FD6");
    feature0.bypass_patch28 = MemoryPatch::createWithHex(il2cppMap, 0x198C5CC, "C0035FD6");
    feature0.bypass_patch29 = MemoryPatch::createWithHex(il2cppMap, 0x18579F0, "C0035FD6");
    feature0.bypass_patch30 = MemoryPatch::createWithHex(il2cppMap, 0x14A42A0, "C0035FD6");
    feature0.bypass_patch31 = MemoryPatch::createWithHex(il2cppMap, 0x1582A70, "C0035FD6");
    feature0.bypass_patch32 = MemoryPatch::createWithHex(il2cppMap, 0x1582AC8, "C0035FD6");
    feature0.bypass_patch33 = MemoryPatch::createWithHex(il2cppMap, 0x1A9424C, "C0035FD6");
    feature0.bypass_patch34 = MemoryPatch::createWithHex(il2cppMap, 0x1582370, "C0035FD6");

    feature0.bypasso_patch1 = MemoryPatch::createWithHex(il2cppMap, 0xFBA2F8, "00 00 80 D2 C0 03 5F D6");
    feature0.bypasso_patch2 = MemoryPatch::createWithHex(il2cppMap, 0xFBB174, "00 00 80 D2 C0 03 5F D6");
    feature0.bypasso_patch3 = MemoryPatch::createWithHex(il2cppMap, 0xFBCE88, "00 00 80 D2 C0 03 5F D6");
    feature0.bypasso_patch4 = MemoryPatch::createWithHex(il2cppMap, 0xFBD8E4, "00 00 80 D2 C0 03 5F D6");
    feature0.bypasso_patch5 = MemoryPatch::createWithHex(il2cppMap, 0xFC0D18, "00 00 80 D2 C0 03 5F D6");
    //وقفنا خمسه
    feature0.bypasso_patch6 = MemoryPatch::createWithHex(il2cppMap, 0xF93C5C, "00 00 80 D2 C0 03 5F D6");
    feature0.bypasso_patch7 = MemoryPatch::createWithHex(il2cppMap, 0xF952C8, "00 00 80 D2 C0 03 5F D6");
    feature0.bypasso_patch8 = MemoryPatch::createWithHex(il2cppMap, 0xF974B8, "00 00 80 D2 C0 03 5F D6");
    feature0.bypasso_patch9 = MemoryPatch::createWithHex(il2cppMap, 0xF97964, "00 00 80 D2 C0 03 5F D6");
    feature0.bypasso_patch10 = MemoryPatch::createWithHex(il2cppMap, 0x1E9744C, "00 00 80 D2 C0 03 5F D6");
    feature0.bypasso_patch11 = MemoryPatch::createWithHex(il2cppMap, 0x1EA4910, "00 00 80 D2 C0 03 5F D6");
    feature0.bypasso_patch12 = MemoryPatch::createWithHex(il2cppMap, 0xF9B324, "00 00 80 D2 C0 03 5F D6");
    feature0.bypasso_patch13 = MemoryPatch::createWithHex(il2cppMap, 0xF93DF8, "00 00 80 D2 C0 03 5F D6");
    feature0.bypasso_patch14 = MemoryPatch::createWithHex(il2cppMap, 0xF95334, "00 00 80 D2 C0 03 5F D6");
    feature0.bypasso_patch15 = MemoryPatch::createWithHex(il2cppMap, 0xF96CF4, "00 00 80 D2 C0 03 5F D6");
    feature0.bypasso_patch16 = MemoryPatch::createWithHex(il2cppMap, 0xF97A30, "00 00 80 D2 C0 03 5F D6");
    feature0.bypasso_patch17 = MemoryPatch::createWithHex(il2cppMap, 0xF9A9AC, "00 00 80 D2 C0 03 5F D6");
    feature0.bypasso_patch18 = MemoryPatch::createWithHex(il2cppMap, 0xF93B84, "00 00 80 D2 C0 03 5F D6");
    feature0.bypasso_patch19 = MemoryPatch::createWithHex(il2cppMap, 0xF951F4, "00 00 80 D2 C0 03 5F D6");
    feature0.bypasso_patch20 = MemoryPatch::createWithHex(il2cppMap, 0xF96B68, "00 00 80 D2 C0 03 5F D6");
    feature0.bypasso_patch21 = MemoryPatch::createWithHex(il2cppMap, 0xF9788C, "00 00 80 D2 C0 03 5F D6");
    feature0.bypasso_patch22 = MemoryPatch::createWithHex(il2cppMap, 0xF9A810, "00 00 80 D2 C0 03 5F D6");
    feature0.bypasso_patch23 = MemoryPatch::createWithHex(il2cppMap, 0xF94BC4, "00 00 80 D2 C0 03 5F D6");
    feature0.bypasso_patch24 = MemoryPatch::createWithHex(il2cppMap, 0xF95A3C, "00 00 80 D2 C0 03 5F D6");
    feature0.bypasso_patch25 = MemoryPatch::createWithHex(il2cppMap, 0xF9775C, "00 00 80 D2 C0 03 5F D6");
    feature0.bypasso_patch26 = MemoryPatch::createWithHex(il2cppMap, 0xF981B0, "00 00 80 D2 C0 03 5F D6");
    feature0.bypasso_patch27 = MemoryPatch::createWithHex(il2cppMap, 0xF9B5EC, "00 00 80 D2 C0 03 5F D6");
    feature0.bypasso_patch28 = MemoryPatch::createWithHex(il2cppMap, 0xF94BB8, "00 00 80 D2 C0 03 5F D6");
    feature0.bypasso_patch29 = MemoryPatch::createWithHex(il2cppMap, 0xF95A34, "00 00 80 D2 C0 03 5F D6");
    feature0.bypasso_patch30 = MemoryPatch::createWithHex(il2cppMap, 0xF97748, "00 00 80 D2 C0 03 5F D6");
    feature0.bypasso_patch31 = MemoryPatch::createWithHex(il2cppMap, 0xF981A4, "00 00 80 D2 C0 03 5F D6");
    feature0.bypasso_patch32 = MemoryPatch::createWithHex(il2cppMap, 0xF9B5D8, "00 00 80 D2 C0 03 5F D6");
    feature0.bypasso_patch33 = MemoryPatch::createWithHex(il2cppMap, 0x185DF5C, "00 00 80 D2 C0 03 5F D6");
    feature0.bypasso_patch34 = MemoryPatch::createWithHex(il2cppMap, 0x185E754, "00 00 80 D2 C0 03 5F D6");
    feature0.bypasso_patch35 = MemoryPatch::createWithHex(il2cppMap, 0x154D458, "00 00 80 D2 C0 03 5F D6");
    feature0.bypasso_patch36 = MemoryPatch::createWithHex(il2cppMap, 0x154CAA8, "00 00 80 D2 C0 03 5F D6");
    feature0.bypasso_patch37 = MemoryPatch::createWithHex(il2cppMap, 0xF96C48, "00 00 80 D2 C0 03 5F D6");

    feature0.game_patch2 = MemoryPatch::createWithHex(il2cppMap, 0x34E3F84, "4066861280B9A7720000271EC0035FD6");
    feature0.game_patch3 = MemoryPatch::createWithHex(il2cppMap, 0x34E3F84, "A0CC8C1220E3A7720000271EC0035FD6");
    feature0.game_patch5 = MemoryPatch::createWithHex(il2cppMap, 0x34E3F84, "0010211EC0035FD6");
    //لاسقوط
    feature0.smootha_patch1 = MemoryPatch::createWithHex(il2cppMap, 0x19DC9F8, "C0035FD6");
    feature0.smootha_patch2 = MemoryPatch::createWithHex(il2cppMap, 0x19E7620, "C0035FD6");
    feature0.smootha_patch3 = MemoryPatch::createWithHex(il2cppMap, 0x1A8DD80, "C0035FD6");
    
    
    
    feature0.normalAura_patch1 = MemoryPatch::createWithHex(il2cppMap, 0x17DD6E8, "20 00 80 52 C0 03 5F D6");
    feature0.normalAura_patch2 = MemoryPatch::createWithHex(il2cppMap, 0x17F06F4, "20 00 80 52 C0 03 5F D6");
    feature0.normalAura_patch3 = MemoryPatch::createWithHex(il2cppMap, 0x1815CB8, "20 00 80 52 C0 03 5F D6");
    feature0.normalAura_patch4 = MemoryPatch::createWithHex(il2cppMap, 0x180BEB0, "20 00 80 52 C0 03 5F D6");
   // feature0.normalAura_patch5 = MemoryPatch::createWithHex(il2cppMap, 0x17E1928, "20 00 80 52 C0 03 5F D6");
    
    feature0.Galaxya_patch1 = MemoryPatch::createWithHex(il2cppMap, 0x1893BA8, "C0035FD6");
    feature0.Galaxya_patch2 = MemoryPatch::createWithHex(il2cppMap, 0x18938BC, "C0035FD6");
    
    feature0.speedPlayer_patch1 = MemoryPatch::createWithHex(il2cppMap,0x1a76b1c, "00 F0 21 1E");
    
    feature0.ipad_patch1 = MemoryPatch::createWithHex(il2cppMap,0x17DAA80, "C0035FD6");
    feature0.ipad_patch2 = MemoryPatch::createWithHex(il2cppMap,0x17CC8A4, "C0035FD6");
    feature0.ipad_patch3 = MemoryPatch::createWithHex(il2cppMap,0x34E42FC, "C0035FD6");
    
    feature0.stopAI_patch1 = MemoryPatch::createWithHex(il2cppMap,0x19A6B08, "200080D2C0035FD6");
    feature0.stopAI_patch2 = MemoryPatch::createWithHex(il2cppMap,0x1A2EF84, "200080D2C0035FD6");
    feature0.stopAI_patch3 = MemoryPatch::createWithHex(il2cppMap,0x19A17E0, "200080D2C0035FD6");
    feature0.stopAI_patch4 = MemoryPatch::createWithHex(il2cppMap,0x19A6998, "200080D2C0035FD6");
    
    feature0.multi_patch1 = MemoryPatch::createWithHex(il2cppMap,0x1811078, "C0035FD6");
    feature0.multi_patch2 = MemoryPatch::createWithHex(il2cppMap,0x19CD5EC, "C0035FD6");
    
    feature0.box_patch1 = MemoryPatch::createWithHex(il2cppMap,0x188A900, "C0035FD6");
    
    feature0.smooth_patch1 = MemoryPatch::createWithHex(il2cppMap,0x1A43CA4, "C0035FD6");
    feature0.smooth_patch2 = MemoryPatch::createWithHex(il2cppMap,0x1A44124, "C0035FD6");

    dlclose(il2cpp_handle);
    LOGI(OBFUSCATE("Done"));
    return NULL;
}


bool bypasso = false;
bool bypass = false;
bool normalAura = false;

bool Galaxya = false;
bool speedPlayer = false;
bool ipad = false;
bool normalAura11 = false;
bool stopAI = false;
bool Multi = false;
bool box = false;
bool smooth = false;
bool smootha = false;
bool longRange15 = false;
bool longRange2 = false;
bool game2 = false; 
bool game3 = false; 
bool game5 = false; 

jobjectArray GetFeatureList(JNIEnv *env, jobject context) {
    if (!env) {
        LOGE(OBFUSCATE("JNIEnv is null"));
        return NULL;
    }

    const char *features[] = {
        OBFUSCATE("Collapse_لاتنسى ان تستغفر الله"),
        //OBFUSCATE("14_ButtonOnOff_The Bypass حماية"),
      //  OBFUSCATE("0_ButtonOnOff_The Bypass حماية"),
        OBFUSCATE("15_Toggle_Multi متعدد"),
       // OBFUSCATE("16_Toggle_longRange مدى بعيد"),
        OBFUSCATE("9_Toggle_Galaxyجلكسي لانهائي"),
        OBFUSCATE("20_Toggle_normaملكي عادي"),
        
        OBFUSCATE("8_Toggle_Speedسبيد جيم"),
        OBFUSCATE("Collapse_تفعيلات سرعة"),
        OBFUSCATE("1_CollapseAdd_Toggle_SpeedPlayer سرعة لاعب"),
        OBFUSCATE("Collapse_تفعيلات عادية"),
        OBFUSCATE("2_CollapseAdd_Toggle_Ipad ايباد"),
        OBFUSCATE("7_CollapseAdd_Toggle_No fall لاسقوط"),
        OBFUSCATE("3_CollapseAdd_Toggle_Stop AI ايقاف بوت"),
        OBFUSCATE("5_CollapseAdd_Toggle_Box صندوق"),
        OBFUSCATE("6_CollapseAdd_Toggle_Smooth اخفاء دمج"),
    };

    int Total_Feature = (sizeof(features) / sizeof(features[0]));
    jobjectArray ret = env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")), env->NewStringUTF(""));
    if (!ret) {
        LOGE(OBFUSCATE("Failed to create jobjectArray"));
        return NULL;
    }

    for (int i = 0; i < Total_Feature; i++) {
        env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));
    }

    return ret;
}

void Changes(JNIEnv *env, jclass clazz, jobject obj, jint featNum, jstring featName, jint value, jlong Lvalue, jboolean boolean, jstring text) {
    if (!env) {
        LOGE(OBFUSCATE("JNIEnv is null"));
        return;
    }

    switch (featNum) {
case 0: // Bypass
    bypass = boolean;
    if (bypass) {   
        feature0.bypass_patch1.Modify();
        feature0.bypass_patch2.Modify();
        feature0.bypass_patch3.Modify();
        feature0.bypass_patch4.Modify();
        feature0.bypass_patch5.Modify();
        feature0.bypass_patch6.Modify();
        feature0.bypass_patch7.Modify();
        feature0.bypass_patch8.Modify();
        feature0.bypass_patch9.Modify();
        feature0.bypass_patch10.Modify();
        feature0.bypass_patch11.Modify();
        feature0.bypass_patch12.Modify();
        feature0.bypass_patch13.Modify();
        feature0.bypass_patch14.Modify();
        feature0.bypass_patch15.Modify();
        feature0.bypass_patch16.Modify();
        feature0.bypass_patch17.Modify();
        feature0.bypass_patch18.Modify();
        feature0.bypass_patch19.Modify();
        feature0.bypass_patch20.Modify();
        feature0.bypass_patch21.Modify();
        feature0.bypass_patch22.Modify();
        feature0.bypass_patch23.Modify();
        feature0.bypass_patch24.Modify();
        feature0.bypass_patch25.Modify();
        feature0.bypass_patch26.Modify();
        feature0.bypass_patch27.Modify();
        feature0.bypass_patch28.Modify();
        feature0.bypass_patch29.Modify();
        feature0.bypass_patch30.Modify();
        feature0.bypass_patch31.Modify();
        feature0.bypass_patch32.Modify();
        feature0.bypass_patch33.Modify();
        feature0.bypass_patch34.Modify();
        LOGI(OBFUSCATE("[+] Bypass ON"));
    } else {
        feature0.bypass_patch1.Restore();
        feature0.bypass_patch2.Restore();
        feature0.bypass_patch3.Restore();
        feature0.bypass_patch4.Restore();
        feature0.bypass_patch5.Restore();
        feature0.bypass_patch6.Restore();
        feature0.bypass_patch7.Restore();
        feature0.bypass_patch8.Restore();
        feature0.bypass_patch9.Restore();
        feature0.bypass_patch10.Restore();
        feature0.bypass_patch11.Restore();
        feature0.bypass_patch12.Restore();
        feature0.bypass_patch13.Restore();
        feature0.bypass_patch14.Restore();
        feature0.bypass_patch15.Restore();
        feature0.bypass_patch16.Restore();
        feature0.bypass_patch17.Restore();
        feature0.bypass_patch18.Restore();
        feature0.bypass_patch19.Restore();
        feature0.bypass_patch20.Restore();
        feature0.bypass_patch21.Restore();
        feature0.bypass_patch22.Restore();
        feature0.bypass_patch23.Restore();
        feature0.bypass_patch24.Restore();
        feature0.bypass_patch25.Restore();
        feature0.bypass_patch26.Restore();
        feature0.bypass_patch27.Restore();
        feature0.bypass_patch28.Restore();
        feature0.bypass_patch29.Restore();
        feature0.bypass_patch30.Restore();
        feature0.bypass_patch31.Restore();
        feature0.bypass_patch32.Restore();
        feature0.bypass_patch33.Restore();
        feature0.bypass_patch34.Restore();
        LOGI(OBFUSCATE("[-] Bypass OFF"));
    }
    break;        case 14:
            bypasso = boolean;
            if (bypasso) {
                feature0.bypasso_patch1.Modify();
                feature0.bypasso_patch2.Modify();
                feature0.bypasso_patch3.Modify();
                feature0.bypasso_patch4.Modify();
                feature0.bypasso_patch5.Modify();
                feature0.bypasso_patch6.Modify();
                feature0.bypasso_patch7.Modify();
                feature0.bypasso_patch8.Modify();
                feature0.bypasso_patch9.Modify();
                feature0.bypasso_patch10.Modify();
                feature0.bypasso_patch11.Modify();
                feature0.bypasso_patch12.Modify();
                feature0.bypasso_patch13.Modify();
                feature0.bypasso_patch14.Modify();
                feature0.bypasso_patch15.Modify();
                feature0.bypasso_patch16.Modify();
                feature0.bypasso_patch17.Modify();
                feature0.bypasso_patch18.Modify();
                feature0.bypasso_patch19.Modify();
                feature0.bypasso_patch20.Modify();
                feature0.bypasso_patch21.Modify();
                feature0.bypasso_patch22.Modify();
                feature0.bypasso_patch23.Modify();
                feature0.bypasso_patch24.Modify();
                feature0.bypasso_patch25.Modify();
                feature0.bypasso_patch26.Modify();
                feature0.bypasso_patch27.Modify();
                feature0.bypasso_patch28.Modify();
                feature0.bypasso_patch29.Modify();
                feature0.bypasso_patch30.Modify();
                feature0.bypasso_patch31.Modify();
                feature0.bypasso_patch32.Modify();
                feature0.bypasso_patch33.Modify();
                feature0.bypasso_patch34.Modify();
                feature0.bypasso_patch35.Modify();
                feature0.bypasso_patch36.Modify();
                feature0.bypasso_patch37.Modify();
                LOGI(OBFUSCATE("[+] bypasso ON"));
            } else {
                feature0.bypasso_patch1.Restore();
                feature0.bypasso_patch2.Restore();
                feature0.bypasso_patch3.Restore();
                feature0.bypasso_patch4.Restore();
                feature0.bypasso_patch5.Restore();
                feature0.bypasso_patch6.Restore();
                feature0.bypasso_patch7.Restore();
                feature0.bypasso_patch8.Restore();
                feature0.bypasso_patch9.Restore();
                feature0.bypasso_patch10.Restore();
                feature0.bypasso_patch11.Restore();
                feature0.bypasso_patch12.Restore();
                feature0.bypasso_patch13.Restore();
                feature0.bypasso_patch14.Restore();
                feature0.bypasso_patch15.Restore();
                feature0.bypasso_patch16.Restore();
                feature0.bypasso_patch17.Restore();
                feature0.bypasso_patch18.Restore();
                feature0.bypasso_patch19.Restore();
                feature0.bypasso_patch20.Restore();
                feature0.bypasso_patch21.Restore();
                feature0.bypasso_patch22.Restore();
                feature0.bypasso_patch23.Restore();
                feature0.bypasso_patch24.Restore();
                feature0.bypasso_patch25.Restore();
                feature0.bypasso_patch26.Restore();
                feature0.bypasso_patch27.Restore();
                feature0.bypasso_patch28.Restore();
                feature0.bypasso_patch29.Restore();
                feature0.bypasso_patch30.Restore();
                feature0.bypasso_patch31.Restore();
                feature0.bypasso_patch32.Restore();
                feature0.bypasso_patch33.Restore();
                feature0.bypasso_patch34.Restore();
                feature0.bypasso_patch35.Restore();
                feature0.bypasso_patch36.Restore();
                feature0.bypasso_patch37.Restore();
                LOGI(OBFUSCATE("[-] bypasso OFF"));
            }
            break;
        
        case 16:
            longRange2 = boolean;
            if (longRange2) {
                feature0.longRange2_patch1.Modify();
                feature0.longRange2_patch2.Modify();
                feature0.longRange2_patch3.Modify();
                feature0.longRange2_patch4.Modify();
                LOGI("[+] longRange2 ON");
            } else {
                feature0.longRange2_patch1.Restore();
                feature0.longRange2_patch2.Restore();
                feature0.longRange2_patch3.Restore();
                feature0.longRange2_patch4.Restore();
                LOGI("[-] longRange2 OFF");
            }
            break;
            
            
                    case 20:
    normalAura = boolean;
    if (normalAura) {
        feature0.normalAura_patch1.Modify();
        feature0.normalAura_patch2.Modify();
        feature0.normalAura_patch3.Modify();
        feature0.normalAura_patch4.Modify();
        feature0.normalAura_patch5.Modify();
        LOGI("[+] normalAura ON");
    } else {
        feature0.normalAura_patch1.Restore();
        feature0.normalAura_patch2.Restore();
        feature0.normalAura_patch3.Restore();
        feature0.normalAura_patch4.Restore();
        feature0.normalAura_patch5.Restore();
        LOGI("[-] normalAura OFF");
    }
    break;
            
        case 19:
            longRange15 = boolean;
            if (longRange15) {
                feature0.longRange15_patch1.Modify();
                feature0.longRange15_patch2.Modify();
                feature0.longRange15_patch3.Modify();
                feature0.longRange15_patch4.Modify();
                feature0.longRange15_patch5.Modify();
                feature0.longRange15_patch6.Modify();
                feature0.longRange15_patch7.Modify();
                feature0.longRange15_patch8.Modify();
                feature0.longRange15_patch9.Modify();
                feature0.longRange15_patch10.Modify();
                feature0.longRange15_patch11.Modify();
                feature0.longRange15_patch12.Modify();
                feature0.longRange15_patch13.Modify();
                feature0.longRange15_patch14.Modify();
                feature0.longRange15_patch15.Modify();
                LOGI("[+] longRange15 ON");
            } else {
                feature0.longRange15_patch1.Restore();
                feature0.longRange15_patch2.Restore();
                feature0.longRange15_patch3.Restore();
                feature0.longRange15_patch4.Restore();
                feature0.longRange15_patch5.Restore();
                feature0.longRange15_patch6.Restore();
                feature0.longRange15_patch7.Restore();
                feature0.longRange15_patch8.Restore();
                feature0.longRange15_patch9.Restore();
                feature0.longRange15_patch10.Restore();
                feature0.longRange15_patch11.Restore();
                feature0.longRange15_patch12.Restore();
                feature0.longRange15_patch13.Restore();
                feature0.longRange15_patch14.Restore();
                feature0.longRange15_patch15.Restore();
                LOGI("[-] longRange15 OFF");
            }
            break;
            
        case 8:
            game2 = boolean;
            if (game2) {
                feature0.game_patch2.Modify();
                LOGI("[+] Game Speed 2 ON");
            } else {
                feature0.game_patch2.Restore();
                LOGI("[-] Game Speed 2 OFF");
            }
            break;
            
        case 24:
            game3 = boolean;
            if (game3) {
                feature0.game_patch3.Modify();
                LOGI("[+] Game Speed 3 ON");
            } else {
                feature0.game_patch3.Restore();
                LOGI("[-] Game Speed 3 OFF");
            }
            break;
            
        case 25:
            game5 = boolean;
            if (game5) {
                feature0.game_patch5.Modify();
                LOGI("[+] Game Speed 5 ON");
            } else {
                feature0.game_patch5.Restore();
                LOGI("[-] Game Speed 5 OFF");
            }
            break;

        case 7:
            smootha = boolean;
            if (smootha) {
                feature0.smootha_patch1.Modify();
                feature0.smootha_patch2.Modify();
                feature0.smootha_patch3.Modify();
                LOGI(OBFUSCATE("[+] No Fall ON"));
            } else {
                feature0.smootha_patch1.Restore();
                feature0.smootha_patch2.Restore();
                feature0.smootha_patch3.Restore();
                LOGI(OBFUSCATE("[-] No Fall OFF"));
            }
            break;

        case 1:
            speedPlayer = boolean;
            if (speedPlayer) {
                feature0.speedPlayer_patch1.Modify();
                LOGI(OBFUSCATE("[+] SpeedPlayer ON"));
            } else {
                feature0.speedPlayer_patch1.Restore();
                LOGI(OBFUSCATE("[-] SpeedPlayer OFF"));
            }
            break;

        case 2:
            ipad = boolean;
            if (ipad) {
                feature0.ipad_patch1.Modify();
                feature0.ipad_patch2.Modify();
                feature0.ipad_patch3.Modify();
                LOGI(OBFUSCATE("[+] Ipad ON"));
            } else {
                feature0.ipad_patch1.Restore();
                feature0.ipad_patch2.Restore();
                feature0.ipad_patch3.Restore();
                LOGI(OBFUSCATE("[-] Ipad OFF"));
            }
            break;

        case 3:
            stopAI = boolean;
            if (stopAI) {
                feature0.stopAI_patch1.Modify();
                feature0.stopAI_patch2.Modify();
                feature0.stopAI_patch3.Modify();
                feature0.stopAI_patch4.Modify();
                LOGI(OBFUSCATE("[+] StopAI ON"));
            } else {
                feature0.stopAI_patch1.Restore();
                feature0.stopAI_patch2.Restore();
                feature0.stopAI_patch3.Restore();
                feature0.stopAI_patch4.Restore();
                LOGI(OBFUSCATE("[-] StopAI OFF"));
            }
            break;

        case 15:
            Multi = boolean;
            if (Multi) {
                feature0.multi_patch1.Modify();
                feature0.multi_patch2.Modify();
                LOGI(OBFUSCATE("[+] Multi ON"));
            } else {
                feature0.multi_patch1.Restore();
                feature0.multi_patch2.Restore();
                LOGI(OBFUSCATE("[-] Multi OFF"));
            }
            break;

        case 5:
            box = boolean;
            if (box) {
                feature0.box_patch1.Modify();
                LOGI(OBFUSCATE("[+] Box ON"));
            } else {
                feature0.box_patch1.Restore();
                LOGI(OBFUSCATE("[-] Box OFF"));
            }
            break;

        case 6:
            smooth = boolean;
            if (smooth) {
                feature0.smooth_patch1.Modify();
                feature0.smooth_patch2.Modify();
                LOGI(OBFUSCATE("[+] Smooth ON"));
            } else {
                feature0.smooth_patch1.Restore();
                feature0.smooth_patch2.Restore();
                LOGI(OBFUSCATE("[-] Smooth OFF"));
            }
            break;
            
        case 9:
            Galaxya = boolean;
            if (Galaxya) {
                feature0.Galaxya_patch1.Modify();
                feature0.Galaxya_patch2.Modify();
                LOGI(OBFUSCATE("[+] Galaxy ON"));
            } else {
                feature0.Galaxya_patch1.Restore();
                feature0.Galaxya_patch2.Restore();
                LOGI(OBFUSCATE("[-] Galaxy OFF"));
            }
            break;

        default:
            LOGE(OBFUSCATE("Unknown feature number: %d"), featNum);
            break;
    }
}

__attribute__((constructor))
void lib_main() {
    pthread_t ptid;
    pthread_create(&ptid, NULL, hack_thread, NULL);
}
