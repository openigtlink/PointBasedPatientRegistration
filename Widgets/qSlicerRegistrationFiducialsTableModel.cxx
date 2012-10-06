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
                               << "Point Name"
                               << "R"
                               << "A"
                               << "S");
  QObject::connect(q, SIGNAL(itemChanged(QStandardItem*)),
                   q, SLOT(onItemChanged(QStandardItem*)),
                   Qt::UniqueConnection);
}


qSlicerRegistrationFiducialsTableModel
::qSlicerRegistrationFiducialsTableModel(QObject *parent)
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
  : QStandardItemModel(parent)
  , d_ptr(pimpl)
{
  Q_D(qSlicerRegistrationFiducialsTableModel);
  d->init();
}


bool qSlicerRegistrationFiducialsTableModel
::setData(const QModelIndex &index, const QVariant &value, int role)
{
  return true;
}


bool qSlicerRegistrationFiducialsTableModel
::insertRows(int position, int rows, const QModelIndex &index)
{
  return true;
}


bool qSlicerRegistrationFiducialsTableModel
::removeRows(int position, int rows, const QModelIndex &index)
{
  return true;
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

    // Disconnect slots from old child nodes
    if (d->HierarchyNode)
      {
      vtkNew<vtkCollection> collection;
      d->HierarchyNode->GetDirectChildren(collection.GetPointer());
      int nItems = collection->GetNumberOfItems();
      collection->InitTraversal();
      for (int i = 0; i < nItems; i ++)
        {
        vtkMRMLAnnotationFiducialNode* fnode;
        fnode = vtkMRMLAnnotationFiducialNode::SafeDownCast(collection->GetNextItemAsObject());
        if (fnode)
          {
          qvtkDisconnect(fnode, vtkMRMLAnnotationFiducialNode::ValueModifiedEvent,
                         this, SLOT(onMRMLChildNodeModified()));
          }
        }
      }

    vtkNew<vtkCollection> collection;
    // Connect slots to handle chlid node event
    hnode->GetDirectChildren(collection.GetPointer());
    int nItems = collection->GetNumberOfItems();
    collection->InitTraversal();
    for (int i = 0; i < nItems; i ++)
      {
      vtkMRMLAnnotationFiducialNode* fnode;
      fnode = vtkMRMLAnnotationFiducialNode::SafeDownCast(collection->GetNextItemAsObject());
      if (fnode)
        {
        qvtkConnect(fnode, vtkMRMLAnnotationFiducialNode::ValueModifiedEvent,
                       this, SLOT(onMRMLChildNodeModified()));
        }
      }

    d->HierarchyNode = hnode;
    }

  this->updateTable();
}


//------------------------------------------------------------------------------
void qSlicerRegistrationFiducialsTableModel
::updateTable()
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
  int nItems = collection->GetNumberOfItems();
  int nFiducials = 0;
  collection->InitTraversal();
  for (int i = 0; i < nItems; i ++)
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
  for (int i = 0; i < nItems; i ++)
    {
    vtkMRMLAnnotationFiducialNode* fnode;
    fnode = vtkMRMLAnnotationFiducialNode::SafeDownCast(collection->GetNextItemAsObject());
    if (fnode)
      {
      QStandardItem* item = this->invisibleRootItem()->child(i, 0);
      if (item == NULL)
        {
        item = new QStandardItem();
        this->invisibleRootItem()->setChild(i, 0, item);
        }
      item->setText(fnode->GetName());
      item->setData(QVariant(),Qt::SizeHintRole);

      for (int j = 0; j < 3; j ++)
        {
        QStandardItem* item = this->invisibleRootItem()->child(i, j+1);
        if (item == NULL)
          {
          item = new QStandardItem();
          this->invisibleRootItem()->setChild(i, j+1, item);
          }
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
  this->updateTable();
}


void qSlicerRegistrationFiducialsTableModel
::onMRMLChildNodeRemoved()
{
  this->updateTable();
}

void qSlicerRegistrationFiducialsTableModel
::onMRMLChildNodeModified()
{
  this->updateTable();
}



