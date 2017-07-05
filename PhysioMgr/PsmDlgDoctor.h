#ifndef PSMDLGDOCTOR_H
#define PSMDLGDOCTOR_H

#include <QDialog>

namespace Ui {
class PsmDlgDoctor;
}

class PsmDlgDoctor : public QDialog
{
    Q_OBJECT

public:
    explicit PsmDlgDoctor(QWidget *parent = 0);
    ~PsmDlgDoctor();

private:
    Ui::PsmDlgDoctor *ui;
};

#endif // PSMDLGDOCTOR_H
