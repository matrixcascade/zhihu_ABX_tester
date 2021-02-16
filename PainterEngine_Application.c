#include "PainterEngine_Application.h"

PX_Application App;

static px_int PX_APP_ExplorerGetPathFolderCount(const px_char *path,const char *filter)
{
	return PX_FileGetDirectoryFileCount(path,PX_FILEENUM_TYPE_FOLDER,filter);
}
static px_int PX_APP_ExplorerGetPathFileCount(const px_char *path,const char *filter)
{
	return PX_FileGetDirectoryFileCount(path,PX_FILEENUM_TYPE_FILE,filter);
}
static px_int PX_APP_ExplorerGetPathFolderName(const char path[],int count,char FileName[][260],const char *filter)
{
	return PX_FileGetDirectoryFileName(path,count,FileName,PX_FILEENUM_TYPE_FOLDER,filter);
}
static px_int PX_APP_ExplorerGetPathFileName(const char path[],int count,char FileName[][260],const char *filter)
{
	return PX_FileGetDirectoryFileName(path,count,FileName,PX_FILEENUM_TYPE_FILE,filter);
}

static px_void PX_APP_OnButtonPlayA(PX_Object *pObject,PX_Object_Event e,px_void *ptr)
{
	PX_Application *pApp=(PX_Application *)ptr;
	PX_SoundPlayClear(&pApp->soundplay);
	PX_SoundPlayAdd(&pApp->soundplay,PX_SoundCreate(&pApp->sounddataA,PX_FALSE));
}

static px_void PX_APP_OnButtonPlayB(PX_Object *pObject,PX_Object_Event e,px_void *ptr)
{
	PX_Application *pApp=(PX_Application *)ptr;
	PX_SoundPlayClear(&pApp->soundplay);
	PX_SoundPlayAdd(&pApp->soundplay,PX_SoundCreate(&pApp->sounddataB,PX_FALSE));
}

static px_void PX_APP_OnButtonPlayX(PX_Object *pObject,PX_Object_Event e,px_void *ptr)
{
	PX_Application *pApp=(PX_Application *)ptr;
	PX_SoundPlayClear(&pApp->soundplay);
	switch(pApp->x)
	{
	case PX_APPLICATION_X_A:
		PX_SoundPlayAdd(&pApp->soundplay,PX_SoundCreate(&pApp->sounddataA,PX_FALSE));
		break;
	case PX_APPLICATION_X_B:
		PX_SoundPlayAdd(&pApp->soundplay,PX_SoundCreate(&pApp->sounddataB,PX_FALSE));
		break;
	}
	pApp->check_a->Visible=PX_TRUE;
	pApp->check_b->Visible=PX_TRUE;
}

static px_void PX_APP_OnButtonCheckA(PX_Object *pObject,PX_Object_Event e,px_void *ptr)
{
	PX_Application *pApp=(PX_Application *)ptr;
	PX_SoundPlayClear(&pApp->soundplay);
	if (pApp->x==PX_APPLICATION_X_A)
	{
		PX_MessageBoxAlertOk(&pApp->messagebox,PX_JsonGetString(&pApp->ui_json,"congratulations right"));
		pApp->correctcount++;
	}
	else
	{
		PX_MessageBoxAlertOk(&pApp->messagebox,PX_JsonGetString(&pApp->ui_json,"sorry wrong"));
	}
	pApp->testcount++;

	pApp->x=PX_randRange(0,1)>=0.5?PX_APPLICATION_X_A:PX_APPLICATION_X_B;
	pApp->check_a->Visible=PX_FALSE;
	pApp->check_b->Visible=PX_FALSE;
	do 
	{
		px_char fmt[16];
		PX_sprintf2(fmt,sizeof(fmt),"%1/%2",PX_STRINGFORMAT_INT(pApp->correctcount),PX_STRINGFORMAT_INT(pApp->testcount));
		PX_Object_LabelSetText(pApp->correct_test,fmt);
	} while (0);

}

static px_void PX_APP_OnButtonCheckB(PX_Object *pObject,PX_Object_Event e,px_void *ptr)
{
	PX_Application *pApp=(PX_Application *)ptr;
	PX_SoundPlayClear(&pApp->soundplay);
	if (pApp->x==PX_APPLICATION_X_B)
	{
		PX_MessageBoxAlertOk(&pApp->messagebox,PX_JsonGetString(&pApp->ui_json,"congratulations right"));
		pApp->correctcount++;
	}
	else
	{
		PX_MessageBoxAlertOk(&pApp->messagebox,PX_JsonGetString(&pApp->ui_json,"sorry wrong"));
	}
	pApp->testcount++;

	pApp->x=PX_randRange(0,1)>=0.5?PX_APPLICATION_X_A:PX_APPLICATION_X_B;
	pApp->check_a->Visible=PX_FALSE;
	pApp->check_b->Visible=PX_FALSE;

	do 
	{
		px_char fmt[16];
		PX_sprintf2(fmt,sizeof(fmt),"%1/%2",PX_STRINGFORMAT_INT(pApp->correctcount),PX_STRINGFORMAT_INT(pApp->testcount));
		PX_Object_LabelSetText(pApp->correct_test,fmt);
	} while (0);
}


static px_void PX_APP_OnButtonFileExplorer(PX_Object *pObject,PX_Object_Event e,px_void *ptr)
{
	PX_Application *pApp=(PX_Application *)ptr;
	PX_Object_ExplorerOpen(pApp->fileExplorer);
}

static px_void PX_APP_OnButtonFileExplorerConfirm(PX_Object *pObject,PX_Object_Event e,px_void *ptr)
{
	PX_Application *pApp=(PX_Application *)ptr;
	px_char Path[260];
	PX_Object_ExplorerGetPath(pObject,Path,0);
	if(PX_LoadSoundFromFile(&pApp->runtime.mp_resources,&pApp->sounddataA,Path))
	{
		if (PX_SoundStaticDataCopy(&pApp->runtime.mp_resources,&pApp->sounddataA,&pApp->sounddataB))
		{
			px_int c=pApp->sounddataB.size/2;
			px_int i;
			for (i=0;i<c;i++)
			{
				px_short *pPCM16=(px_short *)pApp->sounddataB.buffer;
				pPCM16[i]=(pPCM16[i]&0xfffe);//remove the last bit
			}
			PX_MessageBoxAlertOk(&pApp->messagebox,PX_JsonGetString(&pApp->ui_json,"load file succeeded"));
			pApp->openfile->Visible=PX_FALSE;
			pApp->play_a->Visible=PX_TRUE;
			pApp->play_b->Visible=PX_TRUE;
			pApp->play_x->Visible=PX_TRUE;
			pApp->correct_test->Visible=PX_TRUE;
			pApp->correct_test_label->Visible=PX_TRUE;
			
			return;
		}
		PX_SoundStaticDataFree(&pApp->sounddataA);
	}
	
	PX_MessageBoxAlertOk(&pApp->messagebox,PX_JsonGetString(&pApp->ui_json,"load file failed"));

}


px_bool PX_ApplicationInitialize(PX_Application *pApp,px_int screen_width,px_int screen_height)
{
	PX_memset(pApp,0,sizeof(PX_Application));
	PX_ApplicationInitializeDefault(&pApp->runtime, screen_width, screen_height);
	if(!PX_SoundPlayInitialize(&pApp->runtime.mp_game,&pApp->soundplay)) return PX_FALSE;
	if(!PX_AudioInitialize(&pApp->soundplay))return PX_FALSE;
	if(!PX_FontModuleInitialize(&pApp->runtime.mp_resources,&pApp->fontmodule))return PX_FALSE;
	if (!PX_LoadFontModuleFromFile(&pApp->fontmodule,"assets/font18.pxf")) return PX_FALSE;
	if(!PX_UIInitialize(&pApp->runtime.mp_game,&pApp->runtime.mp_ui,&pApp->ui,&pApp->fontmodule))return PX_FALSE;
	do 
	{

		if(!PX_JsonInitialize(&pApp->runtime.mp_resources,&pApp->ui_json)) return PX_FALSE;
		if(!PX_LoadJsonFromFile(&pApp->ui_json,"assets/ui_main.json"))return PX_FALSE;
		pApp->root=PX_UICreate(&pApp->ui,PX_NULL,&pApp->ui_json.rootValue,pApp->runtime.surface_width,pApp->runtime.surface_height);
		if (!pApp->root)
		{
			return PX_FALSE;
		}

	} while (0);

	pApp->openfile=PX_UIGetObjectByID(&pApp->ui,"openfile");
	pApp->play_a=PX_UIGetObjectByID(&pApp->ui,"play_a");
	pApp->play_b=PX_UIGetObjectByID(&pApp->ui,"play_b");
	pApp->play_x=PX_UIGetObjectByID(&pApp->ui,"play_x");
	pApp->check_a=PX_UIGetObjectByID(&pApp->ui,"check_a");
	pApp->check_b=PX_UIGetObjectByID(&pApp->ui,"check_b");
	pApp->correct_test=PX_UIGetObjectByID(&pApp->ui,"correct_test");
	pApp->correct_test_label=PX_UIGetObjectByID(&pApp->ui,"correct_test_label");

	pApp->play_a->Visible=PX_FALSE;
	pApp->play_b->Visible=PX_FALSE;
	pApp->play_x->Visible=PX_FALSE;
	pApp->check_a->Visible=PX_FALSE;
	pApp->check_b->Visible=PX_FALSE;
	pApp->correct_test->Visible=PX_FALSE;
	pApp->correct_test_label->Visible=PX_FALSE;

	PX_ObjectRegisterEvent(pApp->play_a,PX_OBJECT_EVENT_EXECUTE,PX_APP_OnButtonPlayA,pApp);
	PX_ObjectRegisterEvent(pApp->play_b,PX_OBJECT_EVENT_EXECUTE,PX_APP_OnButtonPlayB,pApp);
	PX_ObjectRegisterEvent(pApp->play_x,PX_OBJECT_EVENT_EXECUTE,PX_APP_OnButtonPlayX,pApp);
	PX_ObjectRegisterEvent(pApp->check_a,PX_OBJECT_EVENT_EXECUTE,PX_APP_OnButtonCheckA,pApp);
	PX_ObjectRegisterEvent(pApp->check_b,PX_OBJECT_EVENT_EXECUTE,PX_APP_OnButtonCheckB,pApp);

	if((PX_randRange(0,1))>=0.5)
		pApp->x=PX_APPLICATION_X_A;
	else 
		pApp->x=PX_APPLICATION_X_B;

	if(!PX_MessageBoxInitialize(&pApp->runtime,&pApp->runtime.mp_ui,&pApp->messagebox,&pApp->fontmodule))return PX_FALSE;

	pApp->fileExplorer=PX_Object_ExplorerCreate(&pApp->runtime.mp_ui,pApp->root,0,0,pApp->runtime.surface_width,pApp->runtime.surface_height,&pApp->fontmodule,\
		PX_APP_ExplorerGetPathFolderCount,\
		PX_APP_ExplorerGetPathFileCount,\
		PX_APP_ExplorerGetPathFolderName,\
		PX_APP_ExplorerGetPathFileName,
		""
		);


	PX_ObjectRegisterEvent(pApp->openfile,PX_OBJECT_EVENT_EXECUTE,PX_APP_OnButtonFileExplorer,pApp);
	PX_ObjectRegisterEvent(pApp->fileExplorer,PX_OBJECT_EVENT_EXECUTE,PX_APP_OnButtonFileExplorerConfirm,pApp);
	do 
	{
		px_char fmt[16];
		PX_sprintf2(fmt,sizeof(fmt),"%1/%2",PX_STRINGFORMAT_INT(pApp->correctcount),PX_STRINGFORMAT_INT(pApp->testcount));
		PX_Object_LabelSetText(pApp->correct_test,fmt);
	} while (0);
	return PX_TRUE;
}

px_void PX_ApplicationUpdate(PX_Application *pApp,px_dword elpased)
{
	elpased>100?100:elpased;
	PX_ObjectUpdate(pApp->root,elpased);
	PX_MessageBoxUpdate(&pApp->messagebox,elpased);
}

px_void PX_ApplicationRender(PX_Application *pApp,px_dword elpased)
{
	px_surface *pRenderSurface=&pApp->runtime.RenderSurface;
	PX_RuntimeRenderClear(&pApp->runtime,PX_OBJECT_UI_DEFAULT_BACKGROUNDCOLOR);
	if (pApp->messagebox.show)
	{
		PX_MessageBoxRender(pRenderSurface,&pApp->messagebox,elpased);
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	PX_ObjectRender(pRenderSurface,pApp->root,elpased);
}

px_void PX_ApplicationPostEvent(PX_Application *pApp,PX_Object_Event e)
{
	PX_ApplicationEventDefault(&pApp->runtime, e);
	if (e.Event==PX_OBJECT_EVENT_WINDOWRESIZE)
	{
		PX_UIUpdateObjectsPostions(&pApp->ui,pApp->root,&pApp->ui_json.rootValue,pApp->runtime.surface_width,pApp->runtime.surface_height);
	}
	if (pApp->messagebox.show)
	{
		PX_MessageBoxPostEvent(&pApp->messagebox,e);
		return;
	}
	PX_ObjectPostEvent(pApp->root,e);
}

