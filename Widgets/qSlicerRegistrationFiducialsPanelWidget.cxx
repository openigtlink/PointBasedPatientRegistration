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
#include "qSlicerRegistrationFiducialsPanelWidget.h"
#include "ui_qSlicerRegistrationFiducialsPanelWidget.h"

#include <QList>
#include <QTableWidgetSelectionRange>

#include "qSlicerRegistrationFiducialsTableModel.h"


//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_PointBasedPatientRegistration
class qSlicerRegistrationFiducialsPanelWidgetPrivate
  : public Ui_qSlicerRegistrationFiducialsPanelWidget
{
  Q_DECLARE_PUBLIC(qSlicerRegistrationFiducialsPanelWidget);
protected:
  qSlicerRegistrationFiducialsPanelWidget* const q_ptr;

public:
  qSlicerRegistrationFiducialsPanelWidgetPrivate(
    qSlicerRegistrationFiducialsPanelWidget& object);
  virtual void setupUi(qSlicerRegistrationFiducialsPanelWidget*);

  qSlicerRegistrationFiducialsTableModel* ImagePointsTableModel;
  qSlicerRegistrationFiducialsTableModel* PhysicalPointsTableModel;
};

// --------------------------------------------------------------------------
qSlicerRegistrationFiducialsPanelWidgetPrivate
::qSlicerRegistrationFiducialsPanelWidgetPrivate(
  qSlicerRegistrationFiducialsPanelWidget& object)
  : q_ptr(&object)
{
}

// --------------------------------------------------------------------------
void qSlicerRegistrationFiducialsPanelWidgetPrivate
::setupUi(qSlicerRegistrationFiducialsPanelWidget* widget)
{
  this->Ui_qSlicerRegistrationFiducialsPanelWidget::setupUi(widget);
}

//-----------------------------------------------------------------------------
// qSlicerRegistrationFiducialsPanelWidget methods

//-----------------------------------------------------------------------------
qSlicerRegistrationFiducialsPanelWidget
::qSlicerRegistrationFiducialsPanelWidget(QWidget* parentWidget)
  : Superclass( parentWidget )
  , d_ptr( new qSlicerRegistrationFiducialsPanelWidgetPrivate(*this) )
{
  Q_D(qSlicerRegistrationFiducialsPanelWidget);
  d->setupUi(this);

  d->ImagePointsTableModel    = new qSlicerRegistrationFiducialsTableModel(this);
  d->PhysicalPointsTableModel = new qSlicerRegistrationFiducialsTableModel(this);
  d->ImagePointsTableModel->setCoordinateLabel(qSlicerRegistrationFiducialsTableModel::LABEL_RAS);
  d->PhysicalPointsTableModel->setCoordinateLabel(qSlicerRegistrationFiducialsTableModel::LABEL_XYZ);

  d->ImagePointsTable->setModel(d->ImagePointsTableModel);
  d->PhysicalPointsTable->setModel(d->PhysicalPointsTableModel);
  if (d->ImagePointsAnnotationNodeSelector)
    {
    connect(d->ImagePointsAnnotationNodeSelector, SIGNAL(currentNodeChanged(vtkMRMLNode*)),
            d->ImagePointsTableModel, SLOT(setNode(vtkMRMLNode*)));
    }
  if (d->PhysicalPointsAnnotationNodeSelector)
    {
    connect(d->PhysicalPointsAnnotationNodeSelector, SIGNAL(currentNodeChanged(vtkMRMLNode*)),
            d->PhysicalPointsTableModel, SLOT(setNode(vtkMRMLNode*)));
    }

}


//-----------------------------------------------------------------------------
qSlicerRegistrationFiducialsPanelWidget
::~qSlicerRegistrationFiducialsPanelWidget()
{
}


//-----------------------------------------------------------------------------
void qSlicerRegistrationFiducialsPanelWidget
::setMRMLScene(vtkMRMLScene *newScene)
{
  Q_D(qSlicerRegistrationFiducialsPanelWidget);

  if (d->ImagePointsAnnotationNodeSelector)
    {
    d->ImagePointsAnnotationNodeSelector->setMRMLScene(newScene);
    // Listen for any new new fiducial points
    //this->qvtkReconnect(oldScene, newScene, vtkMRMLScene::NodeAddedEvent, 
    //this, SLOT(onNodeAddedEvent(vtkObject*,vtkObject*)));
    }
  if (d->PhysicalPointsAnnotationNodeSelector)
    {
    d->PhysicalPointsAnnotationNodeSelector->setMRMLScene(newScene);
    }
}
