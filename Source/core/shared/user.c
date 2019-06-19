#include "dev.h"
#include "enx_freertos.h"

volatile tSystem gtSystem;		// System information
volatile tUser gtUser;			// User information
#if defined(__ISP__)
volatile BYTE *gtISP;			// ISP information
#endif
#if defined(__NETWORK__)
volatile tNetwork gtNetwork;	// Network information
#endif

const ResolutionInfo listResolution[ResolutionInfoLength] = {
	{"3840x2160", e_res3840x2160, 3840, 2160},
	{"2560x2048", e_res2560x2048, 2560, 2048},
	{"2560x1600", e_res2560x1600, 2560, 1600},
	{"2560x1440", e_res2560x1440, 2560, 1440},
	{"2048x1536", e_res2048x1536, 2048, 1536},
	{"1920x1200", e_res1920x1200, 1920, 1200},
	{"1920x1080", e_res1920x1080, 1920, 1080},
	{"1280x720", e_res1280x720, 1280, 720},
	{"800x600", e_res800x600, 800, 600},
	{"704x480", e_res704x480, 704, 480},
	{"704x400", e_res704x400, 704, 400},
	{"640x480", e_res640x480, 640, 480},
	{"640x360", e_res640x360, 640, 360},
	{"320x240", e_res320x240, 320, 240}
};



