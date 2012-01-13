/*
 * Dom Based Content Extraction via Text Density
 * Copyright (c) 2011 Sun Fei. All Rights Reserved.
 * Author: Ofey.sunfei@gmail.com (Sun Fei)
 * License: GPLv3 <http://www.gnu.org/copyleft/gpl.html>
 *
 * Uses the information for DOM to delete noisy DOM node
 */

#ifndef CLEAN_H
#define CLEAN_H

#include "foundation.h"

/**
* delete the invisible element in DOM tree
* @param element the element need clean
*/
void PreprocessDOM(QWebElement &element);

/**
* Remove the attribute that appended by the programme
* @param element the element need clean
*/
void RemoveAttribute(QWebElement &element);

/**
* Remove the attribute that appended by the programme
* @param element the element need clean
*/
void RemoveAttributeVariant(QWebElement &element);

/**
* Delete the noisy element under the element by the mark
* @param element the element need clean
*/
void CleanTreeByMark(QWebElement &element);

#endif // CLEAN_H
