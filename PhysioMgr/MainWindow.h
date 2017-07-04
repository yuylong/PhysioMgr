/*-
 * GNU GENERAL PUBLIC LICENSE, version 3
 * See LICENSE file for detail.
 *
 * Author: Yulong Yu, Jun 29th, 2017
 * Copyright(c) 2015-2017 Yulong Yu. All rights reserved.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "PsmDatabase.h"
#include "PsmService.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    PsmService service;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool initService();

private slots:
    void on_tabWidget_currentChanged(int index);

    void on_pbDepartRefrsh_clicked();

    void on_pbPhysioItemRefrsh_clicked();

    void on_pbDepartAdd_clicked();

    void on_pbDepartUpd_clicked();

    void on_pbDepartDel_clicked();

private:
    Ui::MainWindow *ui;

    static const QString dbConfigFile;

    QList<int> departColMap, physioItemColMap;
    void refreshDepartmentList();
    void refreshPhysioItemList();

protected:
    virtual void showEvent(QShowEvent *event);
};

#endif // MAINWINDOW_H
