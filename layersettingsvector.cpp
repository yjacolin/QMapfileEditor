/**********************************************************************
 * $Id$
 *
 * Project: QMapfileEditor
 * Purpose: Settings of Vector Layer Object
 * Author: Pierre Mauduit / Yves Jacolin
 *
 **********************************************************************
 * Copyright (c) 2014, Yves Jacolin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies of this Software or works derived from this Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 ****************************************************************************/

#include "layersettingsvector.h"
#include "ui_layersettingsvector.h"

LayerSettingsVector::LayerSettingsVector(QWidget * parent, MapfileParser * mf) :
  QTabWidget(parent), ui(new Ui::LayerSettingsVector), mapfile(mf)
{
    ui->setupUi(this);

}


//SLOTS
void LayerSettingsVector::accept() {
    //this->saveLayerSettings();

    //QDialog::accept();
}
/** End SLOTS **/

LayerSettingsVector::~LayerSettingsVector() {
  delete ui;
}