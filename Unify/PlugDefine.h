#pragma once

//enum TPlugType  { ptNULL/*初始化用，不要实际使用*/, ptBoot, ptBackground, ptDeskWidget, ptTimeTool, ptMediaTool, ptNormalTool };
#define  PLUG_TYPE_BOOT  0x00000001L//达到多重类型效果
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

	//插件事件
	virtual void OnCreate() {};
	virtual void OnDestroy() {};
	virtual void OnUnload(void *pSender) {};
	virtual void OnLoad(void *pSender) {};

	//主动调用时
	virtual void OnInvoke(void *pSender, int wParam = 0, const TString &lParam = _T(""), const TString &eParam = _T("")) {};
	//外部调用，返回某内部对象值
	virtual TString OnGet(void *pSender, int wParam = 0, const TString &lParam = _T(""), const TString &eParam = _T("")) { return _T(""); };
	//新建一个内部对象时，比如建立两个桌面时钟窗口
	virtual void OnNew(void* pSender = NULL) {}; 

	//点击设置时
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
	//管理器设置
	HINSTANCE Instance;
	TString Path;
	TString FileName;

	void AddPlugIntf(IPlugBase*p)
	{
		p->SetPlugInfo(this);
		IntfList.push_back(p);
	}

	//派生设置
	TString Name;
	UINT Type;
	TString Desc;
	bool Persistent; //在主程序生命期持久存在，还是运行后立即卸载
	int Order; //加载顺序
	UINT PlugIntfCount;
	
	
	//公用
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

//启动主程序后立即执行的插件
class IBootPlug : public IPlugBase
{};

class IControlPlug : public IPlugBase //插件在窗口上表示为一个控件
{
protected:
	TString m_sCaption;
	TString m_sHint;
	TString m_sIconFile; //图标文件路径
public:
	const TString& GetCaption() const { return m_sCaption; } //标题，主程序展示用
	void SetCaption(const TString &val) { m_sCaption = val; }
	const TString &GetHint() const { return m_sHint; } //鼠标Hint
	void SetHint(const TString &val) { m_sHint = val; }
	const TString& GetIconFile() const { return m_sIconFile; }
	void SetIconFile(const TString& val) { m_sIconFile = val; }

	virtual void OnClick(void *pSender = NULL) {}; //被点击时
};

//工具箱内插件
class IToolPlug : public IControlPlug
{
};

//界面插件, 插件一部分表现为在主界面上的插件
class IUIPlug : public IControlPlug
{
public:
	virtual CControlUI* GetUI() = 0; //附加上的UI
	virtual void Init(CPaintManagerUI* pManager) = 0; //初始化，因为在初始化里，有的要等待控件创建完成，所以单独开放一个接口
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