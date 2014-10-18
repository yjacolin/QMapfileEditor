#include <QApplication>
#include <QtCore/QDebug>

#include "qgisimporter.h"

int main(int argc, char ** argv) {
   qDebug() << "QGis Importer test launched";

   QGisImporter *imp = NULL;

   QApplication(argc, argv);

   if (argc > 1) {
     imp = new QGisImporter(argv[1]);
     MapfileParser * p = imp->importMapFile();
   } else {
     qDebug() << "Usage: " << argv[0] <<  "<QGis project>";
   }


   qDebug() << "QGis importer test about to end";

   if (imp)
     delete imp;

  return 0;
}
