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

// RegistrationFiducialsPanel Widgets includes
#include "qSlicerRegistrationFiducialsPanelWidget.h"
#include "ui_qSlicerRegistrationFiducialsPanelWidget.h"

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
}

//-----------------------------------------------------------------------------
qSlicerRegistrationFiducialsPanelWidget
::~qSlicerRegistrationFiducialsPanelWidget()
{
}
