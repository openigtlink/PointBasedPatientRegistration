/*==============================================================================

  Program: 3D Slicer

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

#ifndef __qSlicerRegistrationFiducialsPanelWidget_h
#define __qSlicerRegistrationFiducialsPanelWidget_h

// Qt includes
#include <QWidget>

// RegistrationFiducialsPanel Widgets includes
#include "qSlicerPointBasedPatientRegistrationModuleWidgetsExport.h"

class qSlicerRegistrationFiducialsPanelWidgetPrivate;
class vtkMRMLScene;

/// \ingroup Slicer_QtModules_PointBasedPatientRegistration
class Q_SLICER_MODULE_POINTBASEDPATIENTREGISTRATION_WIDGETS_EXPORT qSlicerRegistrationFiducialsPanelWidget
  : public QWidget
{
  Q_OBJECT
public:
  typedef QWidget Superclass;
  qSlicerRegistrationFiducialsPanelWidget(QWidget *parent=0);
  virtual ~qSlicerRegistrationFiducialsPanelWidget();

public slots:
  virtual void setMRMLScene(vtkMRMLScene *newScene);

protected slots:

protected:
  QScopedPointer<qSlicerRegistrationFiducialsPanelWidgetPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qSlicerRegistrationFiducialsPanelWidget);
  Q_DISABLE_COPY(qSlicerRegistrationFiducialsPanelWidget);
};

#endif
