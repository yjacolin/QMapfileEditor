#include "outputformat.h"


OutputFormat::OutputFormat(const QString & name,
                const QString &mimeType,
                const QString & driver,
                const QString & extension,
                const int  & imageMode,
                const bool & transparent,
                const enum State & state) :
name(name),
mimeType(mimeType),
driver(driver),
extension(extension),
imageMode(imageMode),
transparent(transparent),
state(state) {}

QString const & OutputFormat::getName()        const { return name;        }
QString const & OutputFormat::getMimeType()    const { return mimeType;    }
QString const & OutputFormat::getDriver()      const { return driver;      }
QString const & OutputFormat::getExtension()   const { return extension;   }
int     const & OutputFormat::getImageMode()   const { return imageMode;   }
bool    const & OutputFormat::getTransparent() const { return transparent; }

QHash<QString, QString> const & OutputFormat::getFormatOptions() const { return formatOptions; }

bool OutputFormat::isEmpty() { return name.isEmpty(); };

void OutputFormat::addFormatOption(const QString &k, const QString &v) {
   formatOptions.insert(k,v);
}

void OutputFormat::removeFormatOption(const QString &k) {
  formatOptions.remove(k);
}

void OutputFormat::setName(QString const &v)      { name        = v; }
void OutputFormat::setMimeType(QString const &v)  { mimeType    = v; }
void OutputFormat::setDriver(QString const &v)    { driver      = v; }
void OutputFormat::setExtension(QString const &v) { extension   = v; }
void OutputFormat::setImageMode(int const &v)     { imageMode   = v; }
void OutputFormat::setTransparent(bool const &v)  { transparent = v; }
void OutputFormat::setState(enum State const &v)  { state       = v; }

/** related to OutputFormat Model (representation into the UI) */

OutputFormatsModel::OutputFormatsModel(QObject * parent) : QAbstractListModel(parent) {}

OutputFormatsModel::~OutputFormatsModel() {
  for (int i = 0; i < entries.count(); ++i) {
    delete entries.at(i);
  }
}

int OutputFormatsModel::rowCount(const QModelIndex & parent) const {
  Q_UNUSED(parent);
  return entries.count();
}

int OutputFormatsModel::columnCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return OutputFormatsModel::Transparent + 1;
}

QVariant OutputFormatsModel::headerData (int section, Qt::Orientation orientation, int role) const {
  Q_UNUSED(section);
  Q_UNUSED(orientation);
  Q_UNUSED(role);
  if (role != Qt::DisplayRole)
    return QVariant();
  return QVariant(QObject::tr("Output formats"));
}

void OutputFormatsModel::setEntries(QList<OutputFormat *> const & items) {
  entries = items;
}

QList<OutputFormat *> const & OutputFormatsModel::getEntries(void) const {
  return entries;
}

OutputFormat * OutputFormatsModel::getOutputFormat(const QModelIndex &index) const {
  if (index.row() < 0 || index.row() > entries.size())
    return NULL;
  return entries.at(index.row());
}

QVariant OutputFormatsModel::data(const QModelIndex &index, int role) const {
  if ((role != Qt::DisplayRole) && (role != Qt::EditRole))
    return QVariant();
  if (index.row() > entries.size())
    return QVariant();

  OutputFormat * of = entries.at(index.row());

  if (of == NULL)
    return QVariant();

  switch (index.column()) {
    case OutputFormatsModel::Name:
      return QVariant(of->getName());
    case OutputFormatsModel::MimeType:
      return QVariant(of->getMimeType());
    case OutputFormatsModel::Driver:
      return QVariant(of->getDriver());
    case OutputFormatsModel::Extension:
      return QVariant(of->getExtension());
    case OutputFormatsModel::ImageMode:
      return QVariant(of->getImageMode());
    case OutputFormatsModel::Transparent:
      return QVariant(of->getTransparent());
  }
  return QVariant();
}

/** format options model */
OutputFormatOptionsModel::OutputFormatOptionsModel(QObject * parent) : QAbstractListModel(parent) {}

OutputFormatOptionsModel::~OutputFormatOptionsModel() {}

#include <iostream>
int OutputFormatOptionsModel::rowCount(const QModelIndex & parent) const {
  Q_UNUSED(parent);
  std::cout << "into rowCount, " << this->entries.keys().size() << " rows" <<  std::endl;
  return 4;
  return this->entries.keys().size();
}

int OutputFormatOptionsModel::columnCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return OutputFormatOptionsModel::Value + 1;
}

//QVariant OutputFormatOptionsModel::headerData (int section, Qt::Orientation orientation, int role) const {
//  if ((role != Qt::DisplayRole) && (role != 1) && (role != 6) && (role != 7) && (role != 8) && (role != 9) && (role != 13))
//    return QVariant();
//  if (orientation == Qt::Vertical)
//    return QVariant(section);
//  if (section == OutputFormatOptionsModel::Key)
//    return QVariant(QObject::tr("Key"));
//  else if (section == OutputFormatOptionsModel::Value)
//    return QVariant(QObject::tr("Value"));
//
//  return QVariant(section);
//}

void OutputFormatOptionsModel::setEntries(QHash<QString, QString> items) {
  entries = items;
  emit dataChanged (QAbstractItemModel::createIndex(0,0), QAbstractItemModel::createIndex(entries.keys().size(), 1));
}

QHash<QString, QString> const & OutputFormatOptionsModel::getEntries(void) const {
  return entries;
}

QVariant OutputFormatOptionsModel::data(const QModelIndex &index, int role) const {

  if ((role != Qt::DisplayRole) && (role != Qt::EditRole))
    return QVariant();
  if (index.row() >= entries.keys().size())
    return QVariant();

  QString key = entries.keys().at(index.row());
  if (index.column() == OutputFormatOptionsModel::Key)
    return QVariant(key);
  else if (index.column() == OutputFormatOptionsModel::Value)
    return QVariant(entries.value(key));

  return QVariant();
}




