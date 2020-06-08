#pragma once

#include <QtWidgets/QWidget>
#include "ui_GoogleEarth.h"

namespace EARTHLib
{
	class ApplicationGE;
}

class GoogleEarthDemo : public QWidget
{
	Q_OBJECT

public:
	GoogleEarthDemo(QWidget* parent = Q_NULLPTR);
	~GoogleEarthDemo();

	void Init();
	void Exit();
	void KillProcess(const QString& strProcName);

public slots:
	void OnBtnBingJiangClicked();
	void OnBtnBeiJingClicked();

protected:
	virtual void resizeEvent(QResizeEvent* event);

private:
	Ui::GoogleEarthDemoClass ui;
	EARTHLib::ApplicationGE* appGE;
	int renderOLE;
	int mainOLE;
	HWND renderParentHandle;
	HWND mainHandle;
};