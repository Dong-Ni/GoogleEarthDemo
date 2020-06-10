#include "GoogleEarthDemo.h"
#include "earthlib.h"
#include <Windows.h>
#include <qDebug>
#include <tchar.h>
#include <QDir>

using namespace EARTHLib;

GoogleEarthDemo::GoogleEarthDemo(QWidget *parent)
	: QWidget(parent)
	, appGE(NULL)
{
	ui.setupUi(this);

	Init();
	connect(ui.btnFir, SIGNAL(clicked()), this, SLOT(OnBtnBingJiangClicked()));
	connect(ui.btnSec, SIGNAL(clicked()), this, SLOT(OnBtnBeiJingClicked()));
	connect(ui.btnGet, SIGNAL(clicked()), this, SLOT(OnBtnGetClicked()));
	connect(ui.btnChange, SIGNAL(clicked()), this, SLOT(OnBtnChangeClicked()));
	connect(ui.btnSave, SIGNAL(clicked()), this, SLOT(OnBtnSavePicClicked()));
	connect(ui.btnKML, SIGNAL(clicked()), this, SLOT(OnBtnOpenKMLClicked()));
}

GoogleEarthDemo::~GoogleEarthDemo()
{
	Exit();
}

void GoogleEarthDemo::Init()
{
	CoInitialize(NULL);

	appGE = new ApplicationGE();
	Sleep(500);
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
			qDebug() << QStringLiteral("�Ѿ���¼");
			Sleep(500);
		}
	}

	//qDebug() << "isOnLine" << appGE->IsOnline();
	while (appGE->IsOnline() == 0)
	{
		qDebug() << QStringLiteral("��������");
		Sleep(500);
	}

	appGE->Login();
	qDebug() << "ver" << appGE->VersionMajor() << appGE->VersionMinor() << appGE->VersionBuild();

	qDebug() << QStringLiteral("��ʼ��λ��");
}

void GoogleEarthDemo::Exit()
{
	// Close the main Google Earth app
	PostMessage(mainHandle, WM_QUIT, 0, 0);
	appGE->deleteLater();
}

void GoogleEarthDemo::KillProcess(const QString& strProcName)
{

}

void GoogleEarthDemo::resizeEvent(QResizeEvent* event)
{
	QRect rect = ui.wgt->rect();
	//��С�Բ��� ��ʱ���� ����һ��
	qDebug() << "Resize:" << MoveWindow(mainHandle, 0, 30, rect.width()+270, rect.height()+150, false);
	qDebug() << rect.top() << rect.bottom() << rect.left() << rect.right();
	QObjectList objList = ui.wgt->children();
	QWidget::resizeEvent(event);
}

void GoogleEarthDemo::OnBtnBeiJingClicked()
{
	//RelativeToGroundAltitudeGE ����ڸõ����λ�õĸ߶�
	//AbsoluteAltitudeGE ����ں�ƽ���λ�õĸ߶�
	appGE->SetCameraParams(39.92, 116.40, 100.0, RelativeToGroundAltitudeGE, 16000, 0.0, 0.0, 2.0);
}

void GoogleEarthDemo::OnBtnBingJiangClicked()
{
	appGE->SetCameraParams(30.16, 120.15, 100.0, RelativeToGroundAltitudeGE, 15000, 0.0, 0.0, 2.0);
}

void GoogleEarthDemo::OnBtnGetClicked()
{
	ICameraInfoGE *geInfo = appGE->GetCamera(1);
	if (NULL != geInfo)
	{
		qDebug() << QStringLiteral("���� = ") << geInfo->FocusPointLongitude() << QStringLiteral("γ�� = ") << geInfo->FocusPointLatitude() << QStringLiteral("�߶� = ") << geInfo->FocusPointAltitude()  << "Range =" << geInfo->Range();
	}
}

void GoogleEarthDemo::OnBtnChangeClicked()
{
	double x = ui.edtx->text().toDouble();
	double y = ui.edty->text().toDouble();
	IPointOnTerrainGE *pointInfo = appGE->GetPointOnTerrainFromScreenCoords(x, y);
	if (NULL != pointInfo)
	{
		qDebug() << QStringLiteral("���� = ") << pointInfo->Longitude() << QStringLiteral("γ�� = ") << pointInfo->Latitude() << QStringLiteral("�߶� = ") << pointInfo->Altitude();
	}
}

void GoogleEarthDemo::OnBtnSavePicClicked()
{
	QString strPath = QDir::currentPath();
	//0 ������
	//appGE->SaveScreenShot(strPath + "0", 0);
	//50 ������
	//appGE->SaveScreenShot(strPath + "50", 50);
	//100 ������
	appGE->SaveScreenShot(strPath + "100.jpg", 100);
}

void GoogleEarthDemo::OnBtnOpenKMLClicked()
{
	appGE->OpenKmlFile("D:\\GoogleEarth\\GoogleEarthDemo\\GoogleEarthDemo\\dahuaLocal.kml", 0);
}
