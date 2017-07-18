#ifndef PSMDLGPHYSIOLIST_H
#define PSMDLGPHYSIOLIST_H

#include <QDialog>
#include "PsmService.h"

namespace Ui {
class PsmDlgPhysioList;
}

class PsmDlgPhysioList : public QDialog
{
    Q_OBJECT

public:
    explicit PsmDlgPhysioList(QWidget *parent = 0);
    ~PsmDlgPhysioList();

    PsmService *getService() const;
    void setService(PsmService *service);

    const PsmSrvHospiPhysio &getHospiPhysio() const;
    PsmSrvHospiPhysio &getHospiPhysio();
    void setHospiPhysio(const PsmSrvHospiPhysio &hospiphysio);

    void refreshPhysioList();

private slots:
    void on_pbRefresh_clicked();

private:
    Ui::PsmDlgPhysioList *ui;
    PsmService *service;
    PsmSrvHospiPhysio hospiphysio;
};

#endif // PSMDLGPHYSIOLIST_H
