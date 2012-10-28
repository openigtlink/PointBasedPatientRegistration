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

#ifndef __qSlicerPointBasedPatientRegistrationModuleWidget_h
#define __qSlicerPointBasedPatientRegistrationModuleWidget_h

// SlicerQt includes
#include "qSlicerAbstractModuleWidget.h"

#include "qSlicerPointBasedPatientRegistrationModuleExport.h"

class qSlicerPointBasedPatientRegistrationModuleWidgetPrivate;
class vtkMRMLNode;

/// \ingroup Slicer_QtModules_ExtensionTemplate
class Q_SLICER_QTMODULES_POINTBASEDPATIENTREGISTRATION_EXPORT qSlicerPointBasedPatientRegistrationModuleWidget :
  public qSlicerAbstractModuleWidget
{
  Q_OBJECT

public:

  typedef qSlicerAbstractModuleWidget Superclass;
  qSlicerPointBasedPatientRegistrationModuleWidget(QWidget *parent=0);
  virtual ~qSlicerPointBasedPatientRegistrationModuleWidget();

public slots:
  virtual void setMRMLScene(vtkMRMLScene *newScene);

protected:
  QScopedPointer<qSlicerPointBasedPatientRegistrationModuleWidgetPrivate> d_ptr;
  
  virtual void setup();
  virtual void enter();

private:
  Q_DECLARE_PRIVATE(qSlicerPointBasedPatientRegistrationModuleWidget);
  Q_DISABLE_COPY(qSlicerPointBasedPatientRegistrationModuleWidget);
};

#endif
