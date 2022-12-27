#import "C:\\program files\\xvision\\sense reader\\xvsrd.exe"
#include <windows.h>
#include <stdlib.h>
using namespace SenseReader;
IXVApplication * pinterface;
IXVApplication2 * pinterface2;
HRESULT hr;
const char * geterrorstring()
{
static char errormessage[200];
if(FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, hr, 0, errormessage, 200, 0)) return errormessage;
else return "geterrorstring failed";
}

int sensereader_init()
{
CLSID clsid_sensereader=__uuidof(SenseReaderApplication);
IID iid_ixvapplication=__uuidof(IXVApplication);
CoInitialize(NULL);
hr=CoCreateInstance(clsid_sensereader, NULL, CLSCTX_LOCAL_SERVER, iid_ixvapplication, &(void *)pinterface);
if (FAILED(hr)) return 0;
else return 1;
}
int speak(const char * text)
{
wchar_t * wtext=(wchar_t *)malloc(sizeof(wchar_t)*strlen(text)+1);
if (!MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, text, -1, wtext, strlen(text)+1))
{
hr=(HRESULT)GetLastError();
return 0;
}
hr=pinterface->Speak(wtext);
free(wtext);
if (FAILED(hr)) return 0;
else return 1;
}
void stopspeaking()
{
hr=pinterface->StopSpeaking();
}
const char * getsensereaderid()
{
const wchar_t * wid;
static char id[200];
if (pinterface->GetDemoMode()) return "";
if (!pinterface2) hr=pinterface->QueryInterface(__uuidof(IXVApplication2), &(void *)pinterface2);
wid=pinterface2->GetSenseReaderId();
if (!WideCharToMultiByte(CP_ACP, 0, wid, -1, id, 200, NULL, NULL))
{
hr=(HRESULT)GetLastError();
return (const char *)0;
}
return id;
}
void sensereader_release()
{
if (pinterface2) pinterface2->Release();
pinterface->Release();
CoUninitialize();
}
