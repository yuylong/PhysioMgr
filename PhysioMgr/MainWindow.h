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

    void on_pbPhysioItemAdd_clicked();

    void on_pbPhysioItemUpd_clicked();

    void on_pbPhysioItemDel_clicked();

    void on_pbDoctorAdd_clicked();

    void on_pbDoctorRefrsh_clicked();

    void on_pbDoctorUpd_clicked();

    void on_pbDoctorDel_clicked();

    void on_pbPatientUdp_clicked();

    void on_pbPatientRefresh_clicked();

    void on_pbPatientAdd_clicked();

    void on_pbPatientDel_clicked();

    void on_pbHospiRefrsh_clicked();

private:
    Ui::MainWindow *ui;

    static const QString dbConfigFile;

    QString curDoctorListCond;
    void refreshDoctorList();
    QString curPatientListCond;
    void refreshPatientList();
    QString curHospiRecListCond;
    QDate curHospiRecStartDate, curHospiRecEndDate;
    void refreshHospiRecList();

    void refreshDepartmentList();
    void refreshPhysioItemList();

protected:
    virtual void showEvent(QShowEvent *event);
};

#endif // MAINWINDOW_H
