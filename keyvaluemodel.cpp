#include "keyvaluemodel.h"


KeyValueModel::KeyValueModel(QObject * parent): 
    QAbstractTableModel(parent),
    m_data(QHash<QString,QString>()) {

}

KeyValueModel::~KeyValueModel() {}

int KeyValueModel::rowCount(const QModelIndex & parent) const {
  Q_UNUSED(parent);
  return m_data.keys().size();
}

int KeyValueModel::columnCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return KeyValueModel::Value + 1;
}

QVariant KeyValueModel::headerData (int section, Qt::Orientation orientation, int role) const {
  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Vertical)
    return QVariant(section);

  if (section == KeyValueModel::Key)
    return QVariant(QObject::tr("Key"));

  else if (section == KeyValueModel::Value)
    return QVariant(QObject::tr("Value"));

  return QVariant(section);
}

void KeyValueModel::setData(QHash<QString,QString> const & kv) {
  beginResetModel();
  m_data = kv;
  endResetModel();
}

QHash<QString,QString> const & KeyValueModel::getData(void) const {
  return m_data;
}

void KeyValueModel::addData(QString const & k, QString const &v) {
  // It might appear overkill to reset entirely the model
  // but the interface is not meant to contain hundreds of rows.
  beginResetModel();
  m_data.insert(k,v);
  endResetModel();
}

void KeyValueModel::removeData(QString const &k) {
  beginResetModel();
  m_data.remove(k);
  endResetModel();
}


QVariant KeyValueModel::data(const QModelIndex &index, int role) const {

  if (! index.isValid()) {
    return QVariant();
  }

  if (role != Qt::DisplayRole)
    return QVariant();

  if (index.row() >= m_data.keys().size())
    return QVariant();

  if (index.column() > KeyValueModel::Value)
    return QVariant();

  QString key = m_data.keys().at(index.row());

  if (index.column() == KeyValueModel::Key) {
    return key;
  }
  else if (index.column() == KeyValueModel::Value) {
    return m_data.value(key);
  }

  return QVariant();
}


