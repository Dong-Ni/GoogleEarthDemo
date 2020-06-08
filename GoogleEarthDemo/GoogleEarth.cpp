#include "GoogleEarth.h"
#include "earthlib.h"
#include <Windows.h>
#include <qDebug>
#include <tchar.h>

using namespace EARTHLib;

GoogleEarthDemo::GoogleEarthDemo(QWidget* parent)
	: QWidget(parent)
	, appGE(NULL)
{
	ui.setupUi(this);

	//试图清除其他事先运行的GE
	this->KillProcess("Google Earth");
	this->KillProcess("GoogleEarth");
	this->KillProcess("GoogleCrashHandler");

	Init();
	connect(ui.btnFir, SIGNAL(clicked()), this, SLOT(OnBtnBingJiangClicked()));
	connect(ui.btnSec, SIGNAL(clicked()), this, SLOT(OnBtnBeiJingClicked()));
}

GoogleEarthDemo::~GoogleEarthDemo()
{
	Exit();
}

void GoogleEarthDemo::Init()
{
	CoInitialize(NULL);

	appGE = new ApplicationGE();
	renderOLE = appGE->GetRenderHwnd();
	mainOLE = appGE->GetMainHwnd();

	renderParentHandle = GetParent((HWND)renderOLE);
	mainHandle = (HWND)(mainOLE);

	WId id = ui.wgt->winId();
	SetParent(renderParentHandle, (HWND)id);
	ShowWindow(mainHandle, SW_HIDE);
	if (SUCCEEDED(appGE))
	{
		while (appGE->IsInitialized() == 0)
		{
			qDebug() << QStringLiteral("已经登录");
			Sleep(500);
		}
	}

	qDebug() << "isOnLine" << appGE->IsOnline();
	appGE->Login();
	qDebug() << "ver" << appGE->VersionMajor() << appGE->VersionMinor() << appGE->VersionBuild();

	qDebug() << QStringLiteral("初始化位置");
}

void GoogleEarthDemo::Exit()
{
	// Close the main Google Earth app
	PostMessage(mainHandle, WM_QUIT, 0, 0);
	appGE->deleteLater();
}

void GoogleEarthDemo::KillProcess(const QString& strProcName)
{
	DWORD dwProcID;
	HANDLE hProcess;
	TCHAR wProcName[260];
	int Len = strProcName.toWCharArray(wProcName);
	wProcName[Len] = _T('\0');
	HWND h = FindWindow(0, wProcName);
	if (h)
	{
		GetWindowThreadProcessId(h, &dwProcID);
		hProcess = OpenProcess(PROCESS_TERMINATE, JOB_OBJECT_ASSIGN_PROCESS, dwProcID);
		TerminateProcess(hProcess, 0);
		qDebug() << "Kill Process :" << wProcName;
	}
}

void GoogleEarthDemo::resizeEvent(QResizeEvent* event)
{
	QRect rect = ui.wgt->rect();
	qDebug() << "Resize:" << MoveWindow(mainHandle, 0, 0, rect.width(), rect.height(), false);
	QWidget::resizeEvent(event);
}

void GoogleEarthDemo::OnBtnBeiJingClicked()
{
	//RelativeToGroundAltitudeGE 相对于该地面的位置的高度
	//AbsoluteAltitudeGE 相对于海平面的位置的高度
	appGE->SetCameraParams(39.92, 116.40, 100.0, RelativeToGroundAltitudeGE, 10000, 0.0, 0.0, 2.0);
}

void GoogleEarthDemo::OnBtnBingJiangClicked()
{
	appGE->SetCameraParams(30.16, 120.15, 100.0, RelativeToGroundAltitudeGE, 15000, 0.0, 0.0, 2.0);
}
