#include "clean.h"

void PreprocessDOM(QWebElement &element)
{
    //The underlying content of QWebElement is explicitly shared.
    //Creating a copy of a QWebElement does not create a copy of the content.
    //Instead, both instances point to the same element.

    QWebElement remove_element;
    QString style;
    QString display = "display";

    QWebElement child = element.firstChild();
    for(;!child.isNull();)
    {
        style = child.styleProperty(display, QWebElement::ComputedStyle);

        //if display:none
        if(!style.compare("none"))
        {
            remove_element = child;
            child = child.nextSibling();
            remove_element.removeFromDocument();
            continue;
        }

        child = child.nextSibling();
    }

    child = element.firstChild();
    for(;!child.isNull();child = child.nextSibling())
    {
        PreprocessDOM(child);
    }

//    QWebElementCollection all_script = element.findAll("script");
//    foreach (QWebElement it_script, all_script)
//    {
//        it_script.removeFromDocument();
//    }
}

void RemoveAttribute(QWebElement &element)
{
    //remove the attribute added
    element.removeAttribute(kg_char_num);
    element.removeAttribute(kg_tag_num);
    element.removeAttribute(kg_linkchar_num);
    element.removeAttribute(kg_linktag_num);
    element.removeAttribute(kg_text_density);
    element.removeAttribute(kg_density_sum);
    element.removeAttribute(kg_max_density_sum);
    element.removeAttribute(kg_mark);

    for(QWebElement child = element.firstChild(); !child.isNull(); child = child.nextSibling())
    {
        RemoveAttribute(child);
    }
}

void RemoveAttributeVariant(QWebElement &element)
{
    //remove the attribute added
    element.removeAttribute(kg_char_num);
    element.removeAttribute(kg_tag_num);
    element.removeAttribute(kg_text_density);
    element.removeAttribute(kg_density_sum);
    element.removeAttribute(kg_max_density_sum);
    element.removeAttribute(kg_mark);

    for(QWebElement child = element.firstChild(); !child.isNull(); child = child.nextSibling())
    {
        RemoveAttribute(child);
    }
}

void CleanTreeByMark(QWebElement &element)
{
    int mark = element.attribute(kg_mark).toInt();
    QWebElement remove_element;

    if(0 == mark)
    {
        //element.removeAllChildren();
        element.removeFromDocument();
    }
    else if(1 == mark)
    {
        return;
    }
    else
    {
        for(QWebElement child = element.firstChild(); !child.isNull();)
        {
            remove_element = child;
            child = child.nextSibling();
            CleanTreeByMark(remove_element);
        }
    }
}
