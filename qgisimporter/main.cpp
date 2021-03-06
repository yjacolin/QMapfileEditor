/**********************************************************************
 * $Id$
 *
 * Project: QMapfileEditor
 * Purpose: 
 * Author: Pierre Mauduit
 *
 **********************************************************************
 * Copyright (c) 2014, Pierre Mauduit
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

#include <unistd.h>

#include <QApplication>
#include <QtCore/QDebug>
#include <QTemporaryFile>
#include <QTextStream>

#include "ogrsf_frmts.h"

#include "qgisimporter.h"

int main(int argc, char ** argv) {

  //GDALAllRegister();
   OGRRegisterAll();

   qDebug() << "QGis Importer test launched";

   QGisImporter *imp = NULL;

   QApplication(argc, argv);


   MapfileParser * p = NULL;
   if (argc > 1) {
     imp = new QGisImporter(argv[1]);
   } else {
     qDebug() << "Usage: " << argv[0] <<  "<QGIS project> [optional destination mapfile]";
     return 0;
   }

   p = imp->importMapFile();

   if (! p) {
    qDebug() << "Error parsing mapfile "<< argv[1];
    if (imp)
      delete imp;
    return 1;
   }
   // Touch'ing a temporary file
   //
   // Doing so, because the Mapserver API to save a mapfile into a char * is
   // fiddling with stdout, I don't feel it very secure in term of race
   // condition, if another thread ever pollutes the standard output during
   // dumping the memory object.
   //
   // See mapfile.c: char* msWriteMapToString(mapObj *map)
   //
   QTemporaryFile * f = new QTemporaryFile();
   f->open();
   f->close();
   qDebug() << "";

   p->saveMapfile(f->fileName());

   // reopens the file
   f->open();
   QTextStream in(f);
   qDebug() << in.readAll().toStdString().c_str();
   f->close();

   if (argc > 2) {
     unlink(argv[2]);
     f->copy(argv[2]);
   }

   if (f)
     delete f;
   if (imp)
     delete imp;

  qDebug() << "QGIS importer test about to end";
  return 0;
}
