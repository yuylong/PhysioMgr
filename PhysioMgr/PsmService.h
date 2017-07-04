#ifndef PSMSERVICE_H
#define PSMSERVICE_H

#include <QString>
#include <QtSql>
#include <QLabel>
#include <QWidget>
#include <QTableWidget>
#include <QMessageBox>
#include <QMainWindow>
#include "PsmDatabase.h"

class PsmService
{
protected:
    QString configfile;
    PsmDatabase database;
    QMainWindow *parent;

public:
    PsmService();

    QString getConfigFile() const;
    void setConfigFile(const QString &configfile);
    QMainWindow *getParentWindow() const;
    void setParentWindow(QMainWindow *window);
    const PsmDatabase *getDatabase() const;
    PsmDatabase *getDatabase();

    bool init(QWidget *window = NULL);

    void refreshDepartmentList(QLabel *lbl, QTableWidget *tbl, QWidget *window = NULL);
    void refreshPhysioItemList(QLabel *lbl, QTableWidget *tbl, QWidget *window = NULL);
};

#endif // PSMSERVICE_H
