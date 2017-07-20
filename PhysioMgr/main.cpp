/*-
 * GNU GENERAL PUBLIC LICENSE, version 3
 * See LICENSE file for detail.
 *
 * Author: Yulong Yu, Jun 29th, 2017
 * Copyright(c) 2015-2017 Yulong Yu. All rights reserved.
 */

#include "MainWindow.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    //QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForLocale(codec);
    //QTextCodec::setCodecForCStrings(codec);

    MainWindow w;
    w.showMaximized();
    if (!w.initService())
        return -1;

    return a.exec();
}
