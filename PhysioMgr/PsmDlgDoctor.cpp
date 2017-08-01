#include "PsmDlgDoctor.h"
#include "ui_PsmDlgDoctor.h"

#include <QKeyEvent>

#include "PsmDlgDepartSel.h"

PsmDlgDoctor::PsmDlgDoctor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PsmDlgDoctor)
{
    ui->setupUi(this);
    this->service = NULL;

    this->installEventFilter(this);
}

PsmDlgDoctor::~PsmDlgDoctor()
{
    delete ui;
}

bool PsmDlgDoctor::eventFilter(QObject *obj, QEvent *event)
{
    if ( obj == this ) {
        if ( event->type() == QEvent::KeyPress ) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if ( keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return )
                return true;
        }
    }

    return QDialog::eventFilter(obj, event);
}

PsmService *PsmDlgDoctor::getService() const
{
    return this->service;
}

void PsmDlgDoctor::setService(PsmService *service)
{
    this->service = service;
}

QString PsmDlgDoctor::getDoctorId() const
{
    return ui->leDoctorID->text();
}

QString PsmDlgDoctor::getDoctorName() const
{
    return ui->leDoctorName->text();
}

bool PsmDlgDoctor::getIsNurse() const
{
    return ui->cmbDoctorType->currentIndex() > 0;
}

QString PsmDlgDoctor::getType() const
{
    return ui->cmbDoctorType->currentText();
}

QString PsmDlgDoctor::getDepartId() const
{
    return this->departId;
}

QString PsmDlgDoctor::getDepartName() const
{
    if (this->departId.isEmpty())
        return QString();
    else
        return ui->pbDoctorDepart->text();
}

QString PsmDlgDoctor::getPhoneNum() const
{
    return ui->leDoctorPhone->text();
}

void PsmDlgDoctor::setDoctorId(const QString &id)
{
    ui->leDoctorID->setText(id);
}

void PsmDlgDoctor::setDoctorName(const QString &name)
{
    ui->leDoctorName->setText(name);
}

void PsmDlgDoctor::setIsNurse(bool isnurse)
{
    if (isnurse)
        ui->cmbDoctorType->setCurrentIndex(1);
    else
        ui->cmbDoctorType->setCurrentIndex(0);
}

void PsmDlgDoctor::setType(const QString &)
{
    return;
}

void PsmDlgDoctor::setDepartId(const QString &depid)
{
    this->departId = depid;
}

void PsmDlgDoctor::setDepartName(const QString &depname)
{
    ui->pbDoctorDepart->setText(depname);
}

void PsmDlgDoctor::setPhoneNum(const QString &phonenum)
{
    ui->leDoctorPhone->setText(phonenum);
}

void PsmDlgDoctor::lockDoctorId()
{
    ui->leDoctorID->setEnabled(false);
}

void PsmDlgDoctor::unlockDoctorId()
{
    ui->leDoctorID->setEnabled(true);
}

void PsmDlgDoctor::on_pbDoctorDepart_clicked()
{
    if (this->service == NULL)
        return;

    PsmDlgDepartSel dialog(this);
    dialog.setService(this->service);
    dialog.exec();
    if (dialog.result() != QDialog::Accepted)
        return;

    PsmSrvDepartment dep;
    bool ok = dialog.getSelectedDepart(&dep);
    if (!ok)
        return;

    this->departId = dep.id;
    ui->pbDoctorDepart->setText(dep.name);

    ui->leDoctorPhone->setFocus();
}

void PsmDlgDoctor::on_buttonBox_accepted()
{
    if (ui->leDoctorID->text().isEmpty() || ui->leDoctorName->text().isEmpty()) {
        QMessageBox::warning(this, "信息填写错误", "医生的编号和姓名必须填写。");
        return;
    }
    if (this->departId.isEmpty()) {
        QMessageBox::warning(this, "信息填写错误", "医生的科室信息必须填写。");
        return;
    }
    this->accept();
}

void PsmDlgDoctor::on_leDoctorID_returnPressed()
{
    ui->leDoctorName->setFocus();
}

void PsmDlgDoctor::on_leDoctorName_returnPressed()
{
    ui->cmbDoctorType->setFocus();
}

void PsmDlgDoctor::on_leDoctorPhone_returnPressed()
{
    emit (ui->buttonBox->accepted());
}
