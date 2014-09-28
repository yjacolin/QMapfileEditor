#include "mainwindow.h"
#include "ui_mainwindow.h"

// private

void MainWindow::reinitMapfile() {
  // if a MapSettings window has been opened, closes and destroys it
  if (this->settings) {
    this->settings->close();
    delete this->settings;
    this->settings = NULL;
  }

  // deletes / restores the model for layer tree
  // (should also delete every children)
  delete this->mfStructureModel;
  // frees objects related to mapfileparser
  delete this->mapfile;

  // then recreates the UI elements
  this->mfStructureModel = new QStandardItemModel();
  this->layersItem = new QStandardItem(tr("Layers"));

  this->layersItem->setEditable(false);
  this->mfStructureModel->appendRow(this->layersItem);
  ui->mf_structure->setModel(this->mfStructureModel);

  this->mapfile = new MapfileParser(QString());

  // re-init map preview
  this->mapScene->clear();
}

// public

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  // init default mapfile structure model
  this->showInfo(tr("Initializing default mapfile"));

  this->mfStructureModel = new QStandardItemModel();
  this->layersItem = new QStandardItem(tr("Layers"));
  this->mfStructureModel->appendRow(this->layersItem);
  ui->mf_structure->setModel(this->mfStructureModel);

  this->mapScene = new QGraphicsScene(this);
  ui->mf_preview->setScene(this->mapScene);

  // connects extra actions
  this->showInfo("Activate actions");
  this->connect(ui->actionNew, SIGNAL(triggered()), SLOT(newMapfile()));
  this->connect(ui->actionOpen, SIGNAL(triggered()), SLOT(openMapfile()));
  this->connect(ui->actionSave, SIGNAL(triggered()), SLOT(saveMapfile()));
  this->connect(ui->actionSaveAs, SIGNAL(triggered()), SLOT(saveAsMapfile()));
  this->connect(ui->actionMapSetting, SIGNAL(triggered()), SLOT(showMapSettings()));
  this->connect(ui->actionAbout, SIGNAL(triggered()), SLOT(showAbout()));


  //creates a default empty mapfileparser
  this->mapfile = new MapfileParser(QString());
  this->showInfo(tr("Initialisation process: success !"));
}

void MainWindow::newMapfile()
{
  // check if a mapfile is already opened
  if ((this->mapfile) &&  (this->mapfile->isLoaded())) {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Currently editing an existing mapfile", "Discard current modifications ?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
      this->reinitMapfile();
    }
    else {
      return;
    }
  }
}

void MainWindow::openMapfile()
{
  QString prevFilePath = QDir::homePath();

  // TODO: if modifications made on a new / opened mapfile, warns the user

  QString fileName = QFileDialog::getOpenFileName(this, tr("Open map File"), prevFilePath, tr("Map file (*.map)"));

  // open file dialog has been discarded (escape)
  if (fileName.isEmpty()) {
    return;
  }

  // Reinit / free objects if necessary
  if (this->mapfile) {
    this->reinitMapfile();
  }

  this->mapfile = new MapfileParser(fileName);

  if (! this->mapfile->isLoaded()) {
    QMessageBox::critical(
        this,
        "QMapfileEditor",
        tr("Error occured while loading the mapfile.")
        );
    this->reinitMapfile();
    this->showInfo(tr("Mapfile openend with success."));
    return;
  }

  QStringList layers = this->mapfile->getLayers();
  for (int i = 0; i < layers.size(); ++i) {
    QStandardItem * si = new QStandardItem(layers.at(i));
    si->setEditable(false);
    layersItem->appendRow(si);
  }

  ui->mf_structure->expandAll();


  // rendering the map preview
  QPixmap mapRepr = QPixmap();
  unsigned char * mapImage = this->mapfile->getCurrentMapImage();
  int mapImageSize = this->mapfile->getCurrentMapImageSize();

  mapRepr.loadFromData(mapImage, mapImageSize);

  free(mapImage);
  this->mapScene->addPixmap(mapRepr);
}

/**
 * Displays the map settings window.
 */
void MainWindow::showMapSettings() {
  // Mapfile not loaded
  if ((! this->mapfile) || (! this->mapfile->isLoaded())) {
    return;
  }
  // a window has alrady been created
  if (this->settings) {
    this->settings->show();
    return;
  }
  this->settings = new MapSettings(this, this->mapfile);
  this->settings->show();
}


void MainWindow::showAbout() { 
    QDialog* aboutDialog = new QDialog (this);
    aboutDialog->setWindowFlags( Qt::WindowCancelButtonHint | Qt::WindowSystemMenuHint | Qt::Dialog | Qt::WindowCloseButtonHint );
    aboutDialog->setWindowTitle(tr("About"));
    aboutDialog->setFixedSize(400,300);
    aboutDialog->setModal(true);
    QGridLayout* layout = new QGridLayout (aboutDialog);
    QLabel *content = new QLabel("<h1><b>About</b></h1><h2>Author:</h2><ul><li>Yves Jacolin</li><li>Pierre Mauduit</li></ul><h2>Licence:<h2><h2>Links:</h2><ul><li><a href='http://mapserver.org'>MapServer Project</a></li><li><a href=''>Sheetah</a></li></ul>");
    content->setTextFormat(Qt::RichText);
    //content->setText(tr("<h1><b>About</b></h1><h2>Author:</h2><h2>Licence:<h2><h2>Links:</h2>"));
    layout->addWidget(content, 0, 0);
    aboutDialog->setLayout(layout);
    aboutDialog->show();
}

void MainWindow::saveMapfile()
{
  if (this->mapfile) {
    // if this is a new mapfile, calls saveAsMapfile instead
    if (this->mapfile->isNew()) {
      this->saveAsMapfile();
    } else {
      this->mapfile->saveMapfile(QString());
    }
  }
}

void MainWindow::saveAsMapfile()
{
  QString prevFilePath = QDir::homePath();

  if (this->mapfile) {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save map File"), prevFilePath, tr("Map file (*.map)")); 
    if (fileName.isEmpty()) {
      return;
    }
    if (! this->mapfile->saveMapfile(fileName)) {
      QMessageBox::critical(this, "QMapfileEditor", tr("Error occured while saving the mapfile."));
    }
    return;
  }
}

void MainWindow::showInfo( QString message )
{
   //TODO: if statusBar is on so:
   ui->statusbar->showMessage(message);
   return;
}

MainWindow::~MainWindow()
{
  if (this->mapfile) {
    delete this->mapfile;
  }

  // children items are also destroyed
  delete this->mfStructureModel;

  if (this->settings) {
    delete this->settings;
  }
  // TODO: since mapScene is added to an UI element,
  // is there a risk of double free here ?
  if (this->mapScene) {
    delete this->mapScene;
  }
  // This *should* destroy the children objects
  delete ui;
}
