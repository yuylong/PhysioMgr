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

    QString logPhysioId;
    QString logNurseId;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool initService();

private slots:
    void on_pbDepartRefrsh_clicked();
    void on_pbDepartAdd_clicked();
    void on_pbDepartUpd_clicked();
    void on_pbDepartDel_clicked();

    void on_pbPhysioItemRefrsh_clicked();
    void on_pbPhysioItemAdd_clicked();
    void on_pbPhysioItemUpd_clicked();
    void on_pbPhysioItemDel_clicked();

    void on_pbDoctorRefrsh_clicked();
    void on_leDoctorCond_returnPressed();
    void on_pbDoctorAdd_clicked();
    void on_pbDoctorUpd_clicked();
    void on_pbDoctorDel_clicked();

    void on_pbPatientRefresh_clicked();
    void on_lePatientCond_returnPressed();
    void on_pbPatientAdd_clicked();
    void on_pbPatientUdp_clicked();
    void on_pbPatientDel_clicked();

    void on_pbHospiRefrsh_clicked();
    void on_leHospiRecCond_returnPressed();
    void on_pbPatientHospiAdd_clicked();
    void on_pbHospiUpd_clicked();
    void on_pbHospiDel_clicked();
    void on_pbHospiPhysio_clicked();

    void on_pbLogPhysioSel_clicked();
    void on_pbLogNurseSel_clicked();
    void on_pbPhysioLog_clicked();
    void on_leLogPatientID_returnPressed();

    void on_chbHospiStartHasDate_stateChanged(int arg1);
    void on_chbHospiRecHasStartDate_stateChanged(int arg1);
    void on_chbHospiRecHasEndDate_stateChanged(int arg1);
    void on_deHospiRecStartDate_userDateChanged(const QDate &date);
    void on_deHospiRecEndDate_userDateChanged(const QDate &date);

    void on_leDoctorCond_textChanged(const QString &arg1);

    void on_pbDBExport_clicked();
    void on_pbDBImport_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_pbPatientPsLog_clicked();

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

    void clearPhysioLogPatientId();
};

#endif // MAINWINDOW_H
