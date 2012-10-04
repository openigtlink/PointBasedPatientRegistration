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

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

  This file was originally developed by Junichi Tokuda, Brigham and Women's
  Hospital based on a template developed by Jean-Christophe Fillion-Robin,
  Kitware Inc. The project was supported by NIH P41EB015898.

==============================================================================*/

// Qt includes
#include <QtPlugin>

// ExtensionTemplate Logic includes
#include <vtkSlicerPointBasedPatientRegistrationLogic.h>

// ExtensionTemplate includes
#include "qSlicerPointBasedPatientRegistrationModule.h"
#include "qSlicerPointBasedPatientRegistrationModuleWidget.h"

//-----------------------------------------------------------------------------
Q_EXPORT_PLUGIN2(qSlicerPointBasedPatientRegistrationModule, qSlicerPointBasedPatientRegistrationModule);

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerPointBasedPatientRegistrationModulePrivate
{
public:
  qSlicerPointBasedPatientRegistrationModulePrivate();
};

//-----------------------------------------------------------------------------
// qSlicerPointBasedPatientRegistrationModulePrivate methods

//-----------------------------------------------------------------------------
qSlicerPointBasedPatientRegistrationModulePrivate::qSlicerPointBasedPatientRegistrationModulePrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerPointBasedPatientRegistrationModule methods

//-----------------------------------------------------------------------------
qSlicerPointBasedPatientRegistrationModule::qSlicerPointBasedPatientRegistrationModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicerPointBasedPatientRegistrationModulePrivate)
{
}

//-----------------------------------------------------------------------------
qSlicerPointBasedPatientRegistrationModule::~qSlicerPointBasedPatientRegistrationModule()
{
}

//-----------------------------------------------------------------------------
QString qSlicerPointBasedPatientRegistrationModule::helpText()const
{
  return "This is a loadable module bundled in an extension";
}

//-----------------------------------------------------------------------------
QString qSlicerPointBasedPatientRegistrationModule::acknowledgementText()const
{
  return "This work was was partially funded by NCIGT";
}

//-----------------------------------------------------------------------------
QStringList qSlicerPointBasedPatientRegistrationModule::contributors()const
{
  QStringList moduleContributors;
  moduleContributors << QString("Junichi Tokuda (Brigham and Women's Hospital)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qSlicerPointBasedPatientRegistrationModule::icon()const
{
  return QIcon(":/Icons/PointBasedPatientRegistration.png");
}

//-----------------------------------------------------------------------------
QStringList qSlicerPointBasedPatientRegistrationModule::categories() const
{
  return QStringList() << "IGT";
}

//-----------------------------------------------------------------------------
QStringList qSlicerPointBasedPatientRegistrationModule::dependencies() const
{
  return QStringList();
}

//-----------------------------------------------------------------------------
void qSlicerPointBasedPatientRegistrationModule::setup()
{
  this->Superclass::setup();
}

//-----------------------------------------------------------------------------
qSlicerAbstractModuleRepresentation * qSlicerPointBasedPatientRegistrationModule::createWidgetRepresentation()
{
  return new qSlicerPointBasedPatientRegistrationModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qSlicerPointBasedPatientRegistrationModule::createLogic()
{
  return vtkSlicerPointBasedPatientRegistrationLogic::New();
}
