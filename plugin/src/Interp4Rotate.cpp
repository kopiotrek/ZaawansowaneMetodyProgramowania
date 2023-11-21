#include <iostream>
#include "Interp4Rotate.hh"


using std::cout;
using std::endl;


extern "C" {
  AbstractInterp4Command* CreateCmd(void);
  const char* GetCmdName() { return "Rotate"; }
}




/*!
 * \brief
 *
 *
 */
AbstractInterp4Command* CreateCmd(void)
{
  return Interp4Rotate::CreateCmd();
}


/*!
 *
 */
Interp4Rotate::Interp4Rotate(): _Name(""), _Ang_speed(0), _Angle(0)
{}


/*!
 *
 */
void Interp4Rotate::PrintCmd() const
{
  /*
   *  Tu trzeba napisać odpowiednio zmodyfikować kod poniżej.
   */
  cout << GetCmdName() << " " << _Name  << _Ang_speed << _Angle << endl;
}


/*!
 *
 */
const char* Interp4Rotate::GetCmdName() const
{
  return ::GetCmdName();
}


/*!
 *
 */
bool Interp4Rotate::ExecCmd(Scene *scene) const
{
  MobileObj *obj = scene->FindMobileObj(_Name.c_str());
  double yaw = obj->GetAng_Yaw_deg();

  double time = _Angle / _Speed_mmS; 
  double steps = (int)(time * N);
  double step_distance = _Angle / steps; 
  double step= 0.0333333;            


  double yaw_ = 0;

  for (int i = 0; i < steps; ++i)
  {
    scene->LockAccess();
    
    yaw_ += step_distance;
    obj->SetAng_Yaw_deg(yaw + yaw_);
    scene->MarkChange();
    scene->UnlockAccess();
    usleep(step);
  }
  return true;
}


/*!
 *
 */
bool Interp4Rotate::ReadParams(std::istream& Strm_CmdsList)
{
  /*
   *  Tu trzeba napisać odpowiedni kod.
   */
  if (!(Strm_CmdsList >> _Name))
  {
    std::cout << "Blad wczytywania nazwy obiektu" << std::endl;
    return 1;
  }

  if (!(Strm_CmdsList >> _Speed_mmS))
  {
    std::cout << "Blad wczytywania predkosci katowej obiektu" << std::endl;
    return 1;
  }

  if (!(Strm_CmdsList >> _Angle))
  {
    std::cout << "Blad wczytywania kata" << std::endl;
    return 1;
  }
  return 0;
}


/*!
 *
 */
AbstractInterp4Command* Interp4Rotate::CreateCmd()
{
  return new Interp4Rotate();
}


/*!
 *
 */
void Interp4Rotate::PrintSyntax() const
{
  cout << "   Rotate nazwa_obiektu nazwa_osi szybko±¢_kątowa kąt_obrotu" << endl;
}
