/*
 * Copyright (C) 2019 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/&gt;.
 *
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGSettings>
#include <QSystemTrayIcon>
//#include "powerdispwidget.h"
#include <QMenu>
#include <QScrollArea>
#include "device_widget.h"
#include "enginedevice.h"
#include <QDBusObjectPath>
#include <QDBusMessage>
#include "customtype.h"
#include <QDBusConnection>
#include <QMap>
#include <QListWidgetItem>
#include <QWidgetAction>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

//    void initUi();
    void initUi2();
    void get_power_list();
    void initData();
    int get_engine_dev_number();
    QPixmap set_percent_pixmap(QString str);
    QIcon get_percent_icon(QIcon icon);
    bool want_percent;
public Q_SLOTS:
    void iconThemeChanged();
    void onActivatedIcon(QSystemTrayIcon::ActivationReason reason);
    void set_preference_func();
    void show_percentage_func();
    void set_brightness_func();

    void onIconChanged(QString str);
    void onSumChanged(QString str);
    void low_battery_notify(DEV dev);
    void critical_battery_notify(DEV dev);
    void action_battery_notify(DEV dev);
    void discharge_notify(DEV dev);
    void full_charge_notify(DEV dev);
    void add_one_device(DEVICE *device);
    void remove_one_device(DEVICE *device);
private:
//    QGSettings *setting;
//    PowerDispWidget *dispWidget;
    QSystemTrayIcon* trayIcon;
    bool disp_control;
    QList<QDBusObjectPath> deviceNames;
    EngineDevice* ed;

    QMenu *menu;
    QWidgetAction *set_preference;
    QWidgetAction *show_percentage;
    QLabel *percent_label;
    QWidgetAction *set_bright;
    QScrollArea *scroll_area;
    Ui::MainWindow *ui;
    QWidget *pow_widget;
    QList<DeviceWidget *> device_items;
    QString releaseQss;
    QString pressQss;
    bool saving;
    bool healthing;
    QMap<DEVICE*,QListWidgetItem*> device_item_map;
    int panel_height;
    QDBusInterface *serviceInterface;
    int dev_number;
protected:
    bool event(QEvent *event);
//    void paintEvent(QPaintEvent *event);
private Q_SLOTS:


};

#endif // MAINWINDOW_H
