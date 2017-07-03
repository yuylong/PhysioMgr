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
    QString configpath = appdir + "/" + dbConfigFile;

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

void MainWindow::refreshDepartmentList()
{
    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.exec("SELECT * FROM departments;");
    if (!ok) {
        QSqlError sqlerr = query.lastError();
        QMessageBox::warning(this, "数据库错误",
                             "无法完成科室列表刷新。错误码：" + sqlerr.nativeErrorCode() + "\n" +
                             "数据库系统描述：" + sqlerr.text());
        return;
    }

    ui->tblDepartments->setRowCount(query.size());

    query.seek(-1);
    int linecnt = 0;
    QTableWidgetItem *item;
    while (query.next()) {
        item = ui->tblDepartments->item(linecnt, 0);
        if (item == NULL)
            item = new QTableWidgetItem();
        item->setText(query.value(0).toString());
        ui->tblDepartments->setItem(linecnt, 0, item);

        item = ui->tblDepartments->item(linecnt, 1);
        if (item == NULL)
            item = new QTableWidgetItem();
        item->setText(query.value(1).toString());
        ui->tblDepartments->setItem(linecnt, 1, item);

        qDebug() << query.value(0) << query.value(1);

        linecnt++;
    }
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if (ui->tabWidget->currentWidget() == ui->tabBasicInfo) {
        this->refreshDepartmentList();
    }
}
