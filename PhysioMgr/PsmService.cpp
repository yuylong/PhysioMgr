#include "PsmService.h"

#include <QList>
#include <QApplication>
#include <QtXml>

PsmService::PsmService() :
    configfile(), database()
{
    hospiRecIdLen = 8;
}

QString PsmService::getConfigFile() const
{
    return this->configfile;
}

void PsmService::setConfigFile(const QString &configfile)
{
    this->configfile = configfile;
}

QMainWindow *PsmService::getParentWindow() const
{
    return this->parent;
}

void PsmService::setParentWindow(QMainWindow *window)
{
    this->parent = window;
}

const PsmDatabase *PsmService::getDatabase() const
{
    return &this->database;
}

PsmDatabase *PsmService::getDatabase()
{
    return &this->database;
}

void PsmService::initParametersFromDom(const QDomElement &docelem)
{
    if (QString::compare(docelem.tagName(), "database") != 0)
        return;

    QDomNode domnode = docelem.firstChild();
    while (!domnode.isNull()) {
        QDomElement domelem = domnode.toElement();
        if (QString::compare(domelem.tagName(), "hospiRecIdLen") == 0) {
            this->hospiRecIdLen = domelem.text().toInt();
        } else if (QString::compare(domelem.tagName(), "machineId") == 0) {
            this->machineid = domelem.text();
        }

        domnode = domnode.nextSibling();
    }

}

bool PsmService::init(QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    if (this->configfile.isEmpty()) {
        QMessageBox::critical(window, "致命错误", "数据库配置文件未设置，请联系开发人员解决！");
        return false;
    }

    QFile file(this->configfile);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(window, "致命错误", "数据库配置文件无法打开，请联系开发人员解决！");
        return false;
    }

    QDomDocument domdoc("PsmDbConfig");
    if (!domdoc.setContent(&file)) {
        file.close();
        QMessageBox::critical(window, "致命错误", "数据库配置文件无法解读，请联系开发人员解决！");
        return false;
    }
    file.close();

    QDomElement docelem = domdoc.documentElement();
    this->database.configConnectionFromDom(docelem);
    if (!this->database.isConnectionConfigured()) {
        QMessageBox::critical(window, "致命错误", "配置文件无法成功配置数据库，请联系开发人员解决！");
        return false;
    }

    this->database.startDatabase();
    if (!this->database.isDatabaseStarted()) {
        QMessageBox::critical(window, "致命错误", "数据库无法启动运转，请联系开发人员解决！");
        return false;
    }

    this->initParametersFromDom(docelem);
    return true;
}

QString PsmService::getMachineId() const
{
    return this->machineid;
}

int PsmService::getTableSelectedRow(QTableWidget *tbl)
{
    QList<QTableWidgetSelectionRange> idxlist = tbl->selectedRanges();
    if (idxlist.count() < 1)
        return -1;

    const QTableWidgetSelectionRange &idx = idxlist.at(0);
    return idx.topRow();
}

QString PsmService::readTableSelectedId(QTableWidget *tbl, int idx)
{
    int rowidx = this->getTableSelectedRow(tbl);
    if (rowidx < 0)
        return QString();

    QTableWidgetItem *item = tbl->item(rowidx, idx);
    if (item == NULL)
        return QString();

    return item->text();
}

bool PsmService::readSelectedDepartment(QTableWidget *tbl, PsmSrvDepartment *dep)
{
    int rowidx = this->getTableSelectedRow(tbl);
    if (rowidx < 0)
        return false;

    QTableWidgetItem *itemid = tbl->item(rowidx, 0);
    QTableWidgetItem *itemname = tbl->item(rowidx, 1);
    if (itemid == NULL || itemname == NULL)
        return false;

    dep->id = itemid->text();
    dep->name = itemname->text();
    return true;
}

QString PsmService::readSelectedDepartmentId(QTableWidget *tbl)
{
    return this->readTableSelectedId(tbl, 0);
}

void PsmService::refreshDepartmentList(QLabel *lbl, QTableWidget *tbl, QWidget *window)
{
    static QList<int> colmap;
    if (colmap.isEmpty())
        colmap << 0 << 1;

    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.exec("SELECT * FROM departments;");
    if (!ok) {
        QSqlError sqlerr = query.lastError();
        QString exterrstr;
        if (sqlerr.isValid()) {
            exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                        "数据库系统描述：" + sqlerr.text();
        }
        QMessageBox::warning(window, "数据库错误", "无法完成科室列表刷新。" + exterrstr);
        return;
    }

    lbl->setText(QString::number(query.size()));
    this->database.fillTableWidget(tbl, &query, colmap);
}

void PsmService::searchDepartment(const QString &srchstr, QLabel *lbl, QTableWidget *tbl, QWidget *window)
{
    static QList<int> colmap;
    if (colmap.isEmpty())
        colmap << 0 << 1;

    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("SELECT * FROM departments "
                       "WHERE UPPER(id) LIKE UPPER(:srchstr) OR name LIKE :srchstr;");
    if (!ok)
        goto bad;

    query.bindValue(":srchstr", "%" + srchstr + "%");
    ok = query.exec();
    if (!ok)
        goto bad;

    lbl->setText(QString::number(query.size()));
    this->database.fillTableWidget(tbl, &query, colmap);
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法完成科室信息检索。" + exterrstr);
    return;
}

void PsmService::insertNewDepartment(const PsmSrvDepartment &dep, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("INSERT INTO departments VALUES(?, ?);");
    if (!ok)
        goto bad;

    query.bindValue(0, dep.id);
    query.bindValue(1, dep.name);
    ok = query.exec();
    if (!ok)
        goto bad;
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法添加新的科室。" + exterrstr);
    return;
}

void PsmService::updateDepartment(const PsmSrvDepartment &dep, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("UPDATE departments SET name=? WHERE id=?;");
    if (!ok)
        goto bad;

    query.bindValue(0, dep.name);
    query.bindValue(1, dep.id);
    ok = query.exec();
    if (!ok)
        goto bad;
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法完成科室信息更新。" + exterrstr);
    return;
}

void PsmService::deleteDepartment(const QString &depid, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("DELETE FROM departments WHERE id=?;");
    if (!ok)
        goto bad;

    query.bindValue(0, depid);
    ok = query.exec();
    if (!ok)
        goto bad;
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法删除选定科室。" + exterrstr);
    return;
}

bool PsmService::readSelectedPhysioItem(QTableWidget *tbl, PsmSrvPhysioItem *physio)
{
    int rowidx = this->getTableSelectedRow(tbl);
    if (rowidx < 0)
        return false;

    QTableWidgetItem *itemid = tbl->item(rowidx, 0);
    QTableWidgetItem *itemname = tbl->item(rowidx, 1);
    QTableWidgetItem *itemprice = tbl->item(rowidx, 2);
    if (itemid == NULL || itemname == NULL)
        return false;

    physio->id = itemid->text();
    physio->name = itemname->text();
    physio->price = 1.0;
    if (itemprice != NULL)
        physio->price = itemprice->text().toDouble();
    return true;
}

QString PsmService::readSelectedPhysioItemId(QTableWidget *tbl)
{
    return this->readTableSelectedId(tbl, 0);
}

void PsmService::refreshPhysioItemList(QLabel *lbl, QTableWidget *tbl, QWidget *window)
{
    static QList<int> colmap;
    if (colmap.isEmpty())
        colmap << 0 << 1 << 2;

    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.exec("SELECT * FROM physio_items;");
    if (!ok) {
        QSqlError sqlerr = query.lastError();
        QString exterrstr;
        if (sqlerr.isValid()) {
            exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                        "数据库系统描述：" + sqlerr.text();
        }
        QMessageBox::warning(window, "数据库错误", "无法完成理疗项目列表刷新。" + exterrstr);
        return;
    }

    lbl->setText(QString::number(query.size()));
    this->database.fillTableWidget(tbl, &query, colmap);
}

void PsmService::searchPhysioItem(const QString &srchstr, QLabel *lbl, QTableWidget *tbl, QWidget *window)
{
    static QList<int> colmap;
    if (colmap.isEmpty())
        colmap << 0 << 1 << 2;

    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("SELECT * FROM physio_items "
                       "WHERE UPPER(id) LIKE UPPER(:srchstr) OR name LIKE :srchstr;");
    if (!ok)
        goto bad;

    query.bindValue(":srchstr", "%" + srchstr + "%");
    ok = query.exec();
    if (!ok)
        goto bad;

    lbl->setText(QString::number(query.size()));
    this->database.fillTableWidget(tbl, &query, colmap);
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法完成理疗项目检索。" + exterrstr);
    return;
}

void PsmService::insertNewPhysioItem(const PsmSrvPhysioItem &physio, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("INSERT INTO physio_items VALUES(?, ?, ?);");
    if (!ok)
        goto bad;

    query.bindValue(0, physio.id);
    query.bindValue(1, physio.name);
    query.bindValue(2, physio.price);
    ok = query.exec();
    if (!ok)
        goto bad;
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法添加新的理疗项目。" + exterrstr);
    return;
}

void PsmService::updatePhysioItem(const PsmSrvPhysioItem &physio, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("UPDATE physio_items SET name=?, price=? WHERE id=?;");
    if (!ok)
        goto bad;

    query.bindValue(0, physio.name);
    query.bindValue(1, physio.price);
    query.bindValue(2, physio.id);
    ok = query.exec();
    if (!ok)
        goto bad;
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法完成理疗项目信息更新。" + exterrstr);
    return;
}

void PsmService::deletePhysioItem(const QString &physioid, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("DELETE FROM physio_items WHERE id=?;");
    if (!ok)
        goto bad;

    query.bindValue(0, physioid);
    ok = query.exec();
    if (!ok)
        goto bad;
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法删除选定理疗项目。" + exterrstr);
    return;
}

bool PsmService::readSelectedDoctor(QTableWidget *tbl, PsmSrvDoctor *doctor)
{
    int rowidx = this->getTableSelectedRow(tbl);
    if (rowidx < 0)
        return false;

    QTableWidgetItem *itemid = tbl->item(rowidx, 0);
    QTableWidgetItem *itemname = tbl->item(rowidx, 1);
    QTableWidgetItem *itemtype = tbl->item(rowidx, 2);
    QTableWidgetItem *itemdep = tbl->item(rowidx, 3);
    QTableWidgetItem *itemphone = tbl->item(rowidx, 4);
    if (itemid == NULL || itemname == NULL)
        return false;

    doctor->id = itemid->text();
    doctor->name = itemname->text();
    doctor->isNurse = itemtype->data(Qt::UserRole).toBool();
    doctor->type = itemtype->text();
    doctor->depId = itemdep->data(Qt::UserRole).toString();
    doctor->depName = itemdep->text();
    doctor->phone = itemphone->text();
    return true;
}

QString PsmService::readSelectedDoctorId(QTableWidget *tbl)
{
    return this->readTableSelectedId(tbl, 0);
}

void PsmService::refreshDoctorList(QLabel *lbl, QTableWidget *tbl, QWidget *window)
{
    static QList<int> colmap, datamap;
    if (colmap.isEmpty())
        colmap << 0 << 1 << 3 << 5 << 6;
    if (datamap.isEmpty())
        datamap << -1 << -1 << 2 << 4 << -1;

    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.exec("SELECT * FROM doctors;");
    if (!ok) {
        QSqlError sqlerr = query.lastError();
        QString exterrstr;
        if (sqlerr.isValid()) {
            exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                        "数据库系统描述：" + sqlerr.text();
        }
        QMessageBox::warning(window, "数据库错误", "无法完成医护人员列表刷新。" + exterrstr);
        return;
    }

    lbl->setText(QString::number(query.size()));
    this->database.fillTableWidget(tbl, &query, colmap, datamap);
}

void PsmService::searchDoctor(const QString &srchstr, QLabel *lbl, QTableWidget *tbl, QWidget *window)
{
    static QList<int> colmap, datamap;
    if (colmap.isEmpty())
        colmap << 0 << 1 << 3 << 5 << 6;
    if (datamap.isEmpty())
        datamap << -1 << -1 << 2 << 4 << -1;

    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("SELECT * FROM doctors "
                       "WHERE UPPER(id) LIKE UPPER(:srchstr) OR name LIKE :srchstr OR "
                             "depname LIKE :srchstr OR phonenum LIKE :srchstr;");
    if (!ok)
        goto bad;

    query.bindValue(":srchstr", "%" + srchstr + "%");
    ok = query.exec();
    if (!ok)
        goto bad;

    lbl->setText(QString::number(query.size()));
    this->database.fillTableWidget(tbl, &query, colmap, datamap);
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法完成医护人员检索。" + exterrstr);
    return;
}

void PsmService::insertDoctor(const PsmSrvDoctor &doctor, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("INSERT INTO doctors VALUES(?, ?, ?, ?, ?, ?, ?);");
    if (!ok)
        goto bad;

    query.bindValue(0, doctor.id);
    query.bindValue(1, doctor.name);
    query.bindValue(2, doctor.isNurse);
    query.bindValue(3, doctor.type);
    query.bindValue(4, doctor.depId);
    query.bindValue(5, doctor.depName);
    query.bindValue(6, doctor.phone);
    ok = query.exec();
    if (!ok)
        goto bad;
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法添加新的医护人员。" + exterrstr);
    return;
}

void PsmService::updateDoctor(const PsmSrvDoctor &doctor, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("UPDATE doctors SET name=?, isnurse=?, type=?, "
                                          "depid=?, depname=?, phonenum=? WHERE id=?;");
    if (!ok)
        goto bad;

    query.bindValue(0, doctor.name);
    query.bindValue(1, doctor.isNurse);
    query.bindValue(2, doctor.type);
    query.bindValue(3, doctor.depId);
    query.bindValue(4, doctor.depName);
    query.bindValue(5, doctor.phone);
    query.bindValue(6, doctor.id);
    ok = query.exec();
    if (!ok)
        goto bad;
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法完成医护人员信息更新。" + exterrstr);
    return;
}

void PsmService::deleteDoctor(const QString &doctorid, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("DELETE FROM doctors WHERE id=?;");
    if (!ok)
        goto bad;

    query.bindValue(0, doctorid);
    ok = query.exec();
    if (!ok)
        goto bad;
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法删除选定医护人员。" + exterrstr);
    return;
}

bool PsmService::readSelectedPatient(QTableWidget *tbl, PsmSrvPatient *patient)
{
    int rowidx = this->getTableSelectedRow(tbl);
    if (rowidx < 0)
        return false;

    QTableWidgetItem *itemid = tbl->item(rowidx, 0);
    QTableWidgetItem *itemname = tbl->item(rowidx, 1);
    QTableWidgetItem *itemdob = tbl->item(rowidx, 2);
    QTableWidgetItem *itemphone = tbl->item(rowidx, 3);
    QTableWidgetItem *itemaddr = tbl->item(rowidx, 4);
    QTableWidgetItem *itemcomment = tbl->item(rowidx, 5);
    if (itemid == NULL || itemname == NULL)
        return false;

    patient->id = itemid->text();
    patient->name = itemname->text();
    patient->dob = itemdob->data(Qt::UserRole).toDate();
    patient->phone = itemphone->text();
    patient->address = itemaddr->text();
    patient->comment = itemcomment->text();
    return true;
}

QString PsmService::readSelectedPatientId(QTableWidget *tbl)
{
    return this->readTableSelectedId(tbl, 0);
}

void PsmService::refreshPatientList(QLabel *lbl, QTableWidget *tbl, QWidget *window)
{
    static QList<int> colmap, datamap;
    if (colmap.isEmpty())
        colmap << 0 << 1 << 2 << 3 << 4 << 5;
    if (datamap.isEmpty())
        datamap << -1 << -1 << 2 << -1 << -1 << -1;

    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.exec("SELECT * FROM patients;");
    if (!ok) {
        QSqlError sqlerr = query.lastError();
        QString exterrstr;
        if (sqlerr.isValid()) {
            exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                        "数据库系统描述：" + sqlerr.text();
        }
        QMessageBox::warning(window, "数据库错误", "无法完成患者列表刷新。" + exterrstr);
        return;
    }

    lbl->setText(QString::number(query.size()));
    this->database.fillTableWidget(tbl, &query, colmap, datamap);

}

void PsmService::searchPatient(const QString &srchstr, QLabel *lbl, QTableWidget *tbl, QWidget *window)
{
    static QList<int> colmap, datamap;
    if (colmap.isEmpty())
        colmap << 0 << 1 << 2 << 3 << 4 << 5;
    if (datamap.isEmpty())
        datamap << -1 << -1 << 2 << -1 << -1 << -1;

    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("SELECT * FROM patients "
                       "WHERE UPPER(id) = UPPER(:instr) OR name LIKE :srchstr OR phonenum = :instr;");
    if (!ok)
        goto bad;

    query.bindValue(":instr", srchstr);
    query.bindValue(":srchstr", "%" + srchstr + "%");
    ok = query.exec();
    if (!ok)
        goto bad;

    lbl->setText(QString::number(query.size()));
    this->database.fillTableWidget(tbl, &query, colmap, datamap);
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法完成患者信息检索。" + exterrstr);
    return;
}

int PsmService::getPatientCount(const QString &srchstr)
{
    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("SELECT COUNT(*) FROM patients "
                       "WHERE UPPER(id) = UPPER(:instr) OR name LIKE :srchstr OR phonenum = :instr;");
    if (!ok)
        return 0;

    query.bindValue(":instr", srchstr);
    query.bindValue(":srchstr", "%" + srchstr + "%");
    ok = query.exec();
    if (!ok)
        return 0;

    ok = query.first();
    if (!ok)
        return 0;

    QSqlRecord rec = query.record();
    return rec.value(0).toInt();
}

bool PsmService::getFirstPatient(const QString &srchstr, PsmSrvPatient *patient, QWidget *window)
{
    if (patient == NULL)
        return false;
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    QSqlRecord rec;
    ok = query.prepare("SELECT * FROM patients "
                       "WHERE UPPER(id) = UPPER(:instr) OR name LIKE :srchstr OR phonenum = :instr;");
    if (!ok)
        goto bad;

    query.bindValue(":instr", srchstr);
    query.bindValue(":srchstr", "%" + srchstr + "%");
    ok = query.exec();
    if (!ok)
        goto bad;

    if (query.size() < 1) {
        QMessageBox::warning(window, "信息未找到", "所输入的检索条件下无对应患者信息。");
        return false;
    }

    ok = query.first();
    if (!ok)
        goto bad;

    rec = query.record();
    patient->id = rec.value(0).toString();
    patient->name = rec.value(1).toString();
    patient->dob = rec.value(2).toDate();
    patient->phone = rec.value(3).toString();
    patient->address = rec.value(4).toString();
    patient->comment = rec.value(5).toString();
    return true;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法完成患者信息检索。" + exterrstr);
    return false;
}

void PsmService::insertPatient(const PsmSrvPatient &patient, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("INSERT INTO patients VALUES(?, ?, ?, ?, ?, ?);");
    if (!ok)
        goto bad;

    query.bindValue(0, patient.id);
    query.bindValue(1, patient.name);
    query.bindValue(2, patient.dob);
    query.bindValue(3, patient.phone);
    query.bindValue(4, patient.address);
    query.bindValue(5, patient.comment);
    ok = query.exec();
    if (!ok)
        goto bad;
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法添加新的患者信息。" + exterrstr);
    return;
}

void PsmService::updatePatient(const PsmSrvPatient &patient, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("UPDATE patients SET name=?, dob=?, phonenum=?, "
                                           "address=?, comment=? WHERE id=?;");
    if (!ok)
        goto bad;

    query.bindValue(0, patient.name);
    query.bindValue(1, patient.dob);
    query.bindValue(2, patient.phone);
    query.bindValue(3, patient.address);
    query.bindValue(4, patient.comment);
    query.bindValue(5, patient.id);
    ok = query.exec();
    if (!ok)
        goto bad;
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法完成患者信息更新。" + exterrstr);
    return;
}

void PsmService::deletePatient(const QString &patientid, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("DELETE FROM patients WHERE id=?;");
    if (!ok)
        goto bad;

    query.bindValue(0, patientid);
    ok = query.exec();
    if (!ok)
        goto bad;
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法删除选定患者。" + exterrstr);
    return;
}

bool PsmService::readSelectedHospiRec(QTableWidget *tbl, PsmSrvHospiRec *hospirec)
{
    int rowidx = this->getTableSelectedRow(tbl);
    if (rowidx < 0)
        return false;

    QTableWidgetItem *itemid = tbl->item(rowidx, 0);
    QTableWidgetItem *itempatient = tbl->item(rowidx, 1);
    QTableWidgetItem *itemdep = tbl->item(rowidx, 2);
    QTableWidgetItem *itemroomid = tbl->item(rowidx, 3);
    QTableWidgetItem *itemdisease = tbl->item(rowidx, 4);
    QTableWidgetItem *itemdoctor = tbl->item(rowidx, 5);
    QTableWidgetItem *itemnurse = tbl->item(rowidx, 6);
    QTableWidgetItem *itemstart = tbl->item(rowidx, 7);
    QTableWidgetItem *itemend = tbl->item(rowidx, 8);
    if (itemid == NULL || itempatient == NULL)
        return false;

    hospirec->id = itemid->text();
    hospirec->patientid = itempatient->data(Qt::UserRole).toString();
    hospirec->patientname = itempatient->text();
    hospirec->depid = itemdep->data(Qt::UserRole).toString();
    hospirec->depname = itemdep->text();
    hospirec->roomid = itemroomid->text();
    hospirec->disease = itemdisease->text();
    hospirec->doctorid = itemdoctor->data(Qt::UserRole).toString();
    hospirec->doctorname = itemdoctor->text();
    hospirec->nurseid = itemnurse->data(Qt::UserRole).toString();
    hospirec->nursename = itemnurse->text();
    hospirec->startdate = itemstart->data(Qt::UserRole).toDate();
    hospirec->enddate = itemend->data(Qt::UserRole).toDate();
    return true;
}

QString PsmService::readSelectedHospiRecId(QTableWidget *tbl)
{
    return this->readTableSelectedId(tbl, 0);
}

bool PsmService::readOneHospiRec(const QString &hospirecid, PsmSrvHospiRec *hospirec)
{
    if (hospirecid.isEmpty() || hospirec == NULL)
        return false;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("SELECT * FROM hospi_records WHERE UPPER(id) = UPPER(?);");
    if (!ok)
        return false;

    query.bindValue(0, hospirecid);
    ok = query.exec();
    if (!ok)
        return false;

    if (query.size() < 1)
        return false;

    ok = query.first();
    if (!ok)
        return false;

    QSqlRecord rec = query.record();
    hospirec->id = rec.value(0).toString();
    hospirec->patientid = rec.value(1).toString();
    hospirec->patientname = rec.value(2).toString();
    hospirec->depid = rec.value(3).toString();
    hospirec->depname = rec.value(4).toString();
    hospirec->roomid = rec.value(5).toString();
    hospirec->disease = rec.value(6).toString();
    hospirec->doctorid = rec.value(7).toString();
    hospirec->doctorname = rec.value(8).toString();
    hospirec->nurseid = rec.value(9).toString();
    hospirec->nursename = rec.value(10).toString();
    hospirec->startdate = rec.value(11).toDate();
    hospirec->enddate = rec.value(12).toDate();
    return true;
}

void PsmService::searchHospiRec(const QString &srchstr, const QDate &startdate, const QDate &enddate,
                                QLabel *lbl, QTableWidget *tbl, QWidget *window)
{
    static QList<int> colmap, datamap;
    if (colmap.isEmpty())
        colmap << 0 << 2 << 4 << 5 << 6 << 8 << 10 << 11 << 12;
    if (datamap.isEmpty())
        datamap << -1 << 1 << 3 << -1 << -1 << 7 << 9 << 11 << 12;

    if (window == NULL)
        window = this->parent;

    QString dqstr = "";
    if (startdate.isValid())
        dqstr += " AND enddate >= :sdate";
    if (enddate.isValid())
        dqstr += " AND startdate <= :edate";

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("SELECT * FROM hospi_records "
                       "WHERE (UPPER(id) = UPPER(:instr) OR UPPER(pati_id) = UPPER(:instr) OR "
                              "pati_name LIKE :srchstr OR UPPER(room_id) = UPPER(:instr))" + dqstr + ";");
    if (!ok)
        goto bad;

    query.bindValue(":instr", srchstr);
    query.bindValue(":srchstr", "%" + srchstr + "%");
    if (startdate.isValid())
        query.bindValue(":sdate", startdate);
    if (enddate.isValid())
        query.bindValue(":edate", enddate);
    ok = query.exec();
    if (!ok)
        goto bad;

    lbl->setText(QString::number(query.size()));
    this->database.fillTableWidget(tbl, &query, colmap, datamap);
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法完成住院信息检索。" + exterrstr);
    return;
}

QString PsmService::getCurrentMaxHospiRecId()
{
    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("SELECT MAX(id) FROM hospi_records WHERE LENGTH(id) = ?;");
    if (!ok)
        return QString();

    query.bindValue(0, this->hospiRecIdLen);
    ok = query.exec();
    if (!ok)
        return QString();

    if (query.size() < 1)
        return QString();

    ok = query.first();
    if (!ok)
        return QString();

    QSqlRecord rec = query.record();
    QString curid = rec.value(0).toString();
    return curid;
}

bool PsmService::checkHospiRecId(const QString &curid, bool *hasletter)
{
    bool _hasletter = false;

    for (int i = 0; i < curid.length(); i++) {
        QChar ch = curid.at(i);
        if (!ch.isDigit() && !ch.isUpper())
            return false;
        if (ch == 'I' || ch == 'O')
            return false;

        if (ch.isLetter())
            _hasletter = true;
    }
    if (hasletter != NULL)
        *hasletter = _hasletter;
    return true;
}

bool PsmService::needWarningForHospiRecId(const QString &curid, bool hasletter)
{
    QChar warnletter;
    int ignorecnt;
    if (hasletter) {
        warnletter = 'Z';
        ignorecnt = 2;
    } else {
        warnletter = '9';
        ignorecnt = 2;
    }

    for (int i = 0; i < curid.length() - ignorecnt; i++) {
        if (curid.at(i) != warnletter)
            return false;
    }
    return true;
}

QString PsmService::getNextHospiRecId(const QString &curid, bool *hasletter)
{
    bool _hasletter = false;
    bool ok = this->checkHospiRecId(curid, &_hasletter);
    if (!ok)
        return QString();

    QString nextid = curid;
    for (int i = nextid.length() - 1; i >= 0; i--) {
        char ch = nextid.at(i).toLatin1();
        if ((ch >= '0' && ch < '9') || (ch >= 'A' && ch < 'Z') ) {
            if (ch == 'I' - 1 || ch == 'O' - 1)
                nextid.replace(i, 1, QChar::fromLatin1(ch + 2));
            else
                nextid.replace(i, 1, QChar::fromLatin1(ch + 1));
            break;
        } else if (ch == '9' && !_hasletter && i != 0) {
            nextid.replace(i, 1, QChar::fromLatin1('0'));
            continue;
        } else if (ch == '9') {
            nextid.replace(i, 1, QChar::fromLatin1('A'));
            break;
        } else if (ch == 'Z' && i != 0) {
            nextid.replace(i, 1, QChar::fromLatin1('0'));
            continue;
        } else {
            return QString();
        }
    }
    if (hasletter != NULL)
        *hasletter = _hasletter;
    return nextid;
}

QString PsmService::getCurrentNextHostpiRecId(QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    QString curid = getCurrentMaxHospiRecId();
    if (curid.isEmpty())
        return QString("0").repeated(this->hospiRecIdLen - 1) + "1";

    bool hasletter = false;
    QString nextid = getNextHospiRecId(curid, &hasletter);
    if (nextid.isEmpty()) {
        QMessageBox::critical(window, "系统错误", "数据库无法继续生成住院号，请联系开发人员解决。");
        return QString();
    }

    bool needwarn = this->needWarningForHospiRecId(nextid, hasletter);
    if (needwarn) {
        if (hasletter) {
            QMessageBox::warning(window, "系统告警", "可用住院号已接近枯竭，请联系开发人员扩充号码池。");
        } else {
            QMessageBox::warning(window, "系统告警", "纯数字住院号已快用尽，用尽后将引入字母编号。\n"
                                         "不希望使用字母编号，请联系开发人员扩充号码池。");
        }
    }
    return nextid;
}

void PsmService::insertHospiRec(const PsmSrvHospiRec &hospirec, QString *hospirecid, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    if (!hospirec.id.isEmpty())
        QMessageBox::warning(window, "警告", "住院号需自动生成，预置号码无效！");

    bool ok;
    QString nextid;
    QSqlQuery query = this->database.getQuery();
    ok = query.exec("LOCK TABLES hospi_records WRITE;");
    if (!ok)
        goto bad_nolock;

    nextid = this->getCurrentNextHostpiRecId(window);
    if (nextid.isEmpty())
        goto bad;

    query = this->database.getQuery();
    ok = query.prepare("INSERT INTO hospi_records VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
    if (!ok)
        goto bad;

    query.bindValue(0, nextid);
    query.bindValue(1, hospirec.patientid);
    query.bindValue(2, hospirec.patientname);
    query.bindValue(3, hospirec.depid);
    query.bindValue(4, hospirec.depname);
    query.bindValue(5, hospirec.roomid);
    query.bindValue(6, hospirec.disease);
    query.bindValue(7, hospirec.doctorid);
    query.bindValue(8, hospirec.doctorname);
    query.bindValue(9, hospirec.nurseid);
    query.bindValue(10, hospirec.nursename);
    query.bindValue(11, hospirec.startdate);
    query.bindValue(12, hospirec.enddate);
    ok = query.exec();
    if (!ok)
        goto bad;

    query = this->database.getQuery();
    query.exec("UNLOCK TABLES;");

    if (hospirecid != NULL)
        *hospirecid = nextid;
    return;

bad:
    query = this->database.getQuery();
    query.exec("UNLOCK TABLES;");
bad_nolock:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法添加新的住院信息。" + exterrstr);
    return;
}

void PsmService::updateHospiRec(const PsmSrvHospiRec &hospirec, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("UPDATE hospi_records "
                       "SET pati_id=?, pati_name=?, dep_id=?, dep_name=?, room_id=?, disease=?, "
                           "doct_id=?, doct_name=?, nurse_id=?, nurse_name=?, startdate=?, enddate=? "
                       "WHERE id=?;");
    if (!ok)
        goto bad;

    query.bindValue(0, hospirec.patientid);
    query.bindValue(1, hospirec.patientname);
    query.bindValue(2, hospirec.depid);
    query.bindValue(3, hospirec.depname);
    query.bindValue(4, hospirec.roomid);
    query.bindValue(5, hospirec.disease);
    query.bindValue(6, hospirec.doctorid);
    query.bindValue(7, hospirec.doctorname);
    query.bindValue(8, hospirec.nurseid);
    query.bindValue(9, hospirec.nursename);
    query.bindValue(10, hospirec.startdate);
    query.bindValue(11, hospirec.enddate);
    query.bindValue(12, hospirec.id);
    ok = query.exec();
    if (!ok)
        goto bad;
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法完成住院信息更新。" + exterrstr);
    return;
}

void PsmService::deleteHospiRec(const QString &hospirecid, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("DELETE FROM hospi_records WHERE id=?;");
    if (!ok)
        goto bad;

    query.bindValue(0, hospirecid);
    ok = query.exec();
    if (!ok)
        goto bad;
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法删除选定住院信息。" + exterrstr);
    return;
}

bool PsmService::readSelectedHospiPhysio(QTableWidget *tbl, PsmSrvHospiPhysio *hospiphysio)
{
    int rowidx = this->getTableSelectedRow(tbl);
    if (rowidx < 0)
        return false;

    QTableWidgetItem *itemhospirecid = tbl->item(rowidx, 0);
    QTableWidgetItem *itempatient = tbl->item(rowidx, 1);
    QTableWidgetItem *itemphysio = tbl->item(rowidx, 2);
    QTableWidgetItem *itemfreqperiod = tbl->item(rowidx, 3);
    QTableWidgetItem *itemfreqcount = tbl->item(rowidx, 4);
    QTableWidgetItem *itemstartdate = tbl->item(rowidx, 5);
    QTableWidgetItem *itemenddate = tbl->item(rowidx, 6);
    if (itemhospirecid == NULL)
        return false;

    hospiphysio->hospirecid = itemhospirecid->text();
    hospiphysio->patientid = itempatient->data(Qt::UserRole).toString();
    hospiphysio->patientname = itempatient->text();
    hospiphysio->physioid = itemphysio->data(Qt::UserRole).toString();
    hospiphysio->physioname = itemphysio->text();
    hospiphysio->freqperiod = itemfreqperiod->data(Qt::UserRole).toInt();
    hospiphysio->freqcount = itemfreqcount->data(Qt::UserRole).toInt();
    hospiphysio->startdate = itemstartdate->data(Qt::UserRole).toDate();
    hospiphysio->enddate = itemenddate->data(Qt::UserRole).toDate();
    return true;
}

void PsmService::listHospiPhysio(const QString &hospirecid, QLabel *lbl, QTableWidget *tbl, QWidget *window)
{
    static QList<int> colmap, datamap;
    if (colmap.isEmpty())
        colmap << 0 << 2 << 4 << 5 << 6 << 7 << 8;
    if (datamap.isEmpty())
        datamap << -1 << 1 << 3 << 5 << 6 << 7 << 8;

    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("SELECT * FROM hospi_physio WHERE hospi_id = ?;");
    if (!ok)
        goto bad;

    query.bindValue(0, hospirecid);
    ok = query.exec();
    if (!ok)
        goto bad;

    lbl->setText(QString::number(query.size()));
    this->database.fillTableWidget(tbl, &query, colmap, datamap);
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法完成住院理疗项目信息检索。" + exterrstr);
    return;
}

void PsmService::insertHospiPhysio(const PsmSrvHospiPhysio &hospiphysio, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("INSERT INTO hospi_physio VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?);");
    if (!ok)
        goto bad;

    query.bindValue(0, hospiphysio.hospirecid);
    query.bindValue(1, hospiphysio.patientid);
    query.bindValue(2, hospiphysio.patientname);
    query.bindValue(3, hospiphysio.physioid);
    query.bindValue(4, hospiphysio.physioname);
    query.bindValue(5, hospiphysio.freqperiod);
    query.bindValue(6, hospiphysio.freqcount);
    query.bindValue(7, hospiphysio.startdate);
    query.bindValue(8, hospiphysio.enddate);
    ok = query.exec();
    if (!ok)
        goto bad;
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法添加住院理疗项目信息。" + exterrstr);
    return;
}

void PsmService::updateHospiPhysio(const PsmSrvHospiPhysio &hospiphysio,
                                   const QDate &oldstartdate, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("UPDATE hospi_physio "
                       "SET pati_id=?, pati_name=?, freq_day=?, freq_cnt=?, startdate=?, endate=? "
                       "WHERE hospi_id=? AND physio_id=? AND startdate=?;");
    if (!ok)
        goto bad;

    query.bindValue(0, hospiphysio.patientid);
    query.bindValue(1, hospiphysio.patientname);
    query.bindValue(2, hospiphysio.freqperiod);
    query.bindValue(3, hospiphysio.freqcount);
    query.bindValue(4, hospiphysio.startdate);
    query.bindValue(5, hospiphysio.enddate);
    query.bindValue(6, hospiphysio.hospirecid);
    query.bindValue(7, hospiphysio.physioid);
    query.bindValue(8, (oldstartdate.isValid() ? oldstartdate : hospiphysio.startdate));
    ok = query.exec();
    if (!ok)
        goto bad;
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法更新住院理疗项目信息。" + exterrstr);
    return;
}

void PsmService::deleteHospiPhysio(const PsmSrvHospiPhysio &hospiphysio, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("DELETE FROM hospi_physio WHERE hospi_id=? AND physio_id=? AND startdate=?;");
    if (!ok)
        goto bad;

    query.bindValue(0, hospiphysio.hospirecid);
    query.bindValue(1, hospiphysio.physioid);
    query.bindValue(2, hospiphysio.startdate);
    ok = query.exec();
    if (!ok)
        goto bad;
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法删除选定住院理疗项目信息。" + exterrstr);
    return;
}

bool PsmService::readSelectedPhysioLog(QTableWidget *tbl, PsmSrvPhysioLog *physiolog)
{
    int rowidx = this->getTableSelectedRow(tbl);
    if (rowidx < 0)
        return false;

    QTableWidgetItem *itempatientid = tbl->item(rowidx, 0);
    QTableWidgetItem *itempatient = tbl->item(rowidx, 1);
    QTableWidgetItem *itemphysio = tbl->item(rowidx, 2);
    QTableWidgetItem *itemnurse = tbl->item(rowidx, 3);
    QTableWidgetItem *itemmachineid = tbl->item(rowidx, 4);
    QTableWidgetItem *itemoptime = tbl->item(rowidx, 5);
    if (itempatientid == NULL || itempatient == NULL|| itemphysio == NULL || itemnurse == NULL ||
            itemmachineid == NULL || itemoptime == NULL)
        return false;

    physiolog->patientid = itempatientid->text();
    physiolog->patientname = itempatient->text();
    physiolog->physioid = itemphysio->data(Qt::UserRole).toString();
    physiolog->physioname = itemphysio->text();
    physiolog->nurseid = itemnurse->data(Qt::UserRole).toString();
    physiolog->nursename = itemnurse->text();
    physiolog->machineid = itemmachineid->text();
    physiolog->optime = itemoptime->data(Qt::UserRole).toDateTime();
    return true;
}

void PsmService::listHospiPhysioLog(const PsmSrvHospiPhysio &hospiphysio,
                                    QLabel *lbl, QTableWidget *tbl, QWidget *window)
{
    static QList<int> colmap, datamap;
    if (colmap.isEmpty())
        colmap << 0 << 1 << 3 << 5 << 6 << 7;
    if (datamap.isEmpty())
        datamap << -1 << 0 << 2 << 4 << -1 << 7;

    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("SELECT * FROM physio_rec WHERE pati_id=? AND physio_id=? AND "
                                                      "DATE(optime)>=? AND DATE(optime)<=?;");
    if (!ok)
        goto bad;

    query.bindValue(0, hospiphysio.patientid);
    query.bindValue(1, hospiphysio.physioid);
    query.bindValue(2, hospiphysio.startdate);
    query.bindValue(3, hospiphysio.enddate);
    ok = query.exec();
    if (!ok)
        goto bad;

    lbl->setText(QString::number(query.size()));
    this->database.fillTableWidget(tbl, &query, colmap, datamap);
    return;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法完成理疗记录检索。" + exterrstr);
    return;
}

QList<PsmSrvHospiPhysio> PsmService::getNowPermitPhysio(const QString &patientid, const QString &physioid,
                                                        const QDate &checkdate)
{
    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("SELECT * FROM hospi_physio WHERE pati_id=? AND physio_id=? AND "
                                                        "startdate<=? AND endate>=?;");
    if (!ok)
        return QList<PsmSrvHospiPhysio>();

    query.bindValue(0, patientid);
    query.bindValue(1, physioid);
    query.bindValue(2, checkdate);
    query.bindValue(3, checkdate);
    ok = query.exec();
    if (!ok)
        return QList<PsmSrvHospiPhysio>();

    if (query.size() < 1)
        return QList<PsmSrvHospiPhysio>();

    ok = query.first();
    if (!ok)
        return QList<PsmSrvHospiPhysio>();

    QList<PsmSrvHospiPhysio> retlist;
    do {
        QSqlRecord rec = query.record();
        PsmSrvHospiPhysio hospiphysio;

        hospiphysio.hospirecid = rec.value(0).toString();
        hospiphysio.patientid = rec.value(1).toString();
        hospiphysio.patientname = rec.value(2).toString();
        hospiphysio.physioid = rec.value(3).toString();
        hospiphysio.physioname = rec.value(4).toString();
        hospiphysio.freqperiod = rec.value(5).toInt();
        hospiphysio.freqcount = rec.value(6).toInt();
        hospiphysio.startdate = rec.value(7).toDate();
        hospiphysio.enddate = rec.value(8).toDate();

        retlist.append(hospiphysio);
    } while (query.next());

    return retlist;
}

QDate PsmService::getDateBucket(const QDate &startdate, const QDate &checkdate, int period)
{
    int detdays = (int)startdate.daysTo(checkdate);
    int basedays = detdays - (detdays % period);
    return startdate.addDays(basedays);
}

bool PsmService::checkPhysioPermitNow(const QDate &checkdate, const PsmSrvHospiPhysio &hospiphysio)
{
    QDate bucketstart = this->getDateBucket(hospiphysio.startdate, checkdate, hospiphysio.freqperiod);
    QDate bucketend = bucketstart.addDays(hospiphysio.freqperiod - 1);

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("SELECT COUNT(*) FROM physio_rec WHERE pati_id=? AND physio_id=? AND "
                                                             "DATE(optime)>=? AND DATE(optime)<=?;");
    if (!ok)
        return false;

    query.bindValue(0, hospiphysio.patientid);
    query.bindValue(1, hospiphysio.physioid);
    query.bindValue(2, bucketstart);
    query.bindValue(3, bucketend);
    ok = query.exec();
    if (!ok)
        return false;

    if (query.size() < 1)
        return false;

    ok = query.first();
    if (!ok)
        return false;

    QSqlRecord rec = query.record();
    int physiocnt = rec.value(0).toInt();
    if (physiocnt < hospiphysio.freqcount)
        return true;
    else
        return false;
}

bool PsmService::checkPhysioPermitNow(const QDate &checkdate, const QList<PsmSrvHospiPhysio> &hplist)
{
    PsmSrvHospiPhysio hospiphysio;
    foreach (hospiphysio, hplist) {
        bool res = this->checkPhysioPermitNow(checkdate, hospiphysio);
        if (res)
            return true;
    }
    return false;
}

bool PsmService::insertPhysioLog(const PsmSrvPhysioLog &physiolog, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("INSERT INTO physio_rec VALUES(?, ?, ?, ?, ?, ?, ?, ?);");
    if (!ok)
        goto bad;

    query.bindValue(0, physiolog.patientid);
    query.bindValue(1, physiolog.patientname);
    query.bindValue(2, physiolog.physioid);
    query.bindValue(3, physiolog.physioname);
    query.bindValue(4, physiolog.nurseid);
    query.bindValue(5, physiolog.nursename);
    query.bindValue(6, this->machineid);
    query.bindValue(7, physiolog.optime);
    ok = query.exec();
    if (!ok)
        goto bad;
    return true;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法向数据库添加理疗记录。" + exterrstr);
    return false;
}

bool PsmService::tryAddPhysioLog(const PsmSrvPhysioLog &physiolog, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    QDate logdate = physiolog.optime.date();
    QList<PsmSrvHospiPhysio> hplist = this->getNowPermitPhysio(physiolog.patientid, physiolog.physioid, logdate);
    if (hplist.size() < 1) {
        QMessageBox::warning(window, "打卡操作拒绝", "无法找到患者当前时间打卡对应的住院等级或理疗登记。");
        return false;
    }

    bool allow = this->checkPhysioPermitNow(logdate, hplist);
    if (!allow) {
        QMessageBox::warning(window, "打卡操作拒绝", "患者已完成今日该项理疗的住院登记要求，不再接受打卡。");
        return false;
    }

    bool ok = this->insertPhysioLog(physiolog, window);
    if (!ok)
        return false;

    QMessageBox::information(window, "打卡操作成功", "患者打卡已完成，可以为其进行理疗操作！");
    return true;
}

bool PsmService::deletePhysioLog(const PsmSrvPhysioLog &physiolog, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.prepare("DELETE FROM physio_rec WHERE pati_id=? AND physio_id=? AND optime=?;");
    if (!ok)
        goto bad;

    query.bindValue(0, physiolog.patientid);
    query.bindValue(1, physiolog.physioid);
    query.bindValue(2, physiolog.optime);
    ok = query.exec();
    if (!ok)
        goto bad;
    return true;

bad:
    QSqlError sqlerr = query.lastError();
    QString exterrstr;
    if (sqlerr.isValid()) {
        exterrstr = "错误码：" + sqlerr.nativeErrorCode() + "\n" +
                    "数据库系统描述：" + sqlerr.text();
    }
    QMessageBox::warning(window, "数据库错误", "无法删除理疗记录。" + exterrstr);
    return false;
}

QDateTime PsmService::getDbTime()
{
    bool ok;
    QSqlQuery query = this->database.getQuery();
    ok = query.exec("SELECT NOW(0);");
    if (!ok)
        return QDateTime::currentDateTime();

    ok = query.first();
    if (!ok)
        return QDateTime::currentDateTime();

    QSqlRecord rec = query.record();
    return rec.value(0).toDateTime();
}

void PsmService::insertPhysioLogToTable(const PsmSrvPhysioLog &physiolog, QTableWidget *tbl)
{
    tbl->insertRow(0);
    while (tbl->rowCount() > 100)
        tbl->removeRow(tbl->rowCount() - 1);

    /* Patient ID */
    QTableWidgetItem *item;
    item = tbl->item(0, 0);
    if (item != NULL) {
        item->setText(physiolog.patientid);
    } else {
        item = new QTableWidgetItem(physiolog.patientid);
        tbl->setItem(0, 0, item);
    }

    /* Patient name */
    item = tbl->item(0, 1);
    if (item != NULL) {
        item->setText(physiolog.patientname);
    } else {
        item = new QTableWidgetItem(physiolog.patientname);
        tbl->setItem(0, 1, item);
    }
    item->setData(Qt::UserRole, physiolog.patientid);

    /* Physio item */
    item = tbl->item(0, 2);
    if (item != NULL) {
        item->setText(physiolog.physioname);
    } else {
        item = new QTableWidgetItem(physiolog.physioname);
        tbl->setItem(0, 2, item);
    }
    item->setData(Qt::UserRole, physiolog.physioid);

    /* Nurse */
    item = tbl->item(0, 3);
    if (item != NULL) {
        item->setText(physiolog.nursename);
    } else {
        item = new QTableWidgetItem(physiolog.nursename);
        tbl->setItem(0, 3, item);
    }
    item->setData(Qt::UserRole, physiolog.nurseid);

    /* Machine Id */
    item = tbl->item(0, 4);
    if (item != NULL) {
        item->setText(this->machineid);
    } else {
        item = new QTableWidgetItem(this->machineid);
        tbl->setItem(0, 4, item);
    }

    /* Optime */
    item = tbl->item(0, 5);
    if (item != NULL) {
        item->setText(physiolog.optime.toString(Qt::ISODate));
    } else {
        item = new QTableWidgetItem(physiolog.optime.toString(Qt::ISODate));
        tbl->setItem(0, 5, item);
    }
    item->setData(Qt::UserRole, physiolog.optime);
}

void PsmService::exportDatabase(const QString &dumpexec, const QString &outfile, QWidget *window)
{
    if (window == NULL)
        window = this->parent;

    bool ok = this->database.dumpDatabase(dumpexec, outfile);
    if (!ok) {
        QMessageBox::warning(window, "数据库导出失败", "数据库导出失败！");
        return;
    }

    QMessageBox::information(window, "数据库导出成功", "数据库导出成功！");
}
