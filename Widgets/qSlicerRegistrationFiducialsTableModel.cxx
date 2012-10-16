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
#include "vtkMRMLScene.h"

#include "vtkNew.h"
#include "vtkCollection.h"

#include <map>
#include <sstream>

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

  int PendingItemModified; // -1 means not updating

  vtkMRMLScene* Scene;
};

qSlicerRegistrationFiducialsTableModelPrivate
::qSlicerRegistrationFiducialsTableModelPrivate(
  qSlicerRegistrationFiducialsTableModel& object)
  : q_ptr(&object)
{
  this->HierarchyNode = NULL;
  this->PendingItemModified = -1; // -1 means not updating
  this->Scene = NULL;
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
                   q, SLOT(onItemChanged(QStandardItem*)));

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
                  this, SLOT(onMRMLChildNodeAdded(vtkObject*)));
    qvtkReconnect(d->HierarchyNode, node,
                  vtkMRMLHierarchyNode::ChildNodeRemovedEvent,
                  this, SLOT(onMRMLChildNodeRemoved(vtkObject*)));
    // NOTE (10/13/2012): ChildNodeRemovedEvent works when a child node is moved
    // to another annotation hierarchy, but doesn't work when a child
    // node is removed. For this reason,in addition to ChildNodeRemovedEvent,
    // onMRMLNodeRemovedEvent() is connected to NodeRemovedEvent invoked by the scene.

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
        // Connect the fiducial node to onMRMLChildNodeValueModified().
        // An attribute "RFTEvent" is set "Yes" to mark that the fiducial node is connected. 
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
::setCoordinateLabel(int m)
{
  Q_D(qSlicerRegistrationFiducialsTableModel);

  QStringList list;
  
  switch (m)
    {
    case LABEL_RAS:
      {
      list << "Point Name" << "R" << "A" << "S";
      break;
      }
    case LABEL_XYZ:
      {
      list << "Point Name" << "X" << "Y" << "Z";
      break;
      }
    default:
      {
      return;
      break;
      }
    }
  this->setHorizontalHeaderLabels(list);
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

  d->PendingItemModified = 0;


  //QObject::disconnect(this, SIGNAL(itemChanged(QStandardItem*)),
  //                    this, SLOT(onItemChanged(QStandardItem*)));

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
      //item->setData(QVariant(),Qt::SizeHintRole);
      item->setData(fnode->GetID(),qSlicerRegistrationFiducialsTableModel::NodeIDRole);

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
        //item->setData(QVariant(),Qt::SizeHintRole);
        }
      }
    }

  //QObject::connect(this, SIGNAL(itemChanged(QStandardItem*)),
  //this, SLOT(onItemChanged(QStandardItem*)));
  d->PendingItemModified = -1;

}


//-----------------------------------------------------------------------------
void qSlicerRegistrationFiducialsTableModel
::addPoint(double x, double y, double z)
{
  Q_D(qSlicerRegistrationFiducialsTableModel);

  if (d->Scene && d->HierarchyNode)
    {
    // Generate fiducial point name
    vtkNew<vtkCollection> collection;
    d->HierarchyNode->GetDirectChildren(collection.GetPointer());
    int nItems = collection->GetNumberOfItems();

    std::stringstream ss;
    ss << "Physical_" << (nItems+1);

    vtkSmartPointer< vtkMRMLAnnotationFiducialNode > fid = vtkSmartPointer< vtkMRMLAnnotationFiducialNode >::New();
    fid->SetName(ss.str().c_str());
    double coord[3] = {x, y, z};
    fid->AddControlPoint(coord, 0, 1);
    //fid->SetFiducialCoordinates(x, y, z);
    d->Scene->AddNode(fid);
    //fid->SetParentNodeID(d->HierarchyNode->GetID());
    this->updateTable();
    }
}


//-----------------------------------------------------------------------------
void qSlicerRegistrationFiducialsTableModel
::setMRMLScene(vtkMRMLScene *newScene)
{
  Q_D(qSlicerRegistrationFiducialsTableModel);

  qvtkReconnect(d->Scene, newScene,
                vtkMRMLScene::NodeRemovedEvent,
                this, SLOT(onMRMLNodeRemovedEvent(vtkObject*,vtkObject*)));
  d->Scene = newScene;
}



void qSlicerRegistrationFiducialsTableModel
::onItemChanged(QStandardItem * item)
{
  Q_D(qSlicerRegistrationFiducialsTableModel);

  if (item == this->invisibleRootItem())
    {
    return;
    }
  if (d->PendingItemModified >= 0)
    {
    return;
    }

  // TODO:  item->parent()-> does not work here...
  QStandardItem* nameItem = this->invisibleRootItem()->child(item->row(), 0);
  if (nameItem)
    {
    QString id = nameItem->data(qSlicerRegistrationFiducialsTableModel::NodeIDRole).toString();

    // Find fiducial node from item
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
        if (id == fnode->GetID())
          {
          QString qstr = item->text();
          double coord[3];
          switch (item->column())
            {
            case 0:
              {
              const char* str = qstr.toAscii();
              fnode->SetName(str);
              break;
              }
            case 1:
              {
              fnode->GetFiducialCoordinates(coord);
              coord[0] = qstr.toDouble();
              fnode->SetFiducialCoordinates(coord);
              break;
              }              
            case 2:
              {
              fnode->GetFiducialCoordinates(coord);
              coord[1] = qstr.toDouble();
              fnode->SetFiducialCoordinates(coord);
              break;
              }              
            case 3:
              {
              fnode->GetFiducialCoordinates(coord);
              coord[2] = qstr.toDouble();
              fnode->SetFiducialCoordinates(coord);
              break;
              }              
            }
          fnode->Modified();
          this->updateTable();
          }
        }
      }
    }
}


void qSlicerRegistrationFiducialsTableModel
::onMRMLChildNodeAdded(vtkObject* o)
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
        qvtkConnect(fnode, vtkMRMLAnnotationFiducialNode::ValueModifiedEvent,
                    this, SLOT(onMRMLChildNodeValueModified(vtkObject*)));
        fnode->SetAttribute("RFTEvent", "Yes");
        }
      }
    }
  this->updateTable();
}

void qSlicerRegistrationFiducialsTableModel
::onMRMLChildNodeRemoved(vtkObject* o)
{
  vtkMRMLNode* n = vtkMRMLNode::SafeDownCast(o);
  vtkMRMLAnnotationFiducialNode* fnode = vtkMRMLAnnotationFiducialNode::SafeDownCast(n);
  if (fnode && fnode->GetAttribute("RFTEvent"))
    {
    if (strcmp("Yes", fnode->GetAttribute("RFTEvent")) == 0)
      {
      qvtkDisconnect(fnode, vtkMRMLAnnotationFiducialNode::ValueModifiedEvent,
                     this, SLOT(onMRMLChildNodeValueModified(vtkObject*)));
      fnode->SetAttribute("RFTEvent", NULL);
      this->updateTable();
      }
    }
}

void qSlicerRegistrationFiducialsTableModel
::onMRMLNodeRemovedEvent(vtkObject* caller, vtkObject* callData)
{
  Q_D(qSlicerRegistrationFiducialsTableModel);

  vtkMRMLScene* scene = vtkMRMLScene::SafeDownCast(caller);
  if (scene && d->Scene && scene == d->Scene)
    {
    onMRMLChildNodeRemoved(callData);
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


