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

#ifndef __qSlicerRegistrationFiducialsPanelWidget_h
#define __qSlicerRegistrationFiducialsPanelWidget_h

// Qt includes
#include <QWidget>
#include <ctkVTKObject.h>

// RegistrationFiducialsPanel Widgets includes
#include "qSlicerPointBasedPatientRegistrationModuleWidgetsExport.h"

class qSlicerRegistrationFiducialsPanelWidgetPrivate;
class vtkObject;
class vtkMRMLScene;
class vtkMRMLNode;

/// \ingroup Slicer_QtModules_PointBasedPatientRegistration
class Q_SLICER_MODULE_POINTBASEDPATIENTREGISTRATION_WIDGETS_EXPORT qSlicerRegistrationFiducialsPanelWidget
  : public QWidget
{
  Q_OBJECT
  QVTK_OBJECT
public:
  typedef QWidget Superclass;
  qSlicerRegistrationFiducialsPanelWidget(QWidget *parent=0);
  virtual ~qSlicerRegistrationFiducialsPanelWidget();

public slots:
  virtual void setMRMLScene(vtkMRMLScene *newScene);
  void setTrackerTransform(vtkMRMLNode*);
  void onTrackerTransformModified();

protected slots:
  void clearImagePoints();
  void clearPhysicalPoints();
  void addPhysicalPoint();
  void switchPlaceMode();

protected:
  QScopedPointer<qSlicerRegistrationFiducialsPanelWidgetPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qSlicerRegistrationFiducialsPanelWidget);
  Q_DISABLE_COPY(qSlicerRegistrationFiducialsPanelWidget);
};

#endif
