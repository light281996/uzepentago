const char patch00[] PROGMEM ={ 
0,PC_WAVE,6,
0,PC_ENV_SPEED,-5,
0,PATCH_END
};

const char patch01[] PROGMEM ={ 
0,PC_WAVE,6,
0,PC_ENV_SPEED,-5,
0,PATCH_END
};

const char patch02[] PROGMEM ={
0,PC_WAVE,4,
0,PC_ENV_SPEED,-10,
2,PC_NOTE_DOWN,12,
2,PC_NOTE_DOWN,12,
2,PC_NOTE_DOWN,6,
2,PC_NOTE_DOWN,6,
2,PC_NOTE_DOWN,6,
2,PC_NOTE_CUT,0,
0,PATCH_END
};

const char patch03[] PROGMEM ={	
0,PC_WAVE,4,
0,PC_ENV_SPEED,-10,
2,PC_NOTE_DOWN,12,
2,PC_NOTE_DOWN,12,
2,PC_NOTE_DOWN,6,
2,PC_NOTE_DOWN,6,
2,PC_NOTE_DOWN,6,
2,PC_NOTE_CUT,0,
0,PATCH_END
};

// selection FX, stolen ;) from Megatris
const char patch04[] PROGMEM ={	
0,PC_NOISE_PARAMS,3,
0,PC_ENV_SPEED,-30, 
3,PC_NOISE_PARAMS,0x6,
4,PC_NOISE_PARAMS,0x9,
3,PC_NOTE_CUT,0,
0,PATCH_END
};

// confirm FX, stolen ;) from Megatris
const char patch05[] PROGMEM ={	
0,PC_ENV_SPEED,-12,
5,PC_NOTE_UP,12, 
5,PC_NOTE_DOWN,12,
5,PC_NOTE_UP,12, 
5,PC_NOTE_DOWN,12,
5,PC_NOTE_CUT,0,
0,PATCH_END
};

// error oop
const char patch06[] PROGMEM ={
0,PC_WAVE,0,
0,PC_PITCH,38,
0,PC_ENV_SPEED,-30,
0,PATCH_END
};

const struct PatchStruct patches[] PROGMEM = {
{0,NULL,patch00,0,0},
{0,NULL,patch01,0,0},
{0,NULL,patch02,0,0},
{0,NULL,patch03,0,0},
{0,NULL,patch04,0,0},
{0,NULL,patch05,0,0},
{0,NULL,patch06,0,0}
};
