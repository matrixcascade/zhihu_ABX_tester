#ifndef PAINTERENGINE_APPLICATION_H
#define PAINTERENGINE_APPLICATION_H

#include "PainterEngine_Startup.h"
#include "./architecture/PainterEngine_Console.h"
#include "./architecture/PainterEngine_MessageBox.h"
#include "platform/modules/px_audio.h"

typedef enum
{
	PX_APPLICATION_X_A,
	PX_APPLICATION_X_B,
}PX_APPLICATION_X;

typedef struct
{
	PX_SoundData sounddataA,sounddataB;
	PX_SoundPlay soundplay;
	PX_Json ui_json;
	PX_UI ui;
	px_int testcount;
	px_int correctcount;
	PX_FontModule fontmodule;
	PX_Runtime runtime;
	PX_MessageBox messagebox;
	PX_APPLICATION_X x;
	PX_Object *root,*openfile,*play_a,*play_b,*play_x,*check_a,*check_b,*fileExplorer,*correct_test,*correct_test_label;
}PX_Application;

extern PX_Application App;

px_bool PX_ApplicationInitialize(PX_Application *App,px_int screen_Width,px_int screen_Height);
px_void PX_ApplicationUpdate(PX_Application *App,px_dword elpased);
px_void PX_ApplicationRender(PX_Application *App,px_dword elpased);
px_void PX_ApplicationPostEvent(PX_Application *App,PX_Object_Event e);

#endif
