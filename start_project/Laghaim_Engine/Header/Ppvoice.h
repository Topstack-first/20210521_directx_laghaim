
class CPpvoice
{
public:
	CPpvoice();
	virtual ~CPpvoice();

	int Init( /*HINSTANCE hInst,*/ HWND hWnd);
	BOOL ConnectHost(const char *hostname);
	BOOL JoinRoom();
	BOOL ConnectComplete();
	HRESULT CreateRoom();
	int ChatStart(HWND hWnd);
	int VoiceInit(HWND hWnd);
	//int Chatting();
	int ChatEnd();

	// 모든 채팅을 관리하는 부분...
	void Chatting();

	// 셋업 관련...
	int CheckAudioSetuped(HWND hWnd);
	int RunAudioSetup(HWND hWnd);

	BOOL m_bConnectSuccess;
	BOOL m_bChatStarted;

	int m_ChatStatus;
	HWND m_hWnd;

	// 테스트용...
	char* GetURL();
};

#define CHAT_NONE		0
#define CHAT_JOIN		1
#define CHAT_CONNECTED	2
#define CHAT_STARTED	3
#define CHAT_CHATTING	4

extern DWORD GetDXVersion();

//int PpvoiceRun( HINSTANCE hInst);