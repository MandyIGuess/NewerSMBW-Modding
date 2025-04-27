#include <game.h>
#include <dCourse.h>

void dJellyFishModel_c__draw(void *self) {
	bool visible = (bool)((int)(self) + 0x194);
	m3d::mdl_c *lightMask = (m3d::mdl_c*)((int)(self) + 0x20);
	m3d::mdl_c *lightDisp = (m3d::mdl_c*)((int)(self) + 0x60);
	m3d::mdl_c *lightFog = (m3d::mdl_c*)((int)(self) + 0xA0);

	if (visible) {
		// don't draw lightmask unless we're in darkness
		u8 visibility = dCourseFull_c::instance->courses[GetAreaNum()].getZoneByID(GetZoneNum())->visibility;
		if (visibility & 0x20) {
			lightMask->scheduleForDrawing();
		}
		lightDisp->scheduleForDrawing();
		lightFog->scheduleForDrawing();
	}
}