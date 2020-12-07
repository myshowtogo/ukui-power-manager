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
#include "device_form.h"
#include "ui_deviceform.h"
#include <QIcon>
#include <QDebug>
#include <QStyleOption>
#include <QPainter>
#include <QMouseEvent>
#include <QDir>
#include "mainwindow.h"

DeviceForm::DeviceForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeviceForm)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground,true);
    setWindowFlags(Qt::FramelessWindowHint);
    ed = EngineDevice::getInstance();

    ui->remaindata->setFixedHeight(11);
    QFont font = ui->remaindata->font();
    font.setPixelSize(11);
    ui->remaindata->setFont(font);
}

DeviceForm::~DeviceForm()
{
    if(charge_animation)
    {
       charge_animation->deleteLater();
       charge_animation = NULL;
    }
    delete ui;
}

void DeviceForm::set_timer(UpDeviceKind kind)
{
    charge_animation = new QTimer(this);

    statics_icon_pre = ed->engine_kind_to_string(kind);
    for(int i = 0; i<=24; i++)
    {
        QString filename = QString(":/charging/%1/%2.png").arg(statics_icon_pre).arg(i);
        QPixmap apix = QPixmap(filename);
        if(!apix.isNull())
        {
            animation_list.append(apix);
        }
    }
    connect(charge_animation,&QTimer::timeout,this,&DeviceForm::begin_charge_animation);

}

void DeviceForm::set_charge_animation(bool flag)
{
    if(flag)
    {
        if(!charge_animation->isActive())
        {
            charging_index = 0;
            charge_animation->start(200);
        }
    }
    else
    {
        if(charge_animation->isActive())
        {
            charge_animation->stop();
        }
    }
}

void DeviceForm::setIcon(QString name)
{
    if(animation_list.size()>1)
    {
        if(name.contains("charging"))
        {
            set_charge_animation(true);
            return;
        }
    }
    set_charge_animation(false);

    QString icon_str = QString(":/charging/%1/0.png").arg(statics_icon_pre);
    QIcon icon = QIcon(icon_str);
    if(icon.isNull())
    {
        icon = QIcon(":/charging/battery/0.png");
    }
    QPixmap pixmap = icon.pixmap(QSize(32,32));
    ui->icon->setPixmap(pixmap);
}

void DeviceForm::begin_charge_animation()
{
    if(charging_index < animation_list.size())
        ui->icon->setPixmap(animation_list.at(charging_index));
    charging_index ++;
    if(charging_index > 24)
        charging_index = 0;
}

void DeviceForm::setPercent(QString perct)
{
    ui->percentage->setText(perct);
}

void DeviceForm::setKind(QString kind)
{
    ui->kind->setText(kind);
}

void DeviceForm::setRemain(QString remain)
{
    ui->remaintext->setText("");
    ui->remaindata->setText(remain);
}

void DeviceForm::widget_property_change()
{
    setIcon(icon_name);
    setPercent(percentage);
    slider_changed(percent_number);
    setKind(kind);
    setRemain(predict);
}

void DeviceForm::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    p.setBrush(opt.palette.color(QPalette::Base));
    p.setPen(Qt::NoPen);
    p.setOpacity(0.0);
    p.setRenderHint(QPainter::Antialiasing);
    p.drawRoundedRect(rect(),6,6);
    QWidget::paintEvent(event);
}

 QString DeviceForm::calculate_value(int nValue,int nTotal)
 {
     QString strStyle = "";
     int value1 = nTotal * 0.1;
     if (nValue > value1)
     {

         if (nValue > (value1 * 2))
         {

             strStyle = QString("rgba(61,107,229,1)");

         }
         else
         {

             strStyle = QString("rgba(248,163,76,1)");

         }
     }
     else
     {
         strStyle = "rgba(240, 65, 52, 1)";
     }
     return strStyle;
 }

void DeviceForm::set_device(DEVICE *dev)
{
    if(dev == nullptr)
        return;
    path = dev->m_dev.path;
    /*prepare for device icon animation*/
    UpDeviceKind kind = dev->m_dev.kind;
    set_timer(kind);
    slot_device_change(dev);
    connect(ed,SIGNAL(signal_device_change(DEVICE*)),this,SLOT(slot_device_change(DEVICE*)));

}

void DeviceForm::slot_device_change(DEVICE* device)
{
    if(device == nullptr)
        return;
    if(device->m_dev.path != path)
        return;
    icon_name = ed->engine_get_device_icon(device);
    percentage = QString::number(device->m_dev.Percentage, 'f',0)+"%";
    percent_number = int (device->m_dev.Percentage);
    kind = ed->engine_kind_to_localised_text(device->m_dev.kind,0);
    predict = ed->engine_get_device_predict(device);
    mDev = device->m_dev;
    widget_property_change();
}

void DeviceForm::slider_changed(int value)
{
    QPalette palette;
    ui->progressBar->setValue(value);
    if (value <= 10) {
        palette= ui->progressBar->palette();
        palette.setBrush(QPalette::Highlight, QBrush(QColor(240, 65, 52, 255)));
        ui->progressBar->setPalette(palette);
    } else if (value <= 20){
        palette= ui->progressBar->palette();
        palette.setBrush(QPalette::Highlight, QBrush(QColor(248,163,76,255)));
        ui->progressBar->setPalette(palette);
    }
    else{
        palette= ui->progressBar->palette();
        palette.setBrush(QPalette::Highlight, QBrush(QColor(61,107,229,255)));
        ui->progressBar->setPalette(palette);
    }
}

QString DeviceForm::device_get_ac_online()
{
    QDir power_dir("/sys/class/power_supply/");
    if(!power_dir.exists())
        return QString();

    for(auto dir_name : power_dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
        auto enter_name = power_dir.absoluteFilePath(dir_name);
        QDir new_power_dir(enter_name);
        for(auto file : new_power_dir.entryList(QDir::Files))
        {
            if(file == "online")
            {
                QFile absolute_file(new_power_dir.absoluteFilePath(file));
                if(absolute_file.open(QIODevice::ReadOnly | QIODevice::Text))
                {
                    QByteArray ba = absolute_file.readAll();
                    absolute_file.close();
                    QString ac_online(ba);
                    ac_online = ac_online.simplified();
                    return ac_online;
                }
            }
        }
    }
    return QString();
}

void DeviceForm::device_adjust_battery_parameters()
{
    if(mDev.kind != UP_DEVICE_KIND_BATTERY)
        return;
    //fix state is full ,but percentage is not reach 100;
    if(mDev.State != UP_DEVICE_STATE_FULLY_CHARGED)
        return;
    QString online = device_get_ac_online();
    if((online == "1") && (percent_number<100))
    {
        mDev.State = UP_DEVICE_STATE_CHARGING;
        icon_name = ed->engine_get_dev_icon(mDev);
        predict = ed->engine_get_dev_predict(mDev);
    }

}
