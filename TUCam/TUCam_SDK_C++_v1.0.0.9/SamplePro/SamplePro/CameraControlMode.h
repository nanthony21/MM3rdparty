#pragma once

// CCameraControlMode 对话框
#define UPSPIN					-1				         // 曝光时间编辑框上箭头
#define DOWNSPIN				1				         // 曝光事件编辑框下箭头
	
#define STEP_VSCROLL			10		
#define STEP_MOVE				50

#include <vfw.h>
#include "../sdk/inc/TUCamApi.h"                         // 导入SDK头文件
#include "afxwin.h"
#include "afxcmn.h"
#include <dbt.h>
#include "TUButton.h"

#define  PAGE_CNT 6                                     // 总共6个Tab小功能
#define  MAKETAG(a,b,c,d) (a | (b << 8) | (c << 16) | (d << 24))

///#include <stdio.h>

#define TUDBG_PRINTF(format,...)	{char dbgMsg[1024] = {0}; sprintf_s(dbgMsg, "" format"", ##__VA_ARGS__); OutputDebugStringA(dbgMsg);}


class CCameraControlMode : public CDialog
{
	DECLARE_DYNAMIC(CCameraControlMode)

public:
	CCameraControlMode(CWnd* pParent = NULL);           // 标准构造函数
	virtual ~CCameraControlMode();
	CDialog *m_tabPage[PAGE_CNT];                       // 子对话框指针

	
    void InitDrawingResource();                         // 初始化绘制资源
    void UnInitDrawingResource();                       // 反初始化绘制资源

    void InitInformation();                             // 初始化信息
    void InitControlRange();                            // 初始化控件范围
    void EnableControl(BOOL bLiving = FALSE);           // 控件使能    
//  void EnableTriggerControl(BOOL bEnable = FALSE);    // 触发控制使能
    void RefreshValue();                                // 刷新控件值
    void UpdateExposureTime(DWORD dwExp);               // 更新曝光时间 
    void UpdatePicview();                               // 刷新预览界面

    void StartWaitForFrame();  
    void StopWaitForFrame();  
    
	int FindUSBKomPID(CString str);
	int FindUSBKomVID(CString str);
// 对话框数据
	enum { IDD = IDD_DLG_CTL}; 

protected:
	//LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam); // 禁用窗口移动
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	HACCEL hAccKey;
public:

    static void __cdecl WaitForFrameThread(LPVOID lParam); // 等待数据线程实体
	
	CFont				m_fntEdit;						// 相机控制模块标题栏字体
	CString				m_strFilePath;					// 文件地址
	BOOL				m_bShowScroll;					// 是否显示滚动条

	int					m_nCamCtlView_Width;			// 相机控制区宽度
	int					m_nCamCtlView_Heigth;			// 相机控制区高度
	int					m_nFactScreen_Width;			// 实际窗口宽度
	int					m_nFactScreen_Heigth;			// 实际窗口高度
	int					m_nScrollWindow_Y;				// 屏幕滚动（恢复相机控制区域原状）

	SCROLLINFO			m_Scrollinfo;					// 滚动条信息

    /* 相机初始化参数 */
    TUCAM_INIT          m_itApi;                        // 初始化SDK环境
    TUCAM_OPEN          m_opCam;                        // 打开相机参数

    BOOL                m_bWaitting;                    // 捕获相机数据
    BOOL                m_bLiving;                      // 预览相机
    BOOL                m_bSaving;                      // 图像保存
    int                 m_nCntSave;                     // 保存计数
    int                 m_nFmtSave;                     // 保存格式
    int                 m_nFmtCnt;                      // 格式计数
    char                m_cPath[MAX_PATH];              // 保存路径

    /* 位图绘制参数 */
    HANDLE              m_hThdDraw;                     // 显示线程句柄
    BITMAPINFOHEADER    m_bmpInfo;                      // 位图头部信息
    HDC				    m_hDC;                          // 绘制位图DC
    HDRAWDIB		    m_hDib;                         // 绘制位图DIB
    TUCAM_FRAME         m_frame;                        // 帧对象

    TUCAM_DRAW_INIT     m_itDraw;                       // 绘制初始化参数
    TUCAM_DRAW          m_drawing;                      // 绘制参数

    /* 帧率计算 */
    DWORD               m_dwSTm;                        // 开始计数
    DWORD               m_dwITm;                        // 间隔时间
    DWORD               m_dwFrmCnt;                     // 帧数统计
    float               m_fFps;                         // 帧率

    /* 绘制区域计算 */
    float               m_fScale;                       // 缩放比例
    int                 m_nCurWidth;                    // 当前宽度
    int                 m_nCurHeight;                   // 当前高度
    int                 m_nCliWidth;                    // 当前客户端宽度
    int                 m_nCliHeight;                   // 当前客户端高度
    int                 m_nDrawOffX;                    // 水平偏移  
    int                 m_nDrawOffY;                    // 垂直偏移
    int                 m_nDrawWidth;                   // 绘制宽度
    int                 m_nDrawHeight;                  // 绘制高度

    /* 触发 */
    BOOL                m_bTgrMode;                     // 触发模式
    BOOL                m_bTrigger;                     // 触发状态
    HANDLE              m_hThdTrigger;                  // 触发线程句柄

    HANDLE              m_hThdWaitForFrame;             // 数据捕获线程句柄

    int                 m_nCnt;

	UINT                m_uiTriFrame;                   // 触发帧

    /*保存命名规则 ImageName+Times+Frames*/
    UINT                m_uiCapTimes;                   // 次数
    UINT                m_uiCapFrames;                  // 帧数
    
	HDEVNOTIFY			m_hHDevNotify;
	HDEVNOTIFY			m_hLDevNotify;
	HDEVNOTIFY			m_hUDevNotify;
public:
	/*相机开关*/
	BOOL OnBnClickedBtnLive();

	/*相机ROI按钮*/
	BOOL OnBtnRoiFunction();

	/*刷新ROI按钮*/
	BOOL OnRefreshRoiState();

	/*相机自定义ROI功能*/
	BOOL OnChkRoiFunction();

	/*相机录像*/
	BOOL OnBtnRecord();

	/*相机色阶范围刷新*/
	BOOL OnRefreshLevelRange();

	/*相机界面Raw按钮刷新*/
	BOOL OnBtnRefreshRaw(BOOL bRaw = FALSE);

	/*改变窗口大小*/
	afx_msg void OnSize(UINT nType, int cx, int cy);

	/*滚动条事件*/
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	/*设置滚动条尺寸大小*/
	void SetScrollSize(int nWidth, int nHeigth);

	/*关闭销毁*/
    afx_msg void OnDestroy();

	/*INFO收缩按钮*/
	afx_msg void OnBnClickedBtnInfo();

    afx_msg void OnBnClickedBtnControlcontrol();
    afx_msg void OnBnClickedBtnImagecapture();
    afx_msg void OnBnClickedBtnMorepara();
    afx_msg void OnBnClickedBtnRoi();
    afx_msg void OnBnClickedBtnTrigger();

	/*功能界面选择*/
	//afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
    
	/*禁用键盘按键*/
    virtual BOOL PreTranslateMessage(MSG* pMsg);



    void UpdateControl();

	INT                 m_nPicNum;    // 拍照图片数量
    BOOL                m_bUseTm;
    CString             m_szPicName;  // 文件名称信息
	int                 m_nRecTm;     // 录像计时

    int m_nTriMode;

	/* 录像 */
    BOOL                m_bRecording;                   // 是否正在录像  
    
	/* 标签页 */
	//CTabCtrl            m_tab;                          //添加一个CTab功能模块

	
	/* ROI绘制 */
	BOOL               m_bChk;
	UINT               m_uHOffset;
    UINT               m_uVOffset;
    UINT               m_uWidth;
    UINT               m_uHeight;

	DWORD              m_dwFccHandler;                // 编码解码器句柄  

	BOOL               OnCaptureSnap();               // View 拍照
	BOOL               OnCaptureRecord();             // View 录像
	BOOL               OnCaptureTriger();             // View 触发
	BOOL               OnCapturePlay();               // Viww 预览
    
	BOOL               m_bshow0;                      //表示是否收缩的标志位
	BOOL               m_bshow1;                      //表示是否收缩的标志位
	BOOL               m_bshow2;                      //表示是否收缩的标志位
	BOOL               m_bshow3;                      //表示是否收缩的标志位
	BOOL               m_bshow4;                      //表示是否收缩的标志位
	BOOL               m_bshow5;                      //表示是否收缩的标志位
	BOOL               m_bshow6;                      //表示是否收缩的标志位
	BOOL               m_bshow7;                      //表示是否收缩的标志位

	CRect              rectSmall;                     //收缩时显示最小化
	CRect              rectLarge;                     //扩展时显示最大化
	int                m_nheight;                     //滚动条高度

    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
   
	afx_msg LRESULT OnDeviceChange(WPARAM wParam, LPARAM lParam);  //USB中断消息获取

    CTUButton m_btnInfo;
    CTUButton m_btnCtrl;
    CTUButton m_btnImgCap;
    CTUButton m_btnROI;
    CTUButton m_btnTrigger;
    CTUButton m_btnImgAdjustment;
};
