#include <game.h>
#include <dCourse.h>

#define RANDOMIZES_TERRAIN_SHADING true

extern "C" void *GetSceneLightInfo(int);
extern "C" void *GetFogMgr(int);
extern "C" void LoadBlight(void *lightMgr, u8 *file);
extern "C" void LoadBlmap(void *lightMapClass, u8 *file);
extern "C" void LoadBfog(void *fogMgr, u8 *file);

class LightMgr {
public:
    u8 crap[0x14];
    void *lightMapClass;
};

void LoadWorldMapLighting(void *wmap, int sceneID) {
    char blightBuf[32], blmapBuf[32];

    sprintf(blightBuf, "light/W%X.blight", MakeRandomNumber(8) + 1);
    sprintf(blmapBuf, "light/W%X.blmap", MakeRandomNumber(8) + 1);
    OSReport("%s, %s\n", blightBuf, blmapBuf);

    LightMgr *lightMgr = (LightMgr*)GetSceneLightInfo(sceneID);
    LoadBlight(lightMgr, getResource("Env_world", blightBuf));

    LoadBlmap(lightMgr->lightMapClass, getResource("Env_world", blmapBuf));
}

extern u8 CurrentTerrainLightingValue;

const char *stageLighting[] = {
    "MainScene",
    "ChikaScene",
    "WaterScene",
    "FireScene",
    "SabakuScene",
    "KaiganScene",
    "DaishizenScene",
    "YukiDayScene",
    "SoraScene",
    "IwabaScene",
    "TorideScene",
    "ShiroScene",
    "ObakeScene",
    "KurayamiChikaScene",
    "ObakeOutScene",
    "ShinkaiScene",
    "SabakuChikaScene",
    "KoriChikaScene",
    "Fire2Scene",
    "KoopaScene",
    "Fire3Scene",
    "KumoriSoraScene",
    "Toride2Scene",
};

void LoadStageLighting(int sceneID) {
    dCourse_c *area = &dCourseFull_c::instance->courses[GetAreaNum()];
    LightMgr *lightMgr = (LightMgr*)GetSceneLightInfo(sceneID);
    void *fogMgr = GetFogMgr(sceneID);
    char blightBuf[48], blmapBuf[48], bfogBuf[48];

    if (RANDOMIZES_TERRAIN_SHADING)
        CurrentTerrainLightingValue = MakeRandomNumber(4);
    else
        CurrentTerrainLightingValue = area->getTerrainShadingForZone(GetZoneNum());

    sprintf(blightBuf, "light/%s.blight", stageLighting[MakeRandomNumber(23)]);
    sprintf(blmapBuf, "light/%s.blmap", stageLighting[MakeRandomNumber(23)]);
    sprintf(bfogBuf, "fog/%s.bfog", stageLighting[MakeRandomNumber(23)]);
    OSReport("%s, %s, %s\n", blightBuf, blmapBuf, bfogBuf);

    LoadBlight(lightMgr, getResource("Env_course", blightBuf));
    LoadBlmap(lightMgr->lightMapClass, getResource("Env_course", blmapBuf));
    u8 *bfog = getResource("Env_course", bfogBuf);
    if (bfog != 0)
        LoadBfog(fogMgr, bfog);
}

int LoadStageDepthOfField() {
    return MakeRandomNumber(5);
}