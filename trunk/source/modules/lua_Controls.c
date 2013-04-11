#include "../functions.h"

typedef struct WPADButtons {
	u32 pad;
	u32 pre;
	u32 rel;
	int wpad;
} WPADButtons;

toPush(CB, WPADButtons)
toPush(CD, WPADData*)

static int Controls_read(lua_State *l) {
	if(lua_gettop(l) != 1) return luaL_error(l, "Wrong number of elements.");
	int wpad = luaL_checkint(l, 1);
	WPAD_ScanPads();
	WPADButtons cb;
	cb.pad = WPAD_ButtonsDown(wpad);
	cb.rel = WPAD_ButtonsUp(wpad);
	cb.pre = WPAD_ButtonsHeld(wpad);
	WPADButtons *CB = pushCB(l);
	*CB = cb;
	return 1;
}
static int Data_read(lua_State *l) {
	if(lua_gettop(l) != 1) return luaL_error(l, "Wrong number of elements.");
	int wpad = luaL_checkint(l, 1);
	WPAD_ScanPads();
	WPADData *cd = WPAD_Data(wpad);
	WPADData** CD = pushCD(l);
	*CD = cd;
	return 1;
}

//NUNCHUCK JOYSTICK
#define CNJDataConv(NAMEFUN, TYPE, DATA) \
	static int Controls_##NAMEFUN (lua_State *l) { \
		if(lua_gettop(l) != 1) return luaL_error(l, "Wrong number of elements."); \
		WPADData *cd = *toCD(l, 1);\
		lua_pushnumber(l, cd->exp.nunchuk.js.TYPE.DATA); \
		return 1; \
	}
CNJDataConv(nunJoyPosx, pos, x);
CNJDataConv(nunJoyPosy, pos, y);
CNJDataConv(nunJoyCenterx, center, x);
CNJDataConv(nunJoyCentery, center, y);
CNJDataConv(nunJoyMinx, min, x);
CNJDataConv(nunJoyMiny, min, y);
CNJDataConv(nunJoyMaxx, max, x);
CNJDataConv(nunJoyMaxy, max, y);
static int Controls_nunJoyAng(lua_State *l) {
	if(lua_gettop(l) != 1) return luaL_error(l, "Wrong number of elements.");
	WPADData *cd = *toCD(l, 1);
	lua_pushnumber(l, cd->exp.nunchuk.js.ang);
	return 1;
}
static int Controls_nunJoyMag(lua_State *l) {
	WPADData *cd = *toCD(l, 1);
	lua_pushnumber(l, cd->exp.nunchuk.js.mag);
	return 1;
}

//NUNCHUCK
#define CNDataConv(NAMEFUN, TYPE, DATA) \
	static int Controls_##NAMEFUN (lua_State *l) { \
		if(lua_gettop(l) != 1) return luaL_error(l, "Wrong number of elements."); \
		WPADData *cd = *toCD(l, 1);\
		lua_pushnumber(l, cd->exp.nunchuk.TYPE.DATA); \
		return 1; \
	}
CNDataConv(nunAccelx, accel, x);
CNDataConv(nunAccely, accel, y);
CNDataConv(nunAccelz, accel, z);
CNDataConv(nunGforcex, gforce, x);
CNDataConv(nunGforcey, gforce, y);
CNDataConv(nunOrientRoll, orient, pitch);
CNDataConv(nunOrientPitch, orient, roll);
CNDataConv(nunOrientYaw, orient, yaw);

//WIIMOTE DATA
#define CDataConv(NAMEFUN, TYPE, DATA) \
	static int Controls_##NAMEFUN (lua_State *l) { \
		if(lua_gettop(l) != 1) return luaL_error(l, "Wrong number of elements."); \
		WPADData *cd = *toCD(l, 1);\
		lua_pushnumber(l, cd->TYPE.DATA); \
		return 1; \
	}
CDataConv(IRx, ir, x);
CDataConv(IRy, ir, y);
CDataConv(IRax, ir, ax);
CDataConv(IRay, ir, ay);
CDataConv(IRsx, ir, sx);
CDataConv(IRsy, ir, sy);
CDataConv(IRangle, ir, angle);
CDataConv(Accelx, accel, x);
CDataConv(Accely, accel, y);
CDataConv(Accelz, accel, z);
CDataConv(Gforcex, gforce, x);
CDataConv(Gforcey, gforce, y);
CDataConv(OrientRoll, orient, roll);
CDataConv(OrientPitch, orient, pitch);
CDataConv(OrientYaw, orient, yaw);

//WIIMOTE BUTTONS
#define ControlsConv(NAMEFUN, NAMEBUT, TYPE) \
	static int Controls_##NAMEFUN (lua_State *l) { \
		if(lua_gettop(l) != 1) return luaL_error(l, "Wrong number of elements."); \
		WPADButtons cb = *toCB(l, 1);\
		if (cb.TYPE & WPAD_##NAMEBUT) { \
			lua_pushnumber(l, 1); \
		} \
		return 1; \
	} 
ControlsConv(padNunC, NUNCHUK_BUTTON_C, pad);
ControlsConv(preNunC, NUNCHUK_BUTTON_C, pre);
ControlsConv(relNunC, NUNCHUK_BUTTON_C, rel);
ControlsConv(padNunZ, NUNCHUK_BUTTON_Z, pad);
ControlsConv(preNunZ, NUNCHUK_BUTTON_Z, pre);
ControlsConv(relNunZ, NUNCHUK_BUTTON_Z, rel);
ControlsConv(padA, BUTTON_A, pad);
ControlsConv(preA, BUTTON_A, pre);
ControlsConv(relA, BUTTON_A, rel);
ControlsConv(padB, BUTTON_B, pad);
ControlsConv(preB, BUTTON_B, pre);
ControlsConv(relB, BUTTON_B, rel);
ControlsConv(pad1, BUTTON_1, pad);
ControlsConv(pre1, BUTTON_1, pre);
ControlsConv(rel1, BUTTON_1, rel);
ControlsConv(pad2, BUTTON_2, pad);
ControlsConv(pre2, BUTTON_2, pre);
ControlsConv(rel2, BUTTON_2, rel);
ControlsConv(padplus, BUTTON_PLUS, pad);
ControlsConv(preplus, BUTTON_PLUS, pre);
ControlsConv(relplus, BUTTON_PLUS, rel);
ControlsConv(padminus, BUTTON_MINUS, pad);
ControlsConv(preminus, BUTTON_MINUS, pre);
ControlsConv(relminus, BUTTON_MINUS, rel);
ControlsConv(padup, BUTTON_UP, pad);
ControlsConv(preup, BUTTON_UP, pre);
ControlsConv(relup, BUTTON_UP, rel);
ControlsConv(paddown, BUTTON_DOWN, pad);
ControlsConv(predown, BUTTON_DOWN, pre);
ControlsConv(reldown, BUTTON_DOWN, rel);
ControlsConv(padleft, BUTTON_LEFT, pad);
ControlsConv(preleft, BUTTON_LEFT, pre);
ControlsConv(relleft, BUTTON_LEFT, rel);
ControlsConv(padright, BUTTON_RIGHT, pad);
ControlsConv(preright, BUTTON_RIGHT, pre);
ControlsConv(relright, BUTTON_RIGHT, rel);
ControlsConv(padhome, BUTTON_HOME, pad);
ControlsConv(prehome, BUTTON_HOME, pre);
ControlsConv(relhome, BUTTON_HOME, rel);

static const luaL_Reg Controls[] = {
	{"read", Controls_read},
	{"readData", Data_read},
	{"IRx", Controls_IRx},
	{"IRy", Controls_IRy},
	{"IRrawx", Controls_IRax},
	{"IRrawy", Controls_IRay},
	{"IRsmoothx", Controls_IRsx},
	{"IRsmoothy", Controls_IRsy},
	{"IRangle", Controls_IRangle},
	{"accelx", Controls_Accelx},
	{"accely", Controls_Accely},
	{"accelz", Controls_Accelz},
	{"Gforcex", Controls_Gforcex},
	{"Gforcey", Controls_Gforcey},
	{"orientRoll", Controls_OrientRoll},
	{"orientPitch", Controls_OrientPitch},
	{"orientYaw", Controls_OrientYaw},
	{"nunAccelx", Controls_nunAccelx},
	{"nunAccely", Controls_nunAccely},
	{"nunAccelz", Controls_nunAccelz},
	{"nunGforcex", Controls_nunGforcex},
	{"nunGforcey", Controls_nunGforcey},
	{"nunOrientRoll", Controls_nunOrientRoll},
	{"nunorientPitch", Controls_nunOrientPitch},
	{"nunorientYaw", Controls_nunOrientYaw},
	{"nunJoyPosx", Controls_nunJoyPosx},
	{"nunJoyPosy", Controls_nunJoyPosy},
	{"nunJoyCenterx", Controls_nunJoyCenterx},
	{"nunJoyCentery", Controls_nunJoyCentery},
	{"nunJoyMinx", Controls_nunJoyMinx},
	{"nunJoyMiny", Controls_nunJoyMiny},
	{"nunJoyMaxx", Controls_nunJoyMaxx},
	{"nunJoyMaxy", Controls_nunJoyMaxy},
	{"nunJoyAng", Controls_nunJoyAng},
	{"nunJoyMag", Controls_nunJoyMag},
	{"pressA", Controls_padA },
	{"heldA", Controls_preA },
	{"releaseA", Controls_relA },
	{"pressB", Controls_padB },
	{"heldB", Controls_preB },
	{"releaseB", Controls_relB },
	{"pressUp", Controls_padup },
	{"heldUp", Controls_preup },
	{"releaseUp", Controls_relup },
	{"pressRight", Controls_padright },
	{"heldRight", Controls_preright },
	{"releaseRight", Controls_relright },
	{"pressDown", Controls_paddown },
	{"heldDown", Controls_predown },
	{"releaseDown", Controls_reldown },
	{"pressLeft", Controls_padleft },
	{"heldLeft", Controls_preleft },
	{"releaseLeft", Controls_relleft },
	{"press1", Controls_pad1 },
	{"held1", Controls_pre1 },
	{"release1", Controls_rel1 },
	{"press2", Controls_pad2 },
	{"held2", Controls_pre2 },
	{"release2", Controls_rel2 },
	{"pressPlus", Controls_padplus },
	{"heldPlus", Controls_preplus },
	{"releasePlus", Controls_relplus },
	{"pressMinus", Controls_padminus },
	{"heldMinus", Controls_preminus },
	{"releaseMinus", Controls_relminus },
	{"pressHome", Controls_padhome },
	{"heldHome", Controls_prehome },
	{"releaseHome", Controls_relhome },
	{"pressNunC", Controls_padNunC },
	{"heldNunC", Controls_preNunC },
	{"releaseNunC", Controls_relNunC },
	{"pressNunZ", Controls_padNunZ },
	{"heldNunZ", Controls_preNunZ },
	{"releaseNunZ", Controls_relNunZ },
	/**{"kbdRead", Controls_kbdread },
	{"kbdIsConnected", Controls_kbdisconnected },*/
  {NULL, NULL}
};

int luaopen_Controls(lua_State *l) {
	   luaL_register(l, "Controls", Controls);
       return 1;
}