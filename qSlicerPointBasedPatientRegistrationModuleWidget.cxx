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
  Hospital based on a template developed by Jean-Christophe Fillion-Robin,
  Kitware Inc. The project was supported by NIH P41EB015898.

==============================================================================*/

// Qt includes
#include <QDebug>

// SlicerQt includes
#include "qSlicerPointBasedPatientRegistrationModuleWidget.h"
#include "ui_qSlicerPointBasedPatientRegistrationModule.h"

#include "qSlicerApplication.h"
#include "vtkMRMLScene.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerPointBasedPatientRegistrationModuleWidgetPrivate: public Ui_qSlicerPointBasedPatientRegistrationModule
{
public:
  qSlicerPointBasedPatientRegistrationModuleWidgetPrivate();
};

//-----------------------------------------------------------------------------
// qSlicerPointBasedPatientRegistrationModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qSlicerPointBasedPatientRegistrationModuleWidgetPrivate::qSlicerPointBasedPatientRegistrationModuleWidgetPrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerPointBasedPatientRegistrationModuleWidget methods

//-----------------------------------------------------------------------------
qSlicerPointBasedPatientRegistrationModuleWidget::qSlicerPointBasedPatientRegistrationModuleWidget(QWidget* _parent)
  : Superclass( _parent )
  , d_ptr( new qSlicerPointBasedPatientRegistrationModuleWidgetPrivate )
{
}

//-----------------------------------------------------------------------------
qSlicerPointBasedPatientRegistrationModuleWidget::~qSlicerPointBasedPatientRegistrationModuleWidget()
{
}

//-----------------------------------------------------------------------------
void qSlicerPointBasedPatientRegistrationModuleWidget::setup()
{
  Q_D(qSlicerPointBasedPatientRegistrationModuleWidget);
  d->setupUi(this);
  this->Superclass::setup();
}

//-----------------------------------------------------------------------------
void qSlicerPointBasedPatientRegistrationModuleWidget::enter()
{
  Q_D(qSlicerPointBasedPatientRegistrationModuleWidget);

  this->Superclass::enter();
  if (d->RegistrationFiducialsPanel)
    {
    d->RegistrationFiducialsPanel->enter();
    }
}

//-----------------------------------------------------------------------------
void qSlicerPointBasedPatientRegistrationModuleWidget::setMRMLScene(vtkMRMLScene *newScene)
{
  Q_D(qSlicerPointBasedPatientRegistrationModuleWidget);

  vtkMRMLScene* oldScene = this->mrmlScene();

  this->Superclass::setMRMLScene(newScene);

  qSlicerApplication * app = qSlicerApplication::application();
  if (!app)
    {
    return;
    }
  
  if (oldScene != newScene)
    {
    if (d->RegistrationFiducialsPanel)
      {
      d->RegistrationFiducialsPanel->setMRMLScene(newScene);
      }
    }

  newScene->InitTraversal();

}
