#include <metahook.h>
#include "Renderer.h"
#include "configs.h"
#include "plugins.h"
#include "interface.h"

//Audio.dll
HINTERFACEMODULE g_hAudio;
IAudio *g_pAudio = NULL;
IPlugins *g_pAudioPlugins = NULL;
aud_export_t gAudExports;

//Renderer.dll
HINTERFACEMODULE g_hRenderer;
IRenderer *g_pRenderer = NULL;
IPlugins *g_pRendererPlugins = NULL;
ref_export_t gRefExports;

//ckf3.dll
HINTERFACEMODULE g_hCKFClient;
ICKFClient *g_pCKFClient = NULL;
IPlugins *g_pCKFClientPlugins = NULL;
cl_exportfuncs_t gCkfClientFuncs;

ckf_vars_t gCKFVars;

//bte client
int BTE_GetHUDFov(void);
float *EV_GetPunchAngles(void);
pfnUserMsgHook MSG_HookUserMsg(char *szMsgName, pfnUserMsgHook pfn);
xcommand_t Cmd_HookCmd(char *cmd_name, xcommand_t newfuncs);

int IBTEClient::GetHUDFov(void)
{
	return BTE_GetHUDFov();
}

float *IBTEClient::GetPunchAngles(void)
{
	return EV_GetPunchAngles();
}

pfnUserMsgHook IBTEClient::HookUserMsg(char *pMsgName, pfnUserMsgHook pfnHook)
{
	return MSG_HookUserMsg(pMsgName, pfnHook);
}

xcommand_t IBTEClient::HookCmd(char *cmd_name, xcommand_t newfuncs)
{
	return Cmd_HookCmd(cmd_name,  newfuncs);
}

extern HWND g_hMainWnd;

HWND IBTEClient::GetMainHWND(void)
{
	return g_hMainWnd;
}

EXPOSE_SINGLE_INTERFACE(IBTEClient, IBTEClient, BTECLIENT_API_VERSION);

void Renderer_Init(void)
{
	if (!gConfigs.bEnableRenderer)
		return;

	char filepath[MAX_PATH];
	sprintf(filepath, "%s/Renderer.dll", gConfigs.szGameDir);

	g_hRenderer = Sys_LoadModule(filepath);

	if (!g_hRenderer)
	{
		gConfigs.bEnableRenderer = false;
		return;
	}

	g_pRenderer = (IRenderer *)((CreateInterfaceFn)Sys_GetFactory(g_hRenderer))(RENDERER_API_VERSION, NULL);
	g_pRendererPlugins = (IPlugins *)((CreateInterfaceFn)Sys_GetFactory(g_hRenderer))(METAHOOK_PLUGIN_API_VERSION, NULL);

	if (!g_pRenderer)
	{
		gConfigs.bEnableRenderer = false;
	}
	if (!g_pRendererPlugins)
	{
		gConfigs.bEnableRenderer = false;
	}
	if (g_pRenderer)
	{
		g_pRenderer->GetInterface(&gRefExports, META_RENDERER_VERSION);
	}
}

void CKF_Init(void)
{
	char filepath[MAX_PATH];
	sprintf(filepath, "%s/CaptionMod.dll", gConfigs.szGameDir);

	g_hCKFClient = Sys_LoadModule(filepath);

	memset(&gCkfClientFuncs, 0, sizeof(gCkfClientFuncs));

	if (!g_hCKFClient)
		return;

	g_pCKFClient = (ICKFClient *)((CreateInterfaceFn)Sys_GetFactory(g_hCKFClient))(CKFCLIENT_API_VERSION, NULL);
	g_pCKFClientPlugins = (IPlugins *)((CreateInterfaceFn)Sys_GetFactory(g_hCKFClient))(METAHOOK_PLUGIN_API_VERSION, NULL);

	if (!g_pCKFClient)
		return;

	if (!g_pCKFClientPlugins)
		return;

	g_pCKFClient->GetClientFuncs(&gCkfClientFuncs);
	g_pCKFClient->GetCKFVars(&gCKFVars);
}

void Audio_Init(void)
{
	char filepath[MAX_PATH];
	sprintf(filepath, "%s/Audio.dll", gConfigs.szGameDir);

	g_hAudio = Sys_LoadModule(filepath);

	if (!g_hAudio)
	{
		return;
	}

	g_pAudio = (IAudio *)((CreateInterfaceFn)Sys_GetFactory(g_hAudio))(AUDIO_API_VERSION, NULL);
	g_pAudioPlugins = (IPlugins *)((CreateInterfaceFn)Sys_GetFactory(g_hAudio))(METAHOOK_PLUGIN_API_VERSION, NULL);

	if (!g_pAudio)
	{
		return;
	}
	if (!g_pAudioPlugins)
	{
		return;
	}

	g_pAudio->GetInterface(&gAudExports, META_AUDIO_VERSION);
}

void Renderer_Shutdown(void)
{
	if (g_pRenderer)
		Sys_FreeModule(g_hRenderer);
}

void CKF_Shutdown(void)
{
	if (g_pCKFClient)
		Sys_FreeModule(g_hCKFClient);
}

void Audio_Shutdown(void)
{
	if (g_pAudio)
		Sys_FreeModule(g_hAudio);
}