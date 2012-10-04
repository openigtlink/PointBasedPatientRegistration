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

#ifndef __qSlicerPointBasedPatientRegistrationModule_h
#define __qSlicerPointBasedPatientRegistrationModule_h

// SlicerQt includes
#include "qSlicerLoadableModule.h"

#include "qSlicerPointBasedPatientRegistrationModuleExport.h"

class qSlicerPointBasedPatientRegistrationModulePrivate;

/// \ingroup Slicer_QtModules_ExtensionTemplate
class Q_SLICER_QTMODULES_POINTBASEDPATIENTREGISTRATION_EXPORT qSlicerPointBasedPatientRegistrationModule :
  public qSlicerLoadableModule
{
  Q_OBJECT
  Q_INTERFACES(qSlicerLoadableModule);

public:

  typedef qSlicerLoadableModule Superclass;
  explicit qSlicerPointBasedPatientRegistrationModule(QObject *parent=0);
  virtual ~qSlicerPointBasedPatientRegistrationModule();

  qSlicerGetTitleMacro(QTMODULE_TITLE);

  virtual QString helpText()const;
  virtual QString acknowledgementText()const;
  virtual QStringList contributors()const;

  /// Return a custom icon for the module
  virtual QIcon icon()const;

  virtual QStringList categories()const;
  virtual QStringList dependencies() const;

protected:

  /// Initialize the module. Register the volumes reader/writer
  virtual void setup();

  /// Create and return the widget representation associated to this module
  virtual qSlicerAbstractModuleRepresentation * createWidgetRepresentation();

  /// Create and return the logic associated to this module
  virtual vtkMRMLAbstractLogic* createLogic();

protected:
  QScopedPointer<qSlicerPointBasedPatientRegistrationModulePrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qSlicerPointBasedPatientRegistrationModule);
  Q_DISABLE_COPY(qSlicerPointBasedPatientRegistrationModule);

};

#endif
