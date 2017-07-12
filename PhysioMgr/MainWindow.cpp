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
#include "PsmDlgDoctor.h"
#include "PsmDlgPatient.h"
#include "PsmDlgDepartment.h"
#include "PsmDlgPhysioItem.h"

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

void MainWindow::on_pbDepartAdd_clicked()
{
    PsmDlgDepartment dialog(this);
    dialog.exec();
    if (dialog.result() != QDialog::Accepted)
        return;

    PsmSrvDepartment dep;
    dep.id = dialog.getDepartID();
    dep.name = dialog.getDepartName();
    if (dep.id.isEmpty() || dep.name.isEmpty())
        return;

    this->service.insertNewDepartment(dep);
    this->refreshDepartmentList();
}

void MainWindow::on_pbDepartUpd_clicked()
{
    PsmSrvDepartment dep;
    bool ok = this->service.readSelectedDepartment(ui->tblDepartments, &dep);
    if (!ok)
        return;

    PsmDlgDepartment dialog(this);
    dialog.setDepartID(dep.id);
    dialog.setDepartName(dep.name);
    dialog.lockDepartID();

    dialog.exec();
    if (dialog.result() != QDialog::Accepted)
        return;

    dep.name = dialog.getDepartName();
    if (dep.name.isEmpty())
        return;

    this->service.updateDepartment(dep);
    this->refreshDepartmentList();
}

void MainWindow::on_pbDepartDel_clicked()
{
    PsmSrvDepartment dep;
    bool ok = this->service.readSelectedDepartment(ui->tblDepartments, &dep);
    if (!ok)
        return;

    QMessageBox::StandardButton answer;
    answer = QMessageBox::question(this, "删除确认", "确认要删除科室 " + dep.name + " (" + dep.id +")？");
    if (answer != QMessageBox::Yes)
        return;

    this->service.deleteDepartment(dep.id);
    this->refreshDepartmentList();
}

void MainWindow::on_pbPhysioItemAdd_clicked()
{
    PsmDlgPhysioItem dialog(this);
    dialog.exec();
    if (dialog.result() != QDialog::Accepted)
        return;

    PsmSrvPhysioItem physio;
    physio.id = dialog.getPhysioId();
    physio.name = dialog.getPhysioName();
    physio.price = dialog.getPhysioPrice();
    if (physio.id.isEmpty() || physio.name.isEmpty())
        return;

    this->service.insertNewPhysioItem(physio);
    this->refreshPhysioItemList();
}

void MainWindow::on_pbPhysioItemUpd_clicked()
{
    PsmSrvPhysioItem physio;
    bool ok = this->service.readSelectedPhysioItem(ui->tblPhysioList, &physio);
    if (!ok)
        return;

    PsmDlgPhysioItem dialog(this);
    dialog.setPhysioId(physio.id);
    dialog.setPhysioName(physio.name);
    dialog.setPhysioPrice(physio.price);
    dialog.lockPhysioId();

    dialog.exec();
    if (dialog.result() != QDialog::Accepted)
        return;

    physio.name = dialog.getPhysioName();
    physio.price = dialog.getPhysioPrice();
    if (physio.name.isEmpty())
        return;

    this->service.updatePhysioItem(physio);
    this->refreshPhysioItemList();
}

void MainWindow::on_pbPhysioItemDel_clicked()
{
    PsmSrvPhysioItem physio;
    bool ok = this->service.readSelectedPhysioItem(ui->tblPhysioList, &physio);
    if (!ok)
        return;

    QMessageBox::StandardButton answer;
    answer = QMessageBox::question(this, "删除确认",
                                   "确认要删除理疗项目 " + physio.name + " (" + physio.id +")？");
    if (answer != QMessageBox::Yes)
        return;

    this->service.deletePhysioItem(physio.id);
    this->refreshPhysioItemList();
}

void MainWindow::refreshDoctorList()
{
    if (this->curDoctorListCond.isNull())
        return;
    if (this->curDoctorListCond.isEmpty())
        this->service.refreshDoctorList(ui->lblDoctorCnt, ui->tblDoctors);
    else
        this->service.searchDoctor(this->curDoctorListCond, ui->lblDoctorCnt, ui->tblDoctors);

}

void MainWindow::on_pbDoctorRefrsh_clicked()
{
    if (ui->chbDoctorListAll->isChecked() || ui->leDoctorCond->text().isEmpty())
        this->curDoctorListCond = QString("");
    else
        this->curDoctorListCond = ui->leDoctorCond->text();

    this->refreshDoctorList();
}

void MainWindow::on_pbDoctorAdd_clicked()
{
    PsmDlgDoctor dialog;
    dialog.setService(&this->service);
    dialog.exec();
    if (dialog.result() != QDialog::Accepted)
        return;

    PsmSrvDoctor doctor;
    doctor.id = dialog.getDoctorId();
    doctor.name = dialog.getDoctorName();
    doctor.isNurse = dialog.getIsNurse();
    doctor.type = dialog.getType();
    doctor.depId = dialog.getDepartId();
    doctor.depName = dialog.getDepartName();
    doctor.phone = dialog.getPhoneNum();
    this->service.insertDoctor(doctor);
    this->refreshDoctorList();
}

void MainWindow::on_pbDoctorUpd_clicked()
{
    PsmSrvDoctor doctor;
    bool ok = this->service.readSelectedDoctor(ui->tblDoctors, &doctor);
    if (!ok)
        return;

    PsmDlgDoctor dialog;
    dialog.setService(&this->service);
    dialog.setDoctorId(doctor.id);
    dialog.setDoctorName(doctor.name);
    dialog.setIsNurse(doctor.isNurse);
    dialog.setType(doctor.type);
    dialog.setDepartId(doctor.depId);
    dialog.setDepartName(doctor.depName);
    dialog.setPhoneNum(doctor.phone);
    dialog.lockDoctorId();
    dialog.exec();
    if (dialog.result() != QDialog::Accepted)
        return;

    doctor.id = dialog.getDoctorId();
    doctor.name = dialog.getDoctorName();
    doctor.isNurse = dialog.getIsNurse();
    doctor.type = dialog.getType();
    doctor.depId = dialog.getDepartId();
    doctor.depName = dialog.getDepartName();
    doctor.phone = dialog.getPhoneNum();
    this->service.updateDoctor(doctor);
    this->refreshDoctorList();
}

void MainWindow::on_pbDoctorDel_clicked()
{
    PsmSrvDoctor doctor;
    bool ok = this->service.readSelectedDoctor(ui->tblDoctors, &doctor);
    if (!ok)
        return;

    QMessageBox::StandardButton answer;
    answer = QMessageBox::question(this, "删除确认",
                                   "确认要删除医护人员 " + doctor.name + " (编号：" + doctor.id +")？");
    if (answer != QMessageBox::Yes)
        return;

    this->service.deleteDoctor(doctor.id);
    this->refreshDoctorList();
}

void MainWindow::refreshPatientList()
{
    if (this->curPatientListCond.isEmpty())
        return;

    this->service.searchPatient(this->curPatientListCond, ui->lblPatientCnt, ui->tblPatients);
}

void MainWindow::on_pbPatientRefresh_clicked()
{
    if (ui->lePatientCond->text().isEmpty()) {
        QMessageBox::warning(this, "缺少必要信息", "请务必输入检索关键字。");
        return;
    }

    this->curPatientListCond = ui->lePatientCond->text();
    this->refreshPatientList();
}

void MainWindow::on_pbPatientAdd_clicked()
{
    PsmDlgPatient dialog;
    dialog.exec();
    if (dialog.result() != QDialog::Accepted)
        return;

    PsmSrvPatient patient;
    patient.id = dialog.getPatientId();
    patient.name = dialog.getPatientName();
    patient.dob = dialog.getDob();
    patient.phone = dialog.getPhoneNum();
    patient.address = dialog.getAddress();
    patient.comment = dialog.getComment();
    this->service.insertPatient(patient);

    this->refreshPatientList();
}

void MainWindow::on_pbPatientUdp_clicked()
{

}


