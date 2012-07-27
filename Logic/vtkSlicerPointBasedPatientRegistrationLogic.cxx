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

// PointBasedPatientRegistration Logic includes
#include "vtkSlicerPointBasedPatientRegistrationLogic.h"

// MRML includes

// VTK includes
#include <vtkNew.h>

// STD includes
#include <cassert>

//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkSlicerPointBasedPatientRegistrationLogic);

//----------------------------------------------------------------------------
vtkSlicerPointBasedPatientRegistrationLogic::vtkSlicerPointBasedPatientRegistrationLogic()
{
}

//----------------------------------------------------------------------------
vtkSlicerPointBasedPatientRegistrationLogic::~vtkSlicerPointBasedPatientRegistrationLogic()
{
}

//----------------------------------------------------------------------------
void vtkSlicerPointBasedPatientRegistrationLogic::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//---------------------------------------------------------------------------
void vtkSlicerPointBasedPatientRegistrationLogic::SetMRMLSceneInternal(vtkMRMLScene * newScene)
{
  vtkNew<vtkIntArray> events;
  events->InsertNextValue(vtkMRMLScene::NodeAddedEvent);
  events->InsertNextValue(vtkMRMLScene::NodeRemovedEvent);
  events->InsertNextValue(vtkMRMLScene::EndBatchProcessEvent);
  this->SetAndObserveMRMLSceneEventsInternal(newScene, events.GetPointer());
}

//-----------------------------------------------------------------------------
void vtkSlicerPointBasedPatientRegistrationLogic::RegisterNodes()
{
  assert(this->GetMRMLScene() != 0);
}

//---------------------------------------------------------------------------
void vtkSlicerPointBasedPatientRegistrationLogic::UpdateFromMRMLScene()
{
  assert(this->GetMRMLScene() != 0);
}

//---------------------------------------------------------------------------
void vtkSlicerPointBasedPatientRegistrationLogic
::OnMRMLSceneNodeAdded(vtkMRMLNode* vtkNotUsed(node))
{
}

//---------------------------------------------------------------------------
void vtkSlicerPointBasedPatientRegistrationLogic
::OnMRMLSceneNodeRemoved(vtkMRMLNode* vtkNotUsed(node))
{
}

