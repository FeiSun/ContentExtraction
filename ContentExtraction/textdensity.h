/*
 * Dom Based Content Extraction via Text Density
 * Copyright (c) 2011 Sun Fei. All Rights Reserved.
 * Author: Ofey.sunfei@gmail.com (Sun Fei)
 * License: GPLv3 <http://www.gnu.org/copyleft/gpl.html>
 *
 * conut the character, tag, link character, link tag of the element
 * compute the text density and density's sum of the element; find the max densitysum
 */

#ifndef TEXTDENSITY_H
#define TEXTDENSITY_H

#include <foundation.h>

/**
* count the number of the character under the element
* @param element element need count
*/
void CountChar(QWebElement &element);

/**
* count the number of the tag under the element
* @param element element need count
*/
void CountTag(QWebElement &element);

/**
* update link character number
* @param element element need update
*/
void UpdateLinkChar(QWebElement &element);

/**
* count the number of the link character the element
* @param element element need count
*/
void CountLinkChar(QWebElement &element);

/**
* update link tag number
* @param element element need update
*/
void UpdateLinkTag(QWebElement &element);

/**
* count the number of the link tag the element
* @param element element need count
*/
void CountLinkTag(QWebElement &element);

/**
* compute the text density of the element
* @param element element need compute
* @param ratio the link ratio of body tag
*/
void ComputeTextDensity(QWebElement &element, double ratio);


/**
* compute the densitysum of sub-element under the element
* @param element element need compute
* @param ratio the link ratio of body tag
*/
void ComputeDensitySum(QWebElement &element, double ratio);

/**
* find the max densitysum under the element
* @param element element need compute
* @return the max value of the densitysum under the element
*/
double FindMaxDensitySum(QWebElement &element);

/**
* get threshold
* @param element element need compute
* @param max_density_sum the max value of the densitysum under the element
* @return the threshold
*/
double GetThreshold(QWebElement &element, double max_density_sum);

/**
* set the mark attribute of the element
* @param element element need compute
* @param mark the value of mark
*/
void SetMark(QWebElement &element, int mark);

/**
* find the element with max densitysum under the element
* @param element element need compute
* @param maxRatioSum the max value of the densitysum under the element
*/
void FindMaxDensitySumTag(QWebElement &element, double max_density_sum);

/**
* mark content
* @param element element need extracted
* @param threshold the threshold of the element's text density
*/
void MarkContent(QWebElement &element, double threshold);


//////////////////////////////////////////////////////////////////////////////////////
/////////another method: collapse the character count of link anchors to 0////////////
//////////////////////////////////////////////////////////////////////////////////////

/**
* update link character number
* @param element element need update
*/
void UpdateLinkCharVariant(QWebElement &element);

/**
* count the number of the character under the element in another way
* @param element element need count
*/
void CountCharVariant(QWebElement &element);

/**
* compute the text density of the element in another way
* @param element element need compute
*/
void ComputeTextDensityVariant(QWebElement &element);

/**
* compute the densitysum of sub-element under the element in another way
* @param element element need compute
*/
void ComputeDensitySumVariant(QWebElement &element);

#endif // TEXTDENSITY_H
