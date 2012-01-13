/*
 * Dom Based Content Extraction via Text Density
 * Copyright (c) 2011 Sun Fei. All Rights Reserved.
 * Author: Ofey.sunfei@gmail.com (Sun Fei)
 * License: GPLv3 <http://www.gnu.org/copyleft/gpl.html>
 */
 
#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <foundation.h>
#include <clean.h>
#include <textdensity.h>

namespace Ui {
    class Window;
}

class Window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = 0);
    ~Window();

    void setUrl(const QUrl &url);

private:
    Ui::Window *ui;
    QString path;               //input path
    QString output_path;
    QString processing_file;    //filename of file under processing
    QFileInfoList file_list;    //input file list
    int processing_num;         //record the number of processed webpage

signals:
    void ProcessDone();

private slots:
    void on_startButton_clicked();
    void on_webView_loadFinished(bool );
    void on_folderButton_clicked();
    void on_cancelButton_clicked();

    //process next webpage
    void ProcessNext();
};

#endif // WINDOW_H
