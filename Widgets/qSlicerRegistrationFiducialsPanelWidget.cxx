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

#include <QDebug>
#include <QList>
#include <QTableWidgetSelectionRange>

#include "qSlicerRegistrationFiducialsTableModel.h"
#include "qSlicerAbstractCoreModule.h"
#include "qSlicerCoreApplication.h"
#include "qSlicerModuleManager.h"
#include "qSlicerApplication.h"

#include "vtkObject.h"
#include "vtkSmartPointer.h"
#include "vtkMatrix4x4.h"
#include "vtkMRMLAnnotationHierarchyNode.h"
#include "vtkMRMLLinearTransformNode.h"
#include "vtkMRMLInteractionNode.h"
#include "vtkMRMLSelectionNode.h"

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
  vtkMRMLAnnotationHierarchyNode* createNewHierarchyNode(const char* basename);

  // Tables in "Image Points" and "Physical Points" tabs
  qSlicerRegistrationFiducialsTableModel* ImagePointsTableModel;
  qSlicerRegistrationFiducialsTableModel* PhysicalPointsTableModel;

  // Linear transform node to import tacking data
  vtkMRMLLinearTransformNode* TrackerTransform;

  // Pointer to Logic class of Annotations module to switch ActiveHierarchy node.
  vtkSlicerAnnotationModuleLogic* AnnotationsLogic;

  QString OriginalAnnotationID;
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
  this->OriginalAnnotationID = "";
}

// --------------------------------------------------------------------------
void qSlicerRegistrationFiducialsPanelWidgetPrivate
::setupUi(qSlicerRegistrationFiducialsPanelWidget* widget)
{
  this->Ui_qSlicerRegistrationFiducialsPanelWidget::setupUi(widget);
}

//-----------------------------------------------------------------------------
vtkMRMLAnnotationHierarchyNode* qSlicerRegistrationFiducialsPanelWidgetPrivate
::createNewHierarchyNode(const char* basename)
{
  // NOTE: this method has to be called after setting AnnotationLogic;
  if (this->AnnotationsLogic)
    {
    vtkMRMLScene * scene = qSlicerCoreApplication::application()->mrmlScene();
    QString parentNodeID = this->AnnotationsLogic->GetTopLevelHierarchyNodeID();
    vtkMRMLAnnotationHierarchyNode* newnode
      = vtkMRMLAnnotationHierarchyNode::New();
    scene->AddNode(newnode);
    newnode->HideFromEditorsOff();
    newnode->SetName(scene->GetUniqueNameByString(basename));
    newnode->SetParentNodeID(parentNodeID.toLatin1());
    this->AnnotationsLogic->AddDisplayNodeForHierarchyNode(newnode);
    return newnode;
    }
  else
    {
    return NULL;
    }
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

  qSlicerAbstractCoreModule* annotationsModule =
    qSlicerCoreApplication::application()->moduleManager()->module("Annotations");
  if (annotationsModule)
    {
    d->AnnotationsLogic = 
      vtkSlicerAnnotationModuleLogic::SafeDownCast(annotationsModule->logic());
    }
  vtkMRMLScene * scene = qSlicerCoreApplication::application()->mrmlScene();

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
    connect(d->ImagePointsAnnotationNodeSelector, SIGNAL(currentNodeChanged(vtkMRMLNode*)),
            this, SLOT(setImagePointsAnnotationNode(vtkMRMLNode*)));
    if (scene)
      {
      d->ImagePointsAnnotationNodeSelector->setMRMLScene(scene);
      // Create a new hierarchy node
      vtkMRMLAnnotationHierarchyNode* node = d->createNewHierarchyNode("ImagePoint");
      if (node)
        {
        d->ImagePointsAnnotationNodeSelector->setCurrentNode(node);
        node->Delete();
        }
      }
    }
  if (d->PhysicalPointsAnnotationNodeSelector)
    {
    d->PhysicalPointsTableModel->setMRMLScene(d->PhysicalPointsAnnotationNodeSelector->mrmlScene());
    connect(d->PhysicalPointsAnnotationNodeSelector, SIGNAL(currentNodeChanged(vtkMRMLNode*)),
            d->PhysicalPointsTableModel, SLOT(setNode(vtkMRMLNode*)));
    connect(d->ImagePointsAnnotationNodeSelector, SIGNAL(currentNodeChanged(vtkMRMLNode*)),
            this, SLOT(setPhysicalPointsAnnotationNode(vtkMRMLNode*)));

    if (scene)
      {
      d->PhysicalPointsAnnotationNodeSelector->setMRMLScene(scene);
      // Create a new hierarchy node
      vtkMRMLAnnotationHierarchyNode* node = d->createNewHierarchyNode("PhysicalPoint");
      if (node)
        {
        d->PhysicalPointsAnnotationNodeSelector->setCurrentNode(node);
        node->Delete();
        }
      }
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
  //if (d->AddImagePointButton)
  //  {
  //  connect(d->AddImagePointButton, SIGNAL(clicked()),
  //          this, SLOT(switchPlaceMode()));
  //  }

  if (d->MouseModeToolBar)
    {
    d->MouseModeToolBar->setApplicationLogic(
                                             qSlicerApplication::application()->applicationLogic());
    d->MouseModeToolBar->setMRMLScene(qSlicerApplication::application()->mrmlScene());
    QObject::connect(qSlicerApplication::application(),
                     SIGNAL(mrmlSceneChanged(vtkMRMLScene*)),
                     d->MouseModeToolBar,
                     SLOT(setMRMLScene(vtkMRMLScene*)));
    }
  if (d->PointsTabWidget)
    {
    QObject::connect(d->PointsTabWidget,SIGNAL(currentChanged(int)),
                     this,SLOT(onTabSwitched(int)));
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
::setImagePointsAnnotationNode(vtkMRMLNode* node)
{
  Q_D(qSlicerRegistrationFiducialsPanelWidget);

  vtkMRMLAnnotationHierarchyNode* hnode;
  hnode = vtkMRMLAnnotationHierarchyNode::SafeDownCast(node);
  if (hnode)
    {
    d->AnnotationsLogic->SetActiveHierarchyNodeID(hnode->GetID());
    }
}


//-----------------------------------------------------------------------------
void qSlicerRegistrationFiducialsPanelWidget
::setPhysicalPointsAnnotationNode(vtkMRMLNode* node)
{
  Q_D(qSlicerRegistrationFiducialsPanelWidget);

  vtkMRMLAnnotationHierarchyNode* hnode;
  hnode = vtkMRMLAnnotationHierarchyNode::SafeDownCast(node);
  if (hnode)
    {
    d->AnnotationsLogic->SetActiveHierarchyNodeID(hnode->GetID());
    }
}


//-----------------------------------------------------------------------------
void qSlicerRegistrationFiducialsPanelWidget
::onTabSwitched(int index)
{
  Q_D(qSlicerRegistrationFiducialsPanelWidget);

  vtkMRMLAnnotationHierarchyNode* hnode = NULL;

  switch(index)
    {
    case 0:
      {
      hnode = vtkMRMLAnnotationHierarchyNode::SafeDownCast
        (d->ImagePointsAnnotationNodeSelector->currentNode());
      break;
      }
    case 1:
      {
      hnode = vtkMRMLAnnotationHierarchyNode::SafeDownCast
        (d->PhysicalPointsAnnotationNodeSelector->currentNode());
      break;
      }
    default:
      break;
    }
  if (hnode)
    {
    d->AnnotationsLogic->SetActiveHierarchyNodeID(hnode->GetID());
    }
}


//-----------------------------------------------------------------------------
void qSlicerRegistrationFiducialsPanelWidget
::enter()
{
  Q_D(qSlicerRegistrationFiducialsPanelWidget);
  
  if (d->PointsTabWidget)
    {
    int i = d->PointsTabWidget->currentIndex();
    onTabSwitched(i);
    }
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

////-----------------------------------------------------------------------------
//void qSlicerRegistrationFiducialsPanelWidget
//::switchPlaceMode()
//{
//  // The following code is based on 
//  // void Slicer/Base/QTGUI/qSlicerMouseModeToolBar.cxx (switchPlaceMode())
//  Q_D(qSlicerRegistrationFiducialsPanelWidget);
//
//  if (!d->AddImagePointButton)
//    {
//    return;
//    }
//  if (!d->ImagePointsAnnotationNodeSelector)
//    {
//    return;
//    }
//  if (!d->AnnotationsLogic)
//    {
//    return;
//    }
//    
//  vtkSlicerApplicationLogic* appLogic = qSlicerCoreApplication::application()->applicationLogic();
//  if (!appLogic)
//    {
//    qWarning() << "Mouse Mode Tool Bar not set up with application logic";
//    return;
//    }
//
//  vtkMRMLInteractionNode * interactionNode = appLogic->GetInteractionNode();
//  if (!interactionNode)
//    {
//    qCritical() << "qSlicerMouseModeToolBar::switchPlaceMode: Cannot get interaction node.";
//    }
//
//  vtkMRMLSelectionNode *selectionNode = appLogic->GetSelectionNode();
//  if (!selectionNode )
//    {
//    qCritical() << "qSlicerMouseModeToolBar::switchPlaceMode: cannot get selection node.";
//    return;
//    }
//
//  if (d->AddImagePointButton->isChecked()) // Start place mode. 
//    {
//    d->OriginalAnnotationID = QString(selectionNode->GetActiveAnnotationID());
//    QString currentID = "";
//    vtkMRMLAnnotationHierarchyNode* hnode;
//    hnode = vtkMRMLAnnotationHierarchyNode::SafeDownCast
//      (d->ImagePointsAnnotationNodeSelector->currentNode());
//
//    if (hnode)
//      {
//      currentID = hnode->GetID();
//      }
//    else
//      {
//      // Create a new Annotation hierarchy
//      vtkMRMLScene* scene = d->ImagePointsAnnotationNodeSelector->mrmlScene();
//      QString parentNodeID = d->AnnotationsLogic->GetTopLevelHierarchyNodeID();
//      vtkMRMLAnnotationHierarchyNode* newnode
//        = vtkMRMLAnnotationHierarchyNode::New();
//      scene->AddNode(newnode);
//      newnode->HideFromEditorsOff();
//      newnode->SetName(scene->GetUniqueNameByString("ImagePoints"));
//      newnode->SetParentNodeID(parentNodeID.toLatin1());
//      d->AnnotationsLogic->AddDisplayNodeForHierarchyNode(newnode);
//      currentID = newnode->GetID();
//      d->ImagePointsAnnotationNodeSelector->setCurrentNode(newnode);
//      newnode->Delete();
//      }
//    if (currentID.compare("") != 0)
//      {
//      //qSlicerApplication* app = qobject_cast< qSlicerApplication* > (qSlicerCoreApplication::application());
//      //qSlicerAppMainWindow * mw = qobject_cast< qSlicerAppMainWindow *>(app->mainWindow());
//      //if (mw && mw->MouseModeToolBar)
//      //  {
//      //  }
//      //d->AnnotationsLogic->SetActiveHierarchyNodeID(currentID.toLatin1());
//      selectionNode->SetReferenceActiveAnnotationID(currentID.toLatin1());
//      //interactionNode->SetPlaceModePersistence(1);
//      //interactionNode->SetCurrentInteractionMode(vtkMRMLInteractionNode::Place);
//      interactionNode->SwitchToPersistentPlaceMode();
//
//      }
//    else
//      {
//      d->OriginalAnnotationID = "";
//      }
//    }
//  else
//    {
//    selectionNode->SetReferenceActiveAnnotationID(d->OriginalAnnotationID.toLatin1());
//    //interactionNode->SetPlaceModePersistence(0);
//    //interactionNode->SetCurrentInteractionMode(vtkMRMLInteractionNode::ViewTransform);
//    interactionNode->SwitchToViewTransformMode();
//    interactionNode->InvokeEvent(vtkMRMLInteractionNode::EndPlacementEvent);
//    //d->AnnotationsLogic->SetActiveHierarchyNodeID(d->OriginalAnnotationID.toLatin1());
//    d->OriginalAnnotationID = "";
//    }
//}

