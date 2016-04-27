/*  ozw-admin - Gui for OpenZWave
 *    Copyright (C) 2016  Justin Hammond <justin@dynam.ac>
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QDebug>
#include "nodes.h"
#include "Manager.h"


Node::Node(qint8 m_nodeid, int homeid)
{

    this->m_nodeid = m_nodeid;
    this->m_homeid = homeid;
}





NodeList::NodeList(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int NodeList::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return this->m_Nodelist.size();
}
int NodeList::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return NCN_Count;
}
QVariant NodeList::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (index.row() >= this->m_Nodelist.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {
        Node *node = this->m_Nodelist.at(index.row());
        if (!node) {
            return QVariant();
        }
        switch ((NodeColumnNames)index.column()) {
            case NCN_NodeID:
                return node->getNodeID();
            case NCN_NodeName:
                return node->getNodeName();
            case NCN_NodeLocation:
                return node->getNodeLocation();
            case NCN_NodeManufacturerName:
                return node->getNodeManufacturer();
            case NCN_NodeProductName:
                return node->getNodeProduct();
            case NCN_NodeBasicType:
                return node->getNodeBasicType();
            case NCN_NodeGenericType:
                return node->getNodeGenericType();
        }
    }
    return QVariant();

}
QVariant NodeList::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch ((NodeColumnNames)section) {
            case NCN_NodeID:
                return tr("NodeID");

            case NCN_NodeName:
                return tr("Node Name");

            case NCN_NodeLocation:
                return tr("Location");

            case NCN_NodeManufacturerName:
                return tr("Manufacturer");

            case NCN_NodeProductName:
                return tr("Product");

            case NCN_NodeBasicType:
                return tr("Basic Type");

            case NCN_NodeGenericType:
                return tr("Generic Type");

        }
    }
    return QVariant();
}
Qt::ItemFlags NodeList::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
bool NodeList::setData(const QModelIndex &index, const QVariant &value, int role) {

    if (index.isValid() && role == Qt::EditRole) {
            int row = index.row();

            Node *p = this->m_Nodelist.at(row);

            switch ((NodeColumnNames)index.column()) {
                case NCN_NodeID:
                case NCN_NodeManufacturerName:
                case NCN_NodeProductName:
                case NCN_NodeBasicType:
                case NCN_NodeGenericType:
                    /* read only */
                    return false;
                    break;
                case NCN_NodeName:
                    p->setNodeName(value.toString());
                    break;
                case NCN_NodeLocation:
                    p->setNodeLocation(value.toString());
                    break;
            }
    } else {
        return false;
    }
    emit(dataChanged(index, index));

    return true;
}

bool NodeList::insertRows(int position, int rows, const QModelIndex &index) {

    return false;
}

bool NodeList::removeRows(int position, int rows, const QModelIndex &index) {

    return false;
}





void NodeList::addNode(Node *node) {
    this->m_Nodelist.append(node);
    beginInsertRows(QModelIndex(), this->m_Nodelist.size(), this->m_Nodelist.size());
    endInsertRows();
}

Node* NodeList::getNode(qint8 nodeid) {
    QListIterator<Node *> i(this->m_Nodelist);
    while (i.hasNext()) {
        Node *node = i.next();
        if (node->getNodeID() == nodeid)
            return node;
    }
    return NULL;
}

QModelIndex NodeList::getNodeValueIndex(qint8 nodeid, NodeColumnNames value) {
    int j = 0;
    QListIterator<Node *> i(this->m_Nodelist);
    while (i.hasNext()) {
        Node *node = i.next();
        if (node->getNodeID() == nodeid)
            break;
        j++;
    }
    return this->index(j, (int)value);
}
