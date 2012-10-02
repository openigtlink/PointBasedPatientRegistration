/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

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
  return QStringList() << "Examples";
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
