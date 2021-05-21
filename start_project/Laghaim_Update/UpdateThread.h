#ifndef _UPDATETHREAD_H__
#define _UPDATETHREAD_H__

class CAutoUpgrade;

bool PatchFile(char* szConnectIp, CAutoUpgrade* pUpdate, int ver_idx, int ver_diff, char* szFile);
__declspec(dllexport) void UpdateThread();
bool PatchProc(bool repatch);

bool ConnectPatchServer(char* szConnectIp, CAutoUpgrade* pUpdate);
int VersionCheck(char* szConnectIp, CAutoUpgrade* pUpdate, char* file);

char* GetOneLine(char *arg, char *buf);
int m_LocalVersion;
BOOL GetLocalVersion();
void SetLocalVersion();
#endif