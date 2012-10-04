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

class Q_SLICER_MODULE_POINTBASEDPATIENTREGISTRATION_WIDGETS_EXPORT qSlicerRegistrationFiducialsTableModelPrivate
{
  Q_DECLARE_PUBLIC(qSlicerRegistrationFiducialsTableModel);
 protected:
  qSlicerRegistrationFiducialsTableModel * const q_ptr;

 public:
  qSlicerRegistrationFiducialsTableModelPrivate(
    qSlicerRegistrationFiducialsTableModel& object);
  virtual ~qSlicerRegistrationFiducialsTableModelPrivate();

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


qSlicerRegistrationFiducialsTableModel
::qSlicerRegistrationFiducialsTableModel(QObject *parent)
  : QAbstractTableModel(parent)
  , d_ptr( new qSlicerRegistrationFiducialsTableModelPrivate(*this) )
{
  Q_D(qSlicerRegistrationFiducialsTableModel);
}

qSlicerRegistrationFiducialsTableModel
::qSlicerRegistrationFiducialsTableModel(QList< QPair<QString, QString> > listofPairs, QObject *parent)
{
  Q_D(qSlicerRegistrationFiducialsTableModel);
}

qSlicerRegistrationFiducialsTableModel
::~qSlicerRegistrationFiducialsTableModel()
{
}

qSlicerRegistrationFiducialsTableModel
::qSlicerRegistrationFiducialsTableModel(qSlicerRegistrationFiducialsTableModelPrivate* pimpl, QObject *parent)
  : QAbstractTableModel(parent)
  , d_ptr(pimpl)
{
  Q_D(qSlicerRegistrationFiducialsTableModel);
}


  
int qSlicerRegistrationFiducialsTableModel
::rowCount(const QModelIndex &parent) const
{
}


int qSlicerRegistrationFiducialsTableModel
::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return 4;
}


QVariant qSlicerRegistrationFiducialsTableModel
::data(const QModelIndex &index, int role) const
{
}


QVariant qSlicerRegistrationFiducialsTableModel
::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole)
    return QVariant();
  
  if (orientation == Qt::Horizontal) {
    switch (section) {
    case 0:
      return tr("Point #");
      
    case 1:
      return tr("R");

    case 2:
      return tr("A");

    case 3:
      return tr("S");
      
    default:
      return QVariant();
    }
  }
  return QVariant();
}


Qt::ItemFlags qSlicerRegistrationFiducialsTableModel
::flags(const QModelIndex &index) const
{
}


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


QList< QPair<QString, QString> > qSlicerRegistrationFiducialsTableModel
::getList()
{
}


void qSlicerRegistrationFiducialsTableModel
::setMRMLNode(vtkMRMLNode* node)
{
}

void qSlicerRegistrationFiducialsTableModel
::onNodeAddedEvent(vtkObject* scene, vtkObject* node)
{
}


void qSlicerRegistrationFiducialsTableModel
::onNodeRemovedEvent(vtkObject* scene, vtkObject* node)
{
}


void qSlicerRegistrationFiducialsTableModel
::onLayoutChanged(int)
{
}

