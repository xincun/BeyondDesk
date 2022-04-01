#pragma once

//enum TPlugType  { ptNULL/*��ʼ���ã���Ҫʵ��ʹ��*/, ptBoot, ptBackground, ptDeskWidget, ptTimeTool, ptMediaTool, ptNormalTool };
#define  PLUG_TYPE_BOOT  0x00000001L//�ﵽ��������Ч��
#define  PLUG_TYPE_BACKGROUND  0x00000010L
#define  PLUG_TYPE_DESKWIDGET  0x00000100L
#define  PLUG_TYPE_TIMETOOL  0x00001000L
#define  PLUG_TYPE_MEDIATOOL  0x00010000L
#define  PLUG_TYPE_NORMALTOOL  0x00100000L


class TPlugInfo;
class IPlugBase
{
private:
	TPlugInfo* m_PlugInfo;

protected:
	bool m_bCanNew;
	bool m_bCanSet;
	bool m_bEnabled;
	TString m_sIntfName;
protected:
public:
	TPlugInfo* GetPlugInfo() const { return m_PlugInfo; }
	void SetPlugInfo(TPlugInfo* val) { m_PlugInfo = val; }

	IPlugBase() :m_bCanNew(false), m_bCanSet(false), m_sIntfName(_T("")), m_bEnabled(true) {};

	virtual ~IPlugBase() {};

	bool GetCanNew() const { return m_bCanNew; }
	void SetCanNew(bool val) { m_bCanNew = val; }
	bool GetCanSet() const { return m_bCanSet; }
	void SetCanSet(bool val) { m_bCanSet = val; }

	const TString& GetIntfName() const { return m_sIntfName; }
	void SetIntfName(const TString& val) { m_sIntfName = val; }

	//TPlugType GetPlugType() const { return m_PlugType; }

	//����¼�
	virtual void OnCreate() {};
	virtual void OnDestroy() {};
	virtual void OnUnload(void *pSender) {};
	virtual void OnLoad(void *pSender) {};

	//��������ʱ
	virtual void OnInvoke(void *pSender, int wParam = 0, const TString &lParam = _T(""), const TString &eParam = _T("")) {};
	//�ⲿ���ã�����ĳ�ڲ�����ֵ
	virtual TString OnGet(void *pSender, int wParam = 0, const TString &lParam = _T(""), const TString &eParam = _T("")) { return _T(""); };
	//�½�һ���ڲ�����ʱ�����罨����������ʱ�Ӵ���
	virtual void OnNew(void* pSender = NULL) {}; 

	//�������ʱ
	virtual void OnSet(void* pSender = NULL) {};
	//void OnConfig(void *pSender, int wParam = 0, const TString &lParam = L"",const TString &eParam = L"") = 0;
	//void OnPreConfig(void *pSender);
	virtual bool GetEnabled() const { return m_bEnabled; }
	virtual void SetEnabled(bool val) { m_bEnabled = val; }
};

class IPlugBase;
class TPlugInfo
{
	vector<IPlugBase*> IntfList;
public:
	//����������
	HINSTANCE Instance;
	TString Path;
	TString FileName;

	void AddPlugIntf(IPlugBase*p)
	{
		p->SetPlugInfo(this);
		IntfList.push_back(p);
	}

	//��������
	TString Name;
	UINT Type;
	TString Desc;
	bool Persistent; //�������������ڳ־ô��ڣ��������к�����ж��
	int Order; //����˳��
	UINT PlugIntfCount;
	
	
	//����
	IPlugBase* operator[](UINT nIndex)
	{
		return IntfList[nIndex];
	}

	IPlugBase* GetPlugIntf(UINT nIndex = 0)
	{
		return IntfList[nIndex];
	}

	/*const TString& GetPlugPath() const { return m_sPlugPath; }
	void SetPlugPath(const TString& val) { m_sPlugPath = val; }
	const TString& GetPlugFileName() const { return m_sPlugFileName; }
	void SetPlugFileName(const TString& val) { m_sPlugFileName = val; }*/

	TPlugInfo() : Name(_T("")), Type(0), Desc(_T("")), PlugIntfCount(1), Order(999), Persistent(true)/*, m_sPlugFileName(_T("")), m_sPlugPath(_T(""))*/{};
};

typedef vector<TPlugInfo*> TPlugInfoList;

class IUnify;
typedef  TPlugInfo* (*lpFuncGetInfo)(IUnify *p);
typedef  IPlugBase* (*lpFuncGetIntf)();
typedef void(*lpFuncFreeIntf)();

//���������������ִ�еĲ��
class IBootPlug : public IPlugBase
{};

class IControlPlug : public IPlugBase //����ڴ����ϱ�ʾΪһ���ؼ�
{
protected:
	TString m_sCaption;
	TString m_sHint;
	TString m_sIconFile; //ͼ���ļ�·��
public:
	const TString& GetCaption() const { return m_sCaption; } //���⣬������չʾ��
	void SetCaption(const TString &val) { m_sCaption = val; }
	const TString &GetHint() const { return m_sHint; } //���Hint
	void SetHint(const TString &val) { m_sHint = val; }
	const TString& GetIconFile() const { return m_sIconFile; }
	void SetIconFile(const TString& val) { m_sIconFile = val; }

	virtual void OnClick(void *pSender = NULL) {}; //�����ʱ
};

//�������ڲ��
class IToolPlug : public IControlPlug
{
};

//������, ���һ���ֱ���Ϊ���������ϵĲ��
class IUIPlug : public IControlPlug
{
public:
	virtual CControlUI* GetUI() = 0; //�����ϵ�UI
	virtual void Init(CPaintManagerUI* pManager) = 0; //��ʼ������Ϊ�ڳ�ʼ����е�Ҫ�ȴ��ؼ�������ɣ����Ե�������һ���ӿ�
};

class IPlugManager
{
private:
	
public:
	virtual ~IPlugManager() {};

	virtual void LoadPlugs(LPCTSTR szPlugPath) = 0;
	virtual void UnLoadPlugs() = 0;
	virtual void GetPlugsByType(TPlugInfoList& outVector, UINT plgType) = 0;
	virtual TPlugInfo* GetPlugInfoByName(LPCTSTR szName) = 0;
};