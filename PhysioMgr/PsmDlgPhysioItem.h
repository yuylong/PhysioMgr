#ifndef PSMDLGPHYSIOITEM_H
#define PSMDLGPHYSIOITEM_H

#include <QDialog>

namespace Ui {
class PsmDlgPhysioItem;
}

class PsmDlgPhysioItem : public QDialog
{
    Q_OBJECT

public:
    explicit PsmDlgPhysioItem(QWidget *parent = 0);
    ~PsmDlgPhysioItem();

    QString getPhysioId() const;
    QString getPhysioName() const;
    double getPhysioPrice() const;

    void setPhysioId(const QString &id);
    void setPhysioName(const QString &name);
    void setPhysioPrice(double price);
    void lockPhysioId();
    void unlockPhysioId();

private:
    Ui::PsmDlgPhysioItem *ui;

protected:
    bool eventFilter(QObject *obj, QEvent *event);
private slots:
    void on_lePhysioId_returnPressed();
    void on_lePhysioName_returnPressed();
};

#endif // PSMDLGPHYSIOITEM_H
