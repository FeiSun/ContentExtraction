#include <textdensity.h>

void CountChar(QWebElement &element)
{
    long char_num = 0;
    QString l2s_char_num;

    char_num = element.toPlainText().length();
    l2s_char_num = QString::number(char_num, 10);
    element.setAttribute(kg_char_num, l2s_char_num);

    for(QWebElement child = element.firstChild(); !child.isNull(); child = child.nextSibling())
    {
        CountChar(child);
    }
}

void CountTag(QWebElement &element)
{
    long tag_num = 0;
    QString l2s_tag_num;

    if(element.firstChild().isNull())
    {
        l2s_tag_num = QString::number(0, 10);
        element.setAttribute(kg_tag_num, l2s_tag_num);
    }
    else
    {
        for(QWebElement child = element.firstChild(); !child.isNull(); child = child.nextSibling())
        {
            CountTag(child);
        }
        for(QWebElement child = element.firstChild(); !child.isNull(); child = child.nextSibling())
        {
            tag_num += child.attribute(kg_tag_num).toLong() + 1;
        }

        l2s_tag_num = QString::number(tag_num, 10);
        element.setAttribute(kg_tag_num, l2s_tag_num);
    }
}

void UpdateLinkChar(QWebElement &element)
{
    for(QWebElement child = element.firstChild(); !child.isNull(); child = child.nextSibling())
    {
        child.setAttribute(kg_linkchar_num, child.attribute(kg_char_num));
        UpdateLinkChar(child);
    }
}

//call this function after CountChar
void CountLinkChar(QWebElement &element)
{
    long linkchar_num = 0;
    QString l2s_linkchar_num;
    QString tag_name = element.tagName();

    for(QWebElement child = element.firstChild(); !child.isNull(); child = child.nextSibling())
    {
        CountLinkChar(child);
    }

    //deal with hyperlink and sth like that
    if(!tag_name.compare("A") || !tag_name.compare("BUTTON") || !tag_name.compare("SELECT"))
    {
        linkchar_num = element.attribute(kg_char_num).toLong();
        UpdateLinkChar(element);
    }
    else
    {
        for(QWebElement child = element.firstChild(); !child.isNull(); child = child.nextSibling())
        {
            linkchar_num += child.attribute(kg_linkchar_num).toLong();
        }
    }

    l2s_linkchar_num = QString::number(linkchar_num, 10);
    element.setAttribute(kg_linkchar_num, l2s_linkchar_num);
}

void UpdateLinkTag(QWebElement &element)
{
    for(QWebElement child = element.firstChild(); !child.isNull(); child = child.nextSibling())
    {
        child.setAttribute(kg_linktag_num, child.attribute(kg_tag_num));
        UpdateLinkTag(child);
    }
}

//call this function after CountChar, CountLinkChar
void CountLinkTag(QWebElement &element)
{
    long linktag_num = 0;
    QString l2s_linktag_num;
    QString tag_name = element.tagName();

    for(QWebElement child = element.firstChild(); !child.isNull(); child = child.nextSibling())
    {
        CountLinkTag(child);
    }

    //deal with hyperlink and sth like that
    if(!tag_name.compare("A") || !tag_name.compare("BUTTON") || !tag_name.compare("SELECT"))
    {
        linktag_num = element.attribute(kg_tag_num).toLong();
        UpdateLinkChar(element);
    }
    else
    {
        for(QWebElement child = element.firstChild(); !child.isNull(); child = child.nextSibling())
        {
            linktag_num += child.attribute(kg_linktag_num).toLong();
            tag_name = child.tagName();

            //if a tag is <a> or sth plays similar role in web pages, then anchor number add 1
            if(!tag_name.compare("A") || !tag_name.compare("BUTTON") || !tag_name.compare("SELECT"))
            {
                linktag_num++;
            }
            else
            {
                long child_linktag_num = child.attribute(kg_linktag_num).toLong();
                long child_tag_num = child.attribute(kg_tag_num).toLong();
                long child_char_num = child.attribute(kg_char_num).toLong();
                long child_linkchar_num = child.attribute(kg_linkchar_num).toLong();

                //child_linktag_num != 0: there are some anchor under this child
                if(child_linktag_num == child_tag_num && child_char_num == child_linkchar_num && 0 != child_linktag_num)
                {
                    linktag_num++;
                }
            }
        }
    }

    l2s_linktag_num = QString::number(linktag_num, 10);
    element.setAttribute(kg_linktag_num, l2s_linktag_num);
}

//call this function after CountChar, CountTag, CountLinkChar, CountLinkTag
void ComputeTextDensity(QWebElement &element, double ratio)
{
    long char_num = element.attribute(kg_char_num).toLong();
    long tag_num = element.attribute(kg_tag_num).toLong();
    long linkchar_num = element.attribute(kg_linkchar_num).toLong();
    long linktag_num = element.attribute(kg_linktag_num).toLong();

    double text_density = 0.0;
    QString d2s_text_density;

    if(0 == char_num)
    {
        text_density = 0;
    }
    else
    {
        long un_linkchar_num = char_num - linkchar_num;

        if(0 == tag_num)
        {
            tag_num = 1;
        }
        if(0 == linkchar_num)
        {
            linkchar_num = 1;
        }
        if(0 == linktag_num)
        {
            linktag_num = 1;
        }
        if(0 == un_linkchar_num)
        {
            un_linkchar_num = 1;
        }

        text_density = (1.0 * char_num / tag_num) * qLn((1.0 * char_num * tag_num) / (1.0 * linkchar_num * linktag_num))
                       / qLn(qLn(1.0 * char_num * linkchar_num / un_linkchar_num + ratio * char_num + qExp(1.0)));

//        text_density = 1.0 * char_num / tag_num;
    }

    //convert double to QString
    d2s_text_density = QString::number(text_density, 'f');
    element.setAttribute(kg_text_density, d2s_text_density);

    for(QWebElement child = element.firstChild(); !child.isNull(); child = child.nextSibling())
    {
        ComputeTextDensity(child, ratio);
    }
}

void ComputeDensitySum(QWebElement &element, double ratio)
{
    double density_sum = 0.0;
    long char_num_sum = 0;
    //long char_num = 0;
    QString d2s_density_sum;

    QString content = element.toPlainText();
    QString child_content;
    int from = 0;
    int index = 0;
    int length = 0;
    //double text_density = 0.0;

    if(element.firstChild().isNull())
    {
        density_sum = element.attribute(kg_text_density).toDouble();
    }
    else
    {
        for(QWebElement child = element.firstChild(); !child.isNull(); child = child.nextSibling())
        {
            ComputeDensitySum(child, ratio);
        }
        for(QWebElement child = element.firstChild(); !child.isNull(); child = child.nextSibling())
        {
            density_sum += child.attribute(kg_text_density).toDouble();
            char_num_sum += child.attribute(kg_char_num).toLong();

            //text before tag
            child_content = child.toPlainText();
            index = content.indexOf(child_content, from);
            if(index > -1)
            {
                length = index - from;
                if(length > 0)
                {
                    density_sum += length * qLn(1.0 * length) / qLn(qLn(ratio * length + qExp(1.0)));
                }
                from = index + child_content.length();
            }
        }

        //text after tag
        length = element.toPlainText().length() - from;
        if(length > 0)
        {
            density_sum += length * qLn(1.0 * length) / qLn(qLn(ratio * length + qExp(1.0)));
        }
    }

    d2s_density_sum = QString::number(density_sum, 'f');
    element.setAttribute(kg_density_sum, d2s_density_sum);
}

double FindMaxDensitySum(QWebElement &element)
{
    double max_density_sum = element.attribute(kg_density_sum).toDouble();
    double temp_max_density_sum = 0.0;

    for(QWebElement child = element.firstChild(); !child.isNull(); child = child.nextSibling())
    {
        temp_max_density_sum = FindMaxDensitySum(child);
        if(temp_max_density_sum - max_density_sum > std::numeric_limits<double>::epsilon())
        {
            max_density_sum = temp_max_density_sum;
        }
    }

    //record the max_density_sum under the element
    QString d2s_max_density_sum = QString::number(max_density_sum, 'f');
    element.setAttribute(kg_max_density_sum, d2s_max_density_sum);
    return max_density_sum;
}

double GetThreshold(QWebElement &element, double max_density_sum)
{
    double threshold = -1.0;

    //search the max densitysum element
    QWebElement target = SearchTag(element, kg_density_sum, max_density_sum);
    threshold = target.attribute(kg_text_density).toDouble();
    SetMark(target, 1);

    QWebElement parent = target.parent();
    while(true)
    {
        if(parent.tagName().compare("HTML"))
        {
            double text_density = parent.attribute(kg_text_density).toDouble();
            if((threshold - text_density) > -1 * std::numeric_limits<double>::epsilon())
            {
                threshold = text_density;
            }

            parent.setAttribute(kg_mark, "2");
            parent = parent.parent();
        }
        else
        {
            break;
        }
    }

    return threshold;
}

void SetMark(QWebElement &element, int mark)
{
    QString i2s_mark = QString::number(mark);

    element.setAttribute(kg_mark, i2s_mark);

    for(QWebElement child = element.firstChild(); !child.isNull(); child = child.nextSibling())
    {
        SetMark(child, mark);
    }
}

void FindMaxDensitySumTag(QWebElement &element, double max_density_sum)
{
    //search the max densitysum element
    QWebElement target = SearchTag(element, kg_density_sum, max_density_sum);;

    int mark = target.attribute(kg_mark).toInt();
    if(1 == mark)
    {
        return;
    }

    SetMark(target, 1);

    QWebElement parent = target.parent();
    while(true)
    {
        if(parent.tagName().compare("HTML"))
        {
            parent.setAttribute(kg_mark, "2");
            parent = parent.parent();
        }
        else
        {
            break;
        }
    }
}

void MarkContent(QWebElement &element, double threshold)
{
    double text_density = element.attribute(kg_text_density).toDouble();
    double max_density_sum = element.attribute(kg_max_density_sum).toDouble();
    int mark = element.attribute(kg_mark).toInt();

    if(mark != 1 && (text_density - threshold > -1 * std::numeric_limits<double>::epsilon()))
    {
        FindMaxDensitySumTag(element, max_density_sum);
        for(QWebElement child = element.firstChild(); !child.isNull(); child = child.nextSibling())
        {
            MarkContent(child, threshold);
        }
    }
}


//////////////////////////////////////////////////////////////////////////////////////
/////////another method: collapse the character count of link anchors to 0////////////
//////////////////////////////////////////////////////////////////////////////////////

void UpdateLinkCharVariant(QWebElement &element)
{
    QString l2s_char_num = QString::number(0, 10);
    for(QWebElement child = element.firstChild(); !child.isNull(); child = child.nextSibling())
    {
        child.setAttribute(kg_char_num, l2s_char_num);
        UpdateLinkCharVariant(child);
    }
}

void CountCharVariant(QWebElement &element)
{
    long char_num = 0;
    long plain_text_length = element.toPlainText().length();
    long child_plain_text_length = 0;
    QString l2s_char_num;
    QString tag_name = element.tagName();

    if(!tag_name.compare("A") || !tag_name.compare("BUTTON") || !tag_name.compare("SELECT"))
    {
        l2s_char_num = QString::number(0, 10);
        element.setAttribute(kg_char_num, l2s_char_num);
    }
    else
    {
        if(element.firstChild().isNull())
        {
            l2s_char_num = QString::number(plain_text_length, 10);
            element.setAttribute(kg_char_num, l2s_char_num);
        }
        else
        {
            for(QWebElement child = element.firstChild(); !child.isNull(); child = child.nextSibling())
            {
                CountCharVariant(child);
            }
            for(QWebElement child = element.firstChild(); !child.isNull(); child = child.nextSibling())
            {
                char_num += child.attribute(kg_char_num).toLong();
                child_plain_text_length = child.toPlainText().length();
                plain_text_length -= child_plain_text_length;
            }
            char_num = char_num + plain_text_length;
            l2s_char_num = QString::number(char_num, 10);
            element.setAttribute(kg_char_num, l2s_char_num);
        }
    }
}

void ComputeTextDensityVariant(QWebElement &element)
{
    long char_num = element.attribute(kg_char_num).toLong();
    long tag_num = element.attribute(kg_tag_num).toLong();

    double text_density = 0.0;
    QString d2s_text_density;

    if(0 == char_num)
    {
        text_density = 0;
    }
    else
    {
        if(0 == tag_num)
        {
            tag_num = 1;
        }
        text_density = 1.0 * char_num / tag_num;
    }

    d2s_text_density = QString::number(text_density, 'f');
    element.setAttribute(kg_text_density, d2s_text_density);

    for(QWebElement child = element.firstChild(); !child.isNull(); child = child.nextSibling())
    {
        ComputeTextDensityVariant(child);
    }
}

void ComputeDensitySumVariant(QWebElement &element)
{
    double density_sum = 0.0;
    long char_num_sum = 0;
    long char_num = 0;
    QString d2s_density_sum;

    if(element.firstChild().isNull())
    {
        density_sum = element.attribute(kg_text_density).toDouble();
    }
    else
    {
        for(QWebElement child = element.firstChild(); !child.isNull(); child = child.nextSibling())
        {
            ComputeDensitySumVariant(child);
        }
        for(QWebElement child = element.firstChild(); !child.isNull(); child = child.nextSibling())
        {
            density_sum += child.attribute(kg_text_density).toDouble();
            char_num_sum += child.attribute(kg_char_num).toLong();
        }
        char_num = element.attribute(kg_char_num).toLong();
        if(char_num > char_num_sum)
        {
            char_num -= char_num_sum;
            density_sum += char_num;
        }
    }

    d2s_density_sum = QString::number(density_sum, 'f');
    element.setAttribute(kg_density_sum, d2s_density_sum);
}
