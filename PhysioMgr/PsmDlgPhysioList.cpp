#include "PsmDlgPhysioList.h"
#include "ui_PsmDlgPhysioList.h"

PsmDlgPhysioList::PsmDlgPhysioList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PsmDlgPhysioList)
{
    ui->setupUi(this);
    this->service = NULL;
}

PsmDlgPhysioList::~PsmDlgPhysioList()
{
    delete ui;
}

PsmService *PsmDlgPhysioList::getService() const
{
    return this->service;
}

void PsmDlgPhysioList::setService(PsmService *service)
{
    this->service = service;
}

const PsmSrvHospiPhysio &PsmDlgPhysioList::getHospiPhysio() const
{
    return this->hospiphysio;
}

PsmSrvHospiPhysio &PsmDlgPhysioList::getHospiPhysio()
{
    return this->hospiphysio;
}

void PsmDlgPhysioList::setHospiPhysio(const PsmSrvHospiPhysio &hospiphysio)
{
    this->hospiphysio = hospiphysio;
    ui->leHospiRecId->setText(hospiphysio.hospirecid);
    ui->lePatientName->setText(hospiphysio.patientname);
    ui->lePhysioName->setText(hospiphysio.physioname);
    ui->leDate->setText(hospiphysio.startdate.toString(Qt::ISODate) + " - " +
                        hospiphysio.enddate.toString(Qt::ISODate));

    this->refreshPhysioList();
}

void PsmDlgPhysioList::refreshPhysioList()
{
    if (this->service == NULL || this->hospiphysio.hospirecid.isEmpty())
        return;

    this->service->listHospiPhysioLog(this->hospiphysio, ui->lblCount, ui->tableWidget, this);
}

void PsmDlgPhysioList::on_pbRefresh_clicked()
{
    this->refreshPhysioList();
}

void PsmDlgPhysioList::on_pbDelete_clicked()
{
    PsmSrvPhysioLog physiolog;
    bool ok = this->service->readSelectedPhysioLog(ui->tableWidget, &physiolog);
    if (!ok)
        return;

    QMessageBox::StandardButton answer;
    answer = QMessageBox::question(this, "删除确认",
                                   "确认要删除理疗记录（患者：" + physiolog.patientname +
                                   "，理疗项目：" + physiolog.physioname +
                                   "，操作时间：" + physiolog.optime.toString(Qt::ISODate) + "）？");
    if (answer != QMessageBox::Yes)
        return;

    this->service->deletePhysioLog(physiolog, this);
    this->refreshPhysioList();
}
