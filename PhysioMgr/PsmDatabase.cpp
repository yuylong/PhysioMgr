/*-
 * GNU GENERAL PUBLIC LICENSE, version 3
 * See LICENSE file for detail.
 *
 * Author: Yulong Yu, Jun 29th, 2017
 * Copyright(c) 2015-2017 Yulong Yu. All rights reserved.
 */

#include "PsmDatabase.h"

#include <QIODevice>
#include <QFile>

QString PsmDatabase::connectname("PsmDb");

PsmDatabase::PsmDatabase()
{
    this->host = QString();
    this->username = QString();
    this->password = QString();
    this->schema = QString();
    this->isStarted = false;
}

QString PsmDatabase::getHost() const
{
    return this->host;
}

QString PsmDatabase::getUsername() const
{
    return this->username;
}

QString PsmDatabase::getPassword() const
{
    return this->password;
}

QString PsmDatabase::getSchema() const
{
    return this->schema;
}

bool PsmDatabase::isConnectionConfigured() const
{
    return !(this->host.isEmpty() || this->username.isEmpty() || this->password.isEmpty() || this->schema.isEmpty());
}

void PsmDatabase::configConnection(const QString &host, const QString &username,
                                   const QString &password, const QString &schema)
{
    if (this->isStarted)
        return;

    this->host = host;
    this->username = username;
    this->password = password;
    this->schema = schema;
}

void PsmDatabase::configConnectionFromDom(const QDomElement &docelem)
{
    if (this->isStarted)
        return;
    if (QString::compare(docelem.tagName(), "database") != 0)
        return;

    QDomNode domnode = docelem.firstChild();
    while (!domnode.isNull()) {
        QDomElement domelem = domnode.toElement();
        if (QString::compare(domelem.tagName(), "host") == 0)
            this->host = domelem.text();
        else if (QString::compare(domelem.tagName(), "username") == 0)
            this->username = domelem.text();
        else if (QString::compare(domelem.tagName(), "password") == 0)
            this->password = domelem.text();
        else if (QString::compare(domelem.tagName(), "schema") == 0)
            this->schema = domelem.text();

        domnode = domnode.nextSibling();
    }
}

void PsmDatabase::configConnectionFromFile(const QString &path)
{
    if (this->isStarted)
        return;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return;

    QDomDocument domdoc("PsmDbConfig");
    if (!domdoc.setContent(&file)) {
        file.close();
        return;
    }
    file.close();

    QDomElement docelem = domdoc.documentElement();
    this->configConnectionFromDom(docelem);
}

bool PsmDatabase::startDatabase()
{
    if (!this->isConnectionConfigured())
        return false;
    if (this->isStarted)
        return true;

    if (QSqlDatabase::contains(connectname))
        QSqlDatabase::removeDatabase(connectname);

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", connectname);
    if (!db.isValid())
        return false;

    db.setHostName(this->host);
    db.setUserName(this->username);
    db.setPassword(this->password);
    db.setDatabaseName(this->schema);
    this->isStarted = true;

    // Try open the database.
    db.open();
    return true;
}

void PsmDatabase::stopDatabase()
{
    if (!this->isStarted)
        return;

    QSqlDatabase::removeDatabase(connectname);
    this->isStarted = false;
    return;
}

bool PsmDatabase::isDatabaseStarted() const
{
    return this->isStarted;
}

QSqlQuery PsmDatabase::getQuery()
{
    if (!this->isStarted)
        return QSqlQuery();

    // Database will be open automatically.
    QSqlDatabase db = QSqlDatabase::database(connectname, true);

    QSqlQuery query(db);
    return query;
}

void PsmDatabase::fillTableWidget(QTableWidget *tblwdg, QSqlQuery *query,
                                  const QList<int> &colmap, const QList<int> &datamap)
{
    tblwdg->setRowCount(query->size());
    tblwdg->setColumnCount(colmap.size());

    bool ok = query->first();
    if (!ok)
        return;

    QTableWidgetItem *item;
    int row = 0;
    do {
        QSqlRecord rec = query->record();

        for (int i = 0; i < colmap.size(); i++) {
            QString valstr = rec.value(colmap[i]).toString();
            item = tblwdg->item(row, i);
            if (item != NULL) {
                item->setText(valstr);
            } else {
                item = new QTableWidgetItem(valstr);
                tblwdg->setItem(row, i, item);
            }

            if (datamap.size() > i && datamap[i] >= 0)
                item->setData(Qt::UserRole, rec.value(datamap[i]));
        }
        row++;
    } while (query->next());
}

bool PsmDatabase::dumpDatabase(const QString &dumpexec, const QString &outfile)
{
    QFile f(outfile);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream fots(&f);
    fots << "CREATE DATABASE IF NOT EXISTS `" + this->schema + "` /*!40100 DEFAULT CHARACTER SET utf8 */;" << endl;
    fots << "USE `" + this->schema + "`;" << endl;
    f.close();

    QProcess mysqldump;
    mysqldump.setStandardOutputFile(outfile, QIODevice::Append);

    QStringList arglist;
    arglist << "--user=" + this->username << "--host=" + this->host << "--password=" + this->password <<
               "--protocol=tcp" << "--port=3306" << "--default-character-set=utf8" <<
               "--routines" << "--events" << this->schema;
    mysqldump.start(dumpexec, arglist);
    if (!mysqldump.waitForFinished(3 * 60 * 1000))
        return false;

    qDebug() << mysqldump.readAllStandardError();
    if (mysqldump.exitCode() != 0) {
        qDebug() << "Exit Code:" << mysqldump.exitCode();
        return false;
    }
    return true;
}

bool PsmDatabase::importDatabase(const QString &sqlexec, const QString &infile)
{
    QProcess mysql;
    mysql.setStandardInputFile(infile);

    QStringList arglist;
    arglist << "--host=" + this->host << "--user=" + this->username << "--password=" + this->password <<
               "--protocol=tcp" << "--port=3306" << "--default-character-set=utf8" << "--comments";

    mysql.start(sqlexec, arglist);
    if (!mysql.waitForFinished(3 * 60 * 1000))
        return false;

    qDebug() << mysql.readAllStandardError();
    if (mysql.exitCode() != 0) {
        qDebug() << "Exit Code:" << mysql.exitCode();
        return false;
    }
    return true;
}
