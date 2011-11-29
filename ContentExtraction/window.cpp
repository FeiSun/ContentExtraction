#include "window.h"
#include "ui_window.h"

Window::Window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Window)
{
    ui->setupUi(this);

    QObject::connect(this, SIGNAL(ProcessDone()), this, SLOT(ProcessNext()));

    //disable js in web page
    QWebSettings *glob_setting = ui->webView->settings()->globalSettings();
    glob_setting->setAttribute(QWebSettings::JavascriptEnabled, false);
}

Window::~Window()
{
    delete ui;
}

void Window::setUrl(const QUrl &url)
{
    ui->webView->setUrl(url);
}


void Window::on_webView_loadFinished(bool )
{
    QWebFrame *frame = ui->webView->page()->mainFrame();
    QWebElement document = frame->documentElement();

    QWebElement body = document.firstChild().nextSibling();
    PreprocessDOM(body);

    CountChar(body);
    CountTag(body);
    CountLinkChar(body);
    CountLinkTag(body);

    double char_num = body.attribute(kg_char_num).toDouble();
    double linkchar_num = body.attribute(kg_linkchar_num).toDouble();
    double ratio = linkchar_num / char_num;

    ComputeTextDensity(body, ratio);
    ComputeDensitySum(body, ratio);
    double max_density_sum = FindMaxDensitySum(body);
    SetMark(body, 0);
    double threshold = GetThreshold(body, max_density_sum);
    MarkContent(body, threshold);
    CleanTreeByMark(body);
    RemoveAttribute(body);

//    CountCharVariant(body);
//    CountTag(body);
//    ComputeTextDensityVariant(body);
//    ComputeDensitySumVariant(body);
//    double max_density_sum = FindMaxDensitySum(body);
//    SetMark(body, 0);
//    double threshold = GetThreshold(body, max_density_sum);
//    MarkContent(body, threshold);
//    CleanTreeByMark(body);
//    RemoveAttributeVariant(body);


    // set encoding
    QWebElement meta = document.findFirst("[http-equiv=Content-Type]");
    meta.setAttribute("content","text/html; charset=UTF-8");

    QFile file(output_path + "\\" + processing_file.left(processing_file.lastIndexOf("."))+".txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    QTextStream out(&file);
    out.setCodec("UTF-8");
    out << document.toPlainText();
    file.close();

    emit ProcessDone();
}

void Window::ProcessNext()
{
    processing_num++;
    if(processing_num != file_list.size())
    {
        QString file_path = file_list.at(processing_num).absoluteFilePath();
        processing_file = file_list.at(processing_num).fileName();
        setUrl(QUrl(file_path));
    }
}

void Window::on_folderButton_clicked()
{
    QString output_folder = "output-variant";        //set the output path as you like

    path = QFileDialog::getExistingDirectory(this);

    QDir dir(path);

    file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);

    dir.cdUp();
    dir.mkdir(output_folder);
    dir.cd(output_folder);

    output_path = dir.absolutePath();
}

void Window::on_cancelButton_clicked()
{
    close();
}

void Window::on_startButton_clicked()
{
    if(path.isEmpty())
    {
         QMessageBox msgBox;
         msgBox.setText("You haven't open directory!");
         msgBox.setInformativeText("Click close return");
         msgBox.setStandardButtons(QMessageBox::Close);
         msgBox.setDefaultButton(QMessageBox::Close);
         msgBox.exec();
    }
    else
    {
        if(file_list.isEmpty())
        {
            QMessageBox msgBox;
            msgBox.setText("The directory has no file!");
            msgBox.setInformativeText("Click close return");
            msgBox.setStandardButtons(QMessageBox::Close);
            msgBox.setDefaultButton(QMessageBox::Close);
            msgBox.exec();
        }
        else
        {
            processing_num = 0;
            QString file_path = file_list.at(processing_num).absoluteFilePath();
            processing_file = file_list.at(processing_num).fileName();
            setUrl(QUrl(file_path));
        }
    }
}
