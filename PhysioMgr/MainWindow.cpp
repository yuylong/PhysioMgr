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

    this->initDatabase();
}

MainWindow::~MainWindow()
{
    this->database.stopDatabase();
    delete ui;
}

const QString MainWindow::dbConfigFile("PsmDbConfig.xml");

void MainWindow::initDatabase()
{
    QString appdir = qGuiApp->applicationDirPath();
    QString configpath = appdir + dbConfigFile;

    this->database.configConnectionFromFile(configpath);
    if ( !this->database.isConnectionConfigured() ) {
        QMessageBox::critical(this, "致命错误", "无法打开数据库配置文件，请联系开发人员解决！");
        qGuiApp->exit(-1);
        return;
    }

    this->database.startDatabase();
    if (!this->database.isDatabaseStarted()) {
        QMessageBox::critical(this, "致命错误", "数据库无法完成配置工作，请联系开发人员解决！");
        qGuiApp->exit(-1);
        return;
    }
}
