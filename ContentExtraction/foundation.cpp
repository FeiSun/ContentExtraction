#include <foundation.h>

QWebElement SearchTag(QWebElement &element, const QString attribute, double value)
{
    QString d2s_value = QString::number(value, 'f');
    QWebElement target = element;

    double attr_value = element.attribute(attribute).toDouble();
    if((attr_value - value > -1 * std::numeric_limits<double>::epsilon())
        && (attr_value - value < std::numeric_limits<double>::epsilon()))
    {
         return target;
    }

    //search the max densitysum element using css selector
    QString css_selector = "[" + attribute + "=\"" + d2s_value + "\"]";
    target = element.findFirst(css_selector);
    return target;
}

QFileInfoList GetFileList(QString path)
{
    QDir dir(path);
    QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for(int i = 0; i != folder_list.size(); i++)
    {
         QString name = folder_list.at(i).absoluteFilePath();
         QFileInfoList child_file_list = GetFileList(name);
         file_list.append(child_file_list);
    }

    return file_list;
}
