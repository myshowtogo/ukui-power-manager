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
#include <QMenu>
#include <QScrollArea>
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

    bool want_percent;
    static double transparent;
    void initUi();
    void get_power_list();
    void initData();
    int get_engine_dev_number();
    QPixmap set_percent_pixmap(QString str);
    QIcon get_percent_icon(QIcon icon);

    int getTaskbarHeight(QString str);
    int getTaskbarPos(QString str);
    void set_window_position();
    void create_menu_item();
    QString get_window_style();
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
    void activate_power_statistic();
    void style_name_settings_cb(const QString &);
private:
    QSystemTrayIcon* trayIcon;
    QList<QDBusObjectPath> deviceNames;
    EngineDevice* ed;

    QLabel *percent_label;
    Ui::MainWindow *ui;

    QMenu *menu;

    QMap<DEVICE*,QListWidgetItem*> device_item_map;
    int dev_number;
    QGSettings *style_set = nullptr;
    QString style_nm;
    double get_window_opacity();
protected:
    bool event(QEvent *event);
    void paintEvent(QPaintEvent *event);
Q_SIGNALS:
    void style_modify(QString name);
};

#endif // MAINWINDOW_H
