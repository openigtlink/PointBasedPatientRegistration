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

#include <map>

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
          qvtkDisconnect(fnode, vtkMRMLAnnotationFiducialNode::HierarchyModifiedEvent,
                         this, SLOT(onMRMLChildNodeHierarchyModified(vtkObject*)));
          qvtkDisconnect(fnode, vtkMRMLAnnotationFiducialNode::ValueModifiedEvent,
                         this, SLOT(onMRMLChildNodeValueModified(vtkObject*)));
          fnode->SetAttribute("RFTEvent", NULL);
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
        // Connect the fiducial node to onMRMLChildNodeHierarchyModified() and
        // onMRMLChildNodeValueModified().
        // An attribute "RFTEvent" is set "Yes" to mark that the fiducial node is connected. 
        qvtkConnect(fnode, vtkMRMLAnnotationFiducialNode::HierarchyModifiedEvent,
                    this, SLOT(onMRMLChildNodeHierarchyModified(vtkObject*)));
        qvtkConnect(fnode, vtkMRMLAnnotationFiducialNode::ValueModifiedEvent,
                    this, SLOT(onMRMLChildNodeValueModified(vtkObject*)));
        fnode->SetAttribute("RFTEvent", "Yes");
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
  Q_D(qSlicerRegistrationFiducialsTableModel);

  // Find the newly added node
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
      if (!fnode->GetAttribute("RFTEvent"))
        {
        qvtkConnect(fnode, vtkMRMLAnnotationFiducialNode::HierarchyModifiedEvent,
                    this, SLOT(onMRMLChildNodeHierarchyModified(vtkObject*)));
        qvtkConnect(fnode, vtkMRMLAnnotationFiducialNode::ValueModifiedEvent,
                    this, SLOT(onMRMLChildNodeValueModified(vtkObject*)));
        fnode->SetAttribute("RFTEvent", "Yes");
        }
      }
    }
  this->updateTable();
}


void qSlicerRegistrationFiducialsTableModel
::onMRMLChildNodeHierarchyModified(vtkObject* obj)
{
  Q_D(qSlicerRegistrationFiducialsTableModel);

  vtkMRMLAnnotationFiducialNode* fnode;
  fnode = vtkMRMLAnnotationFiducialNode::SafeDownCast(obj);

  // If the signal is from fiducla node, check if it is still
  // under d->HierarchyNode. If not, disconnect from the slots.
  if (fnode && d->HierarchyNode)
    {
    bool found = false;
    // TODO: Is there any method to obtain parent hierarchy node?
    //       Otherwise, we have to search from the hierarchy node
    vtkNew<vtkCollection> collection;
    d->HierarchyNode->GetDirectChildren(collection.GetPointer());
    int nItems = collection->GetNumberOfItems();
    collection->InitTraversal();
    for (int i = 0; i < nItems; i ++)
      {
      vtkMRMLAnnotationFiducialNode* p;
      p = vtkMRMLAnnotationFiducialNode::SafeDownCast(collection->GetNextItemAsObject());
      if (p && strcmp(p->GetID(), fnode->GetID()) == 0)
        {
        found = true;
        }
      }
    if (!found)
      {
      // The node is not a chlid of d->HierarchyNode anymore. Disconnect from
      // the slots.
      qvtkDisconnect(fnode, vtkMRMLAnnotationFiducialNode::HierarchyModifiedEvent,
                     this, SLOT(onMRMLChildNodeHierarchyModified(vtkObject*)));
      qvtkDisconnect(fnode, vtkMRMLAnnotationFiducialNode::ValueModifiedEvent,
                     this, SLOT(onMRMLChildNodeValueModified(vtkObject*)));
      fnode->SetAttribute("RFTEvent", NULL);
      }
    this->updateTable();
    }
}


void qSlicerRegistrationFiducialsTableModel
::onMRMLChildNodeValueModified(vtkObject* obj)
{
  Q_D(qSlicerRegistrationFiducialsTableModel);

  vtkMRMLAnnotationFiducialNode* fnode;
  fnode = vtkMRMLAnnotationFiducialNode::SafeDownCast(obj);

  this->updateTable();

}


