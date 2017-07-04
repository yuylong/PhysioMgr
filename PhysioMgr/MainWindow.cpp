/*-
 * GNU GENERAL PUBLIC LICENSE, version 3
 * See LICENSE file for detail.
 *
 * Author: Yulong Yu, Jun 29th, 2017
 * Copyright(c) 2015-2017 Yulong Yu. All rights reserved.
 */

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}



bool MainWindow::initService()
{
    static QString dbConfigFile("PsmDbConfig.xml");
    QString appdir = qGuiApp->applicationDirPath();
    QString configpath = appdir + "/" + dbConfigFile;

    this->service.setConfigFile(configpath);
    this->service.setParentWindow(this);

    bool ok = this->service.init();
    if (!ok) {
        qGuiApp->exit(-1);
        return false;
    }

    this->refreshDepartmentList();
    this->refreshPhysioItemList();
    return true;
}

void MainWindow::refreshDepartmentList()
{
    this->service.refreshDepartmentList(ui->lblDepartCnt, ui->tblDepartments);
}

void MainWindow::refreshPhysioItemList()
{
    this->service.refreshPhysioItemList(ui->lblPhysioItemCnt, ui->tblPhysioList);
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
}

void MainWindow::on_pbDepartRefrsh_clicked()
{
    this->refreshDepartmentList();
}

void MainWindow::on_pbPhysioItemRefrsh_clicked()
{
    this->refreshPhysioItemList();
}

void MainWindow::showEvent(QShowEvent *event)
{
}
