/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of duihome.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include <DuiApplication>
#include <DuiApplicationWindow>
#include <QPaintEngine>
#include <QPaintDevice>
#include <DuiButton>
#include <DuiSceneManager>
#include <DuiScene>
#include <DuiDeviceProfile>

#include "ut_desktopview.h"
#include "desktopview.h"
#include "desktop.h"
#include "notificationarea_stub.h"
#include "switcher_stub.h"
#include "windowinfo_stub.h"
#include "homeapplication_stub.h"
#include "notificationmanager.h"
#include "mainwindow_stub.h"
#include "applicationcontext.h"
#include <QDBusConnection>
#include "contextframeworkcontext_stub.h"
#include "phonenetworkstatusindicator_stub.h"
#include "statusindicator_stub.h"
#include "bluetoothstatusindicator_stub.h"
#include "phonenetworksignalstrengthstatusindicator_stub.h"
#include "internetconnectionstatusindicator_stub.h"
#include "clockalarmstatusindicator_stub.h"
#include "batterystatusindicator_stub.h"
#include "appletspace_stub.h"

// Mock Paint Engine
class MockPaintEngine : public QPaintEngine
{
    bool begin(QPaintDevice *pdev) {
        Q_UNUSED(pdev);
        return true;
    }
    bool end() {
        return true;
    }
    void updateState(const QPaintEngineState &state) {
        Q_UNUSED(state);
    }
    void drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr);
    Type type() const {
        return QPaintEngine::User;
    }
};

void MockPaintEngine::drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr)
{
    Q_UNUSED(pm);
    Q_UNUSED(sr);
    Ut_DesktopView::paintArea = Ut_DesktopView::paintArea.united(r);
}

// Mock Paint Device
class MockPaintDevice : public QPaintDevice
{
public:
    MockPaintDevice() : engine(new MockPaintEngine) {}
    ~MockPaintDevice();

    QPaintEngine *paintEngine() const {
        return engine;
    }

    int metric(PaintDeviceMetric metric) const;
private:
    QPaintEngine *engine;
};

MockPaintDevice::~MockPaintDevice()
{
    delete engine;
}

int MockPaintDevice::metric(PaintDeviceMetric metric) const
{
    switch (metric) {
    case QPaintDevice::PdmWidth:
        return 1000;
    case QPaintDevice::PdmHeight:
        return 1000;
    case QPaintDevice::PdmDpiY:
    case QPaintDevice::PdmDpiX:
        return 300;
    default:
        return 0;
    }
    return 0;
}

// QDBusConnection stubs
bool QDBusConnection::registerObject(QString const &, QObject *, QFlags<QDBusConnection::RegisterOption>)
{
    return true;
}

// DuiTheme stubs
void DuiTheme::releasePixmap(const QPixmap *)
{
}

// Desktop stubs
Desktop::Desktop(DuiWidget *parent) : DuiWidgetController(new DesktopModel, parent)
{
}

Desktop::~Desktop()
{
}

// Test desktop view implementation
TestDesktopView::TestDesktopView(Desktop *desktop) : DesktopView(desktop)
{
}

// Tests
QRectF Ut_DesktopView::paintArea;
MainWindow *Ut_DesktopView::mainWindow;

void Ut_DesktopView::initTestCase()
{
    // DuiApplications must be created manually these days due to theme system changes
    static int argc = 1;
    static char *app_name = (char *)"./ut_desktopview";
    app = new HomeApplication(argc, &app_name);
    mainWindow = MainWindow::instance(true);
    gHomeApplicationStub->stubSetReturnValue("mainWindow", mainWindow);

    paintDevice = new MockPaintDevice;
    painter = new QPainter(paintDevice);

    backgroundImage = new QPixmap(50, 50);
    backgroundTopImage = new QPixmap(25, 25);
    backgroundBottomImage = new QPixmap(35, 35);
}

void Ut_DesktopView::cleanupTestCase()
{
    delete painter;
    delete paintDevice;
    delete mainWindow;

    // Destroy the DuiApplication
    delete app;
}

void Ut_DesktopView::init()
{
    desktop = new Desktop;
    desktopView = new TestDesktopView(desktop);
    desktop->setView(desktopView);
    desktopView->modifiableStyle()->setDesktopBackgroundImage(backgroundImage);
    desktopView->modifiableStyle()->setDesktopBackgroundTop(backgroundTopImage);
    desktopView->modifiableStyle()->setDesktopBackgroundBottom(backgroundBottomImage);
    paintArea = QRectF();
}

void Ut_DesktopView::cleanup()
{
    delete desktop;
}

void Ut_DesktopView::testBoundingRectAndDrawBackground()
{
    desktop->setGeometry(QRectF(0, 0, 50, 50));
    QRectF br = desktopView->boundingRect();

    QVERIFY(br.contains(QRectF(0, 0, DuiDeviceProfile::instance()->resolution().width(), DuiDeviceProfile::instance()->resolution().height())));
    QCOMPARE((int)br.y(), -backgroundTopImage->height());

    // Check that the view doesn't draw outside the bounding rectangle
    desktopView->drawBackground(painter, NULL);
    QVERIFY(br == paintArea || br.contains(paintArea) || paintArea.isEmpty());
}

QTEST_APPLESS_MAIN(Ut_DesktopView)
