/*==============================================================================

  Program: Point-based Registration User Interface for 3D Slicer

  Copyright (c) Brigham and Women's Hospital

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Junichi Tokuda, Brigham and Women's
  Hospital. The project was supported by NIH P41EB015898.

==============================================================================*/

// RegistrationFiducialsPanel Widgets includes
#include "qSlicerRegistrationFiducialsTableModel.h"

#include "vtkMRMLAnnotationHierarchyNode.h"
#include "vtkMRMLAnnotationFiducialNode.h"

#include "vtkNew.h"
#include "vtkCollection.h"

class Q_SLICER_MODULE_POINTBASEDPATIENTREGISTRATION_WIDGETS_EXPORT qSlicerRegistrationFiducialsTableModelPrivate
{
  Q_DECLARE_PUBLIC(qSlicerRegistrationFiducialsTableModel);
 protected:
  qSlicerRegistrationFiducialsTableModel * const q_ptr;

 public:
  qSlicerRegistrationFiducialsTableModelPrivate(
    qSlicerRegistrationFiducialsTableModel& object);
  virtual ~qSlicerRegistrationFiducialsTableModelPrivate();

  void init();

  vtkMRMLAnnotationHierarchyNode* HierarchyNode;

};

qSlicerRegistrationFiducialsTableModelPrivate
::qSlicerRegistrationFiducialsTableModelPrivate(
  qSlicerRegistrationFiducialsTableModel& object)
  : q_ptr(&object)
{
  this->HierarchyNode = NULL;
}

qSlicerRegistrationFiducialsTableModelPrivate
::~qSlicerRegistrationFiducialsTableModelPrivate()
{
  //Q_D(qSlicerRegistrationFiducialsTableModel);
}


void qSlicerRegistrationFiducialsTableModelPrivate
::init()
{
  Q_Q(qSlicerRegistrationFiducialsTableModel);
  q->setColumnCount(4);
  q->setHorizontalHeaderLabels(QStringList()
                               << "Point #"
                               << "R"
                               << "A"
                               << "S");
  QObject::connect(q, SIGNAL(itemChanged(QStandardItem*)),
                   q, SLOT(onItemChanged(QStandardItem*)),
                   Qt::UniqueConnection);
}


qSlicerRegistrationFiducialsTableModel
::qSlicerRegistrationFiducialsTableModel(QObject *parent)
//: QAbstractTableModel(parent)
  : QStandardItemModel(parent)
  , d_ptr( new qSlicerRegistrationFiducialsTableModelPrivate(*this) )
{
  Q_D(qSlicerRegistrationFiducialsTableModel);
  d->init();
}

qSlicerRegistrationFiducialsTableModel
::~qSlicerRegistrationFiducialsTableModel()
{
}

qSlicerRegistrationFiducialsTableModel
::qSlicerRegistrationFiducialsTableModel(qSlicerRegistrationFiducialsTableModelPrivate* pimpl, QObject *parent)
//: QAbstractTableModel(parent)
  : QStandardItemModel(parent)
  , d_ptr(pimpl)
{
  Q_D(qSlicerRegistrationFiducialsTableModel);
  d->init();
}


  
//int qSlicerRegistrationFiducialsTableModel
//::rowCount(const QModelIndex &parent) const
//{
//  Q_D(qSlicerRegistrationFiducialsTableModel);
//
//  if (d->HierarchyNode)
//    {
//    vtkNew<vtkCollection> collection;
//    d->HierarchyNode->GetDirectChildren(collection);
//    int n = collection->GetNumberOfItems();
//    int nFiducials = 0;
//    if (int i = 0; i < n; i ++)
//      {
//      vtkMRMLAnnotationFiducialNode* fnode;
//      fnode = vtkMRMLAnnotationFiducialNode::SafeDownCast(collection->GetNextItemAsObject());
//      if (fnode)
//        nFiducials ++;
//      }
//    }
//}


//QVariant qSlicerRegistrationFiducialsTableModel
//::headerData(int section, Qt::Orientation orientation, int role) const
//{
//  if (role != Qt::DisplayRole)
//    return QVariant();
//  
//  if (orientation == Qt::Horizontal) {
//    switch (section) {
//    case 0:
//      return tr("Point #");
//      
//    case 1:
//      return tr("R");
//
//    case 2:
//      return tr("A");
//
//    case 3:
//      return tr("S");
//      
//    default:
//      return QVariant();
//    }
//  }
//  return QVariant();
//}


//Qt::ItemFlags qSlicerRegistrationFiducialsTableModel
//::flags(const QModelIndex &index) const
//{
//}


bool qSlicerRegistrationFiducialsTableModel
::setData(const QModelIndex &index, const QVariant &value, int role)
{
}


bool qSlicerRegistrationFiducialsTableModel
::insertRows(int position, int rows, const QModelIndex &index)
{
}


bool qSlicerRegistrationFiducialsTableModel
::removeRows(int position, int rows, const QModelIndex &index)
{
}


void qSlicerRegistrationFiducialsTableModel
::setNode(vtkMRMLNode* node)
{
  Q_D(qSlicerRegistrationFiducialsTableModel);

  if (node == NULL)
    {
    d->HierarchyNode = NULL;
    }
  
  vtkMRMLAnnotationHierarchyNode* hnode;
  hnode = vtkMRMLAnnotationHierarchyNode::SafeDownCast(node);
  if (hnode)
    {
    qvtkReconnect(d->HierarchyNode, node,
                  vtkMRMLHierarchyNode::ChildNodeAddedEvent,
                  this, SLOT(onMRMLChildNodeAdded()));
    qvtkReconnect(d->HierarchyNode, node,
                  vtkMRMLHierarchyNode::ChildNodeRemovedEvent,
                  this, SLOT(onMRMLChildNodeRemoved()));
    // child node change ?
    d->HierarchyNode = hnode;
    }

  this->updateNode();
}


//------------------------------------------------------------------------------
void qSlicerRegistrationFiducialsTableModel
::updateNode()
{
  Q_D(qSlicerRegistrationFiducialsTableModel);

  if (d->HierarchyNode == 0)
    {
    this->setRowCount(0);
    return;
    }

  QObject::disconnect(this, SIGNAL(itemChanged(QStandardItem*)),
                      this, SLOT(onItemChanged(QStandardItem*)));

  // Count the number of child Fiducial nodes 
  vtkNew<vtkCollection> collection;
  d->HierarchyNode->GetDirectChildren(collection.GetPointer());
  int n = collection->GetNumberOfItems();
  int nFiducials = 0;
  collection->InitTraversal();
  for (int i = 0; i < n; i ++)
    {
    vtkMRMLAnnotationFiducialNode* fnode;
    fnode = vtkMRMLAnnotationFiducialNode::SafeDownCast(collection->GetNextItemAsObject());
    if (fnode)
      {
      nFiducials ++;
      }
    }
  this->setRowCount(nFiducials);

  collection->InitTraversal();
  for (int i = 0; i < nFiducials; i ++)
    {
    vtkMRMLAnnotationFiducialNode* fnode;
    fnode = vtkMRMLAnnotationFiducialNode::SafeDownCast(collection->GetNextItemAsObject());
    if (fnode)
      {
      QStandardItem* item = this->invisibleRootItem()->child(i, 0);
      item->setText(fnode->GetName());
      item->setData(QVariant(),Qt::SizeHintRole);

      for (int j = 0; j < 3; j ++)
        {
        QStandardItem* item = this->invisibleRootItem()->child(i, j+1);
        QString str;
        str.setNum(fnode->GetFiducialCoordinates()[j]);
        item->setText(str);
        item->setData(QVariant(),Qt::SizeHintRole);
        }
      }
    }

  QObject::connect(this, SIGNAL(itemChanged(QStandardItem*)),
                   this, SLOT(onItemChanged(QStandardItem*)),
                   Qt::UniqueConnection);
}


void qSlicerRegistrationFiducialsTableModel
::updateFiducialsFromItem(QStandardItem* item)
{
  item->row();
  //switch(item->column())
  //  {
  //  case 0:
  //    
  //    break;
  //  case 1:
  //    break;
  //  case 2:
  //    break;
  //  case 3:
  //    break;
  //  }
}

void qSlicerRegistrationFiducialsTableModel
::onItemChanged(QStandardItem * item)
{
  if (item == this->invisibleRootItem())
    {
    return;
    }
  this->updateFiducialsFromItem(item);
}


void qSlicerRegistrationFiducialsTableModel
::onMRMLChildNodeAdded()
{
//  if (d->HierarchyNode = node)
//    {
//    emit(dataChanged(index, index));
//    }
}


void qSlicerRegistrationFiducialsTableModel
::onMRMLChildNodeRemoved()
{
}

void qSlicerRegistrationFiducialsTableModel
::onMRMLChildNodeModified()
{
  
}



