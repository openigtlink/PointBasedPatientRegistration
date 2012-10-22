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
#include "qSlicerAbstractCoreModule.h"
#include "qSlicerCoreApplication.h"
#include "qSlicerModuleManager.h"

#include "vtkObject.h"
#include "vtkSmartPointer.h"
#include "vtkMatrix4x4.h"
#include "vtkMRMLAnnotationHierarchyNode.h"
#include "vtkMRMLLinearTransformNode.h"

#include "vtkSlicerAnnotationModuleLogic.h"

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
  vtkMRMLLinearTransformNode* TrackerTransform;
  vtkSlicerAnnotationModuleLogic* AnnotationsLogic;
};

// --------------------------------------------------------------------------
qSlicerRegistrationFiducialsPanelWidgetPrivate
::qSlicerRegistrationFiducialsPanelWidgetPrivate(
  qSlicerRegistrationFiducialsPanelWidget& object)
  : q_ptr(&object)
{
  this->ImagePointsTableModel = NULL;
  this->PhysicalPointsTableModel = NULL;
  this->TrackerTransform = NULL;
  this->AnnotationsLogic = NULL;
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
    d->ImagePointsTableModel->setMRMLScene(d->ImagePointsAnnotationNodeSelector->mrmlScene());
    connect(d->ImagePointsAnnotationNodeSelector, SIGNAL(currentNodeChanged(vtkMRMLNode*)),
            d->ImagePointsTableModel, SLOT(setNode(vtkMRMLNode*)));
    }
  if (d->PhysicalPointsAnnotationNodeSelector)
    {
    d->PhysicalPointsTableModel->setMRMLScene(d->PhysicalPointsAnnotationNodeSelector->mrmlScene());
    connect(d->PhysicalPointsAnnotationNodeSelector, SIGNAL(currentNodeChanged(vtkMRMLNode*)),
            d->PhysicalPointsTableModel, SLOT(setNode(vtkMRMLNode*)));
    }

  if (d->ClearImagePointsButton)
    {
    connect(d->ClearImagePointsButton, SIGNAL(clicked()),
            this, SLOT(clearImagePoints()));
    }
  if (d->ClearPhysicalPointsButton)
    {
    connect(d->ClearPhysicalPointsButton, SIGNAL(clicked()),
            this, SLOT(clearPhysicalPoints()));
    }
  if (d->AddPhysicalPointButton)
    {
    connect(d->AddPhysicalPointButton, SIGNAL(clicked()),
            this, SLOT(addPhysicalPoint()));
    }
  if (d->TrackerTransformNodeSelector)
    {
    connect(d->TrackerTransformNodeSelector, SIGNAL(currentNodeChanged(vtkMRMLNode*)),
            this, SLOT(setTrackerTransform(vtkMRMLNode*)));
    }

  qSlicerAbstractCoreModule* annotationsModule =
    qSlicerCoreApplication::application()->moduleManager()->module("Annotations");

  if (annotationsModule)
    {
    d->AnnotationsLogic = 
      vtkSlicerAnnotationModuleLogic::SafeDownCast(annotationsModule->logic());
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
  if (d->ImagePointsTableModel)
    {
    d->ImagePointsTableModel->setMRMLScene(newScene);
    }
  if (d->PhysicalPointsTableModel)
    {
    d->PhysicalPointsTableModel->setMRMLScene(newScene);
    }
  if (d->TrackerTransformNodeSelector)
    {
    d->TrackerTransformNodeSelector->setMRMLScene(newScene);
    }

}

//-----------------------------------------------------------------------------
void qSlicerRegistrationFiducialsPanelWidget
::setTrackerTransform(vtkMRMLNode* o)
{
  Q_D(qSlicerRegistrationFiducialsPanelWidget);

  vtkMRMLLinearTransformNode* trans = vtkMRMLLinearTransformNode::SafeDownCast(o);
  if (trans)
    {
    qvtkReconnect(d->TrackerTransform, trans,
                  vtkMRMLTransformableNode::TransformModifiedEvent, 
                  this, SLOT(onTrackerTransformModified()));
    d->TrackerTransform = trans;
    }
}


//-----------------------------------------------------------------------------
void qSlicerRegistrationFiducialsPanelWidget
::onTrackerTransformModified()
{
  Q_D(qSlicerRegistrationFiducialsPanelWidget);

  vtkMatrix4x4* matrix = d->TrackerTransform->GetMatrixTransformToParent();
  QString buf;
  d->PositionXEdit->setText(buf.setNum(matrix->Element[0][3]));
  d->PositionYEdit->setText(buf.setNum(matrix->Element[1][3]));
  d->PositionZEdit->setText(buf.setNum(matrix->Element[2][3]));
}



//-----------------------------------------------------------------------------
void qSlicerRegistrationFiducialsPanelWidget
::clearImagePoints()
{
  Q_D(qSlicerRegistrationFiducialsPanelWidget);
  if (d->ImagePointsAnnotationNodeSelector)
    {
    vtkMRMLAnnotationHierarchyNode* hnode;
    hnode = vtkMRMLAnnotationHierarchyNode::SafeDownCast(d->ImagePointsAnnotationNodeSelector->currentNode());
    if (hnode)
      {
      hnode->RemoveChildrenNodes();
      hnode->InvokeEvent(vtkMRMLAnnotationHierarchyNode::HierarchyModifiedEvent);
      }
    }
}


//-----------------------------------------------------------------------------
void qSlicerRegistrationFiducialsPanelWidget
::clearPhysicalPoints()
{
  Q_D(qSlicerRegistrationFiducialsPanelWidget);
  if (d->PhysicalPointsAnnotationNodeSelector)
    {
    vtkMRMLAnnotationHierarchyNode* hnode;
    hnode = vtkMRMLAnnotationHierarchyNode::SafeDownCast(d->PhysicalPointsAnnotationNodeSelector->currentNode());
    if (hnode)
      {
      hnode->RemoveChildrenNodes();
      hnode->InvokeEvent(vtkMRMLAnnotationHierarchyNode::HierarchyModifiedEvent);
      }
    }
}


//-----------------------------------------------------------------------------
void qSlicerRegistrationFiducialsPanelWidget
::addPhysicalPoint()
{
  Q_D(qSlicerRegistrationFiducialsPanelWidget);
  if (d->TrackerTransform && d->PhysicalPointsTableModel && d->AnnotationsLogic)
    {
    // Check the current active AnnotationHierarchy node.
    // If it is different from the one specified in "Fiducials" menu, 
    // we switch the active node before adding a fiducial node. Once the node
    // is added, we switch back to the original active node.

    //vtkMRMLAnnotationHierarchyNode* original = d->AnnotationsLogic->GetActiveHierarchyNodeID();
    //vtkMRMLAnnotationHierarchyNode* current  = NULL;
    std::string original = d->AnnotationsLogic->GetActiveHierarchyNodeID();;
    std::string current = "";

    if (original.compare("") != 0)
      {
      current = original;
      vtkMRMLAnnotationHierarchyNode* hnode;
      hnode = vtkMRMLAnnotationHierarchyNode::SafeDownCast
        (d->PhysicalPointsAnnotationNodeSelector->currentNode());
      if (hnode && original.compare(hnode->GetID()) != 0)
        {
        current = hnode->GetID();
        }
      }
    else
      {
      vtkMRMLAnnotationHierarchyNode* hnode;
      hnode = vtkMRMLAnnotationHierarchyNode::SafeDownCast
        (d->PhysicalPointsAnnotationNodeSelector->currentNode());
      if (hnode)
        {
        current = hnode->GetID();
        }
      }
    if (current.compare("") != 0)
      {
      // Switch the active hierarchy node
      d->AnnotationsLogic->SetActiveHierarchyNodeID(current.c_str());

      // Add a new fiducial node to the active hierarchy
      vtkSmartPointer< vtkMatrix4x4 > matrix = vtkSmartPointer< vtkMatrix4x4 >::New();
      d->TrackerTransform->GetMatrixTransformToWorld(matrix);
      d->PhysicalPointsTableModel->addPoint(matrix->Element[0][3],
                                            matrix->Element[1][3],
                                            matrix->Element[2][3]);

      // Switch the active hierarchy node to the original
      d->AnnotationsLogic->SetActiveHierarchyNodeID(original.c_str());
      }
    }
}



