/*
 * Copyright 2011 Sun Fei. All Rights Reserved.
 * Author: Ofey.sunfei@gmail.com (Sun Fei)
 *
 * some common functions and data structure
 */

#ifndef FOUNDATION_H
#define FOUNDATION_H

#include <qmath.h>
#include <limits>
#include <QString>
#include <QtWebKit>
#include <QUrl>
#include <QWebElement>
#include <QWebElementCollection>
#include <QWebSettings>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>

// The property name we used in DOM tree.
const QString kg_char_num = "char-number";
const QString kg_tag_num = "tag-number";
const QString kg_linkchar_num = "linkchar-number";
const QString kg_linktag_num = "linktag-number";
const QString kg_text_density = "text-density";
const QString kg_density_sum = "density-sum";
const QString kg_max_density_sum = "max-density-sum";
const QString kg_mark = "mark";
const QString kg_geometry = "geometry";

/**
* search tag by the attribute and its value
* @param element search range
* @param attribute attribute's name
* @param value  attribute's value
* @return the target element
*/
QWebElement SearchTag(QWebElement &element,const QString attribute, double value);

/**
* get file list by path
* @param path path name
* @return file info list
*/
QFileInfoList GetFileList(QString path);

#endif // FOUNDATION_H
